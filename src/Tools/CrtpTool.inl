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
		Tool::OptionRegistrar& Tool::OptionRegistrar::Register(const Utf8Char* const name, Option& option) {
			_options.EmplaceBack(name, [&option](Range<const Utf8Char*> value) -> int {
				using namespace ::boost::iostreams;

				// ---

				basic_array_source<Utf8Char> source(value.Begin(), value.End());
				stream<decltype(source)>     stream(source);
				Argument                     optionValue;

				stream >> optionValue;

				if (stream.fail()) {
					return AsPosixInt(Error::InvalidParameter);
				}

				option = optionValue;

				return 0;
			});

			return *this;
		}

		// ---------------------------------------------------

		template <typename Option>
		Tool::OptionRegistrar& Tool::OptionRegistrar::Register(const Utf8Char* const name, const Utf8Char* const shortName, Option& option) {
			return Register(name, option).Register(shortName, option);
		}

		// ---------------------------------------------------

		template <typename ArgumentIterator>
		int Tool::OptionRegistrar::Dispatch(ArgumentIterator begin, ArgumentIterator end) {
			Utf8Char* const arguments(ETStackAlloc(Utf8Char*, eastl::distance(begin, end)));
			return Dispatch({ arguments, eastl::uninitialized_copy(begin, end, arguments) });
		}

	} // namespace Detail

	template <typename ImplementingType>
	template <typename ArgumentIterator>
	int CrtpTool<ImplementingType>::Run(ArgumentIterator begin, ArgumentIterator end) {
		{
			OptionRegistrar registrar;
			static_cast<ImplementingType&>(*this).RegisterOptions(registrar);
			registrar.Dispatch(begin, end);
		}

		return AsPosixInt(static_cast<ImplementingType&>(*this).Process());
	}

}} // namespace Eldritch2::Tools
