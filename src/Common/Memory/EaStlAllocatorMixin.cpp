/*==================================================================*\
  EaStlAllocatorMixin.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Memory/EaStlAllocatorMixin.hpp>
#include <Common/Memory/MallocAllocator.hpp>
#include <Common/Memory/ChildAllocator.hpp>
//------------------------------------------------------------------//

template class Eldritch2::EaStlAllocatorMixin<Eldritch2::MallocAllocator>;
template class Eldritch2::EaStlAllocatorMixin<Eldritch2::ChildAllocator>;