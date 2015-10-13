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
#include <Utility/Containers/Range.hpp>
#include <Utility/MPL/CharTypes.hpp>
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

		// ---

			CUSTOM_TYPE_BEGIN	= 64
		};

	// ---

		template <class Container>
		struct DefaultElementProvider {
		// - TYPE PUBLISHING ---------------------------------

			using ValueType		= typename Container::ValueType;

		// ---------------------------------------------------

			ValueType	operator()() const;
		};

	// ---

		template <class Container>
		struct DefaultKeyExtractor {
		// - TYPE PUBLISHING ---------------------------------

			using ExtractedType	= typename Container::KeyType;
			using ValueType		= typename Container::ValueType;

		// ---------------------------------------------------

			ExtractedType&			operator()( ValueType& value ) const;
			const ExtractedType&	operator()( const ValueType& value ) const;
		};

	// ---

		template <class Container>
		struct DefaultValueExtractor {
		// - TYPE PUBLISHING ---------------------------------

			using ExtractedType = typename Container::MappedType;
			using ValueType		= typename Container::ValueType;

		// ---------------------------------------------------

			ExtractedType&			operator()( ValueType& value ) const;
			const ExtractedType&	operator()( const ValueType& value ) const;
		};

	// ---

		struct Nil {
			bool	Serialize( Utility::MessagePackReader& reader );
			bool	Serialize( Utility::MessagePackWriter& writer );
		};

	// ---

		struct String : public ::Eldritch2::Range<const ::Eldritch2::UTF8Char*> {
			String( const ::Eldritch2::UTF8Char* begin, const ::Eldritch2::UTF8Char* const end );
			String();

		// ---------------------------------------------------
			
			bool	Serialize( Utility::MessagePackReader& reader );
			bool	Serialize( Utility::MessagePackWriter& writer );
		};

	// ---

		struct BinaryData : public ::Eldritch2::Range<const char*> {
			BinaryData( const void* begin, const void* const end );
			BinaryData();

		// ---------------------------------------------------
			
			bool	Serialize( Utility::MessagePackReader& reader );
			bool	Serialize( Utility::MessagePackWriter& writer );
		};

	// ---

		class ArrayHeader {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref ArrayHeader instance.
			/*!	@param[in] elementCount Number of elements in the array.
				@remarks This overload is designed to be called in a write context.
				*/
			ArrayHeader( const ::Eldritch2::uint32 elementCount );

			//!	Constructs this @ref ArrayHeader instance.
			ArrayHeader() = default;

		// ---------------------------------------------------

			bool	Serialize( Utility::MessagePackReader& reader );
			bool	Serialize( Utility::MessagePackWriter& writer );

		// - DATA MEMBERS ------------------------------------

		protected:
			::Eldritch2::uint32 sizeInElements;
		};

	// ---

		class MapHeader {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref MapHeader instance.
			/*!	@param[in] pairCount Number of pairs in the map.
				@remarks This overload is designed to be called in a write context.
				*/
			MapHeader( const ::Eldritch2::uint32 pairCount );
			//!	Constructs this @ref MapHeader instance.
			MapHeader() = default;

		// ---------------------------------------------------

			bool	Serialize( Utility::MessagePackReader& reader );
			bool	Serialize( Utility::MessagePackWriter& writer );

		// - DATA MEMBERS ------------------------------------

		protected:
			::Eldritch2::uint32 sizeInPairs;
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

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/MessagePackBase.inl>
//------------------------------------------------------------------//

