/*==================================================================*\
  ContentProvider.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Platform/SynchronousFileAppender.hpp>
#include <Platform/SynchronousFileReader.hpp>
#include <Platform/SynchronousFileWriter.hpp>
#include <Platform/MemoryMappedFile.hpp>
#include <Platform/ContentProvider.hpp>
#include <Utility/Result.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Platform {

	Result<SynchronousFileReader> ContentProvider::OpenSynchronousFileReader( WellKnownDirectory directory, const Utf8Char* const fileName ) {
		return SynchronousFileReader::Open( ExpandPath( directory, fileName ) );
	}

// ---------------------------------------------------

	Result<MemoryMappedFile> ContentProvider::OpenMemoryMappedFile( WellKnownDirectory directory, const Utf8Char* const fileName ) {
		return MemoryMappedFile::Open( ExpandPath( directory, fileName ), MemoryMappedFile::AccessMode::Read, 0u, 0u );
	}

// ---------------------------------------------------

	Result<SynchronousFileWriter> ContentProvider::OpenSynchronousFileWriter( WellKnownDirectory directory, const Utf8Char* const fileName ) {
		return SynchronousFileWriter::CreateOrOpen( ExpandPath( directory, fileName ) );
	}

// ---------------------------------------------------

	Result<SynchronousFileWriter> ContentProvider::CreateOrTruncateSynchronousFileWriter( WellKnownDirectory directory, const Utf8Char* const fileName ) {
		return SynchronousFileWriter::CreateOrTruncate( ExpandPath( directory, fileName ) );
	}

// ---------------------------------------------------

	Result<SynchronousFileAppender> ContentProvider::CreateSynchronousFileAppender( WellKnownDirectory directory, const Utf8Char* const fileName ) {
		return SynchronousFileAppender::OpenOrCreate( ExpandPath( directory, fileName ) );
	}

// ---------------------------------------------------

	ETPureFunctionHint String<SystemChar> ContentProvider::ExpandPath( WellKnownDirectory directory, const Utf8Char* fileName ) const {
		String<SystemChar>	path( { _allocator, "Path Allocator" } );

		path.Append( GetPathFromSpecifier( directory ) ).Append( fileName );

		return eastl::move( path );
	}

}	// namespace Platform
}	// namespace Eldritch2