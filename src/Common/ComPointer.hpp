/*==================================================================*\
  ComPointer.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Mpl/Compiler.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <class Interface>
	class ComPointer {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref ComPointer instance.
		template <class CompatibleInterface>
		ComPointer( const ComPointer<CompatibleInterface>& handle );
	//!	Constructs this @ref ComPointer instance.
		template <class CompatibleInterface>
		ComPointer( ComPointer<CompatibleInterface>&& handle );
	//!	Constructs this @ref ComPointer instance.
		template <class CompatibleInterface>
		ComPointer( CompatibleInterface* pointer );
	//!	Constructs this @ref ComPointer instance.
		ComPointer( const ComPointer& );
	//!	Constructs this @ref ComPointer instance.
		ComPointer( decltype(nullptr) );
	//!	Constructs this @ref ComPointer instance.
		ComPointer();

	//!	Destroys this @ref ComPointer instance.
		~ComPointer();

	// ---------------------------------------------------

	public:
		ETNeverThrowsHint Interface*	Release() const throw();

		ETNeverThrowsHint Interface*	Get() const throw();

		ETNeverThrowsHint void			Reset() throw();

		ETNeverThrowsHint void			Acquire( Interface* pointer ) throw();

	// ---------------------------------------------------

	public:
		template <class CompatibleInterface>
		ETNeverThrowsHint ComPointer&	operator=( const ComPointer<CompatibleInterface>& );
		template <class CompatibleInterface>
		ETNeverThrowsHint ComPointer&	operator=( ComPointer<CompatibleInterface>&& );
		template <class CompatibleInterface>
		ETNeverThrowsHint ComPointer&	operator=( CompatibleInterface* );
		ETNeverThrowsHint ComPointer&	operator=( const ComPointer& );

	// ---------------------------------------------------

	public:
		Interface**			GetInterfacePointer();

		Interface*			operator->() const;

		explicit operator	bool() const;

	// - DATA MEMBERS ------------------------------------

	private:
		Interface*	_pointer;

		friend void	Swap( ComPointer&, ComPointer& );
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/ComPointer.inl>
//------------------------------------------------------------------//