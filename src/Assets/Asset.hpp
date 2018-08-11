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
#include <Logging/ChildLog.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Assets {

	class ETPureAbstractHint Asset {
		// - TYPE PUBLISHING ---------------------------------

	public:
		class Builder {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref Builder instance.
			Builder(Logging::Log& log, Range<const char*> bytes);
			//!	Constructs this @ref Builder instance.
			Builder(const Builder&) = default;

			~Builder() = default;

			// ---------------------------------------------------

		public:
			template <typename... Arguments>
			void WriteLog(Logging::MessageType severity, StringView<Utf8Char> format, Arguments&&... arguments) const;

			// ---------------------------------------------------

		public:
			const char* Begin() const;
			const char* End() const;

			size_t GetSize() const;

			// - DATA MEMBERS ------------------------------------

		private:
			mutable Logging::ChildLog _log;
			Range<const char*>        _bytes;
		};

		// ---

	public:
		enum : size_t {
			MaxPathLength = 128u
		};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref Asset instance.
		/*!	@param[in] path UTF-8-encoded string view containing the file system path to the resource the @ref Asset describes. */
		Asset(StringView<Utf8Char> path);
		//!	Disable copy construction.
		Asset(const Asset&) = delete;

		virtual ~Asset() = default;

		// ---------------------------------------------------

	public:
		//! Gets the file system filePath to the asset.
		/*! @returns UTF-8-encoded string view containing the file system path to the resource this @ref Asset describes.
			@remarks Thread-safe. */
		StringView<Utf8Char> GetPath() const;

		// ---------------------------------------------------

	public:
		virtual ErrorCode BindResources(const Builder& builder) abstract;

		virtual void FreeResources() abstract;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		Asset& operator=(const Asset&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		Utf8Char _path[MaxPathLength];
	};

}} // namespace Eldritch2::Assets

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Assets/Asset.inl>
//------------------------------------------------------------------//
