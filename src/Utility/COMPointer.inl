/*==================================================================*\
  COMPointer.inl
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
namespace Utility {

	template <class InterfaceType>
	ETInlineHint COMPointer<InterfaceType>::COMPointer() : _pointer( nullptr ) {}

// ---------------------------------------------------

	template <class InterfaceType>
	ETInlineHint COMPointer<InterfaceType>::COMPointer( decltype(nullptr) value ) : COMPointer<InterfaceType>( static_cast<InterfaceType*>(value) ) {}

// ---------------------------------------------------

	template <class InterfaceType>
	template <class CompatibleInterfaceType>
	ETInlineHint COMPointer<InterfaceType>::COMPointer( CompatibleInterfaceType* const pointer ) : COMPointer<InterfaceType>( pointer, ::Eldritch2::PassthroughReferenceCountingSemantics ) {
		if( pointer ) {
			pointer->AddRef();
		}
	}

// ---------------------------------------------------

	template <class InterfaceType>
	template <class CompatibleInterfaceType>
	ETInlineHint COMPointer<InterfaceType>::COMPointer( CompatibleInterfaceType* const pointer, const PassthroughReferenceCountingSemantics ) : _pointer( pointer ) {}

// ---------------------------------------------------

	template <class InterfaceType>
	template <class CompatibleInterfaceType>
	ETInlineHint COMPointer<InterfaceType>::COMPointer( CompatibleInterfaceType& reference ) : COMPointer<InterfaceType>( reference, ::Eldritch2::PassthroughReferenceCountingSemantics ) {
		reference.AddRef();
	}

// ---------------------------------------------------

	template <class InterfaceType>
	template <class CompatibleInterfaceType>
	ETInlineHint COMPointer<InterfaceType>::COMPointer( CompatibleInterfaceType& reference, const PassthroughReferenceCountingSemantics ) : _pointer( &reference ) {}

// ---------------------------------------------------

	template <class InterfaceType>
	template <class CompatibleInterfaceType>
	ETInlineHint COMPointer<InterfaceType>::COMPointer( const Utility::COMPointer<CompatibleInterfaceType>& handle ) : COMPointer<InterfaceType>( handle._pointer ) {}

// ---------------------------------------------------

	template <class InterfaceType>
	template <class CompatibleInterfaceType>
	ETInlineHint COMPointer<InterfaceType>::COMPointer( Utility::COMPointer<CompatibleInterfaceType>&& handle ) : COMPointer<InterfaceType>( handle._pointer, ::Eldritch2::PassthroughReferenceCountingSemantics ) {
		handle._pointer = nullptr;
	}

// ---------------------------------------------------

	template <class InterfaceType>
	COMPointer<InterfaceType>::~COMPointer() {
		if( nullptr != _pointer ) {
			_pointer->Release();
		}
	}

// ---------------------------------------------------

	template <class InterfaceType>
	ETForceInlineHint ETNoThrowHint InterfaceType* COMPointer<InterfaceType>::GetUnadornedPointer() const throw() {
		return _pointer;
	}

// ---------------------------------------------------

	template <class InterfaceType>
	ETForceInlineHint ETNoThrowHint void COMPointer<InterfaceType>::Reset() throw() {
		this->Acquire( nullptr, ::Eldritch2::PassthroughReferenceCountingSemantics );
	}

// ---------------------------------------------------

	template <class InterfaceType>
	ETNoThrowHint void COMPointer<InterfaceType>::Acquire( InterfaceType* const pointer ) throw() {
		if( nullptr != pointer ) {
			pointer->AddRef();
		}

		Acquire( pointer, ::Eldritch2::PassthroughReferenceCountingSemantics );		
	}

// ---------------------------------------------------

	template <class InterfaceType>
	ETNoThrowHint void COMPointer<InterfaceType>::Acquire( InterfaceType* const pointer, const PassthroughReferenceCountingSemantics ) throw() {
		if( nullptr != _pointer ) {
			_pointer->Release();
		}

		_pointer = pointer;
	}

// ---------------------------------------------------

	template <class InterfaceType>
	template <class CompatibleInterfaceType>
	ETInlineHint Utility::COMPointer<InterfaceType>& COMPointer<InterfaceType>::operator=( const Utility::COMPointer<CompatibleInterfaceType>& other ) {
		static_assert( ::std::is_convertible<CompatibleInterfaceType*, InterfaceType*>::value, "COM pointers can only be assigned to compatible types!" );

	// ---

		Acquire( static_cast<InterfaceType*>(other._pointer) );

		return *this;
	}

// ---------------------------------------------------

	template <class InterfaceType>
	template <class CompatibleInterfaceType>
	ETInlineHint Utility::COMPointer<InterfaceType>& COMPointer<InterfaceType>::operator=( Utility::COMPointer<CompatibleInterfaceType>&& other ) {
		static_assert( ::std::is_convertible<CompatibleInterfaceType*, InterfaceType*>::value, "COM pointers can only be assigned to compatible types!" );

	// ---

		Acquire( other._pointer, ::Eldritch2::PassthroughReferenceCountingSemantics );
		other._pointer = nullptr;

		return *this;
	}

// ---------------------------------------------------

	template <class InterfaceType>
	template <class CompatibleInterfaceType>
	COMPointer<InterfaceType>& COMPointer<InterfaceType>::operator=( CompatibleInterfaceType* const pointer ) {
		static_assert( ::std::is_convertible<CompatibleInterfaceType*, InterfaceType*>::value, "COM pointers can only be assigned to compatible types!" );

	// ---

		Acquire( pointer );

		return *this;
	}

// ---------------------------------------------------

	template <class InterfaceType>
	ETForceInlineHint InterfaceType* COMPointer<InterfaceType>::operator->() const {
		return _pointer;
	}

// ---------------------------------------------------

	template <class InterfaceType>
	ETForceInlineHint COMPointer<InterfaceType>::operator bool() const {
		return nullptr != _pointer;
	}

// ---------------------------------------------------

	template <class InterfaceType>
	ETForceInlineHint InterfaceType** COMPointer<InterfaceType>::GetInterfacePointer() {
		this->Reset();
		return &_pointer;
	}

}	// namespace Utility
}	// namespace Eldritch2