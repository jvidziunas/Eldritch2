/*==================================================================*\
  GeometryCompiler.Win32.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/Win32HeapAllocator.hpp>
#include <Tools/Win32/FileAccessorFactory.hpp>
#include <GeometryCompiler.hpp>
//------------------------------------------------------------------//

#if( ET_PLATFORM_WINDOWS )
int main( int argc, ::Eldritch2::UTF8Char** argv ) {
	using namespace ::Eldritch2;

// ---

	return Tools::GeometryCompiler<Win32GlobalHeapAllocator, Tools::Win32::FileAccessorFactory>().Run( { argv, argv + argc } );
}
#endif