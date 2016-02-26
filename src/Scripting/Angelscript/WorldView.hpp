/*==================================================================*\
  WorldView.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  �2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/AngelScript/SmartPointers.hpp>
#include <Utility/Containers/HashMap.hpp>
#include <Scripting/ObjectHandle.hpp>
#include <Scripting/MessageBus.hpp>
#include <Foundation/WorldView.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scripting {
		class	ScriptApiRegistrationInitializationVisitor;
		class	MessageBus;
	}
}

class	asIScriptFunction;
class	asIScriptContext;
class	asIScriptObject;
class	asIScriptEngine;
class	asIObjectType;

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {

	class WorldView : public Foundation::WorldView {
	// - TYPE PUBLISHING ---------------------------------

	public:
		class MessageBus : public Scripting::MessageBus {
		// - TYPE PUBLISHING ---------------------------------

		public:
			using HandlerCache = ::Eldritch2::HashMap<::Eldritch2::Pair<::asIObjectType*, const char*>, ::asIScriptFunction*>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			//!	Constructs this @ref MessageBus instance.
			MessageBus() = default;

			~MessageBus() = default;

		// ---------------------------------------------------

			//!	Pops all script messages currently stored in the queue, calling handler functions on the owned script object.
			/*!	@param[in] scriptObject Angelscript object that the MessageBus is collecting event notifications for.
				@param[in] scriptContext AngelScript execution context that will run script code.
				@remarks Not thread-safe.
				*/
			void	DispatchQueuedMessages( HandlerCache& cache, ::asIScriptObject& scriptObject, ::asIScriptContext& scriptContext );
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//!	Constructs this @ref WorldView instance.
		WorldView( Foundation::World& owningWorld, ::asIScriptEngine& scriptEngine );

		~WorldView();

	// ---------------------------------------------------

		AngelScript::ObjectHandle	Spawn( const char* const className );

	// ---------------------------------------------------

		static void	ExposeScriptAPI( Scripting::ScriptApiRegistrationInitializationVisitor& visitor );

	// ---------------------------------------------------

	protected:
		void	OnFrameTick( Scheduler::WorkerContext& executingContext ) override sealed;

		void	OnScriptTick( Scheduler::WorkerContext& executingContext ) override sealed;

	// ---------------------------------------------------

		void	AcceptViewVisitor( const ScriptExecutionPreparationVisitor ) override sealed;
		void	AcceptViewVisitor( const DeletionPreparationVisitor ) override sealed;

		void	OnGameStart( Scheduler::WorkerContext& executingContext ) override sealed;

	// - DATA MEMBERS ------------------------------------

	private:
		::Eldritch2::ChildAllocator	_stringAllocator;
		::asIScriptEngine&			_scriptEngine;
		AngelScript::ObjectHandle	_rulesEntity;
	};

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2