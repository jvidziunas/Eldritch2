/*==================================================================*\
  ConfigurationRegistrar.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/Range.hpp>
//------------------------------------------------------------------//
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Configuration {

	template <typename PodValue>
	ConfigurationRegistrar& ConfigurationRegistrar::Register( Eldritch2::Utf8Literal name, PodValue& value ) {
		return Register( name, [&] ( Eldritch2::Range<const Eldritch2::Utf8Char*> stringValue ) {
			::boost::iostreams::basic_array_source<Eldritch2::Utf8Char>	source( stringValue.Begin(), stringValue.End() );
			::boost::iostreams::stream<decltype( source )>				inStream( source );

			inStream >> value;
		} );
	}

}	// namespace Configuration
}	// namespace Eldritch2

