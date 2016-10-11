/*==================================================================*\
  SynchronousFileAppender.hpp
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
#include <Utility/MPL/Platform.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	template <typename>
	class	Result;
	
	class	ErrorCode;
}

#if ET_PLATFORM_WINDOWS
using HANDLE = void*;
#endif

namespace Eldritch2 {
namespace Platform {

	class SynchronousFileAppender {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
#	if ET_PLATFORM_WINDOWS
	//!	Constructs this @ref SynchronousFileAppender instance.
		explicit SynchronousFileAppender( HANDLE fileHandle );
#	else
		static_assert( false, "SynchronousFileAppender need implementation for your platform!" );
#	endif
	//!	Disable copying.
		SynchronousFileAppender( const SynchronousFileAppender& ) = delete;
	//!	Constructs this @ref SynchronousFileAppender instance.
		SynchronousFileAppender( SynchronousFileAppender&& );

		~SynchronousFileAppender();

	// ---------------------------------------------------

	public:
		static Eldritch2::Result<SynchronousFileAppender>	OpenOrCreate( const Eldritch2::SystemChar* const path );

		static Eldritch2::Result<SynchronousFileAppender>	Open( const Eldritch2::SystemChar* const path );

	// ---------------------------------------------------

	public:
		Eldritch2::ErrorCode	Append( const void* const sourceData, size_t lengthToWriteInBytes );

	// ---------------------------------------------------

	//!	Disable assignment.
		SynchronousFileAppender&	operator=( const SynchronousFileAppender& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		HANDLE	_fileHandle;
	};

}	// namespace Platform
}	// namespace Eldritch2