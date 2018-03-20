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
#include <Common/SymbolTable.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Action, typename Weight, class Allocator = Eldritch2::MallocAllocator>
	class GoapAgent {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using	ActionType		= Pair<Action, Weight>;
		using	ListType		= ArrayList<ActionType, Allocator>;
		using	ActionQueueType = PriorityQueue<ActionType, ListType>;
		using	VertexType		= typename ActionQueueType::ValueType;
		using	AllocatorType	= typename ListType::AllocatorType;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref GoapAgent instance.
		GoapAgent( const AllocatorType& allocator );
	//!	Disable copy construction.
		GoapAgent( const GoapAgent& ) = delete;

		~GoapAgent() = default;

	// ---------------------------------------------------

	public:
		bool	Plan() const;

	// - DATA MEMBERS ------------------------------------

	private:
		ActionQueueType	_actions;
		Weight			_distanceEstimate;
	};

// ---

	template <typename Action, typename Weight, class Allocator = MallocAllocator>
	class GoapPlanner {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using AllocatorType	= typename Allocator;
		using AgentType		= typename GoapAgent<Action, Weight>;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref GoapPlanner instance.
		GoapPlanner( const AllocatorType& allocator );
	//!	Disable copy construction.
		GoapPlanner( const GoapPlanner& ) = delete;

		~GoapPlanner() = default;

	// - DATA MEMBERS ------------------------------------

	private:
		AllocatorType							_allocator;
		SymbolTable<Utf8Char, ChildAllocator>	_stateTags;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/GoapPlanner.inl>
//------------------------------------------------------------------//