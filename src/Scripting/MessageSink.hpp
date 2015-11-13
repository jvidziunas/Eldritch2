/*==================================================================*\
  MessageSink.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/IntrusiveVyukovMPSCQueue.hpp>
#include <Scripting/Message.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {

	class MessageSink {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		//!	Constructs this @ref MessageSink instance.
		MessageSink() = default;

		//!	Destroys this @ref MessageSink instance.
		~MessageSink() = default;
		
	// - DATA MEMBERS ------------------------------------

		::Eldritch2::IntrusiveVyukovMPSCQueue<Scripting::Message>	_pendingMessages;
	};

}	// namespace Scripting
}	// namespace Eldritch2

