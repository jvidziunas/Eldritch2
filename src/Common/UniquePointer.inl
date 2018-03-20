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
	ETInlineHint UniquePointer<T, Deleter>::UniquePointer( UniquePointer<AlternateObject, AlternateDeleter>&& other ) ETNoexceptHint : UnderlyingContainer( eastl::move( other ) ) {}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint UniquePointer<T, Deleter>::UniquePointer( Pointer pointer, const DeleterType& deleter ) ETNoexceptHint : UnderlyingContainer( pointer, deleter ) {}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint UniquePointer<T, Deleter>::UniquePointer( UniquePointer<T, Deleter>&& source ) ETNoexceptHint : UnderlyingContainer( eastl::forward<UnderlyingContainer>( source ) ) {}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint UniquePointer<T, Deleter>::UniquePointer( std::nullptr_t ) ETNoexceptHint : UnderlyingContainer( nullptr ) {}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint UniquePointer<T, Deleter>::UniquePointer() ETNoexceptHint : UnderlyingContainer() {}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	template <typename AlternateObject, typename AlternateDeleter>
	ETInlineHint UniquePointer<T, Deleter>& UniquePointer<T, Deleter>::operator=( UniquePointer<AlternateObject, AlternateDeleter>&& other ) {
		UnderlyingContainer::operator=( eastl::forward<typename UniquePointer<AlternateObject, AlternateDeleter>::UnderlyingContainer>( other ) );
		return *this;
	}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint UniquePointer<T, Deleter>& UniquePointer<T, Deleter>::operator=( UniquePointer<T, Deleter>&& other ) {
		UnderlyingContainer::operator=( eastl::forward<UnderlyingContainer>( other ) );
		return *this;
	}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint void UniquePointer<T, Deleter>::Reset( Pointer value ) ETNoexceptHint {
		UnderlyingContainer::reset( value );
	}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint typename UniquePointer<T, Deleter>::Pointer UniquePointer<T, Deleter>::Release() ETNoexceptHint {
		return UnderlyingContainer::release();
	}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint typename UniquePointer<T, Deleter>::Pointer UniquePointer<T, Deleter>::Get() const ETNoexceptHint {
		return UnderlyingContainer::get();
	}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint typename const UniquePointer<T, Deleter>::DeleterType& UniquePointer<T, Deleter>::GetDeleter() const ETNoexceptHint {
		return UnderlyingContainer::get_deleter();
	}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint typename UniquePointer<T, Deleter>::DeleterType& UniquePointer<T, Deleter>::GetDeleter() ETNoexceptHint {
		return UnderlyingContainer::get_deleter();
	}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint void Swap( UniquePointer<T, Deleter>& pointer0, UniquePointer<T, Deleter>& pointer1 ) {
		eastl::swap( static_cast<eastl::unique_ptr<T, Deleter>&>(pointer0), static_cast<eastl::unique_ptr<T, Deleter>&>(pointer1) );
	}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint UniquePointer<T[], Deleter>::UniquePointer( Pointer pointer, const DeleterType& deleter ) ETNoexceptHint : UnderlyingContainer( pointer, deleter ) {}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint UniquePointer<T[], Deleter>::UniquePointer( Pointer pointer ) ETNoexceptHint : UnderlyingContainer( pointer, DeleterType() ) {}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint UniquePointer<T[], Deleter>::UniquePointer( std::nullptr_t ) ETNoexceptHint : UnderlyingContainer( nullptr ) {}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint UniquePointer<T[], Deleter>::UniquePointer( UniquePointer<T[], Deleter>&& source ) ETNoexceptHint : UnderlyingContainer( eastl::forward<UnderlyingContainer>( source ) ) {}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint typename UniquePointer<T[], Deleter>::Pointer UniquePointer<T[], Deleter>::Begin() const {
		return UnderlyingContainer::get();
	}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint typename UniquePointer<T[], Deleter>::Pointer UniquePointer<T[], Deleter>::End() const {
		return UnderlyingContainer::get() + GetSize();
	}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint size_t UniquePointer<T[], Deleter>::GetSize() const {
		return UnderlyingContainer::get_deleter().GetSize();
	}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	template <typename AlternateDeleter>
	ETInlineHint UniquePointer<T[], Deleter>& UniquePointer<T[], Deleter>::operator=( UniquePointer<T[], AlternateDeleter>&& other ) {
		UnderlyingContainer::operator=( eastl::move( other ) );
		return *this;
	}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint UniquePointer<T[], Deleter>& UniquePointer<T[], Deleter>::operator=( UniquePointer<T[], Deleter>&& other ) {
		UnderlyingContainer::operator=( eastl::move( other ) );
		return *this;
	}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint UniquePointer<T[], Deleter>::operator bool() const {
		return static_cast<bool>( UnderlyingContainer::operator bool() );
	}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint void UniquePointer<T[], Deleter>::Reset( Pointer value, size_t sizeInElements ) ETNoexceptHint {
		UnderlyingContainer::reset( value );
		UnderlyingContainer::get_deleter().SetSize( sizeInElements );
	}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint void UniquePointer<T[], Deleter>::Reset() ETNoexceptHint {
		UnderlyingContainer::reset();
	}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint typename UniquePointer<T[], Deleter>::Pointer UniquePointer<T[], Deleter>::Release() ETNoexceptHint {
		return UnderlyingContainer::release();
	}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint typename UniquePointer<T[], Deleter>::Pointer UniquePointer<T[], Deleter>::Get() const ETNoexceptHint {
		return UnderlyingContainer::get();
	}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint typename const UniquePointer<T[], Deleter>::DeleterType& UniquePointer<T[], Deleter>::GetDeleter() const ETNoexceptHint {
		return UnderlyingContainer::get_deleter();
	}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint typename UniquePointer<T[], Deleter>::DeleterType& UniquePointer<T[], Deleter>::GetDeleter() ETNoexceptHint {
		return UnderlyingContainer::get_deleter();
	}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint void Swap( UniquePointer<T[], Deleter>& pointer0, UniquePointer<T[], Deleter>& pointer1 ) {
		eastl::swap( static_cast<eastl::unique_ptr<T[], Deleter>&>(pointer0), static_cast<eastl::unique_ptr<T[], Deleter>&>(pointer1) );
	}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint ETPureFunctionHint size_t GetHashCode( const UniquePointer<T, Deleter>& object, size_t seed = 0u ) {
		return GetHashCode( *object, seed );
	}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint ETPureFunctionHint bool operator==( const UniquePointer<T, Deleter>& lhs, const UniquePointer<T, Deleter>& rhs ) {
		return lhs.Get() == rhs.Get();
	}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint ETPureFunctionHint bool operator!=( const UniquePointer<T, Deleter>& lhs, const UniquePointer<T, Deleter>& rhs ) {
		return lhs.Get() != rhs.Get();
	}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint ETPureFunctionHint bool operator==( const UniquePointer<T, Deleter>& lhs, decltype(nullptr) /*rhs*/ ) {
		return !lhs;
	}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint ETPureFunctionHint bool operator!=( const UniquePointer<T, Deleter>& lhs, decltype(nullptr) /*rhs*/ ) {
		return !!lhs;
	}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint ETPureFunctionHint bool operator<( const UniquePointer<T, Deleter>& lhs, const UniquePointer<T, Deleter>& rhs ) {
		return lhs.Get() < rhs.Get();
	}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint ETPureFunctionHint bool operator<=( const UniquePointer<T, Deleter>& lhs, const UniquePointer<T, Deleter>& rhs ) {
		return lhs.Get() <= rhs.Get();
	}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint ETPureFunctionHint bool operator>( const UniquePointer<T, Deleter>& lhs, const UniquePointer<T, Deleter>& rhs ) {
		return lhs.Get() > rhs.Get();
	}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint ETPureFunctionHint bool operator>=( const UniquePointer<T, Deleter>& lhs, const UniquePointer<T, Deleter>& rhs ) {
		return lhs.Get() > rhs.Get();
	}

