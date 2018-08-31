/*==================================================================*\
  FlatBufferTools.hpp
  ------------------------------------------------------------------
  Purpose:
  
  
  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
template <typename Character>
class AbstractStringView;
}

namespace flatbuffers {
struct String;

template <typename T>
class Vector;

} // namespace flatbuffers

namespace Eldritch2 {

AbstractStringView<char> AsString(const flatbuffers::String* string) ETNoexceptHint;

template <typename Type>
using FlatbufferVector = flatbuffers::Vector<Type>;

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Flatbuffers/FlatbufferTools.inl>
//------------------------------------------------------------------//
