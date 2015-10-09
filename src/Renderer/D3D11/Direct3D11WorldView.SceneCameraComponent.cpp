/*==================================================================*\
  Direct3D11WorldView.SceneCameraComponent.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/ScriptAPIRegistrationInitializationVisitor.hpp>
#include <Renderer/D3D11/Direct3D11WorldView.hpp>
#include <Renderer/D3D11/Direct3D11SwapChain.hpp>
#include <Utility/Memory/ArenaAllocator.hpp>
#include <Scripting/ScriptMarshalTypes.hpp>
#include <Utility/Memory/InstanceNew.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Renderer;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Renderer {

	const char* const Direct3D11WorldView::SceneCameraComponent::scriptTypeName = "SceneCamera";

// ---------------------------------------------------

	Direct3D11WorldView::SceneCameraComponent::SceneCameraComponent( Direct3D11SwapChain& swapChain, Direct3D11WorldView& owningView ) : _swapChain( swapChain ) {
		owningView._attachedCameras.PushFront( *this );
	}

// ---------------------------------------------------

	Direct3D11WorldView::SceneCameraComponent::~SceneCameraComponent() {}

// ---------------------------------------------------

	ETNoAliasHint void Direct3D11WorldView::SceneCameraComponent::ExposeScriptAPI( ScriptAPIRegistrationInitializationVisitor& typeRegistrar ) {
		struct FunctionHelper {
			static SceneCameraComponent* ETScriptAPICall Factory0() {
				return Factory1( nullptr );
			}

		// ---------------------------------------------------

			static SceneCameraComponent* ETScriptAPICall Factory1( Direct3D11SwapChain* swapChain ) {
				Direct3D11WorldView&	worldView( GetActiveWorldView() );

				return new(worldView.GetWorldAllocator(), Allocator::AllocationOption::PERMANENT_ALLOCATION) SceneCameraComponent( *swapChain, worldView );
			}

		// ---------------------------------------------------

			static Direct3D11SwapChain* ETScriptAPICall GetSwapChain( const SceneCameraComponent* camera ) {
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

		FixedStackAllocator<16u>	temporaryAllocator( UTF8L("Direct3D11WorldView::SceneCameraComponent::ExposeScriptAPI() Temporary Allocator") );

		if( const auto registerResult = typeRegistrar.RegisterUserDefinedReferenceType<SceneCameraComponent>( temporaryAllocator ) ) {
			auto&	typeBuilder( *registerResult.object );

			typeRegistrar.EnsureReferenceTypeDeclared<Direct3D11SwapChain>();

			typeBuilder.ExposeFactory( &FunctionHelper::Factory0 ).ExposeFactory( &FunctionHelper::Factory1 );

			typeBuilder.ExposeVirtualProperty( "Position", &FunctionHelper::GetPosition ).ExposeVirtualProperty( "Position", &FunctionHelper::SetPosition );
			typeBuilder.ExposeVirtualProperty( "Orientation", &FunctionHelper::GetOrientation ).ExposeVirtualProperty( "SwapChain", &FunctionHelper::GetSwapChain );
			typeBuilder.ExposeVirtualProperty( "NearPlane", &SceneCameraComponent::GetNearPlane ).ExposeVirtualProperty( "NearPlane", &SceneCameraComponent::SetNearPlane );
			typeBuilder.ExposeVirtualProperty( "FarPlane", &SceneCameraComponent::GetFarPlane ).ExposeVirtualProperty( "FarPlane", &SceneCameraComponent::SetFarPlane );

			temporaryAllocator.Delete( typeBuilder );
		}
	}

// ---------------------------------------------------

	void Direct3D11WorldView::SceneCameraComponent::Dispose() {
		auto&	worldView( GetActiveWorldView() );
		auto&	attachedCameras( worldView._attachedCameras );

		attachedCameras.Erase( attachedCameras.IteratorTo( *this ) );
		worldView.GetWorldAllocator().Delete( *this );
	}

}	// namespace Renderer
}	// namespace Eldritch2