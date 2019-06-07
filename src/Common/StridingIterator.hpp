/*==================================================================*\
  StridingIterator.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Mpl/IntTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename T>
class StridingIterator {
public:
	using DifferenceType = ptrdiff;
	using SizeType       = size_t;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref StridingIterator instance.
	ETConstexpr StridingIterator(void* out, SizeType byteStride) ETNoexceptHint;
	//!	Constructs this @ref StridingIterator instance.
	ETConstexpr StridingIterator(const StridingIterator&) ETNoexceptHint = default;

	~StridingIterator() = default;

	// ---------------------------------------------------

public:
	ETConstexpr T& operator[](DifferenceType) const ETNoexceptHint;

	ETConstexpr T& operator*() const ETNoexceptHint;

	// ---------------------------------------------------

public:
	ETConstexpr StridingIterator& operator+=(DifferenceType) ETNoexceptHint;
	ETConstexpr StridingIterator& operator-=(DifferenceType) ETNoexceptHint;

	ETConstexpr StridingIterator operator++(ETPostfixSemantics) ETNoexceptHint;
	ETConstexpr StridingIterator operator++() ETNoexceptHint;

	// ---------------------------------------------------

public:
	ETConstexpr SizeType GetByteStride() const ETNoexceptHint;

	ETConstexpr T* GetBase() const ETNoexceptHint;

	// - DATA MEMBERS ------------------------------------

private:
	void*    _out;
	SizeType _byteStride;

	// ---------------------------------------------------

	template <typename T>
	friend ETConstexpr StridingIterator<T> operator+(StridingIterator<T>, typename StridingIterator<T>::DifferenceType) ETNoexceptHint;
	template <typename T>
	friend ETConstexpr StridingIterator<T> operator-(StridingIterator<T>, typename StridingIterator<T>::DifferenceType) ETNoexceptHint;
};

} // namespace Eldritch2
