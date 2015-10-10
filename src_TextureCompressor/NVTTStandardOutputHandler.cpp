/*==================================================================*\
  NVTTStandardOutputHandler.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <NVTTStandardOutputHandler.hpp>
#include <Utility/FileWriteAccessStrategy.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2;
using namespace ::Eldritch2::Tools;
using namespace ::Eldritch2::FileSystem;

namespace Eldritch2
{

	namespace Tools
	{

		NVTTStandardOutputHandler::NVTTStandardOutputHandler( FileWriteAccessStrategy& fileAccessor ) : _accessor( fileAccessor )
		{
		}

	// ---------------------------------------------------

		NVTTStandardOutputHandler::~NVTTStandardOutputHandler()
		{
		}

	// ---------------------------------------------------
			
		void NVTTStandardOutputHandler::beginImage( int size, int width, int height, int depth, int face, int miplevel )
		{
			ETUnreferencedParameter( size );
			ETUnreferencedParameter( width );
			ETUnreferencedParameter( height );
			ETUnreferencedParameter( depth );
			ETUnreferencedParameter( face );
			ETUnreferencedParameter( miplevel );
		}

	// ---------------------------------------------------

		void NVTTStandardOutputHandler::endImage()
		{
		}

	// ---------------------------------------------------

		bool NVTTStandardOutputHandler::writeData( const void* data, int size )
		{
			_accessor.FinishWrite();

			return _accessor.BeginWrite( data, size );
		}

	}	// namespace Tools

}	// namespace Eldritch2