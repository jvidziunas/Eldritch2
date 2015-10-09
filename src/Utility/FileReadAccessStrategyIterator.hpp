/*==================================================================*\
  FileReadAccessStrategyIterator.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2012 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Compiler.hpp>
//------------------------------------------------------------------//

namespace Eldritch2
{
	namespace FileSystem
	{
		class	FileReadAccessStrategy;
	}
}

namespace Eldritch2
{

	namespace FileSystem
	{

		namespace FileSystem	= ::Eldritch2::FileSystem;

	// ---------------------------------------------------

		class ETPureAbstractHint FileReadAccessStrategyIterator
		{
		public:
			virtual FileSystem::FileReadAccessStrategyIterator&	operator++() abstract;
			virtual												operator bool() const abstract;

			virtual FileSystem::FileReadAccessStrategy&			operator*() abstract;
			virtual FileSystem::FileReadAccessStrategy*			operator->() abstract;

		// ---------------------------------------------------

		protected:
			FileReadAccessStrategyIterator();
			~FileReadAccessStrategyIterator();
		};

	}	// namespace FileSystem

}	// namespace Eldritch2
