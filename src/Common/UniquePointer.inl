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
template <typename AlternateObject, typename AlternateDeleter>
ETInlineHint ETForceInlineHint UniquePointer<T, Deleter>::UniquePointer(UniquePointer<AlternateObject, AlternateDeleter>&& other) ETNoexceptHint : UnderlyingContainer(eastl::move(other)) {}

// ---------------------------------------------------

template <typename T, typename Deleter>
ETInlineHint ETForceInlineHint UniquePointer<T, Deleter>::UniquePointer(Pointer pointer, const DeleterType& deleter) ETNoexceptHint : UnderlyingContainer(pointer, deleter) {}

// ---------------------------------------------------

template <typename T, typename Deleter>
ETInlineHint ETForceInlineHint UniquePointer<T, Deleter>::UniquePointer(UniquePointer<T, Deleter>&& source) ETNoexceptHint : UnderlyingContainer(eastl::forward<UnderlyingContainer>(source)) {}

// ---------------------------------------------------

template <typename T, typename Deleter>
ETInlineHint ETForceInlineHint UniquePointer<T, Deleter>::UniquePointer(std::nullptr_t) ETNoexceptHint : UnderlyingContainer(nullptr) {}

// ---------------------------------------------------

template <typename T, typename Deleter>
ETInlineHint ETForceInlineHint UniquePointer<T, Deleter>::UniquePointer() ETNoexceptHint : UnderlyingContainer() {}

// ---------------------------------------------------

template <typename T, typename Deleter>
template <typename AlternateObject, typename AlternateDeleter>
ETInlineHint ETForceInlineHint UniquePointer<T, Deleter>& UniquePointer<T, Deleter>::operator=(UniquePointer<AlternateObject, AlternateDeleter>&& other) {
	UnderlyingContainer::operator=(eastl::move(other));
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
ETInlineHint ETForceInlineHint void Swap(UniquePointer<T, Deleter>& lhs, UniquePointer<T, Deleter>& rhs) {
	eastl::swap(static_cast<eastl::unique_ptr<T, Deleter>&>(lhs), static_cast<eastl::unique_ptr<T, Deleter>&>(rhs));
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
ETInlineHint ETForceInlineHint UniquePointer<T[], Deleter>::UniquePointer(UniquePointer<T[], Deleter>&& source) ETNoexceptHint : UnderlyingContainer(eastl::forward<UnderlyingContainer>(source)) {}

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
template <typename AlternateDeleter>
ETInlineHint ETForceInlineHint UniquePointer<T[], Deleter>& UniquePointer<T[], Deleter>::operator=(UniquePointer<T[], AlternateDeleter>&& other) {
	UnderlyingContainer::operator=(eastl::move(other));
	return *this;
}

// ---------------------------------------------------

template <typename T, typename Deleter>
ETInlineHint ETForceInlineHint UniquePointer<T[], Deleter>::operator bool() const {
	return UnderlyingContainer::operator bool();
}

// ---------------------------------------------------

template <typename T, typename Deleter>
ETInlineHint ETForceInlineHint void UniquePointer<T[], Deleter>::Reset(Pointer value, size_t sizeInElements) ETNoexceptHint {
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
ETInlineHint ETForceInlineHint void Swap(UniquePointer<T[], Deleter>& lhs, UniquePointer<T[], Deleter>& rhs) {
	eastl::swap(static_cast<eastl::unique_ptr<T[], Deleter>&>(lhs), static_cast<eastl::unique_ptr<T[], Deleter>&>(rhs));
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
	return { new (object) Object(eastl::forward<Args>(args)...), InstanceDeleter(allocator, object) };
}

// ---------------------------------------------------

template <typename Object, typename... Args>
ETInlineHint ETForceInlineHint UniquePointer<Object[], InstanceArrayDeleter> MakeUniqueArray(Allocator& allocator, size_t sizeInElements, Args&... args) {
	const auto objects(static_cast<Object*>(allocator.Allocate(sizeInElements * sizeof(Object))));
	eastl::for_each(objects, objects + sizeInElements, [&](Object& object) {
		new (ETAddressOf(object)) Object(args...);
	});

	return { objects, InstanceArrayDeleter(allocator, objects, sizeInElements) };
}

// ---------------------------------------------------

template <typename Object, typename Iterator>
ETInlineHint ETForceInlineHint UniquePointer<Object[], InstanceArrayDeleter> MakeUniqueArray(Allocator& allocator, Iterator first, Iterator last) {
	const auto size(eastl::distance(first, last));
	const auto objects(static_cast<Object*>(allocator.Allocate(size * sizeof(Object))));

	eastl::uninitialized_move(first, last, objects);
	return { objects, InstanceArrayDeleter(allocator, objects, size_t(size)) };
}

} // namespace Eldritch2
