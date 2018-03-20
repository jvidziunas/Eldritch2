/*==================================================================*\
  Xaudio2AudioScene.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Audio/XAudio2/XAudio2AudioScene.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Audio {
namespace XAudio2 {

	XAudio2AudioScene::XAudio2AudioScene() : _listeners( MallocAllocator( "XAudio2 Scene Listener Collection Allocator" ) ) {}

// ---------------------------------------------------

	void XAudio2AudioScene::AddListener( XAudio2Listener& listener ) {
		_listeners.Append( &listener );
	}

// ---------------------------------------------------

	void XAudio2AudioScene::RemoveListener( XAudio2Listener& listener ) {
		_listeners.Erase( _listeners.Find( &listener ), UnorderedSemantics() );
	}

}	// namespace XAudio2
}	// namespace Audio
}	// namespace Eldritch2