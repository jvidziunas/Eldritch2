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
#include <Filesystem/ReadableMemoryMappedFile.hpp>
#include <Filesystem/SynchronousFileWriter.hpp>
#include <Scripting/AngelScript/ApiExport.hpp>
#include <Utility/Containers/UTF8String.hpp>
#include <Utility/Containers/HashSet.hpp>
#include <cppformat/cppformat/format.h>
//------------------------------------------------------------------//
#include <angelscript.h>
//------------------------------------------------------------------//
#include <algorithm>
//------------------------------------------------------------------//

using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2;

namespace {

	class ExportContext {
	public:
		ExportContext( ::asIScriptEngine& engine, SynchronousFileWriter& writer, ::Eldritch2::Allocator& allocator ) : _currentNamespace( { allocator, UTF8L("Current Namespace String Allocator") } ),
																													   _writer( writer ),
																													   _formatter( _writeBuffer ),
																													   _accessMask( 0u ),
																													   _engine( engine ) {
			engine.SetDefaultNamespace( _currentNamespace.AsCString() );
		}

	// ---------------------------------------------------

		void UpdateCurrentNamespace( const char* newNamespace ) {
			if( newNamespace && (_currentNamespace != newNamespace) ) {
				_formatter << "namespace \"" << newNamespace << "\"\n";

				_currentNamespace = newNamespace;
				_engine.SetDefaultNamespace( _currentNamespace.AsCString() );

				FlushWriter();
			}
		}

	// ---------------------------------------------------

		void UpdateCurrentAccessMask( ::asDWORD accessMask ) {
			if( accessMask != _accessMask ) {
				_formatter << "access " << fmt::hex( (unsigned int)(accessMask) ) << "\n";
				_accessMask = accessMask;

				FlushWriter();
			}
		}

	// ---------------------------------------------------

		ETInlineHint void PushType( const ::asITypeInfo& type ) {
			UpdateCurrentNamespace( type.GetNamespace() );
			UpdateCurrentAccessMask( type.GetAccessMask() );
		}

	// ---------------------------------------------------

