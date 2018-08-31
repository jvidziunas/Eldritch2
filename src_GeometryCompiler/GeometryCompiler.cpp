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
#include <Common/ErrorCode.hpp>
#include <GeometryCompiler.hpp>
//------------------------------------------------------------------//
#include <fbxsdk.h>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_LIBRARY("libfbxsdk-md.lib")
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Tools {

	GeometryCompiler::GeometryCompiler() :
		_password(MallocAllocator("Password Allocator")),
		_sourcePaths(MallocAllocator("Input File Name Collection Allocator")) {
	}

	// ---------------------------------------------------

	void GeometryCompiler::RegisterOptions(OptionRegistrar& options) {
		options.Register<bool>(L"--skipTextureCoordinates", L"-t", _skipTextureCoordinates);
		options.Register<bool>(L"--skipOrientation", L"-o", _skipOrientation);
		options.Register(L"--password", L"-p", [this](StringView<PlatformChar> password) -> int {
			_password = password;

			return 0;
		});
		options.RegisterInputFileHandler([this](StringView<PlatformChar> path) -> int {
			_sourcePaths.Emplace(MallocAllocator("Input File Name Allocator"), path);

			return 0;
		});
	}

	// ---------------------------------------------------

	ErrorCode GeometryCompiler::ProcessMesh(StringView<PlatformChar> path) {
		FbxAutoDestroyPtr<FbxScene>    scene(FbxScene::Create(manager.Get(), ""));
		FbxAutoDestroyPtr<FbxImporter> importer(FbxImporter::Create(manager.Get(), ""));

		if (!scene || !importer) {
			return Error::Unspecified;
		}

		if (!importer->Initialize(path, -1, settings.Get()) || importer->Import(scene.Get())) {
			return Error::Unspecified;
		}
	}

	// ---------------------------------------------------

	ErrorCode GeometryCompiler::Process() {
		FbxAutoDestroyPtr<FbxManager>    manager(FbxManager::Create());
		FbxAutoDestroyPtr<FbxIOSettings> settings(FbxIOSettings::Create(manager.Get(), IOSROOT));

		if (!manager || !settings) {
			return Error::Unspecified;
		}

		//	Set the password for protected files, if applicable.
		if (_password) {
			settings->SetStringProp(IMP_FBX_PASSWORD, _password.AsCString());
			settings->SetBoolProp(IMP_FBX_PASSWORD_ENABLE, true);
		}

		for (const String<>& path : _sourcePaths) {
		}

		return 0;
	}

}} // namespace Eldritch2::Tools
