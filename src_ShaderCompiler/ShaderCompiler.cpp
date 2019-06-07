/*==================================================================*\
  ShaderCompiler.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Containers/ArrayList.hpp>
#include <Common/ComPointer.hpp>
#include <Common/FileWriter.hpp>
#include <ShaderCompiler.hpp>
//------------------------------------------------------------------//
#include <Flatbuffers/SpirVShaderSet_generated.h>
#include <Flatbuffers/DxbcShaderSet_generated.h>
//------------------------------------------------------------------//
#include <Windows.h>
#include <dxc/dxcapi.h>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_LIBRARY("dxcompiler.lib")
//------------------------------------------------------------------//

#define ET_ABORT_UNLESS_HRESULT(operation)                                                                                             \
	{                                                                                                                                  \
		const ::Eldritch2::Result FUNC_RESULT(SUCCEEDED(operation) ? ::Eldritch2::Result::Success : ::Eldritch2::Result::Unspecified); \
		if (Failed(FUNC_RESULT)) {                                                                                                     \
			return FUNC_RESULT;                                                                                                        \
		}                                                                                                                              \
	}

namespace Eldritch2 {
namespace Tools {

	using namespace ::Eldritch2::Graphics::Direct3D::FlatBuffers;
	using namespace ::Eldritch2::Graphics::Vulkan::FlatBuffers;
	using namespace ::Eldritch2::Graphics::FlatBuffers;
	using namespace ::Eldritch2::Logging;
	using namespace ::flatbuffers;

	// ---------------------------------------------------

	namespace {

		ETForceInlineHint ETPureFunctionHint PlatformStringSpan GetName(ShaderModel target) ETNoexceptHint {
			switch (target) {
			case ShaderModel::_5_0: return SL("5_0"); break;
			case ShaderModel::_6_0: return SL("6_0"); break;
			case ShaderModel::_6_1:
				return SL("6_1");
				break;
				ET_NO_DEFAULT_CASE;
			}; // switch(target)
		}

		// ---------------------------------------------------

		ETForceInlineHint ETPureFunctionHint PlatformStringSpan GetName(ShaderStage stage) ETNoexceptHint {
			switch (stage) {
			case ShaderStage::Vertex: return SL("vs"); break;
			case ShaderStage::Hull: return SL("hs"); break;
			case ShaderStage::Domain: return SL("ds"); break;
			case ShaderStage::Geometry: return SL("gs"); break;
			case ShaderStage::Pixel: return SL("ps"); break;
			case ShaderStage::Compute:
				return SL("cs");
				break;
				ET_NO_DEFAULT_CASE;
			}; // switch (stage)
		}

		// ---------------------------------------------------

		class DxcInvocation {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref DxcInvocation instance.
			DxcInvocation(PlatformStringSpan usage) ETNoexceptHint : _usage(MallocAllocator("DXC Invocation Usage Name Allocator"), usage) {}
			//!	Constructs this @ref DxcInvocation instance.
			DxcInvocation(const DxcInvocation&) = default;

			~DxcInvocation() = default;

			// ---------------------------------------------------

		public:
			ETInlineHint size_t GetBytecodeSize() const ETNoexceptHint {
				return Reduce(Begin(_bytecodeByStage), End(_bytecodeByStage), 0u, [](const ComPointer<IDxcBlob>& code, size_t sum) ETNoexceptHint {
					return sum + (code ? code->GetBufferSize() : 0u);
				});
			}

			// ---------------------------------------------------

			template <typename CodeElement>
			ETInlineHint CodeElement* WriteBytecode(CodeElement* out) const ETNoexceptHint {
				for (const ComPointer<IDxcBlob>& code : _bytecodeByStage) {
					const size_t size(code->GetBufferSize());

					CopyMemoryNonTemporal(out, code->GetBufferPointer(), size);
					out += (size / sizeof(CodeElement));
				}

				return out;
			}

			// ---------------------------------------------------

			ETInlineHint void WriteErrors(Log& log) const ETNoexceptHint {
				static const auto IsPrintable([](UINT32 codePage) ETNoexceptHint { return codePage == CP_UTF8 || codePage == CP_ACP; });

				for (const ComPointer<IDxcBlobEncoding>& errors : _outputByStage) {
					BOOL   knownEncoding(FALSE);
					UINT32 codePage;

					if (SUCCEEDED(errors->GetEncoding(ETAddressOf(knownEncoding), ETAddressOf(codePage))) && IsPrintable(codePage)) {
						log.Write(Severity::Error, StringSpan(LPCSTR(errors->GetBufferPointer()), errors->GetBufferSize()));
					}
				}
			}

			// ---------------------------------------------------

		public:
			ETInlineHint Result operator()(IDxcCompiler* const compiler, IDxcBlobEncoding* const hlsl, LPCWSTR path, ShaderModel target, const ArrayList<LPCWSTR>& arguments, const ArrayList<DxcDefine>& defines) ETNoexceptHint {
				for (uint32 stage(uint32(ShaderStage::Vertex)); stage < uint32(ShaderStage::MAX_STAGE); ++stage) {
					if (_invokeStage[stage] == false) {
						continue;
					}

					ET_ABORT_UNLESS((*this)(compiler, hlsl, path, target, ShaderStage(stage), arguments, defines));
				}

				return Result::Success;
			}

			ETInlineHint Result operator()(IDxcCompiler* const compiler, IDxcBlobEncoding* const hlsl, LPCWSTR path, ShaderModel target, ShaderStage stage, const ArrayList<LPCWSTR>& arguments, const ArrayList<DxcDefine>& defines) ETNoexceptHint {
				const PlatformString            entryPoint(MallocAllocator("Entry Point Name Allocator"), SL("{}_{}"), _usage, GetName(stage));
				const PlatformString            profile(MallocAllocator("Target Profile Allocator"), SL("{}_{}"), GetName(target), GetName(stage));
				ComPointer<IDxcOperationResult> result;
				HRESULT                         status;

				ET_ABORT_UNLESS_HRESULT(compiler->Compile(hlsl, path, entryPoint, profile, const_cast<LPCWSTR*>(arguments.GetData()), arguments.GetSize(), defines.GetData(), defines.GetSize(), nullptr, result.GetInterfacePointer()));
				ET_ABORT_UNLESS_HRESULT(result->GetStatus(ETAddressOf(status)));
				if (SUCCEEDED(status)) {
					ComPointer<IDxcBlob> code;
					ET_ABORT_UNLESS_HRESULT(result->GetResult(code.GetInterfacePointer()));
					_bytecodeByStage[uint32(stage)] = Move(code);
				} else {
					ComPointer<IDxcBlobEncoding> errors;
					ET_ABORT_UNLESS_HRESULT(result->GetErrorBuffer(errors.GetInterfacePointer()));
					_outputByStage[uint32(stage)] = Move(errors);
				}

				return Result::Success;
			}

			// - DATA MEMBERS ------------------------------------

		private:
			PlatformString               _usage;
			bool                         _invokeStage[uint32(ShaderStage::MAX_STAGE)];
			ComPointer<IDxcBlob>         _bytecodeByStage[uint32(ShaderStage::MAX_STAGE)];
			ComPointer<IDxcBlobEncoding> _outputByStage[uint32(ShaderStage::MAX_STAGE)];
		};

		// ---------------------------------------------------

		template <typename Allocator>
		ETForceInlineHint size_t GetBytecodeSize(const ArrayList<DxcInvocation, Allocator>& invocations) ETNoexceptHint {
			return Reduce(invocations.Begin(), invocations.End(), 0u, [](const DxcInvocation& invocation, size_t sum) ETNoexceptHint {
				return invocation.GetBytecodeSize() + sum;
			});
		}

		// ---------------------------------------------------

		ETForceInlineHint Result WriteFlatbuffer(const Path& path, const FlatBufferBuilder& flatbuffer) {
			FileWriter file;
			ET_ABORT_UNLESS(file.CreateOrTruncate(path));

			return file.Append(flatbuffer.GetBufferPointer(), flatbuffer.GetSize());
		}

	} // anonymous namespace

	ShaderCompiler::ShaderCompiler() ETNoexceptHint : _pipelinePath(MallocAllocator("Input Pipeline Path Allocator")),
													  _hlslPath(MallocAllocator("Input HLSL Path Allocator")),
													  _spirvOutPath(MallocAllocator("SPIR-V Output Pipeline Path Allocator")),
													  _dxbcOutPath(MallocAllocator("DXBC Output Pipeline Path Allocator")),
													  _shaderModel(ShaderModel::_5_0),
													  _emitSpirV(true),
													  _emitDxbc(true) {}

	// ---------------------------------------------------

	void ShaderCompiler::RegisterOptions(OptionRegistrar& options) {
		options.Register(SL("--emitSpirv"), SL("-s"), OptionRegistrar::MakePodSetter(_emitSpirV));
		options.Register(SL("--emitDxbc"), SL("-d"), OptionRegistrar::MakePodSetter(_emitDxbc));
		options.Register(SL("--shaderModel"), SL("-m"), [this](Log& log, PlatformStringSpan source) ETNoexceptHint -> Result {
			if (source == SL("5_0")) {
				_shaderModel = ShaderModel::_5_0;
			} else if (source == SL("6_0")) {
				_shaderModel = ShaderModel::_6_0;
			} else if (source == SL("6_1")) {
				_shaderModel = ShaderModel::_6_1;
			} else {
				log.Write(Severity::Error, "unknown shader model {}" ET_NEWLINE, source);
				return Result::InvalidParameter;
			}

			return Result::Success;
		});
		options.RegisterInputFileHandler([this](Logging::Log& /*log*/, PlatformStringSpan path) -> Result {
			static ETConstexpr PlatformStringSpan SourceSuffix = SL(".e2shaderset.json");

			Path pipelinePath(_pipelinePath.GetAllocator(), KnownDirectory::Relative, path);
			// Canonicalize path separators.
			pipelinePath.ReplaceAll(SL('\\'), SL('/'));
			pipelinePath.EnsureSuffix(SourceSuffix);

			const PlatformStringSpan nameRoot(pipelinePath.Slice(0, pipelinePath.FindLast(SourceSuffix)));
			Path                     hlslPath(_hlslPath.GetAllocator(), KnownDirectory::Relative, SL("{}.hlsl"), nameRoot);
			Path                     spirVOutPath(_spirvOutPath.GetAllocator(), KnownDirectory::Relative, SL("{}.spirv.e2shaderset"), nameRoot);
			Path                     dxbcOutPath(_dxbcOutPath.GetAllocator(), KnownDirectory::Relative, SL("{}.dxbc.e2shaderset"), nameRoot);

			Swap(_pipelinePath, pipelinePath);
			Swap(_hlslPath, hlslPath);
			Swap(_spirvOutPath, spirVOutPath);
			Swap(_dxbcOutPath, dxbcOutPath);

			return Result::Success;
		});
	}

	// ---------------------------------------------------

	Result ShaderCompiler::Process(Log& log) {
		ComPointer<IDxcCompiler> compiler;
		ET_ABORT_UNLESS_HRESULT(DxcCreateInstance(CLSID_DxcCompiler, __uuidof(IDxcCompiler), reinterpret_cast<void**>(compiler.GetInterfacePointer())));

		ComPointer<IDxcLibrary> library;
		ET_ABORT_UNLESS_HRESULT(DxcCreateInstance(CLSID_DxcLibrary, __uuidof(IDxcLibrary), reinterpret_cast<void**>(library.GetInterfacePointer())));

		ComPointer<IDxcBlobEncoding> hlsl;
		ET_ABORT_UNLESS_HRESULT(library->CreateBlobFromFile(_hlslPath, nullptr, hlsl.GetInterfacePointer()));

		ArrayList<DxcInvocation> invocations(MallocAllocator("Invocation List Allocator"));
		if (_emitDxbc) {
			ArrayList<DxcDefine> defines(MallocAllocator("Define List Allocator"), { { SL("DIRECT3D"), SL("1") } });
			ArrayList<LPCWSTR>   arguments(MallocAllocator("Argument List Allocator"));

			for (ArrayList<DxcInvocation>::Reference dxc : invocations) {
				ET_ABORT_UNLESS(dxc(compiler.Get(), hlsl.Get(), _hlslPath, _shaderModel, arguments, defines));
				dxc.WriteErrors(log);
			}

			FlatBufferBuilder buffer;
			uint8_t*          bytecode;
			const auto        bytecodeOffset(buffer.CreateUninitializedVector(GetBytecodeSize(invocations) / sizeof(*bytecode), ETAddressOf(bytecode)));
			for (const DxcInvocation& invocation : invocations) {
				bytecode = invocation.WriteBytecode(bytecode);
			}

			Offset<UsageDescriptor>* usages;
			const auto               usagesOffset(buffer.CreateUninitializedVector(0u, ETAddressOf(usages)));

			buffer.Finish(CreateDxbcShaderSet(buffer, usagesOffset, bytecodeOffset), DxbcShaderSetIdentifier());
			WriteFlatbuffer(_dxbcOutPath, buffer);

			log.Write(Severity::Message, "wrote {}" ET_NEWLINE, _dxbcOutPath);
		}

		if (_emitSpirV) {
			ArrayList<DxcDefine> defines(MallocAllocator("Define List Allocator"), { { SL("VULKAN"), SL("1") } });
			ArrayList<LPCWSTR>   arguments(MallocAllocator("Argument List Allocator"), { SL("-spirv") });

			for (DxcInvocation& dxc : invocations) {
				ET_ABORT_UNLESS(dxc(compiler.Get(), hlsl.Get(), _hlslPath, _shaderModel, arguments, defines));
				dxc.WriteErrors(log);
			}

			FlatBufferBuilder buffer;
			uint32_t*         bytecode;
			const auto        bytecodeOffset(buffer.CreateUninitializedVector(GetBytecodeSize(invocations) / sizeof(*bytecode), ETAddressOf(bytecode)));
			for (const DxcInvocation& invocation : invocations) {
				bytecode = invocation.WriteBytecode(bytecode);
			}

			Offset<UsageDescriptor>* usages;
			const auto               usagesOffset(buffer.CreateUninitializedVector(0u, ETAddressOf(usages)));

			buffer.Finish(CreateSpirVShaderSet(buffer, usagesOffset, bytecodeOffset), SpirVShaderSetIdentifier());
			WriteFlatbuffer(_spirvOutPath, buffer);

			log.Write(Severity::Message, "wrote {}" ET_NEWLINE, _spirvOutPath);
		}

		return Result::Success;
	}

}} // namespace Eldritch2::Tools
