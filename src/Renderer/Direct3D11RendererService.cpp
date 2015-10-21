/*==================================================================*\
  Direct3D11RendererService.cpp
  ------------------------------------------------------------------
  Purpose:
  Central context object used to centralize all simulation, effects
  processing and resource management associated with drawing.

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Packages/ResourceViewFactoryPublishingInitializationVisitor.hpp>
#include <Foundation/WorldViewFactoryPublishingInitializationVisitor.hpp>
#include <Configuration/ConfigurationPublishingInitializationVisitor.hpp>
#include <Scripting/ScriptAPIRegistrationInitializationVisitor.hpp>
#include <Renderer/D3D11/Builders/Direct3D11DeviceBuilder.hpp>
#include <Renderer/D3D11/Direct3D11ResourceCommon.hpp>
#include <Renderer/D3D11/Direct3D11WorldView.hpp>
#include <Utility/Memory/InstanceDeleters.hpp>
#include <Renderer/Direct3D11RendererService.hpp>
#include <Scheduler/CRTPTransientTask.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Packages/ContentLibrary.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//
#include <TChar.h>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_LIBRARY( "DXGUID.lib" )
ET_LINK_LIBRARY( "D3D11.lib" )
ET_LINK_LIBRARY( "DXGI.lib" )
//------------------------------------------------------------------//

using namespace ::Eldritch2::Configuration;
using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Renderer;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;
using namespace ::std;

namespace {

	static ETThreadLocal Direct3D11RendererService*	activeRenderer	= nullptr;
	static const float32							colorClear[4] = { 0.176f, 0.196f, 0.667f, 0.0f };

}	// anonymous namespace

#ifdef ERROR
#	undef ERROR
#endif

namespace Eldritch2 {
namespace Renderer {

	Direct3D11RendererService::Direct3D11RendererService( GameEngine& owningEngine ) : GameEngineService( owningEngine ),
																					   _defaultMeshName( UTF8L("_default"), GetEngineAllocator() ),
																					   _MSAACount( 1u ),
																					   _MSAAQuality( 0u ),
																					   _adaptiveResolutionMaxAreaFraction( 1.0f ),
																					   _adaptiveResolutionMinAreaFraction( 0.25f ),
									 												   _VSyncMode( 0u ),
																					   _preferredAdapterName( ::Eldritch2::EmptyStringSemantics, GetEngineAllocator() ),
																					   _forceDebugRuntime( false ),
																					   _allowDriverThreadingOptimizations( true ),
																					   _maximumFramesToRenderAhead( 3u ),
																					   _defaultMeshView( nullptr ) {}

// ---------------------------------------------------

	const UTF8Char* const Direct3D11RendererService::GetName() const {
		return UTF8L("Direct3D11 Renderer");
	}

// ---------------------------------------------------

	void Direct3D11RendererService::AcceptInitializationVisitor( ScriptAPIRegistrationInitializationVisitor& visitor ) {
		struct FunctionHelper {
			static Direct3D11SwapChain* ETScriptAPICall GetPrimarySwapChain() {
				auto&	renderer( *activeRenderer );

				if( !renderer._primarySwapChain ) {
					
				}

				renderer._primarySwapChain->AddReference();
				return renderer._primarySwapChain;
			}
		};

	// ---

		Direct3D11SwapChain::ExposeScriptAPI( visitor );
		Direct3D11WorldView::ExposeScriptAPI( visitor );

		visitor.ExposeVirtualProperty( "PrimarySwapChain", &FunctionHelper::GetPrimarySwapChain );
	}

// ---------------------------------------------------

	void Direct3D11RendererService::AcceptInitializationVisitor( ConfigurationPublishingInitializationVisitor& visitor ) {
		visitor.PushSection( UTF8L("Direct3D11") );

		visitor.Register( UTF8L("VSyncMode"), _VSyncMode ).Register( UTF8L("PreferredAdapterName"), _preferredAdapterName ).Register( UTF8L("ForceDebugRuntime"), _forceDebugRuntime );
		visitor.Register( UTF8L("MSAASamplesPerPixel"), _MSAACount ).Register( UTF8L("MSAAQualityLevel"), _MSAAQuality ).Register( UTF8L("MaximumFramesToRenderAhead"), _maximumFramesToRenderAhead );
		visitor.Register( UTF8L("MaximumAdaptiveResolutionScreenFraction"), _adaptiveResolutionMaxAreaFraction ).Register( UTF8L("MinimumAdaptiveResolutionScreenFraction"), _adaptiveResolutionMinAreaFraction );
	}

// ---------------------------------------------------

	void Direct3D11RendererService::AcceptTaskVisitor( Allocator& subtaskAllocator, Task& visitingTask, WorkerContext& executingContext, const PostConfigurationLoadedTaskVisitor ) {
		class CreateDeviceTask : public CRTPTransientTask<CreateDeviceTask> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//! Constructs this @ref CreateDeviceTask instance.
			ETInlineHint CreateDeviceTask( Direct3D11RendererService& host, Task& visitingTask, WorkerContext& executingContext ) : CRTPTransientTask<CreateDeviceTask>( visitingTask, Scheduler::CodependentTaskSemantics ), _host( host ) {
				TrySchedulingOnContext( executingContext );
			}

			// Destroys this @ref CreateDeviceTask instance.
			ETInlineHint ~CreateDeviceTask() = default;

		// ---------------------------------------------------

			const UTF8Char* const GetHumanReadableName() const override sealed {
				return UTF8L("Create Direct3D11 Device Task");
			}

			Task* Execute( WorkerContext& /*executingContext*/ ) override sealed {
				_host.InitializeDirect3D();

				return nullptr;
			}

		// - DATA MEMBERS ------------------------------------

		private:
			Direct3D11RendererService&	_host;
		};

	// ---

		new(subtaskAllocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION) CreateDeviceTask( *this, visitingTask, executingContext );
	}

