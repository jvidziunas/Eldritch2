/*==================================================================*\
  DialogueSet.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/WrenInternal.hpp>
#include <Scripting/Wren/DialogueSet.hpp>
//------------------------------------------------------------------//
#include <wren.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scripting { namespace Wren {

	DialogueResponse::DialogueResponse(WrenHandle* function) :
		_function(function) {}

	// ---------------------------------------------------

	bool DialogueResponse::Invoke(WrenVM* vm, WrenHandle* receiver, WrenHandle* unaryCallStub) const {
		wrenEnsureSlots(vm, 2);

		wrenSetSlotHandle(vm, 0, _function);
		wrenSetSlotHandle(vm, 1, receiver);

		return wrenCall(vm, unaryCallStub) == WREN_RESULT_SUCCESS;
	}

	// ---------------------------------------------------

	DialogueSet::RuleIterator DialogueSet::Match(WrenHandle* factsMap) const {
		const ObjMap* const facts(AS_MAP(factsMap->value));
		QueryBuilderType    query(MallocAllocator(), GetSymbols());

		for (const MapEntry *fact(facts->entries), *end(fact + facts->capacity); fact != end; ++fact) {
			if (ET_UNLIKELY(!IS_STRING(fact->key))) {
				//	Fact names must be strings.
				continue;
			}

			if (IS_STRING(fact->value)) {
				query.InsertFact(AS_CSTRING(fact->key), AS_CSTRING(fact->value));
			} else if (IS_NUM(fact->value)) {
				query.InsertFact(AS_CSTRING(fact->key), AS_NUM(fact->value));
			} else if (IS_BOOL(fact->value)) {
				query.InsertFact(AS_CSTRING(fact->key), AS_BOOL(fact->value));
			} else {
				query.InsertFact(AS_CSTRING(fact->key));
			}
		}

		return RuleSet<DialogueResponse>::Match(MallocAllocator("DialogueSet::Match() Temporary Allocator"), query);
	}

}}} // namespace Eldritch2::Scripting::Wren
