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
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace FileSystem {

	template <typename KnownContentLocationIterator>
	::Eldritch2::Result<FileSystem::AsynchronousFileReader> ContentProvider::CreateAsynchronousFileReader( ::Eldritch2::Allocator& allocator, const ::Eldritch2::Range<KnownContentLocationIterator>& locations, const ::Eldritch2::UTF8Char* const fileName ) {
		::Eldritch2::Result<FileSystem::AsynchronousFileReader>	result { ::Eldritch2::Error::InvalidFileName };

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
	::Eldritch2::Result<FileSystem::SynchronousFileReader> ContentProvider::CreateSynchronousFileReader( ::Eldritch2::Allocator& allocator, const ::Eldritch2::Range<KnownContentLocationIterator>& locations, const ::Eldritch2::UTF8Char* const fileName ) {
		::Eldritch2::Result<FileSystem::SynchronousFileReader>	result { ::Eldritch2::Error::InvalidFileName };

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
	::Eldritch2::Result<FileSystem::ReadableMemoryMappedFile> ContentProvider::CreateReadableMemoryMappedFile( ::Eldritch2::Allocator& allocator, const ::Eldritch2::Range<KnownContentLocationIterator> locations, const ::Eldritch2::UTF8Char* const fileName ) {
		::Eldritch2::Result<FileSystem::ReadableMemoryMappedFile>	result { ::Eldritch2::Error::InvalidFileName };

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
	::Eldritch2::Result<FileSystem::AsynchronousFileWriter> ContentProvider::CreateAsynchronousFileWriter( ::Eldritch2::Allocator& allocator, const ::Eldritch2::Range<KnownContentLocationIterator>& locations, const ::Eldritch2::UTF8Char* const fileName, const FileOverwriteBehavior overwriteBehavior ) {
		::Eldritch2::Result<FileSystem::AsynchronousFileWriter>	result { ::Eldritch2::Error::InvalidFileName };

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
	::Eldritch2::Result<FileSystem::SynchronousFileWriter> ContentProvider::CreateSynchronousFileWriter( ::Eldritch2::Allocator& allocator, const ::Eldritch2::Range<KnownContentLocationIterator>& locations, const ::Eldritch2::UTF8Char* const fileName, const FileOverwriteBehavior overwriteBehavior ) {
		::Eldritch2::Result<FileSystem::SynchronousFileWriter>	result { ::Eldritch2::Error::InvalidFileName };

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
	::Eldritch2::Result<FileSystem::SynchronousFileAppender> ContentProvider::CreateSynchronousFileAppender( ::Eldritch2::Allocator& allocator, const ::Eldritch2::Range<KnownContentLocationIterator>& locations, const ::Eldritch2::UTF8Char* const fileName ) {
		::Eldritch2::Result<FileSystem::SynchronousFileAppender>	result { ::Eldritch2::Error::InvalidFileName };

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