/*==================================================================*\
  ServiceBlackboard.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Platform/UserReadWriteMutex.hpp>
#include <Utility/Containers/HashMap.hpp>
#include <Utility/TypeHandle.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Core {

	class ServiceBlackboard {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref ServiceBlackboard instance.
		ServiceBlackboard( const ServiceBlackboard& topLevelBlackboard, Eldritch2::Allocator& allocator );
	//!	Constructs this @ref ServiceBlackboard instance.
		ServiceBlackboard( Eldritch2::Allocator& allocator );

		~ServiceBlackboard() = default;

	// ---------------------------------------------------

	public:
		template <typename Service>
		ServiceBlackboard&	Publish( Service& service );
		ServiceBlackboard&	Publish( Eldritch2::TypeHandle serviceType, void* service );

		template <typename Service>
		Service&			Locate() const;
		void*				Locate( Eldritch2::TypeHandle serviceType ) const;

	// ---------------------------------------------------

	public:
		ServiceBlackboard&	operator=( const ServiceBlackboard& );
		ServiceBlackboard&	operator=( ServiceBlackboard&& );
		
	// - DATA MEMBERS ------------------------------------

	private:
		mutable Platform::UserReadWriteMutex				_mutex;
		Eldritch2::HashMap<Eldritch2::TypeHandle, void*>	_services;
	};

}	// namespace Core
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Core/ServiceBlackboard.inl>
//------------------------------------------------------------------//

