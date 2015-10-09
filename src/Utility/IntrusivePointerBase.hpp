/*==================================================================*\
  IntrusivePointerBase.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Compiler.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	struct IntrusivePointerBase {
		struct PassthroughReferenceCountingSemantics {};
		struct DefaultReferenceCountingSemantics {};
	};

// ---------------------------------------------------

	extern const IntrusivePointerBase::PassthroughReferenceCountingSemantics	PassthroughReferenceCountingSemantics;
	extern const IntrusivePointerBase::DefaultReferenceCountingSemantics		DefaultReferenceCountingSemantics;

}	// namespace Eldritch2Detail