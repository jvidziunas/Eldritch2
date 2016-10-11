/*==================================================================*\
  AssetLibrary.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/HashMultiMap.hpp>
#include <Platform/UserReadWriteMutex.hpp>
#include <Utility/Containers/HashSet.hpp>
#include <Assets/AssetPackageLoader.hpp>
#include <Logging/FileAppenderLog.hpp>
#include <Utility/StringLiteral.hpp>
#include <Utility/UniquePointer.hpp>
#include <Assets/AssetPackage.hpp>
#include <Assets/AssetView.hpp>
#include <Utility/Result.hpp>
//------------------------------------------------------------------//
#include <EASTL/functional.h>
#include <functional>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Assets {
		class	AssetViewFactoryRegistrar;
		class	AssetPackageLoaderThread;
		class	LiveSet;
	}

	namespace Scheduling {
		class	JobFiber;
	}

	namespace Platform {
		class	ContentProvider;
	}

	class	TypeHandle;
}

namespace Eldritch2 {
namespace Assets {

	class AssetLibrary {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using AssetViewFactory	= std::function<Eldritch2::Result<Eldritch2::UniquePointer<Assets::AssetView>> ( Eldritch2::Allocator&					/*allocator*/,
																												 const AssetLibrary&					/*library*/,
																												 const Eldritch2::Utf8Char* const		/*name*/,
																												 const Eldritch2::Range<const char*>	/*rawBytes*/ )>;

		struct CollectionStatistics {
			size_t	destroyedPackageCount;
		};

		struct AssetViewHash {
			ETPureFunctionHint size_t	operator()( Eldritch2::Pair<const Eldritch2::Utf8Char* const, Eldritch2::TypeHandle> assetProperties ) const;
			ETPureFunctionHint size_t	operator()( const Eldritch2::UniquePointer<const Assets::AssetView>& assetView ) const;
			ETPureFunctionHint size_t	operator()( const Assets::AssetView* assetView ) const;
		};

		struct AssetViewEquals {
			ETPureFunctionHint size_t	operator()( const Assets::AssetView* assetView, Eldritch2::Pair<const Eldritch2::Utf8Char* const, Eldritch2::TypeHandle> assetProperties ) const;
			ETPureFunctionHint size_t	operator()( const Assets::AssetView* lhs, const Assets::AssetView* rhs ) const;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//! Constructs this @ref AssetLibrary instance.
	/*! @param[in] contentProvider @ref ContentProvider instance the new @ref AssetLibrary should use to create logs.
		@param[in] loader @ref AssetPackageLoader the new @ref AssetLibrary should use to load and deserialize packages.
		@param[in] allocator @ref Allocator instance this @ref AssetLibrary should use to perform internal allocations. */
		AssetLibrary( Platform::ContentProvider& contentProvider, Eldritch2::UniquePointer<Assets::AssetPackageLoader> loader, Eldritch2::Allocator& allocator );
	//! Disable copying.
		AssetLibrary( const AssetLibrary& ) = delete;

		~AssetLibrary() = default;

	// - PACKAGE RESOLUTION ------------------------------

	public:
	//! Signals to the @ref AssetLibrary that resources in the specified package will be needed in the near future.
	/*! @param[in] name A null-terminated UTF-8-encoded character sequence containing the name of the package file, without any suffix or extension.
		@returns A @ref Result containing either a handle to the @ref AssetPackage or an @ref ErrorCode indicating why a failure occurred.
		@remarks If the desired package is already resident in memory, then a reference to it is added instead.
		@remarks Thread-safe.
		@see @ref AssetPackage */
		Eldritch2::Result<const Assets::AssetPackage*>	ResolvePackage( const Eldritch2::Utf8Char* const name );

	// - ASSET VIEW RESOLUTION ---------------------------

	public:
	//!	Locates a resource view in the database.
	/*!	@param[in] name A null-terminated UTF-8-encoded character sequence containing the name of the resource to search for.
		@param[in] type Runtime type handle the resource is required to implement.
		@returns A pointer to the found view, if successful, or null in the event no views were found.
		@remarks Thread-safe. */
		const Assets::AssetView*	ResolveView( const Eldritch2::Utf8Char* const name, Eldritch2::TypeHandle type ) const;
	//!	Locates a resource view in the database.
	/*!	@param[in] name A null-terminated UTF-8-encoded character sequence containing the name of the resource to search for.
		@returns A pointer to the found view, if successful, or null in the event no views were found.
		@remarks Thread-safe. */
		template <typename View>
		const View*					ResolveView( const Eldritch2::Utf8Char* const name ) const;

	// ---------------------------------------------------

	public:
		CollectionStatistics	DestroyUnreferencedPackages( const Assets::LiveSet& livePackages );

	// ---------------------------------------------------

	public:
		Logging::Log&	GetLog() const;

	// ---------------------------------------------------

	public:
		void	CreateViews( Assets::AssetPackage& package, const Eldritch2::Utf8Char* const name, const Eldritch2::Utf8Char* const typeName, Eldritch2::Range<const char*> rawBytes );

		void	Unregister( const Assets::AssetView& assetView );

	// ---------------------------------------------------

	//!	Disable assignment.
		AssetLibrary&	operator=( const AssetLibrary& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		mutable Eldritch2::ChildAllocator												_allocator;
	//!	Mutable so we can write to the log even if we only have a const reference to the @ref AssetLibrary.
		mutable Logging::FileAppenderLog												_log;

	//! User-space mutex guarding the global asset package library. _Not_ responsible for protecting the actual asset views.
		mutable Platform::UserReadWriteMutex											_packagesMutex;
		Eldritch2::HashSet<Assets::AssetPackage>										_packages;

	//! User-space mutex guarding the global asset view library. _Not_ responsible for protecting the packages that own the views.
		mutable Platform::UserReadWriteMutex											_assetViewsMutex;
		Eldritch2::HashSet<const Assets::AssetView*, AssetViewHash, AssetViewEquals>	_assetViews;

	//!	Collection of handlers that will allocate concrete @ref AssetView instances given metadata and initialization byte streams.
		Eldritch2::HashMultiMap<Eldritch2::Utf8Literal, AssetViewFactory>				_factories;

		Eldritch2::UniquePointer<Assets::AssetPackageLoader>							_loader;

	// - FRIEND CLASS DECLARATION ------------------------

		friend class Eldritch2::Assets::AssetViewFactoryRegistrar;
	};

}	// namespace Assets
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Assets/AssetLibrary.inl>
//------------------------------------------------------------------//