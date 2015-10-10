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

	namespace Utility	= ::Eldritch2::Utility;

// ---------------------------------------------------

	class ETPureAbstractHint UserSemaphore : private Utility::Noncopyable {
	public:
		virtual size_t	IncreaseCount( const size_t count = 1u ) abstract;

		virtual void	Acquire() abstract;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		// Constructs this UserSemaphore instance.
		UserSemaphore();

	public:
		// Destroys this UserSemaphore instance.
		virtual ~UserSemaphore();
	};

}	// namespace Utility
}	// namespace Eldritch2