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

	template <typename Option>
	ToolBase::OptionRegistrationVisitor& ToolBase::OptionRegistrationVisitor::AddOption( const ::Eldritch2::SystemChar* const name, const ::Eldritch2::SystemChar shortName, Option& option ) {
		auto	insertResult( _shortSettings.Insert( SettingHandler{ shortName, [] ( const ::Eldritch2::Range<const ::Eldritch2::SystemChar*> value, void* const settingAddress ) -> int {
			using namespace ::boost::iostreams;

		// ---

			array_source<::Eldritch2::SystemChar>	source( value.first, value.onePastLast );
			stream<decltype(source)>				stream( source );

			stream >> *static_cast<Option*>(settingAddress);

			return stream.fail() ? -1 : 0;
		} } ) );

		ETRuntimeAssert( insertResult.second );

		return AddOption( name, option );
	}

// ---------------------------------------------------

	template <typename Option>
	ToolBase::OptionRegistrationVisitor& ToolBase::OptionRegistrationVisitor::AddOption( const ::Eldritch2::SystemChar* const name, Option& option ) {
		auto	insertResult( _shortSettings.Insert( SettingHandler{ shortName, [] ( const ::Eldritch2::Range<const ::Eldritch2::SystemChar*> value, void* const settingAddress ) -> int {
			using namespace ::boost::iostreams;

		// ---

			array_source<::Eldritch2::SystemChar>	source( value.first, value.onePastLast );
			stream<decltype(source)>				stream( source );

			stream >> *static_cast<Option*>(settingAddress);

			return stream.fail() ? -1 : 0;
		} } ) );

		ETRuntimeAssert( insertResult.second );

		return *this;
	}

}	// namespace Detail

	template <typename ImplementingType>
	int	ToolCRTPBase<ImplementingType>::Run( const Range<const SystemChar**> options ) {
		{	// Register tool options.
			OptionRegistrationVisitor	visitor( static_cast<ImplementingType&>(*this).GetAllocator() );

			static_cast<ImplementingType&>(*this).RegisterOptions( visitor );

			visitor.DispatchOptions( options );
		}

		return static_cast<ImplementingType&>(*this).Process();
	}

}	// namespace Tools
}	// namespace Eldritch2

