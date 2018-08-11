/*==================================================================*\
  PackageDatabase.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Assets {

	ETInlineHint ETPureFunctionHint bool PackageDatabase::PackageEqual::operator()(const Package& package, StringView<Utf8Char> name) const {
		return package.GetPath() == name;
	}

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint bool PackageDatabase::PackageEqual::operator()(const Package& package0, const Package& package1) const {
		return (*this)(package0, package1.GetPath());
	}

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint size_t PackageDatabase::PackageHash::operator()(const Package& package, size_t seed) const {
		return (*this)(package.GetPath(), seed);
	}

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint size_t PackageDatabase::PackageHash::operator()(StringView<Utf8Char> name, size_t seed) const {
		return GetHashCode(name, seed);
	}

	// ---------------------------------------------------

	ETInlineHint void PackageDatabase::PushRequest(LoadRequest request) {
		Lock _(_requestsMutex);

		_requests.Push(eastl::move(request));
	}

	// ---------------------------------------------------

	ETInlineHint bool PackageDatabase::PopRequest(LoadRequest& outRequest) {
		Lock _(_requestsMutex);

		if (_requests.IsEmpty()) {
			return false;
		}

		outRequest = eastl::move(_requests.Top());
		_requests.Pop();
		return true;
	}

}} // namespace Eldritch2::Assets
