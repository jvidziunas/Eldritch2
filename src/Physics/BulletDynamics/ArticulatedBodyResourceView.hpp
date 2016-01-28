/*==================================================================*\
  ArticulatedBodyResourceView.hpp
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
#include <Packages/ResourceView.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	class	ErrorCode;
}

class	btCollisionShape;

namespace Eldritch2 {
namespace Physics {
namespace BulletDynamics {

	class ArticulatedBodyResourceView : public FileSystem::ResourceView {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref ArticulatedBodyResourceView instance.
		ArticulatedBodyResourceView( FileSystem::ContentLibrary& owningLibrary, FileSystem::ContentPackage& package, const ::Eldritch2::UTF8Char* const name, ::Eldritch2::Allocator& allocator );

		//! Destroys this @ref ArticulatedBodyResourceView instance.
		~ArticulatedBodyResourceView();

	// ---------------------------------------------------

		::Eldritch2::ErrorCode	UpdateFromByteStream( const ::Eldritch2::Range<const char*> bytes ) override;

	// ---------------------------------------------------

		static ETNoAliasHint const ::Eldritch2::UTF8Char* const	GetSerializedDataTag();

	// - DATA MEMBERS ------------------------------------

	private:
		::Eldritch2::ChildAllocator							_shapeAllocator;
		::Eldritch2::ResizableArray<::btCollisionShape*>	_shapes;
	};

}	// namespace BulletDynamics
}	// namespace Physics
}	// namespace Eldritch2