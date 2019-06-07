/*==================================================================*\
  Listener.hpp
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

namespace Eldritch2 { namespace Audio {

	/*	Volume Reference Levels:
		  0dB | Minimum human-perceivable volume
		 30dB | Library, 6'/2m distance attenuation
		 60dB | Normal conversation, 3'/1m distance attenuation
		 90dB | City traffic
		100dB | Small combustion engine (motorcycle, snowmobile)
		115dB | Loud concert
		140dB | Jet engine, 100'/33m distance attenuation
		144dB | .22 rifle discharge
		165dB | 12g shotgun discharge
		172dB | .375 revolver discharge
		194dB | Loudest possible sound */
	class DiegeticVoice {
		// - TYPE PUBLISHING ---------------------------------

	public:
		enum Priority {
			None,
			Low,
			Medium,
			High,
			Special
		};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref DiegeticVoice instance.
		DiegeticVoice(Transformation localToWorld, MicrosecondTime startTime, float32 loudnessDb) ETNoexceptHint;
		//!	Constructs this @ref DiegeticVoice instance.
		DiegeticVoice(const DiegeticVoice&) = default;

		~DiegeticVoice() = default;

		// - DATA MEMBERS ------------------------------------

	public:
		Transformation  localToWorld;
		float32         loudnessDb;
		MicrosecondTime startTime;
	};

	// ---

	template <typename Voice>
	class Listener {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using VoiceList = ArrayList<Voice>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref Listener instance.
		Listener(Transformation localToWorld, Vector linearVelocity) ETNoexceptHint;
		//!	Disable copy construction.
		Listener(const Listener&) = delete;

		~Listener() = default;

		// ---------------------------------------------------

	public:
		ETConstexpr const VoiceList& GetActiveVoices() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		ETConstexpr Transformation ETSimdCall GetLocalToWorld() const ETNoexceptHint;

		ETConstexpr Vector ETSimdCall GetLinearVelocity() const ETNoexceptHint;

		void ETSimdCall MoveTo(Transformation localToWorld, Vector linearVelocity) ETNoexceptHint;

		// ---------------------------------------------------

	public:
		float32 GetCurrentDiegeticLoudnessDb() const ETNoexceptHint;

		ETConstexpr float32 GetWindowTopDb() const ETNoexceptHint;

		// - DATA MEMBERS ------------------------------------

	private:
		VoiceList      _voices;
		Transformation _localToWorld;
		Vector         _linearVelocity;
		float32        _perceivedLoudnessDb;
		float32        _minimumPerceivedLoudnessDb;
	};

	// ---

	ETPureFunctionHint float32 ETSimdCall AttenuateVolumeDb(Vector soundToListener, float32 baseVolumeDb) ETNoexceptHint;

	ETPureFunctionHint float32 GetHdrVolumeScalar(float32 windowTopDb, float32 volumeDb) ETNoexceptHint;

	template <typename InputIterator>
	float32 GetDiegeticLoudnessDb(InputIterator begin, InputIterator end) ETNoexceptHint;

}} // namespace Eldritch2::Audio

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Audio/Listener.inl>
//------------------------------------------------------------------//
