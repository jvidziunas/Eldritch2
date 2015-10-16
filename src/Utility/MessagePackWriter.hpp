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
		template <typename Container>
		class ArrayAdapter : public MessagePackBase::Array {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref ArrayAdapter instance.
			/*!	@param[in] container Container to read serializable elements from.
				*/
			ArrayAdapter( Container& container );

			//!	Destroys this @ref ArrayAdapter instance.
			~ArrayAdapter() = default;

		// ---------------------------------------------------

			bool	Serialize( Utility::MessagePackWriter& writer );

		// - DATA MEMBERS ------------------------------------

		private:
			Container&	_container;
		};

	// ---

		template <typename Container>
		class MapAdapter : public MessagePackBase::Map {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref MapAdapter instance.
			/*!	@param[in] container Container to read serializable elements from.
				*/
			MapAdapter( Container& container );

			//!	Destroys this @ref MapAdapter instance.
			~MapAdapter() = default;

		// ---------------------------------------------------

			bool	Serialize( Utility::MessagePackWriter& writer );

		// - DATA MEMBERS ------------------------------------

		private:
			Container&	_container;
		};

	// ---

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//! Constructs this @ref MessagePackWriter instance.
		/*! @param[in] writer The @SynchronousFileWriter instance the @ref MessagePackWriter will use to stream data out to secondary storage media.
			*/
		MessagePackWriter( FileSystem::SynchronousFileWriter& writer );

		//! Destroys this @ref MessagePackWriter instance.
		~MessagePackWriter() = default;

	// ---------------------------------------------------

		template <typename Container, typename Ignored>
		static MapAdapter<Container>	AdaptMap( Container& container, Ignored&& );

		template <typename Container, typename Ignored>
		static ArrayAdapter<Container>	AdaptArray( Container& container, Ignored&& );

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