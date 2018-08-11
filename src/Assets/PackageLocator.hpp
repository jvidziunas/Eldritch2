/*==================================================================*\
  PackageLocator.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Assets/Package.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Assets {
	class PackageDatabase;
}} // namespace Eldritch2::Assets

namespace Eldritch2 { namespace Assets {

	class AsynchronousImport {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using PackageList = ArrayList<CountedPointer<const Package>, MallocAllocator>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref AsynchronousImport instance.
		AsynchronousImport(const AsynchronousImport&) = default;
		//!	Constructs this @ref AsynchronousImport instance.
		AsynchronousImport(PackageList packages);

		~AsynchronousImport() = default;

		// ---------------------------------------------------

	public:
		bool IsComplete() const;

		// - DATA MEMBERS ------------------------------------

	private:
		PackageList _packages;
	};

	// ---

	class PackageLocator {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using PackageList = AsynchronousImport::PackageList;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref PackageLocator instance.
		PackageLocator(PackageDatabase& packages);
		//!	Disable copy construction.
		PackageLocator(const PackageLocator&) = delete;

		~PackageLocator() = default;

		// ---------------------------------------------------

	public:
		void PollLoadStatus();

		// ---------------------------------------------------

	public:
		ErrorCode RequireAsynchronous(StringView<Utf8Char> path);

		ErrorCode Require(StringView<Utf8Char> path);

		// ---------------------------------------------------

	public:
		ErrorCode BindResources();

		void FreeResources();

		// - DATA MEMBERS ------------------------------------

	private:
		PackageDatabase* _packages;
		PackageList      _imports;
	};

}} // namespace Eldritch2::Assets
