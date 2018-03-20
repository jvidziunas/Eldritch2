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

namespace Eldritch2 {
namespace Scheduling {
namespace Detail {

#if ET_ENABLE_JOB_DEBUGGING
	uint64	GetWaitSerialNumber();
#endif

	template <class Collection, typename Argument>
	void VariadicEmplaceBack( Collection& /*collection*/, Argument& /*argument*/ ) {}

// ---------------------------------------------------

	template <class Collection, typename Argument, typename Work, typename... ExtraWork>
	void VariadicEmplaceBack( Collection& collection, Argument& argument, Work&& work, ExtraWork&&... extraWork ) {
		(collection.EmplaceBack( argument, eastl::forward<Work>( work ) ), VariadicEmplaceBack( collection, argument, eastl::forward<ExtraWork>( extraWork )... ));
	}

}	// namespace Detail

	ETInlineHint JobExecutor::JobClosure::JobClosure( JobFence& completedFence, Function<void ( JobExecutor& )> work ) : completedFence( &completedFence ), work( eastl::move( work ) ) {}

// ---------------------------------------------------

	ETInlineHint JobExecutor::SuspendedJob::SuspendedJob( const char* file, int line, Detail::PlatformFiber fiber, Function<bool()> shouldResume ) : SuspendedJob( fiber, eastl::move( shouldResume ) ) {
#	if ET_ENABLE_JOB_DEBUGGING
		this->file = file;
		this->line = line;
		serialNumber = Detail::GetWaitSerialNumber();
#	else
		ETUnreferencedParameter( file );
		ETUnreferencedParameter( line );
#	endif
	}

// ---------------------------------------------------

	ETInlineHint JobExecutor::SuspendedJob::SuspendedJob( Detail::PlatformFiber fiber, Function<bool()> shouldResume ) : fiber( fiber ), shouldResume( eastl::move( shouldResume ) ) {}

// ---------------------------------------------------

	template <typename... WorkItems>
	ETInlineHint void JobExecutor::StartAsync( JobFence& completed, Function<void ( JobExecutor& )> workItem, WorkItems&&... workItems ) {
		completed.fetch_add( static_cast<int>(1 + sizeof...(workItems)), std::memory_order_relaxed );

		Detail::VariadicEmplaceBack( _jobs, completed, eastl::move( workItem ), eastl::forward<WorkItems>( workItems )... );
	}

// ---------------------------------------------------

	template <size_t splitSize, typename InputIterator, typename OutputIterator, typename AlternateInputIterator, typename TrinaryPredicate>
	ETInlineHint void JobExecutor::Transform( InputIterator begin, InputIterator end, AlternateInputIterator alternateBegin, OutputIterator out, TrinaryPredicate transformer ) {
		struct Split {
			InputIterator			begin;
			InputIterator			end;
			AlternateInputIterator	alternateBegin;
			OutputIterator			out;
			TrinaryPredicate*		transformer;
		}							splits[4u];

		JobFence	splitsCompleted( 0 );
		const auto	inputsPerSplit( Max( static_cast<size_t>(end - begin) / _countof( splits ), splitSize ) );

	//	Start [0 - countof(splits)) subtasks to exploit data-level parallelism.
		for (auto& split : splits) {
			if (static_cast<size_t>(end - begin) <= splitSize) {
				break;
			}

			split.begin				= begin;
			split.end				= begin	+ inputsPerSplit;
			split.alternateBegin	= alternateBegin + inputsPerSplit;
			split.out				= out;
			split.transformer		= &transformer;

			begin			+= inputsPerSplit;
			out				+= inputsPerSplit;
			alternateBegin	+= inputsPerSplit;

			StartAsync( splitsCompleted, [&split] ( JobExecutor& executor ) {
				executor.Transform<splitSize>( split.begin, split.end, split.alternateBegin, split.out, *split.transformer );
			} );
		}

	//	Finish off the remaining [0-splitSize) transform operations.
		while (begin != end) {
			*out++ = transformer( *this, *begin++, *alternateBegin++ );
		}

		AwaitFence( splitsCompleted );
	}

// ---------------------------------------------------

	template <size_t splitSize, typename InputIterator, typename OutputIterator, typename BinaryPredicate>
	ETInlineHint void JobExecutor::Transform( InputIterator begin, InputIterator end, OutputIterator out, BinaryPredicate transformer ) {
		struct {
			InputIterator		begin;
			InputIterator		end;
			OutputIterator		out;
			BinaryPredicate*	transformer;
		}							splits[4u];

		JobFence	splitsCompleted( 0 );
		const auto	inputsPerSplit( Max( static_cast<size_t>(end - begin) / _countof( splits ), splitSize ) );

	//	Start [0 - countof(splits)) subtasks to exploit data-level parallelism.
		for (auto& split : splits) {
			if (static_cast<size_t>(end - begin) <= splitSize) {
				break;
			}

			split.begin			= begin;
			split.end			= begin	+ inputsPerSplit;
			split.out			= out;
			split.transformer	= &transformer;

			begin	+= inputsPerSplit;
			out		+= inputsPerSplit;

			StartAsync( splitsCompleted, [&split] ( JobExecutor& executor ) {
				executor.Transform<splitSize>( split.begin, split.end, split.out, *split.transformer );
			} );
		}

	//	Finish off the remaining [0-splitSize) transform operations.
		while (begin != end) {
			*out++ = transformer( *this, *begin++ );
		}

		AwaitFence( splitsCompleted );
	}

// ---------------------------------------------------

