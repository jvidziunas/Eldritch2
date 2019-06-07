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
#include <GeometryCompiler.hpp>
#include <Common/FileView.hpp>
#include <Common/Result.hpp>
//------------------------------------------------------------------//
#include <fbxsdk.h>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_LIBRARY("libfbxsdk-md.lib")
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Tools {

	using namespace ::Eldritch2::Logging;

	// ---------------------------------------------------

	GeometryCompiler::GeometryCompiler() ETNoexceptHint : _meshes(MallocAllocator("Input File Name Collection Allocator")), _password(MallocAllocator("Password Allocator")) {}

	// ---------------------------------------------------

	void GeometryCompiler::RegisterOptions(OptionRegistrar& options) {
		options.Register<bool>(SL("--skipTextureCoordinates"), SL("-t"), _skipTextureCoordinates);
		options.Register<bool>(SL("--skipOrientation"), SL("-o"), _skipOrientation);
		options.Register(SL("--password"), SL("-p"), [this](PlatformStringSpan password) -> int {
			_password = password;

			return 0;
		});
		options.RegisterInputFileHandler([this](Log&, PlatformStringSpan path) -> int {
			_meshes.Emplace(MallocAllocator("Input File Name Allocator"), path);

			return 0;
		});
	}

	// ---------------------------------------------------

	Result GeometryCompiler::ProcessMesh(Log& log, MeshList::Reference sourceMesh) {
		FbxAutoDestroyPtr<FbxImporter> importer(FbxImporter::Create(manager.Get(), ""));
		FbxAutoDestroyPtr<FbxScene>    scene(FbxScene::Create(manager.Get(), ""));

		ET_ABORT_UNLESS(scene != nullptr && importer != nullptr ? Result::Success : Result::Unspecified);

		if (!importer->Initialize(path, -1, settings.Get()) || importer->Import(scene.Get())) {
			return Error::Unspecified;
		}
	}

	// ---------------------------------------------------

	Result GeometryCompiler::Process(Log& log) {
		FbxAutoDestroyPtr<FbxManager>    manager(FbxManager::Create());
		FbxAutoDestroyPtr<FbxIOSettings> settings(FbxIOSettings::Create(manager.Get(), IOSROOT));

		if (!manager || !settings) {
			return Result::Unspecified;
		}

		//	Set the password for protected files, if applicable.
		if (_password) {
			settings->SetStringProp(IMP_FBX_PASSWORD, _password.AsCString());
			settings->SetBoolProp(IMP_FBX_PASSWORD_ENABLE, true);
		}

		for (MeshList::Reference sourceMesh : _meshes) {
			ET_ABORT_UNLESS(ProcessMesh(log, sourceMesh));
		}

		return Result::Success;
	}

}} // namespace Eldritch2::Tools
