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
#include <Utility/Containers/ResizableArray.hpp>
#include <Utility/Containers/SpatialHash.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Animation {
		class	Armature;
	}
}

namespace Eldritch2 {
namespace Renderer {
namespace Detail {

	class VisibilityCell {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref VisibilityCell instance.
		VisibilityCell() = default;

		~VisibilityCell() = default;

	// - DATA MEMBERS ------------------------------------

	protected:
		Eldritch2::float16	_boundingSphereOffset[3];
		Eldritch2::float16	_boundingSphereRadius;
	};

}	// namespace Detail

	template <class MeshAsset>
	class VisibilitySystem {
	// - TYPE PUBLISHING ---------------------------------

	public:
		class VisibilityCell : public Detail::VisibilityCell {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref VisibilityCell instance.
			VisibilityCell( Eldritch2::Allocator& allocator );
		//!	Constructs this @ref VisibilityCell instance.
			VisibilityCell( VisibilityCell&& ) = default;

			~VisibilityCell() = default;

		// ---------------------------------------------------

		public:
			void	AddElement( const Eldritch2::Pair<const MeshAsset*, const Animation::Armature*>& element );

			void	RemoveElement( const Eldritch2::Pair<const MeshAsset*, const Animation::Armature*>& element );

		// - DATA MEMBERS ------------------------------------

		private:
			Eldritch2::ResizableArray<Eldritch2::Pair<const MeshAsset*, const Animation::Armature*>>	_renderElements;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref VisibilitySystem instance.
		VisibilitySystem( Eldritch2::Allocator& allocator );

		~VisibilitySystem() = default;

	// - DATA MEMBERS ------------------------------------

	private:
		Eldritch2::SpatialHash<VisibilityCell>	_cells;
	};

}	// namespace Renderer
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Renderer/VisibilitySystem.inl>
//------------------------------------------------------------------//