export module Cardinal.Threading:Task;

export import Cardinal.Functionality;

export import :Base;
export import :IThreadable;
export import :Context;
export import :Exceptions;

export import :SynchronizeEvent;

#pragma warning(push)
#pragma warning(disable: 4996)

export namespace Cardinal::Threading
{
	template<typename T, typename TPayload>
	struct TaskAwaiter;

	/// <summary>
	/// Asynchronous task state
	/// </summary>
	enum EBaseTaskState {
		NotStarted,
		Initialized,
		Starting,
		Started,
		Executed,
		ResultRead
	};

	/// <summary>
	/// Base interface with additional information in asynchronous task
	/// </summary>
	/// <typeparam name="TPayload">Additional information type in asynchronous task</typeparam>
	template<typename TPayload, bool TRequireInitAndStop = false>
	class ITaskPayload {
	public:
		template<typename T, typename TPayload>
		friend struct TaskAwaiter;

	protected:
		constexpr static Boolean RequireInitAndStop = TRequireInitAndStop;

	public:
		/// <summary>
		/// Destructor
		/// </summary>
		virtual ~ITaskPayload() = 0 {};
	};

	template<typename TPayload>
	concept IsTaskPayloadRequireInitAndStop = Concepts::IsBasedOn<ITaskPayload<TPayload, true>, TPayload>;

	/// <summary>
	/// Base task interface
	/// </summary>
	class IBaseTask : public IDisposable {
	public:
		/// <summary>
		/// Sets the task context
		/// </summary>
		/// <param name="context">New context value</param>
		virtual void SetContext(Handle context) = 0;

		/// <summary>
		/// Returns the context value
		/// </summary>
		/// <returns>Context value</returns>
		virtual Handle GetContext() = 0;

		/// <summary>
		/// Marks the task as completed
		/// </summary>
		virtual void MarkFinished() = 0;

		/// <summary>
		/// Sets the task exception
		/// </summary>
		/// <param name="ex">The exception object</param>
		virtual void SetException(Exception::RawException&& ex) = 0;

	public:
		virtual ~IBaseTask() override = default;
	};

	/// <summary>
	/// Task interface with the additional arguments type
	/// </summary>
	/// <typeparam name="TPayload">Additional arguments type</typeparam>
	template<typename TPayload>
	class IPayloadBaseTask : public IBaseTask {
	public:
		/// <summary>
		/// Returns the additional arguments
		/// </summary>
		/// <returns>Additional arguments</returns>
		virtual TPayload& GetPayload() = 0;

		/// <summary>
		/// Returns the additional arguments
		/// </summary>
		/// <returns>Additional arguments</returns>
		virtual const TPayload& GetPayload() const = 0;
	};

	/// <summary>
	/// Task object with additional arguments and result
	/// </summary>
	/// <typeparam name="T">The task result type</typeparam>
	/// <typeparam name="TPayload">Additional arguments type</typeparam>
	template<typename T, typename TPayload>
	class BaseTask final : public IPayloadBaseTask<TPayload> {
	private:
		struct Data {
			volatile void* context;
			TPayload payload;

			SynchronizeEvent finishEvent;

			Functionality::Optional<Functionality::UnionType<T, Exception::RawException>> data;
			RefCountT RefCount;

			Data(TPayload&& payload)
				: context(nullptr),
				payload(MoveRValue(payload)),
				finishEvent(true),
				data(),
				RefCount(1) {
			}

			Data(const TPayload& payload)
				: context(nullptr),
				payload(payload),
				finishEvent(true),
				data(),
				RefCount(1) {
			}

			Data(TPayload&& payload, const T& data)
				: context(nullptr),
				payload(MoveRValue(payload)),
				finishEvent(true),
				data(Functionality::UnionType<T, Exception::RawException>(data)),
				RefCount(1) {
			}

			Data(TPayload&& payload, T&& data)
				: context(nullptr),
				payload(MoveRValue(payload)),
				finishEvent(true),
				data(Functionality::UnionType<T, Exception::RawException>(MoveRValue(data))),
				RefCount(1) {
			}

