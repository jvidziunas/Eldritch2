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
#include <iostream>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_LIBRARY("dxcompiler.lib")
//------------------------------------------------------------------//

#define ET_ABORT_UNLESS_HRESULT(operation) \
	{                                      \
		const auto _(operation);           \
		if (FAILED(_)) {                   \
			return _;                      \
		}                                  \
	}

namespace Eldritch2 {
namespace Tools {

	using namespace ::Eldritch2::Graphics::Direct3D::FlatBuffers;
	using namespace ::Eldritch2::Graphics::Vulkan::FlatBuffers;
	using namespace ::Eldritch2::Graphics::FlatBuffers;
	using namespace ::flatbuffers;

	namespace {

		ETInlineHint ETForceInlineHint ETPureFunctionHint PlatformStringView GetName(ShaderModel target) {
			switch (target) {
			case ShaderModel5_0: return L"5_0"; break;
			case ShaderModel6_0: return L"6_0"; break;
			case ShaderModel6_1: return L"6_1"; break;
			default: return L""; break;
			}; // switch(target)
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint ETPureFunctionHint PlatformStringView GetName(ShaderStage stage) {
			switch (stage) {
			case Vertex: return L"vs"; break;
			case Hull: return L"hs"; break;
			case Domain: return L"ds"; break;
			case Geometry: return L"gs"; break;
			case Pixel: return L"ps"; break;
			case Compute: return L"cs"; break;
			default: return L""; break;
			}; // switch (stage)
		}

		// ---------------------------------------------------

		class DxcInvocation {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			DxcInvocation(const DxcInvocation&) = default;
			DxcInvocation(PlatformStringView usage) :
				_usage(MallocAllocator("DXC Invocation Usage Name Allocator"), usage) {}

			~DxcInvocation() = default;

			// ---------------------------------------------------

		public:
			ETInlineHint size_t GetBytecodeSize() const ETNoexceptHint {
				return Reduce(Begin(_bytecodeByStage), End(_bytecodeByStage), 0u, [](const ComPointer<IDxcBlob>& code, size_t sum) {
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

			ETInlineHint void WriteErrors() const ETNoexceptHint {
				for (const ComPointer<IDxcBlobEncoding>& errors : _outputByStage) {
					BOOL   knownEncoding(FALSE);
					UINT32 codePage;

					if (FAILED(errors->GetEncoding(ETAddressOf(knownEncoding), ETAddressOf(codePage)))) {
						continue;
					}

					if (codePage == CP_UTF8 || codePage == CP_ACP) {
						std::cout.write(LPCSTR(errors->GetBufferPointer()), errors->GetBufferSize()).flush();
					}
				}
			}

			// ---------------------------------------------------

		public:
			HRESULT operator()(IDxcCompiler* const compiler, IDxcBlobEncoding* const hlsl, LPCWSTR path, ShaderModel target, const ArrayList<LPCWSTR>& arguments, const ArrayList<DxcDefine>& defines) ETNoexceptHint {
				for (uint32 stage(Vertex); stage < _countof(_invokeStage); ++stage) {
					if (_invokeStage[stage] == false) {
						continue;
					}

					ET_ABORT_UNLESS_HRESULT((*this)(compiler, hlsl, path, target, ShaderStage(stage), arguments, defines));
				}

				return S_OK;
			}

			HRESULT operator()(IDxcCompiler* const compiler, IDxcBlobEncoding* const hlsl, LPCWSTR path, ShaderModel target, ShaderStage stage, const ArrayList<LPCWSTR>& arguments, const ArrayList<DxcDefine>& defines) ETNoexceptHint {
				const PlatformString            entryPoint(MallocAllocator("Entry Point Name Allocator"), L"{}_{}", _usage, GetName(stage));
				const PlatformString            profile(MallocAllocator("Target Profile Allocator"), L"{}_{}", GetName(target), GetName(stage));
				ComPointer<IDxcOperationResult> result;
				HRESULT                         compilation;

				ET_ABORT_UNLESS_HRESULT(compiler->Compile(hlsl, path, entryPoint, profile, const_cast<LPCWSTR*>(arguments.GetData()), arguments.GetSize(), defines.GetData(), defines.GetSize(), nullptr, result.GetInterfacePointer()));
				ET_ABORT_UNLESS_HRESULT(result->GetStatus(ETAddressOf(compilation)));
				if (SUCCEEDED(compilation)) {
					ComPointer<IDxcBlob> code;
					ET_ABORT_UNLESS_HRESULT(result->GetResult(code.GetInterfacePointer()));
					_bytecodeByStage[stage] = eastl::move(code);
				} else {
					ComPointer<IDxcBlobEncoding> errors;
					ET_ABORT_UNLESS_HRESULT(result->GetErrorBuffer(errors.GetInterfacePointer()));
					_outputByStage[stage] = eastl::move(errors);
				}

				return S_OK;
			}

			// - DATA MEMBERS ------------------------------------

		private:
			PlatformString               _usage;
			bool                         _invokeStage[Compute + 1u];
			ComPointer<IDxcBlob>         _bytecodeByStage[Compute + 1u];
			ComPointer<IDxcBlobEncoding> _outputByStage[Compute + 1u];
		};

		// ---------------------------------------------------

		template <typename Allocator>
		ETInlineHint ETForceInlineHint size_t GetBytecodeSize(const ArrayList<DxcInvocation, Allocator>& invocations) {
			return Reduce(invocations.Begin(), invocations.End(), 0u, [](const DxcInvocation& invocation, size_t sum) {
				return invocation.GetBytecodeSize() + sum;
			});
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint ErrorCode WriteFlatbuffer(const Path& path, const FlatBufferBuilder& flatbuffer) {
			FileWriter file;
			ET_ABORT_UNLESS(file.CreateOrTruncate(path));

			return file.Append(flatbuffer.GetBufferPointer(), flatbuffer.GetSize());
		}

	} // anonymous namespace

	ShaderCompiler::ShaderCompiler() :
		_pipelinePath(MallocAllocator("Input Pipeline Path Allocator")),
		_hlslPath(MallocAllocator("Input HLSL Path Allocator")),
		_spirvOutPath(MallocAllocator("SPIR-V Output Pipeline Path Allocator")),
		_dxbcOutPath(MallocAllocator("DXBC Output Pipeline Path Allocator")),
		_shaderModel(ShaderModel5_0),
		_emitSpirV(false),
		_emitDxbc(false) {}

	// ---------------------------------------------------

	void ShaderCompiler::RegisterOptions(OptionRegistrar& options) {
		options.Register(L"--dxbc", L"-d", [this](PlatformStringView /*source*/) -> int {
			_emitDxbc = true;
			return 0;
		});

		options.Register(L"--spirv", L"-s", [this](PlatformStringView /*source*/) -> int {
			_emitSpirV = true;
			return 0;
		});

		options.RegisterInputFileHandler([this](PlatformStringView path) -> int {
			static ETConstexpr PlatformStringView SourceSuffix = L".e2shaderset.json";

			if (path.IsEmpty()) {
				return 1;
			}

			_pipelinePath.Assign(path).EnsureSuffix(SourceSuffix).ReplaceAll('\\', '/');
			_hlslPath.Assign(_pipelinePath.Begin(), _pipelinePath.FindLast(SourceSuffix)).Append(L".hlsl");
			_spirvOutPath.Assign(_pipelinePath.Begin(), _pipelinePath.FindLast(SourceSuffix)).Append(SpirVShaderSetExtension());
			_dxbcOutPath.Assign(_pipelinePath.Begin(), _pipelinePath.FindLast(SourceSuffix)).Append(DxbcShaderSetExtension());

			return 0;
		});
	}

	// ---------------------------------------------------

	int ShaderCompiler::Process() {
		ComPointer<IDxcCompiler> compiler;
		ET_ABORT_UNLESS_HRESULT(DxcCreateInstance(CLSID_DxcCompiler, __uuidof(IDxcCompiler), reinterpret_cast<void**>(compiler.GetInterfacePointer())));

		ComPointer<IDxcLibrary> library;
		ET_ABORT_UNLESS_HRESULT(DxcCreateInstance(CLSID_DxcLibrary, __uuidof(IDxcLibrary), reinterpret_cast<void**>(library.GetInterfacePointer())));

		ComPointer<IDxcBlobEncoding> hlsl;
		ET_ABORT_UNLESS_HRESULT(library->CreateBlobFromFile(_hlslPath, nullptr, hlsl.GetInterfacePointer()));

		ArrayList<DxcInvocation> invocations(MallocAllocator("Invocation List Allocator"));
		if (_emitDxbc) {
			ArrayList<DxcDefine> defines(MallocAllocator("Define List Allocator"), { { L"DIRECT3D", L"1" } });
			ArrayList<LPCWSTR>   arguments(MallocAllocator("Argument List Allocator"));

			for (DxcInvocation& dxc : invocations) {
				ET_ABORT_UNLESS_HRESULT(dxc(compiler.Get(), hlsl.Get(), _hlslPath, _shaderModel, arguments, defines));
				dxc.WriteErrors();
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

			std::cout << "Wrote " << _dxbcOutPath.AsCString() << '.' << std::endl;
		}

		if (_emitSpirV) {
			ArrayList<DxcDefine> defines(MallocAllocator("Define List Allocator"), { { L"VULKAN", L"1" } });
			ArrayList<LPCWSTR>   arguments(MallocAllocator("Argument List Allocator"), { L"-spirv" });

			for (DxcInvocation& dxc : invocations) {
				ET_ABORT_UNLESS_HRESULT(dxc(compiler.Get(), hlsl.Get(), _hlslPath, _shaderModel, arguments, defines));
				dxc.WriteErrors();
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

			std::cout << "Wrote " << _spirvOutPath.AsCString() << '.' << std::endl;
		}

		return 0;
	}

}} // namespace Eldritch2::Tools
