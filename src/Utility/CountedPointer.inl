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

	template <typename Object>
	ETInlineHint ETPureFunctionHint void	NoopDisposer::operator()( Object& /*object*/ ) const {}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	template <typename CompatibleObject, class /*SFINAE*/>
	ETInlineHint CountedPointer<Object, Disposer>::CountedPointer( CompatibleObject pointer, const DisposerType& disposer ) : _pointerAndDisposer( pointer, disposer ) {
		if( !pointer ) {
			return;
		}
			
		pointer->AddReference();
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	template <typename CompatibleObject, typename CompatibleDisposer, class /*SFINAE*/>
	ETInlineHint CountedPointer<Object, Disposer>::CountedPointer( const CountedPointer<CompatibleObject, CompatibleDisposer>& pointer ) : CountedPointer( pointer._pointerAndDisposer.first(), pointer._pointerAndDisposer.second() ) {}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	ETInlineHint CountedPointer<Object, Disposer>::CountedPointer( const CountedPointer<Object, Disposer>& pointer ) : _pointerAndDisposer( pointer._pointerAndDisposer.first(), pointer._pointerAndDisposer.second() ) {}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	template <typename CompatibleObject, typename CompatibleDisposer, class /*SFINAE*/>
	ETInlineHint CountedPointer<Object, Disposer>::CountedPointer( CountedPointer<CompatibleObject, CompatibleDisposer>&& handle ) : _pointerAndDisposer( pointer._pointerAndDisposer.first(), pointer._pointerAndDisposer.second() ) {
		pointer._pointerAndDisposer.first() = nullptr;
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	ETInlineHint CountedPointer<Object, Disposer>::CountedPointer( CountedPointer<Object, Disposer>&& pointer ) : _pointerAndDisposer( pointer._pointerAndDisposer.first(), eastl::move( pointer._pointerAndDisposer.second() ) ) {
		pointer._pointerAndDisposer.first() = nullptr;
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	ETInlineHint CountedPointer<Object, Disposer>::CountedPointer( decltype( nullptr ) ) : _pointer( nullptr ) {}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	ETInlineHint CountedPointer<Object, Disposer>::~CountedPointer() {
		if( _pointerAndDisposer.first() && ( 1u == _pointerAndDisposer.first()->ReleaseReference() ) ) {
			_pointerAndDisposer.second()( *_pointerAndDisposer.first() );
		}
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	ETInlineHint bool CountedPointer<Object, Disposer>::IsSoleReferenceToObject() const {
		return (1u == _pointer->GetReferenceCount());
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	ETInlineHint Object* CountedPointer<Object, Disposer>::Release() {
		Object*	const	result( _pointer );

		_pointer = nullptr;

		return result;
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	ETInlineHint Object* CountedPointer<Object, Disposer>::Get() const {
		return _pointerAndDisposer.first();
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	template <typename CompatibleObject, class /*SFINAE*/>
	ETInlineHint void CountedPointer<Object, Disposer>::Reset( CompatibleObject* pointer ) {
		if( _pointerAndDisposer.first() && (1u == _pointerAndDisposer.first()->ReleaseReference()) ) {
			_pointerAndDisposer.second()( *_pointerAndDisposer.first() );
		}

		_pointerAndDisposer.first() = pointer;
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	ETInlineHint const typename CountedPointer<Object, Disposer>::DisposerType& CountedPointer<Object, Disposer>::GetDisposer() const {
		return _pointerAndDisposer.second();
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	ETInlineHint Object* CountedPointer<Object, Disposer>::operator->() const {
		return _pointerAndDisposer.first();
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	ETInlineHint Object& CountedPointer<Object, Disposer>::operator*() const {
		return *_pointerAndDisposer.first();
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	ETInlineHint CountedPointer<Object, Disposer>::operator bool() const {
		return nullptr != _pointerAndDisposer.first();
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	template <typename CompatibleObject, typename CompatibleDisposer, class /*SFINAE*/>
	ETInlineHint CountedPointer<Object, Disposer>& CountedPointer<Object, Disposer>::operator=( const CountedPointer<CompatibleObject, CompatibleDisposer>& handle ) {
		if( _pointerAndDisposer.first() && ( 1u == _pointerAndDisposer.first()->ReleaseReference() ) ) {
			_pointerAndDisposer.second()( *_pointerAndDisposer.first() );
		}

		_pointerAndDisposer = handle._pointerAndDisposer;

		return *this;
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	ETInlineHint CountedPointer<Object, Disposer>& CountedPointer<Object, Disposer>::operator=( const CountedPointer<Object, Disposer>& handle ) {
		if( _pointerAndDisposer.first() && ( 1u == _pointerAndDisposer.first()->ReleaseReference() ) ) {
			_pointerAndDisposer.second()( *_pointerAndDisposer.first() );
		}

		_pointerAndDisposer.first()		= handle._pointerAndDisposer.first();
		_pointerAndDisposer.second()	= handle._pointerAndDisposer.second();

		return *this;
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	template <typename CompatibleObject, typename CompatibleDisposer, class /*SFINAE*/>
	ETInlineHint CountedPointer<Object, Disposer>& CountedPointer<Object, Disposer>::operator=( CountedPointer<CompatibleObject, CompatibleDisposer>&& handle ) {
		if( _pointerAndDisposer.first() && ( 1u == _pointerAndDisposer.first()->ReleaseReference() ) ) {
			_pointerAndDisposer.second()( *_pointerAndDisposer.first() );
		}

		_pointerAndDisposer.first()		= eastl::move( handle._pointerAndDisposer.first() );
		_pointerAndDisposer.second()	= eastl::move( handle._pointerAndDisposer.second() );

		return *this;
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	ETInlineHint CountedPointer<Object, Disposer>& CountedPointer<Object, Disposer>::operator=( CountedPointer<Object, Disposer>&& handle ) {
		if( _pointerAndDisposer.first() && ( 1u == _pointerAndDisposer.first()->ReleaseReference() ) ) {
			_pointerAndDisposer.second()( *_pointerAndDisposer.first() );
		}

		_pointerAndDisposer = eastl::move( handle._pointerAndDisposer );

		return *this;
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	template <typename CompatibleObject, class /*SFINAE*/>
	ETInlineHint CountedPointer<Object, Disposer>& CountedPointer<Object, Disposer>::operator=( CompatibleObject* object ) {
		if( _pointerAndDisposer.first() && ( 1u == _pointerAndDisposer.first()->ReleaseReference() ) ) {
			_pointerAndDisposer.second()( *_pointerAndDisposer.first() );
		}

		_pointerAndDisposer.first() = object;

		return *this;
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	ETInlineHint ETPureFunctionHint bool	operator<( const CountedPointer<Object, Disposer>& object0, const CountedPointer<Object, Disposer>& object1 ) {
		return object0.Get() < object1.Get();
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	ETInlineHint ETPureFunctionHint bool	operator<=( const CountedPointer<Object, Disposer>& object0, const CountedPointer<Object, Disposer>& object1 ) {
		return object0.Get() <= object1.Get();
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	ETInlineHint ETPureFunctionHint bool	operator==( const CountedPointer<Object, Disposer>& object0, const CountedPointer<Object, Disposer>& object1 ) {
		return object0.Get() == object1.Get();
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	ETInlineHint ETPureFunctionHint bool	operator==( const CountedPointer<Object, Disposer>& object0, const Object* object1 ) {
		return object0.Get() == object1;
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	ETInlineHint ETPureFunctionHint bool	operator!=( const CountedPointer<Object, Disposer>& object0, const CountedPointer<Object, Disposer>& object1 ) {
		return object0.Get() != object1.Get();
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	ETInlineHint ETPureFunctionHint bool	operator!=( const CountedPointer<Object, Disposer>& object0, const Object* object1 ) {
		return object0.Get() != object1;
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	ETInlineHint ETPureFunctionHint bool	operator>=( const CountedPointer<Object, Disposer>& object0, const CountedPointer<Object, Disposer>& object1 ) {
		return object0.Get() >= object1.Get();
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	ETInlineHint ETPureFunctionHint bool	operator>( const CountedPointer<Object, Disposer>& object0, const CountedPointer<Object, Disposer>& object1 ) {
		return object0.Get() > object1.Get();
	}

// ---------------------------------------------------

	template <class Object, typename Disposer>
	ETInlineHint ETPureFunctionHint size_t GetHashCode( const CountedPointer<Object, Disposer>& object, size_t seed = 0u ) {
		return seed + 31 * (reinterpret_cast<uintptr>(object.Get()) >> _alignof(object));
	}

}	// namespace Eldritch2