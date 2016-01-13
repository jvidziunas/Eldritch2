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
#define MICROPROFILE_GPU_TIMERS_D3D11 1
#include <microprofile/microprofile.h>
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
									 						   _VSyncMode( 0u ),
															   _preferredAdapterName( GetEngineAllocator() ),
															   _forceDebugRuntime( false ),
															   _allowDriverThreadingOptimizations( true ),
															   _maximumFramesToRenderAhead( 3u ),
															   _defaultMeshView( nullptr ) {}

// ---------------------------------------------------

	const UTF8Char* const EngineService::GetName() const {
		return UTF8L("Direct3D11 Renderer");
	}

// ---------------------------------------------------

	ErrorCode EngineService::AllocateWorldView( Allocator& allocator, World& world ) {
		return new(allocator, Allocator::AllocationOption::PERMANENT_ALLOCATION) WorldView( world, *_defaultMeshView ) ? Error::NONE : Error::OUT_OF_MEMORY;
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

		visitor.Register( UTF8L("VSyncMode"), _VSyncMode ).Register( UTF8L("PreferredAdapterName"), _preferredAdapterName )
			   .Register( UTF8L("ForceDebugRuntime"), _forceDebugRuntime ).Register( UTF8L("MaximumFramesToRenderAhead"), _maximumFramesToRenderAhead );

		_shaderResourceViewFactory.AcceptInitializationVisitor( visitor );
		_meshResourceViewFactory.AcceptInitializationVisitor( visitor );
		_pipelineViewFactory.AcceptInitializationVisitor( visitor );
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

			~CreateDeviceTask() = default;

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

	void EngineService::AcceptInitializationVisitor( ResourceViewFactoryPublishingInitializationVisitor& visitor ) {
		visitor.PublishFactory( ShaderResourceResourceView::GetSerializedDataTag(), _shaderResourceViewFactory )
			   .PublishFactory( MeshResourceView::GetSerializedDataTag(), _meshResourceViewFactory )
			   .PublishFactory( HLSLPipelineDefinitionView::GetSerializedDataTag(), _pipelineViewFactory );
	}

// ---------------------------------------------------

	void EngineService::InitializeDirect3D() {
		DeviceBuilder	deviceBuilder;
		const bool		useDebugLayer( (0 != ::IsDebuggerPresent()) | _forceDebugRuntime );

		GetLogger()( UTF8L("Creating Direct3D %sdevice.") ET_UTF8_NEWLINE_LITERAL, (useDebugLayer ? UTF8L("debug ") : UTF8L("")) );

		deviceBuilder.SetDebuggingEnabled( useDebugLayer ).SetFreeThreadedModeEnabled().SetDriverThreadingOptimizationsEnabled( _allowDriverThreadingOptimizations );
		deviceBuilder.SetDesiredAdapterName( _preferredAdapterName.GetCharacterArray() ).SetMaximumFramesToRenderAhead( _maximumFramesToRenderAhead );

		if( const auto device = deviceBuilder.Build() ) {
			GetLogger()( UTF8L("Constructed Direct3D device successfully.") ET_UTF8_NEWLINE_LITERAL );

			device->GetImmediateContext( _immediateContext.GetInterfacePointer() );

			::MicroProfileGpuInitD3D11( device.GetUnadornedPointer(), _immediateContext.GetUnadornedPointer() );

			_shaderResourceViewFactory.SetDevice( device );
			_meshResourceViewFactory.SetDevice( device );
			_pipelineViewFactory.SetDevice( device );
		} else {
			GetLogger( LogMessageType::ERROR )( UTF8L("Unable to instantiate Direct3D!.") ET_UTF8_NEWLINE_LITERAL );
		}
	}

}	// namespace Direct3D11
}	// namespace Renderer
}	// namespace Eldritch2