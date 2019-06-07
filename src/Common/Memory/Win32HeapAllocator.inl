/*==================================================================*\
  Win32HeapAllocator.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2019 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {

	ETConstexpr ETForceInlineHint bool operator==(AbstractWin32HeapAllocator& lhs, AbstractWin32HeapAllocator& rhs) ETNoexceptHint {
		return lhs._heap == rhs._heap;
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint HANDLE AbstractWin32HeapAllocator::GetHeap() const ETNoexceptHint {
		return _heap;
	}

	// ---------------------------------------------------

	ETForceInlineHint Win32HeapAllocator::Win32HeapAllocator(const Win32HeapAllocator& allocator) ETNoexceptHint : AbstractWin32HeapAllocator(allocator.GetHeap(), allocator.GetName()) {}

}	// namespace Eldritch2