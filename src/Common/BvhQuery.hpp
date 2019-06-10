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

	template <typename Record>


template <class Bvh>
class BvhQuery {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	using BvhType = Bvh;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref BvhQuery instance.
	BvhQuery(const BvhQuery&) = default;
	//!	Constructs this @ref BvhQuery instance.
	BvhQuery() ETNoexceptHint = default;

	~BvhQuery() = default;

public:
};

} // namespace Eldritch2
