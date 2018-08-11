/*==================================================================*\
  Log.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Logging {

	enum class MessageType {
		VerboseWarning = 0,
		Warning,
		Error,
		Message
	};

	// ---

	class ETPureAbstractHint Log {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		//! Constructs this @ref Log instance.
		/*!	Designed to be called from subclasses. */
		Log(const Log&) = delete;
		//! Constructs this @ref Log instance.
		/*!	Designed to be called from subclasses. */
		Log();

		~Log() = default;

		// ---------------------------------------------------

	public:
		virtual void Write(const Utf8Char* const string, size_t lengthInOctets) abstract;

		template <typename... Arguments>
		void Write(MessageType type, StringView<Utf8Char> format, Arguments&&... arguments);

		// ---------------------------------------------------

	public:
		MessageType GetMuteThreshold() const;

		void SetWriteThreshold(MessageType threshold);

		// ---------------------------------------------------

		//!	Disable assignment.
		Log& operator=(const Log&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		Atomic<MessageType> _muteThreshold;
	};

}} // namespace Eldritch2::Logging

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Logging/Log.inl>
//------------------------------------------------------------------//
