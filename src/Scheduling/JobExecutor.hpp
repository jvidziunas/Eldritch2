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

#if !defined(ET_JOB_CLOSURE_SIZE)
#	define ET_JOB_CLOSURE_SIZE (3u * sizeof(void*))
#endif

#define AwaitCondition(...) AwaitCondition_(__FILE__, __LINE__, [&]() -> bool { return __VA_ARGS__; })
#define AwaitFence(fence) AwaitFence_(__FILE__, __LINE__, fence)
#define AwaitWork(...) AwaitWork_(__FILE__, __LINE__, __VA_ARGS__)

namespace Eldritch2 { namespace Scheduling {

	enum : size_t {
		JobClosureSizeBytes  = ET_JOB_CLOSURE_SIZE,
		WaitClosureSizeBytes = ET_JOB_CLOSURE_SIZE,
	};

	// ---

	using JobFence = Atomic<int>;

	// ---

	class JobExecutor {
		// - TYPE PUBLISHING ---------------------------------

	public:
		class JobClosure {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref JobClosure instance.
			JobClosure(JobFence& completedFence, FixedFunction<JobClosureSizeBytes, void(JobExecutor& /*executor*/)>) ETNoexceptHint;
			//!	Constructs this @ref JobClosure instance.
			JobClosure(const JobClosure&) = default;
			//!	Constructs this @ref JobClosure instance.
			JobClosure(JobClosure&&) ETNoexceptHint = default;

			~JobClosure() = default;

			// ---------------------------------------------------

		public:
			JobClosure& operator=(const JobClosure&) = default;
			JobClosure& operator=(JobClosure&&) ETNoexceptHint = default;

			// - DATA MEMBERS ------------------------------------

		public:
			JobFence*                                                           completedFence;
			FixedFunction<JobClosureSizeBytes, void(JobExecutor& /*executor*/)> work;
		};

		// ---

		struct SuspendedJob {
			// - TYPE PUBLISHING ---------------------------------

		public:
			//!	Constructs this @ref SuspendedJob instance.
			SuspendedJob(const char* file, int line, PlatformFiber fiber, FixedFunction<WaitClosureSizeBytes, bool()> shouldResume) ETNoexceptHint;
			//!	Constructs this @ref SuspendedJob instance.
			SuspendedJob(PlatformFiber fiber, FixedFunction<WaitClosureSizeBytes, bool()> shouldResume) ETNoexceptHint;
			//!	Constructs this @ref SuspendedJob instance.
			SuspendedJob(const SuspendedJob&) = default;
			//!	Constructs this @ref SuspendedJob instance.
			SuspendedJob(SuspendedJob&&) ETNoexceptHint = default;

			~SuspendedJob() = default;

			// ---------------------------------------------------

		public:
			SuspendedJob& operator=(const SuspendedJob&) = default;
			SuspendedJob& operator=(SuspendedJob&&) ETNoexceptHint = default;

			// - DATA MEMBERS ------------------------------------

		public:
#if ET_ENABLE_JOB_DEBUGGING
			//!	Counter value used to distinguish unique wait invocations for a particular line.
			uint64 serialNumber;
			//!	Source file containing the wait instruction. Used for debugging deadlock/stalled task dependencies.
			const char* file;
			//!	Line number in the file indicating where the wait occurred. Used for debugging deadlock/stalled task dependencies.
			int line;
#endif
			PlatformFiber                               fiber;
			FixedFunction<WaitClosureSizeBytes, bool()> shouldResume;
		};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		//!	Disable copy construction.
		JobExecutor(const JobExecutor&) = delete;
		//!	Constructs this @ref JobExecutor instance.
		JobExecutor() ETNoexceptHint;

		~JobExecutor() = default;

		// ---------------------------------------------------

	public:
		void AwaitCondition_(const char* file, int line, FixedFunction<WaitClosureSizeBytes, bool()> condition);
		template <typename... WorkItems>
		void AwaitWork_(const char* file, int line, WorkItems... workItems);
		void AwaitFence_(const char* file, int line, JobFence& fence);

		// ---------------------------------------------------

