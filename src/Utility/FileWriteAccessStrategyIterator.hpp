/*==================================================================*\
  FileWriteAccessStrategyIterator.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2012 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Util/MPL/Compiler.hpp>
//------------------------------------------------------------------//

namespace Eldritch2
{
	namespace FileSystem
	{
		class	FileWriteStrategy;
	}
}

namespace Eldritch2
{

	namespace FileSystem
	{

		namespace FileSystem	= ::Eldritch2::FileSystem;

	// ---------------------------------------------------

		class ETPureAbstractHint FileWriteAccessStrategyIterator
		{
		public:
			virtual FileSystem::FileWriteAccessStrategyIterator&	operator++() abstract;
			virtual													operator bool() const abstract;

			virtual FileSystem::FileWriteAccessStrategy&			operator*() abstract;
			virtual FileSystem::FileWriteAccessStrategy*			operator->() abstract;

		// ---------------------------------------------------

		protected:
			FileWriteAccessStrategyIterator();
			~FileWriteAccessStrategyIterator();
		};

	}	// namespace FileSystem

}	// namespace Eldritch2
