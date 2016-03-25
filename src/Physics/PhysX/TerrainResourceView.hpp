/*==================================================================*\
  TerrainResourceView.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/SmartPointers.hpp>
#include <Packages/ResourceView.hpp>
//------------------------------------------------------------------//

namespace physx {
	class	PxShape;
}

namespace Eldritch2 {
namespace Physics {
namespace PhysX {

	class TerrainResourceView : public FileSystem::ResourceView {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref TerrainResourceView instance.
		/*!	@param[in] name UTF-8-encoded string containing the name of the resource the @ref TerrainResourceView will be describing.
			*/
		TerrainResourceView( const ::Eldritch2::UTF8Char* const name );

		~TerrainResourceView();

	// ---------------------------------------------------

		ETInlineHint ::physx::PxShape&	GetShape() const;

	// ---------------------------------------------------

		::Eldritch2::ErrorCode	AttachToPackage( const ::Eldritch2::Range<const char*> bytes, FileSystem::ContentPackage& package, FileSystem::ContentLibrary& library ) override;

		void					DetachFromPackage( FileSystem::ContentPackage& package, FileSystem::ContentLibrary& library ) const override;

	// ---------------------------------------------------

		static ETNoAliasHint const ::Eldritch2::UTF8Char* const	GetSerializedDataTag();

	// - DATA MEMBERS ------------------------------------

	private:
		PhysX::UniquePointer<::physx::PxShape>	_shape;
	};

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Physics/PhysX/TerrainResourceView.inl>
//------------------------------------------------------------------//