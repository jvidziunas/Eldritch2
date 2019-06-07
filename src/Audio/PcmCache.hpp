/*==================================================================*\
  PcmCache.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Audio {

	template <typename Sample>
	class PcmCache {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		PcmCache(const PcmCache&) = delete;
		//!	Constructs this @ref PcmCache instance.
		PcmCache() ETNoexceptHint;

		~PcmCache() = default;

		// ---------------------------------------------------

	public:
		bool ShouldDecode(Sample*& outSamples);

		// ---------------------------------------------------

	public:
		Result BindResources(typename ArrayList<Sample>::SizeType cachedGrains, uint32 sampleRateHz, typename ArrayList<Sample>::SizeType grainLength);

		void FreeResources() ETNoexceptHint;

		// - DATA MEMBERS ------------------------------------

	public:
		typename ArrayList<Sample>::SizeType _samplesPerGrain;
		ArrayList<Sample>                    _samples;

		// ---------------------------------------------------

		template <typename Sample2>
		friend void Swap(PcmCache<Sample2>&, PcmCache<Sample2>&);
	};

}} // namespace Eldritch2::Audio

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Audio/PcmCache.inl>
//------------------------------------------------------------------//
