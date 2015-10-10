/*==================================================================*\
  FileAppendingLogger.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Logging/Logger.hpp>
#include <Utility/Memory/ArenaAllocator.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace FileSystem {
		class	SynchronousFileAppender;
		class	ContentProvider;
	}
}

namespace Eldritch2 {
namespace Foundation {

	namespace Foundation	= ::Eldritch2::Foundation;
	namespace FileSystem	= ::Eldritch2::FileSystem;

// ---------------------------------------------------

	class FileAppendingLogger : public Foundation::Logger {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		// Constructs this FileAppendingLogger instance.
		FileAppendingLogger( FileSystem::ContentProvider& contentProvider, const ::Eldritch2::UTF8Char* const logName );

		// Destroys this FileAppendingLogger instance.
		~FileAppendingLogger();

	// ---------------------------------------------------

		using Foundation::Logger::WriteString;
		void	WriteString( const ::Eldritch2::UTF8Char* const string, const size_t lengthInOctets ) override sealed;

	// - DATA MEMBERS ------------------------------------

	private:
		::Eldritch2::FixedStackAllocator<64u>	_allocator;
		FileSystem::SynchronousFileAppender&	_appender;
	};

}	// namespace Foundation
}	// namespace Eldritch2