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
#include <Utility/SynchronousFileAppender.hpp>
#include <Utility/MPL/Noncopyable.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	class	ErrorCode;
}

namespace Eldritch2 {
namespace FileSystem {

	namespace FileSystem	= ::Eldritch2::FileSystem;

// ---------------------------------------------------

	class ETPureAbstractHint SynchronousFileAppender : public Utility::Noncopyable {
	public:
		virtual ::Eldritch2::ErrorCode	Append( const void* const sourceData, const size_t lengthToWriteInBytes ) abstract;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		// Constructs this SynchronousFileAppender instance.
		SynchronousFileAppender();

	public:
		// Destroys this SynchronousFileAppender instance.
		virtual ~SynchronousFileAppender();
	};

}	// namespace Foundation
}	// namespace Eldritch2