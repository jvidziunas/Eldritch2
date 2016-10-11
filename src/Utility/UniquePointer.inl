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
#include <Utility/Memory/InstanceNew.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename T, typename Deleter>
	template <typename AlternateObject, typename AlternateDeleter>
	ETInlineHint UniquePointer<T, Deleter>::UniquePointer( UniquePointer<AlternateObject, AlternateDeleter>&& other ) : UnderlyingContainer( eastl::forward<eastl::unique_ptr<AlternateObject, AlternateDeleter>>( other ) ) {}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint UniquePointer<T, Deleter>::UniquePointer( Pointer pointer, const DeleterType& deleter ) : UnderlyingContainer( pointer, deleter ) {}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint UniquePointer<T, Deleter>::UniquePointer( UniquePointer<T, Deleter>&& source ) : UnderlyingContainer( eastl::forward<UnderlyingContainer>( source ) ) {}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint UniquePointer<T, Deleter>::UniquePointer( std::nullptr_t ) : UnderlyingContainer( nullptr ) {}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint UniquePointer<T, Deleter>::UniquePointer() : UnderlyingContainer() {}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	template <typename AlternateObject, typename AlternateDeleter>
	UniquePointer<T, Deleter>& UniquePointer<T, Deleter>::operator=( UniquePointer<AlternateObject, AlternateDeleter>&& other ) {
		UnderlyingContainer::operator=( eastl::forward<typename UniquePointer<AlternateObject, AlternateDeleter>::UnderlyingContainer>( other ) );
		return *this;
	}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	UniquePointer<T, Deleter>& UniquePointer<T, Deleter>::operator=( UniquePointer<T, Deleter>&& other ) {
		UnderlyingContainer::operator=( eastl::forward<UnderlyingContainer>( other ) );
		return *this;
	}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint void UniquePointer<T, Deleter>::Reset( Pointer value ) {
		UnderlyingContainer::reset( value );
	}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint typename UniquePointer<T, Deleter>::Pointer UniquePointer<T, Deleter>::Release() {
		return UnderlyingContainer::release();
	}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint typename UniquePointer<T, Deleter>::Pointer UniquePointer<T, Deleter>::Get() const {
		return UnderlyingContainer::get();
	}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint typename const UniquePointer<T, Deleter>::DeleterType& UniquePointer<T, Deleter>::GetDeleter() const {
		return UnderlyingContainer::get_deleter();
	}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint typename UniquePointer<T, Deleter>::DeleterType& UniquePointer<T, Deleter>::GetDeleter() {
		return UnderlyingContainer::get_deleter();
	}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint UniquePointer<T[], Deleter>::UniquePointer( Pointer pointer, const DeleterType& deleter ) : UnderlyingContainer( pointer, deleter ) {}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint UniquePointer<T[], Deleter>::UniquePointer( Pointer pointer ) : UnderlyingContainer( pointer, DeleterType() ) {}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint UniquePointer<T[], Deleter>::UniquePointer( std::nullptr_t ) : UnderlyingContainer( nullptr ) {}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint UniquePointer<T[], Deleter>::UniquePointer( UniquePointer<T[], Deleter>&& source ) : UnderlyingContainer( eastl::forward<UnderlyingContainer>( source ) ) {}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint UniquePointer<T[], Deleter>::UniquePointer() : UnderlyingContainer() {}

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
	UniquePointer<T[], Deleter>& UniquePointer<T[], Deleter>::operator=( UniquePointer<T[], AlternateDeleter>&& other ) {
		UnderlyingContainer::operator=( eastl::forward<typename UniquePointer<T[], AlternateDeleter>::UnderlyingContainer>( other ) );
		return *this;
	}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	UniquePointer<T[], Deleter>& UniquePointer<T[], Deleter>::operator=( UniquePointer<T[], Deleter>&& other ) {
		UnderlyingContainer::operator=( eastl::forward<UnderlyingContainer>( other ) );
		return *this;
	}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint UniquePointer<T[], Deleter>::operator bool() const {
		return static_cast<bool>( UnderlyingContainer::operator bool() );
	}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint void UniquePointer<T[], Deleter>::Reset( Pointer value, size_t sizeInElements ) {
		UnderlyingContainer::reset( value );
		UnderlyingContainer::get_deleter().SetSize( sizeInElements );
	}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint void UniquePointer<T[], Deleter>::Reset() {
		UnderlyingContainer::reset();
	}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint typename UniquePointer<T[], Deleter>::Pointer UniquePointer<T[], Deleter>::Release() {
		return UnderlyingContainer::release();
	}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint typename UniquePointer<T[], Deleter>::Pointer UniquePointer<T[], Deleter>::Get() const {
		return UnderlyingContainer::get();
	}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint typename const UniquePointer<T[], Deleter>::DeleterType& UniquePointer<T[], Deleter>::GetDeleter() const {
		return UnderlyingContainer::get_deleter();
	}

// ---------------------------------------------------

	template <typename T, typename Deleter>
	ETInlineHint typename UniquePointer<T[], Deleter>::DeleterType& UniquePointer<T[], Deleter>::GetDeleter() {
		return UnderlyingContainer::get_deleter();
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
	Eldritch2::UniquePointer<Object> MakeUnique( Eldritch2::Allocator& allocator, Args&&... args ) {
		return Eldritch2::UniquePointer<Object>( new( allocator, AllocationDuration::Normal ) Object( eastl::forward<Args>( args )... ), allocator );
	}

// ---------------------------------------------------

	template <typename Object, typename... Args>
	Eldritch2::UniquePointer<Object[]> MakeUniqueArray( Eldritch2::Allocator& allocator, size_t arraySizeInElements, Args&&... args ) {
		const auto	objectArray( static_cast<Object*>( allocator.Allocate( arraySizeInElements * sizeof(Object), AllocationDuration::Normal ) ) );
		if( !objectArray ) {
			return nullptr;
		}
		
	//	Intentionally not forwarding here, as we may otherwise try to move resources multiple times.
		eastl::for_each( objectArray, objectArray + arraySizeInElements, [&] ( Object& object ) { new( eastl::addressof( object ) ) Object( args... ); } );

		return { objectArray, { arraySizeInElements, allocator } };
	}

}	// namespace Eldritch2

