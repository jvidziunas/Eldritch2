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
#include <Utility/MPL/CharTypes.hpp>
#include <Utility/MPL/IntTypes.hpp>
//------------------------------------------------------------------//
#include <atomic>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Logging {

	enum class MessageSeverity {
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
		Log( const Log& ) = delete;
	//! Constructs this @ref Log instance.
	/*!	Designed to be called from subclasses. */
		Log();

		~Log() = default;

	// ---------------------------------------------------

	public:
		virtual void	Write( const Eldritch2::Utf8Char* const string, size_t lengthInOctets ) abstract;

		template <typename... Arguments>
		void			operator()( Logging::MessageSeverity severity, ETFormatStringHint( const Eldritch2::Utf8Char* str ), Arguments&&... arguments );

	// ---------------------------------------------------

	public:
		Logging::MessageSeverity	GetMuteThreshold() const;

		void						SetMuteThreshold( Logging::MessageSeverity muteThreshold );

	// ---------------------------------------------------

	//!	Disable assignment.
		Log&	operator=( const Log& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		std::atomic<Logging::MessageSeverity>	_muteThreshold;
	};

}	// namespace Logging
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Logging/Log.inl>
//------------------------------------------------------------------//