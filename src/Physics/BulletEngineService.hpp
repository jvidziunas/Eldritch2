/*==================================================================*\
  BulletEngineService.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
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
		class	BulletEngineService;
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

		::Eldritch2::ErrorCode	InstantiateFromByteArray( const ::Eldritch2::Range<const char*>& sourceBytes );

	// ---------------------------------------------------

		static ETNoAliasHint const ::Eldritch2::UTF8Char* const	GetSerializedDataTag();

	// - DATA MEMBERS ------------------------------------

	private:
		::Eldritch2::ChildAllocator							_shapeAllocator;
		::Eldritch2::ResizableArray<::btCollisionShape*>	_shapes;
	};

// ---

	class BulletEngineService : public Foundation::GameEngineService {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref BulletEngineService instance.
		BulletEngineService( Foundation::GameEngine& owningEngine );

		//! Destroys this @ref BulletEngineService instance.
		~BulletEngineService() = default;

	// ---------------------------------------------------

		const ::Eldritch2::UTF8Char* const	GetName() const override sealed;

	// ---------------------------------------------------

		ETInlineHint size_t	GetWorldPersistentManifoldPoolSizeInElements() const;

		ETInlineHint size_t	GetWorldCollisionAlgorithmPoolSizeInElements() const;

	// ---------------------------------------------------

	protected:
		void	AcceptInitializationVisitor( FileSystem::ResourceViewFactoryPublishingInitializationVisitor& visitor ) override sealed;
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
#include <Physics/BulletEngineService.inl>
//------------------------------------------------------------------//