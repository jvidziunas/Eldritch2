/*==================================================================*\
  EngineService.cpp
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
#include <Renderer/Direct3D11/Builders/DeviceBuilder.hpp>
#include <Renderer/Direct3D11/ResourceCommon.hpp>
#include <Renderer/Direct3D11/EngineService.hpp>
#include <Utility/Memory/InstanceDeleters.hpp>
#include <Renderer/Direct3D11/WorldView.hpp>
#include <Scheduler/CRTPTransientTask.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Packages/ContentLibrary.hpp>
#include <Utility/ErrorCode.hpp>
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

	static ETThreadLocal Direct3D11::EngineService*	activeRenderer	= nullptr;
	static const float32							colorClear[4] = { 0.176f, 0.196f, 0.667f, 0.0f };

}	// anonymous namespace

#ifdef ERROR
#	undef ERROR
#endif

namespace Eldritch2 {
namespace Renderer {
namespace Direct3D11 {

	EngineService::EngineService( GameEngine& owningEngine ) : GameEngineService( owningEngine ),
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

	const UTF8Char* const EngineService::GetName() const {
		return UTF8L("Direct3D11 Renderer");
	}

// ---------------------------------------------------

	void EngineService::AcceptInitializationVisitor( ScriptAPIRegistrationInitializationVisitor& visitor ) {
		struct FunctionHelper {
			static SwapChain* ETScriptAPICall GetPrimarySwapChain() {
				auto&	renderer( *activeRenderer );

				if( !renderer._primarySwapChain ) {
					
				}

				renderer._primarySwapChain->AddReference();
				return renderer._primarySwapChain;
			}
		};

	// ---

		SwapChain::ExposeScriptAPI( visitor );
		WorldView::ExposeScriptAPI( visitor );

		visitor.ExposeVirtualProperty( "PrimarySwapChain", &FunctionHelper::GetPrimarySwapChain );
	}

// ---------------------------------------------------

	void EngineService::AcceptInitializationVisitor( ConfigurationPublishingInitializationVisitor& visitor ) {
		visitor.PushSection( UTF8L("Direct3D11") );

		visitor.Register( UTF8L("VSyncMode"), _VSyncMode ).Register( UTF8L("PreferredAdapterName"), _preferredAdapterName ).Register( UTF8L("ForceDebugRuntime"), _forceDebugRuntime );
		visitor.Register( UTF8L("MSAASamplesPerPixel"), _MSAACount ).Register( UTF8L("MSAAQualityLevel"), _MSAAQuality ).Register( UTF8L("MaximumFramesToRenderAhead"), _maximumFramesToRenderAhead );
		visitor.Register( UTF8L("MaximumAdaptiveResolutionScreenFraction"), _adaptiveResolutionMaxAreaFraction ).Register( UTF8L("MinimumAdaptiveResolutionScreenFraction"), _adaptiveResolutionMinAreaFraction );
	}

// ---------------------------------------------------

	void EngineService::AcceptTaskVisitor( Allocator& subtaskAllocator, Task& visitingTask, WorkerContext& executingContext, const PostConfigurationLoadedTaskVisitor ) {
		class CreateDeviceTask : public CRTPTransientTask<CreateDeviceTask> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//! Constructs this @ref CreateDeviceTask instance.
			ETInlineHint CreateDeviceTask( EngineService& host, Task& visitingTask, WorkerContext& executingContext ) : CRTPTransientTask<CreateDeviceTask>( visitingTask, Scheduler::CodependentTaskSemantics ), _host( host ) {
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
			EngineService&	_host;
		};

	// ---

		new(subtaskAllocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION) CreateDeviceTask( *this, visitingTask, executingContext );
	}

// ---------------------------------------------------

	void EngineService::AcceptInitializationVisitor( WorldViewFactoryPublishingInitializationVisitor& visitor ) {
		visitor.PublishFactory( this, sizeof( WorldView ), [] ( Allocator& allocator, World& world, void* renderer ) -> ErrorCode {
			return new(allocator, Allocator::AllocationOption::PERMANENT_ALLOCATION) WorldView( world, *static_cast<EngineService*>(renderer)->_defaultMeshView ) ? Error::NONE : Error::OUT_OF_MEMORY;
		} );
	}

// ---------------------------------------------------

	void EngineService::AcceptInitializationVisitor( ResourceViewFactoryPublishingInitializationVisitor& visitor ) {
		using AllocationOption	= Allocator::AllocationOption;
		using Initializer		= ResourceView::Initializer;

	// ---

		// Mesh view.
		visitor.PublishFactory( MeshResourceView::GetSerializedDataTag(), this, [] ( Allocator& allocator, const Initializer& initializer, void* /*renderer*/ ) -> Result<ResourceView> {
			unique_ptr<MeshResourceView, InstanceDeleter>	view( new(allocator, AllocationOption::PERMANENT_ALLOCATION) MeshResourceView( initializer, allocator ), { allocator } );

			if( view ) {
				return { *view.release() };
			}

			return { view ? Error::INVALID_PARAMETER : Error::OUT_OF_MEMORY };
		} )
		// HLSL Pipeline Definition view.
		.PublishFactory( HLSLPipelineDefinitionView::GetSerializedDataTag(), this, [] ( Allocator& allocator, const Initializer& initializer, void* renderer ) -> Result<ResourceView> {
			unique_ptr<HLSLPipelineDefinitionView, InstanceDeleter>	view( new(allocator, AllocationOption::PERMANENT_ALLOCATION) HLSLPipelineDefinitionView( initializer, allocator ), { allocator } );

			if( view && view->InstantiateFromByteArray( initializer.serializedAsset, static_cast<EngineService*>(renderer)->_device ) ) {
				return { *view.release() };
			}

			return { view ? Error::INVALID_PARAMETER : Error::OUT_OF_MEMORY };
		} )
		// Shader resource view.
		.PublishFactory( ShaderResourceResourceView::GetSerializedDataTag(), this, [] ( Allocator& allocator, const Initializer& initializer, void* renderer ) -> Result<ResourceView> {
			unique_ptr<ShaderResourceResourceView, InstanceDeleter>	view( new(allocator, AllocationOption::PERMANENT_ALLOCATION) ShaderResourceResourceView( initializer, allocator ), { allocator } );

			if( view && view->InstantiateFromByteArray( initializer.serializedAsset, static_cast<EngineService*>(renderer)->_device ) ) {
				return { *view.release() };
			}

			return { view ? Error::INVALID_PARAMETER : Error::OUT_OF_MEMORY };
		} );
	}

