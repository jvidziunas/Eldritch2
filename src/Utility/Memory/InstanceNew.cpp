/*==================================================================*\
  OperatorNewOverride.cpp
  ------------------------------------------------------------------
  Purpose:
  Provides externally-visible overrides/overloads for the C++ global
  operator new, used in place of malloc() and friends. The flow of
  execution is then routed through to an allocator object adhering
  to the Lakos allocator model (see the description in
  <Utility/Memory/Allocator.hpp>).

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/InstanceNew.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2;

ETRestrictHint ETNoThrowHint void* operator new( size_t sizeInBytes, Allocator& allocator, const Allocator::AllocationOptions options ) throw() {
	return allocator.Allocate( sizeInBytes, options );
}

// ---------------------------------------------------

ETRestrictHint ETNoThrowHint void* operator new( size_t sizeInBytes, Allocator& allocator, const Allocator::AllocationOptions options, const char* /*file*/, int /*line*/ ) throw() {
	return ::operator new( sizeInBytes, allocator, options );
}

// ---------------------------------------------------

ETRestrictHint ETNoThrowHint void* operator new( size_t sizeInBytes, Allocator& allocator, const Allocator::SizeType alignmentInBytes, const Allocator::AllocationOptions options ) throw() {
	return allocator.Allocate( sizeInBytes, alignmentInBytes, options );
}

// ---------------------------------------------------

ETRestrictHint ETNoThrowHint void* operator new( size_t sizeInBytes, Allocator& allocator, const Allocator::SizeType alignmentInBytes, const Allocator::AllocationOptions options, const char* /*file*/, int /*line*/ ) throw() {
	return ::operator new( sizeInBytes, allocator, alignmentInBytes, options );
}

// ---------------------------------------------------

ETNoThrowHint void operator delete( void* allocatedMemory, Allocator& allocator, const ::Eldritch2::Allocator::AllocationOptions /*options*/ ) throw() {
	allocator.Deallocate( allocatedMemory );
}

// ---------------------------------------------------

ETNoThrowHint void operator delete( void* allocatedMemory, Allocator& allocator, const ::Eldritch2::Allocator::AllocationOptions options, const char* /*file*/, int /*line*/ ) throw() {
	::operator delete( allocatedMemory, allocator, options );
}

// ---------------------------------------------------

ETNoThrowHint void operator delete( void* allocatedMemory, Allocator& allocator, const Allocator::SizeType /*alignmentInBytes*/, const Allocator::AllocationOptions /*options*/ ) throw() {
	allocator.Deallocate( allocatedMemory, ::Eldritch2::AlignedDeallocationSemantics );
}

// ---------------------------------------------------

ETNoThrowHint void operator delete( void* allocatedMemory, Allocator& allocator, const Allocator::SizeType alignmentInBytes, const Allocator::AllocationOptions options, const char* /*file*/, int /*line*/ ) throw() {
	::operator delete( allocatedMemory, allocator, alignmentInBytes, options );
}