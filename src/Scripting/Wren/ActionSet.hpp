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
#include <wren.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {
namespace Wren {

	class ActionSet {
	// - TYPE PUBLISHING ---------------------------------

	public:
		struct Action {
			WrenHandle*	handler;
			int32		weight;
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
		void	Activate( uint32 index, float32 amount );

	// ---------------------------------------------------

	public:
		WrenInterpretResult	Dispatch( WrenVM* vm, WrenHandle* receiver, WrenHandle* binaryCallHandle );

	// ---------------------------------------------------

	public:
		void	BindResources( WrenVM* vm, WrenHandle* actionMap );

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