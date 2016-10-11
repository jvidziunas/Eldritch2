/*==================================================================*\
  CharTypes.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <utfcpp/v2_0/source/utf8.h>
#include <EASTL/algorithm.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace {

	template <typename SystemCharType = Eldritch2::SystemChar>
	struct TranslateDispatcher {
		template <typename InputIterator, typename OutputIterator>
		static ETPureFunctionHint void SystemSequenceFromUtf8Sequence( InputIterator begin, InputIterator end, OutputIterator output ) {
			eastl::copy( begin, end, output );
		}

		template <typename InputIterator, typename OutputIterator>
		static ETPureFunctionHint void Utf8SequenceFromSystemSequence( InputIterator begin, InputIterator end, OutputIterator output ) {
			eastl::copy( begin, end, output );
		}
	};

	template <>
	struct TranslateDispatcher</*SystemCharType =*/ wchar_t> {
		template <typename InputIterator, typename OutputIterator>
		static ETPureFunctionHint void SystemSequenceFromUtf8Sequence( InputIterator begin, InputIterator end, OutputIterator output ) {
			utf8::unchecked::utf8to16( begin, end, output );
		}

		template <typename InputIterator, typename OutputIterator>
		static ETPureFunctionHint void Utf8SequenceFromSystemSequence( InputIterator begin, InputIterator end, OutputIterator output ) {
			utf8::unchecked::utf16to8( begin, end, output );
		}
	};

}	// anonymous namespace

	template <typename InputIterator, typename OutputIterator>
	OutputIterator SystemCharFromUtf8Char( InputIterator begin, InputIterator end, OutputIterator output ) {
		static_assert( eastl::is_same<eastl::iterator_traits<InputIterator>::value_type, Eldritch2::Utf8Char>::value, "" );
		static_assert( eastl::is_same<eastl::iterator_traits<OutputIterator>::value_type, Eldritch2::SystemChar>::value, "" );

		TranslateDispatcher<>::SystemSequenceFromUtf8Sequence( begin, end, output );

		return output;
	}

	template <typename InputIterator, typename OutputIterator>
	OutputIterator Utf8CharFromSystemChar( InputIterator begin, InputIterator end, OutputIterator output ) {
		static_assert( eastl::is_same<eastl::iterator_traits<InputIterator>::value_type, Eldritch2::SystemChar>::value, "" );
		static_assert( eastl::is_same<eastl::iterator_traits<OutputIterator>::value_type, Eldritch2::Utf8Char>::value, "" );

		TranslateDispatcher<>::Utf8SequenceFromSystemSequence( begin, end, output );

		return output;
	}

}	// namespace Eldritch2

