/*==================================================================*\
  AssetView.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/CharTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Assets {

	class ETPureAbstractHint AssetView {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//! Constructs this @ref AssetView instance.
	/*!	@param[in] name UTF-8-encoded character sequence containing the name of the resource the @ref AssetView will describe.
		@remarks Designed to be called only from subclasses. */
		AssetView( const Eldritch2::Utf8Char* const name );
	//!	Disable copying.
		AssetView( const AssetView& ) = delete;
	
		virtual ~AssetView() = default;

	// ---------------------------------------------------

	public:
	//! Gets the path/unique identifier describing this resource.
	/*! @returns Null-terminated UTF-8-encoded character sequence containing the name of the resource this @ref AssetView describes.
		@remarks Thread-safe. */
		const Eldritch2::Utf8Char* const	GetName() const;

	// ---------------------------------------------------

	//!	Disable assignment.
		AssetView&	operator=( const AssetView& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		const Eldritch2::Utf8Char* const	_name;
	};

}	// namespace Assets
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Assets/AssetView.inl>
//------------------------------------------------------------------//