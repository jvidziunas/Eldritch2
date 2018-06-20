/*==================================================================*\
  Result.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Assert.hpp>
//------------------------------------------------------------------//
#include <eastl/functional.h>
#include <eastl/memory.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Detail {

	template <typename T, bool isTriviallyDestructible>
	template <typename U, typename... Args>
	ETInlineHint ResultStorage<T, isTriviallyDestructible>::ResultStorage(std::initializer_list<U> initializerList, Args&&... args) :
		errorCode(Error::None) {
		new (eastl::addressof(storage)) ValueType(initializerList, eastl::forward<Args>(args)...);
	}

	// ---------------------------------------------------

	template <typename T, bool isTriviallyDestructible>
	template <typename... Args>
	ETInlineHint ResultStorage<T, isTriviallyDestructible>::ResultStorage(Args&&... args) :
		errorCode(Error::None) {
		new (eastl::addressof(storage)) ValueType(eastl::forward<Args>(args)...);
	}

	// ---------------------------------------------------

	template <typename T, bool isTriviallyDestructible>
	ETInlineHint ResultStorage<T, isTriviallyDestructible>::ResultStorage(ErrorCode errorCode) ETNoexceptHint : errorCode(errorCode) {
		ET_ASSERT(Failed(errorCode), "This usage is considered unsafe. Consider a sentinel result value to avoid uninitialized memory.");
	}

	// ---------------------------------------------------

	template <typename T, bool isTriviallyDestructible>
	ETInlineHint ResultStorage<T, isTriviallyDestructible>::ResultStorage(ResultStorage<T, isTriviallyDestructible>&& other) :
		errorCode(other.errorCode) {
		if (Failed(errorCode)) {
			return;
		}

		new (eastl::addressof(storage)) ValueType(reinterpret_cast<ValueType&&>(other.storage));
	}

	// ---------------------------------------------------

	template <typename T, bool isTriviallyDestructible>
	ETInlineHint ResultStorage<T, isTriviallyDestructible>::~ResultStorage() {
		if (Failed(errorCode)) {
			return;
		}

		DestructValue();
	}

	// ---------------------------------------------------

	template <typename T, bool isTriviallyDestructible>
	ETInlineHint void ResultStorage<T, isTriviallyDestructible>::DestructValue() {
		reinterpret_cast<ValueType*>(eastl::addressof(storage))->~ValueType();
	}

	// ---------------------------------------------------

	template <typename T>
	template <typename U, typename... Args>
	ETInlineHint ResultStorage<T, true>::ResultStorage(std::initializer_list<U> initializerList, Args&&... args) :
		errorCode(Error::None) {
		new (eastl::addressof(storage)) ValueType(initializerList, eastl::forward<Args>(args)...);
	}

	// ---------------------------------------------------

	template <typename T>
	template <typename... Args>
	ETInlineHint ResultStorage<T, true>::ResultStorage(Args&&... args) :
		errorCode(Error::None) {
		new (eastl::addressof(storage)) ValueType(eastl::forward<Args>(args)...);
	}

	// ---------------------------------------------------

	template <typename T>
	ETInlineHint ResultStorage<T, true>::ResultStorage(ErrorCode resultCode) ETNoexceptHint : errorCode(resultCode) {}

	// ---------------------------------------------------

	template <typename T>
	ETInlineHint ResultStorage<T, true>::ResultStorage(ResultStorage<T, true>&& other) {
		if (Failed(errorCode)) {
			return;
		}

		new (eastl::addressof(storage)) ValueType(reinterpret_cast<ValueType&&>(other.storage));
	}

	// ---------------------------------------------------

	template <typename T>
	ETInlineHint void ResultStorage<T, true>::DestructValue() {}

} // namespace Detail

template <typename CreatedObject>
template <typename U, typename... Args, typename /*SFINAE*/>
ETInlineHint Result<CreatedObject>::Result(std::initializer_list<U> initializerList, Args&&... args) :
	Detail::ResultStorage<CreatedObject>(initializerList, eastl::forward<Args>(args)...) {}

// ---------------------------------------------------

template <typename CreatedObject>
template <typename... Args, typename /*SFINAE*/>
ETInlineHint Result<CreatedObject>::Result(Args&&... args) :
	Detail::ResultStorage<CreatedObject>(eastl::forward<Args>(args)...) {}

// ---------------------------------------------------

template <typename CreatedObject>
ETInlineHint Result<CreatedObject>::Result(ErrorCode error) :
	Detail::ResultStorage<CreatedObject>(error) {}

// ---------------------------------------------------

template <typename CreatedObject>
ETInlineHint Result<CreatedObject>::Result(Result<CreatedObject>&& other) :
	Detail::ResultStorage<CreatedObject>(eastl::forward<Detail::ResultStorage<CreatedObject>>(other)) {}

// ---------------------------------------------------

template <typename CreatedObject>
ETInlineHint ErrorCode Result<CreatedObject>::GetErrorCode() const {
	return errorCode;
}

// ---------------------------------------------------

template <typename CreatedObject>
ETInlineHint const CreatedObject* Result<CreatedObject>::operator->() const {
	ET_ASSERT(Succeeded(errorCode), "Referencing result with no object!");

	return reinterpret_cast<const CreatedObject*>(eastl::addressof(storage));
}

// ---------------------------------------------------

template <typename CreatedObject>
ETInlineHint CreatedObject* Result<CreatedObject>::operator->() {
	ET_ASSERT(Succeeded(errorCode), "Referencing result with no object!");

	return reinterpret_cast<CreatedObject*>(eastl::addressof(storage));
}

// ---------------------------------------------------

template <typename CreatedObject>
ETInlineHint const CreatedObject& Result<CreatedObject>::operator*() const {
	ET_ASSERT(Succeeded(errorCode), "Referencing result with no object!");

	return reinterpret_cast<const CreatedObject&>(storage);
}

// ---------------------------------------------------

template <typename CreatedObject>
ETInlineHint CreatedObject& Result<CreatedObject>::operator*() {
	ET_ASSERT(Succeeded(errorCode), "Referencing result with no object!");

	return reinterpret_cast<CreatedObject&>(storage);
}

// ---------------------------------------------------

template <typename CreatedObject>
ETInlineHint Result<CreatedObject>::operator ErrorCode() const {
	return errorCode;
}

} // namespace Eldritch2
