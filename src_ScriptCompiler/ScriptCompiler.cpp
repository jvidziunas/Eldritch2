/*==================================================================*\
  ScriptCompiler.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/Win32HeapAllocator.hpp>
#include <Utility/Containers/Range.hpp>
#include <Tools/Tool.hpp>
//------------------------------------------------------------------//
#include <Scripting/AngelScript/BytecodeMetadata_generated.h>
//------------------------------------------------------------------//
#include <angelscript/sdk/add_on/scriptbuilder/scriptbuilder.h>
//------------------------------------------------------------------//

namespace {

	using namespace ::Eldritch2::Tools;
	using namespace ::Eldritch2;

	class ScriptCompilerTool : public Win32GlobalHeapAllocator, public ToolCRTPBase<ScriptCompilerTool>, public ::CScriptBuilder {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref ScriptCompilerTool instance.
		ScriptCompilerTool() : Win32GlobalHeapAllocator( UTF8L("Root Allocator") ), ToolCRTPBase<ScriptCompilerTool>( GetGlobalAllocator() ) {
			SetIncludeCallback( [] ( const char* path, const char* from, ::CScriptBuilder* builder, void* ) { return static_cast<ScriptCompilerTool*>(builder)->IncludeFile( path, from ); }, nullptr );
		}

		//!	Destroys this @ref ScriptCompilerTool instance.
		~ScriptCompilerTool() = default;

	// ---------------------------------------------------

		ETForceInlineHint Win32GlobalHeapAllocator& GetGlobalAllocator() {
			return static_cast<Win32GlobalHeapAllocator&>(*this);
		}

	// ---------------------------------------------------

	protected:
		int	IncludeFile( const char* path, const char* from ) {
			return ::asSUCCESS;
		}
	};

}	// anonymous namespace

int main( int argc, SystemChar** argv ) {
	return ScriptCompilerTool().Run( { argv, argv + argc } );
}