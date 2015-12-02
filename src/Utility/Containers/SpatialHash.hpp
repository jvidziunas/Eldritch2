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
#include <Utility/MPL/VectorTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	class SpatialHasher {
	// - TYPE PUBLISHING ---------------------------------

	public:
		struct CellIndex {
			::Eldritch2::int32	coordinates[3];
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//!	Constructs this @ref SpatialHasher instance.
		SpatialHasher( const ::Eldritch2::float32 cellLengthInMeters, const ::Eldritch2::float32 cellHeightInMeters );
		//!	Constructs this @ref SpatialHasher instance.
		SpatialHasher( const SpatialHasher& ) = default;

		~SpatialHasher() = default;

	// ---------------------------------------------------

		ETInlineHint size_t	operator()( const CellIndex cellIndex ) const;
		ETInlineHint size_t	operator()( const ::Eldritch2::Float4 position ) const;

	// - DATA MEMBERS ------------------------------------

	private:
		const ::Eldritch2::Float4	_inverseResolution;
		const ::Eldritch2::Float4	_resolution;
	};

// ---------------------------------------------------

	template <typename StoredObject, class Hasher = ::Eldritch2::SpatialHasher, typename Allocator = ::Eldritch2::ChildAllocator, int loadFactor = 6>
	class SpatialHash : public ::Eldritch2::UnorderedMap<typename Hasher::CellIndex, StoredObject, Hasher, ::Eldritch2::Equals<typename Hasher::CellIndex>, Allocator, loadFactor> {
	// - TYPE PUBLISHING ---------------------------------

	protected:
		using UnderlyingContainer	= ::Eldritch2::UnorderedMap<typename Hasher::CellIndex, StoredObject, Hasher, ::Eldritch2::Equals<typename Hasher::CellIndex>, Allocator, loadFactor>;

	public:
		using CellIndex				= typename Hasher::CellIndex;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref SpatialHash instance.
		ETInlineHint SpatialHash( Hasher&& hasher, AllocatorType&& allocator );
		//! Constructs this @ref SpatialHash instance.
		ETInlineHint explicit SpatialHash( AllocatorType&& allocator = AllocatorType() );

		ETInlineHint ~SpatialHash() = default;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Containers/SpatialHash.inl>
//------------------------------------------------------------------//