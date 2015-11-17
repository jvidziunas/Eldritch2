/*==================================================================*\
  MessageSink.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Compiler.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {

	class MessageSink {
	public:
		void	SendMessage();
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		//!	Constructs this @ref MessageSink instance.
		MessageSink() = default;

		//!	Destroys this @ref MessageSink instance.
		~MessageSink() = default;
	};

}	// namespace Scripting
}	// namespace Eldritch2

