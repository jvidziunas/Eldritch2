/*==================================================================*\
  FileAccessorFactory.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Platform/SynchronousFileWriter.hpp>
#include <Platform/SynchronousFileReader.hpp>
#include <Platform/MemoryMappedFile.hpp>
#include <Tools/FileAccessorFactory.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Tools {
namespace Win32 {

	class FileAccessorFactory : public Tools::FileAccessorFactory {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref FileAccessorFactory instance.
		explicit FileAccessorFactory( const FileAccessorFactory& ) = delete;
	//!	Constructs this @ref FileAccessorFactory instance.
		FileAccessorFactory() = default;

		~FileAccessorFactory() = default;

	// ---------------------------------------------------

		Eldritch2::UniquePointer<Platform::MemoryMappedFile>			CreateReadableMemoryMappedFile( Eldritch2::Allocator& allocator, const Eldritch2::Utf8Char* const fileName ) override;

		Eldritch2::UniquePointer<Platform::SynchronousFileReader>		CreateReader( Eldritch2::Allocator& allocator, const Eldritch2::Utf8Char* const fileName ) override;

		Eldritch2::UniquePointer<Platform::SynchronousFileWriter>		CreateWriter( Eldritch2::Allocator& allocator, const Eldritch2::Utf8Char* const fileName ) override;
	};

}	// namespace Win32
}	// namespace Tools
}	// namespace Eldritch2