/*==================================================================*\
  CountedPointer.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Mpl/Compiler.hpp>
//------------------------------------------------------------------//
#include <eastl/bonus/compressed_pair.h>
#include <eastl/type_traits.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

struct NoopDeleter {
	template <typename Object>
	ETInlineHint ETPureFunctionHint void operator()(Object& /*object*/) const {}
};

template <class Object, typename Disposer = NoopDeleter>
class CountedPointer {
	// - TYPE PUBLISHING ---------------------------------

private:
	using DisposerType = Disposer;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref CountedPointer instance.
	template <typename CompatibleObject>
	CountedPointer(CompatibleObject pointer, const DisposerType& deleter = DisposerType());
	//!	Constructs this @ref CountedPointer instance.
	template <typename CompatibleObject, typename CompatibleDeleter>
	CountedPointer(const CountedPointer<CompatibleObject, CompatibleDeleter>& handle);
	//!	Constructs this @ref CountedPointer instance.
	CountedPointer(const CountedPointer& handle);
	//!	Constructs this @ref CountedPointer instance.
	template <typename CompatibleObject, typename CompatibleDeleter>
	CountedPointer(CountedPointer<CompatibleObject, CompatibleDeleter>&& handle);
	//!	Constructs this @ref CountedPointer instance.
	CountedPointer(CountedPointer&&) ETNoexceptHint;
	//!	Constructs this @ref CountedPointer instance.
	CountedPointer(decltype(nullptr) = nullptr);

	~CountedPointer();

	// ---------------------------------------------------

public:
	Object* Release() ETNoexceptHint;

	Object* Get() const ETNoexceptHint;

	void Reset(Object* pointer = nullptr);

	// ---------------------------------------------------

public:
	const DisposerType& GetDisposer() const ETNoexceptHint;

	// ---------------------------------------------------

public:
	template <typename CompatibleObject, typename CompatibleDeleter>
	CountedPointer& operator=(const CountedPointer<CompatibleObject, CompatibleDeleter>& handle) ETNoexceptHint;
	CountedPointer& operator=(const CountedPointer& handle) ETNoexceptHint;
	template <typename CompatibleObject, typename CompatibleDeleter>
	CountedPointer& operator=(CountedPointer<CompatibleObject, CompatibleDeleter>&& handle) ETNoexceptHint;
	CountedPointer& operator=(CountedPointer&&) ETNoexceptHint;
	template <typename CompatibleObject>
	CountedPointer& operator=(CompatibleObject* object) ETNoexceptHint;

	// ---------------------------------------------------

public:
	explicit operator bool() const ETNoexceptHint;

	Object* operator->() const ETNoexceptHint;

	Object& operator*() const ETNoexceptHint;

	// - DATA MEMBERS ------------------------------------

private:
	eastl::compressed_pair<Object*, DisposerType> _pointerAndDeleter;

	// ---------------------------------------------------

	template <class Object2, typename Disposer2>
	friend class ::Eldritch2::CountedPointer;
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/CountedPointer.inl>
//------------------------------------------------------------------//
