/*==================================================================*\
  StringLocalization.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2019 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Flatbuffers/Localization_generated.h>
#include <Flatbuffers/FlatBufferTools.hpp>
#include <Core/StringLocalization.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

StringLocalization::StringLocalization() ETNoexceptHint : _strings(MallocAllocator("String Localization Table String List Allocator")) {}

// ---------------------------------------------------

StringSpan StringLocalization::Localize(StringSpan string) const ETNoexceptHint {
	StringId id;
	if (string.RemovePrefix(ET_LOCALIZED_STRING_PREFIX) && string.Consume(id) && id < _strings.GetSize()) {
		return _strings[id];
	}

	return string;
}

// ---------------------------------------------------

Result StringLocalization::BindResources(Span<const byte*> tableBytes) {
	using namespace ::flatbuffers;

	// FIXME: Should this be an INI?
	const auto localization(GetVerifiedRoot<LocalizationDescriptor>(tableBytes, LocalizationDescriptorIdentifier()));
	ET_ABORT_UNLESS(localization ? Result::Success : Result::InvalidParameter);

	ArrayList<StringSpan> strings(_strings.GetAllocator());
	const auto            entries(localization->Strings()->size());
	const auto            largestEntry(localization->Strings()->Get(entries)->Id());

	strings.Resize(largestEntry, String(String::AllocatorType("Localization Table String Allocator")));
	for (uoffset_t entry(0u); entry < entries; entry++) {
		const auto string(localization->Strings()->Get(entry));
		const auto id(string->Id());

		if (id < strings.GetSize()) {
			strings[id] = AsString(string->String());
		}
	}

	Swap(_strings, strings);

	return Result::Success;
}

// ---------------------------------------------------

void StringLocalization::FreeResources() ETNoexceptHint {
	_strings.Clear();
}

} // namespace Eldritch2
