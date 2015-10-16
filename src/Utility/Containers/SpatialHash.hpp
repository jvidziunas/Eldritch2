/*==================================================================*\
  SpatialHash.hpp
  ------------------------------------------------------------------
  Purpose:

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/UnorderedMap.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	class	Float4;
}

namespace Eldritch2 {

	struct SpatialHasher {
		struct CellIndex {
			::Eldritch2::uint32	coordinates[3];
		};

	// ---

		static CellIndex	CellIndexForPosition( const ::Eldritch2::Float4 position, const ::Eldritch2::Float4 inverseResolution );

	// ---------------------------------------------------

		ETInlineHint size_t	operator()( const CellIndex& cellIndex );
	};

// ---------------------------------------------------

	template <typename StoredObject, class Hasher = ::Eldritch2::SpatialHasher, typename Allocator = ::Eldritch2::ChildAllocator>
	class SpatialHash : public ::Eldritch2::UnorderedMap<typename Hasher::CellIndex, StoredObject, Hasher, ::rde::equal_to<typename Hasher::CellIndex>, Allocator> {
	// - TYPE PUBLISHING ---------------------------------

	protected:
		using UnderlyingContainer	= ::Eldritch2::UnorderedMap<typename Hasher::CellIndex, StoredObject, Hasher, ::rde::equal_to<typename Hasher::CellIndex>, Allocator>;

	public:
		using CellIndex				= typename Hasher::CellIndex;
		using SizeType				= typename UnderlyingContainer::size_type;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref SpatialHash instance.
		ETInlineHint SpatialHash( const SizeType initialBucketCount, const ::Eldritch2::float32 horizontalCellResolution, const ::Eldritch2::float32 verticalCellResolution, AllocatorType&& allocator = AllocatorType() );
		//! Constructs this @ref SpatialHash instance.
		ETInlineHint SpatialHash( const SizeType initialBucketCount, const Hasher& hasher, const ::Eldritch2::float32 horizontalCellResolution, const ::Eldritch2::float32 verticalCellResolution, AllocatorType&& allocator = AllocatorType() );

		//! Destroys this @ref SpatialHash instance.
		ETInlineHint ~SpatialHash() = default;

	// - ELEMENT ACCESS ----------------------------------

		ETInlineHint StoredObject&			operator[]( const ::Eldritch2::Float4 position );
		ETInlineHint const StoredObject&	operator[]( const ::Eldritch2::Float4 position ) const;

	// - DATA MEMBERS ------------------------------------

	private:
		const ::Eldritch2::Float4	_inverseResolution;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Containers/SpatialHash.inl>
//------------------------------------------------------------------//