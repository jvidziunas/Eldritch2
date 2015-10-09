/*==================================================================*\
  SpatialHash.hpp
  ------------------------------------------------------------------
  Purpose:

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/RDESTLAllocatorAdapterMixin.hpp>
#include <Utility/Memory/ChildAllocator.hpp>
//------------------------------------------------------------------//
#include <rdestl/hash_map.h>
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
	class SpatialHash {
	// - TYPE PUBLISHING ---------------------------------

	protected:
		typedef ::Eldritch2Detail::RDESTLAllocatorAdapterMixin<Allocator>	PrivateAllocator;
		typedef typename Hasher::CellIndex								CellIndex;
		typedef ::rde::hash_map<CellIndex,
								StoredObject,
								Hasher,
								4,
								::rde::equal_to<CellIndex>,
								PrivateAllocator>						UnderlyingContainer;
		typedef typename UnderlyingContainer::size_type					SizeType;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		// Constructs this SpatialHash instance.
		template <typename... AllocatorConstructorArguments>
		ETInlineHint SpatialHash( const SizeType						initialBucketCount,
								  const ::Eldritch2::float32			horizontalCellResolution,
								  const ::Eldritch2::float32			verticalCellResolution,
								  AllocatorConstructorArguments&&...	allocatorConstructorArguments );
		// Constructs this SpatialHash instance.
		template <typename... AllocatorConstructorArguments>
		ETInlineHint SpatialHash( const SizeType						initialBucketCount,
								  const Hasher&							hasher,
								  const ::Eldritch2::float32			horizontalCellResolution,
								  const ::Eldritch2::float32			verticalCellResolution,
								  AllocatorConstructorArguments&&...	allocatorConstructorArguments );

		// Destroys this SpatialHash instance.
		ETInlineHint ~SpatialHash();

	// - ELEMENT ACCESS ----------------------------------

		ETInlineHint StoredObject&			operator[]( const ::Eldritch2::Float4 position );
		ETInlineHint const StoredObject&	operator[]( const ::Eldritch2::Float4 position ) const;

	// ---------------------------------------------------

		template <typename Predicate>
		ETInlineHint void	RemoveIf( Predicate predicate );

		ETInlineHint void	Clear();

	// - CAPACITY QUERY ----------------------------------

		ETInlineHint void		Reserve( const SizeType minimumSizeHint );

		ETInlineHint SizeType	BucketCount() const;

		ETInlineHint SizeType	NonemptyBucketCount() const;

		ETInlineHint SizeType	UsedMemory() const;

	// - ALLOCATOR ACCESS --------------------------------

		ETInlineHint const Allocator&	GetAllocator() const;

	// - DATA MEMBERS ------------------------------------

	private:
		const ::Eldritch2::Float4	_inverseResolution;
		UnderlyingContainer			_underlyingContainer;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Containers/SpatialHash.inl>
//------------------------------------------------------------------//