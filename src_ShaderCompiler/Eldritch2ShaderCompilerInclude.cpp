/*==================================================================*\
  Eldritch2ShaderCompilerInclude.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Eldritch2ShaderCompilerInclude.hpp>
#include <Tools/FileReadStrategyFactory.hpp>
#include <Utility/FileReadAccessStrategy.hpp>
#include <Utility/Memory/MemStdLib.hpp>
#include <Utility/ErrorCode.hpp>
#include <Shlwapi.h>
#include <tchar.h>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_LIBRARY( "Shlwapi.lib" )
//------------------------------------------------------------------//

using namespace ::Eldritch2;
using namespace ::Eldritch2::Renderer;
using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Tools;
using namespace ::std;

namespace
{

	static const UTF8Char	fileAllocatorName[]	= UTF8L( "Eldritch2 Shader Compiler Include Handler Allocator" );

}	// anonymous namespace

namespace Eldritch2
{

	namespace Renderer
	{


		Eldritch2ShaderCompilerInclude::Eldritch2ShaderCompilerInclude( const SystemChar*			rootFile,
																		FileReadStrategyFactory&	fileStrategyFactory ) : _fileStrategyFactory( fileStrategyFactory )
		{
			_directoryTable[nullptr].assign( rootFile, ::PathFindFileName( rootFile ) );
		}

	// ---------------------------------------------------

		Eldritch2ShaderCompilerInclude::~Eldritch2ShaderCompilerInclude()
		{
		}

	// ---------------------------------------------------

		::HRESULT Eldritch2ShaderCompilerInclude::Open( ::D3D_INCLUDE_TYPE	type,
														::LPCSTR			fileName,
														::LPCVOID			parentData,
														::LPCVOID*			data,
														::UINT*				dataSize )
		{
			typedef FileReadAccessStrategy::BlockingResult	ReadResult;

		// ---

			SystemString	absoluteFileName( D3D_INCLUDE_LOCAL == type ? GetFilePath( fileName, parentData ) : GetFilePath( fileName ) );
			::HRESULT		result( E_FAIL );

			if( FileReadAccessStrategy* const reader = _fileStrategyFactory.GetAccessor( absoluteFileName.c_str() ) )
			{
				const size_t		fileSize( reader->GetRoundedSizeInBytes() );
				void* const			fileData( _fileStrategyFactory.GetFileDataAllocator().Allocate( fileSize,
																									reader->GetPhysicalMediaSectorSizeInBytes() ) );
				const ReadResult	readResult( reader->Read( fileData, fileSize ) );
			
				if( readResult.result )
				{
					*dataSize	= readResult.readAmountInBytes;
					*data		= fileData;

					_directoryTable[fileData].assign( absoluteFileName.c_str(),
													  ::PathFindFileName( absoluteFileName.c_str() ) );

					result = S_OK;
				}
				else
				{
					_fileStrategyFactory.GetFileDataAllocator().DeallocateAligned( fileData );
				}

				_fileStrategyFactory.DestroyAccessor( *reader );
			}

			return result;
		}

	// ---------------------------------------------------

		::HRESULT Eldritch2ShaderCompilerInclude::Close( ::LPCVOID pData )
		{
			_directoryTable.erase( pData );
			_fileStrategyFactory.GetFileDataAllocator().DeallocateAligned( const_cast<::LPVOID>( pData ) );

			return S_OK;
		}

	// ---------------------------------------------------

		Eldritch2ShaderCompilerInclude::SystemString Eldritch2ShaderCompilerInclude::GetFilePath( ::LPCSTR	fileName,
																								  ::LPCVOID	parentData )
		{
			return SystemString( _directoryTable[parentData] ).append( fileName, FindEndOfString( fileName ) );
		}

	// ---------------------------------------------------

		Eldritch2ShaderCompilerInclude::SystemString Eldritch2ShaderCompilerInclude::GetFilePath( ::LPCSTR fileName )
		{
			SystemChar			basePath[MAX_PATH]	= { SL( '\0' ) };
			const SystemChar*	extraDirs[]			= { _directoryTable[nullptr].c_str(), nullptr };

			::PathFindOnPath( PrintFormatted( basePath,
											  ETIsBuildUnicode() ? SL( "%S" ) : SL( "%s" ),
											  fileName ),
											  extraDirs );

			return SystemString( basePath, FindEndOfString( basePath ) );
		}

	}	// namespace Renderer

}	// namespace Eldritch2