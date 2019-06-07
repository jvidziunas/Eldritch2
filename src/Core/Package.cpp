/*==================================================================*\
  Package.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Core/Package.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Core {

	Package::Package(StringSpan path) ETNoexceptHint : _referenceCount(0u), _readTime(FileTime::Unavailable), _assets(AssetList::AllocatorType("Package Asset List Allocator")) {
		path.Copy(_path, ETCountOf(_path));
	}

	// ---------------------------------------------------

	Package::Package(const Package& package) ETNoexceptHint : Package(package.GetPath()) {}

	// ---------------------------------------------------

	Package::~Package() {
		ETAssert(_assets.IsEmpty(), "Package {} contains assets! Call FreeAssets() before the package is destroyed!", GetPath());
	}

	// ---------------------------------------------------

	void Package::BindAssets(FileTime readTime, AssetList& assets) ETNoexceptHint {
		_readTime.store(readTime, std::memory_order_release);
		Swap(_assets, assets);
	}

}} // namespace Eldritch2::Assets
