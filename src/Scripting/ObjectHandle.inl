/*==================================================================*\
  ObjectHandle.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <type_traits>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {

	template <class Object>
	template <typename CompatibleObject>
	ETInlineHint ObjectHandle<Object>::ObjectHandle( CompatibleObject* const pointer, const DefaultReferenceCountingSemantics ) : ObjectHandle( pointer, ::Eldritch2::PassthroughReferenceCountingSemantics ) {
		static_assert( ::std::is_convertible<CompatibleObject*, Object*>::value, "Object handles can only be assigned to compatible types!" );

	// ---

		if( pointer ) {
			pointer->AddReference();
		}
	}

// ---------------------------------------------------

	template <class Object>
	template <typename CompatibleObject>
	ETInlineHint ObjectHandle<Object>::ObjectHandle( CompatibleObject* const pointer, const PassthroughReferenceCountingSemantics ) : _pointer( pointer ) {
		static_assert( ::std::is_convertible<CompatibleObject*, Object*>::value, "Object handles can only be assigned to compatible types!" );
	}

// ---------------------------------------------------

	template <class Object>
	template <typename CompatibleObject>
	ETInlineHint ObjectHandle<Object>::ObjectHandle( CompatibleObject& reference, const DefaultReferenceCountingSemantics ) : ObjectHandle( reference, ::Eldritch2::PassthroughReferenceCountingSemantics ) {
		static_assert( ::std::is_convertible<CompatibleObject*, Object*>::value, "Object handles can only be assigned to compatible types!" );

	// ---

		reference.AddReference();
	}

// ---------------------------------------------------

	template <class Object>
	template <typename CompatibleObject>
	ETInlineHint ObjectHandle<Object>::ObjectHandle( CompatibleObject& reference, const PassthroughReferenceCountingSemantics ) : _pointer( &reference ) {
		static_assert(::std::is_convertible<CompatibleObject*, Object*>::value, "Object handles can only be assigned to compatible types!");
	}

// ---------------------------------------------------

	template <class Object>
	template <typename CompatibleObject>
	ETInlineHint ObjectHandle<Object>::ObjectHandle( const ObjectHandle<CompatibleObject>& handle ) : ObjectHandle( handle._pointer ) {
		static_assert( ::std::is_convertible<CompatibleObject*, Object*>::value, "Object handles can only be assigned to compatible types!" );
	}

// ---------------------------------------------------

	template <class Object>
	template <typename CompatibleObject>
	ETInlineHint ObjectHandle<Object>::ObjectHandle( ObjectHandle<CompatibleObject>&& handle ) : ObjectHandle( handle.Release(), ::Eldritch2::PassthroughReferenceCountingSemantics ) {
		static_assert( ::std::is_convertible<CompatibleObject*, Object*>::value, "Object handles can only be assigned to compatible types!" );
	}

// ---------------------------------------------------

	template <class Object>
	ETInlineHint ObjectHandle<Object>::ObjectHandle( const ObjectHandle<Object>& handle ) : ObjectHandle( handle._pointer ) {}

// ---------------------------------------------------

	template <class Object>
	ETInlineHint ObjectHandle<Object>::ObjectHandle( decltype(nullptr) value ) : ObjectHandle( static_cast<Object*>(value), ::Eldritch2::PassthroughReferenceCountingSemantics ) {}

// ---------------------------------------------------

	template <class Object>
	ETInlineHint ObjectHandle<Object>::ObjectHandle() : ObjectHandle( nullptr ) {}

// ---------------------------------------------------

	template <class Object>
	ETInlineHint ObjectHandle<Object>::~ObjectHandle() {
		if( _pointer ) {
			_pointer->ReleaseReference();
		}
	}

// ---------------------------------------------------

	template <class Object>
	ETInlineHint bool ObjectHandle<Object>::IsSoleReferenceToObject() const {
		return (1u == _pointer->GetReferenceCount());
	}

// ---------------------------------------------------

	template <class Object>
	ETInlineHint Object* ObjectHandle<Object>::Release() {
		Object*	const	result( _pointer );

		_pointer = nullptr;

		return result;
	}

// ---------------------------------------------------

	template <class Object>
	ETInlineHint Object* ObjectHandle<Object>::operator->() {
		return _pointer;
	}

// ---------------------------------------------------

	template <class Object>
	ETInlineHint const Object* ObjectHandle<Object>::operator->() const {
		return _pointer;
	}

// ---------------------------------------------------

	template <class Object>
	ETInlineHint Object& ObjectHandle<Object>::operator*() {
		return *_pointer;
	}

// ---------------------------------------------------

	template <class Object>
	ETInlineHint const Object& ObjectHandle<Object>::operator*() const {
		return *_pointer;
	}

// ---------------------------------------------------

	template <class Object>
	ETInlineHint ObjectHandle<Object>& ObjectHandle<Object>::operator=( const ObjectHandle<Object>& handle ) {
		return (*this) = handle._pointer;
	}

// ---------------------------------------------------

	template <class Object>
	template <typename CompatibleObject>
	ETInlineHint ObjectHandle<Object>& ObjectHandle<Object>::operator=( const ObjectHandle<CompatibleObject>& handle ) {
		static_assert( ::std::is_convertible<CompatibleObject*, Object*>::value, "Object handles can only be assigned to compatible types!" );

	// ---

		return (*this) = handle._pointer;
	}

// ---------------------------------------------------

	template <class Object>
	template <typename CompatibleObject>
	ETInlineHint ObjectHandle<Object>& ObjectHandle<Object>::operator=( ObjectHandle<CompatibleObject>&& handle ) {
		static_assert( ::std::is_convertible<CompatibleObject*, Object*>::value, "Object handles can only be assigned to compatible types!" );

	// ---

		if( _pointer ) {
			_pointer->ReleaseReference();
		}

		_pointer = handle._pointer;

		handle._pointer = nullptr;

		return *this;
	}

// ---------------------------------------------------

	template <class Object>
	template <typename CompatibleObject>
	ETInlineHint ObjectHandle<Object>& ObjectHandle<Object>::operator=( CompatibleObject* const object ) {
		static_assert( ::std::is_convertible<CompatibleObject*, Object*>::value, "Object handles can only be assigned to compatible types!" );

	// ---

		if( object ) {
			object->AddReference();
		}

		if( _pointer ) {
			_pointer->ReleaseReference();
		}

		_pointer = object;

		return *this;
	}

// ---------------------------------------------------

	template <class Object>
	ETInlineHint ObjectHandle<Object>::operator Object*() {
		return _pointer;
	}

// ---------------------------------------------------

	template <class Object>
	ETInlineHint ObjectHandle<Object>::operator const Object*() const {
		return _pointer;
	}

}	// namespace Scripting
}	// namespace Eldritch2