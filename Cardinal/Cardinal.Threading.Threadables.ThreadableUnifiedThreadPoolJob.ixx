export module Cardinal.Threading:Threadables.ThreadableUnifiedThreadPoolJob;

export import :Threadables.Base;

export namespace Cardinal::Threading::Threadables
{
	/// <summary>
	/// Task object of thread pool during the execution
	/// </summary>
	class ThreadableUnifiedThreadPoolJob final : public Cardinal::Threading::IThreadable, public IDisposable {
	private:
		struct Data {
			UnifiedThreadPoolJob task;
			volatile EThreadabledStates state;
			volatile EThreadabledStates secondState;
			Handle hThread;
			ID threadId;
			ImplSpecificT instance;

			RefCountT RefCount;

			ForceInline Data(
				const UnifiedThreadPoolJob& task,
				Handle hThread,
				ID threadId,
				ImplSpecificT instance
			) :
				task(task),
				state(EThreadabledStates::Executing),
				secondState(EThreadabledStates::Paused),
				hThread(hThread),
				threadId(threadId),
				instance(instance),
				RefCount(1) {
			}
		};

	private:
		Data* This;

	protected:
		ForceInline virtual void Dispose() override {}

	protected:
		ForceInline ThreadableUnifiedThreadPoolJob(
			const UnifiedThreadPoolJob& task,
			Handle hThread,
			ID threadId,
			ImplSpecificT instance
		) :
			This(new Data(task, hThread, threadId, instance)) {
		}

	protected:
		friend ThreadableRef RegisterPoolJobThreadable(const UnifiedThreadPoolJob& task, ImplSpecificT instance);

		friend void UnRegisterPoolJobThreadable(ThreadableRef&& ref);

	private:
		ContextsBlock* GetCurrentContext() const {
			return &This->task.This->contexts;
		}

	public:
		/// <summary>
		/// Returns the new reference
		/// </summary>
		/// <returns>New reference</returns>
		ForceInline virtual SmartPtr<IThreadable> MakeRef() const override {
			return new ThreadableUnifiedThreadPoolJob(*this);
		}

	public:
		/// <summary>
		/// Entity name receiving
		/// </summary>
		/// <returns>String representation of name</returns>
		ForceInline virtual const String& GetName() const override {
			return This->task.Name;
		}

		/// <summary>
		/// Entity name setting
		/// </summary>
		/// <param name="NewName">New entity name</param>
		ForceInline virtual void SetName(const String& NewName) override {
			This->task.Name = NewName;
		}

		/// <summary>
		/// Entity name setting
		/// </summary>
		/// <param name="NewName">New entity name</param>
		ForceInline virtual void SetName(String&& NewName) override {
			This->task.Name = MoveRValue(NewName);
		}

		/// <summary>
		/// Returns the entity identifier
		/// </summary>
		/// <returns>Entity identifier</returns>
		ForceInline virtual ID GetID() const override {
			return This->threadId;
		}

		/// <summary>
		/// Returns the type of threadable entity
		/// </summary>
		/// <returns>Type of threadable entity</returns>
		ForceInline virtual EThreadableTypes GetThreadableType() const override {
			return EThreadableTypes::JobTask;
		}

		/// <summary>
		/// Returns the state of threadable entity
		/// </summary>
		/// <returns>State of threadable entity</returns>
		ForceInline virtual EThreadabledStates GetThreadabledState() const override {
			return This->state;
		}

		/// <summary>
		/// Suspending the executing of threadable entity
		/// </summary>
		/// <returns>A reference to an object</returns>
		virtual IThreadable& Suspend() override;

		/// <summary>
		/// Resuming the executing of threadable entity
		/// </summary>
		/// <returns>A reference to an object</returns>
		virtual IThreadable& Resume() override;

		/// <summary>
		/// Interrupting the executing of threadable entity
		/// </summary>
		/// <returns>A reference to an object</returns>
		virtual IThreadable& Interupt() override;

		/// <summary>
		/// Waiting for the executing of threadable entity
		/// </summary>
		/// <returns>A reference to an object</returns>
		ForceInline virtual IThreadable& Join() override {
			This->task.Join();
			return *this;
		}

		/// <summary>
		/// Waiting for the executing of threadable entity with timeout
		/// </summary>
		/// <param name="timeOut">Maximal waiting time</param>
		/// <returns>A reference to an object</returns>
		ForceInline virtual IThreadable& Join(Time::TimeSpan timeOut) override {
			This->task.Join(timeOut);
			return *this;
		}

		/// <summary>
		/// Waiting for the executing of threadable entity
		/// </summary>
		/// <returns>A reference to an object</returns>
		ForceInline virtual const IThreadable& Join() const override {
			This->task.Join();
			return *this;
		}

		/// <summary>
		/// Waiting for the executing of threadable entity with timeout
		/// </summary>
		/// <param name="timeOut">Maximal waiting time</param>
		/// <returns>A reference to an object</returns>
		ForceInline virtual const IThreadable& Join(Time::TimeSpan timeOut) const override {
			This->task.Join(timeOut);
			return *this;
		}

		/// <summary>
		/// Returns the context of specified object
		/// </summary>
		/// <returns>Context object</returns>
		virtual const ContextsBlock& GetContext() const;

	public:
		RefCountClass(ThreadableUnifiedThreadPoolJob, This);
	};
}