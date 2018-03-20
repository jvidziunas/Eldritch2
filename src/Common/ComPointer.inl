/*==================================================================*\
  ComPointer.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {

	template <class Interface>
	ETInlineHint ComPointer<Interface>::ComPointer() : _pointer( nullptr ) {}

// ---------------------------------------------------

	template <class Interface>
	ETInlineHint ComPointer<Interface>::ComPointer( decltype(nullptr) value ) : _pointer( value ) {}

// ---------------------------------------------------

	template <class Interface>
	template <class CompatibleInterface>
	ETInlineHint ComPointer<Interface>::ComPointer( ComPointer<CompatibleInterface>&& pointer ) : _pointer( pointer.Release() ) {}

// ---------------------------------------------------

	template <class Interface>
	ETInlineHint ComPointer<Interface>::ComPointer( const ComPointer<Interface>& source ) : ComPointer<Interface>( source.Get() ) {}

// ---------------------------------------------------

	template <class Interface>
	template <class CompatibleInterface>
	ETInlineHint ComPointer<Interface>::ComPointer( CompatibleInterface* pointer ) : _pointer( pointer ) {
		if (pointer) {
			pointer->AddRef();
		}
	}

// ---------------------------------------------------

	template <class Interface>
	ComPointer<Interface>::~ComPointer() {
		Reset();
	}

// ---------------------------------------------------

	template <class Interface>
	ETInlineHint ETNeverThrowsHint Interface* ComPointer<Interface>::Release() const throw() {
		Interface*	result( _pointer );

		_pointer = nullptr;

		return result;
	}

// ---------------------------------------------------

	template <class Interface>
	ETInlineHint ETNeverThrowsHint Interface* ComPointer<Interface>::Get() const throw() {
		return _pointer;
	}

// ---------------------------------------------------

	template <class Interface>
	ETInlineHint ETNeverThrowsHint void ComPointer<Interface>::Reset() throw() {
		if (_pointer != nullptr) {
			_pointer->Release();

			_pointer = nullptr;	
		}
	}

// ---------------------------------------------------

	template <class Interface>
	ETInlineHint ETNeverThrowsHint void ComPointer<Interface>::Acquire( Interface* pointer ) throw() {
		if (_pointer) _pointer->Release();
		if (pointer)  pointer->AddRef();

		_pointer = pointer;		
	}

// ---------------------------------------------------

	template <class Interface>
	ETInlineHint ETNeverThrowsHint ComPointer<Interface>& ComPointer<Interface>::operator=( const ComPointer<Interface>& pointer ) {
		Acquire( pointer.Get() );

		return *this;
	}

// ---------------------------------------------------

	template <class Interface>
	template <class CompatibleInterface>
	ETInlineHint ETNeverThrowsHint ComPointer<Interface>& ComPointer<Interface>::operator=( const ComPointer<CompatibleInterface>& pointer ) {
		static_assert( std::is_convertible<CompatibleInterface*, Interface*>::value, "COM pointers can only be assigned to compatible types!" );

	// ---

		Acquire( pointer.Get() );

		return *this;
	}

// ---------------------------------------------------

	template <class Interface>
	template <class CompatibleInterface>
	ETInlineHint ETNeverThrowsHint ComPointer<Interface>& ComPointer<Interface>::operator=( ComPointer<CompatibleInterface>&& pointer ) {
		static_assert( std::is_convertible<CompatibleInterface*, Interface*>::value, "COM pointers can only be assigned to compatible types!" );

	// ---

		Reset();

		_pointer = pointer.Release();

		return *this;
	}

// ---------------------------------------------------

	template <class Interface>
	template <class CompatibleInterface>
	ETInlineHint ETNeverThrowsHint ComPointer<Interface>& ComPointer<Interface>::operator=( CompatibleInterface* pointer ) {
		static_assert( std::is_convertible<CompatibleInterface*, Interface*>::value, "COM pointers can only be assigned to compatible types!" );

	// ---

		Acquire( pointer );

		return *this;
	}

// ---------------------------------------------------

	template <class Interface>
	ETInlineHint Interface* ComPointer<Interface>::operator->() const {
		return _pointer;
	}

// ---------------------------------------------------

	template <class Interface>
	ETInlineHint ComPointer<Interface>::operator bool() const {
		return _pointer != nullptr;
	}

// ---------------------------------------------------

	template <class Interface>
	ETInlineHint Interface** ComPointer<Interface>::GetInterfacePointer() {
		this->Reset();

		return &_pointer;
	}

// ---------------------------------------------------

	template <class Interface>
	ETInlineHint void Swap( ComPointer<Interface> pointer0, ComPointer<Interface>& pointer1 ) {
		Interface* const	temp( pointer1._pointer );

		pointer0._pointer = _pointer;
		pointer1._pointer = temp;
	}

}	// namespace Eldritch2