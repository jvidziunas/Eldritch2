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
#include <Utility/Containers/IntrusiveForwardListHook.hpp>
#include <Utility/Containers/IntrusiveMPSCQueue.hpp>
#include <Utility/Memory/ArenaAllocator.hpp>
#include <Platform/UserReadWriteMutex.hpp>
#include <Utility/Containers/HashMap.hpp>
#include <Scripting/MessageBus.hpp>
#include <Utility/Cache.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scheduler {
		class	ThreadScheduler;
	}

	namespace Utility {
		class	UserMutex;
	}
}

class	asIScriptFunction;
class	asIScriptContext;
class	asIScriptObject;
class	asIScriptEngine;
class	asITypeInfo;

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {

	class BufferingMessageBus : public Scripting::MessageBus {
	// - TYPE PUBLISHING ---------------------------------

	public:
		class Message : public Eldritch2::IntrusiveForwardListBaseHook {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref Message instance.
			Message( asIScriptFunction& handler, ObjectId destinationId, ObjectId senderId, Eldritch2::uint64 time, const void* message );

			~Message() = default;

		// ---------------------------------------------------

			void	Dispatch( asIScriptContext& executor, asIScriptObject& target ) const;

		// - DATA MEMBERS ------------------------------------

		private:
			asIScriptFunction&	_handlerFunction;
			ObjectId			_senderId;
			Eldritch2::uint64	_time;
			const void*			_message;
		};

	// ---

		using HandlerCacheKey	= Eldritch2::Pair<const asITypeInfo*, const asITypeInfo*>;

	// ---

		struct HandlerLookupPolicy {
			ETNoAliasHint asIScriptFunction*	operator()( HandlerCacheKey dispatchArguments ) const;
		};
		
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref BufferingMessageBus instance.
		BufferingMessageBus( Eldritch2::Allocator& allocator );
	//!	Disable copying.
		BufferingMessageBus( const BufferingMessageBus& ) = delete;

		~BufferingMessageBus();

	// ---------------------------------------------------

		void	Dispatch( TypeId messageType, ObjectId destinationId, ObjectId senderId, Eldritch2::uint64 time, const void* message ) override;
		void	Dispatch( const asITypeInfo& messageType, ObjectId destinationId, ObjectId senderId, Eldritch2::uint64 time, const asIScriptObject* message );

	// ---------------------------------------------------

		asIScriptObject*	Spawn( asIScriptEngine& scriptEngine, const asITypeInfo& type );

		void				NotifyOfObjectDestruction( asIScriptObject* const object );

	// ---------------------------------------------------

		void	ResetAllocator();

	// - DATA MEMBERS ------------------------------------

	private:
		mutable Eldritch2::ArenaChildAllocator											_messageAllocator;
		const Eldritch2::ArenaChildAllocator::Checkpoint								_messageAllocatorFrameCheckpoint;

	//!	Responsible for protecting @ref _queueDirectory.
		mutable Platform::UserReadWriteMutex											_queueDirectoryMutex;

		Eldritch2::Cache<HandlerCacheKey, asIScriptFunction*, HandlerLookupPolicy>		_handlerCache;
		Eldritch2::HashMap<asIScriptObject*, Eldritch2::IntrusiveMPSCQueue<Message>>	_queueDirectory;
	};

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/AngelScript/BufferingMessageBus.inl>
//------------------------------------------------------------------//