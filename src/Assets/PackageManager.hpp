/*==================================================================*\
  PackageManager.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Assets/PackageDatabase.hpp>
#include <Assets/Package.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Assets {

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
			bool	IsComplete() const;

		// - DATA MEMBERS ------------------------------------

		private:
			PackageList	_packages;
		};

	// ---

		class PackageManager {
		// - TYPE PUBLISHING ---------------------------------

		public:
			using PackageList = AsynchronousImport::PackageList;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref PackageManager instance.
			PackageManager(PackageDatabase& packages);
		//!	Disable copy construction.
			PackageManager(const PackageManager&) = delete;

			~PackageManager() = default;

		// ---------------------------------------------------

		public:
			void	ProcessTick();

		// ---------------------------------------------------

		public:
			CountedPointer<const Package>	Load(const Utf8Char* path);

			bool							Require(const Utf8Char* path);

		// ---------------------------------------------------

		public:
			ErrorCode	BindResources();

			void		FreeResources();

		// - DATA MEMBERS ------------------------------------

		private:
		//!	Package database that will satisfy content requests made by the scripting system.
			PackageDatabase * _packages;
		//!	List of garbage collection roots for this world.
			PackageList			_imports;
		};

	}	// namespace Assets
}	// namespace Eldritch2