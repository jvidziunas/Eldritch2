/*==================================================================*\
  ContentCookerStrategy.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Tools/ContentCookerStrategy.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2;
using namespace ::Eldritch2::Tools;

namespace Eldritch2
{

	namespace Tools
	{

		ContentCookerStrategy::ContentCookerStrategy()
		{
		}

	// ---------------------------------------------------

		ContentCookerStrategy::~ContentCookerStrategy()
		{
		}

	// ---------------------------------------------------

		void ContentCookerStrategy::ProcessContentExtensionHint( const SystemChar* contentPath, const SystemChar* contentExtension )
		{
			ETUnreferencedParameter( contentPath );
			ETUnreferencedParameter( contentExtension );
		}

	}	// namespace Tools

}	// namespace Eldritch2