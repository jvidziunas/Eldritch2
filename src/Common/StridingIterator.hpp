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

//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename T>
class StridingIterator {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref StridingIterator instance.
	ETConstexpr StridingIterator(char* out, size_t byteStride) ETNoexceptHint;
	//!	Constructs this @ref StridingIterator instance.
	ETConstexpr StridingIterator(const StridingIterator&) ETNoexceptHint = default;

	~StridingIterator() = default;

	// ---------------------------------------------------

public:
	ETConstexpr T& operator[](ptrdiff index) const ETNoexceptHint;

	ETConstexpr T& operator*() const ETNoexceptHint;

	// ---------------------------------------------------

public:
	ETConstexpr StridingIterator& operator+=(ptrdiff offset) ETNoexceptHint;
	ETConstexpr StridingIterator& operator-=(ptrdiff offset) ETNoexceptHint;

	ETConstexpr StridingIterator operator++(ETPostfixOperatorHint) ETNoexceptHint;
	ETConstexpr StridingIterator operator++() ETNoexceptHint;

	// ---------------------------------------------------

public:
	ETConstexpr size_t GetByteStride() const ETNoexceptHint;

	ETConstexpr T* GetBase() const ETNoexceptHint;

	// - DATA MEMBERS ------------------------------------

private:
	char*  _out;
	size_t _byteStride;

	// ---------------------------------------------------

	template <typename T>
	friend ETConstexpr StridingIterator<T> operator+(StridingIterator<T>, ptrdiff offset) ETNoexceptHint;
	template <typename T>
	friend ETConstexpr StridingIterator<T> operator-(StridingIterator<T>, ptrdiff offset) ETNoexceptHint;
};

} // namespace Eldritch2
