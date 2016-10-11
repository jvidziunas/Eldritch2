/*==================================================================*\
  TypeReflection.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

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
#include <Utility/StringLiteral.hpp>
#include <Utility/MPL/IntTypes.hpp>
#include <Utility/TypeHandle.hpp>
//------------------------------------------------------------------//
#include <eastl/type_traits.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scripting {
		class	VirtualMachineContext;
	}
}

namespace Eldritch2 {
namespace Scripting {
namespace Detail {

	template <typename Function>
	struct FlattenInjected {
	public:
		using Type = Function;
	};

}	// namespace Detail

	enum class OperatorType {
		UnaryOperatorBegin,
		PrefixIncrement		= UnaryOperatorBegin,
		PostfixIncrement,
		PrefixDecrement,
		PostfixDecrement,
		Negate,
		Complement,

		UnaryOperatorCount,
		BinaryOperatorBegin	= UnaryOperatorCount,
		Addition			= BinaryOperatorBegin,
		Subtraction,
		Multiplication,
		Division,

		Assignment,

		AdditionAssignment,
		SubtractionAssignment,
		MultiplicationAssignment,
		DivisionAssignment,

		BinaryOperatorEnd,
		Count				= BinaryOperatorEnd
	};

// ---------------------------------------------------

//	MSVC is weird about aliases for function types; please excuse the hack.
	using NativeFunction = eastl::remove_pointer<void (*)( VirtualMachineContext& )>::type;

// ---------------------------------------------------

	template <typename... Arguments>
	struct Injected {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copying.
		Injected( const Injected& ) = delete;
	//!	Disable default construction.
		Injected() = delete;

		~Injected() = delete;
	};

// ---------------------------------------------------

	class TypeDescriptor {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref TypeDescriptor instance.
		TypeDescriptor( Eldritch2::TypeHandle handle, bool isConst, bool isPointer, bool isReference );
	//!	Constructs this @ref TypeDescriptor instance.
		TypeDescriptor( const TypeDescriptor& ) = default;
	//!	Constructs this @ref TypeDescriptor instance.
		TypeDescriptor() = default;

		~TypeDescriptor() = default;

	// - DATA MEMBERS ------------------------------------

	public:
		Eldritch2::TypeHandle	handle;

		bool					isConst		: 1;
		bool					isPointer	: 1;
		bool					isReference : 1;
	};

// ---------------------------------------------------

	class Argument {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Argument instance.
		Argument( Scripting::TypeDescriptor type, Eldritch2::Utf8Literal name );
	//!	Constructs this @ref Argument instance.
		Argument( Scripting::TypeDescriptor type );
	//!	Constructs this @ref Argument instance.
		Argument( const Argument& ) = default;

		~Argument() = default;

	// - DATA MEMBERS ------------------------------------

	public:
		Scripting::TypeDescriptor	type;
		Eldritch2::Utf8Literal		name;
	};

// ---------------------------------------------------

	class Callable {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Callable instance.
		Callable( Scripting::TypeDescriptor	returnDescriptor, const Scripting::Argument* argumentDescriptors, Scripting::NativeFunction* nativeFunction );
	//!	Constructs this @ref Callable instance.
		Callable( const Callable& ) = default;

	// - DATA MEMBERS ------------------------------------

	public:
		Scripting::TypeDescriptor	returnDescriptor;
		const Scripting::Argument*	argumentDescriptors;
		Scripting::NativeFunction*	nativeFunction;
	};

// ---------------------------------------------------

	class Constructor : public Scripting::Callable {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Constructor instance.
		Constructor( const Scripting::Argument* argumentDescriptors, Scripting::NativeFunction* nativeFunction );
	//!	Constructs this @ref Constructor instance.
		Constructor( const Constructor& ) = default;

		~Constructor() = default;
	};

// ---------------------------------------------------

	class Factory : public Scripting::Callable {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Factory instance.
		Factory( Scripting::TypeDescriptor returnDescriptor, const Scripting::Argument* argumentDescriptors, Scripting::NativeFunction* nativeFunction );
	//!	Constructs this @ref Factory instance.
		Factory( const Factory& ) = default;

		~Factory() = default;
	};

// ---------------------------------------------------

	class Function : public Scripting::Callable {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Function instance.
		Function( Scripting::TypeDescriptor returnDescriptor, Eldritch2::Utf8Literal name, const Scripting::Argument* argumentDescriptors, Scripting::NativeFunction* nativeFunction );
	//!	Constructs this @ref Function instance.
		Function( const Function& ) = default;

		~Function() = default;

	// - DATA MEMBERS ------------------------------------

	public:
		Eldritch2::Utf8Literal		name;
	};

// ---------------------------------------------------

	class Method : public Scripting::Function {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Method instance.
		Method( Scripting::TypeDescriptor returnDescriptor, Eldritch2::Utf8Literal name, const Scripting::Argument* argumentDescriptors, bool isConst, Scripting::NativeFunction* nativeFunction );
	//!	Constructs this @ref Method instance.
		Method( const Method& ) = default;

		~Method() = default;

	// - DATA MEMBERS ------------------------------------

	public:
		bool	isConst : 1;
	};

// ---------------------------------------------------

	class Property {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Property instance.
		Property( Scripting::TypeDescriptor type, Eldritch2::Utf8Literal name, NativeFunction* getterFunction, NativeFunction* setterFunction );
	//!	Constructs this @ref Property instance.
		Property( const Property& ) = default;

	// - DATA MEMBERS ------------------------------------

	public:
		Scripting::TypeDescriptor	type;

		Eldritch2::Utf8Literal		name;

		NativeFunction*				getterFunction;
		NativeFunction*				setterFunction;
	};

// ---------------------------------------------------

	class Operator : public Scripting::Callable {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Operator instance.
		Operator( Scripting::TypeDescriptor returnDescriptor, Scripting::OperatorType type, const Scripting::Argument* argumentDescriptors, bool isConst, Scripting::NativeFunction* nativeFunction );
	//!	Constructs this @ref Operator instance.
		Operator( const Operator& ) = default;

		~Operator() = default;

	// - DATA MEMBERS ------------------------------------

	public:
		Scripting::OperatorType		type;
		bool						isConst : 1;
	};

// ---------------------------------------------------

	class Field {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Field instance.
		Field( Scripting::TypeDescriptor type, Eldritch2::Utf8Literal name, Eldritch2::ptrdiff offset );
	//!	Constructs this @ref Field instance.
		Field( const Field& ) = default;

		~Field() = default;

	// - DATA MEMBERS ------------------------------------

	public:
		Scripting::TypeDescriptor	type;
		Eldritch2::Utf8Literal		name;
		Eldritch2::ptrdiff			offset;
	};

// ---------------------------------------------------

	class EnumValue {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref EnumValue instance.
		EnumValue( Eldritch2::Utf8Literal name, int value );
	//!	Constructs this @ref EnumValue instance.
		EnumValue( const EnumValue& ) = default;

		~EnumValue() = default;

	// - DATA MEMBERS ------------------------------------

	public:
		Eldritch2::Utf8Literal	name;
		int						value;
	};

// ---------------------------------------------------

#define DefineOverloadedFunction(name, overload, function)											Eldritch2::Scripting::DefineFunction_<overload, typename Eldritch2::Scripting::Detail::FlattenInjected<overload>::Type, &function>( Eldritch2::Utf8Literal( name ) )
#define DefineFunction(name, function)																DefineOverloadedFunction(name, decltype(&function), function)
#define DefineOverloadedConstructor(overload, function)												Eldritch2::Scripting::DefineConstructor_<overload, typename Eldritch2::Scripting::Detail::FlattenInjected<overload>::Type, &function>()
#define DefineConstructor(function)																	DefineOverloadedConstructor(decltype(&function), function)
#define DefineOverloadedFactory(overload, function)													Eldritch2::Scripting::DefineFactory_<overload, typename Eldritch2::Scripting::Detail::FlattenInjected<overload>::Type, &function>()
#define DefineFactory(function)																		DefineOverloadedFactory(decltype(&function), function)
#define DefineOverloadedMethod(name, overload, method)												Eldritch2::Scripting::DefineMethod_<overload, typename Eldritch2::Scripting::Detail::FlattenInjected<overload>::Type, &method>( Eldritch2::Utf8Literal( name ) )
#define DefineMethod(name, method)																	DefineOverloadedMethod(name, decltype(&method), method)
#define DefineOverloadedUnaryOperator(type, overload, method)										Eldritch2::Scripting::DefineUnaryOperator_<overload, typename Eldritch2::Scripting::Detail::FlattenInjected<overload>::Type, &method>( type )
#define DefineUnaryOperator(type, method)															DefineOverloadedUnaryOperator(type, decltype(&method), method)
#define DefineOverloadedBinaryOperator(type, overload, method)										Eldritch2::Scripting::DefineBinaryOperator_<overload, typename Eldritch2::Scripting::Detail::FlattenInjected<overload>::Type, &method>( type )
#define DefineBinaryOperator(type, method)															DefineOverloadedBinaryOperator(type, decltype(&method), method)
#define DefineOverloadedProperty(name, getterOverload, getterMethod, setterOverload, setterMethod)	Eldritch2::Scripting::DefineProperty_<getterOverload, typename Eldritch2::Scripting::Detail::FlattenInjected<getterOverload>::Type, &getterMethod, setterOverload, typename Eldritch2::Scripting::Detail::FlattenInjected<setterOverload>::Type, &setterMethod>( Eldritch2::Utf8Literal( name ) )
#define DefineProperty(name, getterMethod, setterMethod)											DefineOverloadedProperty(name, decltype(&getterMethod), getterMethod, decltype(&setterMethod), setterMethod)
#define DefineOverloadedGetter(name, overload, getterMethod)										Eldritch2::Scripting::DefineGetter_<overload, typename Eldritch2::Scripting::Detail::FlattenInjected<overload>::Type, &getterMethod>( Eldritch2::Utf8Literal( name ) )
#define DefineGetter(name, getterMethod)															DefineOverloadedGetter(name, decltype(&getterMethod), getterMethod)
#define DefineOverloadedSetter(name, overload, setterMethod)										Eldritch2::Scripting::DefineSetter_<overload, typename Eldritch2::Scripting::Detail::FlattenInjected<overload>::Type, &setterMethod>( Eldritch2::Utf8Literal( name ) )
#define DefineSetter(name, setterMethod)															DefineOverloadedSetter(name, decltype(&setterMethod), setterMethod)
#define DefineField(name, field)																	Eldritch2::Scripting::DefineField_<decltype(&field), &field>( Eldritch2::Utf8Literal( name ) )
#define DefineEnumValue(name, value)																Eldritch2::Scripting::DefineEnumValue_<decltype(value), value>( Eldritch2::Utf8Literal( name ) )

	template <typename Signature, typename FunctionPointer, FunctionPointer functionPointer>
	Scripting::Function		DefineFunction_( Eldritch2::Utf8Literal name );

	template <typename Signature, typename FunctionPointer, FunctionPointer functionPointer>
	Scripting::Constructor	DefineConstructor_();

	template <typename Signature, typename FunctionPointer, FunctionPointer functionPointer>
	Scripting::Factory		DefineFactory_();

	template <typename Signature, typename MethodPointer, MethodPointer methodPointer>
	Scripting::Method		DefineMethod_( Eldritch2::Utf8Literal name );

	template <typename Signature, typename MethodPointer, MethodPointer methodPointer>
	Scripting::Operator		DefineUnaryOperator_( Scripting::OperatorType type );

	template <typename Signature, typename MethodPointer, MethodPointer methodPointer>
	Scripting::Operator		DefineBinaryOperator_( Scripting::OperatorType type );

	template <typename GetterSignature, typename GetterPointer, GetterPointer getterPointer, typename SetterSignature, typename SetterPointer, SetterPointer setterPointer>
	Scripting::Property		DefineProperty_( Eldritch2::Utf8Literal name );

	template <typename Signature, typename GetterPointer, GetterPointer getterPointer>
	Scripting::Property		DefineGetter_( Eldritch2::Utf8Literal name );
	template <typename Signature, typename SetterPointer, SetterPointer setterPointer>
	Scripting::Property		DefineSetter_( Eldritch2::Utf8Literal name );

	template <typename Signature, typename FieldPointer, FieldPointer fieldPointer>
	Scripting::Field		DefineField_( Eldritch2::Utf8Literal name );

	template <typename Signature, typename Enum, Enum value>
	Scripting::EnumValue	DefineEnumValue_( Eldritch2::Utf8Literal name );

}	// namespace Scripting
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/ReflectionTypes.inl>
//------------------------------------------------------------------//
