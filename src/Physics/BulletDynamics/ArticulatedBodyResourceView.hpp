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
#include <Utility/MPL/FloatTypes.hpp>
#include <Packages/ResourceView.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Physics {
		namespace FlatBuffers {
			struct	Body;
		}
	}

	class	ErrorCode;
}

class	btCollisionShape;

namespace Eldritch2 {
namespace Physics {
namespace BulletDynamics {

	class ArticulatedBodyResourceView : public FileSystem::ResourceView {
	// - TYPE PUBLISHING ---------------------------------

	public:
		struct Body {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref Body instance.
			Body( const Physics::FlatBuffers::Body& definition, ::Eldritch2::Allocator& allocator );

			~Body() = default;

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

		//! Constructs this @ref ArticulatedBodyResourceView instance.
		ArticulatedBodyResourceView( FileSystem::ContentLibrary& owningLibrary, FileSystem::ContentPackage& package, const ::Eldritch2::UTF8Char* const name, ::Eldritch2::Allocator& allocator );

		//! Destroys this @ref ArticulatedBodyResourceView instance.
		~ArticulatedBodyResourceView();

	// ---------------------------------------------------

		const ::Eldritch2::ResizableArray<Body>&	GetBodies() const;

	// ---------------------------------------------------

		::Eldritch2::ErrorCode	UpdateFromByteStream( const ::Eldritch2::Range<const char*> bytes ) override;

	// ---------------------------------------------------

		static ETNoAliasHint const ::Eldritch2::UTF8Char* const	GetSerializedDataTag();

	// - DATA MEMBERS ------------------------------------

	private:
		::Eldritch2::ChildAllocator			_shapeAllocator;
		::Eldritch2::ResizableArray<Body>	_bodies;
	};

}	// namespace BulletDynamics
}	// namespace Physics
}	// namespace Eldritch2