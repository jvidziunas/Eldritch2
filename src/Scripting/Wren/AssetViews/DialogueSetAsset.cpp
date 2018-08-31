/*==================================================================*\
  DialogueSetAsset.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/AssetViews/DialogueSetAsset.hpp>
//------------------------------------------------------------------//
#include <Flatbuffers/DialogueSet_generated.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scripting { namespace Wren { namespace AssetViews {

	using namespace ::Eldritch2::Scripting::Wren::FlatBuffers;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Assets;
	using namespace ::flatbuffers;

	namespace {

		ETInlineHint ETForceInlineHint void ParseCriterion(Wren::DialogueSet::RuleBuilderType& result, const FlatBuffers::Criterion* criterion) {
			switch (criterion->Value_type()) {
			case FactValue::BoolValue: criterion->Value_as<BoolValue>()->Value(); break;
			case FactValue::DoubleValue: criterion->Value_as<DoubleValue>()->Value(); break;
			case FactValue::LongValue: criterion->Value_as<LongValue>()->Value(); break;
			case FactValue::UlongValue: criterion->Value_as<UlongValue>()->Value(); break;
			case FactValue::StringValue: criterion->Value_as<StringValue>()->Value(); break;
			}
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint void ParseRule(Wren::DialogueSet& responses, const FlatBuffers::Rule* const rule) {
			Wren::DialogueSet::RuleBuilderType builder(MallocAllocator(""), responses.GetSymbols());
			const auto                         criteria(rule->Criteria());
			for (uoffset_t index(0u); index < criteria->size(); ++index) {
				ParseCriterion(builder, criteria->Get(index));
			}

			responses.Insert(eastl::move(builder), DialogueResponse(nullptr));
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint ErrorCode ParseDialogueSet(Wren::DialogueSet& responses, const FlatBuffers::DialogueSet* const dialogue) {
			const auto rules(dialogue->Rules());
			for (uoffset_t id(0u); id < rules->size(); ++id) {
				ParseRule(responses, rules->Get(id));
			}

			return Error::None;
		}

	} // anonymous namespace

	DialogueSetAsset::DialogueSetAsset(StringView path) :
		Asset(path) {}

	// ---------------------------------------------------

	ErrorCode DialogueSetAsset::BindResources(const Builder& asset) {
		//	Ensure we're working with data that can plausibly represent a dialogue set.
		Verifier verifier(reinterpret_cast<const uint8_t*>(asset.Begin()), asset.GetSize());
		if (!VerifyDialogueSetBuffer(verifier)) {
			asset.WriteLog(Severity::Error, "Data integrity check failed for {}, aborting load." ET_NEWLINE, GetPath());
			return Error::InvalidParameter;
		}

		Wren::DialogueSet responses;
		ET_ABORT_UNLESS(ParseDialogueSet(responses, GetDialogueSet(asset.Begin())));

		Swap(*this, responses);

		return Error::None;
	}

	// ---------------------------------------------------

	void DialogueSetAsset::FreeResources() {}

	// ---------------------------------------------------

	ETPureFunctionHint StringView DialogueSetAsset::GetExtension() {
		return { DialogueSetExtension(), StringLength(DialogueSetExtension()) };
	}

}}}} // namespace Eldritch2::Scripting::Wren::AssetViews
