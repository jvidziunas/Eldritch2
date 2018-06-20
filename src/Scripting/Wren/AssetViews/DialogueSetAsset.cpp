/*==================================================================*\
  DialogueAsset.cpp
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
//------------------------------------------------------------------//
#include <Flatbuffers/DialogueSet_generated.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scripting { namespace Wren { namespace AssetViews {

	using namespace ::Eldritch2::Scripting::Wren::FlatBuffers;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Assets;
	using namespace ::flatbuffers;

	namespace {

		template <typename RuleBuilder>
		ETInlineHint void ParseCriterion(RuleBuilder& result, const FlatBuffers::Criterion* criterion) {
			switch (criterion->Value_type()) {
			default:
			case FlatBuffers::FactValue::NONE: break;
			case FlatBuffers::FactValue::BoolValue: criterion->Value_as<BoolValue>(); break;
			case FlatBuffers::FactValue::DoubleValue: criterion->Value_as<DoubleValue>(); break;
			case FlatBuffers::FactValue::LongValue: criterion->Value_as<LongValue>(); break;
			case FlatBuffers::FactValue::UlongValue: criterion->Value_as<UlongValue>(); break;
			case FlatBuffers::FactValue::StringValue: criterion->Value_as<StringValue>(); break;
			}
		}

		// ---------------------------------------------------

		template <typename RuleSet>
		ETInlineHint void ParseRule(RuleSet& responses, const FlatBuffers::Rule* const rule) {
			typename RuleSet::RuleBuilderType builder(MallocAllocator(""), responses.GetSymbols());

			const auto criteria(rule->Criteria());

			for (uoffset_t index(0u); index < criteria->size(); ++index) {
				ParseCriterion(builder, criteria->Get(index));
			}

			responses.Insert(eastl::move(builder), DialogueResponse(nullptr));
		}

		// ---------------------------------------------------

		template <typename Response, class Allocator>
		ETInlineHint ErrorCode ParseDialogueSet(RuleSet<Response, Allocator>& responses, const FlatBuffers::DialogueSet* const dialogue) {
			const auto rules(dialogue->Rules());

			for (uoffset_t id(0u); id < rules->size(); ++id) {
				ParseRule(responses, rules->Get(id));
			}

			return Error::None;
		}

	} // anonymous namespace

	DialogueSetAsset::DialogueSetAsset(const Utf8Char* const filePath) :
		Asset(filePath) {}

	// ---------------------------------------------------

	ETPureFunctionHint Utf8Literal DialogueSetAsset::GetExtension() {
		return Utf8Literal(DialogueSetExtension());
	}

	// ---------------------------------------------------

	ErrorCode DialogueSetAsset::BindResources(const Builder& asset) {
		//	Ensure we're working with data that can plausibly represent a dialogue set.
		Verifier verifier(reinterpret_cast<const uint8_t*>(asset.Begin()), asset.GetSize());
		if (!VerifyDialogueSetBuffer(verifier)) {
			asset.WriteLog(MessageType::Error, "Data integrity check failed for {}, aborting load." UTF8_NEWLINE, GetPath());
			return Error::InvalidParameter;
		}

		Wren::DialogueSet responses;

		ET_FAIL_UNLESS(ParseDialogueSet(responses, GetDialogueSet(asset.Begin())));

		return Error::None;
	}

	// ---------------------------------------------------

	void DialogueSetAsset::FreeResources() {}

}}}} // namespace Eldritch2::Scripting::Wren::AssetViews
