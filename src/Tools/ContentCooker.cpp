/*==================================================================*\
  ContentCooker.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Tools/ContentCooker.hpp>
#include <iostream>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
#if( ET_PLATFORM_WINDOWS )
	ET_LINK_LIBRARY( "Shlwapi.lib" )
#endif
//------------------------------------------------------------------//

using namespace ::Eldritch2;
using namespace ::Eldritch2Detail;
using namespace ::Eldritch2::Tools;
using namespace ::std;

namespace
{

}	// anonymous namespace

namespace Eldritch2Detail
{

	namespace Tools
	{

		ContentCookerBase::ContentCookerBase()
		{
		}

	// ---------------------------------------------------

		ContentCookerBase::~ContentCookerBase()
		{
		}

	// ---------------------------------------------------

		void ContentCookerBase::WriteString( const SystemChar* const string )
		{
			wcout << string;
		}

	// ---------------------------------------------------

		void ContentCookerBase::WriteErrorString( const SystemChar* const string )
		{
			wcerr << string;
		}

	}	// namespace Tools

}	// namespace Eldritch2