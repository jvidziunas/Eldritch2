/*==================================================================*\
  WorldView.MeshRenderer.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/ScriptApiRegistrationInitializationVisitor.hpp>
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

	const char* const WorldView::MeshRenderer::scriptTypeName = "MeshRenderer";

// ---------------------------------------------------

	WorldView::MeshRenderer::MeshRenderer( Armature& armature, const MeshResourceView& mesh ) : _mesh( mesh ), _armature( armature ) {}

// ---------------------------------------------------

	ETNoAliasHint void WorldView::MeshRenderer::ExposeScriptAPI( ScriptApiRegistrationInitializationVisitor& typeRegistrar ) {
		struct FunctionHelper {
			static MeshRenderer* ETScriptAPICall Factory0( Armature& armature, const StringMarshal& meshName ) {
				const auto	resource( GetActiveWorldView().GetContentLibrary().ResolveViewByName<MeshResourceView>( meshName.GetCharacterArray() ) );

				if( !resource ) {
					return nullptr;
				}

				return new(GetActiveWorldView()._meshAllocator, Allocator::AllocationDuration::Normal) MeshRenderer( armature, *resource );
			}

		// ---------------------------------------------------

			static bool ETScriptAPICall GetVisible( const MeshRenderer* thisPointer ) {
				return thisPointer->_visible != 0u;
			}

		// ---------------------------------------------------

			static void ETScriptAPICall SetVisible( MeshRenderer* thisPointer, bool value ) {
				thisPointer->_visible = value;
			}

		// ---------------------------------------------------

			static Armature* ETScriptAPICall GetArmature( const MeshRenderer* thisPointer ) {
				auto&	armature( thisPointer->_armature );

				return (armature->AddReference(), armature);
			}
		};

	// ---

		typeRegistrar.EnsureValueTypeDeclared<StringMarshal>().EnsureReferenceTypeDeclared<Armature>();

		FixedStackAllocator<16u>	temporaryAllocator( UTF8L("WorldView::MeshRenderer::ExposeScriptAPI() Temporary Allocator") );
		const auto					typeBuilderResult( typeRegistrar.RegisterUserDefinedReferenceType<MeshRenderer>( temporaryAllocator ) );
		auto&						typeBuilder( *typeBuilderResult.object );

		ETRuntimeAssert( typeBuilderResult );

		typeBuilder.ExposeFactory( &FunctionHelper::Factory0 );

		typeBuilder.ExposeVirtualProperty( "Visible", &FunctionHelper::GetVisible ).ExposeVirtualProperty( "Visible", &FunctionHelper::SetVisible );
		typeBuilder.ExposeVirtualProperty( "Armature", &FunctionHelper::GetArmature );

		temporaryAllocator.Delete( typeBuilder );
	}

// ---------------------------------------------------

	void WorldView::MeshRenderer::Dispose() {
		GetActiveWorldView()._meshAllocator.Delete( *this );
	}

}	// namespace Direct3D11
}	// namespace Renderer
}	// namespace Eldritch2