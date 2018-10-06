/*==================================================================*\
  AssetDatabase.hpp
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

namespace Eldritch2 { namespace Assets {
	class Package;
	class Asset;
}} // namespace Eldritch2::Assets

namespace Eldritch2 { namespace Assets {

	class AssetApiBuilder {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using AssetFactory = Function<UniquePointer<Asset>(Allocator& /*allocator*/, const Package& /*package*/, StringView /*path*/) ETNoexceptHint>;
		using FactoryMap   = CachingHashMap<StringView, AssetFactory>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref AssetApiBuilder instance.
		AssetApiBuilder(const AssetApiBuilder&) = default;
		//!	Constructs this @ref AssetApiBuilder instance.
		AssetApiBuilder();

		~AssetApiBuilder() = default;

		// ---------------------------------------------------
	public:
		//!	Registers an object creation handler for a specified class of asset object.
		/*!	@param[in] extension @ref Utf8Literal containing the extension of the asset to listen for. This string should include a
				leading dot character.
			@returns A reference to *this for method chaining. */
		AssetApiBuilder& DefineType(StringView extension, AssetFactory factory);

		// ---------------------------------------------------

	public:
		FactoryMap& GetFactories() ETNoexceptHint;

		// ---------------------------------------------------

	public:
		AssetApiBuilder& operator=(const AssetApiBuilder&) = default;

		// ---------------------------------------------------

	public:
		template <typename ConcreteAsset>
		static ETPureFunctionHint AssetFactory DefaultFactory() ETNoexceptHint;

		// - DATA MEMBERS ------------------------------------

	private:
		FactoryMap _factoriesByExtension;
	};

	// ---

	class AssetDatabase {
		// - TYPE PUBLISHING ---------------------------------

	public:
		struct AssetEqual {
			ETPureFunctionHint bool operator()(const Asset*, const Asset*) const ETNoexceptHint;
			ETPureFunctionHint bool operator()(const Asset*, StringView) const ETNoexceptHint;
		};

		// ---

	public:
		struct AssetHash {
			ETPureFunctionHint size_t operator()(const Asset*, size_t seed = 0u) const ETNoexceptHint;
			ETPureFunctionHint size_t operator()(StringView, size_t seed = 0u) const ETNoexceptHint;
		};

		// ---

	public:
		using AssetFactory = AssetApiBuilder::AssetFactory;
		using FactoryMap   = AssetApiBuilder::FactoryMap;
		using ResidentSet  = CachingHashSet<Asset*, AssetHash, AssetEqual>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Disable copy construction.
		AssetDatabase(const AssetDatabase&) = delete;
		//! Constructs this @ref AssetDatabase instance.
		AssetDatabase();

		~AssetDatabase();

		// - ASSET MANAGEMENT --------------------------------

	public:
		//!	Locates an existing asset in the database.
		/*!	@param[in] path UTF-8-encoded string view containing the name of the resource to search for.
			@returns A pointer to the asset, if successful, or null in the event no matching asset was found.
			@remarks Thread-safe. */
		const Asset* Find(StringView path) const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		UniquePointer<Asset> Insert(const Package& owner, StringView path);

		ResidentSet::SizeType Erase(Asset& asset);

		// ---------------------------------------------------

	public:
		ErrorCode BindResources(FactoryMap factoryByExtension);

		void FreeResources();

		// ---------------------------------------------------

		//!	Disable copy assignment.
		AssetDatabase& operator=(const AssetDatabase&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		mutable UsageMixin<MallocAllocator> _allocator;
		mutable Mutex                       _assetsMutex;
		ResidentSet                         _assets;
		FactoryMap                          _factoryByExtension;
	};

}} // namespace Eldritch2::Assets

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Assets/AssetDatabase.inl>
//------------------------------------------------------------------//
