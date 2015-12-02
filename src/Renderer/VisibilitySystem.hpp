/*==================================================================*\
  VisibilitySystem.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/SpatialHash.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {
namespace Detail {

	class VisibilityCellBase {
	public:
		using BoundingSphereOffsetAndRadius	= ::Eldritch2::float16[4];

	// - DATA MEMBERS ------------------------------------

	protected:
		BoundingSphereOffsetAndRadius	_boundingSphereOffsetAndRadius;
	};

}	// namespace Detail

	template <typename Renderable, class Hasher = ::Eldritch2::SpatialHasher, class HashAllocator = ::Eldritch2::ChildAllocator>
	class VisibilitySystem {
	// - TYPE PUBLISHING ---------------------------------

	public:
		class VisibilityCell : public Detail::VisibilityCellBase {
			
		};

	// ---

		class ShadowCasterCell {

		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//!	Constructs this @ref VisibilitySystem instance.
		VisibilitySystem( Hasher&& shadowCasterHasher, Hasher&& hasher, HashAllocator&& shadowCasterAllocator, HashAllocator&& allocator );
		//!	Constructs this @ref VisibilitySystem instance.
		VisibilitySystem( HashAllocator&& shadowCasterAllocator, HashAllocator&& allocator );

		~VisibilitySystem() = default;

	// - DATA MEMBERS ------------------------------------

	private:
		::Eldritch2::SpatialHash<ShadowCasterCell, Hasher, HashAllocator>	_shadowCasterCells;
		::Eldritch2::SpatialHash<VisibilityCell, Hasher, HashAllocator>		_visibilityCells;
	};

}	// namespace Renderer
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Renderer/VisibilitySystem.inl>
//------------------------------------------------------------------//