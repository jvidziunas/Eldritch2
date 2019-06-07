/*==================================================================*\
  ContentLocator.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Core/ContentLocator.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Core {

	AsynchronousImport::AsynchronousImport() ETNoexceptHint : _packages(MallocAllocator("Asynchronous Import Loaded Package List Allocator")) {}

	// ---------------------------------------------------

	AsynchronousImport::~AsynchronousImport() {
		ETAssert(HasCompleted(), "Asynchronous import {} has pending load requests!", fmt::ptr(this));
	}

	// ---------------------------------------------------

	void AsynchronousImport::OnLoadSuccess(CountedPointer<const Package> package) ETNoexceptHint {
		ET_LOCK_SCOPE(_packagesMutex);
		_packages.Append(Move(package));
		// End of lock scope.
	}

	// ---------------------------------------------------

	Result ContentLocator::BindResources() {
		return Result::Success;
	}

	// ---------------------------------------------------

	void ContentLocator::FreeResources() ETNoexceptHint {}

}} // namespace Eldritch2::Assets
