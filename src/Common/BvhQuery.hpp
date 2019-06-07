/*==================================================================*\
  BvhQuery.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2019 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Containers/ArrayBvh.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <class TargetBvh>
	class BvhQuery {
	public:
		using TargetBvhType = TargetBvh;

	public:
		BvhQuery() ETNoexceptHint = default;

		~BvhQuery() = default;

	public:

	};

}	// namespace Eldritch2
