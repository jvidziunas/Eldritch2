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
#include <utfcpp/v2_0/source/utf8/unchecked.h>
#include <EASTL/algorithm.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace {

	template <typename PlatformCharType = PlatformChar>
	struct TranslateDispatcher {
		template <typename InputIterator, typename OutputIterator>
		static ETPureFunctionHint void AsPlatformSequence( InputIterator begin, InputIterator end, OutputIterator output ) {
			eastl::copy( begin, end, output );
		}

		template <typename InputIterator, typename OutputIterator>
		static ETPureFunctionHint void AsUtf8Sequence( InputIterator begin, InputIterator end, OutputIterator output ) {
			eastl::copy( begin, end, output );
		}
	};

	template <>
	struct TranslateDispatcher</*PlatformCharType =*/ wchar_t> {
		template <typename InputIterator, typename OutputIterator>
		static ETPureFunctionHint void AsPlatformSequence( InputIterator begin, InputIterator end, OutputIterator output ) {
			utf8::unchecked::utf8to16( begin, end, output );
		}

		template <typename InputIterator, typename OutputIterator>
		static ETPureFunctionHint void AsUtf8Sequence( InputIterator begin, InputIterator end, OutputIterator output ) {
			utf8::unchecked::utf16to8( begin, end, output );
		}
	};

}	// anonymous namespace

	template <typename InputIterator, typename OutputIterator>
	OutputIterator AsPlatformString( InputIterator begin, InputIterator end, OutputIterator output ) {
		static_assert( eastl::is_same<eastl::iterator_traits<InputIterator>::value_type, Utf8Char>::value, "" );
		static_assert( eastl::is_same<eastl::iterator_traits<OutputIterator>::value_type, PlatformChar>::value, "" );

		TranslateDispatcher<>::AsPlatformSequence( begin, end, output );

		return output;
	}

	template <typename InputIterator, typename OutputIterator>
	OutputIterator AsUtf8String( InputIterator begin, InputIterator end, OutputIterator output ) {
		static_assert( eastl::is_same<eastl::iterator_traits<InputIterator>::value_type, PlatformChar>::value, "" );
		static_assert( eastl::is_same<eastl::iterator_traits<OutputIterator>::value_type, Utf8Char>::value, "" );

		TranslateDispatcher<>::AsUtf8Sequence( begin, end, output );

		return output;
	}

}	// namespace Eldritch2

