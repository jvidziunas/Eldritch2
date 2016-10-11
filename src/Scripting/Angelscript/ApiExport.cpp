/*==================================================================*\
  ApiExport.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/StdAllocatorAdapterMixin.hpp>
#include <Scripting/AngelScript/ApiExport.hpp>
#include <Platform/SynchronousFileWriter.hpp>
#include <Utility/Containers/Utf8String.hpp>
#include <Utility/Containers/HashSet.hpp>
#include <Platform/MemoryMappedFile.hpp>
#include <Utility/ErrorCode.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//
#define FMT_EXCEPTIONS 0
#include <cppformat/fmt/format.h>
#include <eastl/algorithm.h>
#include <angelscript.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Platform;

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {
namespace {

	class ExportContext {
	public:
		ExportContext( asIScriptEngine& engine, SynchronousFileWriter& writer, Allocator& allocator ) : _currentNamespace( { allocator, "Current Namespace String Allocator" } ),
																										_writer( writer ),
																										_formatter( _writeBuffer ),
																										_accessMask( 0u ),
																										_engine( engine ) {
			engine.SetDefaultNamespace( _currentNamespace );
		}

		~ExportContext() = default;

	// ---------------------------------------------------

		void SetCurrentNamespace( const char* newNamespace ) {
			if( newNamespace && (_currentNamespace != newNamespace) ) {
				_formatter << "namespace \"" << newNamespace << "\"\n";

				_currentNamespace = newNamespace;
				_engine.SetDefaultNamespace( _currentNamespace );

				FlushWriter();
			}
		}

	// ---------------------------------------------------

		void SetCurrentAccessMask( asDWORD accessMask ) {
			if( accessMask != _accessMask ) {
				_formatter << "access " << fmt::hex( (unsigned int)(accessMask) ) << "\n";
				_accessMask = accessMask;

				FlushWriter();
			}
		}

	// ---------------------------------------------------

		ETInlineHint void PushType( const asITypeInfo& type ) {
			SetCurrentNamespace( type.GetNamespace() );
			SetCurrentAccessMask( type.GetAccessMask() );
		}

	// ---------------------------------------------------

		ETInlineHint void PushFunction( const asIScriptFunction& function ) {
			SetCurrentNamespace( function.GetNamespace() );
			SetCurrentAccessMask( function.GetAccessMask() );
		}

	// ---------------------------------------------------

		ETInlineHint void FlushWriter() {
			_writer.Write( _formatter.data(), _formatter.size() );
			_formatter.clear();
		}

	// ---------------------------------------------------

		ETInlineHint fmt::ArrayWriter& GetFormatter() {
			return _formatter;
		}

	// ---------------------------------------------------

		void DeclareType( const asITypeInfo& type ) {
			PushType( type );

			if( type.GetFlags() & asEObjTypeFlags::asOBJ_SCRIPT_OBJECT ) {
			//	This should only be interfaces
				ETRuntimeAssert( type.GetSize() == 0 );

				GetFormatter() << "intf " << type.GetName() << "\n"; 
			} else {
			//	Only the type flags are necessary. The application flags are application
			//	specific and doesn't matter to the offline compiler. The object size is also
			//	unnecessary for the offline compiler
				GetFormatter() << "objtype \"" << type.GetName() << "\" 0x" << fmt::hex( (unsigned int)( type.GetFlags() & asEObjTypeFlags::asOBJ_MASK_VALID_FLAGS ) ) << "\n";
			}

			FlushWriter();
		}

	// ---------------------------------------------------

		void WriteEnum( const asITypeInfo& type ) {
			PushType( type );

			const char* const	enumName( type.GetName() );
			GetFormatter() << "enum " << enumName << "\n"; FlushWriter();

			for( asUINT valueIndex( 0u ); valueIndex < type.GetEnumValueCount(); valueIndex++ ) {
				int			enumValue;
				const char*	valueString( type.GetEnumValueByIndex( valueIndex, &enumValue ) );

				GetFormatter() << "enumval " << enumName << " " << valueString << " " << enumValue << "\n"; FlushWriter();
			}
		}

	// ---------------------------------------------------

		void WriteTypedef( const asITypeInfo& type ) {
			PushType( type );

			GetFormatter() << "typedef " << type.GetName() << " \"" << _engine.GetTypeDeclaration( type.GetTypedefTypeId() ) << "\"\n"; FlushWriter();
		}

	// ---------------------------------------------------

		void WriteFuncdef( const asITypeInfo& type ) {
			PushType( type );

			GetFormatter() << "funcdef \"" << type.GetFuncdefSignature()->GetDeclaration() << "\"\n"; FlushWriter();
		}

	// ---------------------------------------------------

		void WriteType( const asITypeInfo& type ) {
			Utf8String<>	declarationTempString( { _currentNamespace.GetAllocator().GetParent(), "Temporary Type Declaration String Allocator" } );

			PushType( type );

			if( type.GetFlags() & asOBJ_SCRIPT_OBJECT ) {
				for( asUINT methodIndex( 0 ); methodIndex < type.GetMethodCount(); methodIndex++ ) {
					const auto&	method( *type.GetMethodByIndex( methodIndex ) );

					SetCurrentAccessMask( method.GetAccessMask() );

					GetFormatter() << "intfmthd " << type.GetName() << " \"" << CopyDeclarationEscapingQuotes( declarationTempString, method.GetDeclaration( false ) ).AsCString() << "\"\n"; FlushWriter();
				}
			} else {
				for( asUINT factoryIndex( 0 ); factoryIndex < type.GetFactoryCount(); factoryIndex++ ) {
					const auto&	factory( *type.GetFactoryByIndex( factoryIndex ) );

					SetCurrentAccessMask( factory.GetAccessMask() );

					GetFormatter() << "objbeh \"" << type.GetName() << "\" " << asBEHAVE_FACTORY << " \"" << CopyDeclarationEscapingQuotes( declarationTempString, factory.GetDeclaration( false ) ).AsCString() << "\"\n"; FlushWriter();
				}

				for( asUINT behaviorIndex( 0 ); behaviorIndex < type.GetBehaviourCount(); behaviorIndex++ ) {
					asEBehaviours	behaviorClass;
					const auto&		behavior( *type.GetBehaviourByIndex( behaviorIndex, &behaviorClass ) );

					if( asEBehaviours::asBEHAVE_CONSTRUCT == behaviorClass ) {
					//	Prefix 'void'
						GetFormatter() << "objbeh \"" << type.GetName() << "\" " << behaviorClass << " \"void " << CopyDeclarationEscapingQuotes( declarationTempString, behavior.GetDeclaration( false ) ).AsCString() << "\"\n";
					} else if( asEBehaviours::asBEHAVE_DESTRUCT == behaviorClass ) {
					//	Prefix 'void' and remove ~
						GetFormatter() << "objbeh \"" << type.GetName() << "\" " << behaviorClass << " \"void " << CopyDeclarationEscapingQuotes( declarationTempString, behavior.GetDeclaration( false ) ).AsCString() + 1 << "\"\n";
					} else {
						GetFormatter() << "objbeh \"" << type.GetName() << "\" " << behaviorClass << " \"" << CopyDeclarationEscapingQuotes( declarationTempString, behavior.GetDeclaration( false ) ).AsCString() << "\"\n";
					}

					FlushWriter();
				}

				for( asUINT methodIndex( 0 ); methodIndex < type.GetMethodCount(); methodIndex++ ) {
					const auto&	method( *type.GetMethodByIndex( methodIndex ) );

					SetCurrentAccessMask( method.GetAccessMask() );

					GetFormatter() << "objmthd \"" << type.GetName() << "\" \"" << CopyDeclarationEscapingQuotes( declarationTempString, method.GetDeclaration( false ) ).AsCString() << "\"\n"; FlushWriter();
				}

				for( asUINT propertyIndex( 0 ); propertyIndex < type.GetPropertyCount(); propertyIndex++ ) {
					asDWORD	propertyAccessMask;

					type.GetProperty( propertyIndex, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, &propertyAccessMask );

					SetCurrentAccessMask( propertyAccessMask );

					GetFormatter() << "objprop \"" << type.GetName() << "\" \"" << type.GetPropertyDeclaration( propertyIndex ) << "\"\n"; FlushWriter();
				}
			}
		}

	// ---------------------------------------------------

		void WriteFunction( const asIScriptFunction& function ) {
			Utf8String<>	signature( { _currentNamespace.GetAllocator().GetParent(), "Function Signature Allocator" } );

			PushFunction( function );

			GetFormatter() << "func \"" << CopyDeclarationEscapingQuotes( signature, function.GetDeclaration() ).AsCString() << "\"\n"; FlushWriter();
		}

	// ---------------------------------------------------

		static ETPureFunctionHint Utf8String<>& CopyDeclarationEscapingQuotes( Utf8String<>& output, const char* decl ) {
			output.Clear();
			while( '\0' != *decl ) {
				if( '\"' == *decl ) {
					output.Append( '\\' );
				}

				output.Append( *decl++ );
			}

			return output;
		}

	// - DATA MEMBERS ------------------------------------

	private:
		asIScriptEngine&		_engine;
		SynchronousFileWriter&	_writer;
		fmt::ArrayWriter		_formatter;
		Utf8String<>			_currentNamespace;
		asDWORD					_accessMask;
		char					_writeBuffer[768u];
	};

// ---------------------------------------------------

	static ETPureFunctionHint void UnescapeQuotes( Utf8String<>& str ) {
		for( auto position( str.Begin() ); ; ) {
		//	Search for \" in the string
			position = str.FindFirstInstance( "\\\"", position );
			if( position == str.End() ) {
				break;
			}

		//	Remove the \ character
			str.Erase( position );
		}
	}

// ---------------------------------------------------

	static asETokenClass GetToken( asIScriptEngine& engine, Utf8String<>& token, Range<const char*>& source ) {
		const char*		readPointer( source.Begin() );
		const char*		readEnd( source.End() - 1 );
		asUINT			tokenLength( 0 );
		asETokenClass	tokenClass( engine.ParseToken( readPointer, source.GetSize(), &tokenLength ) );

		while( (tokenClass == asETokenClass::asTC_WHITESPACE || tokenClass == asETokenClass::asTC_COMMENT) && (readPointer != readEnd) ) {
			readPointer	+= tokenLength;
			tokenClass = engine.ParseToken( readPointer, readEnd - readPointer, &tokenLength );
		}

		token.Assign( readPointer, readPointer + tokenLength );

		source.SetBegin( readPointer + tokenLength );

		return tokenClass;
	}

// ---------------------------------------------------

	ETInlineHint static int GetLineNumber( const Utf8Char* const fileBegin, const Utf8Char* const currentPosition ) {
		return static_cast<int>( 1 + eastl::count( fileBegin, currentPosition, '\n' ) );
	}

}	// anonymous namespace

	ErrorCode ExportScriptApi( Allocator& allocator, asIScriptEngine& engine, SynchronousFileWriter& writer ) {
	//	Keep a list of the template types, as the methods for these need to be exported first.
		HashSet<const asITypeInfo*>	knownTemplateTypes( { allocator, "Known Template Type Collection Allocator" } );
		ExportContext				exportContext( engine, writer, allocator );
		const bool					initialDefaultArrayExpansionBehavior( engine.GetEngineProperty( asEEngineProp::asEP_EXPAND_DEF_ARRAY_TO_TMPL ) ? true : false );

	//	Export the engine version, just for info
		exportContext.GetFormatter() << "// AngelScript " << asGetLibraryVersion() << "\n";
		exportContext.GetFormatter() << "// Lib options " << asGetLibraryOptions() << "\n";

	//	Export the relevant engine properties
		exportContext.GetFormatter() << "// Engine properties\n"; exportContext.FlushWriter();

		for( asUINT enginePropertyIndex( 0u ); enginePropertyIndex < asEEngineProp::asEP_LAST_PROPERTY; enginePropertyIndex++ ) {
			exportContext.GetFormatter() << "ep " << enginePropertyIndex << " " << fmt::hex( engine.GetEngineProperty( asEEngineProp( enginePropertyIndex ) ) ) << "\n"; exportContext.FlushWriter();
		}

	//	Make sure the default array type is expanded to the template form.
		engine.SetEngineProperty( asEP_EXPAND_DEF_ARRAY_TO_TMPL, true );

	//	Write enum types and their values
		exportContext.GetFormatter() << "\n// Enums\n"; exportContext.FlushWriter();

		for( asUINT enumIndex( 0u ), enumCount( engine.GetEnumCount() ); enumIndex < enumCount; enumIndex++ ) {
			exportContext.WriteEnum( *engine.GetEnumByIndex( enumIndex ) );
		}

	//	Enumerate all types
		exportContext.GetFormatter() << "\n// Types\n"; exportContext.FlushWriter();

		for( asUINT objectIndex( 0u ), objectCount( engine.GetObjectTypeCount() ); objectIndex < objectCount; objectIndex++ ) {
			const auto&	objectType( *engine.GetObjectTypeByIndex( objectIndex ) );

			exportContext.DeclareType( objectType );

			// Store the template types (but not template instances)
			if( !(objectType.GetFlags() & asEObjTypeFlags::asOBJ_SCRIPT_OBJECT) && (objectType.GetFlags() & asEObjTypeFlags::asOBJ_TEMPLATE) && objectType.GetSubType() && (objectType.GetSubType()->GetFlags() & asEObjTypeFlags::asOBJ_TEMPLATE_SUBTYPE)) {
				knownTemplateTypes.Insert( &objectType );
			}
		}

		for( asUINT typedefIndex( 0u ), typedefCount( engine.GetTypedefCount() ); typedefIndex < typedefCount; typedefIndex++ ) {
			exportContext.WriteTypedef( *engine.GetTypedefByIndex( typedefIndex ) );
		}

		for( asUINT funcdefIndex( 0u ), funcdefCount( engine.GetFuncdefCount() ); funcdefIndex < funcdefCount; funcdefIndex++ ) {
			exportContext.WriteFuncdef( *engine.GetFuncdefByIndex( funcdefIndex ) );
		}

	//	Write the members of the template types, so they can be fully registered before any other type uses them.
	//	TODO: Order the template types based on dependency to avoid failure if one type uses instances of another 
		exportContext.GetFormatter() << "\n// Template type members\n"; exportContext.FlushWriter();

		for( auto templateType : knownTemplateTypes ) {
			exportContext.WriteType( *templateType );
		}

	//	Write the object type members
		exportContext.GetFormatter() << "\n// Type members\n"; exportContext.FlushWriter();

		for( asUINT typeIndex( 0u ), typeCount( engine.GetObjectTypeCount() ); typeIndex < typeCount; typeIndex++ ) {
			const auto	typeInfo( engine.GetObjectTypeByIndex( typeIndex ) );

			if( knownTemplateTypes.Find( typeInfo ) == knownTemplateTypes.End() ) {
				exportContext.WriteType( *typeInfo );
			}
		}

	//	Write global functions
		exportContext.GetFormatter() << "\n// Functions\n"; exportContext.FlushWriter();

		for( asUINT functionIndex( 0u ), functionCount( engine.GetGlobalFunctionCount() ); functionIndex < functionCount; ++functionIndex ) {
			exportContext.WriteFunction( *engine.GetGlobalFunctionByIndex( functionIndex ) );
		}

	//	Write global properties
		exportContext.GetFormatter() << "\n// Properties\n"; exportContext.FlushWriter();

		for( asUINT propertyIndex( 0u ), propertyCount( engine.GetGlobalPropertyCount() ); propertyIndex < propertyCount; ++propertyIndex ) {
			const char*	namespaceId( nullptr );
			const char*	name( nullptr );
			asDWORD		accessMask( 0u );
			bool		isConst( false );
			int			typeId( 0u );
			
			engine.GetGlobalPropertyByIndex( propertyIndex, &name, &namespaceId, &typeId, &isConst, 0, 0, &accessMask );

			exportContext.SetCurrentAccessMask( accessMask );
			exportContext.SetCurrentNamespace( namespaceId );

			exportContext.GetFormatter() << "prop \"" << (isConst ? "const " : "") << engine.GetTypeDeclaration( typeId ) << " " << name << "\"\n"; exportContext.FlushWriter();
		}

		engine.SetDefaultNamespace("");

	//	Write string factory
		exportContext.GetFormatter() << "\n// String factory\n"; exportContext.FlushWriter();

		asDWORD		stringFactoryFlags( 0 );

		if( const auto marshaledStringTypeId = engine.GetStringFactoryReturnTypeId( &stringFactoryFlags ) ) {
			exportContext.GetFormatter() << "strfactory \"" << ((stringFactoryFlags & asTM_CONST) ? "const " : "") << engine.GetTypeDeclaration( marshaledStringTypeId ) << ((stringFactoryFlags & asTM_INOUTREF) ? "&" : "") << "\"\n"; exportContext.FlushWriter();
		}

	//	Write default array type
		exportContext.GetFormatter() << "\n// Default array type\n"; exportContext.FlushWriter();

		if( const auto defaultArrayTypeId = engine.GetDefaultArrayTypeId() ) {
			exportContext.GetFormatter() << "defarray \"" << engine.GetTypeDeclaration( defaultArrayTypeId ) << "\"\n"; exportContext.FlushWriter();
		}

	//	Restore original settings
		engine.SetEngineProperty( asEEngineProp::asEP_EXPAND_DEF_ARRAY_TO_TMPL, initialDefaultArrayExpansionBehavior );

		return Error::None;
	}

// ---------------------------------------------------

	ErrorCode ImportScriptApi( Allocator& allocator, asIScriptEngine& engine, const MemoryMappedFile& file ) {
		const Utf8Char	configFile[] = "<Config file>";
		Utf8String<>	token( { allocator, "Current Token String Allocator" } );
		const auto		fileBegin( file.TryGetStructureAtOffset<char>( 0 ) );

	/*	Since we are only going to compile the script and never actually execute it, we turn off the initialization of global variables, so that the compiler
	 *	can just register dummy types and functions for the application interface. */
		if( asSUCCESS != engine.SetEngineProperty( asEEngineProp::asEP_INIT_GLOBAL_VARS_AFTER_BUILD, false ) ) {
			engine.WriteMessage( configFile, 0, 0, asMSGTYPE_ERROR, "Unable to configure engine properties" );
			return Error::Unspecified;
		}

	//	Process the configuration file and register each entity
		for( auto sourceFileRange( file.TryGetStructureArrayAtOffset<const char>( 0, file.GetMappedRegionSizeInBytes() ) ); !sourceFileRange.IsEmpty(); ) {
		//	TODO: The position where the initial token is found should be stored for error messages
			GetToken( engine, token, sourceFileRange );

		//	Engine property
			if( token == "ep" ) {	
				Utf8String<>	temp( { allocator, "Temp String Allocator" } );
				
				GetToken( engine, temp, sourceFileRange );

				const auto	ep( asEEngineProp( strtol( temp, nullptr, 10 ) ) );

			//	Get the value for the property
				GetToken( engine, temp, sourceFileRange );

			//	Only set properties that affect the compiler
				if( ep != asEP_COPY_SCRIPT_SECTIONS && ep != asEP_MAX_STACK_SIZE && ep != asEP_INIT_GLOBAL_VARS_AFTER_BUILD && ep != asEP_EXPAND_DEF_ARRAY_TO_TMPL && ep != asEP_AUTO_GARBAGE_COLLECT ) {
					engine.SetEngineProperty( ep, asPWORD( strtoul( temp, nullptr, 16 ) ) );
				}

				continue;
			}
			
		//	Namespace
			if( token == "namespace" ) {	
				Utf8String<>	namespaceName( { allocator, "Namespace Name Allocator" } );

				GetToken( engine, namespaceName, sourceFileRange );

			//	Remove leading and trailing quote
				namespaceName.Trim( 1, 1 );

				if( asSUCCESS != engine.SetDefaultNamespace( namespaceName ) ) {
					engine.WriteMessage( configFile, GetLineNumber( fileBegin, sourceFileRange.Begin() ), 0, asMSGTYPE_ERROR, "Failed to set namespace" );
					return Error::Unspecified;
				}

				continue;
			}
			
		//	Access mask
			if( token == "access" ) {
				Utf8String<>	accessToken( { allocator, "Access Token Allocator" } );

				GetToken( engine, accessToken, sourceFileRange );
				engine.SetDefaultAccessMask( asDWORD( strtoul( accessToken, nullptr, 16 ) ) );

				continue;
			}
			
		//	Object type declaration
			if( token == "objtype" ) {
				Utf8String<>	typeName( { allocator, "Object Type Name Allocator" } );
				Utf8String<>	flagsToken( { allocator, "Temp String Allocator" } );

				GetToken( engine, typeName, sourceFileRange );
				GetToken( engine, flagsToken, sourceFileRange );

			//	Remove leading and trailing quotes.
				typeName.Trim( 1, 1 );

				const asDWORD	typeFlags( strtoul( flagsToken, nullptr, 16 ) );

			//	The size of the value type doesn't matter, because the engine must adjust it anyway for different platforms
				if( asSUCCESS > engine.RegisterObjectType( typeName, (typeFlags & asOBJ_VALUE ? 1 : 0), typeFlags ) ) {
					engine.WriteMessage( configFile, GetLineNumber( fileBegin, sourceFileRange.Begin() ), 0, asMSGTYPE_ERROR, "Failed to register object type" );
					return Error::Unspecified;
				}

				continue;
			}

		//	Object behavior
			if( token == "objbeh" ) {
				Utf8String<>	typeName( { allocator, "Object Type Name Allocator" } );
				Utf8String<>	temp( { allocator, "Temp String Allocator" } );

				GetToken( engine, typeName, sourceFileRange );

			//	Remove leading and trailing quotes.
				typeName.Trim( 1, 1 );

				GetToken( engine, temp, sourceFileRange );

				const auto	behavior( asEBehaviours( atol( temp ) ) );

				GetToken( engine, temp, sourceFileRange );
			//	Remove leading and trailing quotes.
				temp.Trim( 1, 1 );
				UnescapeQuotes( temp );

			//	Remove the $ that the engine prefixes to the behaviours
				temp.Erase( temp.FindFirstInstance( '$' ) );
				
				if( behavior != asBEHAVE_TEMPLATE_CALLBACK ) {
					if( asSUCCESS > engine.RegisterObjectBehaviour( typeName, behavior, temp, asFUNCTION( 0 ), asCALL_GENERIC ) ) {
						engine.WriteMessage( configFile, GetLineNumber( fileBegin, sourceFileRange.Begin() ), 0, asMSGTYPE_ERROR, "Failed to register behavior" );
						return Error::Unspecified;
					}
				} else {
				//	TODO: How can we let the compiler register this? Maybe through a plug-in system? Or maybe by implementing the callback as a script itself
					engine.WriteMessage( configFile, GetLineNumber( fileBegin, sourceFileRange.Begin() ), 0, asMSGTYPE_WARNING, "Cannot register template callback without the actual implementation" );
				}

				continue;
			}

		//	Object method
			if( token == "objmthd" ) {
				Utf8String<>	typeName( { allocator, "Object Type Name Allocator" } );
				Utf8String<>	declaration( { allocator, "Method Declaration Allocator" } );

				GetToken( engine, typeName, sourceFileRange );
				GetToken( engine, declaration, sourceFileRange );

			//	Remove leading and trailing quotes.
				typeName.Trim( 1, 1 );

				declaration.Trim( 1, 1 );
				UnescapeQuotes( declaration );

				if( asSUCCESS > engine.RegisterObjectMethod( typeName, declaration, asFUNCTION( 0 ), asCALL_GENERIC ) ) {
					engine.WriteMessage( configFile, GetLineNumber( fileBegin, sourceFileRange.Begin() ), 0, asMSGTYPE_ERROR, "Failed to register object method" );
					return Error::Unspecified;
				}

				continue;
			}

		//	Object property
			if( token == "objprop" ) {
				Utf8String<>	typeName( { allocator, "Object Type Name Allocator" } );
				Utf8String<>	propertyName( { allocator, "Property Name Allocator" } );

				GetToken( engine, typeName, sourceFileRange );
				GetToken( engine, propertyName, sourceFileRange );

			//	Remove leading and trailing quotes.
				typeName.Trim( 1, 1 );
				propertyName.Trim( 1, 1 );

				const auto	type( engine.GetTypeInfoById( engine.GetTypeIdByDecl( typeName ) ) );
				if( type == nullptr ) {
					engine.WriteMessage( configFile, GetLineNumber( fileBegin, sourceFileRange.Begin() ), 0, asMSGTYPE_ERROR, "Type doesn't exist for property registration" );
					return Error::Unspecified;
				}

			//	All properties must have different offsets in order to make them distinct, so we simply register them with an incremental offset
				if( asSUCCESS > engine.RegisterObjectProperty( typeName, propertyName, type->GetPropertyCount() ) ) {
					engine.WriteMessage( configFile, GetLineNumber( fileBegin, sourceFileRange.Begin() ), 0, asMSGTYPE_ERROR, "Failed to register object property" );
					return Error::Unspecified;
				}

				continue;
			}

		//	Interface declaration
			if( token == "intf" ) {
				Utf8String<>	interfaceName( { allocator, "Interface Name Allocator" } );

				GetToken( engine, interfaceName, sourceFileRange );

				if( asSUCCESS > engine.RegisterInterface( interfaceName ) ) {
					engine.WriteMessage( configFile, GetLineNumber( fileBegin, sourceFileRange.Begin() ), 0, asMSGTYPE_ERROR, "Failed to register interface" );
					return Error::Unspecified;
				}

				continue;
			}

		//	Interface method
			if( token == "intfmthd" ) {
				Utf8String<>	interfaceName( { allocator, "Interface Name Allocator" } );
				Utf8String<>	methodDeclaration( { allocator, "Interface Method Declaration Allocator" } );

				GetToken( engine, interfaceName, sourceFileRange );
				GetToken( engine, methodDeclaration, sourceFileRange );

			//	Remove leading and trailing quotes.
				methodDeclaration.Trim( 1, 1 );
				UnescapeQuotes( methodDeclaration );

				if( asSUCCESS > engine.RegisterInterfaceMethod( interfaceName, methodDeclaration ) ) {
					engine.WriteMessage( configFile, GetLineNumber( fileBegin, sourceFileRange.Begin() ), 0, asMSGTYPE_ERROR, "Failed to register interface method" );
					return Error::Unspecified;
				}

				continue;
			}

		//	Global/'free' function
			if( token == "func" ) {
				Utf8String<>	functionDeclaration( { allocator, "Function Declaration Allocator" } );

				GetToken( engine, functionDeclaration, sourceFileRange );

			//	Remove leading and trailing quotes.
				functionDeclaration.Trim( 1, 1 );
				UnescapeQuotes( functionDeclaration );

				if( asSUCCESS > engine.RegisterGlobalFunction( functionDeclaration, asFUNCTION( 0 ), asCALL_GENERIC ) ) {
					engine.WriteMessage( configFile, GetLineNumber( fileBegin, sourceFileRange.Begin() ), 0, asMSGTYPE_ERROR, "Failed to register global function" );
					return Error::Unspecified;
				}

				continue;
			}

		//	Global property
			if( token == "prop" ) {
				Utf8String<>	propertyDeclaration( { allocator, "Property Declaration Allocator" } );

				GetToken( engine, propertyDeclaration, sourceFileRange );

			//	Remove leading and trailing quotes.
				propertyDeclaration.Trim( 1, 1 );
				UnescapeQuotes( propertyDeclaration );

			/*	All properties must have different offsets in order to make them distinct, so we simply register them with an incremental offset.
			 *	The pointer must also be non-null so we add 1 to have a value. */
				if( asSUCCESS > engine.RegisterGlobalProperty( propertyDeclaration, reinterpret_cast<void*>( asPWORD( engine.GetGlobalPropertyCount() + 1 ) ) ) ) {
					engine.WriteMessage( configFile, GetLineNumber( fileBegin, sourceFileRange.Begin() ), 0, asMSGTYPE_ERROR, "Failed to register global property" );
					return Error::Unspecified;
				}

				continue;
			}

		//	String factory function registration
			if( token == "strfactory" ) {
				Utf8String<>	stringTypeDeclaration( { allocator, "String Type Declaration Allocator" } );

				GetToken( engine, stringTypeDeclaration, sourceFileRange );

			//	Remove leading and trailing quotes.
				stringTypeDeclaration.Trim( 1, 1 );
				UnescapeQuotes( stringTypeDeclaration );

				if( asSUCCESS > engine.RegisterStringFactory( stringTypeDeclaration, asFUNCTION( 0 ), asCALL_GENERIC ) ) {
					engine.WriteMessage( configFile, GetLineNumber( fileBegin, sourceFileRange.Begin() ), 0, asMSGTYPE_ERROR, "Failed to register string factory" );
					return Error::Unspecified;
				}

				continue;
			}

		//	Default array type declaration
			if( token == "defarray" ) {
				Utf8String<>	defaultArrayTypeDeclaration( { allocator, "Default Array Type Declaration Allocator" } );

				GetToken( engine, defaultArrayTypeDeclaration, sourceFileRange );

			//	Remove leading and trailing quotes.
				defaultArrayTypeDeclaration.Trim( 1, 1 );
				UnescapeQuotes( defaultArrayTypeDeclaration );

				if( asSUCCESS > engine.RegisterDefaultArrayType( defaultArrayTypeDeclaration ) ) {
					engine.WriteMessage( configFile, GetLineNumber( fileBegin, sourceFileRange.Begin() ), 0, asMSGTYPE_ERROR, "Failed to register the default array type" );
					return Error::Unspecified;
				}

				continue;
			}

		//	Enum type declaration
			if( token == "enum" ) {
				Utf8String<>	enumTypeName( { allocator, "Enum Type Name Allocator" } );
				GetToken( engine, enumTypeName, sourceFileRange );

				if( asSUCCESS > engine.RegisterEnum( enumTypeName ) ) {
					engine.WriteMessage( configFile, GetLineNumber( fileBegin, sourceFileRange.Begin() ), 0, asMSGTYPE_ERROR, "Failed to register enum type" );
					return Error::Unspecified;
				}

				continue;
			}

		//	Enum value definition
			if( token == "enumval" ) {
				Utf8String<>	enumTypeName( { allocator, "Enum Type Name Allocator" } );
				Utf8String<>	enumValueName( { allocator, "Enum Value Name Allocator" } );
				Utf8String<>	enumValue( { allocator, "Enum Value Allocator" } );

				GetToken( engine, enumTypeName, sourceFileRange );
				GetToken( engine, enumValueName, sourceFileRange );
				GetToken( engine, enumValue, sourceFileRange );

				if( asSUCCESS > engine.RegisterEnumValue( enumTypeName, enumValueName, atol( enumValue ) ) ) {
					engine.WriteMessage( configFile, GetLineNumber( fileBegin, sourceFileRange.Begin() ), 0, asMSGTYPE_ERROR, "Failed to register enum value" );
					return Error::Unspecified;
				}

				continue;
			}

		//	Typedef definition
			if( token == "typedef" ) {
				Utf8String<>	typeName( { allocator, "Typedef Name Allocator" } );
				Utf8String<>	declaration( { allocator, "Typedef Declaration Allocator" } );

				GetToken( engine, typeName, sourceFileRange );
				GetToken( engine, declaration, sourceFileRange );

			//	Remove leading and trailing quotes.
				declaration.Trim( 1, 1 );

				if( asSUCCESS > engine.RegisterTypedef( typeName, declaration ) ) {
					engine.WriteMessage( configFile, GetLineNumber( fileBegin, sourceFileRange.Begin() ), 0, asMSGTYPE_ERROR, "Failed to register typedef" );
					return Error::Unspecified;
				}

				continue;
			}

		//	Funcdef definition
			if( token == "funcdef" ) {
				Utf8String<>	declaration( { allocator, "Funcdef Declaration Allocator" } );

				GetToken( engine, declaration, sourceFileRange );

			//	Remove leading and trailing quotes.
				declaration.Trim( 1, 1 );

				if( asSUCCESS > engine.RegisterFuncdef( declaration ) ) {
					engine.WriteMessage( configFile, GetLineNumber( fileBegin, sourceFileRange.Begin() ), 0, asMSGTYPE_ERROR, "Failed to register funcdef" );
					return Error::Unspecified;
				}

				continue;
			}
		}

		return Error::None;
	}

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2