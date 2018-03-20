/*==================================================================*\
  PropertyRegistrar.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Core {

	template <typename PodValue>
	PropertyRegistrar& PropertyRegistrar::WhenPropertyChanged( Utf8Literal name, PodValue& value ) {
		return WhenPropertyChanged( name, [&value] ( Range<const Utf8Char*> stringValue ) {
			::boost::iostreams::basic_array_source<Utf8Char>	source( stringValue.Begin(), stringValue.End() );
			::boost::iostreams::stream<decltype( source )>		stream( source );

			stream >> value;
		} );
	}

}	// namespace Core
}	// namespace Eldritch2

