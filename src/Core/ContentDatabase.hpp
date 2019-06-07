/*==================================================================*\
  ContentDatabase.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Core/Package.hpp>
#include <Core/Asset.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Core {
	class ContentDatabase;
}} // namespace Eldritch2::Core

namespace Eldritch2 { namespace Core {

	class AssetApiBuilder {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using FactoryMap        = CachingHashMap<StringSpan, Function<UniquePointer<Asset>(Allocator& /*allocator*/, StringSpan /*path*/)>>;
		using FinalizerMap      = CachingHashMap<StringSpan, Function<void()>>;
		using FactoryDelegate   = FactoryMap::MappedType;
		using FinalizerDelegate = FinalizerMap::MappedType;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref AssetApiBuilder instance.
		AssetApiBuilder(const AssetApiBuilder&) = default;
		//!	Constructs this @ref AssetApiBuilder instance.
		AssetApiBuilder(AssetApiBuilder&&) ETNoexceptHint = default;
		//!	Constructs this @ref AssetApiBuilder instance.
		AssetApiBuilder() ETNoexceptHint;

		~AssetApiBuilder() = default;

		// ---------------------------------------------------
	public:
		//!	Registers an object creation handler for a specified class of asset object.
		/*!	@param[in] extension @ref Utf8Literal containing the extension of the asset to listen for. This string should include a
			leading dot/extension separator char.
			@returns A reference to *this for method chaining. */
		AssetApiBuilder& DefineType(StringSpan extension, FactoryDelegate factory);

		// ---------------------------------------------------

	public:
		ETConstexpr FactoryMap& GetFactories() ETNoexceptHint;

		// ---------------------------------------------------

	public:
		AssetApiBuilder& operator=(const AssetApiBuilder&) = default;
		AssetApiBuilder& operator=(AssetApiBuilder&&) ETNoexceptHint = default;

		// ---------------------------------------------------

	public:
		template <typename ConcreteAsset>
		static ETConstexpr ETPureFunctionHint FactoryDelegate MakeFactory() ETNoexceptHint;

		// - DATA MEMBERS ------------------------------------

	private:
		FactoryMap _factoryByExtension;
	};

	// ---

	class ETPureAbstractHint AbstractPackageClient {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using RequestCount = uint32;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		AbstractPackageClient(const AbstractPackageClient&) = delete;
		//!	Constructs this @ref PackageClient instance.
		AbstractPackageClient() ETNoexceptHint = default;

		~AbstractPackageClient();

		// ---------------------------------------------------

	public:
		bool HasCompleted(MemoryOrder order = std::memory_order_consume) const ETNoexceptHint;

		bool HasFailed(MemoryOrder order = std::memory_order_consume) const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		void InitiateLoad(ContentDatabase& database, StringSpan name) ETNoexceptHint;

		void CompleteLoad(Result result, CountedPointer<const Package> package);

		// ---------------------------------------------------

	protected:
		virtual void OnLoadSuccess(CountedPointer<const Package> package) abstract;

		virtual void OnLoadFailure(Result result);

		// - DATA MEMBERS ------------------------------------

	private:
		Atomic<RequestCount> _remainingLoads;
		Atomic<RequestCount> _failureCount;
	};

	// ---

	class AssetReference {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref AssetReference instance.
		ETConstexpr AssetReference(const AssetReference&) ETNoexceptHint = default;
		//!	Constructs this @ref AssetReference instance.
		AssetReference(const ContentDatabase&, StringSpan path) ETNoexceptHint;

		~AssetReference() = default;

		// ---------------------------------------------------

	public:
		ETConstexpr operator bool() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		ETConstexpr AssetReference& operator=(const AssetReference&) ETNoexceptHint = default;

		// - DATA MEMBERS ------------------------------------

	private:
		Package::AssetHandle _asset;

		// ---------------------------------------------------

		template <typename PublicAsset, bool isChecked>
		friend ETConstexpr const PublicAsset* Get(const AssetReference& asset) ETNoexceptHint;

		friend void Swap(AssetReference&, AssetReference&) ETNoexceptHint;
	};

	// ---

	class ContentDatabase {
		// - TYPE PUBLISHING ---------------------------------

	public:
		struct PackageEqual {
			ETConstexpr ETPureFunctionHint bool operator()(const Package&, const Package&) const ETNoexceptHint;
			ETConstexpr ETPureFunctionHint bool operator()(const Package&, StringSpan) const ETNoexceptHint;
		};

		// ---

		struct AssetEqual {
			ETPureFunctionHint bool operator()(const UniquePointer<Asset>&, const UniquePointer<Asset>&) const ETNoexceptHint;
			ETPureFunctionHint bool operator()(const UniquePointer<Asset>&, Package::AssetHandle) const ETNoexceptHint;
			ETPureFunctionHint bool operator()(const UniquePointer<Asset>&, StringSpan) const ETNoexceptHint;
		};

		// ---

		struct PackageHash {
			ETConstexpr ETPureFunctionHint size_t operator()(const Package&, size_t seed = 0u) const ETNoexceptHint;
			ETConstexpr ETPureFunctionHint size_t operator()(StringSpan, size_t seed = 0u) const ETNoexceptHint;
		};

		// ---

		struct AssetHash {
			ETPureFunctionHint size_t operator()(const UniquePointer<Asset>&, size_t seed = 0u) const ETNoexceptHint;
			ETConstexpr ETPureFunctionHint size_t operator()(Package::AssetHandle, size_t seed = 0u) const ETNoexceptHint;
			ETConstexpr ETPureFunctionHint size_t operator()(StringSpan, size_t seed = 0u) const ETNoexceptHint;
		};

		// ---

	public:
		struct LoadRequest {
			AbstractPackageClient*  source;
			CountedPointer<Package> package;
		};

		// ---

	public:
		using FactoryMap = AssetApiBuilder::FactoryMap;
		using PackageSet = CachingHashSet<Package, PackageHash, PackageEqual>;
		using AssetSet   = CachingHashSet<UniquePointer<Asset>, AssetHash, AssetEqual>;
		using GcCursor   = PackageSet::Iterator;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		ContentDatabase(const ContentDatabase&) = delete;
		//!	Constructs this @ref PackageDatabase instance.
		ContentDatabase() ETNoexceptHint;

		~ContentDatabase() = default;

		// ---------------------------------------------------

	public:
		//!	Performs an incremental garbage collection pass on known content packages.
		/*!	@remarks @parblock Note that this function is *not* safe to be called simulaneously on multiple threads, however other methods that operate on the package set
				may safely be called while this function is being executed. @endparblock */
		void DestroyGarbage(PackageSet::SizeType limit);

		// ---------------------------------------------------

	public:
		Package::AssetHandle Insert(Logging::Log& log, StringSpan path, const AssetBuilder& builder);

		// - PACKAGE MANAGEMENT ------------------------------

	public:
		bool PopRequest(LoadRequest& outRequest);

		void CancelRequests(const AbstractPackageClient& client);

		// ---------------------------------------------------

	public:
		Result BindResources(AssetApiBuilder api, PackageSet packages);

		void FreeResources() ETNoexceptHint;

		// ---------------------------------------------------

	private:
		void PushRequest(LoadRequest request);

		void CancelAllRequests();

		// ---------------------------------------------------

	private:
		void DestroyAssets(Package::AssetList& assets);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		ContentDatabase& operator=(const ContentDatabase&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		mutable UsageMixin<MallocAllocator> _allocator;
		FactoryMap                          _factoryByExtension;
		ETCacheLineAligned mutable Mutex    _assetsMutex;
		AssetSet                            _assets;
		ETCacheLineAligned mutable Mutex    _packagesMutex;
		PackageSet                          _packages;
		GcCursor                            _gcCursor;
		ETCacheLineAligned mutable Mutex    _requestsMutex;
		Stack<LoadRequest>                  _requests;

		// ---------------------------------------------------

		friend class AbstractPackageClient;
		friend class AssetReference;
	};

}} // namespace Eldritch2::Core

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Core/ContentDatabase.inl>
//------------------------------------------------------------------//
