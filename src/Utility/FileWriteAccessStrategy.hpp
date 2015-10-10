/*==================================================================*\
  FileWriteAccessStrategy.hpp
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

namespace Eldritch2 {
namespace FileSystem {

	namespace FileSystem	= ::Eldritch2::FileSystem;
	namespace Utility		= ::Eldritch2::Utility;

// ---------------------------------------------------

	class ETPureAbstractHint FileWriteAccessStrategy : public Utility::PoolAllocatableMixin<Utility::Noncopyable> {
	public:
		struct BlockingResult {
			ETInlineHint BlockingResult();

		// ---------------------------------------------------

			::Eldritch2::ErrorCode	result;
			size_t					writtenAmountInBytes;
		};

	// - ASYNCHRONOUS FILE WRITES-------------------------

		// Begins an (optionally) asynchronous write of the data contained in the buffer identified by the sourceData parameter and spanning
		// the specified length. The buffer itself must persist at least until the next call to FinishWrite() returns.
		virtual ::Eldritch2::ErrorCode	BeginWrite( const void* sourceBuffer, size_t dataLengthInBytes ) abstract;
		// Moves the internal file write pointer to the offset (relative to the beginning of the file) specified in fileOffsetInBytes, 
		// then begins an (optionally) asynchronous write of the data contained in the buffer identified by the sourceData parameter and spanning
		// the specified length. The buffer itself must persist at least until the next call to FinishWrite() returns.
		virtual ::Eldritch2::ErrorCode	BeginWrite( const void* sourceBuffer, size_t dataLengthInBytes, ::Eldritch2::uint64 fileOffsetInBytes ) abstract;

		// Blocks until the write begun by a previous call to BeginWrite() completes, returning the number of bytes successfully copied.
		virtual size_t					FinishWrite() abstract;

	// - SYNCHRONOUS FILE WRITES--------------------------

		// Performs a blocking write to the file with identical semantics to the corresponding BeginWrite() overload. Implementing classes do not
		// necessarily need to override these methods, as they use the BeginWrite() and FinishWrite() functions internally. If a more efficient implementation
		// is possible, then subclasses are encouraged to instead substitute that method.
		virtual BlockingResult	Write( const void* sourceData, size_t dataLengthInBytes );
		// Performs a blocking write to the file with identical semantics to the corresponding BeginWrite() overload. Implementing classes do not
		// necessarily need to override these methods, as they use the BeginWrite() and FinishWrite() functions internally. If a more efficient implementation
		// is possible, then subclasses are encouraged to instead substitute that method.
		virtual BlockingResult	Write( const void* sourceData, size_t dataLengthInBytes, ::Eldritch2::uint64 fileOffsetInBytes );

	// - DISK GEOMETRY QUERIES ---------------------------

		// Retrieves the physical media sector size for this file/pseudofile. All source buffers must be minimally aligned to this value.
		// Thread-safe.
		virtual size_t	GetPhysicalMediaSectorSizeInBytes() const abstract;

	// ---------------------------------------------------

		// Moves the internal file write pointer to immediately past the end of all written data.
		// Not thread-safe.
		virtual void	AdvanceToEnd() abstract;

		// Extends or truncates the on-disk data so the file is exactly the specified size from start to end.
		virtual void	SetFileSize( ::Eldritch2::uint64 fileSizeInBytes ) abstract;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		// Constructs this FileWriteAccessStrategy instance.
		FileWriteAccessStrategy();

		// Destroys this FileWriteAccessStrategy instance.
		~FileWriteAccessStrategy();
	};

}	// namespace Tools
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/FileWriteAccessStrategy.inl>
//------------------------------------------------------------------//