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
	public:
		typedef Allocator		AllocatorType;
		typedef Identifier		IdentifierType;
		typedef IdentifierType	DifferenceType;

	// ---

		struct AllocationResult {
			IdentifierType	identifier;
			bool			successful;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		// Constructs this IdentifierPool instance.
		template <typename... AllocatorConstructorArguments>
		ETInlineHint IdentifierPool( IdentifierType maximumIdentifier, AllocatorConstructorArguments&&... allocatorConstructorArguments );

		// Destroys this IdentifierPool instance.
		~IdentifierPool();

	// ---------------------------------------------------

		AllocationResult	AllocateIdentifier();

		AllocationResult	AllocateIdentifierRange( const DifferenceType rangeSizeInElements );

		void				ReleaseIdentifier( const IdentifierType identifier );

		void				ReleaseIdentifierRange( const IdentifierType rangeBegin, const IdentifierType rangeEnd );

	// ---------------------------------------------------

		DifferenceType	GetLargestAvailableBlockLengthInElements() const;

		template <typename Predicate>
		Predicate		ForEachAvailableIdentifierRange( Predicate predicate ) const;

	// ---------------------------------------------------

	private:
		struct AvailableRange {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			// Constructs this AvailableRange instance.
			ETForceInlineHint AvailableRange( IdentifierType initialElement, IdentifierType endElement );
			// Constructs this AvailableRange instance.
			ETForceInlineHint AvailableRange();

		// ---------------------------------------------------

			ETForceInlineHint bool	ContainsElements() const;

		// - DATA MEMBERS ------------------------------------

			IdentifierType	begin;
			IdentifierType	end;
		};

		typedef ::Eldritch2::ResizableArray<AvailableRange, AllocatorType>	AvailableRangeCollection;

	// - DATA MEMBERS ------------------------------------

		AvailableRangeCollection	_availableRanges;
	};

}	// namespace Utility
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/IdentifierPool.inl>
//------------------------------------------------------------------//