	public:
		//!	Executes one or more functions asynchronously, signaling the provided fence when they have completed.
		/*!	@param[in] completed @parblock @ref JobFence the work items should signal when completed. The lifetime of the fence *must* extend past the
				completion time of all work items submitted in this call to @ref StartAsync(). @endparblock
			@param[in] workItems Variadic collection of unary functors to be executed asynchronously. */
		template <typename... WorkItems>
		void StartAsync(JobFence& completed, WorkItems... workItems);

		//!	General pseudo-synchronous parallel algorithm template.
		/*!	Task split threshold is used to control how many individual processing jobs are created to complete work and should be proportional to the amount of
			computation required for a single block of predicate invocations.
			@param[in] begin Iterator to the first element to be processed.
			@param[in] end Iterator to the last element to be processed.
			@param[in] begin2 Iterator to the first 'dual' element to be processed at 1:1 ratio with the elements in [begin, end).
			@param[in] predicate @parblock Element-processing functionoid with call signature (@ref JobExecutor&, InputIterator, InputIterator, InputIterator2).
				Unlike @ref ForEach(), the predicate is responsible for processing a range of elements rather than a single input. @endparblock */
		template <size_t SplitSize, typename InputIterator, typename InputIterator2, typename QuaternaryPredicate>
		void ForEachSplit(InputIterator begin, InputIterator end, InputIterator2 begin2, QuaternaryPredicate predicate);
		//!	General pseudo-synchronous parallel algorithm template.
		/*!	Task split threshold is used to control how many individual processing jobs are created to complete work and should be proportional to the amount of computation
			@param[in] begin Iterator to the first element to be processed.
			@param[in] end Iterator to the last element to be processed.
			@param[in] predicate @parblock Element-processing functionoid with call signature (@ref JobExecutor&, InputIterator, InputIterator).
				Unlike @ref ForEach(), the predicate is responsible for processing a range of elements rather than a single input. @endparblock */
		template <size_t SplitSize, typename InputIterator, typename TrinaryPredicate>
		void ForEachSplit(InputIterator begin, InputIterator end, TrinaryPredicate predicate);

		template <size_t SplitSize, typename InputIterator, typename InputIterator2, typename TrinaryPredicate>
		void ForEach(InputIterator begin, InputIterator end, InputIterator2 begin2, TrinaryPredicate predicate);
		template <size_t SplitSize, typename InputIterator, typename BinaryPredicate>
		void ForEach(InputIterator begin, InputIterator end, BinaryPredicate predicate);
		template <typename BinaryPredicate, typename... Types>
		void ForEach(Tuple<Types...>&&, BinaryPredicate predicate);
		template <typename BinaryPredicate, typename... Types>
		void ForEach(Tuple<Types...>&, BinaryPredicate predicate);

		template <size_t SplitSize, typename InputIterator, typename OutputIterator, typename BinaryPredicate>
		void Transform(InputIterator begin, InputIterator end, OutputIterator out, BinaryPredicate transformer);

		// ---------------------------------------------------

	protected:
		template <typename BinaryPredicate, typename Tuple, size_t... Indices>
		void ForEach(Tuple&&, IndexSequence<Indices...>, BinaryPredicate predicate);
		template <typename BinaryPredicate, typename Tuple, size_t... Indices>
		void ForEach(Tuple&, IndexSequence<Indices...>, BinaryPredicate predicate);

	// ---------------------------------------------------

	protected:
		void ResumeOnCaller(PlatformFiber fiber);

		void BootFibers(size_t supportStackBytes = 32768u, size_t stackBytes = 524288u);

		//!	Binds this @ref JobExecutor as the active context on the calling thread.
		/*!	@see @ref GetExecutor */
		void SetActive() ETNoexceptHint;

		// ---------------------------------------------------

	protected:
		virtual void FiberEntryPoint() abstract;

		// - DATA MEMBERS ------------------------------------

	protected:
		ArrayList<JobClosure>    _jobs;
		ArrayList<SuspendedJob>  _suspendedJobs;
		ArrayList<PlatformFiber> _pooledFibers;
		PlatformFiber            _bootFiber;
		PlatformFiber            _waitFiber;
		PlatformFiber            _switchFiber;
		PlatformFiber            _transitionTarget;
		SuspendedJob             _transitionSource;
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