	template <size_t splitSize, typename InputIterator, typename AlternateInputIterator, typename QuaternaryPredicate>
	 ETInlineHint void JobExecutor::ForEachSplit( InputIterator begin, InputIterator end, AlternateInputIterator alternateBegin, QuaternaryPredicate predicate ) {
		 struct {
			InputIterator			begin;
			InputIterator			end;
			AlternateInputIterator	alternateBegin;
			QuaternaryPredicate*	predicate;
		}							splits[4u];

		JobFence	splitsCompleted( 0 );
		const auto	inputsPerSplit( Max( static_cast<size_t>(end - begin) / _countof( splits ), splitSize ) );

	//	Start [0 - countof(splits)) subtasks to exploit data-level parallelism.
		for (auto& split : splits) {
			if (static_cast<size_t>(end - begin) <= splitSize) {
				break;
			}

			split.begin				= begin;
			split.end				= begin	+ inputsPerSplit;
			split.alternateBegin	= alternateBegin + inputsPerSplit;
			split.predicate			= &predicate;

			begin			+=	inputsPerSplit;
			alternateBegin	+=	inputsPerSplit;

			StartAsync( splitsCompleted, [&split] ( JobExecutor& executor ) {
				executor.ForEachSplit<splitSize>( split.begin, split.end, split.alternateBegin, *split.predicate );
			} );
		}

	//	Finish off the remaining [0-splitSize) transform operations.
		predicate( *this, begin, end, alternateBegin );

		this->AwaitFence( splitsCompleted );
	 }

// ---------------------------------------------------

	template <size_t splitSize, typename InputIterator, typename TernaryPredicate>
	ETInlineHint void JobExecutor::ForEachSplit( InputIterator begin, InputIterator end, TernaryPredicate predicate ) {
		struct {
			InputIterator		begin;
			InputIterator		end;
			TernaryPredicate*	predicate;
		}							splits[4u];

		JobFence	splitsCompleted( 0 );
		const auto	inputsPerSplit( Max( static_cast<size_t>(end - begin) / _countof( splits ), splitSize ) );

	//	Start [0 - countof(splits)) subtasks to exploit data-level parallelism.
		for (auto& split : splits) {
			if (static_cast<size_t>(end - begin) <= splitSize) {
				break;
			}

			split.begin		= begin;
			split.end		= begin	+ inputsPerSplit;
			split.predicate	= &predicate;

			begin	+= inputsPerSplit;

			StartAsync( splitsCompleted, [&split] ( JobExecutor& executor ) {
				executor.ForEachSplit<splitSize>( split.begin, split.end, *split.predicate );
			} );
		}

	//	Finish off the remaining [0-splitSize) transform operations.
		predicate( *this, begin, end );

		this->AwaitFence( splitsCompleted );
	}

// ---------------------------------------------------

	template <size_t splitSize, typename InputIterator, typename AlternateInputIterator, typename TrinaryPredicate>
	ETInlineHint void JobExecutor::ForEach( InputIterator begin, InputIterator end, AlternateInputIterator alternateBegin, TrinaryPredicate predicate ) {
		this->ForEachSplit<splitSize>( begin, end, alternateBegin, [predicate = eastl::move( predicate )] ( JobExecutor& executor, InputIterator begin, InputIterator end, AlternateInputIterator alternateBegin ) {
			while (begin != end) {
				predicate( executor, *begin++, *alternateBegin++ );
			}
		} );
	}

// ---------------------------------------------------

	template <size_t splitSize, typename InputIterator, typename BinaryPredicate>
	ETInlineHint void JobExecutor::ForEach( InputIterator begin, InputIterator end, BinaryPredicate predicate ) {
		this->ForEachSplit<splitSize>( begin, end, [predicate = eastl::move( predicate )] ( JobExecutor& executor, InputIterator begin, InputIterator end ) {
			while (begin != end) {
				predicate( executor, *begin++ );
			}
		} );
	}

// ---------------------------------------------------

	template <typename... WorkItems>
	ETInlineHint void JobExecutor::Await_( const char* file, int line, Function<void ( JobExecutor& )> workItem, WorkItems&&... workItems ) {
		Scheduling::JobFence	workCompleted( 0 );

		this->StartAsync( workCompleted, eastl::move( workItem ), eastl::forward<WorkItems>( workItems )... );

		Await_( file, line, workCompleted );
	}

// ---------------------------------------------------

	ETInlineHint void JobExecutor::Await_( const char* file, int line, JobFence& fence ) {
		return Await_( file, line, [&fence] () -> bool {
#		if ET_ENABLE_JOB_DEBUGGING
			const int	value( fence.load( std::memory_order_consume ) );

			ET_ASSERT( value >= 0, "Reference count mismatch for job fence!" );

			return value == 0;
#		else
			return fence.load( std::memory_order_consume ) == 0;
#		endif
		} );
	}

}	// namespace Scheduling
}	// namespace Eldritch2