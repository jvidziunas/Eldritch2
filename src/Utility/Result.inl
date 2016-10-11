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
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//
#include <EASTL/functional.h>
#include <EASTL/memory.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Detail {

	template <typename T, bool isTriviallyDestructible>
	template <typename U, typename... Args>
	ETForceInlineHint ResultStorage<T, isTriviallyDestructible>::ResultStorage( std::initializer_list<U> initializerList, Args&&... args ) : errorCode( Eldritch2::Error::None ) {
		new(eastl::addressof(storage)) ValueType( initializerList, eastl::forward<Args>(args)... );
	}
	
// ---------------------------------------------------

	template <typename T, bool isTriviallyDestructible>
	template <typename... Args>
	ETForceInlineHint ResultStorage<T, isTriviallyDestructible>::ResultStorage( Args&&... args ) : errorCode( Eldritch2::Error::None ) {
		new(eastl::addressof(storage)) ValueType( eastl::forward<Args>(args)... );
	}

// ---------------------------------------------------

	template <typename T, bool isTriviallyDestructible>
	ETForceInlineHint ResultStorage<T, isTriviallyDestructible>::ResultStorage( Eldritch2::ErrorCode errorCode ) EA_NOEXCEPT : errorCode( errorCode ) {
		ETRuntimeAssertWithMessage( !errorCode, "This usage is considered unsafe. Consider a sentinel result value to avoid uninitialized memory." );
	}

// ---------------------------------------------------

	template <typename T, bool isTriviallyDestructible>
	ETForceInlineHint ResultStorage<T, isTriviallyDestructible>::ResultStorage( ResultStorage<T, isTriviallyDestructible>&& other ) : errorCode( other.errorCode ) {
		if( !errorCode ) {
			return;
		}

		new(eastl::addressof(storage)) ValueType( reinterpret_cast<ValueType&&>(other.storage) );
	}

// ---------------------------------------------------

	template <typename T, bool isTriviallyDestructible>
	ETForceInlineHint ResultStorage<T, isTriviallyDestructible>::~ResultStorage() {
		if( !errorCode ) {
			return;
		}

		DestructValue();
	}

// ---------------------------------------------------

	template <typename T, bool isTriviallyDestructible>
	ETForceInlineHint void ResultStorage<T, isTriviallyDestructible>::DestructValue() {
		reinterpret_cast<ValueType*>(eastl::addressof( storage ))->~ValueType();
	}

// ---------------------------------------------------

	template <typename T>
	template <typename U, typename... Args>
	ETForceInlineHint ResultStorage<T, true>::ResultStorage( std::initializer_list<U> initializerList, Args&&... args ) : errorCode( Eldritch2::Error::None ) {
		new(eastl::addressof( storage )) ValueType( initializerList, eastl::forward<Args>( args )... );
	}

// ---------------------------------------------------

	template <typename T>
	template <typename... Args>
	ETForceInlineHint ResultStorage<T, true>::ResultStorage( Args&&... args ) : errorCode( Eldritch2::Error::None ) {
		new(eastl::addressof( storage )) ValueType( eastl::forward<Args>( args )... );
	}

// ---------------------------------------------------

	template <typename T>
	ETForceInlineHint ResultStorage<T, true>::ResultStorage( Eldritch2::ErrorCode resultCode ) EA_NOEXCEPT : errorCode( resultCode ) {}

// ---------------------------------------------------

	template <typename T>
	ETForceInlineHint ResultStorage<T, true>::ResultStorage( ResultStorage<T, true>&& other ) {
		if( !errorCode ) {
			return;
		}

		new(eastl::addressof( storage )) ValueType( reinterpret_cast<ValueType&&>(other.storage) );
	}

// ---------------------------------------------------

	template <typename T>
	ETForceInlineHint void ResultStorage<T, true>::DestructValue() {}

}	// namespace Detail

	template <typename CreatedObject>
	template <typename U, typename... Args, typename /*SFINAE*/>
	ETForceInlineHint Result<CreatedObject>::Result( std::initializer_list<U> initializerList, Args&&... args ) : Detail::ResultStorage<CreatedObject>( initializerList, eastl::forward<Args>( args )... ) {}
	
// ---------------------------------------------------

	template <typename CreatedObject>
	template <typename... Args, typename /*SFINAE*/>
	ETForceInlineHint Result<CreatedObject>::Result( Args&&... args ) : Detail::ResultStorage<CreatedObject>( eastl::forward<Args>( args )... ) {}

// ---------------------------------------------------

	template <typename CreatedObject>
	ETForceInlineHint Result<CreatedObject>::Result( Eldritch2::ErrorCode error ) : Detail::ResultStorage<CreatedObject>( error ) {}

// ---------------------------------------------------

	template <typename CreatedObject>
	ETForceInlineHint Result<CreatedObject>::Result( Result<CreatedObject>&& other ) : Detail::ResultStorage<CreatedObject>( eastl::forward<Detail::ResultStorage<CreatedObject>>( other ) ) {}

// ---------------------------------------------------

	template <typename CreatedObject>
	ETForceInlineHint Eldritch2::ErrorCode Result<CreatedObject>::GetErrorCode() const {
		return errorCode;
	}

// ---------------------------------------------------

	template <typename CreatedObject>
	ETForceInlineHint const CreatedObject* Result<CreatedObject>::operator->() const {
		ETRuntimeAssert( *this );

		return reinterpret_cast<const CreatedObject*>( eastl::addressof( storage ) );
	}

// ---------------------------------------------------

	template <typename CreatedObject>
	ETForceInlineHint CreatedObject* Result<CreatedObject>::operator->() {
		ETRuntimeAssert( *this );

		return reinterpret_cast<CreatedObject*>( eastl::addressof( storage ) );
	}

// ---------------------------------------------------

	template <typename CreatedObject>
	ETForceInlineHint const CreatedObject& Result<CreatedObject>::operator*() const {
		ETRuntimeAssert( *this );

		return reinterpret_cast<const CreatedObject&>( storage );
	}

// ---------------------------------------------------

	template <typename CreatedObject>
	ETForceInlineHint CreatedObject& Result<CreatedObject>::operator*() {
		ETRuntimeAssert( *this );

		return reinterpret_cast<CreatedObject&>(storage);
	}

// ---------------------------------------------------

	template <typename CreatedObject>
	ETForceInlineHint Result<CreatedObject>::operator Eldritch2::ErrorCode() const {
		return errorCode;
	}

// ---------------------------------------------------

	template <typename CreatedObject>
	ETForceInlineHint Result<CreatedObject>::operator bool() const {
		return static_cast<bool>(errorCode);
	}

}	// namespace Eldritch2