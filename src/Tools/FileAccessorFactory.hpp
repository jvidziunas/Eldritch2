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
#include <Utility/Memory/InstanceDeleters.hpp>
#include <Utility/MPL/CharTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace FileSystem {
		class	ReadableMemoryMappedFile;
		class	SynchronousFileReader;
		class	SynchronousFileWriter;
	}
}

namespace Eldritch2 {
namespace Tools {

	class ETPureAbstractHint FileAccessorFactory {
	// ---------------------------------------------------

	public:
		virtual ::Eldritch2::InstancePointer<FileSystem::ReadableMemoryMappedFile>	CreateReadableMemoryMappedFile( ::Eldritch2::Allocator& allocator, const ::Eldritch2::SystemChar* const fileName ) abstract;

		virtual ::Eldritch2::InstancePointer<FileSystem::SynchronousFileReader>		CreateReader( ::Eldritch2::Allocator& allocator, const ::Eldritch2::SystemChar* const fileName ) abstract;

		virtual ::Eldritch2::InstancePointer<FileSystem::SynchronousFileWriter>		CreateWriter( ::Eldritch2::Allocator& allocator, const ::Eldritch2::SystemChar* const fileName ) abstract;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		//!	Constructs this @ref FileAccessorFactory instance.
		FileAccessorFactory( const FileAccessorFactory& ) = delete;
		//!	Constructs this @ref FileAccessorFactory instance.
		FileAccessorFactory() = default;

		//!	Destroys this @ref FileAccessorFactory instance.
		~FileAccessorFactory() = default;
	};

}	// namespace Tools
}	// namespace Eldritch2

