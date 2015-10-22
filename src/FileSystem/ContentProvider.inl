/*==================================================================*\
  ContentProvider.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/Range.hpp>
#include <Utility/Result.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace FileSystem {

	namespace FileSystem	= ::Eldritch2::FileSystem;
	namespace Utility		= ::Eldritch2::Utility;

// ---------------------------------------------------

	template <typename KnownContentLocationIterator>
	Utility::Result<FileSystem::AsynchronousFileReader> ContentProvider::CreateAsynchronousFileReader( ::Eldritch2::Allocator& allocator, const ::Eldritch2::Range<KnownContentLocationIterator>& locations, const ::Eldritch2::UTF8Char* const fileName ) {
		Utility::Result<FileSystem::AsynchronousFileReader>	result { ::Eldritch2::Error::BAD_FILE_NAME };

		for( auto searchLocation : locations ) {
			result = CreateAsynchronousFileReader( allocator, searchLocation, fileName );

			if( result ) {
				break;
			}
		}

		return result;
	}

// ---------------------------------------------------

	template <typename KnownContentLocationIterator>
	Utility::Result<FileSystem::SynchronousFileReader> ContentProvider::CreateSynchronousFileReader( ::Eldritch2::Allocator& allocator, const ::Eldritch2::Range<KnownContentLocationIterator>& locations, const ::Eldritch2::UTF8Char* const fileName ) {
		Utility::Result<FileSystem::SynchronousFileReader>	result { ::Eldritch2::Error::BAD_FILE_NAME };

		for( auto searchLocation : locations ) {
			result = CreateSynchronousFileReader( allocator, searchLocation, fileName );

			if( result ) {
				break;
			}
		}

		return result;
	}

// ---------------------------------------------------

	template <typename KnownContentLocationIterator>
	Utility::Result<FileSystem::ReadableMemoryMappedFile> ContentProvider::CreateReadableMemoryMappedFile( ::Eldritch2::Allocator& allocator, const ::Eldritch2::Range<KnownContentLocationIterator> locations, const ::Eldritch2::UTF8Char* const fileName ) {
		Utility::Result<FileSystem::ReadableMemoryMappedFile>	result { ::Eldritch2::Errors::BAD_FILE_NAME };

		for( auto searchLocation : locations ) {
			result = CreateReadableMemoryMappedFile( allocator, searchLocation, fileName );

			if( result ) {
				break;
			}
		}

		return result;
	}

// ---------------------------------------------------

	template <typename KnownContentLocationIterator>
	Utility::Result<FileSystem::AsynchronousFileWriter> ContentProvider::CreateAsynchronousFileWriter( ::Eldritch2::Allocator& allocator, const ::Eldritch2::Range<KnownContentLocationIterator>& locations, const ::Eldritch2::UTF8Char* const fileName, const FileOverwriteBehavior overwriteBehavior ) {
		Utility::Result<FileSystem::AsynchronousFileWriter>	result { ::Eldritch2::Errors::BAD_FILE_NAME };

		for( auto searchLocation : locations ) {
			result = CreateAsynchronousFileWriter( allocator, searchLocation, fileName, overwriteBehavior );

			if( result ) {
				break;
			}
		}

		return result;
	}

// ---------------------------------------------------

	template <typename KnownContentLocationIterator>
	Utility::Result<FileSystem::SynchronousFileWriter> ContentProvider::CreateSynchronousFileWriter( ::Eldritch2::Allocator& allocator, const ::Eldritch2::Range<KnownContentLocationIterator>& locations, const ::Eldritch2::UTF8Char* const fileName, const FileOverwriteBehavior overwriteBehavior ) {
		Utility::Result<FileSystem::SynchronousFileWriter>	result { ::Eldritch2::Errors::BAD_FILE_NAME };

		for( auto searchLocation : locations ) {
			result = CreateSynchronousFileWriter( allocator, searchLocation, fileName, overwriteBehavior );

			if( result ) {
				break;
			}
		}

		return result;
	}

// ---------------------------------------------------

	template <typename KnownContentLocationIterator>
	Utility::Result<FileSystem::SynchronousFileAppender> ContentProvider::CreateSynchronousFileAppender( ::Eldritch2::Allocator& allocator, const ::Eldritch2::Range<KnownContentLocationIterator>& locations, const ::Eldritch2::UTF8Char* const fileName ) {
		Utility::Result<FileSystem::SynchronousFileAppender>	result { ::Eldritch2::Errors::BAD_FILE_NAME };

		for( auto searchLocation : locations ) {
			result = CreateSynchronousFileAppender( allocator, searchLocation, fileName );

			if( result ) {
				break;
			}
		}

		return result;
	}

}	// namespace FileSystem
}	// namespace Eldritch2