// ---------------------------------------------------

	template <typename Object, typename... Args>
	ETInlineHint UniquePointer<Object, InstanceDeleter> CreateUnique( Allocator& allocator, Args&&... args ) {
		const auto	object( static_cast<Object*>(allocator.Allocate( sizeof(Object) )) );

		return { new(object) Object( eastl::forward<Args>( args )... ), InstanceDeleter( allocator, object ) };
	}

// ---------------------------------------------------

	template <typename Object, typename... Args>
	ETInlineHint UniquePointer<Object[], InstanceArrayDeleter> CreateUniqueArray( Allocator& allocator, size_t sizeInElements, Args&&... args ) {
		const auto	objects( static_cast<Object*>(allocator.Allocate( sizeInElements * sizeof(Object) )) );
		if (!objects ) {
			return nullptr;
		}
		
		eastl::for_each( objects, objects + sizeInElements, [&] ( Object& object ) {
			new( eastl::addressof( object ) ) Object( eastl::forward<Args>(args)... );
		} );

		return { objects, InstanceArrayDeleter( allocator, objects, sizeInElements ) };
	}

// ---------------------------------------------------

	template <typename Object, typename Iterator>
	ETInlineHint UniquePointer<Object[], InstanceArrayDeleter> CreateUniqueArray( Allocator& allocator, Iterator firstElement, Iterator lastElement ) {
		const auto	sizeInElements( eastl::distance( firstElement, lastElement ) );
		const auto	objects( static_cast<Object*>(allocator.Allocate( sizeInElements * sizeof(Object) )) );
		if (!objects ) {
			return nullptr;
		}

		eastl::uninitialized_move( firstElement, lastElement, objects );

		return { objects, InstanceArrayDeleter( allocator, objects, static_cast<size_t>(sizeInElements) ) };
	}

}	// namespace Eldritch2

