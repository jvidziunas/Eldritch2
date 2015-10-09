/*==================================================================*\
  BulletEngine.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Configuration/ConfigurablePODVariable.hpp>
#include <Utility/Memory/ChildAllocator.hpp>
#include <Utility/Memory/ArenaAllocator.hpp>
#include <Foundation/GameEngineService.hpp>
#include <Packages/ResourceView.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Physics {
		class	BulletEngine;
	}
}

class	btCollisionShape;

namespace Eldritch2 {
namespace Physics {

	namespace Configuration	= ::Eldritch2::Configuration;
	namespace FileSystem	= ::Eldritch2::FileSystem;
	namespace Foundation	= ::Eldritch2::Foundation;
	namespace Scripting		= ::Eldritch2::Scripting;
	namespace Physics		= ::Eldritch2::Physics;

// ---------------------------------------------------

	class BulletCollisionShapeView : public FileSystem::ResourceView {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref BulletCollisionShapeView instance.
		BulletCollisionShapeView( const Initializer& initializer, ::Eldritch2::Allocator& allocator );

		//! Destroys this @ref BulletCollisionShapeView instance.
		~BulletCollisionShapeView() = default;

	// ---------------------------------------------------

		static ETNoAliasHint const ::Eldritch2::UTF8Char* const	GetSerializedDataTag();

	// ---------------------------------------------------

		static ::Eldritch2::ErrorCode DeserializeFromPackageExport( ::Eldritch2::Allocator& allocator, const Initializer& initializer, Physics::BulletEngine& physicsEngine );

	// - DATA MEMBERS ------------------------------------

	private:
		::Eldritch2::ChildAllocator							_shapeAllocator;
		::Eldritch2::ResizableArray<::btCollisionShape*>	_shapes;
	};

// ---

	class BulletEngine : public Foundation::GameEngineService {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref BulletEngine instance.
		BulletEngine( Foundation::GameEngine& owningEngine );

		//! Destroys this @ref BulletEngine instance.
		~BulletEngine() = default;

	// ---------------------------------------------------

		const ::Eldritch2::UTF8Char* const	GetName() const override sealed;

	// ---------------------------------------------------

		ETInlineHint size_t	GetWorldPersistentManifoldPoolSizeInElements() const;

		ETInlineHint size_t	GetWorldCollisionAlgorithmPoolSizeInElements() const;

	// ---------------------------------------------------

	protected:
		void	AcceptInitializationVisitor( Configuration::ConfigurationPublishingInitializationVisitor& visitor ) override sealed;
		void	AcceptInitializationVisitor( Foundation::WorldViewFactoryPublishingInitializationVisitor& visitor ) override sealed;
		void	AcceptInitializationVisitor( Scripting::ScriptAPIRegistrationInitializationVisitor& visitor ) override sealed;

	// - DATA MEMBERS ------------------------------------

	private:
		Configuration::ConfigurablePODVariable<size_t>	_persistentManifoldPoolSizeInElements;
		Configuration::ConfigurablePODVariable<size_t>	_collisionAlgorithmPoolSizeInElements;
	};

}	// namespace Physics
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Physics/BulletEngine.inl>
//------------------------------------------------------------------//