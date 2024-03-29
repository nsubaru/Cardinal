export module Cardinal.Threading:Threadables.ThreadableThread;

export import :Threadables.Base;

export namespace Cardinal::Threading::Threadables
{
	/// <summary>
	/// Thread object during the execution
	/// </summary>
	class ThreadableThread final : public Cardinal::Threading::IThreadable {
	private:
		Thread thread;

	protected:
		ForceInline ThreadableThread(const Thread& thread) : thread(thread) {}

	protected:
		friend ThreadableRef RegisterThreadThreadable(
			const Thread& thread,
			volatile EThreadabledStates* state,
			volatile EThreadabledStates* secondState,
			ContextsBlock* contexts);

		friend void UnRegisterThreadThreadable(ThreadableRef&& ref);

	public:
		/// <summary>
		/// Copying constructor
		/// </summary>
		/// <param name="from">Original</param>
		ForceInline ThreadableThread(const ThreadableThread& from) : thread(from.thread) {}

		/// <summary>
		/// Moving constructor
		/// </summary>
		/// <param name="from">Original</param>
		ForceInline ThreadableThread(ThreadableThread&& from) : thread(MoveRValue(from.thread)) {}

		/// <summary>
		/// Copying operator
		/// </summary>
		/// <param name="from">Right operand</param>
		/// <returns>A reference to a current object</returns>
		ForceInline ThreadableThread& operator=(const ThreadableThread& from) {
			this->~ThreadableThread();
			this->ThreadableThread::ThreadableThread(from);
			return *this;
		}

		/// <summary>
		/// Moving operator
		/// </summary>
		/// <param name="from">Right operand</param>
		/// <returns>A reference to a current object</returns>
		ForceInline ThreadableThread& operator=(ThreadableThread&& from) {
			this->~ThreadableThread();
			this->ThreadableThread::ThreadableThread(MoveRValue(from));
			return *this;
		}

	public:
		/// <summary>
		/// Returns the new reference
		/// </summary>
		/// <returns>New reference</returns>
		ForceInline virtual SmartPtr<IThreadable> MakeRef() const override {
			return new ThreadableThread(*this);
		}

	public:
		/// <summary>
		/// Entity name receiving
		/// </summary>
		/// <returns>String representation of name</returns>
		ForceInline virtual const String& GetName() const override {
			return thread.Name;
		}

		/// <summary>
		/// Entity name setting
		/// </summary>
		/// <param name="NewName">New entity name</param>
		ForceInline virtual void SetName(const String& NewName) override {
			thread.Name = NewName;
		}

		/// <summary>
		/// Entity name setting
		/// </summary>
		/// <param name="NewName">New entity name</param>
		ForceInline virtual void SetName(String&& NewName) override {
			thread.Name = MoveRValue(NewName);
		}

		/// <summary>
		/// Returns the entity identifier
		/// </summary>
		/// <returns>Entity identifier</returns>
		ForceInline virtual ID GetID() const override {
			return thread.TID;
		}

		/// <summary>
		/// Returns the type of threadable entity
		/// </summary>
		/// <returns>Type of threadable entity</returns>
		ForceInline virtual EThreadableTypes GetThreadableType() const override {
			return EThreadableTypes::Thread;
		}

		/// <summary>
		/// Returns the state of threadable entity
		/// </summary>
		/// <returns>State of threadable entity</returns>
		ForceInline virtual EThreadabledStates GetThreadabledState() const override {
			return thread.ThreadState;
		}

		/// <summary>
		/// Suspending the executing of threadable entity
		/// </summary>
		/// <returns>A reference to an object</returns>
		ForceInline virtual IThreadable& Suspend() override {
			thread.Suspend();
			return *this;
		}

		/// <summary>
		/// Resuming the executing of threadable entity
		/// </summary>
		/// <returns>A reference to an object</returns>
		ForceInline virtual IThreadable& Resume() override {
			thread.Resume();
			return *this;
		}

		/// <summary>
		/// Interrupting the executing of threadable entity
		/// </summary>
		/// <returns>A reference to an object</returns>
		ForceInline virtual IThreadable& Interupt() override {
			thread.Interupt();
			return *this;
		}

		/// <summary>
		/// Waiting for the executing of threadable entity
		/// </summary>
		/// <returns>A reference to an object</returns>
		ForceInline virtual IThreadable& Join() override {
			thread.Join();
			return *this;
		}

		/// <summary>
		/// Waiting for the executing of threadable entity with timeout
		/// </summary>
		/// <param name="timeOut">Maximal waiting time</param>
		/// <returns>A reference to an object</returns>
		ForceInline virtual IThreadable& Join(Time::TimeSpan timeOut) override {
			thread.Join(timeOut);
			return *this;
		}

		/// <summary>
		/// Waiting for the executing of threadable entity
		/// </summary>
		/// <returns>A reference to an object</returns>
		ForceInline virtual const IThreadable& Join() const override {
			UnusedData auto _ = thread.Join();
			return *this;
		}

		/// <summary>
		/// Waiting for the executing of threadable entity with timeout
		/// </summary>
		/// <param name="timeOut">Maximal waiting time</param>
		/// <returns>A reference to an object</returns>
		ForceInline virtual const IThreadable& Join(Time::TimeSpan timeOut) const override {
			UnusedData auto _ = thread.Join(timeOut);
			return *this;
		}

		/// <summary>
		/// Returns the thread object
		/// </summary>
		ForceInline Thread& GetThreadObject() {
			return thread;
		}

		/// <summary>
		/// Returns the thread object
		/// </summary>
		ForceInline const Thread& GetThreadObject() const {
			return thread;
		}

		/// <summary>
		/// Returns the context of specified object
		/// </summary>
		/// <returns>Context object</returns>
		ForceInline virtual const ContextsBlock& GetContext() const {
			return thread.This->contexts;
		}
	};
}