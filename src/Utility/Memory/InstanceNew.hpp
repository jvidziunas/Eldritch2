/*==================================================================*\
  AlignmentNew.hpp
  ------------------------------------------------------------------
  Purpose:
  Declares some additional overloads for the C++ global operator new
  to provide intrinsic support for allocation alignment. Definitions
  for these new functions and replacements for unaligned allocations
  can be found in OperatorNewOverride.cpp;

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/Allocator.hpp>
//------------------------------------------------------------------//

ETRestrictHint ETNoThrowHint void*	operator new( size_t sizeInBytes, ::Eldritch2::Allocator& allocator, const ::Eldritch2::Allocator::AllocationOptions options ) throw();
ETRestrictHint ETNoThrowHint void*	operator new( size_t sizeInBytes, ::Eldritch2::Allocator& allocator, const ::Eldritch2::Allocator::AllocationOptions options, const char* file, int line ) throw();

ETRestrictHint ETNoThrowHint void*	operator new( size_t sizeInBytes, ::Eldritch2::Allocator& allocator, const ::Eldritch2::Allocator::SizeType alignmentInBytes, const Eldritch2::Allocator::AllocationOptions options ) throw();
ETRestrictHint ETNoThrowHint void*	operator new( size_t sizeInBytes, ::Eldritch2::Allocator& allocator, const ::Eldritch2::Allocator::SizeType alignmentInBytes, const Eldritch2::Allocator::AllocationOptions options, const char* file, int line ) throw();

// ---------------------------------------------------

ETNoThrowHint void					operator delete( void* allocatedMemory, ::Eldritch2::Allocator& allocator, const ::Eldritch2::Allocator::AllocationOptions options ) throw();
ETNoThrowHint void					operator delete( void* allocatedMemory, ::Eldritch2::Allocator& allocator, const ::Eldritch2::Allocator::AllocationOptions options, const char* file, int line ) throw();

ETNoThrowHint void					operator delete( void* allocatedMemory, ::Eldritch2::Allocator& allocator, const ::Eldritch2::Allocator::SizeType alignmentInBytes, const ::Eldritch2::Allocator::AllocationOptions options ) throw();
ETNoThrowHint void					operator delete( void* allocatedMemory, ::Eldritch2::Allocator& allocator, const ::Eldritch2::Allocator::SizeType alignmentInBytes, const ::Eldritch2::Allocator::AllocationOptions options, const char* file, int line ) throw();

#define ETDefineClassInstanceNew() ETInlineHint void*	operator new( size_t sizeInBytes, ::Eldritch2::Allocator& allocator, const ::Eldritch2::Allocator::AllocationOptions options ) { return ::operator new( sizeInBytes, allocator, options ); } \
								   ETInlineHint void*	operator new( size_t sizeInBytes, ::Eldritch2::Allocator& allocator, const ::Eldritch2::Allocator::SizeType alignmentInBytes, const ::Eldritch2::Allocator::AllocationOptions options ) { return ::operator new( sizeInBytes, allocator, alignmentInBytes, options ); } \
								   ETInlineHint void	operator delete( void* allocatedMemory, ::Eldritch2::Allocator& allocator, const ::Eldritch2::Allocator::AllocationOptions options ) { ::operator delete( allocatedMemory, allocator, options ); } \
								   ETInlineHint void	operator delete( void* allocatedMemory, ::Eldritch2::Allocator& allocator, const ::Eldritch2::Allocator::SizeType alignmentInBytes, const ::Eldritch2::Allocator::AllocationOptions options ) { ::operator delete( allocatedMemory, allocator, alignmentInBytes, options ); }