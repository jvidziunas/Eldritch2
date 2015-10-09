/*==================================================================*\
  ScopedFPUController.hpp
  ------------------------------------------------------------------
  Purpose:
  Utility class that exploits C++ scoping rules to efficiently and
  deterministically effect the behavior of the floating-point
  arithmetic unit of the executing processor.

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

	class ScopedFPUController : private Utility::Noncopyable {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		// Constructs this ScopedFPUController instance.
		explicit ScopedFPUController();

		// Destroys this ScopedFPUController instance.
		~ScopedFPUController();

	// ---------------------------------------------------

		Utility::ScopedFPUController&	SetDenormalsAreZero( bool state = true );

	// ---------------------------------------------------

		Utility::ScopedFPUController&	SetFlushToZeroEnabled( bool state = true );

	// - DATA MEMBERS ------------------------------------

	private:
		const int	_initialControlWord;
		int			_currentControlWord;
	};

}	// namespace Utility
}	// namespace Eldritch2