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
#include <type_traits>
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

	namespace Detail {

		namespace Detail = ::Eldritch2::Utility::Detail;

	// ---------------------------------------------------

		template <class Class, typename FunctionSignature>
		struct HasMemberSerializeFunction {
			static_assert( ::std::integral_constant<FunctionSignature, false>::value, "Second template parameter needs to be of function type." );
		};

	// ---------------------------------------------------

		template <class Class, typename DesiredReturn, typename... Arguments>
		struct HasMemberSerializeFunction<Class, DesiredReturn( Arguments... )> {
		private:
			template <typename T>
			static typename ::std::is_same<DesiredReturn, decltype(::std::declval<T>().Serialize( ::std::declval<Arguments>()... ))>::type Check( T* );

			template <typename>
			static ::std::false_type Check( ... );

		// ---

			using Type	= decltype(Check<Class>(nullptr));

		public:
			static const bool	value = Type::value;
		};

	// ---------------------------------------------------

		template <class Class, typename FunctionSignature>
		struct HasFreeSerializeFunction {
			static_assert( ::std::integral_constant<FunctionSignature, false>::value, "Second template parameter needs to be of function type." );
		};

	// ---------------------------------------------------

		template <class Class, typename DesiredReturn, typename... Arguments>
		struct HasFreeSerializeFunction<Class, DesiredReturn( Arguments... )> {
		private:
			template <typename T>
			static typename ::std::is_same<DesiredReturn, decltype(Serialize( ::std::declval<T>(), ::std::declval<Arguments>()... ))>::type Check( T* );

			template <typename>
			static ::std::false_type Check( ... );

		// ---

			using Type = decltype(Check<Class>( nullptr ));

		public:
			static const bool	value = Type::value;
		};

	// ---------------------------------------------------

		template <typename ContainerValue, typename Archive, bool typeHasMemberSerializeFunction = Detail::HasMemberSerializeFunction<ContainerValue, bool(Archive&)>::value>
		struct SerializeDispatcher {
			static bool Dispatch( ContainerValue& value, Archive& archive );
		};

	}	// namespace Detail

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

		template <class Element>
		class DefaultElementProvider {
		// - TYPE PUBLISHING ---------------------------------

		public:
			using ValueType	= Element;

		// ---------------------------------------------------

			ValueType	operator()() const;
		};

	// ---
		
		class Nil {
		public:
			bool	Serialize( Utility::MessagePackReader& reader );
			bool	Serialize( Utility::MessagePackWriter& writer );
		};

	// ---

		class String : public ::Eldritch2::Range<const ::Eldritch2::UTF8Char*> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			String( const ::Eldritch2::UTF8Char* begin, const ::Eldritch2::UTF8Char* const end );
			//!	Constructs this @ref String instance.
			String();

			//!	Destroys this @ref String instance.
			~String() = default;

		// ---------------------------------------------------
			
			bool	Serialize( Utility::MessagePackReader& reader );
			bool	Serialize( Utility::MessagePackWriter& writer );
		};

	// ---

		class BinaryData : public ::Eldritch2::Range<const char*> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref BinaryData instance.
			/*!	@param[in] begin Pointer to the first byte of data that will comprise the body of the new @ref BinaryData instance.
				@param[in] end Pointer to one past the last byte of data that will comprise the body of the new @ref BinaryData instance.
				@remarks This overload is designed to be called in a write context.
				*/
			BinaryData( const void* begin, const void* const end );
			//!	Constructs this @ref BinaryData instance.
			BinaryData();

			//!	Destroys this @ref BinaryData instance.
			~BinaryData() = default;

		// ---------------------------------------------------
			
			bool	Serialize( Utility::MessagePackReader& reader );
			bool	Serialize( Utility::MessagePackWriter& writer );
		};

	// ---

		class Array {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref Array instance.
			/*!	@param[in] elementCount Number of elements in the array.
				@remarks This overload is designed to be called in a write context.
				*/
			Array( const ::Eldritch2::uint32 elementCount );
			//!	Constructs this @ref Array instance.
			Array() = default;

		// ---------------------------------------------------

			bool	Serialize( Utility::MessagePackReader& reader );
			bool	Serialize( Utility::MessagePackWriter& writer );

		// - DATA MEMBERS ------------------------------------

			::Eldritch2::uint32 sizeInElements;
		};

	// ---

		class Map {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref Map instance.
			/*!	@param[in] pairCount Number of pairs in the map.
				@remarks This overload is designed to be called in a write context.
				*/
			Map( const ::Eldritch2::uint32 pairCount );
			//!	Constructs this @ref Map instance.
			Map() = default;

		// ---------------------------------------------------

			bool	Serialize( Utility::MessagePackReader& reader );
			bool	Serialize( Utility::MessagePackWriter& writer );

		// - DATA MEMBERS ------------------------------------

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

