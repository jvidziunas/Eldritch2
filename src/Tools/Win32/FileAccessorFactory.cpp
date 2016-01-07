/*==================================================================*\
  FileAccessorFactory.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <FileSystem/Win32/ReadableMemoryMappedFile.hpp>
#include <FileSystem/Win32/SynchronousFileReader.hpp>
#include <FileSystem/Win32/SynchronousFileWriter.hpp>
#include <Tools/Win32/FileAccessorFactory.hpp>
#include <Utility/Memory/StandardLibrary.hpp>
#include <Utility/Memory/InstanceNew.hpp>
//------------------------------------------------------------------//
#include <utfcpp/v2_0/source/utf8/unchecked.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Tools {
namespace Win32 {

	InstancePointer<FileSystem::ReadableMemoryMappedFile> FileAccessorFactory::CreateReadableMemoryMappedFile( Allocator& allocator, const UTF8Char* const fileName ) {
		return { nullptr, { allocator } };
	}

// ---------------------------------------------------

	InstancePointer<FileSystem::SynchronousFileReader> FileAccessorFactory::CreateReader( Allocator& allocator, const UTF8Char* const fileName ) {
		const auto	fileNameEnd( FindEndOfString( fileName ) );
		const auto	wideString( static_cast<wchar_t*>(_alloca( static_cast<size_t>(::utf8::unchecked::distance( fileName, fileNameEnd )) )) );

		::utf8::unchecked::utf8to16( fileName, fileNameEnd, wideString );

		const ::HANDLE	file( ::CreateFileW( wideString, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, nullptr ) );

		return { (INVALID_HANDLE_VALUE != file ? new( allocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION ) FileSystem::Win32::SynchronousFileReader( file, 1u ) : nullptr), { allocator } };
	}

// ---------------------------------------------------

	InstancePointer<FileSystem::SynchronousFileWriter> FileAccessorFactory::CreateWriter( Allocator& allocator, const UTF8Char* const fileName ) {
		const auto	fileNameEnd( FindEndOfString( fileName ) );
		const auto	wideString( static_cast<wchar_t*>(_alloca( static_cast<size_t>(::utf8::unchecked::distance( fileName, fileNameEnd )) )) );

		::utf8::unchecked::utf8to16( fileName, fileNameEnd, wideString );

		const ::HANDLE	file( ::CreateFileW( wideString, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr ) );

		return { (INVALID_HANDLE_VALUE != file ? new( allocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION ) FileSystem::Win32::SynchronousFileWriter( file, 1u ) : nullptr), { allocator } };
	}

}	// namespace Win32
}	// namespace Tools
}	// namespace Eldritch2