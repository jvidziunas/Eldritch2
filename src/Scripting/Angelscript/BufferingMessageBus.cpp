/*==================================================================*\
  MessageBus.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Angelscript/BufferingMessageBus.hpp>
#include <Utility/Memory/StandardLibrary.hpp>
//------------------------------------------------------------------//
#include <angelscript.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2::Platform;

#ifdef SendMessage
#	undef SendMessage
#endif

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {

	void BufferingMessageBus::Message::Dispatch( asIScriptContext& context, asIScriptObject& object ) const {
		static_assert( sizeof(asQWORD) == sizeof(_senderId), "asQWORD and ObjectId are not the same size!" );

	// ---

		if( asERetCodes::asSUCCESS != context.Prepare( &_handlerFunction ) ) {
			return;
		}

		context.SetObject( &object );

		context.SetArgQWord( 0, reinterpret_cast<asQWORD>(_senderId) );
		context.SetArgQWord( 1, _time );
		context.SetArgObject( 2, const_cast<void*>(_message) );

		context.Execute();
	}

// ---------------------------------------------------

	ETNoAliasHint asIScriptFunction* BufferingMessageBus::HandlerLookupPolicy::operator()( HandlerCacheKey dispatchArguments ) const {
		for( asUINT methodIndex( 0u ); methodIndex < dispatchArguments.first->GetMethodCount(); ++methodIndex ) {
			const auto	method( dispatchArguments.first->GetMethodByIndex( methodIndex ) );
			int			messageTypeId;

			if( !EqualityCompareString( "", method->GetName() ) || asSUCCESS != method->GetParam( 2u, &messageTypeId ) || dispatchArguments.second->GetTypeId() != messageTypeId ) {
				continue;
			}

			return method;
		}

		return nullptr;
	}

// ---------------------------------------------------

	BufferingMessageBus::BufferingMessageBus( Allocator& allocator ) : _messageAllocator( allocator, 16384u, AllocationDuration::Normal, "Angelscript Message Bus Message Allocator" ),
																	   _messageAllocatorFrameCheckpoint( _messageAllocator.CreateCheckpoint() ),
																	   _queueDirectoryMutex(),
																	   _queueDirectory( { allocator, "Angelscript Message Bus Object Bucket Allocator" } ),
																	   _handlerCache( { allocator, "Angelscript Handler Cache Bucket Allocator" } ) {}

// ---------------------------------------------------

	BufferingMessageBus::~BufferingMessageBus() {}

// ---------------------------------------------------

	void BufferingMessageBus::Dispatch( TypeId messageType, ObjectId destinationId, ObjectId senderId, uint64 time, const void* message ) {
		const auto candidate( _queueDirectory.Find( static_cast<asIScriptObject*>(destinationId) ) );

		if( candidate == _queueDirectory.End() ) {
			return;
		}

		auto	handler( _handlerCache.Find( { static_cast<asIScriptObject*>( destinationId )->GetObjectType(), reinterpret_cast<const asITypeInfo*>(messageType) } ) );

		if( handler == nullptr ) {
			return;
		}
	}

// ---------------------------------------------------

	void BufferingMessageBus::Dispatch( const asITypeInfo& messageType, ObjectId destinationId, ObjectId senderId, uint64 time, const asIScriptObject* message ) {
		Dispatch( reinterpret_cast<TypeId>( &messageType ), destinationId, senderId, time, static_cast<const void*>(message) );
	}

// ---------------------------------------------------

	asIScriptObject* BufferingMessageBus::Spawn( asIScriptEngine& scriptEngine, const asITypeInfo& type ) {
		auto	result( static_cast<asIScriptObject*>(scriptEngine.CreateScriptObject( &type )) );

		if( !result ) {
			return nullptr;
		}

		{	ScopedWriteLock	_( _queueDirectoryMutex );
			_queueDirectory.Emplace( eastl::move( result ), IntrusiveMPSCQueue<Message>() );
		}

		return result;
	}

// ---------------------------------------------------

	void BufferingMessageBus::ResetAllocator() {
		_messageAllocator.RestoreCheckpoint( _messageAllocatorFrameCheckpoint );
	}

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2