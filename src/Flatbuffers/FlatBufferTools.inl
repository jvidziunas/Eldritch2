/*==================================================================*\
  FlatBufferTools.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Containers/AbstractStringView.hpp>
//------------------------------------------------------------------//
#include <flatbuffers/flatbuffers.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

ETInlineHint AbstractStringView<char> AsString(const flatbuffers::String* string) ETNoexceptHint {
	return string ? AbstractStringView<char>(string->c_str(), string->size()) : AbstractStringView<char>();
}

} // namespace Eldritch2
