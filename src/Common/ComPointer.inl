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
ETInlineHint ETForceInlineHint ComPointer<Interface>::ComPointer() ETNoexceptHint : _pointer(nullptr) {}

// ---------------------------------------------------

template <class Interface>
ETInlineHint ETForceInlineHint ComPointer<Interface>::ComPointer(decltype(nullptr)) ETNoexceptHint : ComPointer() {}

// ---------------------------------------------------

template <class Interface>
template <class CompatibleInterface>
ETInlineHint ETForceInlineHint ComPointer<Interface>::ComPointer(ComPointer<CompatibleInterface>&& pointer) ETNoexceptHint : ComPointer(pointer.Release()) {}

// ---------------------------------------------------

template <class Interface>
template <class CompatibleInterface>
ETInlineHint ETForceInlineHint ComPointer<Interface>::ComPointer(const ComPointer<CompatibleInterface>& pointer) ETNoexceptHintIf(IsNoThrowAddRef<CompatibleInterface>()) : ComPointer(pointer.Get()) {
	if (_pointer) {
		_pointer->AddRef();
	}
}

// ---------------------------------------------------

template <class Interface>
ETInlineHint ETForceInlineHint ComPointer<Interface>::ComPointer(const ComPointer<Interface>& pointer) ETNoexceptHintIf(IsNoThrowAddRef<Interface>()) : ComPointer<Interface>() {
	*this = pointer;
}

// ---------------------------------------------------

template <class Interface>
template <class CompatibleInterface>
ETInlineHint ETForceInlineHint ComPointer<Interface>::ComPointer(CompatibleInterface* pointer) ETNoexceptHint : _pointer(pointer) {}

// ---------------------------------------------------

template <class Interface>
ETInlineHint ETForceInlineHint ComPointer<Interface>::~ComPointer() {
	Reset();
}

// ---------------------------------------------------

template <class Interface>
ETInlineHint ETForceInlineHint Interface* ComPointer<Interface>::Release() ETNoexceptHint {
	return Exchange(_pointer, nullptr);
}

// ---------------------------------------------------

template <class Interface>
ETInlineHint ETForceInlineHint Interface* ComPointer<Interface>::Get() const ETNoexceptHint {
	return _pointer;
}

// ---------------------------------------------------

template <class Interface>
ETInlineHint ETForceInlineHint void ComPointer<Interface>::Reset(Interface* pointer) ETNoexceptHintIf(IsNoThrowRelease<Interface>()) {
	if (Interface* const old = Exchange(_pointer, pointer)) {
		old->Release();
	}
}

// ---------------------------------------------------

template <class Interface>
ETInlineHint ETForceInlineHint void ComPointer<Interface>::Acquire(Interface* pointer) ETNoexceptHintIf(IsNoThrowAddRef<Interface>() && IsNoThrowRelease<Interface>()) {
	if (pointer)
		pointer->AddRef();

	Reset(pointer);
}

// ---------------------------------------------------

template <class Interface>
ETInlineHint ETForceInlineHint ComPointer<Interface>& ComPointer<Interface>::operator=(const ComPointer<Interface>& pointer) ETNoexceptHintIf(IsNoThrowAddRef<Interface>() && IsNoThrowRelease<Interface>()) {
	return *this = Move(ComPointer<Interface>(pointer));
}

// ---------------------------------------------------

template <class Interface>
template <class CompatibleInterface>
ETInlineHint ETForceInlineHint ComPointer<Interface>& ComPointer<Interface>::operator=(const ComPointer<CompatibleInterface>& pointer) ETNoexceptHintIf(IsNoThrowAddRef<CompatibleInterface>() && IsNoThrowRelease<Interface>()) {
	static_assert(eastl::is_convertible<CompatibleInterface*, Interface*>::value, "COM pointers can only be assigned to compatible types!");

	// ---

	return *this = Move(ComPointer<Interface>(pointer));
}

// ---------------------------------------------------

template <class Interface>
template <class CompatibleInterface>
ETInlineHint ETForceInlineHint ETNeverThrowsHint ComPointer<Interface>& ComPointer<Interface>::operator=(ComPointer<CompatibleInterface>&& pointer) ETNoexceptHintIf(IsNoThrowAddRef<CompatibleInterface>() && IsNoThrowRelease<Interface>()) {
	static_assert(eastl::is_convertible<CompatibleInterface*, Interface*>::value, "COM pointers can only be assigned to compatible types!");

	// ---

	Reset(pointer.Release());
	return *this;
}

// ---------------------------------------------------

template <class Interface>
template <class CompatibleInterface>
ETInlineHint ETForceInlineHint ETNeverThrowsHint ComPointer<Interface>& ComPointer<Interface>::operator=(CompatibleInterface* pointer) ETNoexceptHintIf(IsNoThrowAddRef<CompatibleInterface>() && IsNoThrowRelease<Interface>()) {
	static_assert(eastl::is_convertible<CompatibleInterface*, Interface*>::value, "COM pointers can only be assigned to compatible types!");

	// ---

	Reset(pointer);
	return *this;
}

// ---------------------------------------------------

template <class Interface>
ETInlineHint ETForceInlineHint Interface* ComPointer<Interface>::operator->() const ETNoexceptHint {
	return _pointer;
}

// ---------------------------------------------------

template <class Interface>
ETInlineHint ETForceInlineHint ComPointer<Interface>::operator bool() const ETNoexceptHint {
	return _pointer != nullptr;
}

// ---------------------------------------------------

template <class Interface>
ETInlineHint ETForceInlineHint Interface** ComPointer<Interface>::GetInterfacePointer() ETNoexceptHintIf(IsNoThrowRelease<Interface>()) {
	this->Reset();
	return ETAddressOf(_pointer);
}

// ---------------------------------------------------

template <class Interface>
ETInlineHint ETForceInlineHint void Swap(ComPointer<Interface>& lhs, ComPointer<Interface>& rhs) ETNoexceptHint {
	Swap(lhs._pointer, rhs._pointer);
}

} // namespace Eldritch2
