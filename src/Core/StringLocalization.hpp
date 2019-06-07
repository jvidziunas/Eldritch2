/*==================================================================*\
  StringLocalization.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2019 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

#define ET_LOCALIZED_STRING_PREFIX "$str_"
#define ET_LOCALIZED_STRING(numericId) ET_CPP_JOIN(ET_LOCALIZED_STRING_PREFIX, #numericId)

namespace Eldritch2 {

class StringLocalization {
	// - TYPE PUBLISHING ---------------------------------

public:
	using StringId = uint32;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref StringLocalization instance.
	StringLocalization(const StringLocalization&) = default;
	//!	Constructs this @ref StringLocalization instance.
	StringLocalization(StringLocalization&&) ETNoexceptHint = default;
	//!	Constructs this @ref StringLocalization instance.
	StringLocalization() ETNoexceptHint;

	~StringLocalization() = default;

	// ---------------------------------------------------

public:
	StringSpan Localize(StringSpan string) const ETNoexceptHint;

	// ---------------------------------------------------

public:
	Result BindResources(Span<const byte*> tableBytes);

	void FreeResources() ETNoexceptHint;

	// - DATA MEMBERS ------------------------------------

private:
	ArrayList<StringSpan> _strings;
};

} // namespace Eldritch2
