/*==================================================================*\
  ScriptCompilerCookerStrategy.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <ScriptCompilerCookerStrategy.hpp>
#include <Tools/FileWriteStrategyFactory.hpp>
#include <Tools/FileReadStrategyFactory.hpp>
#include <Utility/FileWriteAccessStrategy.hpp>
#include <Utility/FileReadAccessStrategy.hpp>
#include <Utility/ErrorCode.hpp>
#if( ET_PLATFORM_WINDOWS )
#	include <Windows.h>
#endif
#include <angelscript/sdk/add_on/scriptbuilder/scriptbuilder.h>
#include <string>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_LIBRARY( "angelscript.lib" )
//------------------------------------------------------------------//

using namespace ::Eldritch2;
using namespace ::Eldritch2::Tools;
using namespace ::Eldritch2::FileSystem;
using namespace ::boost;
using namespace ::std;

namespace
{

#if( ET_PLATFORM_WINDOWS )
	ENUM_CLASS( DWORD )
	{
		SEARCH_DLL_LOAD_DIR		= 0x00000100,	// LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR
		SEARCH_APPLICATION_DIR	= 0x00000200	// LOAD_LIBRARY_SEARCH_APPLICATION_DIR
	};
#endif

// ---------------------------------------------------

	static const char	buildModuleName[]	= "build";

// ---------------------------------------------------

	static ScriptCompilerCookerStrategy::OptimizeBytecodeOption	optimizeBytecodeOption;

// ---------------------------------------------------

	class Eldritch2ScriptBuilder : public ::CScriptBuilder
	{
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		Eldritch2ScriptBuilder( ::asIScriptEngine* const	scriptEngine,
								FileReadStrategyFactory&	readStrategyFactory ) : CScriptBuilder(),
																					_readStrategyFactory( readStrategyFactory )
		{
			StartNewModule( scriptEngine, buildModuleName );
		}

	// ---------------------------------------------------

		ETInlineHint bool operator()( const ::Eldritch2::SystemChar* const file )
		{
			typedef FileReadAccessStrategy::BlockingResult	ReadResult;

		// ---

			if( FileReadAccessStrategy* const accessor = _readStrategyFactory.GetAccessor( file, FileReadAccessorBehavior::UNBUFFERED ) )
			{
				char* const	fileBuffer( static_cast<char*>( _readStrategyFactory.GetFileDataAllocator().Allocate( accessor->GetRoundedSizeInBytes() + 1u,
																												  accessor->GetPhysicalMediaSectorSizeInBytes() ) ) );
				ReadResult	readResult( accessor->Read( fileBuffer,
														accessor->GetRoundedSizeInBytes() ) );

				if( readResult.result )
				{
					// The script builder expects the file contents to be null-terminated.
					fileBuffer[readResult.readAmountInBytes] = '\0';

					readResult.result = ( ::asSUCCESS <= AddSectionFromMemory( fileBuffer,
																			   string( file, FindEndOfString( file ) ).c_str() ) ) ? Error::NONE : Error::UNSPECIFIED;
				}

				_readStrategyFactory.GetFileDataAllocator().DeallocateAligned( fileBuffer );

				return !( readResult.result );
			}

			return true;
		}

		ETInlineHint ErrorCode	SaveByteCode( FileWriteAccessStrategy& output )
		{
			class FileStream : public ::asIBinaryStream
			{
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

			public:
				FileStream( FileWriteAccessStrategy& writer ) : _writer( writer ) {}

				~FileStream() {}

			// ---------------------------------------------------

				void Write( const void* src, ::asUINT size )	{ _writer.Write( src, size ); }

				void Read( void* dst, ::asUINT dstLength )		{ ETUnreferencedParameter( dst ); ETUnreferencedParameter( dstLength ); }

			// - DATA MEMBERS ------------------------------------

			private:
				FileWriteAccessStrategy&	_writer;
			};

		// ---

			return( ::asSUCCESS <= GetModule()->SaveByteCode( &FileStream( output ) ) ? Error::NONE : Error::UNSPECIFIED );
		}

	// ---------------------------------------------------

	private:
		FileReadStrategyFactory&	_readStrategyFactory;
	};

// ---------------------------------------------------

	static CookerOption<ScriptCompilerCookerStrategy>*	optionsTable[] =
	{
		&optimizeBytecodeOption
	};

}	// anonymous namespace

namespace Eldritch2
{

	namespace Tools
	{

		ScriptCompilerCookerStrategy::OptimizeBytecodeOption::OptimizeBytecodeOption() : BaseType( &ScriptCompilerCookerStrategy::_optimizeBytecode )
		{
		}

	// ---------------------------------------------------

		ScriptCompilerCookerStrategy::OptimizeBytecodeOption::~OptimizeBytecodeOption()
		{
		}

	// ---------------------------------------------------

		const SystemChar* ScriptCompilerCookerStrategy::OptimizeBytecodeOption::GetArgumentString() const
		{
			return SL( "-optimizeBytecode" );
		}

	// ---------------------------------------------------

		ScriptCompilerCookerStrategy::ScriptCompilerCookerStrategy() : ContentCookerStrategy(),
																	   _optimizeBytecode( true )
		{
		}

	// ---------------------------------------------------

		ScriptCompilerCookerStrategy::~ScriptCompilerCookerStrategy()
		{
		}

	// ---------------------------------------------------

		ScriptCompilerCookerStrategy::OptionIterator ScriptCompilerCookerStrategy::OptionsBegin()
		{
			return optionsTable;
		}

	// ---------------------------------------------------

		ScriptCompilerCookerStrategy::OptionIterator ScriptCompilerCookerStrategy::OptionsEnd()
		{
			return optionsTable + _countof( optionsTable );
		}

	// ---------------------------------------------------

		ErrorCode ScriptCompilerCookerStrategy::Cook( FileWriteAccessStrategy&	output,
													  const SystemChar* const*	sourceFiles,
													  const SystemChar* const*	sourceFilesEnd,
													  FileReadStrategyFactory&	readStrategyFactory,
													  FileWriteStrategyFactory&	writeStrategyFactory ) const
		{
			typedef int	(*RegisterScriptAPIProc)( ::asIScriptEngine* const scriptEngine );

		// --

			ETUnreferencedParameter( writeStrategyFactory );

			if( ::asIScriptEngine* const scriptEngine = ::asCreateScriptEngine( ANGELSCRIPT_VERSION ) )
			{
				Eldritch2ScriptBuilder	scriptBuilder( scriptEngine,
													   readStrategyFactory );
				ErrorCode				result( Error::UNSPECIFIED );

#if( ET_PLATFORM_WINDOWS )
				const ::HMODULE			engineLibraryModule( ::LoadLibraryEx( SL( "Eldritch2.exe" ),
															 nullptr,
															 ( SEARCH_DLL_LOAD_DIR | SEARCH_APPLICATION_DIR ) ) );

				if( const RegisterScriptAPIProc	registerScriptAPI = reinterpret_cast<RegisterScriptAPIProc>( ::GetProcAddress( engineLibraryModule, "RegisterScriptAPI" ) ) )
				{
#endif
					scriptEngine->SetMessageCallback( asFUNCTION( ScriptCompilerCookerStrategy::MessageCallback ), const_cast<ScriptCompilerCookerStrategy*>( this ), ::asCALL_CDECL );
					scriptEngine->SetEngineProperty( ::asEP_INIT_GLOBAL_VARS_AFTER_BUILD, false );
					scriptEngine->SetEngineProperty( ::asEP_OPTIMIZE_BYTECODE, _optimizeBytecode );
					scriptEngine->SetEngineProperty( ::asEP_DISALLOW_GLOBAL_VARS, true );

#if( ET_PLATFORM_WINDOWS )
					registerScriptAPI( scriptEngine );
#endif

					if( sourceFilesEnd != find_if( sourceFiles, sourceFilesEnd, static_cast<Eldritch2ScriptBuilder&>( scriptBuilder ) ) &&
						::asSUCCESS <= scriptBuilder.BuildModule() )
					{
						result = scriptBuilder.SaveByteCode( output );
					}

					scriptEngine->Release();
				}

#if( ET_PLATFORM_WINDOWS )
				::FreeLibrary( engineLibraryModule );
#endif

				return result;
			}

			return Error::UNSPECIFIED;
		}

	// ---------------------------------------------------

		const SystemChar* ScriptCompilerCookerStrategy::GetCookedExtension() const
		{
			return SL( ".ASBC" );
		}

	// ---------------------------------------------------

		void ScriptCompilerCookerStrategy::MessageCallback( const ::asSMessageInfo* const				msg,
															const ScriptCompilerCookerStrategy* const	cooker )
		{
			ETUnreferencedParameter( cooker );

			static const char*	messageStringTable[] =
			{
				"Compilation error",
				"Compilation warning",
				"Message"
			};

			printf( "%s in file \'%s\', line %i, column %i: %s\r\n",
					messageStringTable[msg->type],
					msg->section,
					msg->row,
					msg->col,
					msg->message );
		}

	}	// namespace Tools

}	// namespace Eldritch2