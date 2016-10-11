/*==================================================================*\
  EaStlAllocatorAdapterMixin.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/EaStlAllocatorAdapterMixin.hpp>
#include <Utility/Memory/ChildAllocator.hpp>
//------------------------------------------------------------------//

template class Eldritch2::Detail::EaStlAllocatorAdapterMixin<Eldritch2::ChildAllocator>;