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

namespace Eldritch2 { namespace Core {

	ETInlineHint ETForceInlineHint AssetApiBuilder::AssetApiBuilder() ETNoexceptHint : _factoryByExtension(MallocAllocator("Asset Database Factory Bucket Allocator")) {}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint AssetApiBuilder& AssetApiBuilder::DefineType(StringSpan extension, FactoryDelegate factory) {
		ETAssert(extension.StartsWith(ET_EXTENSION_DELIMITER), "Asset extension {} should start with delimiter {}", extension, ET_EXTENSION_DELIMITER);

		_factoryByExtension.Emplace(Move(extension), Move(factory));
		return *this;
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint AssetApiBuilder::FactoryMap& AssetApiBuilder::GetFactories() ETNoexceptHint {
		return _factoryByExtension;
	}

	// ---------------------------------------------------

	template <typename ConcreteAsset>
	ETConstexpr ETForceInlineHint ETPureFunctionHint AssetApiBuilder::FactoryDelegate AssetApiBuilder::MakeFactory() ETNoexceptHint {
		return [](Allocator& allocator, StringSpan path) ETNoexceptHint -> UniquePointer<Asset> {
			return MakeUnique<ConcreteAsset>(allocator, path);
		};
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint bool AbstractPackageClient::HasCompleted(MemoryOrder order) const ETNoexceptHint {
		return _remainingLoads.load(order) == 0u;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint bool AbstractPackageClient::HasFailed(MemoryOrder order) const ETNoexceptHint {
		return _failureCount.load(order) == 0u;
	}

	// ---------------------------------------------------

	ETForceInlineHint void AbstractPackageClient::InitiateLoad(ContentDatabase& database, StringSpan name) ETNoexceptHint {
		CountedPointer<Package> package;
		{
			ET_READ_LOCK_SCOPE(database._packagesMutex);
			const auto candidate(database._packages.Find(name, database._packages.GetHash(), database._packages.GetEqualityPredicate()));
			if (ET_UNLIKELY(candidate == database._packages.End())) {
				CompleteLoad(Result::InvalidPath, /*package =*/nullptr);
			}

			//	Use of const_cast is gross, but this is one of the suggested solutions in http://www.open-std.org/jtc1/sc22/wg21/docs/lwg-defects.html#103
			package = ETAddressOf(const_cast<Package&>(*candidate));
		} // End of lock scope.

		database.PushRequest({ this, Move(package) });
	}

	// ---------------------------------------------------

	ETForceInlineHint void AbstractPackageClient::CompleteLoad(Result result, CountedPointer<const Package> package) {
		if (ET_LIKELY(Succeeded(result))) {
			this->OnLoadSuccess(Move(package));
		} else {
			this->OnLoadFailure(result);
			_failureCount.fetch_add(1u, std::memory_order_release);
		}

		_remainingLoads.fetch_sub(1u, std::memory_order_release);
	}

	// ---------------------------------------------------

	ETInlineHint void AbstractPackageClient::OnLoadFailure(Result /*result*/) {}

	// ---------------------------------------------------

	ETForceInlineHint AssetReference::AssetReference(const ContentDatabase& content, StringSpan path) ETNoexceptHint {
		ET_READ_LOCK_SCOPE(content._assetsMutex);
		const auto candidate(content._assets.Find(path, content._assets.GetHash(), content._assets.GetEqualityPredicate()));
		_asset = ET_LIKELY(candidate != content._assets.End()) ? candidate->Get() : nullptr;
		// End of lock scope.
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint AssetReference::operator bool() const ETNoexceptHint {
		return _asset != nullptr;
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint ETPureFunctionHint bool ContentDatabase::PackageEqual::operator()(const Package& lhs, const Package& rhs) const ETNoexceptHint {
		return lhs.GetPath() == rhs.GetPath();
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint ETPureFunctionHint bool ContentDatabase::PackageEqual::operator()(const Package& lhs, StringSpan rhs) const ETNoexceptHint {
		return lhs.GetPath() == rhs;
	}

	// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint bool ContentDatabase::AssetEqual::operator()(const UniquePointer<Asset>& asset, StringSpan path) const ETNoexceptHint {
		return asset->GetPath() == path;
	}

	// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint bool ContentDatabase::AssetEqual::operator()(const UniquePointer<Asset>& lhs, Package::AssetHandle rhs) const ETNoexceptHint {
		return lhs.Get() == rhs;
	}

	// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint bool ContentDatabase::AssetEqual::operator()(const UniquePointer<Asset>& lhs, const UniquePointer<Asset>& rhs) const ETNoexceptHint {
		return lhs->GetPath() == rhs->GetPath();
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint ETPureFunctionHint size_t ContentDatabase::PackageHash::operator()(const Package& package, size_t seed) const ETNoexceptHint {
		return GetHashCode(package.GetPath(), seed);
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint ETPureFunctionHint size_t ContentDatabase::PackageHash::operator()(StringSpan path, size_t seed) const ETNoexceptHint {
		return GetHashCode(path, seed);
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint ETPureFunctionHint size_t ContentDatabase::AssetHash::operator()(StringSpan path, size_t seed) const ETNoexceptHint {
		return GetHashCode(path, seed);
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint ETPureFunctionHint size_t ContentDatabase::AssetHash::operator()(Package::AssetHandle asset, size_t seed) const ETNoexceptHint {
		return GetHashCode(asset->GetPath(), seed);
	}

	// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint size_t ContentDatabase::AssetHash::operator()(const UniquePointer<Asset>& asset, size_t seed) const ETNoexceptHint {
		return GetHashCode(asset->GetPath(), seed);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void ContentDatabase::PushRequest(LoadRequest request) {
		ET_LOCK_SCOPE(_requestsMutex);
		_requests.Push(Move(request));
	} // End of lock scope.

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint bool ContentDatabase::PopRequest(LoadRequest& outRequest) {
		ET_LOCK_SCOPE(_requestsMutex);
		if (_requests) {
			_requests.Pop(outRequest);
			return true;
		}

		return false;
	} // End of lock scope.

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void ContentDatabase::CancelRequests(const AbstractPackageClient& client) {
		ET_LOCK_SCOPE(_requestsMutex);
		_requests.GetContainer().EraseIf([client = ETAddressOf(client)](const LoadRequest& request) { return request.source == client; });
	} // End of lock scope.

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void ContentDatabase::CancelAllRequests() {
		LoadRequest request;
		while (PopRequest(request)) {
			request.source->CompleteLoad(Result::Unspecified, Move(request.package));
		}
	}

	// ---------------------------------------------------

	template <typename PublicAsset>
	ETConstexpr ETForceInlineHint ETPureFunctionHint const PublicAsset* Get(Package::AssetHandle asset, FalseType /*isChecked*/) ETNoexceptHint {
		return static_cast<const PublicAsset*>(asset);
	}

	// ---------------------------------------------------

	template <typename PublicAsset>
	ETConstexpr ETForceInlineHint ETPureFunctionHint const PublicAsset* Get(Package::AssetHandle asset, TrueType /*isChecked*/) ETNoexceptHint {
		return asset && asset->GetPath().EndsWith(PublicAsset::GetExtension()) ? Get<PublicAsset>(asset, FalseType()) : nullptr;
	}

	// ---------------------------------------------------

	template <typename PublicAsset, bool isChecked = true>
	ETConstexpr ETForceInlineHint const PublicAsset* Get(const AssetReference& asset) ETNoexceptHint {
		return Get<PublicAsset>(asset._asset, IntegralConstant<bool, isChecked>());
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void Swap(AssetReference& lhs, AssetReference& rhs) ETNoexceptHint {
		using ::Eldritch2::Swap;

		Swap(lhs._asset, rhs._asset);
	}

}} // namespace Eldritch2::Core
