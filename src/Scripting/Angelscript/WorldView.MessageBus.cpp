/*==================================================================*\
  WorldView.MessageBus.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Angelscript/WorldView.hpp>
#include <Utility/Memory/StandardLibrary.hpp>
//------------------------------------------------------------------//
#include <angelscript.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {

	void WorldView::MessageBus::DispatchQueuedMessages( HandlerCache& cache, ::asIScriptObject& scriptObject, ::asIScriptContext& scriptContext ) {
		const auto	objectType( scriptObject.GetObjectType() );
		char		functionDeclaration[96u];

#if 0
		_pendingMessages.ClearAndDispose( [&] ( Scripting::Message& message ) {
			::asIScriptFunction*	handlerFunction;
			const auto				cacheCandidate( cache.Find( { objectType, message.GetTypeName() } ) );

			if( cacheCandidate != cache.End() ) {
				handlerFunction = cacheCandidate->second;
			} else {
				handlerFunction = objectType->GetMethodByDecl( PrintFormatted( functionDeclaration, "void AcceptMessage( const %s&in )", message.GetTypeName() ) );
				cache.Insert( { { objectType, message.GetTypeName() }, handlerFunction } );
			}

			if( handlerFunction && ::asSUCCESS == scriptContext.Prepare( handlerFunction ) ) {
				scriptContext.SetArgObject( 0u, message.GetPayload() );
				scriptContext.SetObject( &scriptObject );

				scriptContext.Execute();
			}

			message.~Message();
		} );
#endif
	}

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2