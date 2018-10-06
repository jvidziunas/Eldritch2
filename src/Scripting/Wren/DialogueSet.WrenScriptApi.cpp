/*==================================================================*\
  DialogueSet.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/AssetViews/DialogueSetAsset.hpp>
#include <Scripting/Wren/DialogueSet.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Assets/ContentLocator.hpp>
//------------------------------------------------------------------//

WrenHandle* wrenGetSlotHandle(WrenVM* vm, int slot);
void        wrenReleaseHandle(WrenVM* vm, WrenHandle* handle);

#if 0
DialogueSet::RuleIterator DialogueSet::Match(WrenHandle* wrenFacts) const {
	const ObjMap* const facts(AS_MAP(wrenFacts->value));
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
#endif

namespace Eldritch2 { namespace Scripting { namespace Wren {

	using namespace ::Eldritch2::Scripting::Wren::AssetViews;
	using namespace ::Eldritch2::Assets;

	ET_IMPLEMENT_WREN_CLASS(DialogueSet) {
		api.DefineClass<DialogueSet>(ET_BUILTIN_WREN_MODULE_NAME(Core), "DialogueSet", // clang-format off
			{/*	Static methods */
				ForeignMethod("new(_)", [](WrenVM* vm) {
					const DialogueSetAsset* const asset(Cast<DialogueSetAsset>(GetSlotAs<AssetReference>(vm, 1)));
					ET_ABORT_WREN_UNLESS(asset, "Asset must be a DialogueSetAsset.");

					SetReturn<DialogueSet>(vm, /*classSlot =*/0);
				}), },
			{/*	Methods */
				ForeignMethod("match(_)", [](WrenVM* vm) {
					DialogueSet&      self(GetSlotAs<DialogueSet>(vm, 0));
					WrenHandle* const wrenFacts(wrenGetSlotHandle(vm, 1));
					ET_AT_SCOPE_EXIT(wrenReleaseHandle(vm, wrenFacts));

					// const ObjMap* const facts(AS_MAP(wrenFacts->value));

					DialogueSet::QueryBuilderType query(MallocAllocator(), self.GetSymbols());
					self.Match(MallocAllocator(), query);
				}), }); // clang-format on
	}

}}} // namespace Eldritch2::Scripting::Wren
