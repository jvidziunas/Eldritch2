/*==================================================================*\
  UniquePointer.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Containers/RangeAdapters.hpp>
#include <Common/Memory/InstanceDeleter.hpp>
#include <Common/Mpl/TypeTraits.hpp>
//------------------------------------------------------------------//
#include <eastl/unique_ptr.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename T, typename Deleter = typename Conditional<IsArray<T>::Value, InstanceArrayDeleter, InstanceDeleter>::Type>
class UniquePointer : private eastl::unique_ptr<T, Deleter> {
	// - TYPE PUBLISHING ---------------------------------

private:
	using UnderlyingContainer = eastl::unique_ptr<T, Deleter>;

public:
	using DeleterType = typename UnderlyingContainer::deleter_type;
	using ElementType = typename UnderlyingContainer::element_type;
	using Pointer     = typename UnderlyingContainer::pointer;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref UniquePointer instance.
	UniquePointer(Pointer pointer, const DeleterType& deleter = DeleterType()) ETNoexceptHint;
	//!	Constructs this @ref UniquePointer instance.
	template <typename T2, typename Deleter2>
	UniquePointer(UniquePointer<T2, Deleter2>&&) ETNoexceptHint;
	//!	Disable copy construction.
	explicit UniquePointer(const UniquePointer&) = delete;
	//!	Constructs this @ref UniquePointer instance.
	UniquePointer(UniquePointer&&) ETNoexceptHint;
	//!	Constructs this @ref UniquePointer instance.
	UniquePointer(std::nullptr_t) ETNoexceptHint;
	//!	Constructs this @ref UniquePointer instance.
	UniquePointer() ETNoexceptHint;

	~UniquePointer() = default;

	// ---------------------------------------------------

public:
	template <typename T2, typename Deleter2>
	UniquePointer& operator=(UniquePointer<T2, Deleter2>&&);
	//!	Disable copy assignment.
	UniquePointer& operator=(const UniquePointer&) = delete;
	UniquePointer& operator=(UniquePointer&&) = default;

	// ---------------------------------------------------

public:
	using UnderlyingContainer::operator*;
	using UnderlyingContainer::operator->;
	using UnderlyingContainer::operator bool;

	// ---------------------------------------------------

public:
	Pointer Release() ETNoexceptHint;

	Pointer Get() const ETNoexceptHint;

	void Reset(Pointer value = Pointer()) ETNoexceptHint;

	// ---------------------------------------------------

public:
	const DeleterType& GetDeleter() const ETNoexceptHint;
	DeleterType&       GetDeleter() ETNoexceptHint;

	// ---------------------------------------------------

	template <typename T2, typename Deleter2>
	friend class UniquePointer;

	template <typename T2, typename Deleter2>
	friend void Swap(UniquePointer<T2, Deleter2>&, UniquePointer<T2, Deleter2>&) ETNoexceptHint;
};

// ---

template <typename T, typename Deleter>
class UniquePointer<T[], Deleter> : private eastl::unique_ptr<T[], Deleter> {
	// - TYPE PUBLISHING ---------------------------------

private:
	using UnderlyingContainer = eastl::unique_ptr<T[], Deleter>;

public:
	using DeleterType = typename UnderlyingContainer::deleter_type;
	using ElementType = typename UnderlyingContainer::element_type;
	using Pointer     = typename UnderlyingContainer::pointer;
	using SizeType    = size_t;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref UniquePointer instance.
	UniquePointer(Pointer pointer, const DeleterType& deleter) ETNoexceptHint;
	//!	Disable copy construction.
	UniquePointer(const UniquePointer&) = delete;
	//!	Constructs this @ref UniquePointer instance.
	UniquePointer(Pointer pointer) ETNoexceptHint;
	//!	Constructs this @ref UniquePointer instance.
	UniquePointer(UniquePointer&&) ETNoexceptHint;
	//!	Constructs this @ref UniquePointer instance.
	UniquePointer(std::nullptr_t) ETNoexceptHint;
	//!	Constructs this @ref UniquePointer instance.
	UniquePointer() = default;

	~UniquePointer() = default;

	// ---------------------------------------------------

public:
	Pointer Begin() const;

	Pointer End() const;

	// ---------------------------------------------------

public:
	size_t GetSize() const;

	// ---------------------------------------------------

public:
	template <typename Deleter2>
	UniquePointer& operator=(UniquePointer<T[], Deleter2>&&);
	//!	Disable copy assignment.
	UniquePointer& operator=(const UniquePointer&) = delete;
	UniquePointer& operator=(UniquePointer&&) = default;

	// ---------------------------------------------------

public:
	using UnderlyingContainer::operator[];

	explicit operator bool() const;

	// ---------------------------------------------------

public:
	void Reset(Pointer pointer, SizeType arraySizeInElements) ETNoexceptHint;
	void Reset() ETNoexceptHint;

	// ---------------------------------------------------

public:
	Pointer Release() ETNoexceptHint;

	Pointer Get() const ETNoexceptHint;

	// ---------------------------------------------------

public:
	const DeleterType& GetDeleter() const ETNoexceptHint;
	DeleterType&       GetDeleter() ETNoexceptHint;

	// ---------------------------------------------------

	template <typename AlternateObject, typename AlternateDeleter>
	friend class UniquePointer;

	template <typename T, typename Deleter>
	friend void Swap(UniquePointer<T[], Deleter>&, UniquePointer<T[], Deleter>&) ETNoexceptHint;
};

// ---------------------------------------------------

template <typename Object, typename... Args>
UniquePointer<Object, InstanceDeleter> MakeUnique(Allocator& allocator, Args&&... args);

template <typename Object, typename... Args>
UniquePointer<Object[], InstanceArrayDeleter> MakeUniqueArray(Allocator& allocator, size_t arraySize, Args&... args);

template <typename Object, typename Iterator>
UniquePointer<Object[], InstanceArrayDeleter> MakeUniqueArray(Allocator& allocator, Iterator begin, Iterator end);

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/UniquePointer.inl>
//------------------------------------------------------------------//
