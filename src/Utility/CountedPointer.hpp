/*==================================================================*\
  CountedPointer.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Mpl/Compiler.hpp>
//------------------------------------------------------------------//
#if ET_COMPILER_IS_MSVC && !defined( EA_COMPILER_HAS_C99_FORMAT_MACROS )
//	MSVC complains about *lots* of macro redefinitions in eabase/inttypes.h.
#	define EA_COMPILER_HAS_C99_FORMAT_MACROS
#endif
#include <EASTL/bonus/compressed_pair.h>
#include <eastl/type_traits.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

	struct NoopDisposer {
		template <typename Object>
		ETPureFunctionHint void	operator()( Object& object ) const;
	};

// ---

	template <class Object, typename Disposer>
	class CountedPointer {
	// - TYPE PUBLISHING ---------------------------------

	private:
		using StorageType	= typename eastl::remove_const<Object>::type;
		using DisposerType	= Disposer;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref CountedPointer instance.
		template <typename CompatibleObject, class = typename eastl::enable_if<eastl::is_constructible<Object*, CompatibleObject>::value>::type>
		CountedPointer( CompatibleObject pointer, const DisposerType& disposer = DisposerType() );
	//!	Constructs this @ref CountedPointer instance.
		template <typename CompatibleObject, typename CompatibleDisposer, class = typename eastl::enable_if<eastl::is_constructible<Object*, CompatibleObject*>::value &&
																											eastl::is_constructible<Disposer, eastl::add_reference<CompatibleDisposer>::type>::value>::type>
		CountedPointer( const CountedPointer<CompatibleObject, CompatibleDisposer>& handle );
	//!	Constructs this @ref CountedPointer instance.
		CountedPointer( const CountedPointer& handle );
	//!	Constructs this @ref CountedPointer instance.
		template <typename CompatibleObject, typename CompatibleDisposer, class = typename eastl::enable_if<eastl::is_constructible<Object*, CompatibleObject*>::value &&
																											eastl::is_constructible<Disposer, eastl::add_rvalue_reference<CompatibleDisposer>::type>::value>::type>
		CountedPointer( CountedPointer<CompatibleObject, CompatibleDisposer>&& handle );
	//!	Constructs this @ref CountedPointer instance.
		CountedPointer( CountedPointer&& handle );
	//!	Constructs this @ref CountedPointer instance.
		CountedPointer( decltype( nullptr ) = nullptr );

	//!	Destroys this @ref CountedPointer instance.
		~CountedPointer();

	// ---------------------------------------------------

	public:
		bool	IsSoleReferenceToObject() const;

	// ---------------------------------------------------

	public:
		Object*	Release();

		Object*	Get() const;

		template <typename CompatibleObject, class = typename eastl::enable_if<eastl::is_constructible<StorageType*, CompatibleObject*>::value>::type>
		void	Reset( CompatibleObject* pointer );

	// ---------------------------------------------------

	public:
		const DisposerType&	GetDisposer() const;

	// ---------------------------------------------------

	public:
		template <typename CompatibleObject, typename CompatibleDisposer, class = typename eastl::enable_if<eastl::is_assignable<Object*, CompatibleObject*>::value &&
																											eastl::is_assignable<DisposerType, typename eastl::add_reference<CompatibleDisposer>::type>::value>::type>
		CountedPointer&	operator=( const CountedPointer<CompatibleObject, CompatibleDisposer>& handle );
		CountedPointer&	operator=( const CountedPointer& handle );
		template <typename CompatibleObject, typename CompatibleDisposer, class = typename eastl::enable_if<eastl::is_assignable<Object*, CompatibleObject*>::value &&
																											eastl::is_assignable<DisposerType, typename eastl::add_rvalue_reference<CompatibleDisposer>::type>::value>::type>
		CountedPointer&	operator=( CountedPointer<CompatibleObject, CompatibleDisposer>&& handle );
		CountedPointer&	operator=( CountedPointer&& handle );
		template <typename CompatibleObject, class = typename eastl::enable_if<eastl::is_assignable<Object*, CompatibleObject*>::value>::type>
		CountedPointer&	operator=( CompatibleObject* object );

	// ---------------------------------------------------

	public:
		explicit operator	bool() const EA_NOEXCEPT;

		Object*				operator->() const EA_NOEXCEPT;

		Object&				operator*() const EA_NOEXCEPT;

	// - DATA MEMBERS ------------------------------------

	private:
		eastl::compressed_pair<StorageType*, DisposerType>	_pointerAndDisposer;

	// ---------------------------------------------------

		template <class Object2, typename Disposer2>
		friend class Eldritch2::CountedPointer;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/CountedPointer.inl>
//------------------------------------------------------------------//