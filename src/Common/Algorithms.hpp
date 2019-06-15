/*==================================================================*\
  Algorithms.hpp
  ------------------------------------------------------------------
  Purpose:
  Supplemental function collection designed to complement/augment
  the STL algorithms header.

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

struct InvokeDestructor {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref InvokeDestructor instance.
	ETConstexpr InvokeDestructor(const InvokeDestructor&) ETNoexceptHint = default;
	//!	Constructs this @ref InvokeDestructor instance.
	ETConstexpr InvokeDestructor() ETNoexceptHint = default;

	~InvokeDestructor() = default;

	// ---------------------------------------------------

public:
	template <typename ObjectType>
	ETInlineHint ETForceInlineHint void operator()(ObjectType* object) const {
		eastl::destruct(object);
	}
	template <typename ObjectType>
	ETInlineHint ETForceInlineHint void operator()(ObjectType& object) const {
		eastl::destruct(ETAddressOf(object));
	}
};

// ---------------------------------------------------

struct InvokeTrivialConstructor {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref InvokeTrivialConstructor instance.
	ETConstexpr InvokeTrivialConstructor(const InvokeTrivialConstructor&) ETNoexceptHint = default;
	//!	Constructs this @ref InvokeTrivialConstructor instance.
	ETConstexpr InvokeTrivialConstructor() ETNoexceptHint = default;

	~InvokeTrivialConstructor() = default;

	// ---------------------------------------------------

public:
	template <typename ObjectType>
	ETInlineHint ETForceInlineHint void operator()(ObjectType& object) const ETNoexceptHintIf(IsNoThrowConstructible<ObjectType>()) {
		new (ETAddressOf(object)) ObjectType();
	}
	template <typename ObjectType>
	ETInlineHint ETForceInlineHint void operator()(ObjectType* object) const ETNoexceptHintIf(IsNoThrowConstructible<ObjectType>()) {
		new (object) ObjectType();
	}
};

// ---------------------------------------------------

template <typename Counter>
struct CountingIterator {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//! Constructs this @ref CountingIterator instance.
	ETConstexpr ETForceInlineHint CountingIterator(const CountingIterator&) ETNoexceptHint = default;
	//! Constructs this @ref CountingIterator instance.
	ETConstexpr ETForceInlineHint CountingIterator(Counter value) ETNoexceptHint : _value(Move(value)) {}

	~CountingIterator() = default;

	// ---------------------------------------------------

public:
	ETConstexpr ETForceInlineHint Counter operator*() const ETNoexceptHint {
		return _value;
	}

	ETCpp14Constexpr ETForceInlineHint CountingIterator operator++(ETPostfixSemantics) ETNoexceptHint {
		return CountingIterator(_value++);
	}
	ETCpp14Constexpr ETForceInlineHint CountingIterator& operator++() ETNoexceptHint {
		++value;
		return *this;
	}

	ETCpp14Constexpr ETForceInlineHint CountingIterator operator--(ETPostfixSemantics) ETNoexceptHint {
		return CountingIterator(_value--);
	}
	ETCpp14Constexpr ETForceInlineHint CountingIterator& operator--() ETNoexceptHint {
		--value;
		return *this;
	}

	// - DATA MEMBERS ------------------------------------

private:
	Counter _value;
};

// ---------------------------------------------------

template <typename InputIterator, typename UnaryPredicate>
InputIterator FindIf(InputIterator begin, InputIterator end, UnaryPredicate condition) ETNoexceptHintIf(IsNoThrowCallable<UnaryPredicate, typename DereferenceType<InputIterator>::Type>());

template <typename InputIterator, typename Value>
InputIterator Find(InputIterator begin, InputIterator end, const Value& value) ETNoexceptHintIf(IsNoThrowCallable<EqualTo<Value>, const Value&, typename DereferenceType<InputIterator>::Type>());

// ---------------------------------------------------

template <typename InputIterator, typename UnaryPredicate>
InputIterator FindIfNot(InputIterator begin, InputIterator end, UnaryPredicate condition) ETNoexceptHintIf(IsNoThrowCallable<UnaryPredicate, typename DereferenceType<InputIterator>::Type>());

// ---------------------------------------------------

template <typename InputIterator, typename UnaryPredicate>
bool All(InputIterator begin, InputIterator end, UnaryPredicate condition) ETNoexceptHintIf(IsNoThrowCallable<UnaryPredicate, typename DereferenceType<InputIterator>::Type>());

template <typename InputIterator, typename UnaryPredicate>
bool Any(InputIterator begin, InputIterator end, UnaryPredicate condition) ETNoexceptHintIf(IsNoThrowCallable<UnaryPredicate, typename DereferenceType<InputIterator>::Type>());

// ---------------------------------------------------

//	Applies the operator() method of the specified predicate to each element in the specified range.
//	Returns a copy of the predicate after all operations have been performed.
template <typename InputIterator, typename UnaryPredicate>
UnaryPredicate ForEach(InputIterator begin, InputIterator end, UnaryPredicate predicate) ETNoexceptHintIf(IsNoThrowCallable<UnaryPredicate, typename DereferenceType<InputIterator>::Type>());
template <typename InputIterator, typename InputIterator2, typename BinaryPredicate>
BinaryPredicate ForEach(InputIterator begin, InputIterator end, InputIterator2 begin2, BinaryPredicate predicate) ETNoexceptHintIf(IsNoThrowCallable<BinaryPredicate, typename DereferenceType<InputIterator>::Type, typename DereferenceType<InputIterator2>::Type>());
template <typename Tuple, typename UnaryPredicate>
UnaryPredicate ForEach(const Tuple&, UnaryPredicate predicate);
template <typename Tuple, typename UnaryPredicate>
UnaryPredicate ForEach(Tuple&&, UnaryPredicate predicate);
template <typename Tuple, typename UnaryPredicate>
UnaryPredicate ForEach(Tuple&, UnaryPredicate predicate);

template <typename Tuple, typename... Args>
void ForEach2(const Tuple&, Args&&...);
template <typename Tuple, typename... Args>
void ForEach2(Tuple&&, Args&&...);
template <typename Tuple, typename... Args>
void ForEach2(Tuple&, Args&&...);

// ---------------------------------------------------

template <typename InputIterator, typename Value, typename BinaryPredicate>
Value Reduce(InputIterator begin, InputIterator end, Value initial, BinaryPredicate reducer);

// ---------------------------------------------------

template <typename InputIterator>
InputIterator Unique(InputIterator begin, InputIterator end);
template <typename InputIterator, typename BinaryPredicate>
InputIterator Unique(InputIterator begin, InputIterator end, BinaryPredicate equal);

// ---------------------------------------------------

template <typename InputIterator, typename OutputIterator, typename UnaryPredicate>
OutputIterator Transform(InputIterator begin, InputIterator end, OutputIterator output, UnaryPredicate transform);
template <typename InputIterator, typename InputIterator2, typename OutputIterator, typename BinaryPredicate>
OutputIterator Transform(InputIterator begin, InputIterator end, InputIterator2 begin2, OutputIterator output, BinaryPredicate transform);

// ---------------------------------------------------

template <typename OutputIterator, typename Generator>
OutputIterator Generate(OutputIterator begin, OutputIterator end, Generator generator);
template <typename OutputIterator, typename Size, typename Generator>
OutputIterator GenerateN(OutputIterator begin, Size count, Generator generator);

// ---------------------------------------------------

template <typename ForwardIterator, typename UnaryPredicate>
ForwardIterator RemoveIf(ForwardIterator begin, ForwardIterator end, UnaryPredicate filter);

template <typename ForwardIterator, typename Value>
ForwardIterator Remove(ForwardIterator begin, ForwardIterator end, const Value& value);

// ---------------------------------------------------

template <typename OutputIterator, typename Value>
void Fill(OutputIterator begin, OutputIterator end, const Value& value);

// ---------------------------------------------------

template <typename InputIterator, typename ValueType, typename BinaryPredicate>
InputIterator LowerBound(InputIterator begin, InputIterator end, const ValueType& value, BinaryPredicate compare);

template <typename InputIterator, typename ValueType, typename BinaryPredicate>
InputIterator UpperBound(InputIterator begin, InputIterator end, const ValueType& value, BinaryPredicate compare);

// ---------------------------------------------------

template <typename InputIterator>
void Sort(InputIterator begin, InputIterator end);
template <typename InputIterator, typename BinaryPredicate>
void Sort(InputIterator begin, InputIterator end, BinaryPredicate compare);

// ---------------------------------------------------

template <typename InputIterator>
void TrivialConstruct(InputIterator begin, InputIterator end);

template <typename InputIterator>
void Destroy(InputIterator begin, InputIterator end);

// ---------------------------------------------------

template <typename InputIterator, typename OutputIterator>
OutputIterator Copy(InputIterator begin, InputIterator end, OutputIterator output);

template <typename InputIterator, typename OutputIterator, typename Terminator>
OutputIterator CopyAndTerminate(InputIterator begin, InputIterator end, OutputIterator output, Terminator terminator);

// ---------------------------------------------------

template <typename Container>
typename Container::ConstIterator Begin(const Container&) ETNoexceptHintIf(HasNoThrowMemberBegin<const Container>());
template <typename Container>
typename Container::Iterator Begin(Container&) ETNoexceptHintIf(HasNoThrowMemberBegin<Container>());
template <typename Value, size_t ArraySize>
ETConstexpr const Value* Begin(const Value (&)[ArraySize]) ETNoexceptHint;
template <typename Value, size_t ArraySize>
ETConstexpr Value* Begin(Value (&)[ArraySize]) ETNoexceptHint;
template <typename Value>
ETConstexpr const Value* Begin(const InitializerList<Value>&) ETNoexceptHint;

template <typename Container>
typename Container::ConstIterator End(const Container&) ETNoexceptHintIf(HasNoThrowMemberEnd<const Container>());
template <typename Container>
typename Container::Iterator End(Container&) ETNoexceptHintIf(HasNoThrowMemberEnd<Container>());
template <typename Value, size_t ArraySize>
ETConstexpr const Value* End(const Value (&)[ArraySize]) ETNoexceptHint;
template <typename Value, size_t ArraySize>
ETConstexpr Value* End(Value (&)[ArraySize]) ETNoexceptHint;
template <typename Value>
ETConstexpr const Value* End(const InitializerList<Value>&) ETNoexceptHint;

// ---------------------------------------------------

template <typename T, size_t ArraySize>
ETCpp14Constexpr void Swap(T (&)[ArraySize], T (&)[ArraySize]) ETNoexceptHintIf(IsNoThrowSwappable<T>());
template <typename T, ET_TEMPLATE_SFINAE(!IsArray<T>())>
ETCpp14Constexpr void Swap(T&, T&) ETNoexceptHintIf(IsNoThrowMoveConstructible<T>() && IsNoThrowMoveAssignable<T>());

template <typename T, typename U>
ETCpp14Constexpr T Exchange(T&, U&&) ETNoexceptHintIf(IsNoThrowMoveConstructible<T>() && IsNoThrowAssignable<T, U&&>());

template <typename T>
ETConstexpr CountingIterator<T> AsCounter(T value) ETNoexceptHint;

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Algorithms.inl>
//------------------------------------------------------------------//
