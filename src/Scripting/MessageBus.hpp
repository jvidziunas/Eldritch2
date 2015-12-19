/*==================================================================*\
  MessageBus.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/ArenaAllocator.hpp>
#include <Scripting/Message.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scripting {
		class	Message;
	}
}

namespace Eldritch2 {
namespace Scripting {

	class ETPureAbstractHint MessageBus {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using MessageTarget = void*;

	// ---------------------------------------------------

		template <typename DispatchedMessage>
		void			SendMessage( const MessageTarget target, DispatchedMessage&& message );

	protected:
		virtual void	SendMessage( const MessageTarget target, Scripting::Message& message ) abstract;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//!	Constructs this @ref MessageBus instance.
		/*!	@remarks Designed to be called from within subclasses.
			*/
		MessageBus();

		//!	Destroys this @ref MessageBus instance.
		/*!	@remarks Designed to be called from within subclasses.
			*/
		~MessageBus() = default;

	private:
		::Eldritch2::ExternalArenaAllocator	_messageAllocator;
	};

}	// namespace Scripting
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/MessageBus.inl>
//------------------------------------------------------------------//