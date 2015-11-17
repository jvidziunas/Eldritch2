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
#include <Utility/Containers/UnorderedMap.hpp>
#include <Scripting/ObjectHandle.hpp>
#include <Scripting/MessageSink.hpp>
#include <Foundation/WorldView.hpp>
//------------------------------------------------------------------//
#include <angelscript.h>
//------------------------------------------------------------------//
#include <memory>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scripting {
		class	ScriptAPIRegistrationInitializationVisitor;
	}
}

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {

	class WorldView : public Foundation::WorldView {
	// - TYPE PUBLISHING ---------------------------------

	public:
		//!	Deleter for C++ smart pointer objects.
		struct ScriptObjectDeleter {
			void	operator()( ::asIScriptObject* const object );
		};

	// ---

		class MessageSink : public Scripting::MessageSink {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref MessageSink instance.
			MessageSink() = default;

			~MessageSink() = default;

		// ---------------------------------------------------

			//!	Pops all script messages currently stored in the queue, calling handler functions on the specified script object.
			/*!	@param[in] scriptObject AngelScript object to execute handler functions on.
				@param[in] scriptContext AngelScript execution context that will run script code.
				@remarks Not thread-safe.
				*/
			void	Drain( ::asIScriptObject& scriptObject, ::asIScriptContext& scriptContext );

		private:
			::Eldritch2::IntrusiveVyukovMPSCQueue<Scripting::Message>	_pendingMessages;
		};

	// ---

		using ScriptObjectHandle = ::std::unique_ptr<::asIScriptObject, ScriptObjectDeleter>;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//!	Constructs this @ref WorldView instance.
		WorldView( Foundation::World& owningWorld, ::asIScriptEngine& scriptEngine );

		~WorldView();

	// ---------------------------------------------------

		static void	ExposeScriptAPI( Scripting::ScriptAPIRegistrationInitializationVisitor& visitor );

	// ---------------------------------------------------

	protected:
		void	AcceptTaskVisitor( ::Eldritch2::Allocator& subtaskAllocator, Scheduler::WorkerContext& executingContext, Scheduler::Task& visitingTask, const FrameTickTaskVisitor ) override sealed;
		void	AcceptTaskVisitor( ::Eldritch2::Allocator& subtaskAllocator, Scheduler::WorkerContext& executingContext, Scheduler::Task& visitingTask, const ScriptTickTaskVisitor ) override sealed;

	// ---------------------------------------------------

		void	AcceptViewVisitor( const ScriptExecutionPreparationVisitor ) override sealed;
		void	AcceptViewVisitor( const DeletionPreparationVisitor ) override sealed;
		void	AcceptViewVisitor( const LoadFinalizationVisitor ) override sealed;

	// - DATA MEMBERS ------------------------------------

	private:
		::Eldritch2::ChildAllocator						_stringAllocator;
		::asIScriptEngine&								_scriptEngine;
		
		ScriptObjectHandle								_gameRulesEntity;
		::Eldritch2::UnorderedMap<::Eldritch2::uint64,
								  ScriptObjectHandle>	_entityDispatchTable;
	};

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2