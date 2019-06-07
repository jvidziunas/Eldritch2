/*==================================================================*\
  UniquePointer.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename T, typename Deleter>
template <typename T2, typename Deleter2>
ETInlineHint ETForceInlineHint UniquePointer<T, Deleter>::UniquePointer(UniquePointer<T2, Deleter2>&& other) ETNoexceptHint : UnderlyingContainer(Move(other)) {}

// ---------------------------------------------------

template <typename T, typename Deleter>
ETInlineHint ETForceInlineHint UniquePointer<T, Deleter>::UniquePointer(Pointer pointer, const DeleterType& deleter) ETNoexceptHint : UnderlyingContainer(pointer, deleter) {}

// ---------------------------------------------------

template <typename T, typename Deleter>
ETInlineHint ETForceInlineHint UniquePointer<T, Deleter>::UniquePointer(UniquePointer<T, Deleter>&& source) ETNoexceptHint : UnderlyingContainer(Forward<UnderlyingContainer>(source)) {}

// ---------------------------------------------------

template <typename T, typename Deleter>
ETInlineHint ETForceInlineHint UniquePointer<T, Deleter>::UniquePointer(std::nullptr_t) ETNoexceptHint : UnderlyingContainer(nullptr) {}

// ---------------------------------------------------

template <typename T, typename Deleter>
ETInlineHint ETForceInlineHint UniquePointer<T, Deleter>::UniquePointer() ETNoexceptHint : UnderlyingContainer() {}

// ---------------------------------------------------

template <typename T, typename Deleter>
template <typename T2, typename Deleter2>
ETInlineHint ETForceInlineHint UniquePointer<T, Deleter>& UniquePointer<T, Deleter>::operator=(UniquePointer<T2, Deleter2>&& other) {
	UnderlyingContainer::operator=(Move(other));
	return *this;
}

// ---------------------------------------------------

template <typename T, typename Deleter>
ETInlineHint ETForceInlineHint void UniquePointer<T, Deleter>::Reset(Pointer value) ETNoexceptHint {
	UnderlyingContainer::reset(value);
}

// ---------------------------------------------------

template <typename T, typename Deleter>
ETInlineHint ETForceInlineHint typename UniquePointer<T, Deleter>::Pointer UniquePointer<T, Deleter>::Release() ETNoexceptHint {
	return UnderlyingContainer::release();
}

// ---------------------------------------------------

template <typename T, typename Deleter>
ETInlineHint ETForceInlineHint typename UniquePointer<T, Deleter>::Pointer UniquePointer<T, Deleter>::Get() const ETNoexceptHint {
	return UnderlyingContainer::get();
}

// ---------------------------------------------------

template <typename T, typename Deleter>
ETInlineHint ETForceInlineHint typename const UniquePointer<T, Deleter>::DeleterType& UniquePointer<T, Deleter>::GetDeleter() const ETNoexceptHint {
	return UnderlyingContainer::get_deleter();
}

// ---------------------------------------------------

template <typename T, typename Deleter>
ETInlineHint ETForceInlineHint typename UniquePointer<T, Deleter>::DeleterType& UniquePointer<T, Deleter>::GetDeleter() ETNoexceptHint {
	return UnderlyingContainer::get_deleter();
}

// ---------------------------------------------------

template <typename T, typename Deleter>
ETInlineHint ETForceInlineHint void Swap(UniquePointer<T, Deleter>& lhs, UniquePointer<T, Deleter>& rhs) ETNoexceptHint {
	lhs.swap(rhs);
}

// ---------------------------------------------------

template <typename T, typename Deleter>
ETInlineHint ETForceInlineHint UniquePointer<T[], Deleter>::UniquePointer(Pointer pointer, const DeleterType& deleter) ETNoexceptHint : UnderlyingContainer(pointer, deleter) {}

// ---------------------------------------------------

template <typename T, typename Deleter>
ETInlineHint ETForceInlineHint UniquePointer<T[], Deleter>::UniquePointer(Pointer pointer) ETNoexceptHint : UnderlyingContainer(pointer, DeleterType()) {}

// ---------------------------------------------------

template <typename T, typename Deleter>
ETInlineHint ETForceInlineHint UniquePointer<T[], Deleter>::UniquePointer(std::nullptr_t) ETNoexceptHint : UnderlyingContainer(nullptr) {}

// ---------------------------------------------------

template <typename T, typename Deleter>
ETInlineHint ETForceInlineHint UniquePointer<T[], Deleter>::UniquePointer(UniquePointer<T[], Deleter>&& source) ETNoexceptHint : UnderlyingContainer(Forward<UnderlyingContainer>(source)) {}

// ---------------------------------------------------

template <typename T, typename Deleter>
ETInlineHint ETForceInlineHint typename UniquePointer<T[], Deleter>::Pointer UniquePointer<T[], Deleter>::Begin() const {
	return UnderlyingContainer::get();
}

// ---------------------------------------------------

template <typename T, typename Deleter>
ETInlineHint ETForceInlineHint typename UniquePointer<T[], Deleter>::Pointer UniquePointer<T[], Deleter>::End() const {
	return UnderlyingContainer::get() + GetSize();
}

// ---------------------------------------------------

template <typename T, typename Deleter>
ETInlineHint ETForceInlineHint size_t UniquePointer<T[], Deleter>::GetSize() const {
	return UnderlyingContainer::get_deleter().GetSize();
}

// ---------------------------------------------------

template <typename T, typename Deleter>
template <typename Deleter2>
ETInlineHint ETForceInlineHint UniquePointer<T[], Deleter>& UniquePointer<T[], Deleter>::operator=(UniquePointer<T[], Deleter2>&& other) {
	UnderlyingContainer::operator=(Move(other));
	return *this;
}

// ---------------------------------------------------

template <typename T, typename Deleter>
ETInlineHint ETForceInlineHint UniquePointer<T[], Deleter>::operator bool() const {
	return UnderlyingContainer::operator bool();
}

// ---------------------------------------------------

template <typename T, typename Deleter>
ETInlineHint ETForceInlineHint void UniquePointer<T[], Deleter>::Reset(Pointer value, SizeType sizeInElements) ETNoexceptHint {
	UnderlyingContainer::reset(value);
	UnderlyingContainer::get_deleter().SetSize(sizeInElements);
}

// ---------------------------------------------------

template <typename T, typename Deleter>
ETInlineHint ETForceInlineHint void UniquePointer<T[], Deleter>::Reset() ETNoexceptHint {
	UnderlyingContainer::reset();
}

// ---------------------------------------------------

template <typename T, typename Deleter>
ETInlineHint ETForceInlineHint typename UniquePointer<T[], Deleter>::Pointer UniquePointer<T[], Deleter>::Release() ETNoexceptHint {
	return UnderlyingContainer::release();
}

// ---------------------------------------------------

template <typename T, typename Deleter>
ETInlineHint ETForceInlineHint typename UniquePointer<T[], Deleter>::Pointer UniquePointer<T[], Deleter>::Get() const ETNoexceptHint {
	return UnderlyingContainer::get();
}

// ---------------------------------------------------

template <typename T, typename Deleter>
ETInlineHint ETForceInlineHint typename const UniquePointer<T[], Deleter>::DeleterType& UniquePointer<T[], Deleter>::GetDeleter() const ETNoexceptHint {
	return UnderlyingContainer::get_deleter();
}

// ---------------------------------------------------

template <typename T, typename Deleter>
ETInlineHint ETForceInlineHint typename UniquePointer<T[], Deleter>::DeleterType& UniquePointer<T[], Deleter>::GetDeleter() ETNoexceptHint {
	return UnderlyingContainer::get_deleter();
}

// ---------------------------------------------------

template <typename T, typename Deleter>
ETInlineHint ETForceInlineHint void Swap(UniquePointer<T[], Deleter>& lhs, UniquePointer<T[], Deleter>& rhs) ETNoexceptHint {
	lhs.swap(rhs);
}

// ---------------------------------------------------

template <typename T, typename Deleter>
ETInlineHint ETForceInlineHint ETPureFunctionHint size_t GetHashCode(const UniquePointer<T, Deleter>& object, size_t seed = 0u) {
	return GetHashCode(*object, seed);
}

// ---------------------------------------------------

template <typename T, typename Deleter>
ETInlineHint ETForceInlineHint ETPureFunctionHint bool operator==(const UniquePointer<T, Deleter>& lhs, const UniquePointer<T, Deleter>& rhs) {
	return lhs.Get() == rhs.Get();
}

// ---------------------------------------------------

template <typename T, typename Deleter>
ETInlineHint ETForceInlineHint ETPureFunctionHint bool operator!=(const UniquePointer<T, Deleter>& lhs, const UniquePointer<T, Deleter>& rhs) {
	return lhs.Get() != rhs.Get();
}

// ---------------------------------------------------

template <typename T, typename Deleter>
ETInlineHint ETForceInlineHint ETPureFunctionHint bool operator==(const UniquePointer<T, Deleter>& lhs, decltype(nullptr) /*rhs*/) {
	return !lhs;
}

