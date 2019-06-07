/*==================================================================*\
  Asset.hpp
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

namespace Eldritch2 { namespace Logging {
	class Log;
}} // namespace Eldritch2::Logging

namespace Eldritch2 { namespace Core {

	class AssetBuilder {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref AssetBuilder instance.
		ETConstexpr AssetBuilder(StringSpan package, Span<const byte*> bytes) ETNoexceptHint;
		//!	Constructs this @ref AssetBuilder instance.
		ETConstexpr AssetBuilder(const AssetBuilder&) = default;

		~AssetBuilder() = default;

		// - DATA MEMBERS ------------------------------------

	public:
		StringSpan        package;
		Span<const byte*> bytes;
	};

	// ---

	class ETPureAbstractHint Asset {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref Asset instance.
		/*!	@param[in] path UTF-8-encoded string view containing the file system path to the resource the @ref Asset describes. */
		Asset(StringSpan path) ETNoexceptHint;
		//!	Disable copy construction.
		Asset(const Asset&) = delete;

		virtual ~Asset() = default;

		// ---------------------------------------------------

	public:
		//! Gets the file system filePath to the asset.
		/*! @returns UTF-8-encoded string view containing the file system path to the resource this @ref Asset describes.
			@remarks Thread-safe. */
		ETConstexpr StringSpan GetPath() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		virtual Result BindResources(Logging::Log& log, const AssetBuilder& builder) abstract;

		virtual void FreeResources() ETNoexceptHint abstract;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		Asset& operator=(const Asset&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		Utf8Char _path[128];
	};

}} // namespace Eldritch2::Core

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Core/Asset.inl>
//------------------------------------------------------------------//
