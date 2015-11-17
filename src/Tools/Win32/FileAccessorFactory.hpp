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
#include <FileSystem/ReadableMemoryMappedFile.hpp>
#include <FileSystem/SynchronousFileWriter.hpp>
#include <FileSystem/SynchronousFileReader.hpp>
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

		::Eldritch2::InstancePointer<FileSystem::ReadableMemoryMappedFile>	CreateReadableMemoryMappedFile( ::Eldritch2::Allocator& allocator, const ::Eldritch2::SystemChar* const fileName ) override;

		::Eldritch2::InstancePointer<FileSystem::SynchronousFileReader>		CreateReader( ::Eldritch2::Allocator& allocator, const ::Eldritch2::SystemChar* const fileName ) override;

		::Eldritch2::InstancePointer<FileSystem::SynchronousFileWriter>		CreateWriter( ::Eldritch2::Allocator& allocator, const ::Eldritch2::SystemChar* const fileName ) override;
	};

}	// namespace Win32
}	// namespace Tools
}	// namespace Eldritch2