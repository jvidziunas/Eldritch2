/*==================================================================*\
  ResolutionScale.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  �2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/ResolutionScale.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics {

	ResolutionScale::ResolutionScale() ETNoexceptHint : _targetTimeMicroseconds(1u),
														_lastTimeMicroseconds(1u),
														_minScale(0.5f),
														_maxScale(1.25f) {
	}

	// ---------------------------------------------------

	void ResolutionScale::ForceIdentityScale() ETNoexceptHint {
		_lastTimeMicroseconds = _targetTimeMicroseconds;
	}

	// ---------------------------------------------------

	void ResolutionScale::ScaleMax(uint32_t& width, uint32_t& height) const ETNoexceptHint {
		float32 tempWidth(AsFloat(width));
		float32 tempHeight(AsFloat(height));

		ScaleMax(tempWidth, tempHeight);

		width  = AsInt(tempHeight);
		height = AsInt(tempHeight);
	}

	// ---------------------------------------------------

	void ResolutionScale::ScaleMax(float32& width, float32& height) const ETNoexceptHint {
		width *= _maxScale;
		height *= _maxScale;
	}

	// ---------------------------------------------------

	void ResolutionScale::Scale(uint32_t& width, uint32_t& height) const ETNoexceptHint {
		float32 tempWidth(AsFloat(width));
		float32 tempHeight(AsFloat(height));

		Scale(tempWidth, tempHeight);

		width  = AsInt(tempHeight);
		height = AsInt(tempHeight);
	}

	// ---------------------------------------------------

	void ResolutionScale::Scale(float32& width, float32& height) const ETNoexceptHint {
		const float32 scale(Clamp(Sqrt(_lastTimeMicroseconds / float32(_targetTimeMicroseconds)), _minScale, _maxScale));

		width *= scale;
		height *= scale;
	}

	// ---------------------------------------------------

	void ResolutionScale::UpdateTime(uint64 timeInMicroseconds) ETNoexceptHint {
		_lastTimeMicroseconds = timeInMicroseconds;
	}

}} // namespace Eldritch2::Graphics
