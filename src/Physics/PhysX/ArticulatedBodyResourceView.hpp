/*==================================================================*\
  ArticulatedBodyResourceView.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/ResizableArray.hpp>
#include <Physics/PhysX/SmartPointers.hpp>
#include <Packages/ResourceView.hpp>
//------------------------------------------------------------------//
#include <geometry/PxGeometry.h>
#include <PxMaterial.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Physics {
namespace PhysX {

	class ArticulatedBodyResourceView : public FileSystem::ResourceView {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		class Body {
		public:
			Body( PhysX::UniquePointer<::physx::PxMaterial>&& material );
			Body( Body&& body );

		// - DATA MEMBERS ------------------------------------

			PhysX::UniquePointer<::physx::PxMaterial>	material;
		};

		//!	Constructs this @ref ArticulatedBodyResourceView instance.
		/*!	@param[in] owningLibrary @ref ContentLibrary the new @ref ResourceView will register itself into.
			@param[in] package @ref ContentPackage responsible for managing the lifespan of the new @ref ResourceView.
			@param[in] name @parblock Null-terminated C string containing the name of the resource. The referenced memory does not need
				to exist past the end of the constructor, as the view will maintain an internal copy. @endparblock
			@param[in] nameAllocator @ref Allocator instance for providing memory for the internal name copy.
			@param[in] bodyDefinitionAllocator @ref Allocator instance for providing memory for the internal body descriptor collection.
			*/
		ArticulatedBodyResourceView( FileSystem::ContentLibrary& owningLibrary, FileSystem::ContentPackage& package, const::Eldritch2::UTF8Char* const name, ::Eldritch2::Allocator& nameAllocator, ::Eldritch2::Allocator& bodyDefinitionAllocator );

		~ArticulatedBodyResourceView() = default;

	// ---------------------------------------------------

		::Eldritch2::ErrorCode	UpdateFromByteStream( const ::Eldritch2::Range<const char*> bytes ) override;

	// ---------------------------------------------------

		static ETNoAliasHint const ::Eldritch2::UTF8Char* const	GetSerializedDataTag();

	// - DATA MEMBERS ------------------------------------

	private:
		::Eldritch2::ResizableArray<Body>	_bodies;
	};

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2