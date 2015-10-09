/*==================================================================*\
  FileWriteAccessStrategy.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/FileWriteAccessStrategy.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2;
using namespace ::Eldritch2::FileSystem;

namespace Eldritch2
{

	namespace FileSystem
	{

		FileWriteAccessStrategy::FileWriteAccessStrategy()
		{
		}

	// ---------------------------------------------------

		FileWriteAccessStrategy::~FileWriteAccessStrategy()
		{
		}

	// ---------------------------------------------------

		FileWriteAccessStrategy::BlockingResult FileWriteAccessStrategy::Write( const void* sourceData, size_t dataLengthInBytes )
		{
			BlockingResult	result;
			
			result.result = BeginWrite( sourceData, dataLengthInBytes );

			if( ETBranchLikelyHint( result.result ) )
			{
				result.writtenAmountInBytes = FinishWrite();
			}

			return result;
		}

	// ---------------------------------------------------

		FileWriteAccessStrategy::BlockingResult FileWriteAccessStrategy::Write( const void* sourceData, size_t dataLengthInBytes, uint64 fileOffsetInBytes )
		{
			BlockingResult	result;
			
			result.result = BeginWrite( sourceData, dataLengthInBytes, fileOffsetInBytes );

			if( ETBranchLikelyHint( result.result ) )
			{
				result.writtenAmountInBytes = FinishWrite();
			}

			return result;
		}

	}	// namespace FileSystem

}	// namespace Eldritch2