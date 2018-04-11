/*==================================================================*\
  ApiBuilder.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/Context.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {
namespace Wren {
namespace Detail {

	template <typename Property>
	struct PropertySignatureHelper {
		using GetterSignature = typename Property (*)();
		using SetterSignature = typename void (*)( Property );
	};

	template <size_t arity>
	struct ParameterList {
		static_assert( arity == 0, "Too many arguments for Wren function! Reduce the number of arguments or update the API registrar to handle functions with greater arity." );
	};

// ---

	template <>
	struct ParameterList<0u>	{ static Utf8Literal Get() { return "()"; } };
	template <>
	struct ParameterList<1u>	{ static Utf8Literal Get() { return "(_)"; } };
	template <>
	struct ParameterList<2u>	{ static Utf8Literal Get() { return "(_,_)"; } };
	template <>
	struct ParameterList<3u>	{ static Utf8Literal Get() { return "(_,_,_)"; } };
	template <>
	struct ParameterList<4u>	{ static Utf8Literal Get() { return "(_,_,_,_)"; } };
	template <>
	struct ParameterList<5u>	{ static Utf8Literal Get() { return "(_,_,_,_,_)"; } };
	template <>
	struct ParameterList<6u>	{ static Utf8Literal Get() { return "(_,_,_,_,_,_)"; } };
	template <>
	struct ParameterList<7u>	{ static Utf8Literal Get() { return "(_,_,_,_,_,_,_)"; } };
	template <>
	struct ParameterList<8u>	{ static Utf8Literal Get() { return "(_,_,_,_,_,_,_,_)"; } };
	template <>
	struct ParameterList<9u>	{ static Utf8Literal Get() { return "(_,_,_,_,_,_,_,_,_)"; } };
	template <>
	struct ParameterList<10u>	{ static Utf8Literal Get() { return "(_,_,_,_,_,_,_,_,_,_)"; } };

// ---

	ETPureFunctionHint size_t	HashForeignMethod( const char* const module, const char* const className, bool isStatic, const char* const signature, size_t seed = 0u );
	ETPureFunctionHint size_t	HashForeignClass( const char* const module, const char* const className, size_t seed = 0u );
	void						BindFinalizer( WrenVM* vm, WrenHandle* target, void (*finalizer)( void* ) );
	void						Bind( WrenVM* vm, WrenHandle* target, const ForeignMethod& method, bool isStatic );

}	// namespace Detail

	ETInlineHint ForeignMethod::ForeignMethod( decltype(nullptr) ) : body( nullptr ) {
		signature[0] = '\0';
	}

// ---------------------------------------------------

	template <typename Signature>
	ETInlineHint ETPureFunctionHint ForeignMethod DefineConstructor( Utf8Literal name, ForeignMethod::Body body ) {
		enum : size_t { Arity = FunctionTraits<Signature>::Arity };

		return ForeignMethod( name, Detail::ParameterList<Arity>::Get(), body );
	}

// ---------------------------------------------------

	template <typename Signature>
	ETInlineHint ETPureFunctionHint ForeignMethod DefineMethod( Utf8Literal name, ForeignMethod::Body body ) {
		enum : size_t { Arity = FunctionTraits<Signature>::Arity };

		return ForeignMethod( name, Detail::ParameterList<Arity>::Get(), body );
	}

// ---------------------------------------------------

	template <typename Property>
	ETInlineHint ETPureFunctionHint Pair<ForeignMethod, ForeignMethod> DefineGetter( Utf8Literal name, ForeignMethod::Body body ) {
		using GetterSignature = typename Detail::PropertySignatureHelper<Property>::GetterSignature;

		return {
			ForeignMethod( name, "", body ),
			ForeignMethod( nullptr )
		};
	}

// ---------------------------------------------------

	template <typename Property>
	ETInlineHint ETPureFunctionHint Pair<ForeignMethod, ForeignMethod> DefineSetter( Utf8Literal name, ForeignMethod::Body body ) {
		using SetterSignature = typename Detail::PropertySignatureHelper<Property>::SetterSignature;

		return { ForeignMethod( nullptr ), ForeignMethod( name, "=(_)", body ) };
	}

// ---------------------------------------------------

	template <typename Property>
	ETInlineHint ETPureFunctionHint Pair<ForeignMethod, ForeignMethod> DefineProperty( Utf8Literal name, ForeignMethod::Body getterBody, ForeignMethod::Body setterBody ) {
		using GetterSignature = typename Detail::PropertySignatureHelper<Property>::GetterSignature;
		using SetterSignature = typename Detail::PropertySignatureHelper<Property>::SetterSignature;

		return { ForeignMethod( name, "", getterBody ), ForeignMethod( name, "=(_)", setterBody ) };
	}

// ---------------------------------------------------

	template <class Class>
	ETInlineHint void ApiBuilder::CreateClass(
		const char* module,
		const char* name,
		std::initializer_list<ForeignMethod> constructors,
		std::initializer_list<Pair<ForeignMethod, ForeignMethod>> properties,
		std::initializer_list<ForeignMethod> methods,
		std::initializer_list<ForeignMethod> staticMethods,
		std::initializer_list<ForeignMethod> operators
	) {
	//	Only define types once.
		if (_context->FindForeignClass<Class>() != nullptr) { return; }

		WrenHandle* const	newClass( _context->CreateForeignClass( module, name, typeid(Class) ) );
		WrenVM* const		vm( _context->GetVm() );

		Detail::BindFinalizer( vm, newClass, [] ( void* object ) {
		//	MSVC bug workaround.
			ETUnreferencedParameter( object );
			static_cast<Class*>(object)->~Class();
		} );

		for (const ForeignMethod& constructor : constructors) {
			Detail::Bind( vm, newClass, constructor, true );
		}

		for (const ForeignMethod& method : methods) {
			Detail::Bind( vm, newClass, method, false );
		}

		for (const ForeignMethod& method : staticMethods) {
			Detail::Bind( vm, newClass, method, true );
		}

		for (const ForeignMethod& op : operators) {
			Detail::Bind( vm, newClass, op, false );
		}

		for (const Pair<ForeignMethod, ForeignMethod>& property : properties) {
			if (property.first.body) {
				Detail::Bind( vm, newClass, property.first, false );
			}

			if (property.second.body) {
				Detail::Bind( vm, newClass, property.second, false );
			}
		}
	}

// ---------------------------------------------------

	template <class Class, typename... Arguments>
	ETInlineHint Class* ApiBuilder::CreateVariable( const char* module, const char* name, Arguments&&... arguments ) {
		WrenHandle* const	classHandle( _context->FindForeignClass<Class>() );

		ET_ASSERT( classHandle != nullptr, "Class has not been registered with the API builder!" );

		return new( _context->CreateVariable( module, name, classHandle, sizeof(Class)) ) Class( eastl::forward<Arguments>( arguments )... );
	}

// ---------------------------------------------------

	ETInlineHint void ApiBuilder::CreateVariable( const char* module, const char* name, double value ) {
		_context->CreateVariable( module, name, value );
	}

}	// namespace Wren
}	// namespace Scripting
}	// namespace Eldritch2