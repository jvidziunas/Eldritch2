/*==================================================================*\
  Message.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/IntrusiveForwardListHook.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {

	class Message : public ::Eldritch2::IntrusiveForwardListBaseHook {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		//!	Constructs this @ref Message instance.
		Message() = default;

	public:
		//!	Destroys this @ref Message instance.
		~Message() = default;
	};

}	// namespace Scripting
}	// namespace Eldritch2


