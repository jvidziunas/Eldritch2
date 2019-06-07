/*==================================================================*\
  ComPointer.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Mpl/TypeTraits.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <class Interface>
class ComPointer {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref ComPointer instance.
	template <class CompatibleInterface>
	ComPointer(const ComPointer<CompatibleInterface>&) ETNoexceptHintIf(IsNoThrowAddRef<CompatibleInterface>());
	//!	Constructs this @ref ComPointer instance.
	template <class CompatibleInterface>
	ComPointer(ComPointer<CompatibleInterface>&&) ETNoexceptHint;
	//!	Constructs this @ref ComPointer instance.
	template <class CompatibleInterface>
	ComPointer(CompatibleInterface*) ETNoexceptHint;
	//!	Constructs this @ref ComPointer instance.
	ComPointer(const ComPointer&) ETNoexceptHintIf(IsNoThrowAddRef<Interface>());
	//!	Constructs this @ref ComPointer instance.
	ComPointer(decltype(nullptr)) ETNoexceptHint;
	//!	Constructs this @ref ComPointer instance.
	ComPointer() ETNoexceptHint;

	//!	Destroys this @ref ComPointer instance.
	~ComPointer();

	// ---------------------------------------------------

public:
	Interface* Release() ETNoexceptHint;

	Interface* Get() const ETNoexceptHint;

	void Reset(Interface* = nullptr) ETNoexceptHintIf(IsNoThrowRelease<Interface>());

	void Acquire(Interface*) ETNoexceptHintIf(IsNoThrowAddRef<Interface>() && IsNoThrowRelease<Interface>());

	// ---------------------------------------------------

public:
	template <class CompatibleInterface>
	ComPointer& operator=(const ComPointer<CompatibleInterface>&) ETNoexceptHintIf(IsNoThrowAddRef<CompatibleInterface>() && IsNoThrowRelease<Interface>());
	template <class CompatibleInterface>
	ComPointer& operator=(ComPointer<CompatibleInterface>&&) ETNoexceptHintIf(IsNoThrowAddRef<CompatibleInterface>() && IsNoThrowRelease<Interface>());
	template <class CompatibleInterface>
	ComPointer& operator=(CompatibleInterface*) ETNoexceptHintIf(IsNoThrowAddRef<CompatibleInterface>() && IsNoThrowRelease<Interface>());
	ComPointer& operator=(const ComPointer&) ETNoexceptHintIf(IsNoThrowAddRef<Interface>() && IsNoThrowRelease<Interface>());

	// ---------------------------------------------------

public:
	Interface** GetInterfacePointer() ETNoexceptHintIf(IsNoThrowRelease<Interface>());

	Interface* operator->() const ETNoexceptHint;

	explicit operator bool() const ETNoexceptHint;

	// - DATA MEMBERS ------------------------------------

private:
	Interface* _pointer;

	// ---------------------------------------------------

	template <typename T>
	friend void Swap(ComPointer<T>&, ComPointer<T>&) ETNoexceptHint;
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/ComPointer.inl>
//------------------------------------------------------------------//
