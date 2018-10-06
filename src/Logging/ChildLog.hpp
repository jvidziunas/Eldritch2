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
		ChildLog() ETNoexceptHint;

		~ChildLog() = default;

		// ---------------------------------------------------

	public:
		void Write(const Utf8Char* string, size_t lengthInOctets) ETNoexceptHint override;
		using Log::Write;

		// ---------------------------------------------------

	public:
		ErrorCode BindResources(Log& parent);

		void FreeResources();

		// ---------------------------------------------------

	public:
		ChildLog& operator=(const ChildLog&) ETNoexceptHint = default;

		// - DATA MEMBERS ------------------------------------

	private:
		Log* _parent;

		// ---------------------------------------------------

		friend void Swap(ChildLog&, ChildLog&) ETNoexceptHint;
	};

}} // namespace Eldritch2::Logging

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Logging/ChildLog.inl>
//------------------------------------------------------------------//