			Data(const TPayload& payload, const T& data)
				: context(nullptr),
				payload(payload),
				finishEvent(true),
				data(Functionality::UnionType<T, Exception::RawException>(data)),
				RefCount(1) {
			}

			Data(const TPayload& payload, T&& data)
				: context(nullptr),
				payload(payload),
				finishEvent(true),
				data(Functionality::UnionType<T, Exception::RawException>(MoveRValue(data))),
				RefCount(1) {
			}
		};

	private:
		Data* This;

	protected:
		virtual void Dispose() override {}

	public:
		/// <summary>
		/// Constructor with additional arguments
		/// </summary>
		/// <param name="payload">Additional arguments</param>
		BaseTask(const TPayload& payload) : This(new Data(payload)) {}

		/// <summary>
		/// Constructor with additional arguments
		/// </summary>
		/// <param name="payload">Additional arguments</param>
		BaseTask(TPayload&& payload) : This(new Data(MoveRValue(payload))) {}

		/// <summary>
		/// Constructor with additional arguments and result
		/// </summary>
		/// <param name="payload">Additional arguments</param>
		/// <param name="data">Task result</param>
		BaseTask(TPayload&& payload, const T& data) : This(new Data(MoveRValue(payload), data)) {}

		/// <summary>
		/// Constructor with additional arguments and result
		/// </summary>
		/// <param name="payload">Additional arguments</param>
		/// <param name="data">Task result</param>
		BaseTask(const TPayload& payload, const T& data) : This(new Data(payload, data)) {}

		/// <summary>
		/// Constructor with additional arguments and result
		/// </summary>
		/// <param name="payload">Additional arguments</param>
		/// <param name="data">Task result</param>
		BaseTask(TPayload&& payload, T&& data) : This(new Data(MoveRValue(payload), MoveRValue(data))) {}

		/// <summary>
		/// Constructor with additional arguments and result
		/// </summary>
		/// <param name="payload">Additional arguments</param>
		/// <param name="data">Task result</param>
		BaseTask(const TPayload& payload, T&& data) : This(new Data(payload, MoveRValue(const_cast<T&&>(data)))) {}

	public:
		/// <summary>
		/// Returns the result or exception which the task contains
		/// </summary>
		/// <returns>Result or exception which the task contains</returns>
		T GetValue()&& {
			This->finishEvent.Wait();

			if (This->RefCount == 1)
			{
				if (This->data.Value.HoldedTypeNumber == 0)
				{
					return MoveRValue(MoveRValue(This->data).Value).template GetAs<T>();
				}
				else
				{
					MoveRValue(MoveRValue(This->data).Value).template GetAs<Exception::RawException>().ThrowThis();
				}
			}
			else
			{
				if (This->data.Value.HoldedTypeNumber == 0)
				{
					return This->data.Value.template GetAs<T>();
				}
				else
				{
					This->data.Value.template GetAs<Exception::RawException>().ThrowThis();
				}
			}
		}

		/// <summary>
		/// Returns the result or exception which the task contains
		/// </summary>
		/// <returns>Result or exception which the task contains</returns>
		T& GetValue()& {
			This->finishEvent.Wait();

			if (This->data.Value.HoldedTypeNumber == 0)
			{
				return This->data.Value.template GetAs<T>();
			}
			else
			{
				This->data.Value.template GetAs<Exception::RawException>().ThrowThis();
			}
		}

		/// <summary>
		/// Returns the result or exception which the task contains
		/// </summary>
		/// <returns>Result or exception which the task contains</returns>
		const T& GetValue() const& {
			This->finishEvent.Wait();

			if (This->data.Value.HoldedTypeNumber == 0)
			{
				return This->data.Value.template GetAs<T>();
			}
			else
			{
				This->data.Value.template GetAs<Exception::RawException>().ThrowThis();
			}
		}

