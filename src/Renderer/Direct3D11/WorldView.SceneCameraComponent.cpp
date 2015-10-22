/*==================================================================*\
  WorldView.SceneCameraComponent.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/ScriptAPIRegistrationInitializationVisitor.hpp>
#include <Renderer/Direct3D11/WorldView.hpp>
#include <Renderer/Direct3D11/SwapChain.hpp>
#include <Utility/Memory/ArenaAllocator.hpp>
#include <Scripting/ScriptMarshalTypes.hpp>
#include <Utility/Memory/InstanceNew.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Renderer;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Renderer {
namespace Direct3D11 {

	const char* const WorldView::SceneCameraComponent::scriptTypeName = "SceneCamera";

// ---------------------------------------------------

	WorldView::SceneCameraComponent::SceneCameraComponent( SwapChain& swapChain, WorldView& owningView ) : _swapChain( swapChain ) {
		owningView._attachedCameras.PushFront( *this );
	}

// ---------------------------------------------------

	ETNoAliasHint void WorldView::SceneCameraComponent::ExposeScriptAPI( ScriptAPIRegistrationInitializationVisitor& typeRegistrar ) {
		struct FunctionHelper {
			static SceneCameraComponent* ETScriptAPICall Factory0() {
				return Factory1( nullptr );
			}

		// ---------------------------------------------------

			static SceneCameraComponent* ETScriptAPICall Factory1( SwapChain* swapChain ) {
				WorldView&	worldView( GetActiveWorldView() );

				return new(worldView.GetWorldAllocator(), Allocator::AllocationOption::PERMANENT_ALLOCATION) SceneCameraComponent( *swapChain, worldView );
			}

		// ---------------------------------------------------

			static SwapChain* ETScriptAPICall GetSwapChain( const SceneCameraComponent* camera ) {
				auto&	swapChain( camera->_swapChain );

				return (swapChain->AddReference(), swapChain);
			}

		// ---------------------------------------------------

			static Float4Marshal ETScriptAPICall GetPosition( const SceneCameraComponent* camera ) {
				return camera->GetPosition();
			}

		// ---------------------------------------------------

			static void ETScriptAPICall SetPosition( SceneCameraComponent* camera, const Float4Marshal& postion ) {
				camera->SetPosition( postion );
			}

		// ---------------------------------------------------

			static OrientationMarshal ETScriptAPICall GetOrientation( const SceneCameraComponent* camera ) {
				return camera->GetOrientation();
			}
		};

	// ---

		FixedStackAllocator<16u>	temporaryAllocator( UTF8L("WorldView::SceneCameraComponent::ExposeScriptAPI() Temporary Allocator") );

		if( const auto registerResult = typeRegistrar.RegisterUserDefinedReferenceType<SceneCameraComponent>( temporaryAllocator ) ) {
			auto&	typeBuilder( *registerResult.object );

			typeRegistrar.EnsureReferenceTypeDeclared<SwapChain>();

			typeBuilder.ExposeFactory( &FunctionHelper::Factory0 ).ExposeFactory( &FunctionHelper::Factory1 );

			typeBuilder.ExposeVirtualProperty( "Position", &FunctionHelper::GetPosition ).ExposeVirtualProperty( "Position", &FunctionHelper::SetPosition );
			typeBuilder.ExposeVirtualProperty( "Orientation", &FunctionHelper::GetOrientation ).ExposeVirtualProperty( "SwapChain", &FunctionHelper::GetSwapChain );
			typeBuilder.ExposeVirtualProperty( "NearPlane", &GetNearPlane ).ExposeVirtualProperty( "NearPlane", &SetNearPlane );
			typeBuilder.ExposeVirtualProperty( "FarPlane", &GetFarPlane ).ExposeVirtualProperty( "FarPlane", &SetFarPlane );

			temporaryAllocator.Delete( typeBuilder );
		}
	}

// ---------------------------------------------------

	void WorldView::SceneCameraComponent::Dispose() {
		auto&	worldView( GetActiveWorldView() );
		auto&	attachedCameras( worldView._attachedCameras );

		attachedCameras.Erase( attachedCameras.IteratorTo( *this ) );
		worldView.GetWorldAllocator().Delete( *this );
	}

}	// namespace Direct3D11
}	// namespace Renderer
}	// namespace Eldritch2