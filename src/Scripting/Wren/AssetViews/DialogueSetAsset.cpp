/*==================================================================*\
  DialogueSetAsset.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/AssetViews/DialogueSetAsset.hpp>
#include <Flatbuffers/FlatBufferTools.hpp>
#include <Logging/Log.hpp>
//------------------------------------------------------------------//
#include <Flatbuffers/DialogueSet_generated.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scripting { namespace Wren { namespace AssetViews {

	using namespace ::Eldritch2::Scripting::Wren::FlatBuffers;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Core;
	using namespace ::flatbuffers;

	// ---------------------------------------------------

	namespace {

		ETForceInlineHint void ParseCriterion(DialogueSet::RuleBuilderType& rule, const RuleCriterion* criterion) ETNoexceptHint {
			switch (criterion->Value_type()) {
			default: break;
			case FactValue::BoolValue: criterion->Value_as<BoolValue>()->Value(); break;
			case FactValue::DoubleValue: criterion->Value_as<DoubleValue>()->Value(); break;
			case FactValue::LongValue: criterion->Value_as<LongValue>()->Value(); break;
			case FactValue::UlongValue: criterion->Value_as<UlongValue>()->Value(); break;
			case FactValue::StringValue: criterion->Value_as<StringValue>()->Value(); break;
			}
		}

		// ---------------------------------------------------

		ETForceInlineHint Result ParseRule(DialogueSet& responses, const RuleDescriptor* const rule) {
			DialogueSet::RuleBuilderType builder(MallocAllocator(""), responses.GetSymbols());
			const auto                   criteria(rule->Criteria());
			for (uoffset_t index(0u); index < criteria->size(); ++index) {
				ParseCriterion(builder, criteria->Get(index));
			}

			responses.Emplace(Move(builder), DialogueResponse(nullptr));
			return Result::Success;
		}

		// ---------------------------------------------------

		ETForceInlineHint Result ParseDialogueSet(DialogueSet& responses, const DialogueSetDescriptor* const dialogue) {
			const auto rules(dialogue->Rules());
			for (uoffset_t id(0u); id < rules->size(); ++id) {
				ET_ABORT_UNLESS(ParseRule(responses, rules->Get(id)));
			}

			return Result::Success;
		}

	} // anonymous namespace

	DialogueSetAsset::DialogueSetAsset(StringSpan path) ETNoexceptHint : Asset(path) {}

	// ---------------------------------------------------

	Result DialogueSetAsset::BindResources(Log& log, const AssetBuilder& asset) {
		//	Ensure we're working with data that can plausibly represent a dialogue set.
		const auto dialogue(GetVerifiedRoot<DialogueSetDescriptor>(asset.bytes, DialogueSetDescriptorIdentifier()));
		ET_ABORT_UNLESS(dialogue ? Result::Success : Result::InvalidParameter, log.Write(Error, "Data integrity check failed for {}, aborting load." ET_NEWLINE, GetPath()));

		DialogueSet responses;
		ET_ABORT_UNLESS(ParseDialogueSet(responses, dialogue));

		Swap(*this, responses);

		return Result::Success;
	}

	// ---------------------------------------------------

	void DialogueSetAsset::FreeResources() ETNoexceptHint {
		Clear();
	}

	// ---------------------------------------------------

	ETPureFunctionHint StringSpan DialogueSetAsset::GetExtension() ETNoexceptHint {
		return { DialogueSetDescriptorExtension(), StringLength(DialogueSetDescriptorExtension()) };
	}

}}}} // namespace Eldritch2::Scripting::Wren::AssetViews
