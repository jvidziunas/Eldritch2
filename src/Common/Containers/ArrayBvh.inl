/*==================================================================*\
  ArrayBvh.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Value, typename KeyExtractor, class Allocator>
	ETInlineHint ArrayBvh<Value, KeyExtractor, Allocator>::ArrayBvh(
		const KeyExtractorType& extractor,
		const AllocatorType& allocator
	) : _allocator( allocator ),
		_leaves( ChildAllocator{ _allocator, "Array BVH Leaf Allocator" } ),
		_splits( ChildAllocator{ _allocator, "Array BVH Split Allocator" } ),
		_extractor( extractor ) {
	}

// ---------------------------------------------------

	template <typename Value, typename KeyExtractor, class Allocator>
	ETInlineHint void ArrayBvh<Value, KeyExtractor, Allocator>::Insert( const ValueType& value ) {
		_values.Append( value );
	}

// ---------------------------------------------------

	template <typename Value, typename KeyExtractor, class Allocator>
	ETInlineHint typename ArrayBvh<Value, KeyExtractor, Allocator>::Iterator ArrayBvh<Value, KeyExtractor, Allocator>::Find( const ValueType& value ) {
		return eastl::find_if( _leaves.Begin(), _leaves.End(), [&value] ( const LeafType& leaf ) {
			return leaf.second == &value;
		} );
	}

// ---------------------------------------------------

	template <typename Value, typename KeyExtractor, class Allocator>
	ETInlineHint typename ArrayBvh<Value, KeyExtractor, Allocator>::Iterator ArrayBvh<Value, KeyExtractor, Allocator>::Erase( Iterator position ) {
		_leaves.Erase( position, Eldritch2::UnorderedSemantics );
	}

// ---------------------------------------------------

	template <typename Value, typename KeyExtractor, class Allocator>
	ETInlineHint void ArrayBvh<Value, KeyExtractor, Allocator>::Clear() {
		_leaves.Clear();
		_splits.Clear();
	}

// ---------------------------------------------------

	template <typename Value, typename KeyExtractor, class Allocator>
	ETInlineHint void Swap( ArrayBvh<Value, KeyExtractor, Allocator>& bvh0, ArrayBvh<Value, KeyExtractor, Allocator>& bvh1 ) {
		Swap( bvh0._allocator,	bvh1._allocator );
		Swap( bvh0._extractor,	bvh1._extractor );
		Swap( bvh0._leaves,		bvh1._leaves );
		Swap( bvh0._splits,		bvh1._splits );
	}

}	// namespace Eldritch2