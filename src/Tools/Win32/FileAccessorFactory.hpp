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
#include <Tools/FileAccessorFactory.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Tools {
namespace Win32 {

	class FileAccessorFactory : public Tools::FileAccessorFactory {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref FileAccessorFactory instance.
		FileAccessorFactory() = default;

		//!	Destroys this @ref FileAccessorFactory instance.
		~FileAccessorFactory() = default;

	// ---------------------------------------------------

		virtual ::Eldritch2::InstancePointer<FileSystem::SynchronousFileReader>	CreateReader( ::Eldritch2::Allocator& allocator, const ::Eldritch2::SystemChar* const fileName ) override;

		virtual ::Eldritch2::InstancePointer<FileSystem::SynchronousFileWriter>	CreateWriter( ::Eldritch2::Allocator& allocator, const ::Eldritch2::SystemChar* const fileName ) override;
	};

}	// namespace Win32
}	// namespace Tools
}	// namespace Eldritch2