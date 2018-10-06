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
//------------------------------------------------------------------//

namespace Eldritch2 { namespace FlatBuffers {

	class FlatBufferPackageProvider : public Thread {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		FlatBufferPackageProvider(const FlatBufferPackageProvider&) = delete;
		//!	Constructs this @ref FlatBufferPackageProvider instance.
		FlatBufferPackageProvider();

		~FlatBufferPackageProvider() = default;

		// ---------------------------------------------------

	public:
		ETConstexpr Assets::PackageDatabase& GetPackageDatabase() ETNoexceptHint;

		ETConstexpr Assets::AssetDatabase& GetAssetDatabase() ETNoexceptHint;

		// ---------------------------------------------------

	public:
		void SetShouldShutDown() ETNoexceptHint override;

		bool ShouldRun(MemoryOrder order = std::memory_order_consume) const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		ErrorCode BindResources();

		void FreeResources();

		// ---------------------------------------------------

	private:
		ErrorCode EnterOnCaller() override;

		// - DATA MEMBERS ------------------------------------

	private:
		Assets::PackageDatabase _packageDatabase;
		Atomic<RunBehavior>     _runBehavior;
	};

}} // namespace Eldritch2::FlatBuffers

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <FlatBuffers/FlatBufferPackageProvider.inl>
//------------------------------------------------------------------//
