/*==================================================================*\
  SynchronousFileAppender.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Noncopyable.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	class	ErrorCode;
}

namespace Eldritch2 {
namespace FileSystem {

	class ETPureAbstractHint SynchronousFileAppender : public Utility::Noncopyable {
	public:
		virtual ::Eldritch2::ErrorCode	Append( const void* const sourceData, const size_t lengthToWriteInBytes ) abstract;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		//!	Constructs this @ref SynchronousFileAppender instance.
		SynchronousFileAppender() = default;

	public:
		//!	Destroys this @ref SynchronousFileAppender instance.
		virtual ~SynchronousFileAppender() = default;
	};

}	// namespace Foundation
}	// namespace Eldritch2