/*==================================================================*\
  Bakinator.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/Win32HeapAllocator.hpp>
#include <Tools/Win32/FileAccessorFactory.hpp>
#include <Bakinator.hpp>
//------------------------------------------------------------------//

#if( ET_PLATFORM_WINDOWS )
int main( int argc, const ::Eldritch2::SystemChar** argv ) {
	using namespace ::Eldritch2;

// ---

	return Tools::Bakinator<Win32GlobalHeapAllocator, Tools::Win32::FileAccessorFactory>().Run( { argv + 1, argv + argc } );
}
#endif