/*==================================================================*\
  ContentPackage.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace FileSystem {

	ETInlineHint const ContentPackage::DependencyCollection& ContentPackage::GetDependencies() const {
		return _referencedPackages;
	}

// ---------------------------------------------------

	ETInlineHint ContentPackage::DependencyCollection& ContentPackage::GetDependencies() {
		return _referencedPackages;
	}

// ---------------------------------------------------

	ETInlineHint const ContentPackage::ExportCollection& ContentPackage::GetExports() const {
		return _exportedResources;
	}

// ---------------------------------------------------

	ETInlineHint ContentPackage::ExportCollection& ContentPackage::GetExports() {
		return _exportedResources;
	}

// ---------------------------------------------------

	ETInlineHint const FileSystem::ContentLibrary& ContentPackage::GetContentLibrary() const {
		return _owningLibrary;
	}

// ---------------------------------------------------

	ETInlineHint FileSystem::ContentLibrary& ContentPackage::GetContentLibrary() {
		return _owningLibrary;
	}

// ---------------------------------------------------

	ETInlineHint const ::Eldritch2::UTF8Char* const ContentPackage::GetName() const {
		return _name.AsCString();
	}

// ---------------------------------------------------

	ETInlineHint ContentPackage::ResidencyState	ContentPackage::GetResidencyState() const {
		return _residencyState.load( ::std::memory_order_acquire );
	}

// ---------------------------------------------------

	ETInlineHint::Eldritch2::Allocator& ContentPackage::GetAllocator() {
		return _allocator;
	}

// ---------------------------------------------------

	ETInlineHint void ContentPackage::UpdateResidencyStateOnLoaderThread( const ContentPackage::ResidencyState newState ) {
		_residencyState.store( newState, ::std::memory_order_release );
	}

}	// namespace FileSystem
}	// namespace Eldritch2