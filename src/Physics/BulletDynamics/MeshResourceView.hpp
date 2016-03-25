/*==================================================================*\
  MeshResourceView.hpp
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
#include <Utility/MPL/FloatTypes.hpp>
#include <Packages/ResourceView.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Physics {
		namespace FlatBuffers {
			struct	RigidBody;
		}
	}

	class	ErrorCode;
}

class	btCollisionShape;

namespace Eldritch2 {
namespace Physics {
namespace BulletDynamics {

	class MeshResourceView : public FileSystem::ResourceView {
	// - TYPE PUBLISHING ---------------------------------

	public:
		struct RigidBody {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref RigidBody instance.
			RigidBody( const FlatBuffers::RigidBody& definition, ::Eldritch2::Allocator& allocator );

			~RigidBody() = default;

		// - DATA MEMBERS ------------------------------------

			::Eldritch2::uint16		boneIndex;
			::btCollisionShape*		shape;
			::Eldritch2::float32	mass;
			::Eldritch2::float32	linearDamping;
			::Eldritch2::float32	angularDamping;
			::Eldritch2::float32	friction;
			::Eldritch2::float32	rollingFriction;
			::Eldritch2::float32	restitution;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//! Constructs this @ref MeshResourceView instance.
		MeshResourceView( const ::Eldritch2::UTF8Char* const name, ::Eldritch2::Allocator& allocator );

		//! Destroys this @ref MeshResourceView instance.
		~MeshResourceView();

	// ---------------------------------------------------

		const ::Eldritch2::ResizableArray<RigidBody>&	GetBodies() const;

	// ---------------------------------------------------

		::Eldritch2::ErrorCode	AttachToPackage( const ::Eldritch2::Range<const char*> bytes, FileSystem::ContentPackage& package, FileSystem::ContentLibrary& library ) override;

		void					DetachFromPackage( FileSystem::ContentPackage& package, FileSystem::ContentLibrary& library ) const override;

	// ---------------------------------------------------

		static ETNoAliasHint const ::Eldritch2::UTF8Char* const	GetSerializedDataTag();

	// - DATA MEMBERS ------------------------------------

	private:
		::Eldritch2::ChildAllocator				_shapeAllocator;
		::Eldritch2::ResizableArray<RigidBody>	_rigidBodies;
	};

}	// namespace BulletDynamics
}	// namespace Physics
}	// namespace Eldritch2