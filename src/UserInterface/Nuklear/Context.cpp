/*==================================================================*\
  Context.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#define NK_IMPLEMENTATION
#define NK_MEMSET Eldritch2::SetMemory
#define NK_MEMCPY Eldritch2::CopyMemory
#define NK_SQRT Eldritch2::Sqrt
#define NK_SIN Eldritch2::FastSine
#define NK_COS Eldritch2::FastCosine
#include <Utility/Memory/StandardLibrary.hpp>
#include <Utility/Math/StandardLibrary.hpp>
#include <UserInterface/Nuklear/Context.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace UserInterface {
namespace Nuklear {

	Context::Context() {
		nk_buffer_init_fixed( &_commandPool, nullptr, 0 );
		nk_init_custom( &_context, nullptr, &_commandPool, nullptr );
	}

// ---------------------------------------------------

	Context::~Context() {
		// nk_de
	}

}	// namespace Nuklear
}	// namespace UserInterface
}	// namespace Eldritch2

