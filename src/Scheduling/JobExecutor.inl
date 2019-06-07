/*==================================================================*\
  JobExecutor.inl
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

namespace Eldritch2 { namespace Scheduling {

	ETInlineHint ETForceInlineHint JobExecutor::JobClosure::JobClosure(JobFence& completedFence, FixedFunction<JobClosureSizeBytes, void(JobExecutor& /*executor*/)> work) ETNoexceptHint : completedFence(ETAddressOf(completedFence)),
																																															work(Move(work)) {}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint JobExecutor::SuspendedJob::SuspendedJob(const char* file, int line, PlatformFiber fiber, FixedFunction<WaitClosureSizeBytes, bool()> shouldResume) ETNoexceptHint :
#if ET_ENABLE_JOB_DEBUGGING
		serialNumber([]() ETNoexceptHint -> uint64 { static ETThreadLocal uint64 serial(0u); return serial++; }()),
		file(file),
		line(line),
#endif
		fiber(fiber),
		shouldResume(Move(shouldResume)) {
		ETUnreferencedParameter(file);
		ETUnreferencedParameter(line);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint JobExecutor::SuspendedJob::SuspendedJob(PlatformFiber fiber, FixedFunction<WaitClosureSizeBytes, bool()> shouldResume) ETNoexceptHint :
#if ET_ENABLE_JOB_DEBUGGING
		file(""),
		line(-1),
#endif
		fiber(fiber),
		shouldResume(Move(shouldResume)) {
	}

	// ---------------------------------------------------

	template <typename... WorkItems>
	ETInlineHint ETForceInlineHint void JobExecutor::StartAsync(JobFence& completed, WorkItems... workItems) {
		completed.fetch_add(static_cast<int>(sizeof...(workItems)), std::memory_order_relaxed);
		DiscardArguments((_jobs.EmplaceBack(completed, Move(workItems)), 0)...);
	}

	// ---------------------------------------------------

	template <size_t SplitSize, typename InputIterator, typename InputIterator2, typename QuaternaryPredicate>
	ETInlineHint void JobExecutor::ForEachSplit(InputIterator begin, InputIterator end, InputIterator2 begin2, QuaternaryPredicate predicate) {
		struct Split {
			InputIterator       begin;
			InputIterator       end;
			InputIterator2      begin2;
			QuaternaryPredicate predicate;
		};

		const auto splitCount(eastl::distance(begin, end) / SplitSize);
		const auto splits(ETStackAlloc(Split, Maximum(splitCount, 1u)));
		JobFence   completed(0);
		//	Start [0 - (end - begin) / splitSize) subtasks to exploit data-level parallelism.
		for (size_t split(0u); split < splitCount; split++) {
			new (splits + split) Split{
				eastl::next(begin, split * SplitSize),
				eastl::next(begin, split * SplitSize + SplitSize),
				eastl::next(begin2, split * SplitSize),
				predicate
			};
			this->StartAsync(completed, [current = splits + split](JobExecutor& executor) ETNoexceptHint {
				executor.ForEachSplit<SplitSize, InputIterator, InputIterator2, QuaternaryPredicate>(Move(current->begin), Move(current->end), Move(current->begin2), Move(current->predicate));
			});
		}
		//	Finish off the remaining [0 - SplitSize) transform operations.
		predicate(*this, eastl::next(begin, splitCount * SplitSize), end, eastl::next(begin2, splitCount * SplitSize));

		this->AwaitFence(completed);
	}

	// ---------------------------------------------------

	template <size_t SplitSize, typename InputIterator, typename TrinaryPredicate>
	ETInlineHint void JobExecutor::ForEachSplit(InputIterator begin, InputIterator end, TrinaryPredicate predicate) {
		struct Split {
			InputIterator    begin;
			InputIterator    end;
			TrinaryPredicate predicate;
		};

		const auto splitCount(eastl::distance(begin, end) / SplitSize);
		const auto splits(ETStackAlloc(Split, Maximum(splitCount, 1u)));
		JobFence   completed(0);
		//	Start [0 - splitCount) subtasks to exploit data-level parallelism.
		for (size_t split(0u); split < splitCount; split++) {
			new (splits + split) Split{
				eastl::next(begin, split * SplitSize),
				eastl::next(begin, split * SplitSize + SplitSize),
				predicate
			};
			this->StartAsync(completed, [current = splits + split](JobExecutor& executor) ETNoexceptHint {
				executor.ForEachSplit<SplitSize, InputIterator, TrinaryPredicate>(Move(current->begin), Move(current->end), Move(current->predicate));
			});
		}
		//	Finish off the remaining [0 - SplitSize) transform operations.
		predicate(*this, eastl::next(begin, splitCount * SplitSize), end);

		this->AwaitFence(completed);
	}

	// ---------------------------------------------------

	template <size_t SplitSize, typename InputIterator, typename InputIterator2, typename TrinaryPredicate>
	ETInlineHint ETForceInlineHint void JobExecutor::ForEach(InputIterator begin, InputIterator end, InputIterator2 begin2, TrinaryPredicate predicate) {
		this->ForEachSplit<SplitSize, InputIterator, InputIterator2>(Move(begin), Move(end), Move(begin2), [predicate = Move(predicate)](JobExecutor& executor, InputIterator begin, InputIterator end, InputIterator2 begin2) ETNoexceptHint {
			while (begin != end) {
				predicate(executor, *begin++, *begin2++);
			}
		});
	}

	// ---------------------------------------------------

	template <size_t SplitSize, typename InputIterator, typename BinaryPredicate>
	ETInlineHint ETForceInlineHint void JobExecutor::ForEach(InputIterator begin, InputIterator end, BinaryPredicate predicate) {
		this->ForEachSplit<SplitSize, InputIterator>(Move(begin), Move(end), [predicate = Move(predicate)](JobExecutor& executor, InputIterator begin, InputIterator end) ETNoexceptHint {
			while (begin != end) {
				predicate(executor, *begin++);
			}
		});
	}

	// ---------------------------------------------------

	template <typename BinaryPredicate, typename... Types>
	ETInlineHint ETForceInlineHint void JobExecutor::ForEach(Tuple<Types...>&& tuple, BinaryPredicate predicate) {
		this->ForEach(tuple, IndexSequenceFor<Types...>(), predicate);
	}

	// ---------------------------------------------------

	template <typename BinaryPredicate, typename... Types>
	ETInlineHint ETForceInlineHint void JobExecutor::ForEach(Tuple<Types...>& tuple, BinaryPredicate predicate) {
		this->ForEach(tuple, IndexSequenceFor<Types...>(), predicate);
	}

	// ---------------------------------------------------

	template <typename BinaryPredicate, typename Tuple, size_t... Indices>
	ETInlineHint ETForceInlineHint void JobExecutor::ForEach(Tuple&& tuple, IndexSequence<Indices...>, BinaryPredicate predicate) {
		JobFence completed(0);

		StartAsync(completed, [&](JobExecutor& executor) ETNoexceptHint { predicate(executor, Get<Indices>(tuple)); }...);
		AwaitFence(completed);
	}

	// ---------------------------------------------------

	template <typename BinaryPredicate, typename Tuple, size_t... Indices>
	ETInlineHint ETForceInlineHint void JobExecutor::ForEach(Tuple& tuple, IndexSequence<Indices...>, BinaryPredicate predicate) {
		JobFence completed(0);

		StartAsync(completed, [&](JobExecutor& executor) ETNoexceptHint { predicate(executor, Get<Indices>(tuple)); }...);
		AwaitFence(completed);
	}

	// ---------------------------------------------------

	template <size_t SplitSize, typename InputIterator, typename OutputIterator, typename BinaryPredicate>
	ETInlineHint void JobExecutor::Transform(InputIterator begin, InputIterator end, OutputIterator out, BinaryPredicate transformer) {
		this->ForEachSplit<SplitSize, InputIterator, OutputIterator>(Move(begin), Move(end), Move(out), [transformer = Move(transformer)](JobExecutor& executor, InputIterator begin, InputIterator end, OutputIterator out) ETNoexceptHint {
			while (begin != end) {
				*out++ = Move(transformer(executor, *begin++));
			}
		});
	}

	// ---------------------------------------------------

	template <typename... WorkItems>
	ETInlineHint ETForceInlineHint void JobExecutor::AwaitWork_(const char* file, int line, WorkItems... workItems) {
		JobFence completed(0);

		this->StartAsync<WorkItems...>(completed, Move(workItems)...);
		this->AwaitFence_(file, line, completed);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void JobExecutor::AwaitFence_(const char* file, int line, JobFence& fence) {
		return AwaitCondition_(file, line, [&fence]() -> bool {
			const int value(fence.load(std::memory_order_consume));
			ETAssert(value >= 0, "Reference count mismatch for job fence!");
			return value == 0;
		});
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void JobExecutor::SetActive() ETNoexceptHint {
		SetFiberUserData(this);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint JobExecutor* GetExecutor() ETNoexceptHint {
		return static_cast<JobExecutor*>(GetFiberUserData());
	}

}} // namespace Eldritch2::Scheduling
