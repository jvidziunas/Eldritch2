/*==================================================================*\
  ChildLog.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Logging/Log.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Logging {

	class ChildLog : public Log {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref ChildLog instance.
		ChildLog(const ChildLog&) = delete;
		//!	Constructs this @ref ChildLog instance.
		ChildLog(Log& parent);

		~ChildLog() = default;

		// ---------------------------------------------------

	public:
		Log& SetParent(Log& parent);

		Log& GetParent() const;

		// ---------------------------------------------------

	public:
		using Log::Write;

		void Write(const Utf8Char* const string, size_t lengthInOctets) override sealed;

		// ---------------------------------------------------

	public:
		ChildLog& operator=(const ChildLog&) = default;

		// - DATA MEMBERS ------------------------------------

	private:
		Log* _parent;
	};

}} // namespace Eldritch2::Logging

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Logging/ChildLog.inl>
//------------------------------------------------------------------//
