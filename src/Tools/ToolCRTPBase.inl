/*==================================================================*\
  ToolCRTPBase.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/Range.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Tools {
namespace Detail {

	template <typename Argument>
	ToolBase::OptionRegistrationVisitor& ToolBase::OptionRegistrationVisitor::AddTypedArgument( const ::Eldritch2::UTF8Char* const name, const ::std::function<int (const Argument)>& handler ) {
		using namespace ::std::placeholders;
		using namespace ::boost::iostreams;

	// ---

		_knownOptions.PushBack( { name, [handler] ( const ::Eldritch2::UTF8Char* const begin, const ::Eldritch2::UTF8Char* const end ) -> int {
			basic_array_source<::Eldritch2::UTF8Char>	source( begin, end );
			stream<decltype(source)>					stream( source );
			Argument									optionValue;

			stream >> optionValue;

			return stream.fail() ? -1 : handler( optionValue );
		} } );

		return *this;
	}

// ---------------------------------------------------

	template <typename Argument>
	ToolBase::OptionRegistrationVisitor& ToolBase::OptionRegistrationVisitor::AddTypedArgument( const ::Eldritch2::UTF8Char* const name, const ::Eldritch2::UTF8Char* const shortName, const ::std::function<int (const Argument)>& handler ) {
		AddTypedArgument( name, handler );
		return AddTypedArgument( shortName, handler );
	}

}	// namespace Detail

	template <typename ImplementingType>
	int	ToolCRTPBase<ImplementingType>::Run( const Range<UTF8Char**> options ) {
		{	OptionRegistrationVisitor	visitor( static_cast<ImplementingType&>(*this).GetAllocator() );

			// Register tool options.
			static_cast<ImplementingType&>(*this).RegisterOptions( visitor );

			visitor.DispatchProgramArguments( options );
		}

		return static_cast<ImplementingType&>(*this).Process();
	}

}	// namespace Tools
}	// namespace Eldritch2

