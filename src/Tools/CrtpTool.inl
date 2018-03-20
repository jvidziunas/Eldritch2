/*==================================================================*\
  CrtpTool.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Containers/Range.hpp>
#include <Common/ErrorCode.hpp>
//------------------------------------------------------------------//
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Tools {
namespace Detail {

	template <typename Argument>
	Tool::OptionRegistrar& Tool::OptionRegistrar::Register( const Utf8Char* const name, Argument& argument ) {
		_options.EmplaceBack( name, [&argument] ( Range<const Utf8Char*> value ) -> int {
			using namespace ::boost::iostreams;

		// ---

			basic_array_source<Utf8Char>	source( value.Begin(), value.End() );
			stream<decltype(source)>		stream( source );
			Argument						optionValue;

			stream >> optionValue;

			if (stream.fail()) {
				return AsPosixInt( Error::InvalidParameter );
			}

			argument = optionValue;

			return 0;
		} );

		return *this;
	}

// ---------------------------------------------------

	template <typename Argument>
	Tool::OptionRegistrar& Tool::OptionRegistrar::Register( const Utf8Char* const name, const Utf8Char* const shortName, Argument& argument ) {
		Register( name, argument );
		return Register( shortName, argument );
	}

// ---------------------------------------------------

	template <typename ArgumentIterator>
	int Tool::OptionRegistrar::Dispatch( ArgumentIterator argumentBegin, ArgumentIterator argumentEnd ) {
		auto	argumentCount( eastl::distance( argumentBegin, argumentEnd ) );
		auto	flattenedArgs( static_cast<Utf8Char**>(_alloca( sizeof(Utf8Char*) * argumentCount )) );

		eastl::uninitialized_copy( argumentBegin, argumentEnd, flattenedArgs );

		return Dispatch( { flattenedArgs, flattenedArgs + argumentCount } );
	}

}	// namespace Detail

	template <typename ImplementingType>
	template <typename ArgumentIterator>
	int	CrtpTool<ImplementingType>::Run( ArgumentIterator argumentBegin, ArgumentIterator argumentEnd ) {
		{	OptionRegistrar	visitor;

		//	Register tool options.
			static_cast<ImplementingType&>(*this).RegisterOptions( visitor );

			visitor.Dispatch( argumentBegin, argumentEnd );
		}

		return static_cast<ImplementingType&>(*this).Process();
	}

}	// namespace Tools
}	// namespace Eldritch2

