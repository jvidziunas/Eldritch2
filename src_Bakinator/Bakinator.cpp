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
#include <Utility/Containers/Range.hpp>
#include <Tools/Tool.hpp>
//------------------------------------------------------------------//

namespace {

	using namespace ::Eldritch2::Tools;
	using namespace ::Eldritch2;

	class BakinatorTool : public Win32GlobalHeapAllocator, public ToolCRTPBase<BakinatorTool> {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref BakinatorTool instance.
		BakinatorTool() : Win32GlobalHeapAllocator( UTF8L("Root Allocator") ), ToolCRTPBase<BakinatorTool>( GetGlobalAllocator() ) {}

		//!	Destroys this @ref BakinatorTool instance.
		~BakinatorTool() = default;

	// ---------------------------------------------------

		ETForceInlineHint Win32GlobalHeapAllocator& GetGlobalAllocator() {
			return static_cast<Win32GlobalHeapAllocator&>(*this);
		}
	};

}	// anonymous namespace

int main( int argc, SystemChar** argv ) {
	return BakinatorTool().Run( { argv, argv + argc } );
}