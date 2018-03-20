/*==================================================================*\
  GeometryCompiler.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/MappedFile.hpp>
#include <GeometryCompiler.hpp>
//------------------------------------------------------------------//
#include <fbxsdk.h>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_LIBRARY( "libfbxsdk-md.lib" )
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Tools {

	GeometryCompiler::GeometryCompiler(
	) : _password( MallocAllocator( "Password Allocator" ) ),
		_sourcePaths( MallocAllocator( "Input File Name Collection Allocator" ) ) {
	}

// ---------------------------------------------------

	void GeometryCompiler::RegisterOptions( OptionRegistrar& options ) {
		options.Register<bool>( "--skipTextureCoordinates", "-t", _skipTextureCoordinates );
		options.Register<bool>( "--skipOrientation",        "-o", _skipOrientation );
		options.Register(       "--password",               "-p", [this] ( Range<const Utf8Char*> password ) {
			_password.Assign( password.Begin(), password.End() );

			return 0;
		} );
		options.RegisterInputFileHandler( [this] ( Range<const Utf8Char*> path ) {
			_sourcePaths.Insert( { path.Begin(), path.End(), MallocAllocator( "Input File Name Allocator" ) } );

			return 0;
		} );
	}

// ---------------------------------------------------

	int GeometryCompiler::Process() {
		FbxAutoDestroyPtr<FbxManager>		manager( FbxManager::Create() );
		FbxAutoDestroyPtr<FbxIOSettings>	settings( FbxIOSettings::Create( manager.Get(), IOSROOT ) );

		if (!manager || !settings) {
			return -1;
		}

	//	Set the password for protected files, if applicable.
		if (_password) {
			settings->SetStringProp( IMP_FBX_PASSWORD, _password.AsCString() );
			settings->SetBoolProp( IMP_FBX_PASSWORD_ENABLE, true );
		}

		for (const String<>& path : _sourcePaths) {
			FbxAutoDestroyPtr<FbxScene>		scene( FbxScene::Create( manager.Get(), "" ) );
			FbxAutoDestroyPtr<FbxImporter>	importer( FbxImporter::Create( manager.Get(), "" ) );

			if (!scene || !importer) {
				continue;
			}

			if (!importer->Initialize( path, -1, settings.Get() ) || importer->Import( scene.Get() )) {
				continue;
			}
		}

		return 0;
	}

}	// namespace Tools
}	// namespace Eldritch2