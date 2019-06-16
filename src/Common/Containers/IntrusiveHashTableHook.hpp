/*==================================================================*\
  IntrusiveHashTableHook.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2019 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <eastl/internal/intrusive_hashtable.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

using IntrusiveHashTableHook = eastl::intrusive_hash_node;

template <typename Key>
struct IntrusiveHashMapHook : public IntrusiveHashTableHook {
	using KeyType  = Key;
	using key_type = KeyType;

	Key mKey;
};

} // namespace Eldritch2
