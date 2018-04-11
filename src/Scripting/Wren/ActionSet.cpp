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
#include <Input/InputDevice.hpp>
//------------------------------------------------------------------//

void wrenSetSlotHandle( WrenVM* vm, int slot, WrenHandle* handle );
void wrenSetSlotDouble( WrenVM* vm, int slot, double value );
void wrenReleaseHandle( WrenVM* vm, WrenHandle* handle );

namespace Eldritch2 {
namespace Scripting {
namespace Wren {

	using namespace ::Eldritch2::Input;

	ActionSet::ActionSet() : _actions( MallocAllocator( "Wren Action List Allocator" ) ) {}

// ---------------------------------------------------

	ActionSet::~ActionSet() {
		ET_ASSERT( _actions.IsEmpty(), "Leaking Wren handles!" );
	}

// ---------------------------------------------------

	bool ActionSet::DispatchEvents( WrenVM* vm, WrenHandle* receiver, WrenHandle* binaryCallHandle ) {
		for (Action& action : _actions) {
			wrenSetSlotHandle( vm, 0, action.closure );
			wrenSetSlotHandle( vm, 1, receiver );
			wrenSetSlotDouble( vm, 2, AsFloat64( eastl::exchange( action.weight, 0 ) ) );

			if (ET_UNLIKELY( wrenCall( vm, binaryCallHandle ) != WREN_RESULT_SUCCESS )) {
				return false;
			}
		}

		return true;
	}

// ---------------------------------------------------

	bool ActionSet::TryAcquire( InputDevice& device ) {
		InputDevice::BindingMap<>	bindings( MallocAllocator( "asdf" ) );

		return device.TryAcquire( eastl::move( bindings ), [this] ( ActionId id, int32 weight ) {
			_actions[id].weight += weight;
		} );
	}

// ---------------------------------------------------

	void ActionSet::BindResources( WrenVM* vm, int bindingsSlot ) {
		WrenHandle* const	map( wrenGetSlotHandle( vm, bindingsSlot ) );
		const ObjMap* const bindings( AS_MAP( map->value ) );
		ArrayList<Action>	actions( _actions.GetAllocator() );

		for (const MapEntry& binding : Range<const MapEntry*>( bindings->entries, bindings->entries + bindings->capacity )) {
			if (ET_LIKELY( IS_STRING( binding.key ) & IS_OBJ( binding.value ) )) {
			//	Action names must be strings, and bindings must implement call(_).
				actions.Append( { wrenMakeHandle( vm, binding.value ), 0, 0u } );
			}
		}

		FreeResources( vm );

		Swap( _actions, actions );

		wrenReleaseHandle( vm, map );
	}

// ---------------------------------------------------

	void ActionSet::FreeResources( WrenVM* vm ) {
		for (Action& action : _actions) {
			wrenReleaseHandle( vm, action.closure );
		}

		_actions.Clear();
	}

}	// namespace Wren
}	// namespace Scripting
}	// namespace Eldritch2