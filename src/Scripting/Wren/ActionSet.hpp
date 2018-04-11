/*==================================================================*\
  ActionSet.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Input {
		class	InputDevice;
	}
}

struct	WrenHandle;

namespace Eldritch2 {
namespace Scripting {
namespace Wren {

	class ActionSet {
	// - TYPE PUBLISHING ---------------------------------

	public:
		struct Action {
			WrenHandle*	closure;
			int32		weight;
			uint32		flags;
		};

	// ---------------------------------------------------

	public:
	//!	Disable copy construction.
		ActionSet( const ActionSet& ) = delete;
	//!	Constructs this @ref ActionSet instance.
		ActionSet();

		~ActionSet();

	// ---------------------------------------------------

	public:
		bool	DispatchEvents( WrenVM* vm, WrenHandle* receiver, WrenHandle* binaryCallHandle );

	// ---------------------------------------------------

	public:
		bool	TryAcquire( Input::InputDevice& device );

	// ---------------------------------------------------

	public:
		void	BindResources( WrenVM* vm, int bindingsSlot );

		void	FreeResources( WrenVM* vm );

	// ---------------------------------------------------

	//!	Disable copy assignment.
		ActionSet&	operator=( const ActionSet& ) = delete;

	// ---------------------------------------------------

	public:
		ArrayList<Action>	_actions;
	};

}	// namespace Wren
}	// namespace Scripting
}	// namespace Eldritch2