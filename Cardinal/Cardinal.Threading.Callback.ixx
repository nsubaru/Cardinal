export module Cardinal.Threading:Callback;

export import :Base;
export import Cardinal.Containers.Delegate;

export import :IThreadable;
export import :Context;
export import :Exceptions;
export import :SynchronizeEvent;

namespace Cardinal::Threading::Threadables
{
	class ThreadableCallback;
}

export namespace Cardinal::Threading
{
	class ExternalCallback;

	/// <summary>
	/// Provide functionality to processing callbacks in kernel enviroment
	/// </summary>
	class ExternalCallbackProvider : public IDisposable {
	private:
		struct Data {
			String threadableName;
			ContextsBlock contexts;
			RefCountT RefCount;

			Data(String&& threadableName, const ContextsBlock& contexts) : threadableName(MoveRValue(threadableName)), contexts(contexts), RefCount(1) {}
		};
	private:
		Data* This;

	protected:
		virtual void Dispose() {}

	public:
		/// <summary>
		/// Create provider with callbacks threads name
		/// </summary>
		/// <param name="threadableName">Callbacks threads name</param>
		ExternalCallbackProvider(String&& threadableName);

		/// <summary>
		/// Create provider with callbacks threads name
		/// </summary>
		/// <param name="threadableName">Callbacks threads name</param>
		ExternalCallbackProvider(const String& threadableName) : ExternalCallbackProvider(String(threadableName)) {}

	public:
		/// <summary>
		/// Start new callback in initialized kernel enviroment
		/// </summary>
		/// <param name="delegate">Callback action</param>
		/// <returns>Threadable object which hold action with result and provide functionality to control callback thread</returns>
		ExternalCallback InvokeNewInstance(Delegate&& delegate);

	public:
		RefCountClass(ExternalCallbackProvider, This);
	};

	/// <summary>
	/// Threadable object for external callbacks
	/// </summary>
	class ExternalCallback : public IDisposable {
	private:
		struct Data {
			ExternalCallbackProvider provider;
			String name;
			Delegate delegate;
			ContextsBlock contextBlock;
			volatile EThreadabledStates state;
			volatile EThreadabledStates secondState;
			SynchronizeEvent event;
			Handle hThread;
			ID tid;

			RefCountT RefCount;

			Data(
				const ExternalCallbackProvider& provider,
				String&& name,
				Delegate&& delegate,
				const ContextsBlock& contextBlock
			) :
				provider(provider),
				name(name),
				delegate(MoveRValue(delegate)),
				contextBlock(contextBlock),
				state(EThreadabledStates::NotStarted),
				secondState(EThreadabledStates::Paused),
				event(),
				hThread(nullptr),
				tid(0),
				RefCount(1) {
			}
		};

	private:
		Data* This;

	private:
		virtual void Dispose() {}

	protected:
		ExternalCallback(
			const ExternalCallbackProvider& provider,
			String&& name,
			Delegate&& delegate,
			const ContextsBlock& contextBlock
		) :
			This(new Data(provider, MoveRValue(name), MoveRValue(delegate), contextBlock)) {
		}

	public:
		/// <summary>
		/// Returns callback thread name
		/// </summary>
		/// <returns>Callback thread name</returns>
		const String& GetName() const {
			return This->name;
		}

		/// <summary>
		/// Set callback thread name
		/// </summary>
		/// <param name="newName">New callback thread name</param>
		void SetName(const String& newName) {
			This->name = newName;
		}

		/// <summary>
		/// Set callback thread name
		/// </summary>
		/// <param name="newName">New callback thread name</param>
		void SetName(String&& newName) {
			This->name = MoveRValue(newName);
		}

		/// <summary>
		/// Returns callback thread state
		/// </summary>
		/// <returns>Callback thread state</returns>
		EThreadabledStates GetThreadabledState() const {
			return This->state;
		}

		/// <summary>
		/// Entity identifier
		/// </summary>
		/// <returns>Entity identifier</returns>
		virtual ID GetID() const {
			return This->tid;
		}

	public:
		/// <summary>
		/// Callback thread name
		/// </summary>
		String Property(GetName, SetName) Name;

		/// <summary>
		/// Callback thread state
		/// </summary>
		EThreadabledStates Property(GetThreadabledState) ThreadabledState;

		/// <summary>
		/// Callback thread id
		/// </summary>
		ID Property(GetID) TID;

	public:

		/// <summary>
		/// Suspending the executing of threadable entity
		/// </summary>
		/// <returns>A reference to an object</returns>
		virtual ExternalCallback& Suspend();

		/// <summary>
		/// Resuming the executing of threadable entity
		/// </summary>
		/// <returns>A reference to an object</returns>
		virtual ExternalCallback& Resume();

		/// <summary>
		/// Interrupting the executing of threadable entity
		/// </summary>
		/// <returns>A reference to an object</returns>
		virtual ExternalCallback& Interupt();

		/// <summary>
		/// Waiting for the executing of threadable entity
		/// </summary>
		/// <returns>A reference to an object</returns>
		virtual ExternalCallback& Join();

		/// <summary>
		/// Waiting for the executing of threadable entity with timeout
		/// </summary>
		/// <param name="timeOut">Maximal waiting time</param>
		/// <returns>A reference to an object</returns>
		virtual ExternalCallback& Join(Time::TimeSpan timeOut);

		/// <summary>
		/// Waiting for the executing of threadable entity
		/// </summary>
		/// <returns>A reference to an object</returns>
		virtual const ExternalCallback& Join() const;

		/// <summary>
		/// Waiting for the executing of threadable entity with timeout
		/// </summary>
		/// <param name="timeOut">Maximal waiting time</param>
		/// <returns>A reference to an object</returns>
		virtual const ExternalCallback& Join(Time::TimeSpan timeOut) const;

	public:
		/// <summary>
		/// Return callback action result
		/// </summary>
		/// <typeparam name="T">Callback action result type</typeparam>
		/// <returns>Callback action result</returns>
		template<typename T>
		ForceInline T GetResult() {
			return This->delegate.GetResult<T>();
		}

	public:
		RefCountClass(ExternalCallback, This)

	public:
		friend class ExternalCallbackProvider;
		friend class Threadables::ThreadableCallback;
	};
}