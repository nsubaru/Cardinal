//export module Cardinal.Threading:Async.Awaitable;
//
//export import :Async.Base;
//export import :UnifiedThreadPool;
//
//export namespace Cardinal::Threading
//{
//	class AwaitablePayload;
//
//	template<typename T>
//	concept TAwaitable = Concepts::IsBasedOn<IAwaitable, T>;
//
//	/// <summary>
//	/// The waiting task object with result type "T"
//	/// </summary>
//	/// <typeparam name="T">The result type</typeparam>
//	template<typename T>
//	using AwaitableTask = BaseTask<T, AwaitablePayload>;
//
//	/// <summary>
//	/// Information class of waiting task
//	/// </summary>
//	class AwaitablePayload : public IDisposable {
//		struct Data {
//			ContextsBlock contexts;
//			Handle hAwaitable;
//			RefCountT RefCount;
//			UnifiedThreadPool pool;
//
//			Data(ContextsBlock&& contexts, UnifiedThreadPool pool)
//				: contexts(contexts), hAwaitable(nullptr), pool(pool), RefCount(1) {}
//		};
//
//	private:
//		Data* This;
//
//	protected:
//		virtual void Dispose() override;
//
//	protected:
//		AwaitablePayload(UnifiedThreadPool threadPool);
//
//	public:
//		bool IsSetted() const;
//
//	public:
//		RefCountClass(AwaitablePayload, This);
//
//	private:
//		friend ContextsBlock& GetContext(AwaitablePayload& payload);
//
//		friend void InitAwaitableThreadable(const IAwaitable& awaitable, IPayloadBaseTask<AwaitablePayload>& task);
//		friend void RunAwaitableThreadable(IPayloadBaseTask<AwaitablePayload>*&& task);
//
//	public:
//		template<TAwaitable T>
//		friend AwaitableTask<void> AsAwaitableTask(const T& awaitable, UnifiedThreadPool&& threadPool);
//
//		template<TAwaitable T>
//		friend AwaitableTask<void> AsAwaitableTask(const T& awaitable, const UnifiedThreadPool& threadPool);
//
//		template<TAwaitable T>
//		friend AwaitableTask<void> AsAwaitableTask(T&& awaitable, UnifiedThreadPool&& threadPool);
//
//		template<TAwaitable T>
//		friend AwaitableTask<void> AsAwaitableTask(T&& awaitable, const UnifiedThreadPool& threadPool);
//
//		template<TAwaitable T, typename U>
//		friend AwaitableTask<U> AsAwaitableTask(const U& value, const T& awaitable, UnifiedThreadPool&& threadPool);
//
//		template<TAwaitable T, typename U>
//		friend AwaitableTask<U> AsAwaitableTask(const U& value, const T& awaitable, const UnifiedThreadPool& threadPool);
//
//		template<TAwaitable T, typename U>
//		friend AwaitableTask<U> AsAwaitableTask(const U& value, T&& awaitable, UnifiedThreadPool&& threadPool);
//
//		template<TAwaitable T, typename U>
//		friend AwaitableTask<U> AsAwaitableTask(const U& value, T&& awaitable, const UnifiedThreadPool& threadPool);
//
//		template<TAwaitable T, typename U>
//		friend AwaitableTask<U> AsAwaitableTask(U&& value, const T& awaitable, UnifiedThreadPool&& threadPool);
//
//		template<TAwaitable T, typename U>
//		friend AwaitableTask<U> AsAwaitableTask(U&& value, const T& awaitable, const UnifiedThreadPool& threadPool);
//
//		template<TAwaitable T, typename U>
//		friend AwaitableTask<U> AsAwaitableTask(U&& value, T&& awaitable, UnifiedThreadPool&& threadPool);
//
//		template<TAwaitable T, typename U>
//		friend AwaitableTask<U> AsAwaitableTask(U&& value, T&& awaitable, const UnifiedThreadPool& threadPool);
//
//		template<typename U>
//		friend AwaitableTask<Task<U>> AsAwaitableTask(Task<U>&& task, UnifiedThreadPool&& threadPool);
//
//		template<typename U>
//		friend AwaitableTask<Task<U>> AsAwaitableTask(Task<U>&& task, const UnifiedThreadPool& threadPool);
//
//		template<typename T, typename TPayload>
//		friend struct TaskAwaiter;
//	};
//#pragma region AsAwaitableTask void with threadPool
//	/// <summary>
//	/// Asynchronous object wait
//	/// </summary>
//	/// <param name="awaitable">The object which is need to wait</param>
//	/// <param name="threadPool">Task pool which performs the await</param>
//	/// <returns>Await task object</returns>
//	template<TAwaitable T>
//	inline AwaitableTask<void> AsAwaitableTask(const T& awaitable, UnifiedThreadPool&& threadPool) {
//		auto task = AwaitableTask<void>(AwaitablePayload(MoveRValue(threadPool)));
//		InitAwaitableThreadable(static_cast<const IAwaitable&>(awaitable), task);
//		return task;
//	}
//
//	/// <summary>
//	/// Asynchronous object wait
//	/// </summary>
//	/// <param name="awaitable">The object which is need to wait</param>
//	/// <param name="threadPool">Task pool which performs the await</param>
//	/// <returns>Await task object</returns>
//	template<TAwaitable T>
//	inline AwaitableTask<void> AsAwaitableTask(const T& awaitable, const UnifiedThreadPool& threadPool) {
//		auto task = AwaitableTask<void>(AwaitablePayload(UnifiedThreadPool(threadPool)));
//		InitAwaitableThreadable(static_cast<const IAwaitable&>(awaitable), task);
//		return task;
//	}
//
//	/// <summary>
//	/// Asynchronous object wait
//	/// </summary>
//	/// <param name="awaitable">The object which is need to wait</param>
//	/// <param name="threadPool">Task pool which performs the await</param>
//	/// <returns>Await task object</returns>
//	template<TAwaitable T>
//	inline AwaitableTask<void> AsAwaitableTask(T&& awaitable, UnifiedThreadPool&& threadPool) {
//		auto task = AwaitableTask<void>(AwaitablePayload(MoveRValue(threadPool)));
//		InitAwaitableThreadable(static_cast<const IAwaitable&>(awaitable), task);
//		return task;
//	}
//
//	/// <summary>
//	/// Asynchronous object wait
//	/// </summary>
//	/// <param name="awaitable">The object which is need to wait</param>
//	/// <param name="threadPool">Task pool which performs the await</param>
//	/// <returns>Await task object</returns>
//	template<TAwaitable T>
//	inline AwaitableTask<void> AsAwaitableTask(T&& awaitable, const UnifiedThreadPool& threadPool) {
//		auto task = AwaitableTask<void>(AwaitablePayload(UnifiedThreadPool(threadPool)));
//		InitAwaitableThreadable(static_cast<const IAwaitable&>(awaitable), task);
//		return task;
//	}
//
//#pragma endregion
//
//#pragma region AsAwaitableTask with value, awaitable and threadPool
//	/// <summary>
//	/// Asynchronous object wait
//	/// </summary>
//	/// <typeparam name="U">Task result type</typeparam>
//	/// <param name="value">Task result value</param>
//	/// <param name="awaitable">Awaitable object</param>
//	/// <param name="threadPool">Pool for asynchronous object wait</param>
//	/// <returns>Awaitable task</returns>
//	template<TAwaitable T, typename U>
//	inline AwaitableTask<U> AsAwaitableTask(const U& value, const T& awaitable, UnifiedThreadPool&& threadPool) {
//		auto task = AwaitableTask<U>(AwaitablePayload(MoveRValue(threadPool)), value);
//		InitAwaitableThreadable(static_cast<const IAwaitable&>(awaitable), task);
//		return task;
//	}
//
//	/// <summary>
//	/// Asynchronous object wait
//	/// </summary>
//	/// <typeparam name="U">Task result type</typeparam>
//	/// <param name="value">Task result value</param>
//	/// <param name="awaitable">Awaitable object</param>
//	/// <param name="threadPool">Pool for asynchronous object wait</param>
//	/// <returns>Awaitable task</returns>
//	template<TAwaitable T, typename U>
//	inline AwaitableTask<U> AsAwaitableTask(U&& value, const T& awaitable, UnifiedThreadPool&& threadPool) {
//		auto task = AwaitableTask<U>(AwaitablePayload(MoveRValue(threadPool)), MoveRValue(value));
//		InitAwaitableThreadable(static_cast<const IAwaitable&>(awaitable), task);
//		return task;
//	}
//
//	/// <summary>
//	/// Asynchronous object wait
//	/// </summary>
//	/// <typeparam name="U">Task result type</typeparam>
//	/// <param name="value">Task result value</param>
//	/// <param name="awaitable">Awaitable object</param>
//	/// <param name="threadPool">Pool for asynchronous object wait</param>
//	/// <returns>Awaitable task</returns>
//	template<TAwaitable T, typename U>
//	inline AwaitableTask<U> AsAwaitableTask(const U& value, T&& awaitable, UnifiedThreadPool&& threadPool) {
//		auto task = AwaitableTask<U>(AwaitablePayload(MoveRValue(threadPool)), value);
//		InitAwaitableThreadable(static_cast<const IAwaitable&>(awaitable), task);
//		return task;
//	}
//
//	/// <summary>
//	/// Asynchronous object wait
//	/// </summary>
//	/// <typeparam name="U">Task result type</typeparam>
//	/// <param name="value">Task result value</param>
//	/// <param name="awaitable">Awaitable object</param>
//	/// <param name="threadPool">Pool for asynchronous object wait</param>
//	/// <returns>Awaitable task</returns>
//	template<TAwaitable T, typename U>
//	inline AwaitableTask<U> AsAwaitableTask(U&& value, T&& awaitable, UnifiedThreadPool&& threadPool) {
//		auto task = AwaitableTask<U>(AwaitablePayload(MoveRValue(threadPool)), MoveRValue(value));
//		InitAwaitableThreadable(static_cast<const IAwaitable&>(awaitable), task);
//		return task;
//	}
//
//	/// <summary>
//	/// Asynchronous object wait
//	/// </summary>
//	/// <typeparam name="U">Task result type</typeparam>
//	/// <param name="value">Task result value</param>
//	/// <param name="awaitable">Awaitable object</param>
//	/// <param name="threadPool">Pool for asynchronous object wait</param>
//	/// <returns>Awaitable task</returns>
//	template<TAwaitable T, typename U>
//	inline AwaitableTask<U> AsAwaitableTask(const U& value, const T& awaitable, const UnifiedThreadPool& threadPool) {
//		auto task = AwaitableTask<U>(AwaitablePayload(UnifiedThreadPool(threadPool)), value);
//		InitAwaitableThreadable(static_cast<const IAwaitable&>(awaitable), task);
//		return task;
//	}
//
//	/// <summary>
//	/// Asynchronous object wait
//	/// </summary>
//	/// <typeparam name="U">Task result type</typeparam>
//	/// <param name="value">Task result value</param>
//	/// <param name="awaitable">Awaitable object</param>
//	/// <param name="threadPool">Pool for asynchronous object wait</param>
//	/// <returns>Awaitable task</returns>
//	template<TAwaitable T, typename U>
//	inline AwaitableTask<U> AsAwaitableTask(U&& value, const T& awaitable, const UnifiedThreadPool& threadPool) {
//		auto task = AwaitableTask<U>(AwaitablePayload(UnifiedThreadPool(threadPool)), MoveRValue(value));
//		InitAwaitableThreadable(static_cast<const IAwaitable&>(awaitable), task);
//		return task;
//	}
//
//	/// <summary>
//	/// Asynchronous object wait
//	/// </summary>
//	/// <typeparam name="U">Task result type</typeparam>
//	/// <param name="value">Task result value</param>
//	/// <param name="awaitable">Awaitable object</param>
//	/// <param name="threadPool">Pool for asynchronous object wait</param>
//	/// <returns>Awaitable task</returns>
//	template<TAwaitable T, typename U>
//	inline AwaitableTask<U> AsAwaitableTask(const U& value, T&& awaitable, const UnifiedThreadPool& threadPool) {
//		auto task = AwaitableTask<U>(AwaitablePayload(UnifiedThreadPool(threadPool)), value);
//		InitAwaitableThreadable(static_cast<const IAwaitable&>(awaitable), task);
//		return task;
//	}
//
//	/// <summary>
//	/// Asynchronous object wait
//	/// </summary>
//	/// <typeparam name="U">Task result type</typeparam>
//	/// <param name="value">Task result value</param>
//	/// <param name="awaitable">Awaitable object</param>
//	/// <param name="threadPool">Pool for asynchronous object wait</param>
//	/// <returns>Awaitable task</returns>
//	template<TAwaitable T, typename U>
//	inline AwaitableTask<U> AsAwaitableTask(U&& value, T&& awaitable, const UnifiedThreadPool& threadPool) {
//		auto task = AwaitableTask<U>(AwaitablePayload(UnifiedThreadPool(threadPool)), MoveRValue(value));
//		InitAwaitableThreadable(static_cast<const IAwaitable&>(awaitable), task);
//		return task;
//	}
//#pragma endregion
//
//#pragma region AsAwaitableTask with value and awaitable
//	/// <summary>
//	/// Asynchronous object wait
//	/// </summary>
//	/// <typeparam name="U">Task result type</typeparam>
//	/// <param name="value">Task result value</param>
//	/// <param name="awaitable">Awaitable object</param>
//	/// <returns>Awaitable task</returns>
//	template<TAwaitable T, typename U> requires (!Concepts::IsBasedOn<IBaseTask, T> && !Concepts::IsSame<U, UnifiedThreadPool>)
//		inline AwaitableTask<U> AsAwaitableTask(const U& value, const T& awaitable) {
//		auto task = AwaitableTask<U>(AwaitablePayload(UnifiedThreadPool(Contexts::GetContext<ThreadPoolThreadContext>().ThreadPool)), value);
//		InitAwaitableThreadable(static_cast<const IAwaitable&>(awaitable), task);
//		return task;
//	}
//
//	/// <summary>
//	/// Asynchronous object wait
//	/// </summary>
//	/// <typeparam name="U">Task result type</typeparam>
//	/// <param name="value">Task result value</param>
//	/// <param name="awaitable">Awaitable object</param>
//	/// <returns>Awaitable task</returns>
//	template<TAwaitable T, typename U> requires (!Concepts::IsBasedOn<IBaseTask, T> && !Concepts::IsSame<U, UnifiedThreadPool>)
//		inline AwaitableTask<U> AsAwaitableTask(U&& value, const T& awaitable) {
//		auto task = AwaitableTask<U>(AwaitablePayload(UnifiedThreadPool(Contexts::GetContext<ThreadPoolThreadContext>().ThreadPool)), MoveRValue(value));
//		InitAwaitableThreadable(static_cast<const IAwaitable&>(awaitable), task);
//		return task;
//	}
//
//	/// <summary>
//	/// Asynchronous object wait
//	/// </summary>
//	/// <typeparam name="U">Task result type</typeparam>
//	/// <param name="value">Task result value</param>
//	/// <param name="awaitable">Awaitable object</param>
//	/// <returns>Awaitable task</returns>
//	template<TAwaitable T, typename U> requires (!Concepts::IsBasedOn<IBaseTask, T> && !Concepts::IsSame<U, UnifiedThreadPool>)
//		inline AwaitableTask<void> AsAwaitableTask(const U& value, T&& awaitable) {
//		auto task = AwaitableTask<U>(AwaitablePayload(UnifiedThreadPool(Contexts::GetContext<ThreadPoolThreadContext>().ThreadPool)), value);
//		InitAwaitableThreadable(static_cast<const IAwaitable&>(awaitable), task);
//		return task;
//	}
//
//	/// <summary>
//	/// Asynchronous object wait
//	/// </summary>
//	/// <typeparam name="U">Task result type</typeparam>
//	/// <param name="value">Task result value</param>
//	/// <param name="awaitable">Awaitable object</param>
//	/// <returns>Awaitable task</returns>
//	template<TAwaitable T, typename U> requires (!Concepts::IsBasedOn<IBaseTask, T> && !Concepts::IsSame<U, UnifiedThreadPool>)
//		inline AwaitableTask<U> AsAwaitableTask(U&& value, T&& awaitable) {
//		auto task = AwaitableTask<U>(AwaitablePayload(UnifiedThreadPool(Contexts::GetContext<ThreadPoolThreadContext>().ThreadPool)), MoveRValue(value));
//		InitAwaitableThreadable(static_cast<const IAwaitable&>(awaitable), task);
//		return task;
//	}
//#pragma endregion
//
//	/// <summary>
//	/// Waiting for task completing
//	/// </summary>
//	/// <typeparam name="U">The task result type</typeparam>
//	/// <param name="awaitable">Task object</param>
//	/// <param name="threadPool">Task pool which performs the await</param>
//	/// <returns>Await task object</returns>
//	template<typename U>
//	inline AwaitableTask<Task<U>> AsAwaitableTask(Task<U>&& awaitable, UnifiedThreadPool&& threadPool) {
//		auto task = AwaitableTask<Task<U>>(AwaitablePayload(MoveRValue(threadPool)), awaitable);
//		InitAwaitableThreadable(static_cast<const IAwaitable&>(awaitable), task);
//		return task;
//	}
//
//	/// <summary>
//	/// Waiting for task completing
//	/// </summary>
//	/// <typeparam name="U">The task result type</typeparam>
//	/// <param name="awaitable">Task object</param>
//	/// <param name="threadPool">Task pool which performs the await</param>
//	/// <returns>Await task object</returns>
//	template<typename U>
//	inline AwaitableTask<Task<U>> AsAwaitableTask(Task<U>&& awaitable, const UnifiedThreadPool& threadPool) {
//		auto task = AwaitableTask<Task<U>>(AwaitablePayload(UnifiedThreadPool(threadPool)), awaitable);
//		InitAwaitableThreadable(static_cast<const IAwaitable&>(awaitable), task);
//		return task;
//	}
//}
//
//namespace Cardinal::Threading
//{
//	/// <summary>
//	/// Task state machine type
//	/// </summary>
//	/// <typeparam name="T">Task result type</typeparam>
//	template<typename T>
//	struct TaskAwaiter<Task<T>, AwaitablePayload> {
//		AwaitableTask<Task<T>> task;
//
//		/// <summary>
//		/// Called when before init async operation
//		/// </summary>
//		/// <returns>If true then result alredy available and async operation not neaded</returns>
//		bool await_ready() const {
//			return task.GetPayload().IsSetted();
//		}
//
//		/// <summary>
//		/// Called for init async operation
//		/// </summary>
//		/// <param name="resume_cb">Function context(Data stack)</param>
//		void await_suspend(std::coroutine_handle<> resume_cb) {
//			RunAwaitableThreadable(new AwaitableTask<Task<T>>(task));
//			task.SetContext(resume_cb.address());
//		}
//
//		/// <summary>
//		/// Called for complete async operation
//		/// </summary>
//		/// <returns>Task result</returns>
//		decltype(auto) await_resume() {
//			return MoveRValue(task.GetValue()).GetValue();
//		}
//	};
//
//	/// <summary>
//	/// Task state machine type
//	/// </summary>
//	/// <typeparam name="T">Awaitable result type</typeparam>
//	template<typename T>
//	struct TaskAwaiter<T, AwaitablePayload> {
//		AwaitableTask<T> task;
//
//		/// <summary>
//		/// Called when before init async operation
//		/// </summary>
//		/// <returns>If true then result alredy available and async operation not neaded</returns>
//		bool await_ready() const {
//			return task.GetPayload().IsSetted();
//		}
//
//		/// <summary>
//		/// Called for init async operation
//		/// </summary>
//		/// <param name="resume_cb">Function context(Data stack)</param>
//		void await_suspend(std::coroutine_handle<> resume_cb) {
//			RunAwaitableThreadable(new AwaitableTask<T>(task));
//			task.SetContext(resume_cb.address());
//		}
//
//		/// <summary>
//		/// Called for complete async operation
//		/// </summary>
//		/// <returns>Task result</returns>
//		decltype(auto) await_resume() {
//			return MoveRValue(task).GetValue();
//		}
//	};
//
//	/// <summary>
//	/// Task state machine type
//	/// </summary>
//	template<>
//	struct TaskAwaiter<void, AwaitablePayload> {
//		AwaitableTask<void> task;
//
//		/// <summary>
//		/// Called when before init async operation
//		/// </summary>
//		/// <returns>If true then result alredy available and async operation not neaded</returns>
//		bool await_ready() const {
//			return task.GetPayload().IsSetted();
//		}
//
//		/// <summary>
//		/// Called for init async operation
//		/// </summary>
//		/// <param name="resume_cb">Function context(Data stack)</param>
//		void await_suspend(std::coroutine_handle<> resume_cb) {
//			RunAwaitableThreadable(new AwaitableTask<void>(task));
//			task.SetContext(resume_cb.address());
//		}
//
//		/// <summary>
//		/// Called for complete async operation
//		/// </summary>
//		void await_resume() {
//			task.GetValue();
//		}
//	};
//}
//
///// <summary>
///// The launching of asynchronous awaiting on task
///// </summary>
///// <typeparam name="T">The task result type</typeparam>
///// <param name="task">Task object</param>
///// <returns>Task result</returns>
//template<typename T>
//inline decltype(auto) operator co_await(Cardinal::Threading::Task<T>&& task) {
//	using namespace Cardinal;
//	using namespace Cardinal::Threading;
//
//	return TaskAwaiter<Task<T>, AwaitablePayload>{
//		AsAwaitableTask(
//			MoveRValue(task),
//			Contexts::GetContext<ThreadPoolThreadContext>().ThreadPool)
//	};
//}
//
///// <summary>
///// Asynchronous awaiting for event
///// </summary>
///// <param name="event">Event object</param>
///// <returns>Void</returns>
//inline decltype(auto) operator co_await(const Cardinal::Threading::SynchronizeEvent& event) {
//	using namespace Cardinal;
//	using namespace Cardinal::Threading;
//
//	return TaskAwaiter<void, AwaitablePayload>{
//		AsAwaitableTask<SynchronizeEvent>(
//			event,
//			Contexts::GetContext<ThreadPoolThreadContext>().ThreadPool)
//	};
//}
//
///// <summary>
///// Asynchronous awaiting for event
///// </summary>
///// <param name="event">Event object</param>
///// <returns>Void</returns>
//inline decltype(auto) operator co_await(Cardinal::Threading::SynchronizeEvent&& event) {
//	using namespace Cardinal;
//	using namespace Cardinal::Threading;
//
//	return TaskAwaiter<void, AwaitablePayload>{
//		AsAwaitableTask<SynchronizeEvent>(
//			MoveRValue(event),
//			Contexts::GetContext<ThreadPoolThreadContext>().ThreadPool)
//	};
//}
//
///// <summary>
///// Asynchronous object await which supports awaiting
///// </summary>
///// <param name="awaitable">An object which supports awaiting</param>
///// <returns>Void</returns>
//template<Cardinal::Threading::TAwaitable TAwaitable>
//inline decltype(auto) operator co_await(const TAwaitable& awaitable) {
//	using namespace Cardinal;
//	using namespace Cardinal::Threading;
//
//	return TaskAwaiter<void, AwaitablePayload>{
//		AsAwaitableTask(
//			awaitable,
//			Contexts::GetContext<ThreadPoolThreadContext>().ThreadPool)
//	};
//}
//
///// <summary>
///// Asynchronous object await which supports awaiting 
///// </summary>
///// <param name="awaitable">An object which supports awaiting</param>
///// <returns>Void</returns>
//template<Cardinal::Threading::TAwaitable TAwaitable>
//inline decltype(auto) operator co_await(TAwaitable&& awaitable) {
//	using namespace Cardinal;
//	using namespace Cardinal::Threading;
//
//	return TaskAwaiter<void, AwaitablePayload>{
//		AsAwaitableTask(
//			MoveRValue(awaitable),
//			Contexts::GetContext<ThreadPoolThreadContext>().ThreadPool)
//	};
//}