/*==================================================================*\
  ArrayBvh.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Memory/ChildAllocator.hpp>
#include <Common/Containers/ArrayList.hpp>
#include <Common/Mpl/IntTypes.hpp>
#include <Common/Pair.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Value, typename KeyExtractor, class Allocator = MallocAllocator>
	class ArrayBvh {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using SortKey		= uintptr;
		using SplitIndex	= uint32_t;

	// ---

	public:
		struct Split {
			SplitIndex	minima[3];
			SplitIndex	maxima[3];

			SplitIndex	leftChild;
			SplitIndex	rightChild;
		};

	// ---

	private:
		using LeafContainer		= ArrayList<Pair<SortKey, const Value*>, ChildAllocator>;
		using SplitContainer	= ArrayList<Split, ChildAllocator>;

	public:
		using ConstIterator		= typename LeafContainer::ConstIterator;
		using Iterator			= typename LeafContainer::Iterator;
		using SizeType			= typename LeafContainer::SizeType;
		using AllocatorType		= Allocator;
		using ValueType			= Value;
		using LeafType			= typename LeafContainer::ValueType;
		using KeyExtractorType	= KeyExtractor;
		
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref ArrayBvh instance.
		ArrayBvh( const KeyExtractorType& extractor, const AllocatorType& = AllocatorType() );
	//!	Constructs this @ref ArrayBvh instance.
		ArrayBvh( const ArrayBvh& ) = default;

		~ArrayBvh() = default;

	// ---------------------------------------------------
		
	public:
		void		Insert( const ValueType& value );

		Iterator	Find( const ValueType& value );

		Iterator	Erase( Iterator position );

	// ---------------------------------------------------

	public:
		void	Clear();

	// - DATA MEMBERS ------------------------------------

	private:
		Allocator			_allocator;
		KeyExtractorType	_extractor;
		LeafContainer		_leaves;
		SplitContainer		_splits;

	// ---------------------------------------------------

		friend void	Swap( ArrayBvh&, ArrayBvh& );
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Containers/ArrayBvh.inl>
//------------------------------------------------------------------//