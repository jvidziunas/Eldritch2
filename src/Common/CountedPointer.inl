/*==================================================================*\
  CountedPointer.inl
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

	template <class Object, typename Disposer>
	template <typename CompatibleObject>
	ETInlineHint CountedPointer<Object, Disposer>::CountedPointer( CompatibleObject pointer, const DisposerType& disposer ) : _pointerAndDeleter( pointer, disposer ) {}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	template <typename CompatibleObject, typename CompatibleDeleter>
	ETInlineHint CountedPointer<Object, Disposer>::CountedPointer( const CountedPointer<CompatibleObject, CompatibleDeleter>& pointer ) : CountedPointer( nullptr ) {
		*this = pointer;
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	ETInlineHint CountedPointer<Object, Disposer>::CountedPointer( const CountedPointer<Object, Disposer>& pointer ) : CountedPointer( nullptr ) {
		*this = pointer;
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	template <typename CompatibleObject, typename CompatibleDeleter>
	ETInlineHint CountedPointer<Object, Disposer>::CountedPointer( CountedPointer<CompatibleObject, CompatibleDeleter>&& pointer ) : CountedPointer( nullptr ) {
		eastl::swap( _pointerAndDeleter, pointer._pointerAndDeleter );
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	ETInlineHint CountedPointer<Object, Disposer>::CountedPointer( CountedPointer<Object, Disposer>&& pointer ) ETNoexceptHint : CountedPointer( nullptr ) {
		eastl::swap( _pointerAndDeleter, pointer._pointerAndDeleter );
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	ETInlineHint CountedPointer<Object, Disposer>::CountedPointer( decltype( nullptr ) ) : _pointerAndDeleter( nullptr, Disposer() ) {}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	ETInlineHint CountedPointer<Object, Disposer>::~CountedPointer() {
		Reset();
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	ETInlineHint Object* CountedPointer<Object, Disposer>::Release() ETNoexceptHint {
		return eastl::exchange( _pointerAndDeleter.first(), nullptr );
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	ETInlineHint Object* CountedPointer<Object, Disposer>::Get() const ETNoexceptHint {
		return _pointerAndDeleter.first();
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	ETInlineHint void CountedPointer<Object, Disposer>::Reset( Object* pointer ) {
		const auto old( eastl::exchange( _pointerAndDeleter.first(), pointer ) );

		if (old && (old->ReleaseReference() == 1u)) {
			GetDisposer()( *old );
		}

		if (pointer) {
			pointer->AddReference();
		}
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	ETInlineHint const typename CountedPointer<Object, Disposer>::DisposerType& CountedPointer<Object, Disposer>::GetDisposer() const ETNoexceptHint {
		return _pointerAndDeleter.second();
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	ETInlineHint Object* CountedPointer<Object, Disposer>::operator->() const ETNoexceptHint {
		return _pointerAndDeleter.first();
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	ETInlineHint Object& CountedPointer<Object, Disposer>::operator*() const ETNoexceptHint {
		return *_pointerAndDeleter.first();
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	ETInlineHint CountedPointer<Object, Disposer>::operator bool() const ETNoexceptHint {
		return _pointerAndDeleter.first() != nullptr;
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	template <typename CompatibleObject, typename CompatibleDeleter>
	ETInlineHint CountedPointer<Object, Disposer>& CountedPointer<Object, Disposer>::operator=( const CountedPointer<CompatibleObject, CompatibleDeleter>& pointer ) ETNoexceptHint {
		Reset( pointer._pointerAndDeleter.first() );

		_pointerAndDeleter.second() = pointer._pointerAndDeleter.second();

		return *this;
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	ETInlineHint CountedPointer<Object, Disposer>& CountedPointer<Object, Disposer>::operator=( const CountedPointer<Object, Disposer>& pointer ) ETNoexceptHint {
		Reset( pointer._pointerAndDeleter.first() );

		_pointerAndDeleter.second() = pointer._pointerAndDeleter.second();

		return *this;
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	template <typename CompatibleObject, typename CompatibleDeleter>
	ETInlineHint CountedPointer<Object, Disposer>& CountedPointer<Object, Disposer>::operator=( CountedPointer<CompatibleObject, CompatibleDeleter>&& pointer ) ETNoexceptHint {
		Reset( eastl::exchange( pointer._pointerAndDeleter.first(), nullptr ) );

		_pointerAndDeleter.second()	= eastl::move( handle._pointerAndDeleter.second() );

		return *this;
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	ETInlineHint CountedPointer<Object, Disposer>& CountedPointer<Object, Disposer>::operator=( CountedPointer<Object, Disposer>&& pointer ) ETNoexceptHint {
		Reset( eastl::exchange( pointer._pointerAndDeleter.first(), nullptr ) );

		_pointerAndDeleter = eastl::move( pointer._pointerAndDeleter );

		return *this;
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	template <typename CompatibleObject>
	ETInlineHint CountedPointer<Object, Disposer>& CountedPointer<Object, Disposer>::operator=( CompatibleObject* object ) ETNoexceptHint {
		Reset( object );

		return *this;
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	ETInlineHint ETPureFunctionHint bool operator<( const CountedPointer<Object, Disposer>& object0, const CountedPointer<Object, Disposer>& object1 ) {
		return object0.Get() < object1.Get();
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	ETInlineHint ETPureFunctionHint bool operator<=( const CountedPointer<Object, Disposer>& object0, const CountedPointer<Object, Disposer>& object1 ) {
		return object0.Get() <= object1.Get();
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	ETInlineHint ETPureFunctionHint bool operator==( const CountedPointer<Object, Disposer>& object0, const CountedPointer<Object, Disposer>& object1 ) {
		return object0.Get() == object1.Get();
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	ETInlineHint ETPureFunctionHint bool operator==( const CountedPointer<Object, Disposer>& object0, const Object* object1 ) {
		return object0.Get() == object1;
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	ETInlineHint ETPureFunctionHint bool operator!=( const CountedPointer<Object, Disposer>& object0, const CountedPointer<Object, Disposer>& object1 ) {
		return object0.Get() != object1.Get();
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	ETInlineHint ETPureFunctionHint bool operator!=( const CountedPointer<Object, Disposer>& object0, const Object* object1 ) {
		return object0.Get() != object1;
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	ETInlineHint ETPureFunctionHint bool operator>=( const CountedPointer<Object, Disposer>& object0, const CountedPointer<Object, Disposer>& object1 ) {
		return object0.Get() >= object1.Get();
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	ETInlineHint ETPureFunctionHint bool operator>( const CountedPointer<Object, Disposer>& object0, const CountedPointer<Object, Disposer>& object1 ) {
		return object0.Get() > object1.Get();
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	ETInlineHint ETPureFunctionHint size_t GetHashCode( const CountedPointer<Object, Disposer>& object, size_t seed = 0u ) {
		return seed + 31 * (reinterpret_cast<uintptr>(object.Get()) / sizeof(object));
	}

}	// namespace Eldritch2