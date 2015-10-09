/*==================================================================*\
  ContentProvider.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2012 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/Range.hpp>
#include <Utility/ResultPair.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace FileSystem {

	namespace FileSystem	= ::Eldritch2::FileSystem;
	namespace Utility		= ::Eldritch2::Utility;

// ---------------------------------------------------

	template <typename KnownContentLocationIterator>
	Utility::ResultPair<FileSystem::AsynchronousFileReader> ContentProvider::CreateAsynchronousFileReader( ::Eldritch2::Allocator& allocator, const ::Eldritch2::Range<KnownContentLocationIterator>& locations, const ::Eldritch2::UTF8Char* const fileName ) {
		Utility::ResultPair<FileSystem::AsynchronousFileReader>	result { nullptr, ::Eldritch2::Errors::BAD_FILE_NAME };

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
	Utility::ResultPair<FileSystem::SynchronousFileReader> ContentProvider::CreateSynchronousFileReader( ::Eldritch2::Allocator& allocator, const ::Eldritch2::Range<KnownContentLocationIterator>& locations, const ::Eldritch2::UTF8Char* const fileName ) {
		Utility::ResultPair<FileSystem::SynchronousFileReader>	result { nullptr, ::Eldritch2::Errors::BAD_FILE_NAME };

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
	Utility::ResultPair<FileSystem::ReadableMemoryMappedFile> ContentProvider::CreateReadableMemoryMappedFile( ::Eldritch2::Allocator& allocator, const ::Eldritch2::Range<KnownContentLocationIterator> locations, const ::Eldritch2::UTF8Char* const fileName ) {
		Utility::ResultPair<FileSystem::ReadableMemoryMappedFile>	result { nullptr, ::Eldritch2::Errors::BAD_FILE_NAME };

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
	Utility::ResultPair<FileSystem::AsynchronousFileWriter> ContentProvider::CreateAsynchronousFileWriter( ::Eldritch2::Allocator& allocator, const ::Eldritch2::Range<KnownContentLocationIterator>& locations, const ::Eldritch2::UTF8Char* const fileName, const FileOverwriteBehavior overwriteBehavior ) {
		Utility::ResultPair<FileSystem::AsynchronousFileWriter>	result { nullptr, ::Eldritch2::Errors::BAD_FILE_NAME };

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
	Utility::ResultPair<FileSystem::SynchronousFileWriter> ContentProvider::CreateSynchronousFileWriter( ::Eldritch2::Allocator& allocator, const ::Eldritch2::Range<KnownContentLocationIterator>& locations, const ::Eldritch2::UTF8Char* const fileName, const FileOverwriteBehavior overwriteBehavior ) {
		Utility::ResultPair<FileSystem::SynchronousFileWriter>	result { nullptr, ::Eldritch2::Errors::BAD_FILE_NAME };

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
	Utility::ResultPair<FileSystem::SynchronousFileAppender> ContentProvider::CreateSynchronousFileAppender( ::Eldritch2::Allocator& allocator, const ::Eldritch2::Range<KnownContentLocationIterator>& locations, const ::Eldritch2::UTF8Char* const fileName ) {
		Utility::ResultPair<FileSystem::SynchronousFileAppender>	result { nullptr, ::Eldritch2::Errors::BAD_FILE_NAME };

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