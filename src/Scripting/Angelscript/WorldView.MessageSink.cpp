/*==================================================================*\
  WorldView.MessageSink.cpp
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

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {

	void WorldView::ScriptObjectDeleter::operator()( ::asIScriptObject* const object ) {
		object->GetEngine()->ReleaseScriptObject( object, object->GetObjectType() );
	}

// ---------------------------------------------------

	void WorldView::MessageSink::Drain( ::asIScriptObject& scriptObject, ::asIScriptContext& scriptContext ) {
		const auto	objectType( scriptObject.GetObjectType() );
		char		functionDeclaration[96u];

		_pendingMessages.ClearAndDispose( [&] ( Scripting::Message& message ) {
			const auto	function( objectType->GetMethodByDecl( PrintFormatted( functionDeclaration, "void AcceptMessage( const %s&in )", "Message" ) ) );

			if( function && ::asSUCCESS == scriptContext.Prepare( function ) ) {
				scriptContext.SetArgObject( 0u, &message );
				scriptContext.SetObject( &scriptObject );

				scriptContext.Execute();
			}
		} );
	}

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2