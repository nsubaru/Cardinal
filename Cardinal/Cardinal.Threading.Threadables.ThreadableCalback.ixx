export module Cardinal.Threading:Threadables.ThreadableCalback;

export import :Threadables.Base;

export namespace Cardinal::Threading::Threadables
{
	/// <summary>
	/// Callback object during the execution
	/// </summary>
	class ThreadableCallback final : public Cardinal::Threading::IThreadable {
	private:
		ExternalCallback callbackInstance;

	protected:
		ForceInline ThreadableCallback(const ExternalCallback& callbackInstance) : callbackInstance(callbackInstance) {}

	protected:
		friend ThreadableRef RegisterThreadableCallback(
			const ExternalCallback& callbackInstance,
			volatile EThreadabledStates* state,
			volatile EThreadabledStates* secondState,
			ContextsBlock* contexts);

		friend void UnRegisterThreadableCallback(ThreadableRef&& ref);

	public:
		/// <summary>
		/// Copying constructor
		/// </summary>
		/// <param name="from">Original</param>
		ForceInline ThreadableCallback(const ThreadableCallback& from) : callbackInstance(from.callbackInstance) {}

		/// <summary>
		/// Moving constructor
		/// </summary>
		/// <param name="from">Original</param>
		ForceInline ThreadableCallback(ThreadableCallback&& from) : callbackInstance(MoveRValue(from.callbackInstance)) {}

		/// <summary>
		/// Copying operator
		/// </summary>
		/// <param name="from">Right operand</param>
		/// <returns>A reference to a current object</returns>
		ForceInline ThreadableCallback& operator=(const ThreadableCallback& from) {
			this->~ThreadableCallback();
			this->ThreadableCallback::ThreadableCallback(from);
			return *this;
		}

		/// <summary>
		/// Moving operator
		/// </summary>
		/// <param name="from">Right operand</param>
		/// <returns>A reference to a current object</returns>
		ForceInline ThreadableCallback& operator=(ThreadableCallback&& from) {
			this->~ThreadableCallback();
			this->ThreadableCallback::ThreadableCallback(MoveRValue(from));
			return *this;
		}

	public:
		/// <summary>
		/// Returns the new reference
		/// </summary>
		/// <returns>New reference</returns>
		ForceInline virtual SmartPtr<IThreadable> MakeRef() const override {
			return new ThreadableCallback(*this);
		}

	public:
		/// <summary>
		/// Entity name receiving
		/// </summary>
		/// <returns>String representation of name</returns>
		ForceInline virtual const String& GetName() const override {
			return callbackInstance.Name;
		}

		/// <summary>
		/// Entity name setting
		/// </summary>
		/// <param name="NewName">New entity name</param>
		ForceInline virtual void SetName(const String& NewName) override {
			callbackInstance.Name = NewName;
		}

		/// <summary>
		/// Entity name setting
		/// </summary>
		/// <param name="NewName">New entity name</param>
		ForceInline virtual void SetName(String&& NewName) override {
			callbackInstance.Name = MoveRValue(NewName);
		}

		/// <summary>
		/// Returns the entity identifier
		/// </summary>
		/// <returns>Entity identifier</returns>
		ForceInline virtual ID GetID() const override {
			return callbackInstance.TID;
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
			return callbackInstance.ThreadabledState;
		}

		/// <summary>
		/// Suspending the executing of threadable entity
		/// </summary>
		/// <returns>A reference to an object</returns>
		ForceInline virtual IThreadable& Suspend() override {
			callbackInstance.Suspend();
			return *this;
		}

		/// <summary>
		/// Resuming the executing of threadable entity
		/// </summary>
		/// <returns>A reference to an object</returns>
		ForceInline virtual IThreadable& Resume() override {
			callbackInstance.Resume();
			return *this;
		}

		/// <summary>
		/// Interrupting the executing of threadable entity
		/// </summary>
		/// <returns>A reference to an object</returns>
		ForceInline virtual IThreadable& Interupt() override {
			callbackInstance.Interupt();
			return *this;
		}

		/// <summary>
		/// Waiting for the executing of threadable entity
		/// </summary>
		/// <returns>A reference to an object</returns>
		ForceInline virtual IThreadable& Join() override {
			callbackInstance.Join();
			return *this;
		}

		/// <summary>
		/// Waiting for the executing of threadable entity with timeout
		/// </summary>
		/// <param name="timeOut">Maximal waiting time</param>
		/// <returns>A reference to an object</returns>
		ForceInline virtual IThreadable& Join(Time::TimeSpan timeOut) override {
			callbackInstance.Join(timeOut);
			return *this;
		}

		/// <summary>
		/// Waiting for the executing of threadable entity
		/// </summary>
		/// <returns>A reference to an object</returns>
		ForceInline virtual const IThreadable& Join() const override {
			UnusedData auto _ = callbackInstance.Join();
			return *this;
		}

		/// <summary>
		/// Waiting for the executing of threadable entity with timeout
		/// </summary>
		/// <param name="timeOut">Maximal waiting time</param>
		/// <returns>A reference to an object</returns>
		ForceInline virtual const IThreadable& Join(Time::TimeSpan timeOut) const override {
			UnusedData auto _ = callbackInstance.Join(timeOut);
			return *this;
		}

		/// <summary>
		/// Returns the callbackInstance object
		/// </summary>
		ForceInline ExternalCallback& GetThreadObject() {
			return callbackInstance;
		}

		/// <summary>
		/// Returns the callbackInstance object
		/// </summary>
		ForceInline const ExternalCallback& GetThreadObject() const {
			return callbackInstance;
		}

		/// <summary>
		/// Returns the context of specified object
		/// </summary>
		/// <returns>Context object</returns>
		ForceInline virtual const ContextsBlock& GetContext() const {
			return callbackInstance.This->contextBlock;
		}
	};
}