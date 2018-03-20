/*==================================================================*\
  ActionSet.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/WrenInternal.hpp>
#include <Scripting/Wren/ActionSet.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {
namespace Wren {

	ActionSet::ActionSet() : _actions( MallocAllocator( "Wren Action List Allocator" ) ) {}

// ---------------------------------------------------

	ActionSet::~ActionSet() {
		ET_ASSERT( _actions.IsEmpty(), "Leaking Wren handles!" );
	}

// ---------------------------------------------------

	void ActionSet::Activate( uint32 index, float32 amount ) {
		ET_ASSERT( index < _actions.GetSize(), "Index out of bounds" );

		_actions[index].weight += amount;
	}

// ---------------------------------------------------

	WrenInterpretResult ActionSet::Dispatch( WrenVM* vm, WrenHandle* receiver, WrenHandle* binaryCallHandle ) {
		for (Action& action : _actions) {
			if (action.weight == 0.0f) {
				continue;
			}

			wrenSetSlotHandle( vm, 0, action.handler );
			wrenSetSlotHandle( vm, 1, receiver );
			wrenSetSlotDouble( vm, 2, eastl::exchange( action.weight, 0.0f ) );

			const WrenInterpretResult result( wrenCall( vm, binaryCallHandle ) );
			if (ET_UNLIKELY( result != WREN_RESULT_SUCCESS )) {
				return result;
			}
		}

		return WREN_RESULT_SUCCESS;
	}

// ---------------------------------------------------

	void ActionSet::BindResources( WrenVM* vm, WrenHandle* bindingsMap ) {
		const ObjMap* const bindings( AS_MAP( bindingsMap->value ) );
		ArrayList<Action>	actions( _actions.GetAllocator() );

		for (const MapEntry* binding( bindings->entries ), *end( binding + bindings->capacity ); binding != end; ++binding) {
			if (ET_UNLIKELY( !(IS_STRING( binding->key ) & IS_OBJ( binding->value ) ) )) {
			//	Action names must be strings, and bindings must implement call(_).
				continue;
			}

			actions.Append( { wrenMakeHandle( vm, binding->value ), 0 } );
		}

		FreeResources( vm );

		Swap( _actions, actions );
	}

// ---------------------------------------------------

	void ActionSet::FreeResources( WrenVM* vm ) {
		for (Action& action : _actions) {
			wrenReleaseHandle( vm, action.handler );
		}

		_actions.Clear();
	}

}	// namespace Wren
}	// namespace Scripting
}	// namespace Eldritch2