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

namespace Eldritch2 { namespace Tools {
	namespace Detail {

		template <typename Option>
		Tool::OptionRegistrar& Tool::OptionRegistrar::Register(PlatformStringView const name, Option& option) {
			_options.EmplaceBack(name, [&option](StringView<PlatformChar> value) -> int {
				using namespace ::boost::iostreams;

				// ---

				basic_array_source<PlatformChar> source(value.Begin(), value.End());
				stream<decltype(source)>         stream(source);

				stream >> value;

				return stream.fail() ? Error::InvalidParameter : Error::None;
			});

			return *this;
		}

		// ---------------------------------------------------

		template <typename Option>
		Tool::OptionRegistrar& Tool::OptionRegistrar::Register(PlatformStringView name, PlatformStringView shortName, Option& option) {
			return Register(name, option).Register(shortName, option);
		}

	} // namespace Detail

	template <typename ImplementingType>
	int CrtpTool<ImplementingType>::Run(int argc, PlatformChar** argv) {
		{
			OptionRegistrar registrar;
			static_cast<ImplementingType&>(*this).RegisterOptions(registrar);
			registrar.Dispatch(argc, argv);
		}

		return AsPosixInt(static_cast<ImplementingType&>(*this).Process());
	}

}} // namespace Eldritch2::Tools
