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
#include <Packages/ResourceView.hpp>
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

}	// namespace BulletDynamics
}	// namespace Physics
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Physics/BulletDynamics/EngineService.inl>
//------------------------------------------------------------------//