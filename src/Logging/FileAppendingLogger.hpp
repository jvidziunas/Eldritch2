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
#include <Utility/Memory/InstanceDeleters.hpp>
#include <Utility/Memory/Allocator.hpp>
#include <Logging/Logger.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace FileSystem {
		class	SynchronousFileAppender;
		class	ContentProvider;
	}
}

namespace Eldritch2 {
namespace Foundation {

	class FileAppendingLogger : public Foundation::Logger {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref FileAppendingLogger instance.
		FileAppendingLogger( FileSystem::ContentProvider& contentProvider, const ::Eldritch2::UTF8Char* const logName, ::Eldritch2::Allocator& allocator );

		//!	Destroys this @ref FileAppendingLogger instance.
		~FileAppendingLogger();

	// ---------------------------------------------------

		void	Write( const ::Eldritch2::UTF8Char* const string, const size_t lengthInOctets ) override sealed;

	// - DATA MEMBERS ------------------------------------

	private:
		::Eldritch2::InstancePointer<FileSystem::SynchronousFileAppender>	_appender;
	};

}	// namespace Foundation
}	// namespace Eldritch2