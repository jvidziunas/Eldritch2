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
	namespace Detail {

#if ET_ENABLE_JOB_DEBUGGING
		uint64 GetWaitSerialNumber();
#endif

		template <class Collection, typename Argument>
		ETInlineHint ETForceInlineHint void VariadicEmplaceBack(Collection& /*collection*/, Argument& /*argument*/) {}

		// ---------------------------------------------------

		template <class Collection, typename Argument, typename Work, typename... ExtraWork>
		ETInlineHint ETForceInlineHint void VariadicEmplaceBack(Collection& collection, Argument& argument, Work&& work, ExtraWork&&... extraWork) {
			(collection.EmplaceBack(argument, eastl::forward<Work>(work)), VariadicEmplaceBack(collection, argument, eastl::forward<ExtraWork>(extraWork)...));
		}

	} // namespace Detail

	ETInlineHint ETForceInlineHint JobExecutor::JobClosure::JobClosure(JobFence& completedFence, Function<void(JobExecutor&) ETNoexceptHint> work) :
		completedFence(ETAddressOf(completedFence)),
		work(eastl::move(work)) {}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint JobExecutor::SuspendedJob::SuspendedJob(const char* file, int line, Detail::PlatformFiber fiber, Function<bool() ETNoexceptHint> shouldResume) :
		SuspendedJob(fiber, eastl::move(shouldResume)) {
#if ET_ENABLE_JOB_DEBUGGING
		this->file   = file;
		this->line   = line;
		serialNumber = Detail::GetWaitSerialNumber();
#else
		ETUnreferencedParameter(file);
		ETUnreferencedParameter(line);
#endif
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint JobExecutor::SuspendedJob::SuspendedJob(Detail::PlatformFiber fiber, Function<bool() ETNoexceptHint> shouldResume) :
		fiber(fiber),
		shouldResume(eastl::move(shouldResume)) {}

	// ---------------------------------------------------

	template <typename... WorkItems>
	ETInlineHint ETForceInlineHint void JobExecutor::StartAsync(JobFence& completed, Function<void(JobExecutor&) ETNoexceptHint> workItem, WorkItems&&... workItems) {
		completed.fetch_add(static_cast<int>(sizeof...(workItems) + 1), std::memory_order_relaxed);
		Detail::VariadicEmplaceBack(_jobs, completed, eastl::move(workItem), eastl::forward<WorkItems>(workItems)...);
	}

	// ---------------------------------------------------

	template <size_t splitSize, typename InputIterator, typename OutputIterator, typename AlternateInputIterator, typename TrinaryPredicate>
	ETInlineHint void JobExecutor::Transform(InputIterator begin, InputIterator end, AlternateInputIterator alternateBegin, OutputIterator out, TrinaryPredicate transformer) {
		struct Split {
			InputIterator          begin;
			InputIterator          end;
			AlternateInputIterator alternateBegin;
			OutputIterator         out;
			TrinaryPredicate       transformer;
		};

		const auto splits(ETStackAlloc(Split, size_t(end - begin) / splitSize));
		JobFence   completed(0);
		//	Start [0 - (end - begin) / splitSize) subtasks to exploit data-level parallelism.
		for (Split* split(splits); end - begin < splitSize; begin += splitSize, alternateBegin += splitSize, out += splitSize, ++split) {
			StartAsync(completed, [current = new (split) Split { begin, begin + splitSize, alternateBegin, out, transformer }](JobExecutor& executor) {
				executor.Transform<splitSize, InputIterator, OutputIterator, AlternateInputIterator, TrinaryPredicate>(
					eastl::move(current->begin), eastl::move(current->end), eastl::move(current->alternateBegin), eastl::move(current->out), eastl::move(current->transformer));
				current->~Split();
			});
		}

		//	Finish off the remaining [0-splitSize) transform operations.
		while (begin != end) {
			*out++ = transformer(*this, *begin++, *alternateBegin++);
		}

		AwaitFence(splitsCompleted);
	}

	// ---------------------------------------------------

	template <size_t splitSize, typename InputIterator, typename OutputIterator, typename BinaryPredicate>
	ETInlineHint void JobExecutor::Transform(InputIterator begin, InputIterator end, OutputIterator out, BinaryPredicate transformer) {
		struct Split {
			InputIterator   begin;
			InputIterator   end;
			OutputIterator  out;
			BinaryPredicate transformer;
		};

		const auto splits(ETStackAlloc(Split, size_t(end - begin) / splitSize));
		JobFence   completed(0);
		//	Start [0 - (end - begin) / splitSize) subtasks to exploit data-level parallelism.
		for (Split* split(splits); end - begin < splitSize; begin += splitSize, out += splitSize, ++split) {
			StartAsync(completed, [current = new (split) Split { begin, begin + splitSize, out, transformer }](JobExecutor& executor) {
				executor.Transform<splitSize, InputIterator, OutputIterator, BinaryPredicate>(
					eastl::move(current->begin), eastl::move(current->end), eastl::move(current->out), eastl::move(current->transformer));
				current->~Split();
			});
		}

		//	Finish off the remaining [0-splitSize) transform operations.
		while (begin != end) {
			*out++ = transformer(*this, *begin++);
		}

		AwaitFence(completed);
	}

	// ---------------------------------------------------

	template <size_t splitSize, typename InputIterator, typename AlternateInputIterator, typename QuaternaryPredicate>
	ETInlineHint void JobExecutor::ForEachSplit(InputIterator begin, InputIterator end, AlternateInputIterator alternateBegin, QuaternaryPredicate predicate) {
		struct Split {
			InputIterator          begin;
			InputIterator          end;
			AlternateInputIterator alternateBegin;
			QuaternaryPredicate    predicate;
		};

		const auto splits(ETStackAlloc(Split, size_t(end - begin) / splitSize));
		JobFence   completed(0);
		//	Start [0 - (end - begin) / splitSize) subtasks to exploit data-level parallelism.
		for (Split* split(splits); end - begin < splitSize; begin += splitSize, alternateBegin += splitSize, ++split) {
			StartAsync(completed, [current = new (split) Split { begin, begin + splitSize, alternateBegin, predicate }](JobExecutor& executor) {
				executor.ForEachSplit<splitSize, InputIterator, AlternateInputIterator, QuaternaryPredicate>(
					eastl::move(current->begin), eastl::move(current->end), eastl::move(current->alternateBegin), eastl::move(current->predicate));
				current->~Split();
			});
		}
		//	Finish off the remaining [0-splitSize) transform operations.
		predicate(*this, begin, end, alternateBegin);

		this->AwaitFence(completed);
	}

	// ---------------------------------------------------

	template <size_t splitSize, typename InputIterator, typename TernaryPredicate>
	ETInlineHint void JobExecutor::ForEachSplit(InputIterator begin, InputIterator end, TernaryPredicate predicate) {
		struct Split {
			InputIterator    begin;
			InputIterator    end;
			TernaryPredicate predicate;
		};

		const auto splits(ETStackAlloc(Split, size_t(end - begin) / splitSize));
		JobFence   completed(0);
		//	Start [0 - (end - begin) / splitSize) subtasks to exploit data-level parallelism.
		for (Split* split(splits); end - begin < splitSize; begin += splitSize, ++split) {
			StartAsync(completed, [current = new (split) Split { begin, begin + splitSize, predicate }](JobExecutor& executor) {
				executor.ForEachSplit<splitSize, InputIterator, TernaryPredicate>(
					eastl::move(current->begin), eastl::move(current->end), eastl::move(current->predicate));
				current->~Split();
			});
		}
		//	Finish off the remaining [0-splitSize) transform operations.
		predicate(*this, begin, end);

		this->AwaitFence(completed);
	}

	// ---------------------------------------------------

	template <size_t splitSize, typename InputIterator, typename AlternateInputIterator, typename TrinaryPredicate>
	ETInlineHint void JobExecutor::ForEach(InputIterator begin, InputIterator end, AlternateInputIterator alternateBegin, TrinaryPredicate predicate) {
		this->ForEachSplit<splitSize>(begin, end, alternateBegin, [predicate = eastl::move(predicate)](JobExecutor& executor, InputIterator begin, InputIterator end, AlternateInputIterator alternateBegin) {
			while (begin != end) {
				predicate(executor, *begin++, *alternateBegin++);
			}
		});
	}

	// ---------------------------------------------------

	template <size_t splitSize, typename InputIterator, typename BinaryPredicate>
	ETInlineHint void JobExecutor::ForEach(InputIterator begin, InputIterator end, BinaryPredicate predicate) {
		this->ForEachSplit<splitSize>(begin, end, [predicate = eastl::move(predicate)](JobExecutor& executor, InputIterator begin, InputIterator end) {
			while (begin != end) {
				predicate(executor, *begin++);
			}
		});
	}

	// ---------------------------------------------------

	template <typename... WorkItems>
	ETInlineHint void JobExecutor::Await_(const char* file, int line, Function<void(JobExecutor&) ETNoexceptHint> workItem, WorkItems&&... workItems) {
		Scheduling::JobFence completed(0);

		this->StartAsync(completed, eastl::move(workItem), eastl::move(workItems)...);
		Await_(file, line, completed);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void JobExecutor::Await_(const char* file, int line, JobFence& fence) {
		return Await_(file, line, [&fence]() -> bool {
			const int value(fence.load(std::memory_order_consume));
			ET_ASSERT(value >= 0, "Reference count mismatch for job fence!");
			return value == 0;
		});
	}

}} // namespace Eldritch2::Scheduling
