/*==================================================================*\
  Allocator.inl
  ------------------------------------------------------------------
  Purpose:
  Definition of a basic allocator object that can be used to manage
  memory. Note that this class uses the slightly different (and
  vastly superior) per-instance Lakos allocator model (see "Towards
  a Better Allocator Model" for details) as opposed to the standard
  per-type C++ model. If compatibility with the old style allocator
  is of concern, see the accompanying wrapper class defined in
  ETStdAllocator.hpp.

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename ObjectType>
	ETForceInlineHint ETNoAliasHint typename ObjectType* AllocateTemporaryArray( ::Eldritch2::Allocator& allocator, size_t arraySizeInObjects ) {
		return static_cast<ObjectType*>(allocator.Allocate( arraySizeInObjects * sizeof(ObjectType), Allocator::AllocationOption::TEMPORARY_ALLOCATION ));
	}

// ---------------------------------------------------

	template <typename ObjectType>
	ETForceInlineHint ETNoAliasHint typename ObjectType* AllocateTemporaryArray( ::Eldritch2::Allocator& allocator, size_t arraySizeInObjects, size_t arrayAlignmentInBytes ) {
		return static_cast<ObjectType*>(allocator.Allocate( arraySizeInObjects * sizeof(ObjectType), arrayAlignmentInBytes, Allocator::AllocationOption::TEMPORARY_ALLOCATION ));
	}

// ---------------------------------------------------

	template <typename ObjectType>
	ETForceInlineHint void DeleteTemporaryArray( ::Eldritch2::Allocator& allocator, ObjectType* const allocatedArray ) {
		allocator.Deallocate( allocatedArray );
	}

// ---------------------------------------------------

	template <typename Allocator, typename ObjectType>
	ETForceInlineHint void DeleteTemporaryArray( ::Eldritch2::Allocator& allocator, ObjectType* const allocatedArray, const ::Eldritch2::Allocator::AlignedDeallocationSemantics ) {
		allocator.Deallocate( allocatedArray, ::Eldritch2::AlignedDeallocationSemantics );
	}

// ---------------------------------------------------

	template <typename Object>
	ETForceInlineHint void Allocator::Delete( Object& object ) {
		this->Deallocate( (object.~Object(), &object) );
	}

// ---------------------------------------------------

	template <typename Object>
	ETForceInlineHint void Allocator::Delete( Object* const object ) {
		if( nullptr != object ) {
			this->Delete( *object );
		}
	}

// ---------------------------------------------------

	template <typename Object>
	ETForceInlineHint void Allocator::Delete( Object& object, const ::Eldritch2::Allocator::AlignedDeallocationSemantics semantics ) {
		this->Deallocate( (object.~Object(), &object), semantics );
	}

// ---------------------------------------------------

	template <typename Object>
	ETForceInlineHint void Allocator::Delete( Object* const object, const ::Eldritch2::Allocator::AlignedDeallocationSemantics semantics ) {
		if( nullptr != object ) {
			this->Delete( *object, semantics );
		}
	}

}	// namespace Eldritch2