/*==================================================================*\
  MessagePackBase.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/IntTypes.hpp>
//------------------------------------------------------------------//
#include <cmp/cmp.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Utility {
		class	MessagePackReader;
		class	MessagePackWriter;
	}
}

namespace Eldritch2 {
namespace Utility {

	namespace Utility	= ::Eldritch2::Utility;

// ---------------------------------------------------

	class ETPureAbstractHint MessagePackBase : protected ::cmp_ctx_t {
	// - TYPE PUBLISHING ---------------------------------

	public:
		enum KnownExtendedDataMarkers : ::Eldritch2::int8 {
			OBJECT_REFERENCE	= 0,
			TYPE_INDEX,
			TYPE_STRING,
			FLOAT4,
			INT4,

			CUSTOM_TYPE_BEGIN	= 64
		};

	// ---

		template <class Container>
		struct DefaultKeyExtractor {
			auto operator()( typename Container::ValueType& value ) -> decltype((value.first)) {
				return value.first;
			}

			auto operator()( typename const Container::ValueType& value ) -> decltype((value.first)) {
				return value.first;
			}
		};

	// ---

		template <class Container>
		struct DefaultValueExtractor {
			auto operator()( typename Container::ValueType& value ) -> decltype((value.second)) {
				return value.second;
			}

			auto operator()( typename const Container::ValueType& value ) -> decltype((value.second)) {
				return value.second;
			}
		};

	// ---

		struct Nil {
			bool	Serialize( Utility::MessagePackReader& reader );
			bool	Serialize( Utility::MessagePackWriter& writer );
		};

	// ---

		struct ArrayHeader {
			::Eldritch2::uint32 sizeInElements;

			bool	Serialize( Utility::MessagePackReader& reader );
			bool	Serialize( Utility::MessagePackWriter& writer );
		};

	// ---

		struct ObjectReference {
			::Eldritch2::uint32	identifier;

			bool	Serialize( Utility::MessagePackReader& reader );
			bool	Serialize( Utility::MessagePackWriter& writer );
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		//! Constructs this @ref MessagePackBase instance.
		MessagePackBase( const ::cmp_reader readFunction, const void* const buffer = nullptr );
		//! Constructs this @ref MessagePackBase instance.
		MessagePackBase( const ::cmp_writer writeFunction, void* const buffer = nullptr );
	};

}	// namespace Utility
}	// namespace Eldritch2
