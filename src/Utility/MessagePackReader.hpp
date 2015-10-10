/*==================================================================*\
  MessagePackReader.hpp
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
#include <Utility/MPL/Noncopyable.hpp>
#include <Utility/MessagePackBase.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Utility {

	namespace Utility	= ::Eldritch2::Utility;

// ---------------------------------------------------

	class MessagePackReader : public Utility::MessagePackBase {
	// - TYPE PUBLISHING ---------------------------------

	public:
		class DynamicElement : protected ::cmp_object_t {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//! Constructs this DynamicElement instance.
			ETInlineHint DynamicElement( Utility::MessagePackReader& reader );
			//! Constructs this DynamicElement instance.
			DynamicElement( const DynamicElement& ) = delete;

		// ---------------------------------------------------

			//! Attempts to deserialize the MessagePack-encoded byte stream this DynamicElement represents into a real-typed element.
			/*!	@param[in] element Element to parse data into.
				@remarks Specialized for all built-in data types. User defined
				*/
			template <typename ElementToParse>
			ETInlineHint bool	operator()( ElementToParse& element ) const;

			template <typename Head, typename... Tail>
			bool	ParseOneOf( size_t& indexOfUsedElement, Head&& head, Tail&&... tail ) const;
			template <typename Head>
			bool	ParseOneOf( size_t& indexOfUsedElement, Head&& head ) const;

		// - DATA MEMBERS ------------------------------------

		private:
			MessagePackReader&	_reader;
		};

	// ---

		struct InPlaceString : public ::Eldritch2::Range<const char*> {
			//! Constructs this InPlaceString instance.
			ETInlineHint InPlaceString();
		};

	// ---

		struct InPlaceBulkData : public ::Eldritch2::Range<const char*> {
			//! Constructs this InPlaceBulkData instance.
			ETInlineHint InPlaceBulkData();
		};

	// ---

		struct TypeString : public InPlaceString {
			using InPlaceString::InPlaceString;
		};

	// ---

		template <class Container, class DefaultElementProvider>
		class ArrayWrapper {
		// - TYPE PUBLISHING ---------------------------------

		public:
			typedef Container				ContainerType;
			typedef DefaultElementProvider	DefaultElementProviderType;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			//! Constructs this @ref ArrayWrapper instance.
			ArrayWrapper( Container& container, DefaultElementProvider&& defaultElementProvider );

		// ---------------------------------------------------

			bool	Serialize( Utility::MessagePackReader& reader );

		// - DATA MEMBERS ------------------------------------

		private:
			Container&				_container;
			DefaultElementProvider	_defaultElementProvider;
		};

	// ---

		template <class Container, class DefaultElementProvider, class KeyExtractor = MessagePackBase::DefaultKeyExtractor<Container>, class ValueExtractor = MessagePackBase::DefaultValueExtractor<Container>>
		class MapWrapper {
		// - TYPE PUBLISHING ---------------------------------

		public:
			typedef Container				ContainerType;
			typedef DefaultElementProvider	DefaultElementProviderType;
			typedef KeyExtractor			KeyExtractorType;
			typedef ValueExtractor			ValueExtractorType;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			//! Constructs this @ref MapWrapper instance.
			MapWrapper( Container& container, DefaultElementProvider&& defaultElementProvider, KeyExtractor&& keyExtractor, ValueExtractor&& valueExtractor );

		// ---------------------------------------------------

			bool	Serialize( Utility::MessagePackReader& reader );

		// - DATA MEMBERS ------------------------------------

		private:
			Container&				_container;
			DefaultElementProvider	_defaultElementProvider;
			KeyExtractor			_keyExtractor;
			ValueExtractor			_valueExtractor;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//! Constructs this @ref MessagePackReader instance.
		MessagePackReader( const ::Eldritch2::Range<const char*> encodedData );

		//! Destroys this @ref MessagePackReader instance.
		~MessagePackReader() = default;

	// ---------------------------------------------------

		template <class Container, class DefaultElementProvider>
		static MessagePackReader::ArrayWrapper<Container, DefaultElementProvider>&&								WrapArrayContainer( Container& container, DefaultElementProvider&& elementProvider );

		template <class Container, class DefaultElementProvider, class KeyExtractor, class ValueExtractor>
		static MessagePackReader::MapWrapper<Container, DefaultElementProvider, KeyExtractor, ValueExtractor>&&	WrapMapContainer( Container& container, DefaultElementProvider&& elementProvider, KeyExtractor&& keyExtractor, ValueExtractor&& valueExtractor );
		template <class Container, class DefaultElementProvider>
		static MessagePackReader::MapWrapper<Container, DefaultElementProvider>&&								WrapMapContainer( Container& container, DefaultElementProvider&& elementProvider );

	// ---------------------------------------------------

		//! <BriefDescription>
		/*!	@param[out] indexOfUsedElement Zero-based index of the actual parsed element. The function will not modify this value if no elements are parsed.
			@param[in] fields Parameter pack containing elements to attempt deserialization on. The function will attempt to deserialize each element, in order, until a match is found.
			@returns A bool indicating whether _any_ element was successfully parsed.
			*/
		template <typename... Fields>
		ETInlineHint bool	ParseOneOf( size_t&& indexOfUsedElement, Fields&&... fields );

		template <typename... Fields>
		ETInlineHint bool	operator()( Fields&&... fields );

	// ---------------------------------------------------

		bool	ReadRaw( void* const destination, const size_t readSizeInBytes );

		bool	SkipBytes( const size_t skipSpanInBytes );

	// ---------------------------------------------------

	protected:
		bool	GetInPlaceDataSpan( const size_t spanSizeInBytes, ::Eldritch2::Range<const char*>& range );

	// ---------------------------------------------------

		template <typename Head, typename... Tail>
		bool	Parse( Head&& head, Tail&&... tail );
		template <typename Head>
		bool	Parse( Head&& head );

	// - DATA MEMBERS ------------------------------------

	private:
		const char* const	_encodedDataEnd;
	};

}	// namespace Utility
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/MessagePackReader.inl>
//------------------------------------------------------------------//