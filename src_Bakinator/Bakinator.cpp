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
#include <Tools/Tool.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2;

namespace {

	class BakinatorTool : public Tools::Tool {
		int Run() {

		}
	};

}	// anonymous namespace

int main( int argc, SystemChar** argv ) {
	return Tools::ExecuteTool<BakinatorTool>( Win32GlobalHeapAllocator( UTF8L("Root Allocator") ), argc, argv );
}