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
#include <Utility/MessagePackBase.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Utility {

	class MessagePackReader : public Utility::MessagePackBase {
	// - TYPE PUBLISHING ---------------------------------

	public:
		template <typename Container, typename ElementProvider>
		class ArrayAdapter {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref ArrayAdapter instance.
			/*!	@param[in] container Container to insert deserialized elements into.
				@param[in] elementProvider Default element provider the @ref ArrayAdapter will use to construct elements before deserializing fields.
				*/
			ArrayAdapter( Container& container, ElementProvider elementProvider );

			//!	Destroys this @ref ArrayAdapter instance.
			~ArrayAdapter() = default;

		// ---------------------------------------------------

			bool	Serialize( Utility::MessagePackReader& reader );

		// - DATA MEMBERS ------------------------------------

		private:
			Container&		_container;
			ElementProvider	_elementProvider;
		};

	// ---

		template <typename Container, typename ElementProvider>
		class MapAdapter {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref MapAdapter instance.
			/*!	@param[in] container Container to insert deserialized elements into.
				@param[in] elementProvider Default element provider the @ref MapAdapter will use to construct elements before deserializing fields.
				*/
			MapAdapter( Container& container, ElementProvider&& elementProvider );

			//!	Destroys this @ref MapAdapter instance.
			~MapAdapter() = default;

		// ---------------------------------------------------

			bool	Serialize( Utility::MessagePackReader& reader );

		// - DATA MEMBERS ------------------------------------

		private:
			Container&		_container;
			ElementProvider	_elementProvider;
		};

	// ---

		using Checkpoint = const void*;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//! Constructs this @ref MessagePackReader instance.
		MessagePackReader( const ::Eldritch2::Range<const char*> encodedData );

		//! Destroys this @ref MessagePackReader instance.
		~MessagePackReader() = default;

	// ---------------------------------------------------

		template <typename Container, typename ElementProvider = DefaultElementProvider<typename Container::ValueType>>
		static MapAdapter<Container, ElementProvider>	AdaptMap( Container& container, ElementProvider&& elementProvider = ElementProvider() );

		template <typename Container, typename ElementProvider = DefaultElementProvider<typename Container::ValueType>>
		static ArrayAdapter<Container, ElementProvider>	AdaptArray( Container& container, ElementProvider&& elementProvider = ElementProvider() );

	// ---------------------------------------------------

		//! Parses at most one of the passed-in fields.
		/*!	@param[out] indexOfUsedElement Zero-based index of the actual parsed element. The function will not modify this value if no elements are parsed.
			@param[in] fields Parameter pack containing elements to attempt deserialization on. The function will attempt to deserialize each element, in order, until a match is found.
			@returns _True_ if _any_ element in the set was parsed correctly, _false_ if _no_ elements were parsed correctly.
			@remarks @parblock The function creates read checkpoints and will roll back any reads in the event a type isn't parsed fully. This means that, depending on the Serialize function
				of the fields used, the stream will have consumed more logical MessagePack records then the number of fields sent to the function.
			@see @ref CreateCheckpoint(), @ref RestoreCheckpoint()
			*/
		template <typename... Fields>
		ETInlineHint bool	ParseOneOf( size_t&& indexOfUsedElement, Fields&&... fields );

		//!	Parses all the fields passed in as parameters.
		/*!	@param[in] fields Parameter pack containing the fields to be parsed.
			@returns _True_ if all fields were parsed correctly, _false_ if at least one field did not parse fully.
			@remarks Unlike @ref ParseOneOf(), this function will _not_ roll back unsuccessful read attempts in the event they fail.
			*/
		template <typename... Fields>
		ETInlineHint bool	operator()( Fields&&... fields );

	// ---------------------------------------------------

		bool	ReadRaw( void* const destination, const size_t readSizeInBytes );

		bool	SkipBytes( const size_t skipSpanInBytes );

	// ---------------------------------------------------

	protected:
		template <typename Head, typename... Fields>
		ETInlineHint bool	TryParseOneOf( size_t&& indexOfUsedElement, Head&& head, Fields&&... fields );
		template <typename Head>
		ETInlineHint bool	TryParseOneOf( size_t&& indexOfUsedElement, Head&& head );

	// ---------------------------------------------------

		bool	GetInPlaceDataSpan( const size_t spanSizeInBytes, ::Eldritch2::Range<const char*>& range );

	// ---------------------------------------------------

		Checkpoint	CreateCheckpoint() const;

		void		RestoreCheckpoint( const Checkpoint checkpoint );

	// ---------------------------------------------------

		template <typename Head, typename... Tail>
		bool	Parse( Head& head, Tail&... tail );
		template <typename Head>
		bool	Parse( Head& head );

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