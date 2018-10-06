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
namespace FlatBuffers {
	struct CompressedUnitQuaternion;
	struct Transformation;
	struct Quaternion;
	struct Float3;
	struct Float2;
} // namespace FlatBuffers

template <typename Character>
class AbstractStringView;
class Transformation;
class Vector;
} // namespace Eldritch2

namespace flatbuffers {
struct String;

template <typename T>
class Vector;

} // namespace flatbuffers

namespace Eldritch2 {

AbstractStringView<char> AsString(const flatbuffers::String* string) ETNoexceptHint;
Transformation ETSimdCall AsTransformation(const FlatBuffers::Transformation& transform) ETNoexceptHint;
Quaternion ETSimdCall AsQuaternion(const FlatBuffers::CompressedUnitQuaternion& quaternion) ETNoexceptHint;
Quaternion ETSimdCall AsQuaternion(const FlatBuffers::Quaternion& quaternion) ETNoexceptHint;
Vector ETSimdCall AsVector(const FlatBuffers::Float3& vector) ETNoexceptHint;

template <typename Type>
using FlatbufferVector = flatbuffers::Vector<Type>;

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Flatbuffers/FlatbufferTools.inl>
//------------------------------------------------------------------//
