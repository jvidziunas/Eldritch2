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
#include <Common/ErrorCode.hpp>
//------------------------------------------------------------------//
#include <eastl/initializer_list.h>
#include <eastl/type_traits.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Detail {

		template <typename T, bool isTriviallyDestructible = eastl::is_trivially_destructible<T>::value>
		struct ResultStorage {
		// - TYPE PUBLISHING ---------------------------------

		public:
			using ValueType = typename eastl::remove_const<T>::type;
			using StorageType = typename eastl::aligned_storage<sizeof(ValueType), _alignof(ValueType)>::type;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref ResultStorage instance.
			template <typename U, typename... Args>
			ResultStorage(std::initializer_list<U> initializerList, Args&&... args);
		//!	Constructs this @ref ResultStorage instance.
			ResultStorage(ErrorCode errorCode) ETNoexceptHint;
		//!	Disable copy construction.
			ResultStorage(const ResultStorage&) = delete;
		//!	Constructs this @ref ResultStorage instance.
			ResultStorage(ResultStorage&&);
		//!	Constructs this @ref ResultStorage instance.
			template <typename... Args>
			ResultStorage(Args&&... args);

			~ResultStorage();

		// ---------------------------------------------------

		public:
			void	DestructValue();

		// ---------------------------------------------------

			ResultStorage&	operator=(const ResultStorage&) = delete;

		// ---------------------------------------------------

			ErrorCode	errorCode;
			StorageType	storage;
		};

	// ---------------------------------------------------

		template <typename T>
		struct ResultStorage<T, true> {
		// - TYPE PUBLISHING ---------------------------------

		public:
			using ValueType = typename eastl::remove_const<T>::type;
			using StorageType = typename eastl::aligned_storage<sizeof(ValueType), _alignof(ValueType)>::type;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref ResultStorage instance.
			template <typename U, typename... Args>
			ResultStorage(std::initializer_list<U> initializerList, Args&&... args);
		//!	Disable copy construction.
			ResultStorage(const ResultStorage&) = delete;
		//!	Constructs this @ref ResultStorage instance.
			ResultStorage(ErrorCode errorCode) ETNoexceptHint;
		//!	Constructs this @ref ResultStorage instance.
			ResultStorage(ResultStorage&&);
		//!	Constructs this @ref ResultStorage instance.
			template <typename... Args>
			ResultStorage(Args&&... args);

			~ResultStorage() = default;

		// ---------------------------------------------------

		public:
			void	DestructValue();

		// ---------------------------------------------------

		//!	Disable assignment.
			ResultStorage&	operator=(const ResultStorage&) = delete;

		// ---------------------------------------------------

		public:
			ErrorCode	errorCode;
			StorageType	storage;
		};

	}	// namespace Detail

	template <typename CreatedObject>
	class Result : private Eldritch2::Detail::ResultStorage<CreatedObject> {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using ValueType = CreatedObject;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Result instance.
		template <typename U, typename... Args, typename = typename eastl::enable_if<eastl::is_constructible<CreatedObject, std::initializer_list<U>&, Args&&...>::value>::type>
		Result(std::initializer_list<U> initializerList, Args&&... args);
	//!	Constructs this @ref Result instance.
		Result(ErrorCode errorCode);
	//!	Constructs this @ref Result instance.
		template <typename... Args, typename = typename eastl::enable_if<eastl::is_constructible<CreatedObject, Args&&...>::value>::type>
		Result(Args&&... args);
	//!	Disable copy construction.
		Result(const Result&) = delete;
	//!	Constructs this @ref Result instance.
		Result(Result&&);

		~Result() = default;

	// ---------------------------------------------------

	public:
		ErrorCode	GetErrorCode() const;

	// ---------------------------------------------------

	public:
		const CreatedObject*	operator->() const;
		CreatedObject*			operator->();

		const CreatedObject&	operator*() const;
		CreatedObject&			operator*();

		operator ErrorCode() const;

	// ---------------------------------------------------

	//!	Disable assignment.
		Result&	operator=(const Result&) = delete;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Result.inl>
//------------------------------------------------------------------//

