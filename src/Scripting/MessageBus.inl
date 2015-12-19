/*==================================================================*\
  MessageBus.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/InstanceNew.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {

	template <typename DispatchedMessage>
	void MessageBus::SendMessage( const MessageTarget target, DispatchedMessage&& message ) {
		class MessageWrapper : public Scripting::Message {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			ETForceInlineHint MessageWrapper( DispatchedMessage&& message ) : Scripting::Message(), _message( ::std::move( message ) ) {}

			MessageWrapper() = default;

		// - DATA MEMBERS ------------------------------------

		private:
			DispatchedMessage	_message;
		};

	// ---

		if( const auto wrappedMessage = new(allocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION) MessageWrapper( ::std::move( message ) ) ) {
			SendMessage( target, *static_cast<Scripting::Message*>(wrappedMessage) );
		}
	}

}	// namespace Scripting
}	// namespace Eldritch2