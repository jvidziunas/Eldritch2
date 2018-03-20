/*==================================================================*\
  XAudio2Listener.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Audio/XAudio2/XAudio2AudioScene.hpp>
#include <Audio/XAudio2/Xaudio2Listener.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Scripting/Wren/Context.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Audio {
namespace XAudio2 {

	using namespace ::Eldritch2::Scripting::Wren;

	ET_IMPLEMENT_WREN_CLASS( XAudio2Listener ) {
		api.CreateClass<XAudio2Listener>( ET_BUILTIN_WREN_MODULE_NAME( Audio ), "AudioListener",
			{/* Constructors */
				DefineConstructor<XAudio2Listener ( XAudio2AudioScene, Transformation )>( "new", [] ( WrenVM* vm ) {
					SetReturn<XAudio2Listener>( vm, GetSlotAs<Transformation>( vm, 2 ) );
				} )
			},
			{/*	Properties */
				DefineGetter<Transformation>( "localToWorld", [] ( WrenVM* vm ) {
					const XAudio2Listener&	self( GetSlotAs<XAudio2Listener>( vm, 0 ) );

					wrenSetSlotHandle( vm, 0, AsContext( vm ).GetForeignClass<Transformation>() );
					SetReturn<Transformation>( vm, self.GetLocalToWorld() );
				} ),
				DefineGetter<Transformation>( "worldToLocal", [] ( WrenVM* vm ) {
					const XAudio2Listener&	self( GetSlotAs<XAudio2Listener>( vm, 0 ) );

					wrenSetSlotHandle( vm, 0, AsContext( vm ).GetForeignClass<Transformation>() );
					SetReturn<Transformation>( vm, self.GetWorldToLocal() );
				} )
			},
			{/*	Methods */},
			{/*	Operators */}
		);
	}

}	// namespace XAudio2
}	// namespace Audio
}	// namespace Eldritch2