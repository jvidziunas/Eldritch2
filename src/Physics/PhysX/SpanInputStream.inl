/*==================================================================*\
  PackageInputStream.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX {

	ETConstexpr ETForceInlineHint SpanInputStream::SpanInputStream(Span<const byte*> bytes) ETNoexceptHint : Span<const byte*>(bytes), _position(0u) {}

}}} // namespace Eldritch2::Physics::PhysX
