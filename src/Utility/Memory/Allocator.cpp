/*==================================================================*\
  Allocator.cpp
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


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Concurrency/AtomicOperations.hpp>
#include <Utility/Memory/Allocator.hpp>
#include <Utility/Memory/MemStdLib.hpp>
#include <Logging/Logger.hpp>
#include <memory>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2;

namespace Eldritch2 {

	const Allocator::AlignedDeallocationSemantics AlignedDeallocationSemantics;

// ---------------------------------------------------

	Allocator::Allocator( const UTF8Char* const name ) : _name( name ) {}

// ---------------------------------------------------

	Allocator::Allocator( Allocator&& allocator ) : _name( allocator._name ) {}

// ---------------------------------------------------

	ETRestrictHint void* Allocator::Allocate( const SizeType sizeInBytes, const SizeType alignmentInBytes, const AllocationOptions options ) {
		const SizeType	allocationSize( this->EstimateActualAllocationSizeInBytes( sizeInBytes, alignmentInBytes ) );
		void* const		allocation( this->Allocate( allocationSize, options ) );
		
		if( nullptr != allocation ) {
			size_t	remainingSpace;
			void*	userPointer( static_cast<void**>(allocation) + 1 );

			if( ::std::align( alignmentInBytes, allocationSize, userPointer, remainingSpace ) ) {
				// Store the 'real' pointer that will be internally fed back to the allocator just before what the user sees.
				static_cast<void**>(userPointer)[-1] = allocation;
				ETRuntimeAssert( GetAllocationPointerFromAlignedUserPointer( userPointer ) == allocation );

				return userPointer;
			}

			this->Deallocate( allocation );
		}

		return nullptr;
	}

// ---------------------------------------------------

	void Allocator::Deallocate( void* address, AlignedDeallocationSemantics ) {
		return this->Deallocate( GetAllocationPointerFromAlignedUserPointer( address ) );
	}

// ---------------------------------------------------

	ETNoAliasHint void* Allocator::GetAllocationPointerFromAlignedUserPointer( void* userPointer ) {
		return static_cast<void**>(userPointer)[-1];
	}

// ---------------------------------------------------

	Allocator::SizeType Allocator::EstimateActualAllocationSizeInBytes( const SizeType allocationSizeInBytes, const SizeType alignmentInBytes ) const {
		return static_cast<SizeType>(allocationSizeInBytes + sizeof(void*) + (Max( alignmentInBytes, sizeof(void*) ) - 1u));
	}

// ---------------------------------------------------

	const UTF8Char* Allocator::GetName() const {
		return _name;
	}

}	// namespace Eldritch2