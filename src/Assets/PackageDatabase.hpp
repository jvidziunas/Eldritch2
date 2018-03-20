/*==================================================================*\
  PackageDatabase.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Assets/Package.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Assets {
		class	AssetDatabase;
	}
}

namespace Eldritch2 {
namespace Assets {

	class PackageDatabase {
	// - TYPE PUBLISHING ---------------------------------

	public:
		struct PackageEqual {
			ETPureFunctionHint bool	operator()( const Package&, const Utf8Char* ) const;
			ETPureFunctionHint bool	operator()( const Package&, const Package& ) const;
		};

	// ---

	public:
		struct PackageHash {
			ETPureFunctionHint size_t	operator()( const Utf8Char*, size_t seed = 0u ) const;
			ETPureFunctionHint size_t	operator()( const Package&, size_t seed = 0u ) const;
		};

	public:
		using LoadFunction	= Function<void ( Package& )>;
		using LoadableSet	= CachingHashSet<Package, PackageHash, PackageEqual>;
		using GcCursor		= typename LoadableSet::Iterator;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		PackageDatabase( const PackageDatabase& ) = delete;
	//!	Constructs this @ref PackageDatabase instance.
		PackageDatabase();

		~PackageDatabase() = default;

	// - PACKAGE MANAGEMENT ------------------------------

	public:
	//! Signals to the @ref AssetDatabase that resources in the specified package will be needed in the near future.
	/*! @param[in] path A null-terminated UTF-8-encoded character sequence containing the path to the package file, without any suffix or extension.
		@remarks Thread-safe.
		@see @ref Package */
		CountedPointer<const Package>	RequirePackage( const Utf8Char* const path ) const;

	// ---------------------------------------------------

	public:
		void	SetPackages( LoadableSet packages, LoadFunction loadFunction );

	//!	Performs an incremental garbage collection pass on the set of unreferenced asset packages.
	/*!	@param[in] maxPackagesToDestroy @parblock Maximum number of packages to be considered for garbage collection, in the range [0, UINT_MAX]. This value
			will be clamped to the number of actual packages known to the @ref PackageDatabase; the caller is not responsible for bounds checking. @endparblock
		@remarks @parblock Note that this function is *not* safe to be called simulaneously on multiple threads, however other methods that operate on the package set
			may be safely called while this function is being executed. @endparblock */
		void	DestroyGarbage( size_t maxPackagesToDestroy, AssetDatabase& assetDatabase );

	// ---------------------------------------------------

	//!	Disable copy assignment.
		PackageDatabase&	operator=( const PackageDatabase& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		mutable Mutex	_packagesMutex;
		LoadFunction	_loadFunction;
		LoadableSet		_packages;
	/*!	Current package under consideration for incremental garbage collection. The cursor works in circular fashion,
	 	traversing the known packages collection in small chunks during calls to @ref DestroyGarbage and looping back to
		the start of the collection once all packages have been considered. */
		GcCursor		_gcCursor;
	};

}	// namespace Assets
}	// namespace Eldritch2