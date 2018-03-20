/*==================================================================*\
  Package.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <atomic>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Assets {
		class	Asset;
	}
}

namespace Eldritch2 {
namespace Assets {

	class Package {
	// - TYPE PUBLISHING ---------------------------------

	public:
		enum : size_t { MaxPathLength = 128u };
		
		using AssetList = ArrayList<UniquePointer<Asset>>;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//! Constructs this @ref Package instance.
	/*!	@param[in] path Null-terminated, UTF-8-encoded character sequence containing the file system path to the package. */
		Package( const Utf8Char* const path );
	//! Disable copy construction.
		Package( const Package& ) = delete;
	
		~Package();

	// - REFERENCE MANAGEMENT ----------------------------

	public:
		size_t	AddReference( std::memory_order order = std::memory_order_relaxed ) const;

		size_t	ReleaseReference( std::memory_order order = std::memory_order_release ) const;

		bool	IsReferenced( std::memory_order order = std::memory_order_consume ) const;

	// ---------------------------------------------------

	public:
		const Utf8Char*	GetPath() const;

	// ---------------------------------------------------

	public:
		void		BindAssets( AssetList assets );

		AssetList	FreeAssets();

	// ---------------------------------------------------

	//!	Disable copy assignment.
		Package&	operator=( const Package& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
	/*!	File system path to the package. Used for identity comparison between different @ref Package instances;
		*DO NOT CHANGE* once the package has been registered with an @ref AssetDatabase. */
		Utf8Char					_path[MaxPathLength];
		mutable std::atomic<size_t>	_referenceCount;
		AssetList					_assets;
	};

}	// namespace Assets
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Assets/Package.inl>
//------------------------------------------------------------------//