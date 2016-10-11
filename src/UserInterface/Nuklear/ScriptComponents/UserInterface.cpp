/*==================================================================*\
  UserInterface.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <UserInterface/Nuklear/ScriptComponents/UserInterface.hpp>
#include <UserInterface/Nuklear/AssetViews/UserInterfaceView.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace UserInterface {
namespace Nuklear {
namespace ScriptComponents {

	UserInterface::UserInterface( const AssetViews::UserInterfaceView& asset ) : _asset( &asset ) {}

// ---------------------------------------------------

	UserInterface::~UserInterface() {}

// ---------------------------------------------------

	void UserInterface::ConsumeInput( const Input::Mouse& /*mouse*/, const Input::Keyboard& /*keyboard*/ ) {
		_asset->Draw( context );
	}

}	// namespace ScriptComponents
}	// namespace Nuklear
}	// namespace UserInterface
}	// namespace Eldritch2



