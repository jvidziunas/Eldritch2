/*==================================================================*\
  UserSemaphore.hpp
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

	class ETPureAbstractHint UserSemaphore : private Utility::Noncopyable {
	public:
		virtual int		IncreaseCount( const int count = 1u ) abstract;

		virtual void	Acquire() abstract;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		//!	Constructs this @ref UserSemaphore instance.
		UserSemaphore() = default;

	public:
		//!	Destroys this @ref UserSemaphore instance.
		virtual ~UserSemaphore() = default;
	};

}	// namespace Utility
}	// namespace Eldritch2