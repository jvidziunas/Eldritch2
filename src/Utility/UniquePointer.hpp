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
#include <Utility/Containers/RangeAdapters.hpp>
#include <Utility/Memory/InstanceDeleter.hpp>
#include <Utility/Mpl/IntTypes.hpp>
#include <Utility/Mpl/Compiler.hpp>
//------------------------------------------------------------------//
#if ET_COMPILER_IS_MSVC && !defined( EA_COMPILER_HAS_C99_FORMAT_MACROS )
//	MSVC complains about *lots* of macro redefinitions in eabase/inttypes.h.
#	define EA_COMPILER_HAS_C99_FORMAT_MACROS
#endif
#include <eastl/type_traits.h>
#include <eastl/unique_ptr.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename T, typename Deleter = Eldritch2::InstanceDeleter<T>>
	class UniquePointer : private eastl::unique_ptr<typename eastl::remove_const<T>::type, Deleter> {
	// - TYPE PUBLISHING ---------------------------------

	private:
		using UnderlyingContainer	= eastl::unique_ptr<typename eastl::remove_const<T>::type, Deleter>;

	public:
		using DeleterType			= typename UnderlyingContainer::deleter_type;
		using ElementType			= typename UnderlyingContainer::element_type;
		using Pointer				= typename UnderlyingContainer::pointer;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	//	static_assert( !eastl::is_polymorphic<T>::value || eastl::has_virtual_destructor<T>::value, "Polymorphic types managed by InstanceDeleters must have virtual destructors!" );

	public:
		template <typename AlternateObject, typename AlternateDeleter>
		UniquePointer( UniquePointer<AlternateObject, AlternateDeleter>&& ) EA_NOEXCEPT;
	//!	Constructs this @ref UniquePointer instance.
		UniquePointer( Pointer pointer, const DeleterType& deleter = DeleterType() ) EA_NOEXCEPT;
	//!	Constructs this @ref UniquePointer instance.
		UniquePointer( std::nullptr_t ) EA_NOEXCEPT;
	//!	Constructs this @ref UniquePointer instance.
		UniquePointer( UniquePointer&& ) EA_NOEXCEPT;
	//!	Constructs this @ref UniquePointer instance.
		explicit UniquePointer( const UniquePointer& ) = delete;
	//!	Constructs this @ref UniquePointer instance.
		UniquePointer() EA_NOEXCEPT;

		~UniquePointer() = default;

	// ---------------------------------------------------

	public:
		template <typename AlternateObject, typename AlternateDeleter>
		UniquePointer&	operator=( UniquePointer<AlternateObject, AlternateDeleter>&& );
		UniquePointer&	operator=( const UniquePointer& ) = delete;
		UniquePointer&	operator=( UniquePointer&& );

	// ---------------------------------------------------

	public:
		using UnderlyingContainer::operator *;
		using UnderlyingContainer::operator ->;
		using UnderlyingContainer::operator bool;

	// ---------------------------------------------------

	public:
		void	Reset( Pointer value = Pointer() ) EA_NOEXCEPT;

	// ---------------------------------------------------

	public:
		Pointer	Release() EA_NOEXCEPT;

		Pointer	Get() const EA_NOEXCEPT;

	// ---------------------------------------------------

	public:
		const DeleterType&	GetDeleter() const EA_NOEXCEPT;
		DeleterType&		GetDeleter() EA_NOEXCEPT;

	// ---------------------------------------------------

		template <typename AlternateObject, typename AlternateDeleter>
		friend class Eldritch2::UniquePointer;
	};

// ---

	template <typename T, typename Deleter>
	class UniquePointer<T[], Deleter> : private eastl::unique_ptr<T[], Deleter> {
	// - TYPE PUBLISHING ---------------------------------

	private:
		using UnderlyingContainer	= eastl::unique_ptr<T[], Deleter>;

	public:
		using DeleterType			= typename UnderlyingContainer::deleter_type;
		using ElementType			= typename UnderlyingContainer::element_type;
		using Pointer				= typename UnderlyingContainer::pointer;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref UniquePointer instance.
		UniquePointer( Pointer pointer, const DeleterType& deleter ) EA_NOEXCEPT;
	//!	Constructs this @ref UniquePointer instance.
		UniquePointer( Pointer pointer ) EA_NOEXCEPT;
	//!	Constructs this @ref UniquePointer instance.
		UniquePointer( std::nullptr_t ) EA_NOEXCEPT;
	//!	Constructs this @ref UniquePointer instance.
		UniquePointer( UniquePointer&& ) EA_NOEXCEPT;
	//!	Constructs this @ref UniquePointer instance.
		explicit UniquePointer( const UniquePointer& ) = delete;
	//!	Constructs this @ref UniquePointer instance.
		UniquePointer();

		~UniquePointer() = default;

	// ---------------------------------------------------

	public:
		Pointer	Begin() const;

		Pointer	End() const;

	// ---------------------------------------------------

	public:
		size_t	GetSize() const;

	// ---------------------------------------------------

	public:
		template <typename AlternateDeleter>
		UniquePointer&	operator=( UniquePointer<T[], AlternateDeleter>&& );
		UniquePointer&	operator=( const UniquePointer& ) = delete;
		UniquePointer&	operator=( UniquePointer&& );

	// ---------------------------------------------------

	public:
		using UnderlyingContainer::operator [];

		explicit	operator bool() const;

	// ---------------------------------------------------

	public:
		void	Reset( Pointer pointer, size_t arraySizeInElements ) EA_NOEXCEPT;
		void	Reset() EA_NOEXCEPT;

	// ---------------------------------------------------

	public:
		Pointer	Release() EA_NOEXCEPT;

		Pointer	Get() const EA_NOEXCEPT;

	// ---------------------------------------------------

	public:
		const DeleterType&	GetDeleter() const EA_NOEXCEPT;
		DeleterType&		GetDeleter() EA_NOEXCEPT;
	};

// ---------------------------------------------------

	template <typename Object, typename... Args>
	Eldritch2::UniquePointer<Object>	MakeUnique( Eldritch2::Allocator& allocator, Args&&... args );

	template <typename Object, typename... Args>
	Eldritch2::UniquePointer<Object[]>	MakeUniqueArray( Eldritch2::Allocator& allocator, size_t arraySizeInElements, Args&&... args );

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/UniquePointer.inl>
//------------------------------------------------------------------//