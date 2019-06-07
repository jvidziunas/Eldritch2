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

	enum Severity {
		Debug,
		VerboseWarning,
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
		Log(const Log&) ETNoexceptHint;
		//! Constructs this @ref Log instance.
		/*!	Designed to be called from subclasses. */
		Log() ETNoexceptHint;

		~Log() = default;

		// ---------------------------------------------------

	public:
		virtual void Write(const Utf8Char* string, size_t lengthInOctets) ETNoexceptHint abstract;

		template <typename... Arguments>
		void Write(Severity severity, StringSpan format, Arguments&&... arguments);

		// ---------------------------------------------------

	public:
		Severity GetWriteThreshold(MemoryOrder order = std::memory_order_consume) const ETNoexceptHint;

		void SetWriteThreshold(Severity threshold, MemoryOrder order = std::memory_order_release) ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable assignment.
		Log& operator=(const Log&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		Atomic<Severity> _writeThreshold;
	};

}} // namespace Eldritch2::Logging

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Logging/Log.inl>
//------------------------------------------------------------------//