// ---------------------------------------------------

template <typename T, typename Deleter>
ETInlineHint ETForceInlineHint ETPureFunctionHint bool operator!=(const UniquePointer<T, Deleter>& lhs, decltype(nullptr) /*rhs*/) {
	return !!lhs;
}

// ---------------------------------------------------

template <typename T, typename Deleter>
ETInlineHint ETForceInlineHint ETPureFunctionHint bool operator<(const UniquePointer<T, Deleter>& lhs, const UniquePointer<T, Deleter>& rhs) {
	return lhs.Get() < rhs.Get();
}

// ---------------------------------------------------

template <typename T, typename Deleter>
ETInlineHint ETForceInlineHint ETPureFunctionHint bool operator<=(const UniquePointer<T, Deleter>& lhs, const UniquePointer<T, Deleter>& rhs) {
	return lhs.Get() <= rhs.Get();
}

// ---------------------------------------------------

template <typename T, typename Deleter>
ETInlineHint ETForceInlineHint ETPureFunctionHint bool operator>(const UniquePointer<T, Deleter>& lhs, const UniquePointer<T, Deleter>& rhs) {
	return lhs.Get() > rhs.Get();
}

// ---------------------------------------------------

template <typename T, typename Deleter>
ETInlineHint ETForceInlineHint ETPureFunctionHint bool operator>=(const UniquePointer<T, Deleter>& lhs, const UniquePointer<T, Deleter>& rhs) {
	return lhs.Get() > rhs.Get();
}

