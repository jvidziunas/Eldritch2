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

#define ET_FAIL_UNLESS2(operation) \
	{                              \
		const auto _(operation);   \
		if (FAILED(_)) {           \
			return _;              \
		}                          \
	}

namespace Eldritch2 {
namespace Tools {

	using namespace ::Eldritch2::Graphics::Direct3D::FlatBuffers;
	using namespace ::Eldritch2::Graphics::Vulkan::FlatBuffers;
	using namespace ::Eldritch2::Graphics::FlatBuffers;
	using namespace ::flatbuffers;

	namespace {

		ETInlineHint ETPureFunctionHint LPCWSTR GetName(ShaderModel target) {
			switch (target) {
			case ShaderModel5_0: return L"5_0"; break;
			case ShaderModel6_0: return L"6_0"; break;
			case ShaderModel6_1: return L"6_1"; break;
			default: return L""; break;
			}; // switch(target)
		}

		// ---------------------------------------------------

		ETInlineHint ETPureFunctionHint LPCWSTR GetName(ShaderStage stage) {
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

		class DxcInvocation {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			DxcInvocation(const DxcInvocation&) = default;
			DxcInvocation(LPCWSTR usage) :
				_usage(usage, MallocAllocator("DXC Invocation Usage Name Allocator")) {}

			~DxcInvocation() = default;

			// ---------------------------------------------------

		public:
			ETInlineHint size_t GetBytecodeSize() const {
				size_t size(0u);

				for (const ComPointer<IDxcBlob>& code : _bytecodeByStage) {
					size += code ? code->GetBufferSize() : 0u;
				}

				return size;
			}

			template <typename CodeElement>
			ETInlineHint CodeElement* WriteBytecode(CodeElement* out) const {
				for (const ComPointer<IDxcBlob>& code : _bytecodeByStage) {
					const size_t codeSize(code->GetBufferSize());

					CopyMemoryNonTemporal(out, code->GetBufferPointer(), codeSize);
					out += (codeSize / sizeof(CodeElement));
				}

				return out;
			}

			ETInlineHint void WriteErrors() const {
				for (const ComPointer<IDxcBlobEncoding>& errors : _outputByStage) {
					BOOL   knownEncoding(FALSE);
					UINT32 codePage;

					if (FAILED(errors->GetEncoding(&knownEncoding, &codePage))) {
						continue;
					}

					if (codePage == CP_UTF8 || codePage == CP_ACP) {
						std::cout.write(LPCSTR(errors->GetBufferPointer()), errors->GetBufferSize()).flush();
					}
				}
			}

			// ---------------------------------------------------

		public:
			HRESULT operator()(IDxcCompiler* const compiler, IDxcBlobEncoding* const hlsl, LPCWSTR path, ShaderModel target, const ArrayList<LPCWSTR>& arguments, const ArrayList<DxcDefine>& defines) {
				for (uint32 stage(Vertex); stage < _countof(_invokeStage); ++stage) {
					if (_invokeStage[stage] == false) {
						continue;
					}

					ET_FAIL_UNLESS2((*this)(compiler, hlsl, path, target, ShaderStage(stage), arguments, defines));
				}

				return S_OK;
			}

			HRESULT operator()(IDxcCompiler* const compiler, IDxcBlobEncoding* const hlsl, LPCWSTR path, ShaderModel target, ShaderStage stage, const ArrayList<LPCWSTR>& arguments, const ArrayList<DxcDefine>& defines) {
				PlatformString<MallocAllocator> entryPoint(MallocAllocator("Entry Point Name Allocator"));
				entryPoint.Format(L"{}_{}", _usage.AsCString(), GetName(stage));

				PlatformString<MallocAllocator> profile(MallocAllocator("Target Profile Allocator"));
				profile.Format(L"{}_{}", GetName(target), GetName(stage));

				ComPointer<IDxcOperationResult> result;
				ET_FAIL_UNLESS2(compiler->Compile(hlsl, path, entryPoint.AsCString(), profile.AsCString(), const_cast<LPCWSTR*>(arguments.GetData()), arguments.GetSize(), defines.GetData(), defines.GetSize(), nullptr, result.GetInterfacePointer()));

				HRESULT compilation;
				ET_FAIL_UNLESS2(result->GetStatus(eastl::addressof(compilation)));
				if (SUCCEEDED(compilation)) {
					ComPointer<IDxcBlob> code;
					ET_FAIL_UNLESS2(result->GetResult(code.GetInterfacePointer()));
					_bytecodeByStage[stage] = eastl::move(code);
				} else {
					ComPointer<IDxcBlobEncoding> errors;
					ET_FAIL_UNLESS2(result->GetErrorBuffer(errors.GetInterfacePointer()));
					_outputByStage[stage] = eastl::move(errors);
				}

				return S_OK;
			}

			// - DATA MEMBERS ------------------------------------

		private:
			PlatformString<>             _usage;
			bool                         _invokeStage[Compute + 1u];
			ComPointer<IDxcBlob>         _bytecodeByStage[Compute + 1u];
			ComPointer<IDxcBlobEncoding> _outputByStage[Compute + 1u];
		};

		// ---------------------------------------------------

		template <typename Allocator>
		ETInlineHint size_t GetBytecodeSize(const ArrayList<DxcInvocation, Allocator>& invocations) {
			size_t size(0u);

			for (const DxcInvocation& invocation : invocations) {
				size += invocation.GetBytecodeSize();
			}

			return size;
		}

		// ---------------------------------------------------

		template <typename Allocator>
		ETInlineHint ErrorCode WriteFlatbuffer(const PlatformString<Allocator>& path, const FlatBufferBuilder& flatbuffer) {
			FileWriter file;
			ET_FAIL_UNLESS(file.CreateOrTruncate(path));

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
		static const PlatformChar SourceSuffix[] = L".e2shaderset.json";
		static const PlatformChar HlslSuffix[]   = L".hlsl";

		options.Register("--dxbc", "-d", [this](Range<const Utf8Char*> /*source*/) -> int {
			_emitDxbc = true;
			return 0;
		});

		options.Register("--spirv", "-s", [this](Range<const Utf8Char*> /*source*/) -> int {
			_emitSpirV = true;
			return 0;
		});

		options.RegisterInputFileHandler([this](Range<const Utf8Char*> source) -> int {
			if (source.IsEmpty()) {
				return 1;
			}

			_pipelinePath.Assign(source.Begin(), source.End()).EnsureSuffix(SourceSuffix).Replace('\\', '/');
			_hlslPath.Assign(_pipelinePath.Begin(), _pipelinePath.FindLastInstance(SourceSuffix)).Append(HlslSuffix);
			_spirvOutPath.Assign(_pipelinePath.Begin(), _pipelinePath.FindLastInstance(SourceSuffix)).Append(SpirVShaderSetExtension());
			_dxbcOutPath.Assign(_pipelinePath.Begin(), _pipelinePath.FindLastInstance(SourceSuffix)).Append(DxbcShaderSetExtension());

			return 0;
		});
	}

	// ---------------------------------------------------

	int ShaderCompiler::Process() {
		ComPointer<IDxcCompiler> compiler;
		ET_FAIL_UNLESS2(DxcCreateInstance(CLSID_DxcCompiler, __uuidof(IDxcCompiler), reinterpret_cast<void**>(compiler.GetInterfacePointer())));

		ComPointer<IDxcLibrary> library;
		ET_FAIL_UNLESS2(DxcCreateInstance(CLSID_DxcLibrary, __uuidof(IDxcLibrary), reinterpret_cast<void**>(library.GetInterfacePointer())));

		ComPointer<IDxcBlobEncoding> hlsl;
		ET_FAIL_UNLESS2(library->CreateBlobFromFile(_hlslPath, nullptr, hlsl.GetInterfacePointer()));

		ArrayList<DxcInvocation> invocations(MallocAllocator("DXC Invocation List Allocator"));

		if (_emitDxbc) {
			ArrayList<LPCWSTR>   arguments(MallocAllocator("DXC Argument List Allocator"));
			ArrayList<DxcDefine> defines(MallocAllocator("Define List Allocator"));

			defines.Append({ L"DIRECT3D", L"1" });
			for (DxcInvocation& dxc : invocations) {
				ET_FAIL_UNLESS2(dxc(compiler.Get(), hlsl.Get(), _hlslPath, _shaderModel, arguments, defines));
				dxc.WriteErrors(std::cout);
			}

			FlatBufferBuilder       flatbuffer;
			uint8_t*                bytecode;
			Offset<Vector<uint8_t>> bytecodeOffset(flatbuffer.CreateUninitializedVector<uint8_t>(GetBytecodeSize(invocations), &bytecode));
			for (const DxcInvocation& invocation : invocations) {
				bytecode = invocation.WriteBytecode(bytecode);
			}

			Offset<UsageDescriptor>*                usages;
			Offset<Vector<Offset<UsageDescriptor>>> usagesOffset(flatbuffer.CreateUninitializedVector<Offset<UsageDescriptor>>(0u, &usages));

			flatbuffer.Finish(CreateDxbcShaderSet(flatbuffer, usagesOffset, bytecodeOffset), DxbcShaderSetIdentifier());

			WriteFlatbuffer(_dxbcOutPath, flatbuffer);

			std::cout << "Wrote " << _dxbcOutPath.AsCString() << '.' << std::endl;
		}

		if (_emitSpirV) {
			ArrayList<LPCWSTR>   arguments(MallocAllocator("DXC Argument List Allocator"));
			ArrayList<DxcDefine> defines(MallocAllocator("Define List Allocator"));

			arguments.Append(L"-spirv");
			defines.Append({ L"VULKAN", L"1" });
			for (DxcInvocation& dxc : invocations) {
				ET_FAIL_UNLESS2(dxc(compiler.Get(), hlsl.Get(), _hlslPath, _shaderModel, arguments, defines));
				dxc.WriteErrors(std::cout);
			}

			FlatBufferBuilder        flatbuffer;
			uint32_t*                bytecode;
			Offset<Vector<uint32_t>> bytecodeOffset(flatbuffer.CreateUninitializedVector<uint32_t>(GetBytecodeSize(invocations), &bytecode));
			for (const DxcInvocation& invocation : invocations) {
				bytecode = invocation.WriteBytecode(bytecode);
			}

			Offset<UsageDescriptor>*                usages;
			Offset<Vector<Offset<UsageDescriptor>>> usagesOffset(flatbuffer.CreateUninitializedVector<Offset<UsageDescriptor>>(0u, &usages));

			flatbuffer.Finish(CreateSpirVShaderSet(flatbuffer, usagesOffset, bytecodeOffset), SpirVShaderSetIdentifier());

			WriteFlatbuffer(_spirvOutPath, flatbuffer);

			std::cout << "Wrote " << _spirvOutPath.AsCString() << '.' << std::endl;
		}

		return 0;
	}

}} // namespace Eldritch2::Tools
