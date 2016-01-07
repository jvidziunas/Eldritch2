/*==================================================================*\
  EngineService.hpp
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
#include <Foundation/GameEngineService.hpp>
#include <Packages/ResourceViewFactory.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Physics {
namespace BulletDynamics {

	class EngineService : public Foundation::GameEngineService {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref EngineService instance.
		EngineService( Foundation::GameEngine& owningEngine );

		//! Destroys this @ref EngineService instance.
		~EngineService() = default;

	// ---------------------------------------------------

		const ::Eldritch2::UTF8Char* const	GetName() const override sealed;

		ETInlineHint size_t					GetWorldPersistentManifoldPoolSizeInElements() const;

		ETInlineHint size_t					GetWorldCollisionAlgorithmPoolSizeInElements() const;

	// ---------------------------------------------------

		::Eldritch2::ErrorCode	AllocateWorldView( ::Eldritch2::Allocator& allocator, Foundation::World& world ) override;

	// ---------------------------------------------------

	protected:
		void	AcceptInitializationVisitor( FileSystem::ResourceViewFactoryPublishingInitializationVisitor& visitor ) override sealed;
		void	AcceptInitializationVisitor( Configuration::ConfigurationPublishingInitializationVisitor& visitor ) override sealed;
		void	AcceptInitializationVisitor( Scripting::ScriptAPIRegistrationInitializationVisitor& visitor ) override sealed;

	// - TYPE PUBLISHING ---------------------------------

	private:
		class CollisionShapeViewFactory : public FileSystem::ResourceViewFactory {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref CollisionShapeViewFactory instance.
			CollisionShapeViewFactory() = default;

			~CollisionShapeViewFactory() = default;

		// ---------------------------------------------------

			void	AcceptInitializationVisitor( Configuration::ConfigurationPublishingInitializationVisitor& visitor );

		// ---------------------------------------------------

			::Eldritch2::ErrorCode	AllocateResourceView( ::Eldritch2::Allocator&				allocator,
														  FileSystem::ContentLibrary&			contentLibrary,
														  FileSystem::ContentPackage&			package,
														  const ::Eldritch2::UTF8Char* const	name,
														  const ::Eldritch2::Range<const char*>	sourceAsset ) override;
		};

	// - DATA MEMBERS ------------------------------------

		CollisionShapeViewFactory						_collisionShapeFactory;

		Configuration::ConfigurablePODVariable<size_t>	_persistentManifoldPoolSizeInElements;
		Configuration::ConfigurablePODVariable<size_t>	_collisionAlgorithmPoolSizeInElements;
	};

}	// namespace BulletDynamics
}	// namespace Physics
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Physics/BulletDynamics/EngineService.inl>
//------------------------------------------------------------------//