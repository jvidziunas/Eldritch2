/*==================================================================*\
  ResolutionScale.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/ResolutionScale.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {

	ResolutionScale::ResolutionScale(
	) : _targetTimeMicroseconds( 1u ),
		_lastTimeMicroseconds( 1u ),
		_minScale( 0.75f ),
		_maxScale( 1.25f ) {}

// ---------------------------------------------------

	void ResolutionScale::ForceIdentityScale() {
		_lastTimeMicroseconds = _targetTimeMicroseconds;
	}

// ---------------------------------------------------

	void ResolutionScale::Scale( float32& width, float32& height ) const {
		const float32	scale( Clamp( Sqrt( _lastTimeMicroseconds / float32( _targetTimeMicroseconds ) ), _minScale, _maxScale ) );

		width	*= scale;
		height	*= scale;
	}

// ---------------------------------------------------

	void ResolutionScale::UpdateTime( uint32 timeInMicroseconds ) {
		_lastTimeMicroseconds = timeInMicroseconds;
	}

}	// namespace Graphics
}	// namespace Eldritch2