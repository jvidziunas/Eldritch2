/*==================================================================*\
  Result.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//
#if ET_COMPILER_IS_MSVC && !defined( EA_COMPILER_HAS_C99_FORMAT_MACROS )
//	MSVC complains about *lots* of macro redefinitions in eabase/inttypes.h.
#	define EA_COMPILER_HAS_C99_FORMAT_MACROS
#endif
#include <eastl/type_traits.h>
#include <EASTL/initializer_list.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Detail {

	template <typename T, bool isTriviallyDestructible = eastl::is_trivially_destructible<T>::value>
	struct ResultStorage {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using ValueType		= typename eastl::remove_const<T>::type;
		using StorageType	= typename eastl::aligned_storage<sizeof(ValueType), _alignof(ValueType)>::type;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref ResultStorage instance.
		template <typename U, typename... Args>
		ResultStorage( std::initializer_list<U> initializerList, Args&&... args );
	//!	Constructs this @ref ResultStorage instance.
		template <typename... Args>
		ResultStorage( Args&&... args );
	//!	Constructs this @ref ResultStorage instance.
		ResultStorage( Eldritch2::ErrorCode errorCode ) EA_NOEXCEPT;
	//!	Disable copying.
		ResultStorage( const ResultStorage& ) = delete;
	//!	Constructs this @ref ResultStorage instance.
		ResultStorage( ResultStorage&& );

		~ResultStorage();

	// ---------------------------------------------------

	public:
		void	DestructValue();

	// ---------------------------------------------------

		ResultStorage&	operator=( const ResultStorage& ) = delete;

	// ---------------------------------------------------

		Eldritch2::ErrorCode	errorCode;
		StorageType				storage;
	};

// ---------------------------------------------------

	template <typename T>
	struct ResultStorage<T, true> {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using ValueType		= typename eastl::remove_const<T>::type;
		using StorageType	= typename eastl::aligned_storage<sizeof(ValueType), _alignof(ValueType)>::type;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref ResultStorage instance.
		template <typename U, typename... Args>
		ResultStorage( std::initializer_list<U> initializerList, Args&&... args );
	//!	Constructs this @ref ResultStorage instance.
		template <typename... Args>
		ResultStorage( Args&&... args );
	//!	Constructs this @ref ResultStorage instance.
		ResultStorage( Eldritch2::ErrorCode errorCode ) EA_NOEXCEPT;
	//!	Disable copying.
		ResultStorage( const ResultStorage& ) = delete;
	//!	Constructs this @ref ResultStorage instance.
		ResultStorage( ResultStorage&& );

		~ResultStorage() = default;

	// ---------------------------------------------------

	public:
		void	DestructValue();

	// ---------------------------------------------------

	//!	Disable assignment.
		ResultStorage&	operator=( const ResultStorage& ) = delete;

	// ---------------------------------------------------

	public:
		Eldritch2::ErrorCode	errorCode;
		StorageType				storage;
	};

}	// namespace Detail

	template <typename CreatedObject>
	class Result : private Detail::ResultStorage<CreatedObject> {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using ValueType	= CreatedObject;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Result instance.
		template <typename U, typename... Args, typename = typename eastl::enable_if<eastl::is_constructible<CreatedObject, std::initializer_list<U>&, Args&&...>::value>::type>
		Result( std::initializer_list<U> initializerList, Args&&... args );
	//!	Constructs this @ref Result instance.
		template <typename... Args, typename = typename eastl::enable_if<eastl::is_constructible<CreatedObject, Args&&...>::value>::type>
		Result( Args&&... args );
	//!	Constructs this @ref Result instance.
		Result( Eldritch2::ErrorCode errorCode );
	//!	Disable copying.
		Result( const Result& ) = delete;
	//!	Constructs this @ref Result instance.
		Result( Result&& );

		~Result() = default;

	// ---------------------------------------------------

	public:
		Eldritch2::ErrorCode	GetErrorCode() const;

	// ---------------------------------------------------

	public:
		const CreatedObject*	operator->() const;
		CreatedObject*			operator->();

		const CreatedObject&	operator*() const;
		CreatedObject&			operator*();

		operator				Eldritch2::ErrorCode() const;

		explicit operator		bool() const;

	// ---------------------------------------------------

	//!	Disable assignment.
		Result&	operator=( const Result& ) = delete;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Result.inl>
//------------------------------------------------------------------//

