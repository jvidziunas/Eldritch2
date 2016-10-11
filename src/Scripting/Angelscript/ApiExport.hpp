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
	namespace Platform {
		class	SynchronousFileWriter;
		class	MemoryMappedFile;
	}

	class	Allocator;
	class	ErrorCode;
}

class	asIScriptEngine;

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {

	Eldritch2::ErrorCode	ExportScriptApi( Eldritch2::Allocator& allocator, asIScriptEngine& engine, Platform::SynchronousFileWriter& writer );

	Eldritch2::ErrorCode	ImportScriptApi( Eldritch2::Allocator& allocator, asIScriptEngine& engine, const Platform::MemoryMappedFile& file );

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2