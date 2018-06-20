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
		ResolutionScale(const ResolutionScale&) = default;
		//!	Constructs this @ref ResolutionScale instance.
		ResolutionScale();

		~ResolutionScale() = default;

		// ---------------------------------------------------

	public:
		void ForceIdentityScale();

		void UpdateTime(uint32 timeInMicroseconds);

		// ---------------------------------------------------

	public:
		void ScaleMax(uint32_t& width, uint32_t& height) const;
		void ScaleMax(float32& width, float32& height) const;

		void Scale(uint32_t& width, uint32_t& height) const;
		void Scale(float32& width, float32& height) const;

		// - DATA MEMBERS ------------------------------------

	private:
		uint32  _targetTimeMicroseconds;
		uint32  _lastTimeMicroseconds;
		float32 _minScale;
		float32 _maxScale;
	};

}} // namespace Eldritch2::Graphics
