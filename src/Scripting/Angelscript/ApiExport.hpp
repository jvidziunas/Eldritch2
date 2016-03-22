/*==================================================================*\
  ApiExport.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Compiler.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace FileSystem {
		class	ReadableMemoryMappedFile;
		class	SynchronousFileWriter;
	}

	class	Allocator;
	class	ErrorCode;
}

class	asIScriptEngine;

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {

	::Eldritch2::ErrorCode	ExportScriptApi( ::Eldritch2::Allocator& allocator, ::asIScriptEngine& engine, FileSystem::SynchronousFileWriter& writer );

	::Eldritch2::ErrorCode	ImportScriptApi( ::Eldritch2::Allocator& allocator, ::asIScriptEngine& engine, const FileSystem::ReadableMemoryMappedFile& file );

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2