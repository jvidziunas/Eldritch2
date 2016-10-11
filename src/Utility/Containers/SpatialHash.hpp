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
#include <Utility/Containers/HashMap.hpp>
#include <Utility/Mpl/VectorTypes.hpp>
#include <Utility/Mpl/Compiler.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	class SpatialHasher {
	// - TYPE PUBLISHING ---------------------------------

	public:
		struct CellIndex {
			Eldritch2::int32	coordinates[3];
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref SpatialHasher instance.
		SpatialHasher( Eldritch2::float32 cellLengthInMeters, Eldritch2::float32 cellHeightInMeters );
	//!	Constructs this @ref SpatialHasher instance.
		SpatialHasher( const SpatialHasher& ) = default;

		~SpatialHasher() = default;

	// ---------------------------------------------------

	public:
		size_t ETSimdCall	operator()( Eldritch2::Float4 position ) const;
		size_t ETSimdCall	operator()( CellIndex cellIndex ) const;

	// - DATA MEMBERS ------------------------------------

	private:
		const Eldritch2::Float4	_inverseResolution;
		const Eldritch2::Float4	_resolution;
	};

// ---------------------------------------------------

	template <typename Value, class Hasher = Eldritch2::SpatialHasher, class Allocator = Eldritch2::ChildAllocator>
	class SpatialHash : public Eldritch2::HashMap<typename Hasher::CellIndex, Value, Hasher, Eldritch2::EqualTo<typename Hasher::CellIndex>, Allocator> {
	// - TYPE PUBLISHING ---------------------------------

	protected:
		using UnderlyingContainer	= Eldritch2::HashMap<typename Hasher::CellIndex, Value, Hasher, Eldritch2::EqualTo<typename Hasher::CellIndex>, Allocator>;

	public:
		using CellIndex				= typename Hasher::CellIndex;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//! Constructs this @ref SpatialHash instance.
		SpatialHash( const HashPredicateType& hashPredicate, const AllocatorType& allocator = AllocatorType() );

		~SpatialHash() = default;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Containers/SpatialHash.inl>
//------------------------------------------------------------------//