/*==================================================================*\
  SynchronousFileReader.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Noncopyable.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace FileSystem {

	class ETPureAbstractHint SynchronousFileReader : public Utility::Noncopyable {
	// - TYPE PUBLISHING ---------------------------------

	public:
		struct BlockingResult {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			//!	Constructs this @ref BlockingResult instance.
			/*!	@param[in] readResult a ResultCode describing the ultimate result of the read operation, optionally containing information on why a failure occurred.
				@param[in] successfullyReadAmountInBytes Amount of source data successfully read from backing storage before a failure occurred.
				*/
			ETInlineHint BlockingResult( const ::Eldritch2::ErrorCode readResult, const size_t successfullyReadAmountInBytes );
			//!	Constructs this @ref BlockingResult instance.
			/*!	@param[in] itemTemplate the template @ref BlockingResult that will be copied.
				*/
			ETInlineHint BlockingResult( const BlockingResult& itemTemplate ) = default;

		// - DATA MEMBERS ------------------------------------

			//!	The final state of the read operation.
			::Eldritch2::ErrorCode	result;

			//!	Amount of data successfully read before a failure case, or the original size of the write request in the event no errors were encountered.
			size_t					readAmountInBytes;
		};

	// ---------------------------------------------------

		//! Performs a blocking read operation to the output device this @ref SynchronousFileReader uses as its backing.
		virtual BlockingResult	Read( void* const destinationBuffer, const size_t lengthToReadInBytes ) abstract;
		//! Performs a blocking read operation to the output device this @ref SynchronousFileReader uses as its backing.
		virtual BlockingResult	Read( void* const destinationBuffer, const size_t lengthToReadInBytes, const ::Eldritch2::uint64 fileOffsetInBytes ) abstract;

	// ---------------------------------------------------

		//!	Retrieves the total on-disk size of the file/pseudofile being accessed.
		/*!	@returns On disk size of the file, in bytes.
			@remarks Thread-safe.
			*/
		virtual ::Eldritch2::uint64			GetSizeInBytes() const abstract;

		//!	Retrieves the total on-disk size of the file/pseudofile being accessed.
		/*!	@returns On disk size of the file, in bytes, after rounding up the nearest sector boundary.
			@remarks Thread-safe.
			*/
		ETInlineHint ::Eldritch2::uint64	GetRoundedSizeInBytes() const;

		//!	Retrieves the actual/'physical' read size that should be passed to BeginRead() and similar given a 'logical' read size specified in bytes.
		/*!	@returns Real read size, in bytes.
			@remarks Thread-safe.
			*/
		ETInlineHint size_t					GetRoundedReadSizeInBytes( const size_t logicalReadSizeInBytes ) const;

		// Given a desired offset into the file, calculates the actual offset that should be passed to BeginRead() in such a way that access boundaries will be correctly preserved.
		// Thread-safe.
		ETInlineHint ::Eldritch2::uint64	GetOffsetForRead( const ::Eldritch2::uint64 logicalOffsetInBytes ) const;

	// ---------------------------------------------------

		//! Retrieves the physical media sector size for this file/pseudofile.
		/*!	@returns The sector size for the file.
			@remark All read offsets passed to @ref Read(void*, const size_t, const ::Eldritch2::uint64) must be multiples of this value.
			@remark All read lengths passed to @ref Read() must be a multiple of this value.
			@remark Thread-safe.
			@see @ref Read()
			*/
		virtual size_t						GetPhysicalMediaSectorSizeInBytes() const abstract;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		//!	Constructs this @ref SynchronousFileReader instance.
		SynchronousFileReader() = default;

	public:
		//!	Destroys this @ref SynchronousFileReader instance.
		virtual ~SynchronousFileReader() = default;
	};

}	// namespace FileSystem
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <FileSystem/SynchronousFileReader.inl>
//------------------------------------------------------------------//