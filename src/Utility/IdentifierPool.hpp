/*==================================================================*\
  IdentifierPool.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/ResizableArray.hpp>
#include <Utility/Pair.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Detail {

	template <typename Identifier>
	class IdentifierPoolBase {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using IdentifierType = Identifier;

	// ---

		struct IdentifierRange {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref IdentifierRange instance.
			IdentifierRange( IdentifierType firstIdentifier, IdentifierType lastIdentifier );
		//!	Constructs this @ref IdentifierRange instance.
			IdentifierRange() = default;

			~IdentifierRange() = default;

		// ---------------------------------------------------

		public:
			bool	CanMergeForwardWith( const IdentifierRange& range ) const;

		// ---------------------------------------------------

		public:
			IdentifierType	GetSize() const;

		// ---------------------------------------------------

		public:
			bool	CanAccomodateRangeOfSize( IdentifierType size ) const;

			bool	Contains( IdentifierType identifier ) const;

			bool	IsEmpty() const;

		// - DATA MEMBERS ------------------------------------

		public:
			IdentifierType	firstIdentifier;
			IdentifierType	lastIdentifier;
		};
	};

}	// namespace Detail

	template <typename Identifier, class Allocator = Eldritch2::ChildAllocator>
	class IdentifierPool : Detail::IdentifierPoolBase<Identifier> {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using IdentifierRange	= typename IdentifierPoolBase<Identifier>::IdentifierRange;
		using IdentifierType	= typename IdentifierPoolBase<Identifier>::IdentifierType;
		using AllocatorType		= Allocator;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref IdentifierPool instance.
		explicit IdentifierPool( std::initializer_list<IdentifierRange> ranges, const AllocatorType& allocator = AllocatorType() );
	//!	Constructs this @ref IdentifierPool instance.
		explicit IdentifierPool( IdentifierRange initialRange, const AllocatorType& allocator = AllocatorType() );
	//!	Constructs this @ref IdentifierPool instance.
		explicit IdentifierPool( IdentifierType maximumIdentifier, const AllocatorType& allocator = AllocatorType() );
	//!	Constructs this @ref IdentifierPool instance.
		explicit IdentifierPool( const AllocatorType& allocator = AllocatorType() );
	//!	Constructs this @ref IdentifierPool instance.
		IdentifierPool( IdentifierPool&& );
	//!	Disable copying.
		IdentifierPool( const IdentifierPool& ) = delete;

	//!	Destroys this @ref IdentifierPool instance.
		~IdentifierPool() = default;

	// ---------------------------------------------------

	public:
		Eldritch2::Pair<Identifier, bool>	AllocateIdentifier();

		Eldritch2::Pair<Identifier, bool>	AllocateRangeOfSize( IdentifierType rangeSizeInElements );

		void								ReleaseRange( IdentifierRange range );

		void								ReleaseIdentifier( IdentifierType identifier );

	// ---------------------------------------------------

	public:
		bool	IsEmpty() const;

	// ---------------------------------------------------

	//!	Disable assignment.
		IdentifierPool&	operator=( const IdentifierPool& ) = delete;

	// ---------------------------------------------------

	public:
		IdentifierType	GetLargestAvailableBlockLengthInElements() const;

		template <typename Predicate>
		void			ForEachAvailableIdentifierRange( Predicate predicate ) const;

	// - DATA MEMBERS ------------------------------------

	private:
		Eldritch2::ResizableArray<IdentifierRange, AllocatorType>	_availableRanges;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/IdentifierPool.inl>
//------------------------------------------------------------------//