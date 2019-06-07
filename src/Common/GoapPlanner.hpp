/*==================================================================*\
  GoapPlanner.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Containers/PriorityQueue.hpp>
#include <Common/Memory/ChildAllocator.hpp>
#include <Common/Containers/ArrayMap.hpp>
#include <Common/Mpl/FloatTypes.hpp>
#include <Common/SymbolSet.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Action, typename Weight, class Allocator = Eldritch2::MallocAllocator>
class GoapAgent {
	// - TYPE PUBLISHING ---------------------------------

public:
	using ActionList     = ArrayList<Pair<Action, Weight>, Allocator>;
	using ActionQueue    = PriorityQueue<typename ActionList::ValueType, ActionList>;
	using ActionType     = typename ActionList::ValueType;
	using PlanVertexType = typename ActionQueue::ValueType;
	using AllocatorType  = typename ActionList::AllocatorType;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref GoapAgent instance.
	GoapAgent(const AllocatorType& allocator) ETNoexceptHint;
	//!	Disable copy construction.
	GoapAgent(const GoapAgent&) = delete;

	~GoapAgent() = default;

	// ---------------------------------------------------

public:
	bool Plan() const;

	// - DATA MEMBERS ------------------------------------

private:
	ActionQueue _actions;
	Weight      _distanceEstimate;
};

// ---

template <typename Action, typename Weight, class Allocator = MallocAllocator>
class GoapPlanner {
	// - TYPE PUBLISHING ---------------------------------

public:
	using SymbolSetType = SymbolSet<Utf8Char, Allocator>;
	using AllocatorType = typename SymbolSetType::AllocatorType;
	using AgentType     = typename GoapAgent<Action, Weight>;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref GoapPlanner instance.
	GoapPlanner(const AllocatorType& allocator) ETNoexceptHint;
	//!	Disable copy construction.
	GoapPlanner(const GoapPlanner&) = delete;

	~GoapPlanner() = default;

	// - DATA MEMBERS ------------------------------------

private:
	SymbolSetType _tags;
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/GoapPlanner.inl>
//------------------------------------------------------------------//
