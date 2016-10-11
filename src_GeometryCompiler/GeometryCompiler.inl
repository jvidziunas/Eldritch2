/*==================================================================*\
  GeometryCompiler.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <FileSystem/ReadableMemoryMappedFile.hpp>
#include <fbxsdk.h>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_LIBRARY( "libfbxsdk-md.lib" )
//------------------------------------------------------------------//

namespace {



}	// anonymous namespace

namespace Eldritch2 {
namespace Tools {

	template <class GlobalAllocator, class FileAccessorFactory>
	GeometryCompiler<GlobalAllocator, FileAccessorFactory>::GeometryCompiler() : _allocator( UTF8L("Root Allocator") ),
																				 _password( { GetAllocator(), UTF8L("Password Allocator") } ),
																				 _inputNames( { GetAllocator(), UTF8L("Imported Package Name Collection Allocator") } ) {}

// ---------------------------------------------------

	template <class GlobalAllocator, class FileAccessorFactory>
	ETInlineHint FileAccessorFactory& GeometryCompiler<GlobalAllocator, FileAccessorFactory>::GetFileAccessorFactory() {
		return _fileAccessorFactory;
	}

// ---------------------------------------------------

	template <class GlobalAllocator, class FileAccessorFactory>
	ETInlineHint GlobalAllocator& GeometryCompiler<GlobalAllocator, FileAccessorFactory>::GetAllocator() {
		return _allocator;
	}

// ---------------------------------------------------

	template <class GlobalAllocator, class FileAccessorFactory>
	void GeometryCompiler<GlobalAllocator, FileAccessorFactory>::RegisterOptions( OptionRegistrationVisitor& visitor ) {
		using namespace ::std::placeholders;

	// ---

		visitor.AddTypedArgument<bool>(	UTF8L("--skipTextureCoordinates"),	UTF8L("-t"),	::std::bind( &GeometryCompiler::SetSkipTextureCoordinates, this, _1 ) );
		visitor.AddTypedArgument<bool>(	UTF8L("--skipOrientation"),			UTF8L("-o"),	::std::bind( &GeometryCompiler::SetSkipOrientation, this, _1 ) );
		visitor.AddArgument(			UTF8L("--password" ),				UTF8L("-p"),	::std::bind( &GeometryCompiler::SetPassword, this, _1, _2 ) );

		visitor.AddInputFileHandler( ::std::bind( &GeometryCompiler::AddInputFile, this, _1, _2 ) );
	}

// ---------------------------------------------------

	template <class GlobalAllocator, class FileAccessorFactory>
	int	GeometryCompiler<GlobalAllocator, FileAccessorFactory>::Process() {
		FbxAutoDestroyPtr<FbxManager>		manager( FbxManager::Create() );
		FbxAutoDestroyPtr<FbxIOSettings>	settings( FbxIOSettings::Create( manager.Get(), IOSROOT ) );

		if( !manager || !settings ) {
			return -1;
		}

		// Set the password for protected files, if applicable.
		if( _password ) {
			settings->SetStringProp( IMP_FBX_PASSWORD, _password.AsCString() );
			settings->SetBoolProp( IMP_FBX_PASSWORD_ENABLE, true );
		}

		for( const auto& inputName : _inputNames ) {
			FbxAutoDestroyPtr<FbxScene>		scene( FbxScene::Create( manager.Get(), "" ) );
			FbxAutoDestroyPtr<FbxImporter>	importer( FbxImporter::Create( manager.Get(), "" ) );

			if( !scene || !importer ) {
				continue;
			}

			if( !importer->Initialize( inputName.AsCString(), -1, settings.Get() ) || importer->Import( scene.Get() ) ) {
				continue;
			}
		}

		return 0;
	}

// ---------------------------------------------------

	template <class GlobalAllocator, class FileAccessorFactory>
	int GeometryCompiler<GlobalAllocator, FileAccessorFactory>::SetPassword( const Eldritch2::Utf8Char* const password, const Eldritch2::Utf8Char* const passwordEnd ) {
		_password.Assign( password, passwordEnd );

		return 0;
	}

// ---------------------------------------------------

	template <class GlobalAllocator, class FileAccessorFactory>
	int	GeometryCompiler<GlobalAllocator, FileAccessorFactory>::AddInputFile( const Eldritch2::Utf8Char* const name, const Eldritch2::Utf8Char* const nameEnd ) {
		_inputNames.PushBack( { name, nameEnd, { GetAllocator(), UTF8L("Input File Name Allocator") } } );

		return 0;
	}

// ---------------------------------------------------

	template <class GlobalAllocator, class FileAccessorFactory>
	int GeometryCompiler<GlobalAllocator, FileAccessorFactory>::SetSkipTextureCoordinates( const bool value ) {
		_skipTextureCoordinates = value;

		return 0;
	}

// ---------------------------------------------------

	template <class GlobalAllocator, class FileAccessorFactory>
	int GeometryCompiler<GlobalAllocator, FileAccessorFactory>::SetSkipOrientation( const bool value ) {
		_skipOrientation = value;

		return 0;
	}

}	// namespace Tools
}	// namespace Eldritch2