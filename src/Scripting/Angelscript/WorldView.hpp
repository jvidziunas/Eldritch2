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

namespace std {

	template<>
	struct default_delete<::asIScriptObject> {
		ETInlineHint void operator()( ::asIScriptObject* const object ) {
			object->GetEngine()->ReleaseScriptObject( object, object->GetObjectType() );
		}
	};

}	// namespace std

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {

	class WorldView : public Foundation::WorldView {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref WorldView instance.
		WorldView( Foundation::World& owningWorld, ::asIScriptEngine& scriptEngine );

		//!	Destroys this @ref WorldView instance.
		~WorldView();

	// ---------------------------------------------------

		static void	ExposeScriptAPI( Scripting::ScriptAPIRegistrationInitializationVisitor& visitor );

	// ---------------------------------------------------

	protected:
		void	AcceptTaskVisitor( ::Eldritch2::Allocator& subtaskAllocator, Scheduler::WorkerContext& executingContext, Scheduler::Task& visitingTask, const FrameTickTaskVisitor ) override sealed;
		void	AcceptTaskVisitor( ::Eldritch2::Allocator& subtaskAllocator, Scheduler::WorkerContext& executingContext, Scheduler::Task& visitingTask, const ScriptTickTaskVisitor ) override sealed;

	// ---------------------------------------------------

		void	AcceptViewVisitor( const ScriptExecutionPreparationVisitor ) override sealed;
		void	AcceptViewVisitor( Scripting::ScriptMessageSink& messageSink ) override sealed;
		void	AcceptViewVisitor( const DeletionPreparationVisitor ) override sealed;
		void	AcceptViewVisitor( const LoadFinalizationVisitor ) override sealed;

	// - DATA MEMBERS ------------------------------------

	private:
		::Eldritch2::ChildAllocator																_stringAllocator;
		::asIScriptEngine&																		_scriptEngine;
		
		::std::unique_ptr<::asIScriptObject>													_gameRulesEntity;
		::Eldritch2::UnorderedMap<::Eldritch2::uint64, ::std::unique_ptr<::asIScriptObject>>	_entityDispatchTable;
	};

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2