		/// <summary>
		/// Returns the buffer which is added to task object
		/// </summary>
		T& GetTaskDataBuffer()& {
			if (This->data.Value.HoldedTypeNumber == 0)
			{
				return This->data.Value.template GetAs<T>();
			}
			else
			{
				Throw(NullPointException(SourcePosition));
			}
		}

	public:
		/// <summary>
		/// Sets the new task result
		/// </summary>
		/// <param name="data">New task result</param>
		void SetValue(const T& data) {
			This->data.Value = Functionality::UnionType<T, Exception::RawException>(data);
		}

		/// <summary>
		/// Sets the new task result
		/// </summary>
		/// <param name="data">New task result</param>
		void SetValue(T&& data) {
			This->data.Value = Functionality::UnionType<T, Exception::RawException>(ForwardReference<T>(data));
		}

		/// <summary>
		/// Sets the task exception
		/// </summary>
		/// <param name="data">Task exception object</param>
		virtual void SetException(Exception::RawException&& data) override {
			This->data.Value = Functionality::UnionType<T, Exception::RawException>(ForwardReference<Exception::RawException>(data));
		}

		/// <summary>
		/// Marks the task as completed
		/// </summary>
		virtual void MarkFinished() override {
			This->finishEvent.Set();
		}

	public:
		/// <summary>
		/// Sets the task context
		/// </summary>
		/// <param name="context">Task context</param>
		virtual void SetContext(Handle context) override {
			if (Interlocked::CompareAndSwap(This->context, context, static_cast<void*>(nullptr)) != nullptr)
			{
				Throw(InvalidArgumentException(SourcePosition, L"context", L"context alredy setted"));
			}
		}

		/// <summary>
		/// Returns the task context
		/// </summary>
		/// <returns>Task context object</returns>
		virtual Handle GetContext() override {
			return Interlocked::Load(This->context);
		}

	public:
		/// <summary>
		/// Returns the additional arguments of task
		/// </summary>
		/// <returns>Additional arguments of task</returns>
		virtual TPayload& GetPayload() override { return This->payload; }

		/// <summary>
		/// Returns the additional arguments of task
		/// </summary>
		/// <returns>Additional arguments of task</returns>
		virtual const TPayload& GetPayload() const override { return This->payload; }

	public:
		template<typename T, typename TPayload>
		friend struct TaskAwaiter;

	public:
		RefCountClass(BaseTask, This);
	};

	/// <summary>
	/// Task object only with result and without additional arguments
	/// </summary>
	/// <typeparam name="T">The task result type</typeparam>
	template<typename T>
	class BaseTask<T, void> final : public IBaseTask, public IAwaitable {
	private:
		struct Data {
			volatile void* context;

			SynchronizeEvent finishEvent;

			Functionality::Optional<Functionality::UnionType<T, Exception::RawException>> data;
			RefCountT RefCount;

			Data()
				: context(nullptr),
				finishEvent(true),
				data(),
				RefCount(1) {
			}

			Data(const T& data)
				: context(nullptr),
				finishEvent(true),
				data(Functionality::UnionType<T, Exception::RawException>(data)),
				RefCount(1) {
			}

			Data(T&& data)
				: context(nullptr),
				finishEvent(true),
				data(Functionality::UnionType<T, Exception::RawException>(MoveRValue(data))),
				RefCount(1) {
			}
		};

	private:
		Data* This;

	protected:
		virtual void Dispose() override {}

	protected:
		virtual Handle GetAwaitableHandle() const override {
			return (Handle) This->finishEvent;
		}

	public:
		/// <summary>
		/// Constructor without task result
		/// </summary>
		BaseTask() : This(new Data()) {}

		/// <summary>
		/// Constructor with task result
		/// </summary>
		/// <param name="data">Task result</param>
		BaseTask(const T& data) : This(new Data(data)) {}

		/// <summary>
		/// Constructor with task result
		/// </summary>
		/// <param name="data">Task result</param>
		BaseTask(T&& data) : This(new Data(data)) {}

