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

	template <typename Renderable, class Hasher, class HashAllocator>
	VisibilitySystem<Renderable, Hasher, HashAllocator>::VisibilitySystem( Hasher&& shadowCasterHasher, Hasher&& hasher, HashAllocator&& shadowCasterAllocator, HashAllocator&& allocator ) : _shadowCasterCells( ::std::forward<Hasher>( shadowCasterHasher ), ::std::forward<HashAllocator>( shadowCasterAllocator ) ),
																																															  _visibilityCells( ::std::forward<Hasher>( hasher ), ::std::forward<HashAllocator>( allocator ) ) {}

// ---------------------------------------------------

	template <typename Renderable, class Hasher, class HashAllocator>
	VisibilitySystem<Renderable, Hasher, HashAllocator>::VisibilitySystem( HashAllocator&& allocator, HashAllocator&& shadowCasterAllocator ) : _shadowCasterCells( ::std::forward<HashAllocator>( shadowCasterAllocator ) ), _visibilityCells( ::std::forward<HashAllocator>( allocator ) ) {}

}	// namespace Renderer
}	// namespace Eldritch2

