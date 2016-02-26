/*==================================================================*\
  MeshResourceView.hpp
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
#include <Utility/Memory/InstanceDeleters.hpp>
#include <Physics/PhysX/SmartPointers.hpp>
#include <Utility/MPL/FloatTypes.hpp>
#include <Packages/ResourceView.hpp>
//------------------------------------------------------------------//
#include <foundation/PxTransform.h>
#include <PxMaterial.h>
#include <PxShape.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
	template <typename Iterator>
	class	Range;
}

namespace Eldritch2 {
namespace Physics {
namespace PhysX {

	class MeshResourceView : public FileSystem::ResourceView {
	// - TYPE PUBLISHING ---------------------------------

	public:
		class RigidBody {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref RigidBody instance.
			/*!	@param[in] shape PhysX shape describing the geometry, positioning and material data for the rigid body.
				*/
			RigidBody( const ::Eldritch2::uint16 bodyIndex, PhysX::UniquePointer<::physx::PxShape>&& shape );
			//!	Constructs this @ref RigidBody instance.
			/*!	@param[in] body Rigid body definition to transfer resources from.
				*/
			RigidBody( RigidBody&& body );

		// ---------------------------------------------------

			ETInlineHint ::Eldritch2::uint16	GetBodyIndex() const;

			ETInlineHint ::physx::PxShape&		GetShape() const;

		// - DATA MEMBERS ------------------------------------

		private:
			PhysX::UniquePointer<::physx::PxShape>	_shape;
			::Eldritch2::uint16						_bodyIndex;
		};

	// ---

		class SoftBody {

		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//!	Constructs this @ref MeshResourceView instance.
		/*!	@param[in] owningLibrary @ref ContentLibrary the new @ref ResourceView will register itself into.
			@param[in] package @ref ContentPackage responsible for managing the lifespan of the new @ref ResourceView.
			@param[in] name @parblock Null-terminated C string containing the name of the resource. The referenced memory does not need
				to exist past the end of the constructor, as the view will maintain an internal copy. @endparblock
			@param[in] nameAllocator @ref Allocator instance to provide memory for internal allocations.
			*/
		MeshResourceView( FileSystem::ContentLibrary& owningLibrary, FileSystem::ContentPackage& package, const::Eldritch2::UTF8Char* const name, ::Eldritch2::Allocator& allocator );

		~MeshResourceView() = default;

	// ---------------------------------------------------

		ETInlineHint ::Eldritch2::Range<const RigidBody*>	GetRigidBodies() const;

		ETInlineHint ::Eldritch2::Range<const SoftBody*>	GetSoftBodies() const;

	// ---------------------------------------------------

		::Eldritch2::ErrorCode	UpdateFromByteStream( const ::Eldritch2::Range<const char*> bytes ) override;

	// ---------------------------------------------------

		static ETNoAliasHint const ::Eldritch2::UTF8Char* const	GetSerializedDataTag();

	// - DATA MEMBERS ------------------------------------

	private:
		::Eldritch2::ResizableArray<RigidBody>	_rigidBodies;
		::Eldritch2::ResizableArray<SoftBody>	_softBodies;
	};

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Physics/PhysX/MeshResourceView.inl>
//------------------------------------------------------------------//