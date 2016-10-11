/*==================================================================*\
  DeclarationFormatter.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {

	template <typename StringAllocator>
	void DeclarationFormatter::RenderDeclaration( Eldritch2::Utf8String<StringAllocator>& string, const Scripting::Function& descriptor ) {
		RenderDeclaration( string, descriptor.returnDescriptor );
		string.Append( ' ' );
		string.Append( descriptor.name );

		string.Append( '(' );

		for( auto argument( descriptor.argumentDescriptors ); argument->type != TypeDescriptor(); ++argument ) {
			if( argument != descriptor.argumentDescriptors ) {
				string.Append( ", " );
			}

			RenderDeclaration( string, *argument );
		}

		string.Append( ')' );
	}

// ---------------------------------------------------

	template <typename StringAllocator>
	void DeclarationFormatter::RenderDeclaration( Eldritch2::Utf8String<StringAllocator>& string, const Scripting::Method& descriptor ) {
		RenderDeclaration( string, static_cast<const Scripting::Function&>( descriptor ) );

		if( descriptor.isConst ) {
			string.Append( " const" );
		}
	}

// ---------------------------------------------------

	template <typename StringAllocator>
	void DeclarationFormatter::RenderDeclaration( Eldritch2::Utf8String<StringAllocator>& string, const Scripting::Callable& descriptor ) {
		RenderDeclaration( string, descriptor.returnDescriptor );

		string.Append( ' ' );
		string.Append( 'f' );

		string.Append( '(' );

		for( auto argument( descriptor.argumentDescriptors ); argument->type != TypeDescriptor(); ++argument ) {
			if( argument != descriptor.argumentDescriptors ) {
				string.Append( ", " );
			}

			RenderDeclaration( string, *argument );
		}

		string.Append( ')' );
	}

// ---------------------------------------------------

	template <typename StringAllocator>
	void DeclarationFormatter::RenderDeclaration( Eldritch2::Utf8String<StringAllocator>& string, const Scripting::Argument& descriptor ) {
		if( descriptor.type.isConst ) {
			string.Append( "const " );
		}

		string.Append( GetName( descriptor.type.handle ) );

		if( descriptor.type.isPointer ) {
			string.Append( '@' );
		}

		if( descriptor.type.isReference ) {
			string.Append( descriptor.type.isConst ? "&in" : "&out" );
		}

		string.Append( ' ' ).Append( descriptor.name );
	}

// ---------------------------------------------------

	template <typename StringAllocator>
	void DeclarationFormatter::RenderDeclaration( Eldritch2::Utf8String<StringAllocator>& string, const Scripting::TypeDescriptor& descriptor ) {
		if( descriptor.isConst ) {
			string.Append( "const " );
		}

		string.Append( GetName( descriptor.handle ) );

		if( descriptor.isPointer ) {
			string.Append( '@' );
		}
		
		if( descriptor.isReference ) {
			string.Append( '&' );
		}
	}

// ---------------------------------------------------

	template <typename StringAllocator>
	void DeclarationFormatter::RenderDeclaration( Eldritch2::Utf8String<StringAllocator>& string, const Scripting::Field& descriptor ) {
		RenderDeclaration( string, descriptor.type );
		string.Append( ' ' ).Append( descriptor.name );
	}

// ---------------------------------------------------

	template <typename T>
	void DeclarationFormatter::OverrideName( Eldritch2::Utf8Literal name ) {
		this->OverrideName( typeid(T), name );
	}

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2

