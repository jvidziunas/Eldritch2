/*==================================================================*\
  FileAppendingLogger.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/SynchronousFileAppender.hpp>
#include <Logging/FileAppendingLogger.hpp>
#include <Packages/ContentProvider.hpp>
#include <Utility/ErrorCode.hpp>
#include <Build.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2;

namespace {

	class NullAppender : public SynchronousFileAppender {
	public:
		ErrorCode Append( const void* const /*sourceData*/, const size_t /*lengthToWriteInBytes*/ ) {
			return Errors::NONE;
		}
	};

	static NullAppender	nullAppender;

// ---

	SynchronousFileAppender& CreateFileAppender( Allocator& allocator, ContentProvider& contentProvider, const UTF8Char* const fileName ) {
		if( const auto createAppenderResult = contentProvider.CreateSynchronousFileAppender( allocator, ContentProvider::KnownContentLocation::USER_DOCUMENTS, fileName ) ) {
			return *createAppenderResult.object;
		}

		return nullAppender;
	}

}	// anonymous namespace

namespace Eldritch2 {
namespace Foundation {

	FileAppendingLogger::FileAppendingLogger( ContentProvider& contentProvider, const UTF8Char* const logName ) : _allocator( UTF8L("Logger Allocator") ),
																												  _appender( CreateFileAppender( _allocator, contentProvider, logName ) ) {
		WriteString( UTF8L( "- BEGINNING LOG OF APPLICATION RUN -" ) ET_UTF8_NEWLINE_LITERAL );
	}

// ---------------------------------------------------

	FileAppendingLogger::~FileAppendingLogger() {
		WriteString( UTF8L( "- TERMINATING LOG -" ) ET_UTF8_NEWLINE_LITERAL ET_UTF8_NEWLINE_LITERAL );

		if( &_appender != &nullAppender ) {
			_allocator.Delete( _appender );
		}
	}

// ---------------------------------------------------

	void FileAppendingLogger::WriteString( const UTF8Char* const string, const size_t lengthInOctets ) {
		_appender.Append( string, lengthInOctets );
	}

}	// namespace Foundation
}	// namespace Eldritch2