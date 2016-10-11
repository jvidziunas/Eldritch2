/*==================================================================*\
  ChildAllocator.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {

	ETInlineHint size_t ChildAllocator::GetPeakAllocationAmountInBytes() const {
		return _peakAllocatedAmount.load( std::memory_order_acquire );
	}

// ---------------------------------------------------

	ETInlineHint Eldritch2::Allocator& ChildAllocator::GetParent() const {
		return *_parent;
	}

}	// namespace Eldritch2