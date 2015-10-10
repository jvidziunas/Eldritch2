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
#include <Utility/Containers/Range.hpp>
#include <Utility/MPL/Noncopyable.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace FileSystem {

	namespace FileSystem	= ::Eldritch2::FileSystem;
	namespace Utility		= ::Eldritch2::Utility;

// ---------------------------------------------------

	class ETPureAbstractHint ResourceView : public Utility::Noncopyable, public ::Eldritch2::IntrusiveForwardListBaseHook {
	// - TYPE PUBLISHING ---------------------------------

	public:
		struct Initializer {
			::Eldritch2::Range<const ::Eldritch2::UTF8Char*>	name;
			::Eldritch2::Range<const ::Eldritch2::UTF8Char*>	typeName;
			::Eldritch2::Range<const char*>						serializedAsset;
		};

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
		/*! @param[in] initializer @ref Initializer instance the @ResourceView should use to construct its members.
			@param[in] nameAllocator The @ref Allocator that the @ref ResourceView should use to allocate memory for the internal name copy.
			@remarks Designed to be called only from subclasses.
			*/
		ResourceView( const Initializer& initializer, ::Eldritch2::Allocator& nameAllocator );

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