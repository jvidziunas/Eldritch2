/*==================================================================*\
  ShaderCompilerCookerStrategy.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <ShaderCompilerCookerStrategy.hpp>
#include <Eldritch2ShaderCompilerInclude.hpp>
#include <Tools/FileReadStrategyFactory.hpp>
#include <Utility/Memory/MemStdLib.hpp>
#include <Utility/Math/MathUtils.hpp>
#include <Utility/FileWriteAccessStrategy.hpp>
#include <Utility/FileReadAccessStrategy.hpp>
#include <Utility/COMPointer.hpp>
#include <Utility/ErrorCode.hpp>
#include <Utility/Assert.hpp>
#ifndef WIN32_LEAN_AND_MEAN
#	define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#	define NOMINMAX
#endif
#include <D3DCompiler.h>
#include <tchar.h>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_LIBRARY( "D3DCompiler.lib" )
ET_LINK_LIBRARY( "dxguid.lib" )
//------------------------------------------------------------------//

using namespace ::Eldritch2;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2::Tools;
using namespace ::Eldritch2::Renderer;
using namespace ::Eldritch2::FileSystem;
using namespace ::std;

namespace
{
	static ShaderCompilerCookerStrategy::OptimizationLevelOption	optimizationLevelOption;
	static ShaderCompilerCookerStrategy::WarningsAreErrorsOption	warningsAreErrorsOption;
	static ShaderCompilerCookerStrategy::StripDebugInfoOption		stripDebugInfoOption;
	static ShaderCompilerCookerStrategy::ShaderProfileOption		shaderProfileOption;

// ---------------------------------------------------

	static const ShaderCompilerCookerStrategy::ShaderProfileOption::EnumValueDescriptor	shaderProfileDescriptorTable[] =
	{
		{
			SL( "4_0_level_9_1" ),
			ShaderCompilerCookerStrategy::_4_0_level_9_1
		},
		{
			SL( "4_0_level_9_3" ),
			ShaderCompilerCookerStrategy::_4_0_level_9_3
		},
		{
			SL( "4_0" ),
			ShaderCompilerCookerStrategy::_4_0
		},
		{
			SL( "4_1" ),
			ShaderCompilerCookerStrategy::_4_1
		},
		{
			SL( "5_0" ),
			ShaderCompilerCookerStrategy::_5_0
		}
	};

	static const ::UINT	optimizationLevelTable[] =
	{
		D3DCOMPILE_OPTIMIZATION_LEVEL0,
		D3DCOMPILE_OPTIMIZATION_LEVEL1,
		D3DCOMPILE_OPTIMIZATION_LEVEL2,
		D3DCOMPILE_OPTIMIZATION_LEVEL3
	};

	static const char* entryPointNames[] =
	{
		"VSMain",
		"HSMain",
		"DSMain",
		"GSMain",
		"PSMain",
		"CSMain"
	};

	static const char* shaderClassPrefixTable[] =
	{
		"",
		"vs",
		"hs",
		"ds",
		"gs",
		"ps",
		"cs"
	};

	static const char* shaderProfileSuffixTable[] =
	{
		"4_0_level_9_1",
		"4_0_level_9_3",
		"4_0",
		"4_1",
		"5_0"
	};

	static const SystemChar* shaderClassSourceExtensionTable[] =
	{
		SL( "vshader" ),
		SL( "hshader" ),
		SL( "dshader" ),
		SL( "gshader" ),
		SL( "pshader" ),
		SL( "cshader" )
	};

	static const ShaderCompilerCookerStrategy::ShaderClass	shaderClassTable[] =
	{
		ShaderCompilerCookerStrategy::VERTEX_SHADER,
		ShaderCompilerCookerStrategy::HULL_SHADER,
		ShaderCompilerCookerStrategy::DOMAIN_SHADER,
		ShaderCompilerCookerStrategy::GEOMETRY_SHADER,
		ShaderCompilerCookerStrategy::PIXEL_SHADER,
		ShaderCompilerCookerStrategy::COMPUTE_SHADER,
		ShaderCompilerCookerStrategy::UNKNOWN
	};

	static CookerOption<ShaderCompilerCookerStrategy>*	optionsTable[] =
	{
		&optimizationLevelOption,
		&warningsAreErrorsOption,
		&stripDebugInfoOption,
		&shaderProfileOption,
	};

}	// anonymous namespace

namespace Eldritch2
{

	namespace Tools
	{

		ShaderCompilerCookerStrategy::ShaderProfileOption::ShaderProfileOption() : BaseType( &ShaderCompilerCookerStrategy::_shaderProfile,
																							 shaderProfileDescriptorTable,
																							 shaderProfileDescriptorTable + _countof( shaderProfileDescriptorTable ) )
		{
		}

	// ---------------------------------------------------

		ShaderCompilerCookerStrategy::ShaderProfileOption::~ShaderProfileOption()
		{
		}

	// ---------------------------------------------------

		const SystemChar* ShaderCompilerCookerStrategy::ShaderProfileOption::GetArgumentString() const
		{
			return SL( "-shaderProfile" );
		}

	// ---------------------------------------------------

		ShaderCompilerCookerStrategy::OptimizationLevelOption::OptimizationLevelOption() : BaseType( &ShaderCompilerCookerStrategy::_optimizationLevel )
		{
		}

	// ---------------------------------------------------

		ShaderCompilerCookerStrategy::OptimizationLevelOption::~OptimizationLevelOption()
		{
		}

	// ---------------------------------------------------

		const SystemChar* ShaderCompilerCookerStrategy::OptimizationLevelOption::GetArgumentString() const
		{
			return SL( "-optimizationLevel" );
		}

	// ---------------------------------------------------

		uint32 ShaderCompilerCookerStrategy::OptimizationLevelOption::CoerceToValidValue( uint32 value ) const
		{
			return Min( value, static_cast<uint32>( _countof( optimizationLevelTable ) ) );
		}

	// ---------------------------------------------------

		ShaderCompilerCookerStrategy::StripDebugInfoOption::StripDebugInfoOption() : BaseType( &ShaderCompilerCookerStrategy::_stripDebugInfo )
		{
		}

	// ---------------------------------------------------

		ShaderCompilerCookerStrategy::StripDebugInfoOption::~StripDebugInfoOption()
		{
		}

	// ---------------------------------------------------

		const SystemChar* ShaderCompilerCookerStrategy::StripDebugInfoOption::GetArgumentString() const
		{
			return SL( "-stripDebugInfo" );
		}

	// ---------------------------------------------------

		ShaderCompilerCookerStrategy::WarningsAreErrorsOption::WarningsAreErrorsOption() : BaseType( &ShaderCompilerCookerStrategy::_warningsAreErrors )
		{
		}

	// ---------------------------------------------------

		ShaderCompilerCookerStrategy::WarningsAreErrorsOption::~WarningsAreErrorsOption()
		{
		}

	// ---------------------------------------------------

		const SystemChar* ShaderCompilerCookerStrategy::WarningsAreErrorsOption::GetArgumentString() const
		{
			return SL( "-warningsAreErrors" );
		}

	// ---------------------------------------------------

		ShaderCompilerCookerStrategy::ShaderCompilerCookerStrategy() : ContentCookerStrategy(),
																	   _optimizationLevel( _countof( optimizationLevelTable ) - 1u ),
																	   _shaderProfile( DEFAULT ),
																	   _shaderClass( UNKNOWN ),
																	   _warningsAreErrors( false ),
																	   _stripDebugInfo( true )
		{
		}

	// ---------------------------------------------------

		ShaderCompilerCookerStrategy::~ShaderCompilerCookerStrategy()
		{
		}

	// ---------------------------------------------------

		ShaderCompilerCookerStrategy::OptionIterator ShaderCompilerCookerStrategy::OptionsBegin()
		{
			return optionsTable;
		}

	// ---------------------------------------------------

		ShaderCompilerCookerStrategy::OptionIterator ShaderCompilerCookerStrategy::OptionsEnd()
		{
			return optionsTable + _countof( optionsTable );
		}

	// ---------------------------------------------------

		ErrorCode ShaderCompilerCookerStrategy::Cook( FileWriteAccessStrategy&	output,
													  const SystemChar* const*	sourceFiles,
													  const SystemChar* const*	sourceFilesEnd,
													  FileReadStrategyFactory&	readStrategyFactory,
													  FileWriteStrategyFactory&	writeStrategyFactory ) const
		{
			typedef FileReadAccessStrategy::BlockingResult	ReadResult;

		// ---

			if( FileReadAccessStrategy* const accessor = readStrategyFactory.GetAccessor( *sourceFiles ) )
			{
				Eldritch2ShaderCompilerInclude	include( *sourceFiles,
														 readStrategyFactory );
				COMPointer<::ID3DBlob>			strippedBlob;
				COMPointer<::ID3DBlob>			bytecodeBlob;
				COMPointer<::ID3DBlob>			errorBlob;
				void* const						fileBuffer( readStrategyFactory.GetFileDataAllocator().Allocate( accessor->GetRoundedSizeInBytes(),
																												 accessor->GetPhysicalMediaSectorSizeInBytes() ) );
				const ReadResult				readResult( accessor->Read( fileBuffer,
																			accessor->GetRoundedSizeInBytes() ) );
				::HRESULT						result( E_FAIL );
				char							profileString[18u];

				// We're done reading, no point in keeping this around...
				readStrategyFactory.DestroyAccessor( *accessor );

				if( readResult.result )
				{
					result = ::D3DCompile( fileBuffer,
										   readResult.readAmountInBytes,
										   string( *sourceFiles,
												   FindEndOfString( *sourceFiles ) ).c_str(),
										   nullptr,
										   &include,
										   entryPointNames[_shaderClass],
										   PrintFormatted( profileString,
														   "%s_%s",
														   _shaderClass,
														   _shaderProfile ),
										   optimizationLevelTable[_optimizationLevel] | ( _warningsAreErrors ? D3DCOMPILE_WARNINGS_ARE_ERRORS : 0u ),
										   0u,
										   bytecodeBlob,
										   errorBlob );
				}

				readStrategyFactory.GetFileDataAllocator().DeallocateAligned( fileBuffer );

				if( SUCCEEDED( result ) && SUCCEEDED( ::D3DStripShader( bytecodeBlob->GetBufferPointer(),
																		bytecodeBlob->GetBufferSize(),
																		_stripDebugInfo ? D3DCOMPILER_STRIP_DEBUG_INFO : 0u,
																		strippedBlob ) ) )
				{
					return output.Write( strippedBlob->GetBufferPointer(),
										 strippedBlob->GetBufferSize() ).result;
				}
			}

			return Errors::UNSPECIFIED;
		}

	// ---------------------------------------------------

		void ShaderCompilerCookerStrategy::ProcessContentExtensionHint( const SystemChar* contentPath, const SystemChar* contentExtension )
		{
			class IsSameExtensionPredicate
			{
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

			public:
				ETInlineHint IsSameExtensionPredicate( const SystemChar* const extension ) : _extension( extension )
				{
				}

			// ---------------------------------------------------

				ETInlineHint bool operator()( const SystemChar* extension ) const
				{
					return EqualityCompareStringCaseInsensitive( extension, _extension );
				}

			// - DATA MEMBERS ------------------------------------

			private:
				const SystemChar* const	_extension;
			};

		// ---

			ETUnreferencedParameter( contentPath );

			_shaderClass = shaderClassTable[distance( shaderClassSourceExtensionTable,
													  find_if( shaderClassSourceExtensionTable,
															   shaderClassSourceExtensionTable + _countof( shaderClassSourceExtensionTable ),
															   IsSameExtensionPredicate( contentExtension ) ) )];
		}

	// ---------------------------------------------------

		const SystemChar* ShaderCompilerCookerStrategy::GetCookedExtension() const
		{
			static const SystemChar* classExtensionTable[] =
			{
				SL( "" ),
				SL( ".DXVS" ),
				SL( ".DXHS" ),
				SL( ".DXDS" ),
				SL( ".DXGS" ),
				SL( ".DXPS" ),
				SL( ".DXCS" )
			};

			return classExtensionTable[_shaderClass];
		}

	}	// namespace Tools

}	// namespace Eldritch2