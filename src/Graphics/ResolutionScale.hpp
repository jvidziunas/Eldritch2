/*==================================================================*\
  ResolutionScale.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics {

	class ResolutionScale {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref ResolutionScale instance.
		ResolutionScale(const ResolutionScale&) ETNoexceptHint = default;
		//!	Constructs this @ref ResolutionScale instance.
		ResolutionScale() ETNoexceptHint;

		~ResolutionScale() = default;

		// ---------------------------------------------------

	public:
		void ForceIdentityScale() ETNoexceptHint;

		void UpdateTime(uint64 timeInMicroseconds) ETNoexceptHint;

		// ---------------------------------------------------

	public:
		void ScaleMax(uint32_t& width, uint32_t& height) const ETNoexceptHint;
		void ScaleMax(float32& width, float32& height) const ETNoexceptHint;

		void Scale(uint32_t& width, uint32_t& height) const ETNoexceptHint;
		void Scale(float32& width, float32& height) const ETNoexceptHint;

		// - DATA MEMBERS ------------------------------------

	private:
		uint64  _targetTimeMicroseconds;
		uint64  _lastTimeMicroseconds;
		float32 _minScale;
		float32 _maxScale;
	};

}} // namespace Eldritch2::Graphics
