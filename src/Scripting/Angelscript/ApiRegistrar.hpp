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
#include <Scripting/AngelScript/DeclarationFormatter.hpp>
#include <Scripting/ApiRegistrar.hpp>
//------------------------------------------------------------------//
#include <angelscript.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {

	class ApiRegistrar : public Scripting::ApiRegistrar {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref ApiRegistrar instance.
		ApiRegistrar( asIScriptEngine& scriptEngine, Eldritch2::Allocator& allocator );
	//!	Disable copying.
		ApiRegistrar( const ApiRegistrar& ) = delete;

		~ApiRegistrar() = default;

	// ---------------------------------------------------

	public:
		using Scripting::ApiRegistrar::DefineUserAllocatedClass;
		using Scripting::ApiRegistrar::DefineClass;
		using Scripting::ApiRegistrar::DefineEnum;

	// ---------------------------------------------------

	protected:
		void	DefineUserAllocatedClass( Eldritch2::TypeHandle							type,
										  std::initializer_list<Scripting::Factory>		factories,
										  std::initializer_list<Scripting::Property>	properties,
										  std::initializer_list<Scripting::Method>		methods,
										  std::initializer_list<Scripting::Operator>	operators,
										  std::initializer_list<Scripting::Field>		fields ) override;

		void	DefineClass( Eldritch2::TypeHandle							type,
							 size_t											sizeInBytes,
							 std::initializer_list<Scripting::Constructor>	constructors,
							 std::initializer_list<Scripting::Property>		properties,
							 std::initializer_list<Scripting::Method>		methods,
							 std::initializer_list<Scripting::Operator>		operators,
							 std::initializer_list<Scripting::Field>		fields ) override;

		void	DefineEnum( Eldritch2::TypeHandle type, std::initializer_list<Scripting::EnumValue> values ) override;

	// ---------------------------------------------------

	public:
		void	DefineCoreTypes();

	// ---------------------------------------------------

	public:
		void	RegisterFunction( const Scripting::Function& function );

		void	RegisterFactory( const asITypeInfo& type, const Scripting::Factory& factory );

		void	RegisterConstructor( const asITypeInfo& type, const Scripting::Constructor& constructor );

		void	RegisterProperty( const asITypeInfo& type, const Scripting::Property& property );

		void	RegisterMethod( const asITypeInfo& type, const Scripting::Method& method );

		void	RegisterOperator( const asITypeInfo& type, const Scripting::Operator& op );

		void	RegisterBehavior( const asITypeInfo& type, asEBehaviours behavior, const Scripting::Function& function );
		void	RegisterBehavior( const asITypeInfo& type, asEBehaviours behavior, const Scripting::Method& method );

		void	RegisterField( const asITypeInfo& type, const Scripting::Field& field );

	// ---------------------------------------------------

	public:
		const Eldritch2::Utf8Char*	GetTypeName( Eldritch2::TypeHandle type ) const;

	// - DATA MEMBERS ------------------------------------

	private:
		DeclarationFormatter	_declarationFormatter;

	/*!	While conceptually a glorified local variable, this was made a class member to avoid repeated heap allocations/deallocations. */
		Eldritch2::Utf8String<>	_declaration;

		asIScriptEngine&		_scriptEngine;
	};

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/Angelscript/ApiRegistrar.inl>
//------------------------------------------------------------------//