// ---------------------------------------------------

	void Direct3D11RendererService::AcceptInitializationVisitor( WorldViewFactoryPublishingInitializationVisitor& visitor ) {
		visitor.PublishFactory( this, sizeof( Direct3D11WorldView ), [] ( Allocator& allocator, World& world, void* renderer ) -> ErrorCode {
			return new(allocator, Allocator::AllocationOption::PERMANENT_ALLOCATION) Direct3D11WorldView( world, *static_cast<Direct3D11RendererService*>(renderer)->_defaultMeshView ) ? Error::NONE : Error::OUT_OF_MEMORY;
		} );
	}

// ---------------------------------------------------

	void Direct3D11RendererService::AcceptInitializationVisitor( ResourceViewFactoryPublishingInitializationVisitor& visitor ) {
		using AllocationOption	= Allocator::AllocationOption;
		using Initializer		= ResourceView::Initializer;

	// ---

		// Mesh view.
		visitor.PublishFactory( Direct3D11MeshView::GetSerializedDataTag(), this, [] ( Allocator& allocator, const Initializer& initializer, void* /*renderer*/ ) -> Result<ResourceView> {
			unique_ptr<Direct3D11MeshView, InstanceDeleter>	view( new(allocator, AllocationOption::PERMANENT_ALLOCATION) Direct3D11MeshView( initializer, allocator ), { allocator } );

			if( view ) {
				return { *view.release() };
			}

			return { view ? Error::INVALID_PARAMETER : Error::OUT_OF_MEMORY };
		} )
		// HLSL Pipeline Definition view.
		.PublishFactory( Direct3D11HLSLPipelineDefinitionView::GetSerializedDataTag(), this, [] ( Allocator& allocator, const Initializer& initializer, void* renderer ) -> Result<ResourceView> {
			unique_ptr<Direct3D11HLSLPipelineDefinitionView, InstanceDeleter>	view( new(allocator, AllocationOption::PERMANENT_ALLOCATION) Direct3D11HLSLPipelineDefinitionView( initializer, allocator ), { allocator } );

			if( view && view->InstantiateFromByteArray( initializer.serializedAsset, static_cast<Direct3D11RendererService*>(renderer)->GetDevice() ) ) {
				return { *view.release() };
			}

			return { view ? Error::INVALID_PARAMETER : Error::OUT_OF_MEMORY };
		} )
		// Shader resource view.
		.PublishFactory( Direct3D11ShaderResourceView::GetSerializedDataTag(), this, [] ( Allocator& allocator, const Initializer& initializer, void* renderer ) -> Result<ResourceView> {
			unique_ptr<Direct3D11ShaderResourceView, InstanceDeleter>	view( new(allocator, AllocationOption::PERMANENT_ALLOCATION) Direct3D11ShaderResourceView( initializer, allocator ), { allocator } );

			if( view && view->InstantiateFromByteArray( initializer.serializedAsset, static_cast<Direct3D11RendererService*>(renderer)->GetDevice() ) ) {
				return { *view.release() };
			}

			return { view ? Error::INVALID_PARAMETER : Error::OUT_OF_MEMORY };
		} );
	}

// ---------------------------------------------------

	void Direct3D11RendererService::InitializeDirect3D() {
		Direct3D11DeviceBuilder	deviceBuilder;
		const bool				useDebugLayer( (0 != ::IsDebuggerPresent()) | _forceDebugRuntime );

		GetLogger()( UTF8L("Creating Direct3D %sdevice.") ET_UTF8_NEWLINE_LITERAL, (useDebugLayer ? UTF8L("debug ") : UTF8L("")) );

		deviceBuilder.SetDebuggingEnabled( useDebugLayer ).SetFreeThreadedModeEnabled().SetDriverThreadingOptimizationsEnabled( _allowDriverThreadingOptimizations );
		deviceBuilder.SetDesiredAdapterName( _preferredAdapterName.GetCharacterArray() ).SetMaximumFramesToRenderAhead( _maximumFramesToRenderAhead );

		if( const auto createDeviceResult = deviceBuilder.Build() ) {
			_immediateContext	= deviceBuilder.GetImmediateContext();
			_factory			= deviceBuilder.GetFactory();
			_device				= deviceBuilder.GetDevice();

			GetLogger()( UTF8L("Constructed Direct3D11 device successfully.") ET_UTF8_NEWLINE_LITERAL );
		} else {
			GetLogger( LogMessageType::ERROR )( UTF8L("Unable to instantiate Direct3D: %s.") ET_UTF8_NEWLINE_LITERAL, createDeviceResult.ToUTF8String() );
		}
	}

}	// namespace Renderer
}	// namespace Eldritch2