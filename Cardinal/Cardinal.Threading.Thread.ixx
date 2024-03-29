export module Cardinal.Threading:ThreadObject;

export import Cardinal.Containers.Delegate;

export import :Base;
export import :IThreadable;
export import :Context;
export import :Exceptions;

export import :ScopedLock;
export import :SynchronizeEvent;

#pragma warning(push)
#pragma warning(disable: 4251)

export namespace Cardinal::Threading
{
	/// <summary>
	/// Thread class
	/// </summary>
	class Thread final : IDisposable {
	private:
		struct Data {
			RefCountT RefCount;
			volatile Delegate delegate;
			volatile String name;
			volatile ID TID;
			volatile EThreadabledStates State;
			volatile EThreadabledStates SecondState;
			Handle hThread;
			SynchronizeEvent* AbortEvent;
			Containers::ConcurencySensitiveList<SmartPtr<IThreadable>>::Iterator This;
			ContextsBlock contexts;

			ForceInline Data(const String& Name, Delegate&& delegate)
				: RefCount(1),
				delegate(MoveRValue(delegate)),
				name(Name),
				TID(0),
				State(EThreadabledStates::NotStarted),
				SecondState(),
				hThread(nullptr),
				AbortEvent(nullptr),
				This(),
				contexts() {
			}

			ForceInline Data(String&& Name, Delegate&& delegate)
				: RefCount(1),
				delegate(MoveRValue(delegate)),
				name(MoveRValue(Name)),
				TID(0),
				State(EThreadabledStates::NotStarted),
				SecondState(),
				hThread(nullptr),
				AbortEvent(nullptr),
				This(),
				contexts() {
			}
		};

	private:
		Data* This = nullptr;

	private:
		friend volatile Delegate& GetThreadEntryPoint(Thread* thread);

		friend volatile EThreadabledStates* volatile GetActiveState(Thread* thread);

		friend volatile EThreadabledStates* volatile GetSecondState(Thread* thread);

		friend SynchronizeEvent* GetAbortEvent(Thread* thread);

		friend void MainThread(Delegate&& delegate);

		friend void KillThread(Thread& target, UInt32 statusCode);

		friend ContextsBlock* GetContextBlock(Thread* thread);

		friend Threadables::ThreadableRef RegisterThreadThreadable(
			const Thread& thread,
			volatile EThreadabledStates* state,
			volatile EThreadabledStates* secondState,
			ContextsBlock* contexts);

	private:
		void RefreshName();

	protected:
		virtual void Dispose() override;

	public:
		/// <summary>
		/// Thread constructor with the delegate
		/// </summary>
		/// <param name="delegate">The action that the thread will perform</param>
		ForceInline Thread(Delegate&& delegate) : This(new Data(L"Unnamed Thread", MoveRValue(delegate))) {}

		/// <summary>
		/// Constructor with thread name and delegate
		/// </summary>
		/// <param name="Name">Thread name</param>
		/// <param name="delegate">The action that the thread will perform</param>
		ForceInline Thread(String&& Name, Delegate&& delegate) : This(new Data(MoveRValue(Name), MoveRValue(delegate))) {}

		/// <summary>
		/// Constructor with thread name and delegate
		/// </summary>
		/// <param name="Name">Thread name</param>
		/// <param name="delegate">The action that the thread will perform</param>
		ForceInline Thread(const String& Name, Delegate&& delegate) : This(new Data(Name, MoveRValue(delegate))) {}

	public:
		/// <summary>
		/// Run the thread for execution
		/// </summary>
		/// <returns>A reference to a current object</returns>
		Thread& Start();

		/// <summary>
		/// Waiting for the thread to complete
		/// </summary>
		/// <returns>A reference to a current object</returns>
		Thread& Join();

		/// <summary>
		/// Waiting for the thread to complete
		/// </summary>
		/// <param name="timeOut">Waiting timeout</param>
		/// <returns>A reference to a current object</returns>
		Thread& Join(Time::TimeSpan timeOut);

		/// <summary>
		/// Waiting for the thread to complete
		/// </summary>
		/// <returns>A reference to a current object</returns>
		const Thread& Join() const;

		/// <summary>
		/// Waiting for the thread to complete
		/// </summary>
		/// <param name="timeOut">Waiting timeout</param>
		/// <returns>A reference to a current object</returns>
		const Thread& Join(Time::TimeSpan timeOut) const;

		/// <summary>
		/// Interrupting the thread execution
		/// </summary>
		/// <returns>A reference to a current object</returns>
		Thread& Interupt();

		/// <summary>
		/// Suspending the thread execution
		/// </summary>
		Thread& Suspend();

		/// <summary>
		/// Resuming the thread execution
		/// </summary>
		Thread& Resume();

	public:
		/// <summary>
		/// Returns the result of thread execution
		/// </summary>
		template<typename T>
		ForceInline T GetThreadResult() {
			return This->delegate.GetResult<T>();
		}

	public:
		/// <summary>
		/// Convert to a thread interface reference
		/// </summary>
		/// <returns>Thread interface reference</returns>
		ForceInline operator volatile IThreadable& () {
			if (This->This != nullptr)
			{
				return **This->This;
			}
			else
			{
				Throw(NullPointException(SourcePosition));
			}
		}

		/// <summary>
		/// Convert to a constant reference to the thread interface
		/// </summary>
		/// <returns>Constant reference to the thread interface</returns>
		ForceInline operator const volatile IThreadable& () const {
			if (This->This != nullptr)
			{
				return **This->This;
			}
			else
			{
				Throw(NullPointException(SourcePosition));
			}
		}

		/// <summary>
		/// Thread identifier convertion
		/// </summary>
		/// <returns>Thread identifier</returns>
		ForceInline operator ID () const { return This->TID; }

		/// <summary>
		/// Convert into a boolean value
		/// </summary>
		/// <returns>Boolean value which indicates whether the thread is active</returns>
		ForceInline operator Boolean () const { return This->TID != 0; }

	public:
		/// <summary>
		/// Setting the thread name
		/// </summary>
		/// <param name="name">New thread name</param>
		ForceInline void SetName(const String& name) { const_cast<String&>(This->name) = name; RefreshName(); }

		/// <summary>
		/// Setting the thread name
		/// </summary>
		/// <param name="name">New thread name</param>
		ForceInline void SetName(String&& name) { const_cast<String&>(This->name) = MoveRValue(name); RefreshName(); }

		/// <summary>
		/// Current thread name
		/// </summary>
		/// <returns>String representation of thread name</returns>
		ForceInline const String& GetName() const { return const_cast<String&>(This->name); }

		/// <summary>
		/// Thread state
		/// </summary>
		/// <returns>Current thread state</returns>
		ForceInline EThreadabledStates GetThreadState() const { return this->This->State; }

		/// <summary>
		/// Thread identifier (numeric)
		/// </summary>
		/// <returns>Numeric thread identifier</returns>
		ForceInline ID GetTID() const { return this->This->TID; }

		/// <summary>
		/// Thread name
		/// </summary>
		String Property(GetName, SetName) Name;

		/// <summary>
		/// Thread state
		/// </summary>
		EThreadabledStates Property(GetThreadState) ThreadState;

		/// <summary>
		/// Thread identifier
		/// </summary>
		ID Property(GetTID) TID;

	public:
		RefCountClass(Thread, This);

	public:
		friend class Threadables::ThreadableThread;
	};
}

#pragma warning(pop)