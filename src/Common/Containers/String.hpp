/*==================================================================*\
  String.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Containers/AbstractString.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

using PlatformString     = AbstractString<PlatformChar, MallocAllocator>;
using String             = AbstractString<Utf8Char, MallocAllocator>;
using PlatformStringView = AbstractStringView<PlatformString::CharacterType>;
using StringView         = AbstractStringView<String::CharacterType>;

} // namespace Eldritch2
