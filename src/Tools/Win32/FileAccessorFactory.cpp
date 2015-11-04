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
#include <FileSystem/Win32/SynchronousFileReader.hpp>
#include <FileSystem/Win32/SynchronousFileWriter.hpp>
#include <Tools/Win32/FileAccessorFactory.hpp>
#include <Utility/Memory/InstanceNew.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Tools {
namespace Win32 {

	InstancePointer<FileSystem::SynchronousFileReader> FileAccessorFactory::CreateReader( Allocator& allocator, const SystemChar* const fileName ) {
		const ::HANDLE	file( ::CreateFile( fileName, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, nullptr ) );

		return { (INVALID_HANDLE_VALUE != file ? new( allocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION ) FileSystem::Win32::SynchronousFileReader( file, 1u ) : nullptr), { allocator } };
	}

// ---------------------------------------------------

	InstancePointer<FileSystem::SynchronousFileWriter> FileAccessorFactory::CreateWriter( Allocator& allocator, const SystemChar* const fileName ) {
		const ::HANDLE	file( ::CreateFile( fileName, GENERIC_READ, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr ) );

		return { (INVALID_HANDLE_VALUE != file ? new( allocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION ) FileSystem::Win32::SynchronousFileWriter( file, 1u ) : nullptr), { allocator } };
	}

}	// namespace Win32
}	// namespace Tools
}	// namespace Eldritch2