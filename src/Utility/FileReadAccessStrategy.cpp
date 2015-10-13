/*==================================================================*\
  FileReadAccessStrategy.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/FileReadAccessStrategy.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2;
using namespace ::Eldritch2::FileSystem;

namespace Eldritch2
{

	namespace FileSystem
	{
		FileReadAccessStrategy::BlockingResult::BlockingResult() : result( Error::NONE ),
																   readAmountInBytes( 0u )
		{
		}

	// ---------------------------------------------------

		FileReadAccessStrategy::FileReadAccessStrategy()
		{
		}

	// ---------------------------------------------------

		FileReadAccessStrategy::~FileReadAccessStrategy()
		{
		}

	// ---------------------------------------------------

		FileReadAccessStrategy::BlockingResult FileReadAccessStrategy::Read( void* destinationBuffer, size_t lengthToReadInBytes )
		{
			BlockingResult	result;
			
			result.result = BeginRead( destinationBuffer, lengthToReadInBytes );

			if( ETBranchLikelyHint( result.result ) )
			{
				result = FinishRead();
			}

			return result;
		}

	// ---------------------------------------------------

		FileReadAccessStrategy::BlockingResult FileReadAccessStrategy::Read( void* destinationBuffer, size_t lengthToReadInBytes, uint64 fileOffsetInBytes )
		{
			BlockingResult	result;
			
			result.result = BeginRead( destinationBuffer, lengthToReadInBytes, fileOffsetInBytes );

			if( ETBranchLikelyHint( result.result ) )
			{
				result = FinishRead();
			}

			return result;
		}

	// ---------------------------------------------------

		uint64 FileReadAccessStrategy::GetRoundedSizeInBytes() const
		{
			const uint64	sectorSize( GetPhysicalMediaSectorSizeInBytes() );

			return( ( ( GetSizeInBytes() + sectorSize - 1u) / sectorSize ) * sectorSize );
		}

	}	// namespace FileSystem

}	// namespace Eldritch2