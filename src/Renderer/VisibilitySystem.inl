/*==================================================================*\
  VisibilitySystem.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {

	template <class MeshAsset>
	ETInlineHint void VisibilitySystem<MeshAsset>::VisibilityCell::AddElement( const Eldritch2::Pair<const MeshAsset*, const Animation::Armature*>& element ) {
		_renderElements.PushBack( element );
	}

// ---------------------------------------------------

	template <class MeshAsset>
	ETInlineHint void VisibilitySystem<MeshAsset>::VisibilityCell::RemoveElement( const Eldritch2::Pair<const MeshAsset*, const Animation::Armature*>& element ) {
		_renderElements.Erase( _renderElements.Find( element ), Eldritch2::UnorderedSemantics );
	}

// ---------------------------------------------------

	template <class MeshAsset>
	ETInlineHint VisibilitySystem<MeshAsset>::VisibilitySystem( Eldritch2::Allocator& allocator ) : _cells( { allocator, "Visibility System Cell Allocator" } ) {}

}	// namespace Renderer
}	// namespace Eldritch2

