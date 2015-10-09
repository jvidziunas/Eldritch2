/*==================================================================*\
  SchedulerBase.inl
  ------------------------------------------------------------------
  Purpose:

  ------------------------------------------------------------------
  ©2010-2012 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scheduler/Closures.hpp>
//------------------------------------------------------------------//

namespace Eldritch2
{

	namespace Foundation
	{

		template<typename Function>
		::Eldritch2::ErrorCode Scheduler::EnqueueModule( const Function& func, const ::Eldritch2::UTF8Char* name )
		{
			typedef ::Eldritch2Detail::Util::Closure0<Function> Closure;

			return this->EnqueueModuleInternal( &Closure::ThreadEntryPoint, new Closure( func ), name );
		}

	// ---------------------------------------------------

		template<typename Function, typename Arg0Type>
		::Eldritch2::ErrorCode Scheduler::EnqueueModule( const Function& func,
														 const Arg0Type& arg0,
														 const ::Eldritch2::UTF8Char* name )
		{
			typedef ::Eldritch2Detail::Util::Closure1<Function, Arg0Type>	Closure;

			return this->EnqueueModuleInternal( &Closure::ThreadEntryPoint, new Closure( func, arg0 ), name );
		}

	// ---------------------------------------------------

		template<typename Function, typename Arg0Type, typename Arg1Type>
		Eldritch2::ErrorCode Scheduler::EnqueueModule( const Function& func,
													   const Arg0Type& arg0,
													   const Arg1Type& arg1,
													   const ::Eldritch2::UTF8Char* name )
		{
			typedef ::Eldritch2Detail::Util::Closure2<Function, Arg0Type, Arg1Type> Closure;

			return this->EnqueueModuleInternal( &Closure::ThreadEntryPoint, new Closure( func, arg0, arg1 ), name );
		}

	// ---------------------------------------------------

		template<typename Function, typename Arg0Type, typename Arg1Type, typename Arg2Type>
		::Eldritch2::ErrorCode Scheduler::EnqueueModule( const Function& func,
														 const Arg0Type& arg0,
														 const Arg1Type& arg1,
														 const Arg2Type& arg2,
														 const ::Eldritch2::UTF8Char* name )
		{
			typedef ::Eldritch2Detail::Util::Closure3<Function, Arg0Type, Arg1Type, Arg2Type>	Closure;

			return this->EnqueueModuleInternal( &Closure::ThreadEntryPoint, new Closure( func, arg0, arg1, arg2 ), name );
		}

	// ---------------------------------------------------

		template<typename Function, typename Arg0Type, typename Arg1Type, typename Arg2Type, typename Arg3Type>
		::Eldritch2::ErrorCode Scheduler::EnqueueModule( const Function& func,
														 const Arg0Type& arg0,
														 const Arg1Type& arg1,
														 const Arg2Type& arg2,
														 const Arg3Type& arg3,
														 const ::Eldritch2::UTF8Char* name )
		{
			typedef ::Eldritch2Detail::Util::Closure4<Function, Arg0Type, Arg1Type, Arg2Type, Arg3Type>	Closure;

			return this->EnqueueModuleInternal( &Closure::ThreadEntryPoint, new Closure( func, arg0, arg1, arg2, arg3 ), name );
		}

	// ---------------------------------------------------

		template<typename Function, typename Arg0Type, typename Arg1Type, typename Arg2Type, typename Arg3Type, typename Arg4Type>
		::Eldritch2::ErrorCode Scheduler::EnqueueModule( const Function& func,
														 const Arg0Type& arg0,
														 const Arg1Type& arg1,
														 const Arg2Type& arg2,
														 const Arg3Type& arg3,
														 const Arg4Type& arg4,
														 const ::Eldritch2::UTF8Char* name )
		{
			typedef ::Eldritch2Detail::Util::Closure5<Function, Arg0Type, Arg1Type, Arg2Type, Arg3Type, Arg4Type>	Closure;

			return this->EnqueueModuleInternal( &Closure::ThreadEntryPoint, new Closure( func, arg0, arg1, arg2, arg3, arg4 ), name );
		}

	// ---------------------------------------------------

		template<typename Function>
		::Eldritch2::ErrorCode Scheduler::EnqueueTask( const Function& func )
		{
			typedef ::Eldritch2Detail::Util::Closure0<Function>	Closure;

			return this->EnqueueTaskInternal( &Closure::ThreadEntryPoint, new Closure( func ) );
		}

	// ---------------------------------------------------

		template<typename Function, typename Arg0Type>
		::Eldritch2::ErrorCode Scheduler::EnqueueTask( const Function& func, const Arg0Type& arg0 )
		{
			typedef ::Eldritch2Detail::Util::Closure1<Function, Arg0Type>	Closure;

			return this->EnqueueTaskInternal( &Closure::ThreadEntryPoint, new Closure( func, arg0 ) );
		}

	// ---------------------------------------------------

		template<typename Function, typename Arg0Type, typename Arg1Type>
		::Eldritch2::ErrorCode Scheduler::EnqueueTask( const Function& func, const Arg0Type& arg0, const Arg1Type& arg1 )
		{
			typedef ::Eldritch2Detail::Util::Closure2<Function, Arg0Type, Arg1Type>	Closure;

			return this->EnqueueTaskInternal( &Closure::ThreadEntryPoint, new Closure( func, arg0, arg1 ) );
		}

	// ---------------------------------------------------

		template<typename Function, typename Arg0Type, typename Arg1Type, typename Arg2Type>
		::Eldritch2::ErrorCode Scheduler::EnqueueTask( const Function& func, const Arg0Type& arg0, const Arg1Type& arg1, const Arg2Type& arg2 )
		{
			typedef ::Eldritch2Detail::Util::Closure3<Function, Arg0Type, Arg1Type, Arg2Type>	Closure;

			return this->EnqueueTaskInternal( &Closure::ThreadEntryPoint, new Closure( func, arg0, arg1, arg2 ) );
		}

	// ---------------------------------------------------

		template<typename Function, typename Arg0Type, typename Arg1Type, typename Arg2Type, typename Arg3Type>
		::Eldritch2::ErrorCode Scheduler::EnqueueTask( const Function& func, const Arg0Type& arg0, const Arg1Type& arg1, const Arg2Type& arg2, const Arg3Type& arg3 )
		{
			typedef ::Eldritch2Detail::Util::Closure4<Function, Arg0Type, Arg1Type, Arg2Type, Arg3Type>	Closure;

			return this->EnqueueTaskInternal( &Closure::ThreadEntryPoint, new Closure( func, arg0, arg1, arg2, arg3 ) );
		}

	// ---------------------------------------------------

		template<typename Function, typename Arg0Type, typename Arg1Type, typename Arg2Type, typename Arg3Type, typename Arg4Type>
		::Eldritch2::ErrorCode Scheduler::EnqueueTask( const Function& func, const Arg0Type& arg0, const Arg1Type& arg1, const Arg2Type& arg2, const Arg3Type& arg3, const Arg4Type& arg4 )
		{
			typedef ::Eldritch2Detail::Util::Closure5<Function, Arg0Type, Arg1Type, Arg2Type, Arg3Type, Arg4Type>	Closure;

			return this->EnqueueTaskInternal( &Closure::ThreadEntryPoint, new Closure( func, arg0, arg1, arg2, arg3, arg4 ) );
		}

	}	// namespace Foundation

}	// namespace Eldritch2