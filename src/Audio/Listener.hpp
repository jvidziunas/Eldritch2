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

	class Voice {
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
		//!	Constructs this @ref Voice instance.
		Voice(float32 loudnessDb);
		//!	Constructs this @ref Voice instance.
		Voice(const Voice&) = default;

		~Voice() = default;

		// ---------------------------------------------------

	public:
		float32 GetHdrAmplitudeScalar(float32 windowTopDb) const;

		// - DATA MEMBERS ------------------------------------

	public:
		float32 loudnessDb;
	};

	// ---

	template <typename Voice>
	class Listener {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		Listener(const Listener&) = delete;
		//!	Constructs this @ref Listener instance.
		Listener();

		~Listener() = default;

		// ---------------------------------------------------

	public:
		const ArrayList<Voice>& GetActiveVoices() const;

		// ---------------------------------------------------

	public:
		float32 GetCurrentLoudnessDb() const;

		float32 GetWindowTop() const;

		// - DATA MEMBERS ------------------------------------

	private:
		//!	Playing sounds, ordered by logical volume.
		ArrayList<Voice> _activeVoices;
		float32          _perceivedLoudnessDb;
		float32          _minimumPerceivedLoudnessDb;
		float32          _windowBottomDb;
	};

	// ---

	template <typename Iterator>
	float32 GetLoudnessDb(Iterator begin, Iterator end);

}} // namespace Eldritch2::Audio

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Audio/Listener.inl>
//------------------------------------------------------------------//
