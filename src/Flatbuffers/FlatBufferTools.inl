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
#include <FlatBuffers/Eldritch2Types_generated.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

ETInlineHint ETForceInlineHint AbstractStringView<char> AsString(const flatbuffers::String* string) ETNoexceptHint {
	return string ? AbstractStringView<char>(string->c_str(), string->size()) : AbstractStringView<char>();
}

ETInlineHint ETForceInlineHint Transformation ETSimdCall AsTransformation(const FlatBuffers::Transformation& transform) ETNoexceptHint {
	return { AsVector(transform.Translation()), AsQuaternion(transform.Orientation()) };
}

ETInlineHint ETForceInlineHint Quaternion ETSimdCall AsQuaternion(const FlatBuffers::CompressedUnitQuaternion& quaternion) ETNoexceptHint {
	return Quaternion(quaternion.Bits());
}

ETInlineHint ETForceInlineHint Quaternion ETSimdCall AsQuaternion(const FlatBuffers::Quaternion& quaternion) ETNoexceptHint {
	return { quaternion.I(), quaternion.J(), quaternion.K(), quaternion.W() };
}

ETInlineHint ETForceInlineHint Vector AsVector(const FlatBuffers::Float3& vector) ETNoexceptHint {
	return { vector.X(), vector.Y(), vector.Z(), 0.0f };
}

} // namespace Eldritch2
