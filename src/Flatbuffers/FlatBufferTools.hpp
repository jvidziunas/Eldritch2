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
class AbstractStringSpan;
class Transformation;
class Vector;

} // namespace Eldritch2

namespace flatbuffers {

template <typename T>
class Vector;

struct Transformation;
struct UnitQuaternion;
struct Quaternion;
struct Float3;
struct Float2;
struct String;

} // namespace flatbuffers

namespace flatbuffers {

template <typename T>
ETPureFunctionHint Eldritch2::Span<const T*> AsSpan(const Vector<T>* vector) ETNoexceptHint;

ETPureFunctionHint Eldritch2::AbstractStringSpan<char> AsString(const String* string) ETNoexceptHint;

ETPureFunctionHint Eldritch2::Transformation ETSimdCall AsTransformation(const Transformation& transform) ETNoexceptHint;

ETPureFunctionHint Eldritch2::Quaternion ETSimdCall AsQuaternion(const UnitQuaternion& quaternion) ETNoexceptHint;
ETPureFunctionHint Eldritch2::Quaternion ETSimdCall AsQuaternion(const Quaternion& quaternion) ETNoexceptHint;

ETPureFunctionHint Eldritch2::Vector ETSimdCall AsVector(const Float3& vector) ETNoexceptHint;

ETPureFunctionHint Eldritch2::Span<const Eldritch2::byte*> AsBytes(const Vector<uint8_t>* bytes) ETNoexceptHint;

template <typename T>
const T* GetVerifiedRoot(Eldritch2::Span<const Eldritch2::byte*> bytes, const char identifier[]) ETNoexceptHint;

} // namespace flatbuffers

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Flatbuffers/FlatbufferTools.inl>
//------------------------------------------------------------------//
