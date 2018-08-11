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

		// ---------------------------------------------------

	public:
		void ScanPackages();

		// ---------------------------------------------------

	public:
		ErrorCode BindResources();

		void FreeResources();

		// ---------------------------------------------------

	private:
		ErrorCode DeserializeAssets(Package& package);

		void SatisfyLoad();

		// - DATA MEMBERS ------------------------------------

	private:
		mutable Logging::FileLog _log;
		Assets::AssetDatabase    _assetDatabase;
		Assets::PackageDatabase  _packageDatabase;
	};

}} // namespace Eldritch2::Assets

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Assets/FlatBufferPackageProvider.inl>
//------------------------------------------------------------------//
