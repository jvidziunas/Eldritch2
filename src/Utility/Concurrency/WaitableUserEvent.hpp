/*==================================================================*\
  WaitableUserEvent.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Noncopyable.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Utility {

	namespace Utility	= ::Eldritch2::Utility;

// ---------------------------------------------------

	class ETPureAbstractHint WaitableUserEvent : private Utility::Noncopyable {
	public:
		virtual void	BlockUntilSignaled() abstract;

		virtual void	Signal() abstract;

		virtual void	Reset() abstract;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		// Constructs this WaitableUserEvent instance.
		WaitableUserEvent();

		// Destroys this WaitableUserEvent instance.
		~WaitableUserEvent();
	};

}	// namespace Utility
}	// namespace Eldritch2