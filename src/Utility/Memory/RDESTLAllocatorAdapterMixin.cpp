/*==================================================================*\
  RDESTLAllocatorAdapterMixin.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/RDESTLAllocatorAdapterMixin.hpp>
#include <Utility/Memory/ChildAllocator.hpp>
//------------------------------------------------------------------//

template class ::Eldritch2::Detail::RDESTLAllocatorAdapterMixin<::Eldritch2::ChildAllocator>;