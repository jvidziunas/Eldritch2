/*==================================================================*\
  Interface.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#define NK_IMPLEMENTATION
#if defined(CopyMemory)
#	undef CopyMemory
#endif // defined(CopyMemory)
#define NK_MEMSET ::Eldritch2::SetMemory
#define NK_MEMCPY ::Eldritch2::CopyMemory
#define NK_SQRT ::Eldritch2::Sqrt
#define NK_SIN ::Eldritch2::FastSine
#define NK_COS ::Eldritch2::FastCosine
#include <Scripting/Wren/Interface.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scripting { namespace Wren {

	Interface::Interface() {}

	// ---------------------------------------------------

	Interface::~Interface() {
		FreeResources();
	}

	// ---------------------------------------------------

	ErrorCode Interface::BindResources() {
		nk_buffer_init_fixed(ETAddressOf(_commandPool), /*m =*/nullptr, /*size =*/0);
		nk_init_custom(ETAddressOf(_context), /*cmds =*/nullptr, ETAddressOf(_commandPool), /*font =*/nullptr);
	}

	// ---------------------------------------------------

	void Interface::FreeResources() {
		nk_clear(ETAddressOf(_context));
	}

}}} // namespace Eldritch2::Scripting::Wren
