/*==================================================================*\
  UserWaitableEvent.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Mpl/Compiler.hpp>
#include <Utility/Mpl/Platform.hpp>
//------------------------------------------------------------------//

#if ET_PLATFORM_WINDOWS
using HANDLE = void*;
#endif

namespace Eldritch2 {
namespace Platform {

	class UserWaitableEvent {
	// - TYPE PUBLISHING ---------------------------------

	public:
		enum State {
			Unsignaled,	//!< An outside source must manually invoke @ref Signal() on this @ref UserWaitableEvent instance before threads waiting on it will wake up.
			Signaled	//!< The first thread to block on this @ref UserWaitableEvent will immediately wake.
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref UserWaitableEvent instance.
		UserWaitableEvent( State initialState );

		~UserWaitableEvent();

	// ---------------------------------------------------

	public:
		void	AwaitSignal() const;

		bool	IsSignaled() const;

		void	Signal();

	// - DATA MEMBERS ------------------------------------

	private:
#	if ET_PLATFORM_WINDOWS
		HANDLE	_event;
#	endif
	};

}	// namespace Platform
}	// namespace Eldritch2