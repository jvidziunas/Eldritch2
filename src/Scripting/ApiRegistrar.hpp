/*==================================================================*\
  ApiRegistrar.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/ReflectionTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {

	class ETPureAbstractHint ApiRegistrar {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref ApiRegistrar instance.
		ApiRegistrar() = default;
	//!	Disable copying.
		ApiRegistrar( const ApiRegistrar& ) = delete;

		~ApiRegistrar() = default;

	// ---------------------------------------------------

	public:
		template <typename Class>
		void	DefineUserAllocatedClass( std::initializer_list<Scripting::Factory>		factories,
										  std::initializer_list<Scripting::Property>	properties,
										  std::initializer_list<Scripting::Method>		methods,
										  std::initializer_list<Scripting::Operator>	operators,
										  std::initializer_list<Scripting::Field>		fields );

		template <typename Class>
		void	DefineClass( std::initializer_list<Scripting::Constructor>	constructors,
							 std::initializer_list<Scripting::Property>		properties,
							 std::initializer_list<Scripting::Method>		methods,
							 std::initializer_list<Scripting::Operator>		operators,
							 std::initializer_list<Scripting::Field>		fields );

		template <typename Enum>
		void	DefineEnum( std::initializer_list<Scripting::EnumValue> values );

	// ---------------------------------------------------

	protected:
		virtual void	DefineUserAllocatedClass( Eldritch2::TypeHandle							type,
												  std::initializer_list<Scripting::Factory>		factories,
												  std::initializer_list<Scripting::Property>	properties,
												  std::initializer_list<Scripting::Method>		methods,
												  std::initializer_list<Scripting::Operator>	operators,
												  std::initializer_list<Scripting::Field>		fields ) abstract;

		virtual void	DefineClass( Eldritch2::TypeHandle							type,
									 size_t											sizeInBytes,
									 std::initializer_list<Scripting::Constructor>	constructors,
									 std::initializer_list<Scripting::Property>		properties,
									 std::initializer_list<Scripting::Method>		methods,
									 std::initializer_list<Scripting::Operator>		operators,
									 std::initializer_list<Scripting::Field>		fields ) abstract;

		virtual void	DefineEnum( Eldritch2::TypeHandle type, std::initializer_list<Scripting::EnumValue> values ) abstract;
	};

}	// namespace Scripting
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/ApiRegistrar.inl>
//------------------------------------------------------------------//