// ---------------------------------------------------

template <typename Object, typename... Args>
ETInlineHint ETForceInlineHint UniquePointer<Object, InstanceDeleter> MakeUnique(Allocator& allocator, Args&&... args) {
	Object* const object(static_cast<Object*>(allocator.Allocate(sizeof(Object))));
	return { new (object) Object(Forward<Args>(args)...), InstanceDeleter(allocator, object) };
}

// ---------------------------------------------------

template <typename Object, typename... Args>
ETInlineHint ETForceInlineHint UniquePointer<Object[], InstanceArrayDeleter> MakeUniqueArray(Allocator& allocator, size_t sizeInElements, Args&... args) {
	const auto objects(static_cast<Object*>(allocator.Allocate(sizeInElements * sizeof(Object))));
	ForEach(objects, objects + sizeInElements, [&](Object& object) {
		new (ETAddressOf(object)) Object(args...);
	});

	return { objects, InstanceArrayDeleter(allocator, objects, sizeInElements) };
}

// ---------------------------------------------------

template <typename Object, typename Iterator>
ETInlineHint ETForceInlineHint UniquePointer<Object[], InstanceArrayDeleter> MakeUniqueArray(Allocator& allocator, Iterator begin, Iterator end) {
	const auto size(eastl::distance(begin, end));
	const auto objects(static_cast<Object*>(allocator.Allocate(size * sizeof(Object))));

	eastl::uninitialized_move(begin, end, objects);
	return { objects, InstanceArrayDeleter(allocator, objects, size_t(size)) };
}

} // namespace Eldritch2
