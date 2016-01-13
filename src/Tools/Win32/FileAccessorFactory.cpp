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

using namespace ::utf8::unchecked;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Tools {
namespace Win32 {

	InstancePointer<FileSystem::ReadableMemoryMappedFile> FileAccessorFactory::CreateReadableMemoryMappedFile( Allocator& allocator, const UTF8Char* const fileName ) {
		FileSystem::ReadableMemoryMappedFile*	returnObject( nullptr );
		// Determine the length of the UTF-16 string to be handed off to the Win32 API.
		const auto								fileNameEnd( FindEndOfString( fileName ) + 1 );
		const auto								wideString( static_cast<wchar_t*>(_alloca( static_cast<size_t>(::utf8::unchecked::distance( fileName, fileNameEnd )) * sizeof(wchar_t) )) );

		utf8to16( fileName, fileNameEnd, wideString );

		const ::HANDLE	file( ::CreateFileW( wideString, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, nullptr ) );

		if( const ::HANDLE fileMapping { ::CreateFileMappingW( file, nullptr, PAGE_READONLY, static_cast<::DWORD>(0u), static_cast<::DWORD>(0u), nullptr ) } ) {
			::LARGE_INTEGER	mappingSize;
			if( FALSE == ::GetFileSizeEx( file, &mappingSize ) ) {
				mappingSize.QuadPart = 0;
			}

			if( const auto mappedView = static_cast<const char*>(::MapViewOfFile( fileMapping, FILE_MAP_READ, static_cast<::DWORD>(0u), static_cast<::DWORD>(0u), static_cast<::SIZE_T>(0u) )) ) {
				returnObject = new(allocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION) FileSystem::Win32::ReadableMemoryMappedFile(Range<const char*>{ mappedView, mappedView + mappingSize.QuadPart });

				if( !returnObject ) {
					// If we've reached this point, the final allocation failed and we're still responsible for releasing resources.
					::UnmapViewOfFile( mappedView );
				}
			}

			::CloseHandle( fileMapping );
		}

		// If MapViewOfFile() was previously successful, the file mapping object has created an additional reference to the file that will be released when the @ref ReadableMemoryMappedFile is destructed.
		// As such, we need to release the reference here to ensure resources aren't leaked.
		::CloseHandle( file );

		return { returnObject, { allocator } };
	}

// ---------------------------------------------------

	InstancePointer<FileSystem::SynchronousFileReader> FileAccessorFactory::CreateReader( Allocator& allocator, const UTF8Char* const fileName ) {
		// Determine the length of the UTF-16 string to be handed off to the Win32 API.
		const auto	fileNameEnd( FindEndOfString( fileName ) + 1 );
		const auto	wideString( static_cast<wchar_t*>(_alloca( static_cast<size_t>(::utf8::unchecked::distance( fileName, fileNameEnd )) * sizeof(wchar_t) )) );

		utf8to16( fileName, fileNameEnd, wideString );

		const ::HANDLE	file( ::CreateFileW( wideString, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, nullptr ) );

		return { (INVALID_HANDLE_VALUE != file ? new(allocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION) FileSystem::Win32::SynchronousFileReader( file, 1u ) : nullptr), { allocator } };
	}

// ---------------------------------------------------

	InstancePointer<FileSystem::SynchronousFileWriter> FileAccessorFactory::CreateWriter( Allocator& allocator, const UTF8Char* const fileName ) {
		const auto	fileNameEnd( FindEndOfString( fileName ) + 1 );
		const auto	wideString( static_cast<wchar_t*>(_alloca( static_cast<size_t>(::utf8::unchecked::distance( fileName, fileNameEnd )) * sizeof(wchar_t) )) );

		utf8to16( fileName, fileNameEnd, wideString );

		const ::HANDLE	file( ::CreateFileW( wideString, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr ) );

		return { (INVALID_HANDLE_VALUE != file ? new(allocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION) FileSystem::Win32::SynchronousFileWriter( file, 1u ) : nullptr), { allocator } };
	}

}	// namespace Win32
}	// namespace Tools
}	// namespace Eldritch2