// ---------------------------------------------------

	void EngineService::InitializeDirect3D() {
		DeviceBuilder	deviceBuilder;
		const bool		useDebugLayer( (0 != ::IsDebuggerPresent()) | _forceDebugRuntime );

		GetLogger()( UTF8L("Creating Direct3D %sdevice.") ET_UTF8_NEWLINE_LITERAL, (useDebugLayer ? UTF8L("debug ") : UTF8L("")) );

		deviceBuilder.SetDebuggingEnabled( useDebugLayer ).SetFreeThreadedModeEnabled().SetDriverThreadingOptimizationsEnabled( _allowDriverThreadingOptimizations );
		deviceBuilder.SetDesiredAdapterName( _preferredAdapterName.GetCharacterArray() ).SetMaximumFramesToRenderAhead( _maximumFramesToRenderAhead );

		if( auto device = deviceBuilder.Build() ) {
			GetLogger()( UTF8L("Constructed Direct3D device successfully.") ET_UTF8_NEWLINE_LITERAL );

			_device	= move( device );
		} else {
			GetLogger( LogMessageType::ERROR )( UTF8L("Unable to instantiate Direct3D!.") ET_UTF8_NEWLINE_LITERAL );
		}
	}

}	// namespace Direct3D11
}	// namespace Renderer
}	// namespace Eldritch2