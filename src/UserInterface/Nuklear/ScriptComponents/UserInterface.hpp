/*==================================================================*\
  UserInterface.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <UserInterface/Nuklear/Context.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace UserInterface {
		namespace Nuklear {
			namespace AssetViews {
				class	UserInterfaceView;
			}
		}
	}

	namespace Input {
		class	Keyboard;
		class	Mouse;
	}
}

namespace Eldritch2 {
namespace UserInterface {
namespace Nuklear {
namespace ScriptComponents {

	class UserInterface {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref UserInterface instance.
		UserInterface( const AssetViews::UserInterfaceView& asset );
	//!	Disable copying.
		UserInterface( const UserInterface& ) = delete;

		~UserInterface();

	// ---------------------------------------------------

	public:
		void	ConsumeInput( const Input::Mouse& mouse, const Input::Keyboard& keyboard );

	// - DATA MEMBERS ------------------------------------

	private:
		const AssetViews::UserInterfaceView*	_asset;
		Nuklear::Context						context;
	};

}	// namespace ScriptComponents
}	// namespace Nuklear
}	// namespace UserInterface
}	// namespace Eldritch2

