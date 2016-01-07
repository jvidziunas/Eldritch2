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
#include <Scripting/AngelScript/SmartPointers.hpp>
#include <Utility/Memory/ChildAllocator.hpp>
#include <Foundation/GameEngineService.hpp>
#include <Packages/ResourceViewFactory.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scripting {
		class	StringMarshal;
	}
}

class	asIScriptEngine;
struct	asSMessageInfo;

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {

	class EngineService : public Foundation::GameEngineService {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref EngineService instance.
		EngineService( Foundation::GameEngine& owningEngine );

		//! Destroys this @ref EngineService instance.
		~EngineService() = default;

	// ---------------------------------------------------

		const ::Eldritch2::UTF8Char* const	GetName() const override sealed;

		ETInlineHint ::asIScriptEngine&		GetScriptEngine() const;

	// ---------------------------------------------------

		::Eldritch2::ErrorCode	AllocateWorldView( ::Eldritch2::Allocator& allocator, Foundation::World& world ) override;

	// ---------------------------------------------------

	protected:
		void	AcceptInitializationVisitor( FileSystem::ResourceViewFactoryPublishingInitializationVisitor& visitor ) override sealed;
		void	AcceptInitializationVisitor( Scripting::ScriptAPIRegistrationInitializationVisitor& visitor ) override sealed;

	// ---------------------------------------------------

		void	AcceptTaskVisitor( ::Eldritch2::Allocator& subtaskAllocator, Scheduler::Task& visitingTask, Scheduler::WorkerContext& executingContext, const InitializeEngineTaskVisitor ) override sealed;
		void	AcceptTaskVisitor( ::Eldritch2::Allocator& subtaskAllocator, Scheduler::Task& visitingTask, Scheduler::WorkerContext& executingContext, const ServiceTickTaskVisitor ) override sealed;

	// ---------------------------------------------------

		Scripting::StringMarshal	MarshalStringLiteral( const unsigned int literalLengthInOctets, const ::Eldritch2::UTF8Char* const stringLiteral );

		void						MessageCallback( const ::asSMessageInfo* messageInfo );

		void						CreateScriptAPI();

	// - TYPE PUBLISHING ---------------------------------

	private:
		class BytecodePackageViewFactory : public FileSystem::ResourceViewFactory {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref BytecodePackageViewFactory instance.
			BytecodePackageViewFactory();

			~BytecodePackageViewFactory() = default;

		// ---------------------------------------------------

			ETInlineHint ::asIScriptEngine&	GetScriptEngine() const;

			ETInlineHint void				SetScriptEngine( AngelScript::EngineHandle&& engine );

		// ---------------------------------------------------

			::Eldritch2::ErrorCode	AllocateResourceView( ::Eldritch2::Allocator&				allocator,
														  FileSystem::ContentLibrary&			contentLibrary,
														  FileSystem::ContentPackage&			package,
														  const ::Eldritch2::UTF8Char* const	name,
														  const ::Eldritch2::Range<const char*>	sourceAsset ) override;

		// ---------------------------------------------------

			void	AcceptInitializationVisitor( Configuration::ConfigurationPublishingInitializationVisitor& visitor );

		// - DATA MEMBERS ------------------------------------

		private:
			AngelScript::EngineHandle	_scriptEngine;
		};

	// ---

		class ObjectGraphViewFactory : public FileSystem::ResourceViewFactory {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref ObjectGraphViewFactory instance.
			ObjectGraphViewFactory() = default;

			~ObjectGraphViewFactory() = default;

		// ---------------------------------------------------

			::Eldritch2::ErrorCode	AllocateResourceView( ::Eldritch2::Allocator&				allocator,
														  FileSystem::ContentLibrary&			contentLibrary,
														  FileSystem::ContentPackage&			package,
														  const ::Eldritch2::UTF8Char* const	name,
														  const ::Eldritch2::Range<const char*>	sourceAsset ) override;

		// ---------------------------------------------------
			
			void	AcceptInitializationVisitor( Configuration::ConfigurationPublishingInitializationVisitor& visitor );
		};

	// - DATA MEMBERS ------------------------------------

		::Eldritch2::ChildAllocator	_allocator;

		BytecodePackageViewFactory	_bytecodePackageFactory;
		ObjectGraphViewFactory		_objectGraphFactory;
	};

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/AngelScript/EngineService.inl>
//------------------------------------------------------------------//