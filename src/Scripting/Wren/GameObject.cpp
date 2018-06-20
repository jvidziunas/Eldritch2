/*==================================================================*\
  GameObject.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/WrenInternal.hpp>
#include <Scripting/Wren/GameObject.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scripting {
		namespace Wren {

			GameObject::GameObject() : _components(MallocAllocator("GameObject Component Allocator")) {}

		// ---------------------------------------------------

			GameObject::~GameObject() {
				ET_ASSERT(_components.IsEmpty(), "Leaking Wren game object components!");
			}

		// ---------------------------------------------------

			GameObject::Iterator GameObject::FindComponentByType(WrenVM* vm, WrenHandle* type) {
				return FindIf(_components.Begin(), _components.End(), [vm, type](WrenHandle* component) {
					return wrenGetClassInline(vm, component->value) == AS_CLASS(type->value);
				});
			}

		// ---------------------------------------------------

			bool GameObject::HasComponentOfType(WrenVM* vm, WrenHandle* type) {
				return FindComponentByType(vm, type) != _components.End();
			}

		// ---------------------------------------------------

			void GameObject::InsertComponent(WrenVM* vm, WrenHandle* component) {
				const Iterator	existing(FindComponentByType(vm, component));

				if (existing != _components.End()) {
				//	Discard the old component, replacing it with the passed-in object.
					Swap(*existing, component);

					wrenReleaseHandle(vm, component);
				}

				_components.Append(component);
			}

		// ---------------------------------------------------

			void GameObject::FreeComponents(WrenVM* vm) {
				for (WrenHandle* component : _components) {
					wrenReleaseHandle(vm, component);
				}

				_components.Clear(ReleaseMemorySemantics());
			}

		}	// namespace Wren
	}	// namespace Scripting
}	// namespace Eldritch2