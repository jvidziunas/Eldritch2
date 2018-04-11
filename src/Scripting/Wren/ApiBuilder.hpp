/*==================================================================*\
  ApiBuilder.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/Marshal.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scripting {
		namespace Wren {
			class	Context;
		}
	}
}

namespace Eldritch2 {
namespace Scripting {
namespace Wren {

	class ForeignMethod {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using Body = void (*)( WrenVM* );

	// ---

		enum : size_t { MaxSignatureLength = 64u };

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref ForeignMethod instance.
		ForeignMethod( Utf8Literal name, Utf8Literal argumentSpecifier, Body body );
	//!	Constructs this @ref ForeignMethod instance.
		ForeignMethod( const ForeignMethod& ) = default;
	//!	Constructs this @ref ForeignMethod instance.
		ForeignMethod( decltype(nullptr) );

		~ForeignMethod() = default;

	// - DATA MEMBERS ------------------------------------

	public:
		char	signature[MaxSignatureLength];
		Body	body;
	};

// ---

	class ApiBuilder {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref ApiBuilder instance.
		ApiBuilder( Context& wren );
	//!	Disable copy construction.
		ApiBuilder( const ApiBuilder& ) = delete;

		~ApiBuilder() = default;

	// ---------------------------------------------------

	public:
		template <class Class>
		void	CreateClass(
			const char* module,
			const char* name,
			std::initializer_list<ForeignMethod> constructors,
			std::initializer_list<Pair<ForeignMethod, ForeignMethod>> properties,
			std::initializer_list<ForeignMethod> methods,
			std::initializer_list<ForeignMethod> staticMethods,
			std::initializer_list<ForeignMethod> operators
		);

		template <class Class, typename... Arguments>
		Class*	CreateVariable( const char* module, const char* name, Arguments&&... arguments );
		void	CreateVariable( const char* module, const char* name, double value );

	// - DATA MEMBERS ------------------------------------

	private:
		Context*	_context;
	};

// ---------------------------------------------------

	template <typename Signature>
	ETPureFunctionHint ForeignMethod						DefineConstructor( Utf8Literal name, ForeignMethod::Body body );

	template <typename Signature>
	ETPureFunctionHint ForeignMethod						DefineMethod( Utf8Literal name, ForeignMethod::Body body );

	template <typename Property>
	ETPureFunctionHint Pair<ForeignMethod, ForeignMethod>	DefineGetter( Utf8Literal name, ForeignMethod::Body body );

	template <typename Property>
	ETPureFunctionHint Pair<ForeignMethod, ForeignMethod>	DefineSetter( Utf8Literal name, ForeignMethod::Body body );

	template <typename Property>
	ETPureFunctionHint Pair<ForeignMethod, ForeignMethod>	DefineProperty( Utf8Literal name, ForeignMethod::Body getterBody, ForeignMethod::Body setterBody );

}	// namespace Wren
}	// namespace Scripting
}	// namespace Eldritch2

#define ET_BUILTIN_WREN_MODULE_NAME(name) "builtin/" #name

#define ET_ABORT_WREN_IF(condition, message) if((condition)) { wrenSetSlotString( vm, 0, message ); return wrenAbortFiber( vm, 0 ); }
#define ET_ABORT_WREN_UNLESS(condition, message) if(!(condition)) { wrenSetSlotString( vm, 0, message ); return wrenAbortFiber( vm, 0 ); }

#define ET_DECLARE_WREN_CLASS(className) void className##WrenDefinition( Scripting::Wren::ApiBuilder& api );
#define ET_IMPLEMENT_WREN_CLASS(className) void className##WrenDefinition( Scripting::Wren::ApiBuilder& api )
#define ET_REGISTER_WREN_CLASS(className, builder) className##WrenDefinition( builder )

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/Wren/ApiBuilder.inl>
//------------------------------------------------------------------//