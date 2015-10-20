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
#include <Utility/MPL/Noncopyable.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Utility {

	namespace Utility	= ::Eldritch2::Utility;

// ---------------------------------------------------

	template <typename Identifier, class Allocator = ::Eldritch2::ChildAllocator>
	class IdentifierPool : public Utility::Noncopyable {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using AllocatorType		= Allocator;
		using IdentifierType	= Identifier;
		using DifferenceType	= IdentifierType;

	// ---

		struct AvailableRange {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			//!	Constructs this @ref AvailableRange instance.
			ETForceInlineHint AvailableRange( IdentifierType initialElement, IdentifierType endElement );
			//!	Constructs this @ref AvailableRange instance.
			ETForceInlineHint AvailableRange() = default;

			//!	Destroys this @ref AvailableRange instance.
			ETForceInlineHint ~AvailableRange() = default;

		// ---------------------------------------------------

			ETForceInlineHint bool	ContainsElements() const;

		// - DATA MEMBERS ------------------------------------

			IdentifierType	begin;
			IdentifierType	end;
		};

	// ---

		struct AllocationResult {
			IdentifierType	identifier;
			bool			successful;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//!	Constructs this @ref IdentifierPool instance.
		ETInlineHint IdentifierPool( IdentifierType maximumIdentifier, AllocatorType&& allocator = AllocatorType() );

		//!	Destroys this @ref IdentifierPool instance.
		~IdentifierPool() = default;

	// ---------------------------------------------------

		AllocationResult	AllocateIdentifier();

		AllocationResult	AllocateIdentifierRange( const DifferenceType rangeSizeInElements );

		void				ReleaseIdentifier( const IdentifierType identifier );

		void				ReleaseIdentifierRange( const IdentifierType rangeBegin, const IdentifierType rangeEnd );

	// ---------------------------------------------------

		DifferenceType	GetLargestAvailableBlockLengthInElements() const;

		template <typename Predicate>
		Predicate		ForEachAvailableIdentifierRange( Predicate predicate ) const;

	// - DATA MEMBERS ------------------------------------

	private:
		::Eldritch2::ResizableArray<AvailableRange, AllocatorType>	_availableRanges;
	};

}	// namespace Utility
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/IdentifierPool.inl>
//------------------------------------------------------------------//