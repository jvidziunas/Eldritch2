/*==================================================================*\
  WorldView.hpp
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
#include <Utility/Containers/UnorderedMap.hpp>
#include <Scripting/ObjectHandle.hpp>
#include <Scripting/MessageBus.hpp>
#include <Foundation/WorldView.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scripting {
		class	ScriptAPIRegistrationInitializationVisitor;
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
			using HandlerCache = ::Eldritch2::UnorderedMap<::Eldritch2::Pair<::asIObjectType*, const char*>, ::asIScriptFunction*>;

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

		static void	ExposeScriptAPI( Scripting::ScriptAPIRegistrationInitializationVisitor& visitor );

	// ---------------------------------------------------

	protected:
		void	AcceptTaskVisitor( Scheduler::WorkerContext& executingContext, Scheduler::WorkerContext::FinishCounter& finishCounter, const FrameTickTaskVisitor ) override sealed;
		void	AcceptTaskVisitor( Scheduler::WorkerContext& executingContext, Scheduler::WorkerContext::FinishCounter& finishCounter, const ScriptTickTaskVisitor ) override sealed;

	// ---------------------------------------------------

		void	AcceptViewVisitor( const ScriptExecutionPreparationVisitor ) override sealed;
		void	AcceptViewVisitor( const DeletionPreparationVisitor ) override sealed;
		void	AcceptViewVisitor( const LoadFinalizationVisitor ) override sealed;

	// - DATA MEMBERS ------------------------------------

	private:
		::Eldritch2::ChildAllocator	_stringAllocator;
		::asIScriptEngine&			_scriptEngine;
		AngelScript::ObjectHandle	_rulesEntity;
	};

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2