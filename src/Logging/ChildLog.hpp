/*==================================================================*\
  ChildLog.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Logging/Log.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Logging {

	class ChildLog : public Logging::Log {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref ChildLog instance.
		ChildLog( Logging::Log& logger );
	//!	Constructs this @ref ChildLog instance.
		ChildLog( const ChildLog& ) = delete;

		~ChildLog() = default;

	// ---------------------------------------------------

	public:
		Logging::Log&	SetParent( Logging::Log& newParent );

		Logging::Log&	GetParent() const;

	// ---------------------------------------------------

	public:
		void	Write( const Eldritch2::Utf8Char* const string, size_t lengthInOctets ) override sealed;

	// ---------------------------------------------------

	public:
		ChildLog&	operator=( const ChildLog& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		Logging::Log*	_parent;
	};

}	// namespace Logging
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Logging/ChildLog.inl>
//------------------------------------------------------------------//