		ETInlineHint void PushFunction( const ::asIScriptFunction& function ) {
			UpdateCurrentNamespace( function.GetNamespace() );
			UpdateCurrentAccessMask( function.GetAccessMask() );
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

		void DeclareType( const ::asITypeInfo& type ) {
			PushType( type );

			if( type.GetFlags() & asOBJ_SCRIPT_OBJECT ) {
				// This should only be interfaces
				assert( type.GetSize() == 0 );

				GetFormatter() << "intf " << type.GetName() << "\n"; 
			} else {
				// Only the type flags are necessary. The application flags are application
				// specific and doesn't matter to the offline compiler. The object size is also
				// unnecessary for the offline compiler
				GetFormatter() << "objtype \"" << type.GetName() << "\" " << (unsigned int)(type.GetFlags() & asOBJ_MASK_VALID_FLAGS) << "\n";
			}

			FlushWriter();
		}

	// ---------------------------------------------------

		void WriteEnum( const ::asITypeInfo& type ) {
			PushType( type );

			const char* const	enumName( type.GetName() );
			GetFormatter() << "enum " << enumName << "\n"; FlushWriter();

			for( ::asUINT valueIndex( 0u ); valueIndex < type.GetEnumValueCount(); valueIndex++ ) {
				int			enumValue;
				const char*	valueString( type.GetEnumValueByIndex( valueIndex, &enumValue ) );

				GetFormatter() << "enumval " << enumName << " " << valueString << " " << enumValue << "\n"; FlushWriter();
			}
		}

	// ---------------------------------------------------

		void WriteTypedef( const ::asITypeInfo& type ) {
			PushType( type );

			GetFormatter() << "typedef " << type.GetName() << " \"" << _engine.GetTypeDeclaration( type.GetTypedefTypeId() ) << "\"\n"; FlushWriter();
		}

	// ---------------------------------------------------

		void WriteFuncdef( const ::asITypeInfo& type ) {
			PushType( type );

			GetFormatter() << "funcdef \"" << type.GetFuncdefSignature()->GetDeclaration() << "\"\n"; FlushWriter();
		}

	// ---------------------------------------------------

		void WriteType( const ::asITypeInfo& type ) {
			UTF8String<>	declarationTempString( { _currentNamespace.GetAllocator().GetParent(), UTF8L("Temporary Type Declaration String Allocator") } );

			PushType( type );

			if( type.GetFlags() & asOBJ_SCRIPT_OBJECT ) {
				for( ::asUINT methodIndex( 0 ); methodIndex < type.GetMethodCount(); methodIndex++ ) {
					const auto&	method( *type.GetMethodByIndex( methodIndex ) );

					UpdateCurrentAccessMask( method.GetAccessMask() );

					GetFormatter() << "intfmthd " << type.GetName() << " \"" << CopyDeclarationEscapingQuotes( declarationTempString, method.GetDeclaration( false ) ).AsCString() << "\"\n"; FlushWriter();
				}
			} else {
				for( ::asUINT factoryIndex( 0 ); factoryIndex < type.GetFactoryCount(); factoryIndex++ ) {
					const auto&	factory( *type.GetFactoryByIndex( factoryIndex ) );

					UpdateCurrentAccessMask( factory.GetAccessMask() );

					GetFormatter() << "objbeh \"" << type.GetName() << "\" " << asBEHAVE_FACTORY << " \"" << CopyDeclarationEscapingQuotes( declarationTempString, factory.GetDeclaration( false ) ).AsCString() << "\"\n"; FlushWriter();
				}

				for( ::asUINT behaviorIndex( 0 ); behaviorIndex < type.GetBehaviourCount(); behaviorIndex++ ) {
					::asEBehaviours	behaviorClass;
					const auto&		behavior( *type.GetBehaviourByIndex( behaviorIndex, &behaviorClass ) );

					if( asEBehaviours::asBEHAVE_CONSTRUCT == behaviorClass ) {
						// Prefix 'void'
						GetFormatter() << "objbeh \"" << type.GetName() << "\" " << behaviorClass << " \"void " << CopyDeclarationEscapingQuotes( declarationTempString, behavior.GetDeclaration( false ) ).AsCString() << "\"\n";
					} else if( asEBehaviours::asBEHAVE_DESTRUCT == behaviorClass ) {
						// Prefix 'void' and remove ~
						GetFormatter() << "objbeh \"" << type.GetName() << "\" " << behaviorClass << " \"void " << CopyDeclarationEscapingQuotes( declarationTempString, behavior.GetDeclaration( false ) ).AsCString() + 1 << "\"\n";
					} else {
						GetFormatter() << "objbeh \"" << type.GetName() << "\" " << behaviorClass << " \"" << CopyDeclarationEscapingQuotes( declarationTempString, behavior.GetDeclaration( false ) ).AsCString() << "\"\n";
					}

					FlushWriter();
				}

				for( ::asUINT methodIndex( 0 ); methodIndex < type.GetMethodCount(); methodIndex++ ) {
					const auto&	method( *type.GetMethodByIndex( methodIndex ) );

					UpdateCurrentAccessMask( method.GetAccessMask() );

					GetFormatter() << "objmthd \"" << type.GetName() << "\" \"" << CopyDeclarationEscapingQuotes( declarationTempString, method.GetDeclaration( false ) ).AsCString() << "\"\n"; FlushWriter();
				}

				for( ::asUINT propertyIndex( 0 ); propertyIndex < type.GetPropertyCount(); propertyIndex++ ) {
					::asDWORD	propertyAccessMask;

					type.GetProperty( propertyIndex, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, &propertyAccessMask );

					UpdateCurrentAccessMask( propertyAccessMask );

					GetFormatter() << "objprop \"" << type.GetName() << "\" \"" << type.GetPropertyDeclaration( propertyIndex ) << "\"\n"; FlushWriter();
				}
			}
		}

	// ---------------------------------------------------

		void WriteFunction( const ::asIScriptFunction& function ) {
			UTF8String<>	signature( { _currentNamespace.GetAllocator().GetParent(), UTF8L("Function Signature Allocator") } );

			PushFunction( function );

			GetFormatter() << "func \"" << CopyDeclarationEscapingQuotes( signature, function.GetDeclaration() ).AsCString() << "\"\n"; FlushWriter();
		}

	// ---------------------------------------------------

		static ETNoAliasHint UTF8String<>& CopyDeclarationEscapingQuotes( UTF8String<>& output, const char* decl ) {
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
		::asIScriptEngine&		_engine;
		SynchronousFileWriter&	_writer;
		fmt::ArrayWriter		_formatter;
		UTF8String<>			_currentNamespace;
		::asDWORD				_accessMask;
		char					_writeBuffer[768u];
	};

// ---------------------------------------------------

	static ETNoAliasHint void UnescapeQuotes( UTF8String<>& str ) {
		for( UTF8String<>::ConstIterator position( str.Begin() ); ; ) {
			// Search for \" in the string
			position = str.FindFirstInstance( "\\\"", position );
			if( position == str.End() ) {
				break;
			}

			// Remove the \ character
			str.Erase( position );
		}
	}

// ---------------------------------------------------

	static asETokenClass GetToken( ::asIScriptEngine& engine, UTF8String<>& token, Range<const char*>& source ) {
		const char*		readPointer( source.first );
		const char*		readEnd( source.onePastLast - 1 );
		asUINT			tokenLength( 0 );
		asETokenClass	tokenClass( engine.ParseToken( readPointer, source.Size(), &tokenLength ) );

		while( (tokenClass == asTC_WHITESPACE || tokenClass == asTC_COMMENT) && (readPointer != readEnd) ) {
			readPointer	+= tokenLength;
			tokenClass = engine.ParseToken( readPointer, readEnd - readPointer, &tokenLength );
		}

		token.Assign( readPointer, readPointer + tokenLength );
		source.first = readPointer + tokenLength;

		return tokenClass;
	}

}	// anonymous namespace

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {

	ErrorCode ExportScriptApi( Allocator& allocator, ::asIScriptEngine& engine, SynchronousFileWriter& writer ) {
		// Keep a list of the template types, as the methods for these need to be exported first.
		HashSet<const ::asITypeInfo*>	knownTemplateTypes( { allocator, UTF8L("Known Template Type Collection Allocator") } );
		ExportContext					exportContext( engine, writer, allocator );
		const bool						initialDefaultArrayExpansionBehavior( engine.GetEngineProperty( asEP_EXPAND_DEF_ARRAY_TO_TMPL ) ? true : false );

		// Export the engine version, just for info
		exportContext.GetFormatter() << "// AngelScript " << ::asGetLibraryVersion() << "\n";
		exportContext.GetFormatter() << "// Lib options " << ::asGetLibraryOptions() << "\n";

		// Export the relevant engine properties
		exportContext.GetFormatter() << "// Engine properties\n"; exportContext.FlushWriter();

		for( ::asUINT enginePropertyIndex( 0u ); enginePropertyIndex < asEP_LAST_PROPERTY; enginePropertyIndex++ ) {
			exportContext.GetFormatter() << "ep " << enginePropertyIndex << " " << engine.GetEngineProperty( ::asEEngineProp( enginePropertyIndex ) ) << "\n"; exportContext.FlushWriter();
		}

		// Make sure the default array type is expanded to the template form.
		engine.SetEngineProperty( asEP_EXPAND_DEF_ARRAY_TO_TMPL, true );

		// Write enum types and their values
		exportContext.GetFormatter() << "\n// Enums\n"; exportContext.FlushWriter();

		for( ::asUINT enumIndex( 0u ), enumCount( engine.GetEnumCount() ); enumIndex < enumCount; enumIndex++ ) {
			exportContext.WriteEnum( *engine.GetEnumByIndex( enumIndex ) );
		}

		// Enumerate all types
		exportContext.GetFormatter() << "\n// Types\n"; exportContext.FlushWriter();

		for( ::asUINT objectIndex( 0u ), objectCount( engine.GetObjectTypeCount() ); objectIndex < objectCount; objectIndex++ ) {
			const auto&	objectType( *engine.GetObjectTypeByIndex( objectIndex ) );

			exportContext.DeclareType( objectType );

			// Store the template types (but not template instances)
			if( !(objectType.GetFlags() & asOBJ_SCRIPT_OBJECT) && (objectType.GetFlags() & asOBJ_TEMPLATE) && objectType.GetSubType() && (objectType.GetSubType()->GetFlags() & asOBJ_TEMPLATE_SUBTYPE)) {
				knownTemplateTypes.Insert( &objectType );
			}
		}

		for( ::asUINT typedefIndex( 0u ), typedefCount( engine.GetTypedefCount() ); typedefIndex < typedefCount; typedefIndex++ ) {
			exportContext.WriteTypedef( *engine.GetTypedefByIndex( typedefIndex ) );
		}

		for( ::asUINT funcdefIndex( 0u ), funcdefCount( engine.GetFuncdefCount() ); funcdefIndex < funcdefCount; funcdefIndex++ ) {
			exportContext.WriteFuncdef( *engine.GetFuncdefByIndex( funcdefIndex ) );
		}

		// Write the members of the template types, so they can be fully registered before any other type uses them
		// TODO: Order the template types based on dependency to avoid failure if one type uses instances of another 
		exportContext.GetFormatter() << "\n// Template type members\n"; exportContext.FlushWriter();

		for( auto templateType : knownTemplateTypes ) {
			exportContext.WriteType( *templateType );
		}

		// Write the object types members
		exportContext.GetFormatter() << "\n// Type members\n"; exportContext.FlushWriter();

		for( ::asUINT typeIndex( 0u ), typeCount( engine.GetObjectTypeCount() ); typeIndex < typeCount; typeIndex++ ) {
			const auto	typeInfo( engine.GetObjectTypeByIndex( typeIndex ) );

			if( knownTemplateTypes.Find( typeInfo ) == knownTemplateTypes.End() ) {
				exportContext.WriteType( *typeInfo );
			}
		}

		// Write functions
		exportContext.GetFormatter() << "\n// Functions\n"; exportContext.FlushWriter();

		for( ::asUINT functionIndex( 0u ), functionCount( engine.GetGlobalFunctionCount() ); functionIndex < functionCount; functionIndex++ ) {
			exportContext.WriteFunction( *engine.GetGlobalFunctionByIndex( functionIndex ) );
		}

		// Write global properties
		exportContext.GetFormatter() << "\n// Properties\n"; exportContext.FlushWriter();

		for( ::asUINT propertyIndex( 0u ), propertyCount( engine.GetGlobalPropertyCount() ); propertyIndex < propertyCount; propertyIndex++ ) {
			const char*	propertyNamespace( nullptr );
			const char*	propertyName( nullptr );
			::asDWORD	propertyAccessMask( 0u );
			bool		propertyIsConst( false );
			int			propertyTypeId( 0u );
			
			engine.GetGlobalPropertyByIndex( propertyIndex, &propertyName, &propertyNamespace, &propertyTypeId, &propertyIsConst, 0, 0, &propertyAccessMask );

			exportContext.UpdateCurrentAccessMask( propertyAccessMask );
			exportContext.UpdateCurrentNamespace( propertyNamespace );

			exportContext.GetFormatter() << "prop \"" << (propertyIsConst ? "const " : "") << engine.GetTypeDeclaration( propertyTypeId ) << " " << propertyName << "\"\n"; exportContext.FlushWriter();
		}

		engine.SetDefaultNamespace("");

		// Write string factory
		exportContext.GetFormatter() << "\n// String factory\n"; exportContext.FlushWriter();

		::asDWORD	stringFactoryFlags( 0 );
		const int	stringFactoryReturnTypeId( engine.GetStringFactoryReturnTypeId( &stringFactoryFlags ) );

		if( stringFactoryReturnTypeId > 0 ) {
			exportContext.GetFormatter() << "strfactory \"" << ((stringFactoryFlags & asTM_CONST) ? "const " : "") << engine.GetTypeDeclaration( stringFactoryReturnTypeId ) << ((stringFactoryFlags & asTM_INOUTREF) ? "&" : "") << "\"\n"; exportContext.FlushWriter();
		}

		// Write default array type
		exportContext.GetFormatter() << "\n// Default array type\n"; exportContext.FlushWriter();

		const int	defaultArrayTypeId( engine.GetDefaultArrayTypeId() );

		if( defaultArrayTypeId > 0 ) {
			exportContext.GetFormatter() << "defarray \"" << engine.GetTypeDeclaration( defaultArrayTypeId ) << "\"\n"; exportContext.FlushWriter();
		}

		// Restore original settings
		engine.SetEngineProperty( asEP_EXPAND_DEF_ARRAY_TO_TMPL, initialDefaultArrayExpansionBehavior );

		return Error::None;
	}

// ---------------------------------------------------

	ErrorCode ImportScriptApi( Allocator& allocator, ::asIScriptEngine& engine, const ReadableMemoryMappedFile& file ) {
		const ::Eldritch2::UTF8Char	configFile[] = UTF8L( "<Config file>" );
		UTF8String<>				token( { allocator, UTF8L("Current Token String Allocator") } );
		const auto					fileBegin( file.TryGetStructureAtOffset<char>( 0 ) );
		int							r;

		// Since we are only going to compile the script and never actually execute it,
		// we turn off the initialization of global variables, so that the compiler can
		// just register dummy types and functions for the application interface.
		r = engine.SetEngineProperty( asEP_INIT_GLOBAL_VARS_AFTER_BUILD, false ); assert( r >= 0 );

		// Process the configuration file and register each entity
		for( auto sourceFileRange( file.TryGetStructureArrayAtOffset<char>( 0, file.GetAccessibleRegionSizeInBytes() ) ); !sourceFileRange.IsEmpty(); ) {
			// TODO: The position where the initial token is found should be stored for error messages
			GetToken( engine, token, sourceFileRange );

			// Engine property
			if( token == "ep" ) {
				UTF8String<>	temp( { allocator, UTF8L("Temp String Allocator") } );
				::asPWORD		propertyValue;
				long			propertyIndex;
				
				GetToken( engine, temp, sourceFileRange );

				temp.ParseInto( propertyIndex );

				const auto	ep( static_cast<::asEEngineProp>( propertyIndex ) );

				// Get the value for the property
				GetToken( engine, temp, sourceFileRange );

				// Only set properties that affect the compiler
				if( ep != asEP_COPY_SCRIPT_SECTIONS &&
					ep != asEP_MAX_STACK_SIZE &&
					ep != asEP_INIT_GLOBAL_VARS_AFTER_BUILD &&
					ep != asEP_EXPAND_DEF_ARRAY_TO_TMPL &&
					ep != asEP_AUTO_GARBAGE_COLLECT &&
					temp.ParseInto( propertyValue ) ) {
					engine.SetEngineProperty( ep, propertyValue );
				}

			// Namespace
			} else if( token == "namespace" ) {
				UTF8String<>	namespaceName( { allocator, UTF8L("Namespace Name Allocator") } );

				GetToken( engine, namespaceName, sourceFileRange );

				// Remove leading and trailing quote
				namespaceName.Trim( 1, 1 );

				r = engine.SetDefaultNamespace( namespaceName.AsCString() );
				if( r < 0 ) {
					engine.WriteMessage( configFile, ::std::count( fileBegin, sourceFileRange.first, '\n' ), 0, asMSGTYPE_ERROR, "Failed to set namespace" );
					return Error::Unspecified;
				}

			// Access mask
			} else if( token == "access" ) {
				UTF8String<>	accessToken( { allocator, UTF8L("Access Token Allocator") } );

				GetToken( engine, accessToken, sourceFileRange );
				engine.SetDefaultAccessMask( static_cast<::asDWORD>(strtoul( accessToken.AsCString(), 0, 16 )) );

			// Object type declaration
			} else if( token == "objtype" ) {
				UTF8String<>	objectTypeName( { allocator, UTF8L("Object Type Name Allocator") } );
				UTF8String<>	typeFlagsToken( { allocator, UTF8L("Temp String Allocator") } );
				::asDWORD		typeFlags;

				GetToken( engine, objectTypeName, sourceFileRange );
				GetToken( engine, typeFlagsToken, sourceFileRange );

				// Remove leading and trailing quotes.
				objectTypeName.Trim( 1, 1 );
				typeFlagsToken.ParseInto( typeFlags );

				// The size of the value type doesn't matter, because the
				// engine must adjust it anyway for different platforms
				r = engine.RegisterObjectType( objectTypeName.AsCString(), (typeFlags & asOBJ_VALUE ? 1 : 0), typeFlags );
				if( r < 0 ) {
					engine.WriteMessage( configFile, ::std::count( fileBegin, sourceFileRange.first, '\n' ), 0, asMSGTYPE_ERROR, "Failed to register object type" );
					return Error::Unspecified;
				}

			// Object behavior
			} else if( token == "objbeh" ) {
				UTF8String<>	objectTypeName( { allocator, UTF8L("Object Type Name Allocator") } );
				UTF8String<>	temp( { allocator, UTF8L("Temp String Allocator") } );

				GetToken( engine, objectTypeName, sourceFileRange );
				// Remove leading and trailing quotes.
				objectTypeName.Trim( 1, 1 );

				GetToken( engine, temp, sourceFileRange );
				const ::asEBehaviours	behave( static_cast<asEBehaviours>(atol( temp.AsCString() )) );

				GetToken( engine, temp, sourceFileRange );
				// Remove leading and trailing quotes.
				temp.Trim( 1, 1 );
				UnescapeQuotes( temp );

				// Remove the $ that the engine prefixes the behaviours with
				temp.Erase( temp.FindFirstInstance( '$' ) );
				
				if( behave == asBEHAVE_TEMPLATE_CALLBACK ) {
					// TODO: How can we let the compiler register this? Maybe through a plug-in system? Or maybe by implementing the callback as a script itself
					engine.WriteMessage( configFile, ::std::count( fileBegin, sourceFileRange.first, '\n' ), 0, asMSGTYPE_WARNING, "Cannot register template callback without the actual implementation" );
				} else {
					r = engine.RegisterObjectBehaviour( objectTypeName.AsCString(), behave, temp.AsCString(), asFUNCTION( 0 ), asCALL_GENERIC );
					if( r < 0 ) {
						engine.WriteMessage( configFile, ::std::count( fileBegin, sourceFileRange.first, '\n' ), 0, asMSGTYPE_ERROR, "Failed to register behavior" );
						return Error::Unspecified;
					}
				}

			// Object method
			} else if( token == "objmthd" ) {
				UTF8String<>	objectTypeName( { allocator, UTF8L("Object Type Name Allocator") } );
				UTF8String<>	methodDeclaration( { allocator, UTF8L("Method Declaration Allocator") } );

				GetToken( engine, objectTypeName, sourceFileRange );
				GetToken( engine, methodDeclaration, sourceFileRange );

				// Remove leading and trailing quotes.
				objectTypeName.Trim( 1, 1 );

				methodDeclaration.Trim( 1, 1 );
				UnescapeQuotes( methodDeclaration );

				r = engine.RegisterObjectMethod( objectTypeName.AsCString(), methodDeclaration.AsCString(), asFUNCTION( 0 ), asCALL_GENERIC );
				if( r < 0 ) {
					engine.WriteMessage( configFile, ::std::count( fileBegin, sourceFileRange.first, '\n' ), 0, asMSGTYPE_ERROR, "Failed to register object method" );
					return Error::Unspecified;
				}

			// Object property
			} else if( token == "objprop" ) {
				UTF8String<>	objectTypeName( { allocator, UTF8L("Object Type Name Allocator") } );
				UTF8String<>	propertyName( { allocator, UTF8L("Property Name Allocator") } );

				GetToken( engine, objectTypeName, sourceFileRange );
				GetToken( engine, propertyName, sourceFileRange );

				// Remove leading and trailing quotes.
				objectTypeName.Trim( 1, 1 );
				propertyName.Trim( 1, 1 );

				const auto	type( engine.GetTypeInfoById( engine.GetTypeIdByDecl( objectTypeName.AsCString() ) ) );
				if( type == nullptr ) {
					engine.WriteMessage( configFile, ::std::count( fileBegin, sourceFileRange.first, '\n' ), 0, asMSGTYPE_ERROR, "Type doesn't exist for property registration" );
					return Error::Unspecified;
				}

				// All properties must have different offsets in order to make them
				// distinct, so we simply register them with an incremental offset
				r = engine.RegisterObjectProperty( objectTypeName.AsCString(), propertyName.AsCString(), type->GetPropertyCount() );
				if( r < 0 ) {
					engine.WriteMessage( configFile, ::std::count( fileBegin, sourceFileRange.first, '\n' ), 0, asMSGTYPE_ERROR, "Failed to register object property" );
					return Error::Unspecified;
				}

			// Interface declaration
			} else if( token == "intf" ) {
				UTF8String<>	interfaceName( { allocator, UTF8L("Interface Name Allocator") } );

				GetToken( engine, interfaceName, sourceFileRange );

				r = engine.RegisterInterface( interfaceName.AsCString() );
				if( r < 0 ) {
					engine.WriteMessage( configFile, ::std::count( fileBegin, sourceFileRange.first, '\n' ), 0, asMSGTYPE_ERROR, "Failed to register interface" );
					return Error::Unspecified;
				}

			// Interface method
			} else if( token == "intfmthd" ) {
				UTF8String<>	interfaceName( { allocator, UTF8L("Interface Name Allocator") } );
				UTF8String<>	methodDeclaration( { allocator, UTF8L("Interface Method Declaration Allocator") } );

				GetToken( engine, interfaceName, sourceFileRange );
				GetToken( engine, methodDeclaration, sourceFileRange );

				// Remove leading and trailing quotes.
				methodDeclaration.Trim( 1, 1 );
				UnescapeQuotes( methodDeclaration );

				r = engine.RegisterInterfaceMethod( interfaceName.AsCString(), methodDeclaration.AsCString() );
				if( r < 0 ) {
					engine.WriteMessage( configFile, ::std::count( fileBegin, sourceFileRange.first, '\n' ), 0, asMSGTYPE_ERROR, "Failed to register interface method" );
					return Error::Unspecified;
				}

			// Global/'free' function
			} else if( token == "func" ) {
				UTF8String<>	functionDeclaration( { allocator, UTF8L("Function Declaration Allocator") } );

				GetToken( engine, functionDeclaration, sourceFileRange );

				// Remove leading and trailing quotes.
				functionDeclaration.Trim( 1, 1 );
				UnescapeQuotes( functionDeclaration );

				r = engine.RegisterGlobalFunction( functionDeclaration.AsCString(), asFUNCTION( 0 ), asECallConvTypes::asCALL_GENERIC );
				if( r < 0 ) {
					engine.WriteMessage( configFile, ::std::count( fileBegin, sourceFileRange.first, '\n' ), 0, asMSGTYPE_ERROR, "Failed to register global function" );
					return Error::Unspecified;
				}

			// Global property
			} else if( token == "prop" ) {
				UTF8String<>	propertyDeclaration( { allocator, UTF8L("Property Declaration Allocator") } );

				GetToken( engine, propertyDeclaration, sourceFileRange );

				// Remove leading and trailing quotes.
				propertyDeclaration.Trim( 1, 1 );
				UnescapeQuotes( propertyDeclaration );

				// All properties must have different offsets in order to make them
				// distinct, so we simply register them with an incremental offset.
				// The pointer must also be non-null so we add 1 to have a value.
				r = engine.RegisterGlobalProperty( propertyDeclaration.AsCString(), reinterpret_cast<void*>(asPWORD( engine.GetGlobalPropertyCount() + 1 )) );
				if( r < 0 ) {
					engine.WriteMessage( configFile, ::std::count( fileBegin, sourceFileRange.first, '\n' ), 0, asMSGTYPE_ERROR, "Failed to register global property" );
					return Error::Unspecified;
				}

			// String factory function registration
			} else if( token == "strfactory" ) {
				UTF8String<>	stringTypeDeclaration( { allocator, UTF8L("String Type Declaration Allocator") } );

				GetToken( engine, stringTypeDeclaration, sourceFileRange );

				// Remove leading and trailing quotes.
				stringTypeDeclaration.Trim( 1, 1 );
				UnescapeQuotes( stringTypeDeclaration );

				r = engine.RegisterStringFactory( stringTypeDeclaration.AsCString(), asFUNCTION( 0 ), asECallConvTypes::asCALL_GENERIC );
				if( r < 0 ) {
					engine.WriteMessage( configFile, ::std::count( fileBegin, sourceFileRange.first, '\n' ), 0, asMSGTYPE_ERROR, "Failed to register string factory" );
					return Error::Unspecified;
				}

			// Default array type declaration
			} else if( token == "defarray" ) {
				UTF8String<>	defaultArrayTypeDeclaration( { allocator, UTF8L("Default Array Type Declaration Allocator") } );

				GetToken( engine, defaultArrayTypeDeclaration, sourceFileRange );

				// Remove leading and trailing quotes.
				defaultArrayTypeDeclaration.Trim( 1, 1 );
				UnescapeQuotes( defaultArrayTypeDeclaration );

				r = engine.RegisterDefaultArrayType( defaultArrayTypeDeclaration.AsCString() );
				if( r < 0 ) {
					engine.WriteMessage( configFile, ::std::count( fileBegin, sourceFileRange.first, '\n' ), 0, asMSGTYPE_ERROR, "Failed to register the default array type" );
					return Error::Unspecified;
				}

			// Enum type declaration
			} else if( token == "enum" ) {
				UTF8String<>	enumTypeName( { allocator, UTF8L("Enum Type Name Allocator") } );
				GetToken( engine, enumTypeName, sourceFileRange );

				r = engine.RegisterEnum( enumTypeName.AsCString() );
				if( r < 0 ) {
					engine.WriteMessage( configFile, ::std::count( fileBegin, sourceFileRange.first, '\n' ), 0, asMSGTYPE_ERROR, "Failed to register enum type" );
					return Error::Unspecified;
				}

			// Enum value definition
			} else if( token == "enumval" ) {
				UTF8String<>	enumTypeName( { allocator, UTF8L("Enum Type Name Allocator") } );
				UTF8String<>	enumValueName( { allocator, UTF8L("Enum Value Name Allocator") } );
				UTF8String<>	enumValue( { allocator, UTF8L("Enum Value Allocator") } );

				GetToken( engine, enumTypeName, sourceFileRange );
				GetToken( engine, enumValueName, sourceFileRange );
				GetToken( engine, enumValue, sourceFileRange );

				r = engine.RegisterEnumValue( enumTypeName.AsCString(), enumValueName.AsCString(), atol( enumValue.AsCString() ) );
				if( r < 0 ) {
					engine.WriteMessage( configFile, ::std::count( fileBegin, sourceFileRange.first, '\n' ), 0, asMSGTYPE_ERROR, "Failed to register enum value" );
					return Error::Unspecified;
				}

			// Typedef definition
			} else if( token == "typedef" ) {
				UTF8String<>	typeName( { allocator, UTF8L("Typedef Name Allocator") } );
				UTF8String<>	declaration( { allocator, UTF8L("Typedef Declaration Allocator") } );

				GetToken( engine, typeName, sourceFileRange );
				GetToken( engine, declaration, sourceFileRange );

				// Remove leading and trailing quotes.
				declaration.Trim( 1, 1 );

				r = engine.RegisterTypedef( typeName.AsCString(), declaration.AsCString() );
				if( r < 0 ) {
					engine.WriteMessage( configFile, ::std::count( fileBegin, sourceFileRange.first, '\n' ), 0, asMSGTYPE_ERROR, "Failed to register typedef" );
					return Error::Unspecified;
				}

			// Funcdef definition
			} else if( token == "funcdef" ) {
				UTF8String<>	declaration( { allocator, UTF8L("Funcdef Declaration Allocator") } );

				GetToken( engine, declaration, sourceFileRange );

				// Remove leading and trailing quotes.
				declaration.Trim( 1, 1 );

				r = engine.RegisterFuncdef( declaration.AsCString() );
				if( r < 0 ) {
					engine.WriteMessage( configFile, ::std::count( fileBegin, sourceFileRange.first, '\n' ), 0, asMSGTYPE_ERROR, "Failed to register funcdef" );
					return Error::Unspecified;
				}
			}
		}

		return Error::None;
	}

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2