	public:
		/// <summary>
		/// Returns the result or exception which the task contains
		/// </summary>
		/// <returns>Result or exception which the task contains</returns>
		T GetValue()&& {
			This->finishEvent.Wait();

			if (This->RefCount == 1)
			{
				if (This->data.Value.HoldedTypeNumber == 0)
				{
					return MoveRValue(MoveRValue(This->data).Value).GetAs<T>();
				}
				else
				{
					MoveRValue(MoveRValue(This->data).Value).GetAs<Exception::RawException>().ThrowThis();
				}
			}
			else
			{
				if (This->data.Value.HoldedTypeNumber == 0)
				{
					return This->data.Value.GetAs<T>();
				}
				else
				{
					This->data.Value.GetAs<Exception::RawException>().ThrowThis();
				}
			}
		}

		/// <summary>
		/// Returns the result or exception which the task contains
		/// </summary>
		/// <returns>Result or exception which the task contains</returns>
		T& GetValue()& {
			This->finishEvent.Wait();

			if (This->data.Value.HoldedTypeNumber == 0)
			{
				return This->data.Value.GetAs<T>();
			}
			else
			{
				This->data.Value.GetAs<Exception::RawException>().ThrowThis();
			}
		}

		/// <summary>
		/// Returns the result or exception which the task contains
		/// </summary>
		/// <returns>Result or exception which the task contains</returns>
		const T& GetValue() const& {
			This->finishEvent.Wait();

			if (This->data.Value.HoldedTypeNumber == 0)
			{
				return This->data.Value.GetAs<T>();
			}
			else
			{
				This->data.Value.GetAs<Exception::RawException>().ThrowThis();
			}
		}

		/// <summary>
		/// Returns the buffer which is added to task object
		/// </summary>
		T& GetTaskDataBuffer()& {
			if (This->data.Value.HoldedTypeNumber == 0)
			{
				return This->data.Value.template GetAs<T>();
			}
			else
			{
				Throw(NullPointException(SourcePosition));
			}
		}

	public:
		/// <summary>
		/// Sets the new task result
		/// </summary>
		/// <param name="data">New task result</param>
		void SetValue(const T& data) {
			This->data.Value = Functionality::UnionType<T, Exception::RawException>(data);
		}

		/// <summary>
		/// Sets the new task result
		/// </summary>
		/// <param name="data">New task result</param>
		void SetValue(T&& data) {
			This->data.Value = Functionality::UnionType<T, Exception::RawException>(ForwardReference<T>(data));
		}

		/// <summary>
		/// Sets the task exception
		/// </summary>
		/// <param name="data">Task exception object</param>
		virtual void SetException(Exception::RawException&& data) override {
			This->data.Value = Functionality::UnionType<T, Exception::RawException>(ForwardReference<Exception::RawException>(data));
		}

		/// <summary>
		/// Marks the task as completed
		/// </summary>
		virtual void MarkFinished() override {
			This->finishEvent.Set();
		}

	public:
		/// <summary>
		/// Sets the task context
		/// </summary>
		/// <param name="context">Task context</param>
		virtual void SetContext(Handle context) override {
			if (Interlocked::CompareAndSwap(This->context, context, static_cast<void*>(nullptr)) != nullptr)
			{
				Throw(InvalidArgumentException(SourcePosition, L"context", L"context alredy setted"));
			}
		}

		/// <summary>
		/// Returns the task context
		/// </summary>
		/// <returns>Task context object</returns>
		virtual Handle GetContext() override {
			return Interlocked::Load(This->context);
		}

	public:
		template<typename T, typename TPayload>
		friend struct TaskAwaiter;

	public:
		RefCountClass(BaseTask, This);
	};

	/// <summary>
	/// Task object without the result and with additional arguments
	/// </summary>
	/// <typeparam name="TPayload">Additional arguments type</typeparam>
	template<typename TPayload>
	class BaseTask<void, TPayload> final : public IPayloadBaseTask<TPayload> {
	private:
		struct Data {
			volatile void* context;
			TPayload payload;

