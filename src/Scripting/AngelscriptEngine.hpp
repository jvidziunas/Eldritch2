/*==================================================================*\
  AngelscriptEngine.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/ChildAllocator.hpp>
#include <Foundation/GameEngineService.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scripting {
		class	AngelscriptWorldView;
		class	AngelscriptEngine;
		class	StringMarshal;
	}
}

class	asIScriptEngine;
struct	asSMessageInfo;

namespace Eldritch2 {
namespace Scripting {

	namespace Configuration	= ::Eldritch2::Configuration;
	namespace Foundation	= ::Eldritch2::Foundation;
	namespace Scripting		= ::Eldritch2::Scripting;

// ---------------------------------------------------

	class AngelscriptEngine : public Foundation::GameEngineService {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref AngelscriptEngine instance.
		AngelscriptEngine( Foundation::GameEngine& owningEngine );

		//! Destroys this @ref AngelscriptEngine instance.
		~AngelscriptEngine() = default;

	// ---------------------------------------------------

		const ::Eldritch2::UTF8Char* const	GetName() const override sealed;

	// ---------------------------------------------------

		ETInlineHint ::asIScriptEngine&	GetScriptEngine() const;

	// ---------------------------------------------------

	protected:
		void	AcceptInitializationVisitor( FileSystem::ResourceViewFactoryPublishingInitializationVisitor& visitor ) override sealed;
		void	AcceptInitializationVisitor( Foundation::WorldViewFactoryPublishingInitializationVisitor& visitor ) override sealed;
		void	AcceptInitializationVisitor( Scripting::ScriptAPIRegistrationInitializationVisitor& visitor ) override sealed;

		void	AcceptTaskVisitor( ::Eldritch2::Allocator& subtaskAllocator, Scheduler::Task& visitingTask, Scheduler::WorkerContext& executingContext, const InitializeEngineTaskVisitor ) override sealed;
		void	AcceptTaskVisitor( ::Eldritch2::Allocator& subtaskAllocator, Scheduler::Task& visitingTask, Scheduler::WorkerContext& executingContext, const ServiceTickTaskVisitor ) override sealed;

	// ---------------------------------------------------

		Scripting::StringMarshal	MarshalStringLiteral( const unsigned int literalLengthInOctets, const ::Eldritch2::UTF8Char* const stringLiteral );

		void						MessageCallback( const ::asSMessageInfo* messageInfo );

		void						CreateScriptAPI();

	// ---------------------------------------------------

	private:
		struct EngineDeleter {
			void	operator()( ::asIScriptEngine* const scriptEngine );
		};

	// - DATA MEMBERS ------------------------------------

		::Eldritch2::ChildAllocator							_allocator;
		::std::unique_ptr<::asIScriptEngine, EngineDeleter>	_scriptEngine;
	};

}	// namespace Scripting
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/AngelscriptEngine.inl>
//------------------------------------------------------------------//