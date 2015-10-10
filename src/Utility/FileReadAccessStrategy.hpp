/*==================================================================*\
  FileReadAccessStrategy.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/PoolAllocatable.hpp>
#include <Utility/MPL/Noncopyable.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//

namespace Eldritch2
{

	namespace FileSystem
	{

		namespace FileSystem	= ::Eldritch2::FileSystem;
		namespace Utility		= ::Eldritch2::Utility;

	// ---------------------------------------------------

		class ETPureAbstractHint FileReadAccessStrategy : public Utility::PoolAllocatableMixin< Utility::Noncopyable<FileSystem::FileReadAccessStrategy> >
		{
		public:
			struct BlockingResult
			{
				BlockingResult();

			// ---------------------------------------------------

				::Eldritch2::ErrorCode	result;
				size_t					readAmountInBytes;
			};

		// ---------------------------------------------------

			// Begins an (optionally) asynchronous read of a block of data starting at the current file pointer and continuing for (at most) the specified
			// number of bytes. The buffer itself must persist at least until the next call to FinishRead() returns.
			// Not thread-safe, though it is likely that independent read strategies can safely access the same file.
			virtual ::Eldritch2::ErrorCode	BeginRead( void* destinationBuffer, size_t lengthToReadInBytes ) abstract;
			// Begins an (optionally) asynchronous read of a block of data starting at the file offset passed in through the third parameter and continuing for
			// (at most) the specified number of bytes. The buffer itself must persist at least until the next call to FinishRead() returns.
			// Not thread-safe, though it is likely that independent read strategies can safely access the same file.
			virtual ::Eldritch2::ErrorCode	BeginRead( void* destinationBuffer, size_t lengthToReadInBytes, ::Eldritch2::uint64 fileOffsetInBytes ) abstract;

			// Tests to see whether the previous asynchronous read started by a call to BeginRead() has completed.
			// Thread-safe.
			virtual bool					IsReadComplete() const abstract;

			// Blocks until the read begun by a previous call to BeginRead() completes, returning the number of bytes successfully copied.
			// Not thread-safe, though it is likely that independent read strategies can safely access the same file.
			virtual BlockingResult			FinishRead() abstract;

			// Performs a blocking read from the source file with identical semantics to the corresponding BeginRead() overload. Implementing classes do not
			// necessarily need to override these methods, as they use the BeginRead() and FinishRead() functions internally. If a more efficient implementation
			// is possible, then subclasses are encouraged to instead substitute that method.
			// Not thread-safe, though it is likely that independent read strategies can safely access the same file.
			virtual BlockingResult			Read( void* destinationBuffer, size_t lengthToReadInBytes );
			// Performs a blocking read from the source file with identical semantics to the corresponding BeginRead() overload. Implementing classes do not
			// necessarily need to override these methods, as they use the BeginRead() and FinishRead() functions internally. If a more efficient implementation
			// is possible, then subclasses are encouraged to instead substitute that method.
			// Not thread-safe, though it is likely that independent read strategies can safely access the same file.
			virtual BlockingResult			Read( void* destinationBuffer, size_t lengthToReadInBytes, ::Eldritch2::uint64 fileOffsetInBytes );

		// ---------------------------------------------------

			// Retrieves the total on-disk size of the file/pseudofile being accessed.
			// Thread-safe.
			virtual ::Eldritch2::uint64	GetSizeInBytes() const abstract;

			// Retrieves the total on-disk size of the file/pseudofile being accessed, rounded up to the nearest multiple of the
			// media sector size.
			// Thread-safe.
			::Eldritch2::uint64			GetRoundedSizeInBytes() const;

			// Retrieves the physical media sector size for this file/pseudofile. All destination buffers must be minimally aligned to
			// this value, and all reads must be an even multiple of this.
			// Thread-safe.
			virtual size_t				GetPhysicalMediaSectorSizeInBytes() const abstract;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		protected:
			// Constructs this FileReadAccessStrategy instance.
			FileReadAccessStrategy();

			// Destroys this FileReadAccessStrategy instance.
			~FileReadAccessStrategy();
		};

	}	// namespace FileSystem

}	// namespace Eldritch2