			SynchronizeEvent finishEvent;

			Functionality::Optional<Exception::RawException> data;
			RefCountT RefCount;

			Data(const TPayload& payload)
				: context(nullptr),
				payload(payload),
				finishEvent(true),
				data(),
				RefCount(1) {
			}

			Data(TPayload&& payload)
				: context(nullptr),
				payload(MoveRValue(payload)),
				finishEvent(true),
				data(),
				RefCount(1) {
			}
		};

	private:
		Data* This;

	protected:
		virtual void Dispose() override {}

	public:
		/// <summary>
		/// Constructor with additional arguments
		/// </summary>
		/// <param name="payload">Additional arguments</param>
		BaseTask(const TPayload& payload) : This(new Data(MoveRValue(payload))) {}

		/// <summary>
		/// Constructor with additional arguments
		/// </summary>
		/// <param name="payload">Additional arguments</param>
		BaseTask(TPayload&& payload) : This(new Data(MoveRValue(payload))) {}

	public:
		/// <summary>
		/// Receives the result or task exception
		/// </summary>
		/// <returns>"Void" or exception</returns>
		void GetValue()&& {
			This->finishEvent.Wait();

			if (This->RefCount == 1)
			{
				if (This->data.HasValue)
				{
					MoveRValue(MoveRValue(This->data).Value).ThrowThis();
				}
			}
			else
			{
				if (This->data.HasValue)
				{
					This->data.Value.ThrowThis();
				}
			}
		}

		/// <summary>
		/// Receives the result or task exception
		/// </summary>
		/// <returns>"Void" or exception</returns>
		void GetValue()& {
			This->finishEvent.Wait();

			if (This->data.HasValue)
			{
				This->data.Value.ThrowThis();
			}
		}

		/// <summary>
		/// Receives the result or task exception
		/// </summary>
		/// <returns>"Void" or exception</returns>
		void GetValue() const& {
			This->finishEvent.Wait();

			if (This->data.HasValue)
			{
				This->data.Value.ThrowThis();
			}
		}

	public:
		/// <summary>
		/// Sets the task exception
		/// </summary>
		/// <param name="data">Task exception object</param>
		virtual void SetException(Exception::RawException&& data) override {
			This->data.Value = ForwardReference<Exception::RawException>(data);
		}

		/// <summary>
		/// Marks the task as completed
		/// </summary>
		virtual void MarkFinished() override {
			This->finishEvent.Set();
		}

	public:
		/// <summary>
		/// Sets the task context
		/// </summary>
		/// <param name="context">Task context</param>
		virtual void SetContext(Handle context) override {
			if (Interlocked::CompareAndSwap(This->context, context, static_cast<void*>(nullptr)) != nullptr)
			{
				Throw(InvalidArgumentException(SourcePosition, L"context", L"context alredy setted"));
			}
		}

		/// <summary>
		/// Returns the task context
		/// </summary>
		/// <returns>Task context object</returns>
		virtual Handle GetContext() override {
			return Interlocked::Load(This->context);
		}

	public:
		/// <summary>
		/// Returns the additional arguments of task
		/// </summary>
		/// <returns>Additional arguments of task</returns>
		virtual TPayload& GetPayload() override { return This->payload; }

		/// <summary>
		/// Returns the additional arguments of task
		/// </summary>
		/// <returns>Additional arguments of task</returns>
		virtual const TPayload& GetPayload() const override { return This->payload; }

	public:
		template<typename T, typename TPayload>
		friend struct TaskAwaiter;

	public:
		RefCountClass(BaseTask, This)
	};

	/// <summary>
	/// Task object without the result and additional arguments
	/// </summary>
	template<>
	class BaseTask<void, void> final : public IBaseTask, public IAwaitable {
	private:
		struct Data {
			volatile void* context;

			SynchronizeEvent finishEvent;

