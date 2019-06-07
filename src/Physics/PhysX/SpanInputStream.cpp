/*==================================================================*\
  PackageInputStream.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/SpanInputStream.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX {

	uint32_t SpanInputStream::read(void* outBytes, uint32_t byteLength) ETNoexceptHint {
		const SliceType slice(Slice(Exchange(_position, _position + byteLength), byteLength));
		return eastl::distance(static_cast<byte*>(outBytes), Copy(slice.Begin(), slice.End(), static_cast<byte*>(outBytes)));
	}

}}} // namespace Eldritch2::Physics::PhysX
