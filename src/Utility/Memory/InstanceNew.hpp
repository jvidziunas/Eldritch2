/*==================================================================*\
  AlignmentNew.hpp
  ------------------------------------------------------------------
  Purpose:
  Declares some additional overloads for the C++ global operator new
  to provide intrinsic support for allocation alignment. Definitions
  for these new functions and replacements for unaligned allocations
  can be found in OperatorNewOverride.cpp;

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
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