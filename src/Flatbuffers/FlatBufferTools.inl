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
#include <Common/Containers/AbstractStringSpan.hpp>
#include <Common/Mpl/Vector.hpp>
//------------------------------------------------------------------//
#include <flatbuffers/flatbuffers.h>
//------------------------------------------------------------------//
#include <FlatBuffers/Eldritch2Types_generated.h>
//------------------------------------------------------------------//

namespace flatbuffers {

template <typename T>
ETInlineHint ETForceInlineHint ETPureFunctionHint Eldritch2::Span<const T*> AsSpan(const Vector<T>* vector) ETNoexceptHint {
	return vector ? Eldritch2::Span<const T*>(vector->data(), vector->data() + vector->size());
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint ETPureFunctionHint Eldritch2::AbstractStringSpan<char> AsString(const String* string) ETNoexceptHint {
	return string ? Eldritch2::AbstractStringSpan<char>(string->c_str(), string->size()) : Eldritch2::AbstractStringSpan<char>();
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint ETPureFunctionHint Eldritch2::Span<const Eldritch2::byte*> AsBytes(const Vector<uint8_t>* bytes) ETNoexceptHint {
	return bytes ? Eldritch2::Span<const Eldritch2::byte*>(bytes->data(), bytes->data() + bytes->size()) : Eldritch2::Span<const Eldritch2::byte*>();
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint ETPureFunctionHint Eldritch2::Transformation ETSimdCall AsTransformation(const Transformation& transform) ETNoexceptHint {
	return Eldritch2::Transformation(AsVector(transform.Translation()), AsQuaternion(transform.Orientation()));
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint ETPureFunctionHint Eldritch2::Quaternion ETSimdCall AsQuaternion(const UnitQuaternion& quaternion) ETNoexceptHint {
	return Eldritch2::Quaternion(quaternion.Bits());
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint ETPureFunctionHint Eldritch2::Quaternion ETSimdCall AsQuaternion(const Quaternion& quaternion) ETNoexceptHint {
	return Eldritch2::Quaternion(quaternion.I(), quaternion.J(), quaternion.K(), quaternion.W());
}

// ---------------------------------------------------

ETInlineHint ETForceInlineHint ETPureFunctionHint Eldritch2::Vector ETSimdCall AsVector(const Float3& vector) ETNoexceptHint {
	return Eldritch2::Vector(vector.X(), vector.Y(), vector.Z(), 0.0f);
}

// ---------------------------------------------------

template <typename T>
ETInlineHint ETForceInlineHint const T* GetVerifiedRoot(Eldritch2::Span<const Eldritch2::byte*> bytes, const char identifier[]) ETNoexceptHint {
	return Verifier(bytes.Begin(), bytes.GetSize()).VerifySizePrefixedBuffer<T>(identifier) ? GetSizePrefixedRoot<T>(bytes.Begin()) : nullptr;
}

} // namespace flatbuffers
