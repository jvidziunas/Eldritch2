/*==================================================================*\
  WorldView.MeshComponent.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/ScriptAPIRegistrationInitializationVisitor.hpp>
#include <Renderer/Direct3D11/MeshResourceView.hpp>
#include <Renderer/Direct3D11/WorldView.hpp>
#include <Utility/Memory/ArenaAllocator.hpp>
#include <Scripting/ScriptMarshalTypes.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Packages/ContentLibrary.hpp>
#include <Animation/Armature.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Animation;
using namespace ::Eldritch2::Renderer;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Renderer {
namespace Direct3D11 {

	const char* const WorldView::MeshComponent::scriptTypeName = "Mesh";

// ---------------------------------------------------

	WorldView::MeshComponent::MeshComponent( const MeshResourceView& mesh, Armature& armature ) : _mesh( mesh ), _armature( armature ) {}

// ---------------------------------------------------

	ETNoAliasHint void WorldView::MeshComponent::ExposeScriptAPI( ScriptAPIRegistrationInitializationVisitor& typeRegistrar ) {
		struct FunctionHelper {
			static MeshComponent* ETScriptAPICall Factory0( const StringMarshal& meshName, Armature* armature ) {
				auto&		worldView( GetActiveWorldView() );
				const auto&	contentLibrary( worldView.GetEngineContentLibrary() );

				return new(worldView._meshPool, Allocator::AllocationOption::PERMANENT_ALLOCATION) MeshComponent( contentLibrary.ResolveViewByName( meshName.GetCharacterArray(), worldView._defaultMesh ), *armature );
			}

		// ---------------------------------------------------

			static bool ETScriptAPICall GetVisible( const MeshComponent* thisPointer ) {
				return thisPointer->_visible != 0u;
			}

		// ---------------------------------------------------

			static void ETScriptAPICall SetVisible( MeshComponent* thisPointer, bool value ) {
				thisPointer->_visible = value;
			}

		// ---------------------------------------------------

			static Armature* ETScriptAPICall GetArmature( const MeshComponent* thisPointer ) {
				auto&	armature( thisPointer->_armature );

				return (armature->AddReference(), armature);
			}
		};

	// ---

		FixedStackAllocator<16u>	temporaryAllocator( UTF8L("WorldView::MeshComponent::ExposeScriptAPI() Temporary Allocator") );

		if( const auto registerResult = typeRegistrar.RegisterUserDefinedReferenceType<MeshComponent>( temporaryAllocator ) ) {
			auto&	typeBuilder( *registerResult.object );

			typeRegistrar.EnsureValueTypeDeclared<StringMarshal>().EnsureReferenceTypeDeclared<Armature>();

			typeBuilder.ExposeFactory( &FunctionHelper::Factory0 );

			typeBuilder.ExposeVirtualProperty( "Visible", &FunctionHelper::GetVisible ).ExposeVirtualProperty( "Visible", &FunctionHelper::SetVisible );
			typeBuilder.ExposeVirtualProperty( "Armature", &FunctionHelper::GetArmature );

			temporaryAllocator.Delete( typeBuilder );
		}
	}

// ---------------------------------------------------

	void WorldView::MeshComponent::Dispose() {
		GetActiveWorldView()._meshPool.Delete( *this );
	}

}	// namespace Direct3D11
}	// namespace Renderer
}	// namespace Eldritch2