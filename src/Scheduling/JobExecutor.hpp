/*==================================================================*\
  JobExecutor.hpp
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

#if !defined(ET_ENABLE_JOB_DEBUGGING)
#	define ET_ENABLE_JOB_DEBUGGING ET_DEBUG_BUILD
#endif

namespace Eldritch2 { namespace Scheduling {

	namespace Detail {

#if ET_PLATFORM_WINDOWS
		using PlatformFiber = void*;
#else
#	error Platform needs fiber implementation!
#endif

	} // namespace Detail

	using JobFence = Atomic<int>;

	// ---------------------------------------------------

	class JobExecutor {
		// - TYPE PUBLISHING ---------------------------------

	public:
		class JobClosure {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref JobClosure instance.
			JobClosure(JobFence& completedFence, Function<void(JobExecutor& /*executor*/)> work);
			//!	Constructs this @ref JobClosure instance.
			JobClosure(const JobClosure&) = default;
			//!	Constructs this @ref JobClosure instance.
			JobClosure(JobClosure&&) = default;

			~JobClosure() = default;

			// ---------------------------------------------------

		public:
			JobClosure& operator=(const JobClosure&) = default;
			JobClosure& operator=(JobClosure&&) = default;

			// - DATA MEMBERS ------------------------------------

		public:
			JobFence*                    completedFence;
			Function<void(JobExecutor&)> work;
		};

		// ---

		struct SuspendedJob {
			// - TYPE PUBLISHING ---------------------------------

		public:
			//!	Constructs this @ref SuspendedJob instance.
			SuspendedJob(const char* file, int line, Detail::PlatformFiber fiber, Function<bool() ETNoexceptHint> shouldResume);
			//!	Constructs this @ref SuspendedJob instance.
			SuspendedJob(Detail::PlatformFiber fiber, Function<bool() ETNoexceptHint> shouldResume);
			//!	Constructs this @ref SuspendedJob instance.
			SuspendedJob(const SuspendedJob&) = default;
			//!	Constructs this @ref SuspendedJob instance.
			SuspendedJob(SuspendedJob&&) = default;
			//!	Constructs this @ref SuspendedJob instance.
			SuspendedJob() = default;

			~SuspendedJob() = default;

			// ---------------------------------------------------

		public:
			SuspendedJob& operator=(const SuspendedJob&) = default;
			SuspendedJob& operator=(SuspendedJob&&) = default;

			// - DATA MEMBERS ------------------------------------

		public:
			Detail::PlatformFiber           fiber;
			Function<bool() ETNoexceptHint> shouldResume;
#if ET_ENABLE_JOB_DEBUGGING
			//!	Source file containing the wait instruction. Used for debugging deadlock/stalled task dependencies.
			const char* file;
			//!	Line number in the file indicating where the wait occurred. Used for debugging deadlock/stalled task dependencies.
			int line;
			//!	Counter value used to distinguish unique wait invocations for a particular line.
			uint64 serialNumber;
#endif
		};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		//!	Disable copy construction.
		JobExecutor(const JobExecutor&) = delete;
		//!	Constructs this @ref JobExecutor instance.
		JobExecutor();

		~JobExecutor() = default;

		// ---------------------------------------------------

	public:
		template <typename... WorkItems>
		void Await_(const char* file, int line, Function<void(JobExecutor&) ETNoexceptHint> workItem, WorkItems&&... workItems);
		void Await_(const char* file, int line, Function<bool() ETNoexceptHint> condition);
		void Await_(const char* file, int line, JobFence& fence);

#define AwaitCondition(...) Await_(__FILE__, __LINE__, [&]() -> bool { return __VA_ARGS__; })
#define AwaitFence(fence) Await_(__FILE__, __LINE__, fence)
#define AwaitWork(...) Await_(__FILE__, __LINE__, __VA_ARGS__)

		// ---------------------------------------------------

	public:
		//!	Executes one or more functions asynchronously, signaling the provided fence when they have completed.
		/*!	@param[in] completed @parblock @ref JobFence the work items should signal when completed. The lifetime of the fence *must* extend past the
				completion time of all work items submitted in this call to @ref StartAsync(). @endparblock
			@param[in] workItems Variadic collection of unary functors to be executed asynchronously. */
		template <typename... WorkItems>
		void StartAsync(JobFence& completed, Function<void(JobExecutor&) ETNoexceptHint> workItem, WorkItems&&... workItems);

		template <size_t splitSize, typename InputIterator, typename OutputIterator, typename AlternateInputIterator, typename TrinaryPredicate>
		void Transform(InputIterator begin, InputIterator end, AlternateInputIterator alternateBegin, OutputIterator out, TrinaryPredicate transformer);
		template <size_t splitSize, typename InputIterator, typename OutputIterator, typename BinaryPredicate>
		void Transform(InputIterator begin, InputIterator end, OutputIterator out, BinaryPredicate transformer);

		template <size_t splitSize, typename InputIterator, typename AlternateInputIterator, typename QuaternaryPredicate>
		void ForEachSplit(InputIterator begin, InputIterator end, AlternateInputIterator alternateBegin, QuaternaryPredicate predicate);
		template <size_t splitSize, typename InputIterator, typename TrinaryPredicate>
		void ForEachSplit(InputIterator begin, InputIterator end, TrinaryPredicate predicate);

		template <size_t splitSize, typename InputIterator, typename AlternateInputIterator, typename TrinaryPredicate>
		void ForEach(InputIterator begin, InputIterator end, AlternateInputIterator alternateBegin, TrinaryPredicate predicate);
		template <size_t splitSize, typename InputIterator, typename BinaryPredicate>
		void ForEach(InputIterator begin, InputIterator end, BinaryPredicate predicate);

		// ---------------------------------------------------

	protected:
		void SwitchFibers(Detail::PlatformFiber fiber);

		void BootFibers(size_t supportFiberStackSizeInBytes = 32768u, size_t fiberStackSizeInBytes = 524288u);

		//!	Binds this @ref JobExecutor as the active context on the calling thread.
		/*!	@see @ref GetExecutor */
		void SetActive();

		// ---------------------------------------------------

	protected:
		virtual void FiberEntryPoint() abstract;

		// - DATA MEMBERS ------------------------------------

	protected:
		ArrayList<JobClosure>            _jobs;
		ArrayList<SuspendedJob>          _suspendedJobs;
		ArrayList<Detail::PlatformFiber> _pooledFibers;
		Detail::PlatformFiber            _bootFiber;
		Detail::PlatformFiber            _waitFiber;
		Detail::PlatformFiber            _switchFiber;
		Detail::PlatformFiber            _transitionTarget;
		SuspendedJob                     _transitionSource;
	};

	// ---------------------------------------------------

	//!	Retrieves the active job fiber for the thread.
	/*!	@returns A pointer to the previously-bound @ref JobExecutor for the calling thread.
		@remarks @parblock This function is anticipated to be significantly slower than simply passing a value
			into a function; it is highly recommended that alternative means of communicating this value to dependent
			code is conveyed via alternate means wherever possible. @endparblock
		@see @ref JobExecutor::MakeActive */
	JobExecutor* GetExecutor() ETNoexceptHint;

}} // namespace Eldritch2::Scheduling

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scheduling/JobExecutor.inl>
//------------------------------------------------------------------//
