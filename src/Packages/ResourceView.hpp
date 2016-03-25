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
#include <Utility/MPL/Noncopyable.hpp>
#include <Utility/MPL/CharTypes.hpp>
//------------------------------------------------------------------//
#include <typeinfo>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace FileSystem {
		class	ContentLibrary;
		class	ContentPackage;
	}

	class	ErrorCode;
	template <typename Iterator>
	class	Range;
}

namespace Eldritch2 {
namespace FileSystem {

	class ETPureAbstractHint ResourceView : public Utility::Noncopyable, public ::Eldritch2::IntrusiveForwardListBaseHook {
	// ---------------------------------------------------

	public:
		virtual ::Eldritch2::ErrorCode	AttachToPackage( const ::Eldritch2::Range<const char*> bytes, FileSystem::ContentPackage& package, FileSystem::ContentLibrary& library ) abstract;

		virtual void					DetachFromPackage( FileSystem::ContentPackage& package, FileSystem::ContentLibrary& library ) const abstract;

	// ---------------------------------------------------

		//! Gets the path/unique identifier describing this resource.
		/*! @returns UTF-8-encoded character sequence containing the name of the resource this @ref ResourceView describes.
			@remarks Thread-safe.
			*/
		ETInlineHint const ::Eldritch2::UTF8Char* const	GetName() const;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		//! Constructs this @ref ResourceView instance.
		/*!	@param[in] name UTF-8-encoded character sequence containing the name of the resource the @ref ResourceView will be describing.
			@remarks Designed to be called only from subclasses.
			*/
		ResourceView( const ::Eldritch2::UTF8Char* const name );

	public:
		//! Destroys this @ref ResourceView instance.
		virtual ~ResourceView() = default;

	// ---------------------------------------------------

	protected:
		template <typename PublicType>
		const ResourceView&	PublishToLibraryAs( FileSystem::ContentLibrary& owningLibrary ) const;

		template <typename PublicType>
		const ResourceView&	RemoveFromLibraryAs( FileSystem::ContentLibrary& owningLibrary ) const;

	private:
		const ResourceView&	PublishToLibrary( FileSystem::ContentLibrary& owningLibrary, const ::std::type_info& publicType ) const;

		const ResourceView&	RemoveFromLibrary( FileSystem::ContentLibrary& owningLibrary, const ::std::type_info& publicType ) const;

	// - DATA MEMBERS ------------------------------------

		const ::Eldritch2::UTF8Char* const	_name;
	};

}	// namespace FileSystem
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Packages/ResourceView.inl>
//------------------------------------------------------------------//