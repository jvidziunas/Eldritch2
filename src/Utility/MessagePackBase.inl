/*==================================================================*\
  MessagePackBase.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <rdestl/pair.h>
//------------------------------------------------------------------//
#include <utility>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Utility {

	namespace Detail {

		template <typename Value, typename Archive>
		struct SerializeDispatcher<Value, Archive, true> {
			static bool Dispatch( Value& value, Archive& archive ) {
				static_assert( !Detail::HasFreeSerializeFunction<Value, bool( Value&, Archive& )>::value,
							   "Incorrect serialization behavior detected!\n"
							   "Types to be serialized must have either a member function with signature 'bool Serialize( <Archive type>& )'\n"
							   "or a free (optionally friend) function in the containing namespace with signature 'bool Serialize( <serializing type>&, <archive type>& )'\n"
							   "as per Koenig lookup.\n"
							   "\n"
							   "If you are encountering this error, and you know at least one one such function exists, then there is likely an alternate form \n"
							   "that exists somewhere in the translation unit, causing ambiguity. Templates will contribute to the overload set!\n" );
				return value.Serialize( archive );
			}
		};

	// ---------------------------------------------------

		template <typename Value, typename Archive>
		struct SerializeDispatcher<Value, Archive, false> {
			static bool Dispatch( Value& value, Archive& archive ) {
				return Serialize( value, archive );
			}
		};

	// ---------------------------------------------------

		template <typename FirstType, typename SecondType, typename Archive>
		bool	Serialize( ::rde::pair<FirstType, SecondType>& pair, Archive& archive ) {
			return archive( pair.first, pair.second );
		}

	// ---------------------------------------------------

		template <typename FirstType, typename SecondType, typename Archive>
		bool	Serialize( ::std::pair<FirstType, SecondType>& pair, Archive& archive ) {
			return archive( pair.first, pair.second );
		}

	}	// namespace Detail

// ---------------------------------------------------

	template <class Element>
	typename MessagePackBase::DefaultElementProvider<Element>::ValueType MessagePackBase::DefaultElementProvider<Element>::operator()() const {
		return {};
	}

}	// namespace Utility
}	// namespace Eldritch2