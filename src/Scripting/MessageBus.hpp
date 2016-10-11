/*==================================================================*\
  MessageBus.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/ScriptTypeToken.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {

	class MessageBus {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using ObjectId	= void*;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref MessageBus instance.
		MessageBus() = default;

		~MessageBus() = default;

	// ---------------------------------------------------

	protected:
		virtual void	Dispatch( RawTypeToken messageType, ObjectId destinationId, ObjectId senderId, Eldritch2::uint64 time, const void* message ) abstract;

	public:
		template <typename Message>
		void			Dispatch( ScriptTypeToken<Message> typeToken, ObjectId destinationId, ObjectId senderId, Eldritch2::uint64 time, const Message& message );
	};

}	// namespace Scripting
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/MessageBus.inl>
//------------------------------------------------------------------//