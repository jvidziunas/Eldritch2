/*==================================================================*\
  AssetPackage.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/ResizableArray.hpp>
#include <Utility/Containers/Utf8String.hpp>
#include <Utility/Containers/ArraySet.hpp>
#include <Utility/UniquePointer.hpp>
#include <Assets/AssetView.hpp>
//------------------------------------------------------------------//
#include <atomic>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Assets {
		class	AssetLibrary;
		class	AssetView;
	}
}

namespace Eldritch2 {
namespace Assets {

	enum class ResidencyState {
		Loading,	//!< The package and/or one of its dependencies are currently in the process of being paged into memory. The contents of the package may be available in the future.
		Published,	//!< The package, its dependencies and their contents have loaded fully.
		Failed,		//!< The package or one of its dependencies encountered a fatal error while loading. The package contents will never be available.
	};
	
// ---

	class AssetPackage {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//! Constructs this @ref AssetPackage instance.
	/*!	@param[in] name Null-terminated UTF-8 character sequence containing the name of the package.
		@param[in] allocator The @ref Allocator instance the @ref AssetPackage will use to perform internal allocations. */
		AssetPackage( const Eldritch2::Utf8Char* const name, Eldritch2::Allocator& allocator );
	//! Disable copy.
		AssetPackage( const AssetPackage& ) = delete;
	
		~AssetPackage() = default;

	// ---------------------------------------------------

	public:
		const Eldritch2::Utf8String<>&	GetName() const;

	// ---------------------------------------------------

	public:
		bool	ReferencesPackage( const AssetPackage& package ) const;

	// ---------------------------------------------------

	public:
		void	TakeOwnership( Eldritch2::UniquePointer<Assets::AssetView> assetView );

		void	AddDependency( const AssetPackage& package );

		void	NotifyOfLoadFailure( const AssetPackage& failedLoad );

	// ---------------------------------------------------

	public:
		template <typename UnaryPredicate>
		void	ForEachAssetView( UnaryPredicate&& predicate ) const;

		template <typename UnaryPredicate>
		void	ForEachDependency( UnaryPredicate&& predicate ) const;

	// ---------------------------------------------------

	public:
	//! Queries the current residency state of the package.
	/*!	@returns A @ref ResidencyState enum describing the state of the package. This can be used to check whether or not content is available, i.e. a world resource.
		@remarks Thread-safe.
		@see @ref ResidencyState */
		Assets::ResidencyState	GetResidencyState() const;

		bool					IsDeserializable() const;

	// ---------------------------------------------------

	//!	Disable assignment.
		AssetPackage&	operator=( const AssetPackage& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
	//!	Name of the package. Used for identity comparison between different @ref AssetPackage instances.
		Eldritch2::Utf8String<>													_name;

		std::atomic<Assets::ResidencyState>										_residencyState;

	//!	Assets for which this @ref AssetPackage relies on for proper instantiation of its constituent @ref AssetViews.
		Eldritch2::ArraySet<const AssetPackage*>								_dependencies;

	//!	Collection of @ref AssetView instances describing content contained within this @ref AssetPackage.
		Eldritch2::ResizableArray<Eldritch2::UniquePointer<Assets::AssetView>>	_assetViews;
	};

}	// namespace Assets
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Assets/AssetPackage.inl>
//------------------------------------------------------------------//