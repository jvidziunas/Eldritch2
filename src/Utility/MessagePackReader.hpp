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
			//! Constructs this @ref DynamicElement instance.
			ETInlineHint DynamicElement( Utility::MessagePackReader& reader );
			//! Constructs this @ref DynamicElement instance.
			DynamicElement( const DynamicElement& ) = delete;

			//!	Destroys this @ref DynamicElement instance.
			~DynamicElement() = default;

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

		using Checkpoint = const void*;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//! Constructs this @ref MessagePackReader instance.
		MessagePackReader( const ::Eldritch2::Range<const char*> encodedData );

		//! Destroys this @ref MessagePackReader instance.
		~MessagePackReader() = default;

	// ---------------------------------------------------

		//! Parses at most one of the passed-in elements.
		/*!	@param[out] indexOfUsedElement Zero-based index of the actual parsed element. The function will not modify this value if no elements are parsed.
			@param[in] fields Parameter pack containing elements to attempt deserialization on. The function will attempt to deserialize each element, in order, until a match is found.
			@returns A bool indicating whether _any_ element was successfully parsed.
			@remarks The function creates read checkpoints and will roll back any reads in the event a type isn't parsed fully. If none of the elements
			@see @ref CreateCheckpoint(), @ref RestoreCheckpoint()
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

		Checkpoint	CreateCheckpoint() const;

		void		RestoreCheckpoint( const Checkpoint checkpoint );

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