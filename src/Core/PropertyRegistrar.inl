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
#include <eastl/type_traits.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Core {
	namespace Detail {

		template <bool isFloatingPoint = true, bool isSigned = true>
		struct NumericParser {
			template <typename Value>
			static ETInlineHint ETPureFunctionHint void Parse(StringView<Utf8Char> string, Value& value) ETNoexceptHint {
				auto       end(const_cast<Utf8Char*>(string.End()));
				const auto temp(std::strtold(string.Begin(), &end));
				if (string.Begin() != end) {
					value = Value(temp);
				}
			}
		};

		// ---

		template <>
		struct NumericParser</*bool isFloatingPoint = */ false, /*isSigned = */ true> {
			template <typename Value>
			static ETInlineHint ETPureFunctionHint void Parse(StringView<Utf8Char> string, Value& value) ETNoexceptHint {
				auto       end(const_cast<Utf8Char*>(string.End()));
				const auto temp(std::strtoll(string.Begin(), &end, 0));
				if (string.Begin() != end) {
					value = Value(temp);
				}
			}
		};

		// ---

		template <>
		struct NumericParser</*bool isFloatingPoint = */ false, /*isSigned = */ false> {
			template <typename Value>
			static ETInlineHint ETPureFunctionHint void Parse(StringView<Utf8Char> string, Value& value) ETNoexceptHint {
				auto       end(const_cast<Utf8Char*>(string.End()));
				const auto temp(std::strtoull(string.Begin(), &end, 0));
				if (string.Begin() != end) {
					value = Value(temp);
				}
			}
		};

	} // namespace Detail

	template <typename PodValue>
	PropertyRegistrar& PropertyRegistrar::DefineProperty(StringView<Utf8Char> name, PodValue& property) {
		using ParserType = Detail::NumericParser<eastl::is_floating_point<PodValue>::value, eastl::is_signed<PodValue>::value>;

		return DefineProperty(name, [&property](StringView<Utf8Char> value) { ParserType::Parse(value, property); });
	}

}} // namespace Eldritch2::Core
