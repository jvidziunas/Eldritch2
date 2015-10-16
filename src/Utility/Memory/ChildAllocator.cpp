/*==================================================================*\
  ChildAllocator.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/ChildAllocator.hpp>
#include <Utility/Memory/NullAllocator.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//
#include <utility>
//------------------------------------------------------------------//

using namespace ::Eldritch2;

namespace {

	static const UTF8Char	anonymousAllocatorName[] = UTF8L("Anonymous Child Allocator");

}	// anonymous namespace

namespace Eldritch2 {

	ChildAllocator::ChildAllocator( Allocator& allocator, const UTF8Char* const name ) : Allocator( name ), _parent( allocator ) {}

// ---------------------------------------------------

#if( ET_DEBUG_MODE_ENABLED )
	ChildAllocator::ChildAllocator( ChildAllocator& allocator, const ::Eldritch2::UTF8Char* const name ) : Allocator( name ), _parent( allocator ) {
		ETRuntimeAssert( !allocator.IsAnonymous() );
	}
#endif

// ---------------------------------------------------

	ChildAllocator::ChildAllocator( const ChildAllocator& allocator ) : Allocator( allocator.GetName() ), _parent( allocator.GetParent() ) {}

// ---------------------------------------------------

	ChildAllocator::ChildAllocator( ChildAllocator&& allocator ) : Allocator( ::std::move( allocator ) ), _parent( allocator._parent ) {}

// ---------------------------------------------------

	ChildAllocator::ChildAllocator() : Allocator( anonymousAllocatorName ), _parent( NullAllocator::GetInstance() ) {}

// ---------------------------------------------------
		
	ETRestrictHint void* ChildAllocator::Allocate( const SizeType sizeInBytes, const AllocationOptions options ) {
		return _parent.Allocate( sizeInBytes, options );
	}

// ---------------------------------------------------

	ETRestrictHint void* ChildAllocator::Allocate( const SizeType sizeInBytes, const SizeType alignmentInBytes, const AllocationOptions options ) {
		return _parent.Allocate( sizeInBytes, alignmentInBytes, options );
	}

// ---------------------------------------------------
		
	ETRestrictHint void* ChildAllocator::Reallocate( void* const address, const SizeType newSizeInBytes, const ReallocationOptions options ) {
		return _parent.Reallocate( address, newSizeInBytes, options );
	}

// ---------------------------------------------------

	ETRestrictHint void* ChildAllocator::Reallocate( void* const address, const SizeType newSizeInBytes, const SizeType alignmentInBytes, const ReallocationOptions options ) {
		return _parent.Reallocate( address, newSizeInBytes, alignmentInBytes, options );
	}

// ---------------------------------------------------
		
	void ChildAllocator::Deallocate( void* const address ) {
		_parent.Deallocate( address );
	}

// ---------------------------------------------------

	void ChildAllocator::Deallocate( void* const address, const AlignedDeallocationSemantics semantics ) {
		_parent.Deallocate( address, semantics );
	}

// ---------------------------------------------------

#if( ET_DEBUG_MODE_ENABLED )
	bool ChildAllocator::IsAnonymous() const {
		return (anonymousAllocatorName == _name) && (&NullAllocator::GetInstance() == &_parent);
	}
#endif

}	// namespace Eldritch2