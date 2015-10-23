/*==================================================================*\
  WaitableUserEvent.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Noncopyable.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Utility {

	class ETPureAbstractHint WaitableUserEvent : private Utility::Noncopyable {
	public:
		virtual void	BlockUntilSignaled() abstract;

		virtual void	Signal() abstract;

		virtual void	Reset() abstract;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		//!	Constructs this @ref WaitableUserEvent instance.
		WaitableUserEvent() = default;

		//!	Destroys this @ref WaitableUserEvent instance.
		~WaitableUserEvent() = default;
	};

}	// namespace Utility
}	// namespace Eldritch2