/*==================================================================*\
  FileWriteAccessStrategy.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2012 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2
{

	namespace FileSystem
	{

		ETInlineHint FileWriteAccessStrategy::BlockingResult::BlockingResult() : result( Errors::NONE ),
																				 writtenAmountInBytes( 0u )
		{
		}

	}	// namespace FileSystem

}	// namespace Eldritch2