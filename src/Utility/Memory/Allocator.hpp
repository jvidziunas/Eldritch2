/*==================================================================*\
  Allocator.hpp
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
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/CharTypes.hpp>
#include <Utility/MPL/IntTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	enum AllocationDuration {
		Temporary = 0,
		Normal = 1,
		Permanent = 2
	};

// ---

	class ETPureAbstractHint Allocator {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using DifferenceType	= Eldritch2::ptrdiff;
		using SizeType			= size_t;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
	//! Constructs this @ref Allocator instance.
		Allocator( const Eldritch2::Utf8Char* const name );
	//! Constructs this @ref Allocator instance.
		Allocator( const Allocator& ) = default;
		
		~Allocator() = default;

	// - MEMORY ALLOCATION/DEALLOCATION ------------------

	public:
	//! Allocates a contiguous chunk of memory with the specified length and alignment using the passed-in allocation behavior duration.
		virtual ETRestrictHint void*	Allocate( SizeType sizeInBytes, SizeType alignmentInBytes, SizeType offsetInBytes, AllocationDuration duration = AllocationDuration::Normal ) abstract;
	//! Allocates a contiguous chunk of memory with the specified length using the passed-in allocation behavior duration.
		virtual ETRestrictHint void*	Allocate( SizeType sizeInBytes, AllocationDuration duration = AllocationDuration::Normal ) abstract;

	//! Releases a chunk of memory previously allocated by @ref Allocate().
		virtual void					Deallocate( void* const address, SizeType sizeInBytes ) abstract;

	// - DEBUG INFORMATION -------------------------------

	public:
	//! Gets a human-readable string identifying the allocator.
		const Eldritch2::Utf8Char*	GetName() const;

		void						SetName( const Eldritch2::Utf8Char* const name );

	// ---------------------------------------------------

	public:
		Allocator&	operator=( const Allocator& ) = default;

	// - DATA MEMBERS ------------------------------------

	private:
		const Eldritch2::Utf8Char*	_name;
	};

}	// namespace Eldritch2