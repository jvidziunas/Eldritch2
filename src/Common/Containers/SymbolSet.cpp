/*==================================================================*\
  SymbolSet.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  �2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

template class Eldritch2::SymbolSet<Eldritch2::Utf8Char, Eldritch2::MallocAllocator>;
template class Eldritch2::SymbolSet<Eldritch2::Utf8Char, Eldritch2::ChildAllocator>;