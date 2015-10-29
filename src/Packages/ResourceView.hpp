/*==================================================================*\
  ResourceView.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/IntrusiveForwardListHook.hpp>
#include <Utility/Containers/UTF8String.hpp>
#include <Utility/MPL/Noncopyable.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	class	ErrorCode;
	template <typename Iterator>
	class	Range;
}

namespace Eldritch2 {
namespace FileSystem {

	class ETPureAbstractHint ResourceView : public Utility::Noncopyable, public ::Eldritch2::IntrusiveForwardListBaseHook {
	// ---------------------------------------------------

	public:
		virtual ::Eldritch2::ErrorCode	UpdateFromByteStream( const ::Eldritch2::Range<const char*> bytes ) abstract;

	// ---------------------------------------------------

		//! Gets the path/unique identifier describing this resource.
		/*! @returns A const reference to a @ref UTF8String containing the name of the resource.
			@remarks While no two @ref ResourceView instances will refer to the same @ref UTF8String, note that it _is_ possible that two of the actual strings will compare equal to one another.
			@remarks Thread-safe.
			*/
		ETInlineHint const ::Eldritch2::UTF8String<>&	GetName() const;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		//! Constructs this @ref ResourceView instance.
		/*! @param[in] name Null-terminated C string containing the name of the resource the @ref ResourceView will be describing.
			@param[in] nameAllocator The @ref Allocator that the @ref ResourceView should use to allocate memory for the internal name copy.
			@remarks Designed to be called only from subclasses.
			*/
		ResourceView( const ::Eldritch2::UTF8Char* const name, ::Eldritch2::Allocator& nameAllocator );

	public:
		//! Destroys this @ref ResourceView instance.
		virtual ~ResourceView() = default;

	// - DATA MEMBERS ------------------------------------

	private:
		const ::Eldritch2::UTF8String<>	_name;
	};

}	// namespace FileSystem
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Packages/ResourceView.inl>
//------------------------------------------------------------------//