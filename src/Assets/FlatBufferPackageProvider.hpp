/*==================================================================*\
  FlatBufferPackageProvider.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Assets/PackageDatabase.hpp>
#include <Assets/AssetDatabase.hpp>
#include <Logging/FileLog.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Assets {

	class FlatBufferPackageProvider {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		FlatBufferPackageProvider(const FlatBufferPackageProvider&) = delete;
		//!	Constructs this @ref FlatBufferPackageProvider instance.
		FlatBufferPackageProvider();

		~FlatBufferPackageProvider() = default;

		// ---------------------------------------------------

	public:
		Assets::PackageDatabase& GetPackageDatabase();

		Assets::AssetDatabase& GetAssetDatabase();

		FileSystem& GetFileSystem();

		// ---------------------------------------------------

	public:
		void ScanPackages();

		// ---------------------------------------------------

	public:
		ErrorCode BindResources();

		void FreeResources();

		// ---------------------------------------------------

	private:
		void Load(Package& package);

		// - DATA MEMBERS ------------------------------------

	private:
		mutable Logging::FileLog _log;
		FileSystem               _fileSystem;
		Assets::AssetDatabase    _assetDatabase;
		Assets::PackageDatabase  _packageDatabase;
	};

}} // namespace Eldritch2::Assets
