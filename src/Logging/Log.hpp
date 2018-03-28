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

namespace Eldritch2 {
namespace Logging {

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
		Log( const Log& ) = delete;
	//! Constructs this @ref Log instance.
	/*!	Designed to be called from subclasses. */
		Log();

		~Log() = default;

	// ---------------------------------------------------

	public:
		virtual void	Write( const Utf8Char* const string, size_t lengthInOctets ) abstract;

		template <size_t formatSize, typename... Arguments>
		void			Write( MessageType type, const Utf8Char (&formatString)[formatSize], Arguments&&... arguments );

	// ---------------------------------------------------

	public:
		MessageType	GetMuteThreshold() const;

		void		SetMuteThreshold( MessageType threshold );

	// ---------------------------------------------------

	//!	Disable assignment.
		Log&	operator=( const Log& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		Atomic<Logging::MessageType>	_muteThreshold;
	};

}	// namespace Logging
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Logging/Log.inl>
//------------------------------------------------------------------//