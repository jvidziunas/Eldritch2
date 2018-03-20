/*==================================================================*\
  Stack.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Containers/ArrayList.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Value, class Container = ArrayList<Value>>
	class Stack {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using ContainerType		= typename Container;
		using ValueType			= typename ContainerType::ValueType;
		using SizeType			= typename ContainerType::SizeType;
		using ConstReference	= typename ContainerType::ConstReference;
		using Reference			= typename ContainerType::Reference;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Stack instance.
		template <class AllocatorType>
		Stack( const Stack&, const AllocatorType& allocator );
	//!	Constructs this @ref Stack instance.
		template <class AllocatorType>
		Stack( Stack&&, const AllocatorType& allocator );
	//!	Constructs this @ref Stack instance.
		template <class AllocatorType>
		explicit Stack( const AllocatorType& allocator );
	//!	Constructs this @ref Stack instance.
		explicit Stack( const ContainerType& );
	//!	Constructs this @ref Stack instance.
		explicit Stack( ContainerType&& );
	//!	Constructs this @ref Stack instance.
		Stack() = default;

		~Stack() = default;

	// ---------------------------------------------------

	public:
		SizeType	GetSize() const;

		bool		IsEmpty() const;

	// ---------------------------------------------------

	public:
		ConstReference	Top() const;
		Reference       Top();

	// ---------------------------------------------------
		
	public:
		void	Push( const ValueType& value );
		void	Push( ValueType&& value );

		template <class... Args>
		void	EmplaceBack( Args&&... args );

		void	Pop();

	// ---------------------------------------------------

	public:
		const ContainerType&	GetContainer() const;
		ContainerType&			GetContainer();

	// - DATA MEMBERS ------------------------------------

	protected:
		ContainerType	c;

	// ---------------------------------------------------

		template <typename Value, class Container>
		friend void	Swap( Stack<Value, Container>&, Stack<Value, Container>& );
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Containers/Stack.inl>
//------------------------------------------------------------------//