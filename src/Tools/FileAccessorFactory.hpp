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
#include <Utility/UniquePointer.hpp>
#include <Utility/MPL/CharTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Platform {
		class	SynchronousFileReader;
		class	SynchronousFileWriter;
		class	MemoryMappedFile;
	}
}

namespace Eldritch2 {
namespace Tools {

	class ETPureAbstractHint FileAccessorFactory {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
	//!	Constructs this @ref FileAccessorFactory instance.
		FileAccessorFactory( const FileAccessorFactory& ) = delete;
	//!	Constructs this @ref FileAccessorFactory instance.
		FileAccessorFactory() = default;

		~FileAccessorFactory() = default;

	// ---------------------------------------------------

	public:
		virtual Eldritch2::UniquePointer<Platform::MemoryMappedFile>			CreateReadableMemoryMappedFile( Eldritch2::Allocator& allocator, const Eldritch2::Utf8Char* const fileName ) abstract;

		virtual Eldritch2::UniquePointer<Platform::SynchronousFileReader>		CreateReader( Eldritch2::Allocator& allocator, const Eldritch2::Utf8Char* const fileName ) abstract;

		virtual Eldritch2::UniquePointer<Platform::SynchronousFileWriter>		CreateWriter( Eldritch2::Allocator& allocator, const Eldritch2::Utf8Char* const fileName ) abstract;
	};

}	// namespace Tools
}	// namespace Eldritch2

