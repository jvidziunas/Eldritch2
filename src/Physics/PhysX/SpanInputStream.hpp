/*==================================================================*\
  PackageInputStream.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <foundation/PxIO.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX {

	class SpanInputStream : public physx::PxInputStream, public Span<const byte*> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref PackageInputStream instance.
		ETConstexpr SpanInputStream(const SpanInputStream&) ETNoexceptHint = default;
		//!	Constructs this @ref PackageInputStream instance.
		ETConstexpr SpanInputStream(Span<const byte*> bytes) ETNoexceptHint;

		~SpanInputStream() override = default;

		// ---------------------------------------------------

	public:
		uint32_t read(void* outBytes, uint32_t byteLength) ETNoexceptHint override;

		// ---------------------------------------------------

	public:
		SpanInputStream& operator=(const SpanInputStream&) ETNoexceptHint = default;

		// - DATA MEMBERS ------------------------------------

	private:
		SizeType _position;
	};

}}} // namespace Eldritch2::Physics::PhysX

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Physics/PhysX/SpanInputStream.inl>
//------------------------------------------------------------------//
