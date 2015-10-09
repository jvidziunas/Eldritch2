/*==================================================================*\
  ObjectHandle.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {

	namespace Scripting = ::Eldritch2::Scripting;

// ---------------------------------------------------

	template <class Object>
	ETInlineHint ObjectHandle<Object>::ObjectHandle() : ObjectHandle( nullptr, ::Eldritch2::PassthroughReferenceCountingSemantics ) {}

// ---------------------------------------------------

	template <class Object>
	ETInlineHint ObjectHandle<Object>::ObjectHandle( decltype(nullptr) value ) : ObjectHandle( static_cast<Object*>(value), ::Eldritch2::PassthroughReferenceCountingSemantics ) {}

// ---------------------------------------------------

	template <class Object>
	ETInlineHint ObjectHandle<Object>::ObjectHandle( Object* const pointer, const DefaultReferenceCountingSemantics ) : ObjectHandle( pointer, ::Eldritch2::PassthroughReferenceCountingSemantics ) {
		if( pointer ) {
			pointer->AddReference();
		}
	}

// ---------------------------------------------------

	template <class Object>
	ETInlineHint ObjectHandle<Object>::ObjectHandle( Object* const pointer, const PassthroughReferenceCountingSemantics ) : _pointer( pointer ) {}

// ---------------------------------------------------

	template <class Object>
	ETInlineHint ObjectHandle<Object>::ObjectHandle( Object& reference, const DefaultReferenceCountingSemantics ) : ObjectHandle( reference, ::Eldritch2::PassthroughReferenceCountingSemantics ) {
		reference.AddReference();
	}

// ---------------------------------------------------

	template <class Object>
	ETInlineHint ObjectHandle<Object>::ObjectHandle( Object& reference, const PassthroughReferenceCountingSemantics ) : _pointer( &reference ) {}

// ---------------------------------------------------

	template <class Object>
	ETInlineHint ObjectHandle<Object>::ObjectHandle( const Scripting::ObjectHandle<Object>& handle ) : ObjectHandle( handle._pointer ) {}

// ---------------------------------------------------

	template <class Object>
	ETInlineHint ObjectHandle<Object>::ObjectHandle( Scripting::ObjectHandle<Object>&& handle ) : ObjectHandle( handle._pointer, ::Eldritch2::PassthroughReferenceCountingSemantics ) {
		handle._pointer = nullptr;
	}

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
	ETInlineHint Scripting::ObjectHandle<Object>& ObjectHandle<Object>::operator=( const Scripting::ObjectHandle<Object>& handle ) {
		return (*this) = handle._pointer;
	}

// ---------------------------------------------------

	template <class Object>
	ETInlineHint Scripting::ObjectHandle<Object>& ObjectHandle<Object>::operator=( Scripting::ObjectHandle<Object>&& handle ) {
		if( _pointer ) {
			_pointer->ReleaseReference();
		}

		_pointer = handle._pointer;

		handle._pointer = nullptr;

		return *this;
	}

// ---------------------------------------------------

	template <class Object>
	ETInlineHint Scripting::ObjectHandle<Object>& ObjectHandle<Object>::operator=( Object* const object ) {
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