			Functionality::Optional<Exception::RawException> data;
			RefCountT RefCount;

			Data()
				: context(nullptr),
				finishEvent(false),
				data(),
				RefCount(1) {
			}
		};

	private:
		Data* This;

	protected:
		virtual void Dispose() override {}

	protected:
		virtual Handle GetAwaitableHandle() const override {
			return (Handle) This->finishEvent;
		}

	public:
		/// <summary>
		/// Empty constructor
		/// </summary>
		BaseTask() : This(new Data()) {}

	public:
		/// <summary>
		/// Returns the additional arguments of task
		/// </summary>
		/// <returns>Additional arguments of task</returns>
		void GetValue() const&& {
			This->finishEvent.Wait();

			if (This->RefCount == 1)
			{
				if (This->data.HasValue)
				{
					MoveRValue(MoveRValue(This->data).Value).ThrowThis();
				}
			}
			else
			{
				if (This->data.HasValue)
				{
					This->data.Value.ThrowThis();
				}
			}
		}

		/// <summary>
		/// Returns the additional arguments of task
		/// </summary>
		/// <returns>Additional arguments of task</returns>
		void GetValue()& {
			This->finishEvent.Wait();

			if (This->data.HasValue)
			{
				This->data.Value.ThrowThis();
			}
		}

		/// <summary>
		/// Returns the additional arguments of task
		/// </summary>
		/// <returns>Additional arguments of task</returns>
		void GetValue() const& {
			This->finishEvent.Wait();

			if (This->data.HasValue)
			{
				This->data.Value.ThrowThis();
			}
		}

	public:
		/// <summary>
		/// Sets the task exception
		/// </summary>
		/// <param name="data">Task exception object</param>
		virtual void SetException(Exception::RawException&& data) override {
			This->data.Value = ForwardReference<Exception::RawException>(data);
		}

		/// <summary>
		/// Marks the task as completed
		/// </summary>
		virtual void MarkFinished() override {
			This->finishEvent.Set();
		}

	public:
		/// <summary>
		/// Sets the task context
		/// </summary>
		/// <param name="context">Task context</param>
		virtual void SetContext(const Handle context) override {
			if (Interlocked::CompareAndSwap(This->context, context, static_cast<void*>(nullptr)) != nullptr)
			{
				Throw(InvalidArgumentException(SourcePosition, L"context", L"context alredy setted"));
			}
		}

		/// <summary>
		/// Returns the task context
		/// </summary>
		/// <returns>Task context object</returns>
		virtual Handle GetContext() override {
			return Interlocked::Load(This->context);
		}

	public:
		template<typename T, typename TPayload>
		friend struct TaskAwaiter;

	public:
		RefCountClass(BaseTask, This);
	};

	/// <summary>
	/// Task nickname without additional arguments
	/// </summary>
	/// <typeparam name="TType">The result type</typeparam>
	template<typename TType>
	using Task = BaseTask<TType, void>;

	/// <summary>
	/// Base interface with additional information in asynchronous task with
	/// additional operations of initialization and completing the task, 
	/// which allows to run the tasks not immediately, but after a certain period of time
	/// </summary>
	/// <typeparam name="TPayload">Additional information type in asynchronous task</typeparam>
	template<typename TPayload>
	class ITaskPayload<TPayload, true> {
	public:
		template<typename T, typename TPayload>
		friend struct TaskAwaiter;

	protected:
		constexpr static Boolean RequireInitAndStop = true;

	protected:
		void InitTask(IPayloadBaseTask<TPayload>& task) {
			static_cast<TPayload&>(*this).InitImpl(task);
		}

		void EndTask(IPayloadBaseTask<TPayload>& targetTask, Int32& status) {
			static_cast<TPayload&>(*this).EndImpl(targetTask, status);
		}

	public:
		/// <summary>
		/// Destructor
		/// </summary>
		virtual ~ITaskPayload() = 0 {};
	};
}

#pragma warning(pop)