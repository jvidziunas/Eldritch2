/*==================================================================*\
  ScopedFPUController.cpp
  ------------------------------------------------------------------
  Purpose:
  Utility class that exploits C++ scoping rules to efficiently and
  deterministically effect the behavior of the floating-point
  arithmetic unit of the executing processor.

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/ScopedFPUController.hpp>
#if( ET_PLATFORM_WINDOWS || ET_PLATFORM_MAC || ET_PLATFORM_LINUX )
#	include <xmmintrin.h>
#endif
//------------------------------------------------------------------//

using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;

namespace {

	enum : unsigned int {
		_MM_DAZ_ON = (1 << 6)
	};

// ---------------------------------------------------

	static ETForceInlineHint bool FlushToZeroEnabled( int controlWord ) {
		return !!( controlWord & _MM_FLUSH_ZERO_ON );
	}

// ---------------------------------------------------

	static ETForceInlineHint bool DenormalsAreZeroEnabled( int controlWord ) {
		return !!( controlWord & _MM_DAZ_ON );
	}

}	// anonymous namespace

namespace Eldritch2 {
namespace Utility {

	ScopedFPUController::ScopedFPUController() : _initialControlWord( ::_mm_getcsr() ), _currentControlWord( _initialControlWord ) {}

// ---------------------------------------------------

	ScopedFPUController::~ScopedFPUController() {
		::_mm_setcsr( _initialControlWord );
	}

// ---------------------------------------------------

	ScopedFPUController& ScopedFPUController::SetDenormalsAreZero( bool state ) {
		if( !DenormalsAreZeroEnabled( _currentControlWord ) && state ) {
			_currentControlWord |= _MM_DAZ_ON;
			::_mm_setcsr( _currentControlWord );
		} else if( DenormalsAreZeroEnabled( _currentControlWord ) && !state ) {
			_currentControlWord &= ~( _MM_DAZ_ON );
			::_mm_setcsr( _currentControlWord );
		}

		return *this;
	}

// ---------------------------------------------------

	ScopedFPUController& ScopedFPUController::SetFlushToZeroEnabled( bool state ) {
		if( !FlushToZeroEnabled( _currentControlWord ) && state ) {
			_currentControlWord |= _MM_FLUSH_ZERO_ON;
			::_mm_setcsr( _currentControlWord );
		} else if( FlushToZeroEnabled( _currentControlWord ) && !state ) {
			_currentControlWord &= ~( _MM_FLUSH_ZERO_ON );
			::_mm_setcsr( _currentControlWord );
		}

		return *this;
	}

}	// namespace Utility
}	// namespace Eldritch2