/*==================================================================*\
  MessagePackWriter.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MessagePackBase.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace FileSystem {
		class	SynchronousFileWriter;
	}	// namespace FileSystem
}	// namespace Eldritch2

namespace Eldritch2 {
namespace Utility {

	namespace Utility	= ::Eldritch2::Utility;

// ---------------------------------------------------

	class MessagePackWriter : public Utility::MessagePackBase {
	// - TYPE PUBLISHING ---------------------------------

	public:
		template <class Container>
		class ArrayWrapper {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//! Constructs this @ref ArrayWrapper instance.
			ArrayWrapper( Container& container );

		// ---------------------------------------------------

			bool	Serialize( Utility::MessagePackWriter& writer );

		// - DATA MEMBERS ------------------------------------

		private:
			Container&	_container;
		};

	// ---

		template <class Container, class KeyExtractor = MessagePackBase::DefaultKeyExtractor<Container>, class ValueExtractor = MessagePackBase::DefaultValueExtractor<Container>>
		class MapWrapper {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//! Constructs this @ref MapWrapper instance.
			/*! @param[in] container Container to be serialized.
				@param[in] keyExtractor Key extractor predicate.
				@param[in] valueExtractor Value extractor predicate.
				*/
			MapWrapper( Container& container, KeyExtractor&& keyExtractor, ValueExtractor&& valueExtractor );

		// ---------------------------------------------------

			bool	Serialize( Utility::MessagePackWriter& writer );

		// - DATA MEMBERS ------------------------------------

		private:
			Container&		_container;
			KeyExtractor	_keyExtractor;
			ValueExtractor	_valueExtractor;
		};

	// ---

		struct TypeString {

		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//! Constructs this @ref MessagePackWriter instance.
		/*! @param[in] writer The @SynchronousFileWriter instance the @ref MessagePackWriter will use to stream data out to secondary storage media.
			*/
		MessagePackWriter( FileSystem::SynchronousFileWriter& writer );

		//! Destroys this @ref MessagePackWriter instance.
		~MessagePackWriter() = default;

	// ---------------------------------------------------

		template <class Container, typename Ignored>
		static MessagePackWriter::ArrayWrapper<Container>&&								WrapArrayContainer( Container& container, Ignored&& );

		template <class Container, typename Ignored, class KeyExtractor, class ValueExtractor>
		static MessagePackWriter::MapWrapper<Container, KeyExtractor, ValueExtractor>&&	WrapMapContainer( Container& container, Ignored&&, KeyExtractor&& keyExtractor, ValueExtractor&& valueExtractor );
		template <class Container, typename Ignored>
		static MessagePackWriter::MapWrapper<Container>&&								WrapMapContainer( Container& container, Ignored&& );

	// ---------------------------------------------------

		template <typename... Fields>
		ETInlineHint bool	operator()( Fields&&... fields );

	// ---------------------------------------------------

		bool	PatchSizeForBinaryData( ::Eldritch2::uint32 sizeInBytes );

		size_t	WriteRaw( const void* const sourceData, const size_t sourceDataSizeInBytes );

	// ---------------------------------------------------

		//! Gets the current write cursor.
		/*! @returns A 64-bit unsigned integer representing the current offset into the file.
			@remarks Not thread-safe.
			*/
		::Eldritch2::uint64	GetWriteCursorInBytes() const;

	// ---------------------------------------------------

	protected:
		template <typename Head, typename... Tail>
		bool	Write( Head&& head, Tail&&... tail );
		template <typename Head>
		bool	Write( Head&& head );

	// - DATA MEMBERS ------------------------------------

	private:
		FileSystem::SynchronousFileWriter&	_writer;
	};

}	// namespace Utility
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/MessagePackWriter.inl>
//------------------------------------------------------------------//