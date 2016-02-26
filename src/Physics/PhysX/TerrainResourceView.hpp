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
		/*!	@param[in] owningLibrary Reference to the @ref ContentLibrary that will own/index the @ref TerrainResourceView.
			@param[in] package Reference to the @ref ContentPackage that will manage the lifetime of the @ref TerrainResourceView.
			@param[in] name Null-terminated C string containing the name of the resource the @ref TerrainResourceView will be describing.
			@param[in] allocator The @ref Allocator that the @ref TerrainResourceView should use to perform internal allocations.
			*/
		TerrainResourceView( FileSystem::ContentLibrary& library, FileSystem::ContentPackage& package, const ::Eldritch2::UTF8Char* const name, ::Eldritch2::Allocator& allocator );

		~TerrainResourceView();

	// ---------------------------------------------------

		ETInlineHint ::physx::PxShape&	GetShape() const;

	// ---------------------------------------------------

		::Eldritch2::ErrorCode	UpdateFromByteStream( const ::Eldritch2::Range<const char*> bytes ) override;

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