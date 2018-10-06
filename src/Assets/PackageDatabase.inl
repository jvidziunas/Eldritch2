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

	ETInlineHint ETForceInlineHint ETPureFunctionHint bool PackageDatabase::PackageEqual::operator()(const Package& lhs, const Package& rhs) const ETNoexceptHint {
		return (*this)(lhs, rhs.GetPath());
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint ETPureFunctionHint bool PackageDatabase::PackageEqual::operator()(const Package& lhs, StringView rhs) const ETNoexceptHint {
		return lhs.GetPath() == rhs;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint ETPureFunctionHint size_t PackageDatabase::PackageHash::operator()(const Package& package, size_t seed) const ETNoexceptHint {
		return (*this)(package.GetPath(), seed);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint ETPureFunctionHint size_t PackageDatabase::PackageHash::operator()(StringView name, size_t seed) const ETNoexceptHint {
		return GetHashCode(name, seed);
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint AssetDatabase& PackageDatabase::GetAssetDatabase() ETNoexceptHint {
		return _assets;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void PackageDatabase::PushRequest(LoadRequest request) {
		Lock _(_requestsMutex);
		_requests.Push(eastl::move(request));
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint bool PackageDatabase::PopRequest(LoadRequest& outRequest) {
		Lock _(_requestsMutex);
		if (_requests.IsEmpty()) {
			return false;
		}

		outRequest = eastl::move(_requests.Top());
		_requests.Pop();
		return true;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void PackageDatabase::CancelClientRequests(const PackageLoadClient& client) {
		Lock _(_requestsMutex);
		_requests.GetContainer().EraseIf([client = ETAddressOf(client)](const LoadRequest& request) { return request.client == client; });
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void PackageDatabase::CancelAllRequests() {
		LoadRequest current;
		while (_requests) {
			current = eastl::move(_requests.Top());
			_requests.Pop();

			current.client->CompleteLoad(eastl::move(current.package), ErrorCode(Error::Unspecified));
		}
	}

}} // namespace Eldritch2::Assets
