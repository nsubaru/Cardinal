export module Cardinal.Threading:Threadables.ThreadableUnifiedThreadPoolScheduledTask;

export import :Threadables.Base;

export namespace Cardinal::Threading::Threadables
{
	/// <summary>
	/// Task object of thread pool timer during the processing
	/// </summary>
	class ThreadableUnifiedThreadPoolScheduledTask final : public Cardinal::Threading::IThreadable, public IDisposable {
	private:
		struct Data {
			UnifiedThreadPoolScheduledTask task;
			SynchronizeEvent event;
			volatile EThreadabledStates state;
			volatile EThreadabledStates secondState;
			Handle hThread;
			ID threadId;
			ImplSpecificT instance;

			RefCountT RefCount;

			ForceInline Data(
				const UnifiedThreadPoolScheduledTask& task,
				SynchronizeEvent event,
				Handle hThread,
				ID threadId,
				ImplSpecificT instance
			) :
				task(task),
				event(event),
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
		ForceInline ThreadableUnifiedThreadPoolScheduledTask(
			const UnifiedThreadPoolScheduledTask& task,
			const SynchronizeEvent& event,
			Handle hThread,
			ID threadId,
			ImplSpecificT instance
		) :
			This(new Data(task, event, hThread, threadId, instance)) {
		}

	protected:
		friend ThreadableRef RegisterPoolScheduledThreadable(const UnifiedThreadPoolScheduledTask& task, ImplSpecificT instance);

		friend void UnRegisterPoolScheduledThreadable(ThreadableRef&& ref);

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
			return new ThreadableUnifiedThreadPoolScheduledTask(*this);
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
			return EThreadableTypes::ScheduledTask;
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
			This->event.Wait();
			return *this;
		}

		/// <summary>
		/// Waiting for the executing of threadable entity with timeout
		/// </summary>
		/// <param name="timeOut">Maximal waiting time</param>
		/// <returns>A reference to an object</returns>
		ForceInline virtual IThreadable& Join(Time::TimeSpan timeOut) override {
			This->event.Wait(timeOut.Ticks);
			return *this;
		}

		/// <summary>
		/// Waiting for the executing of threadable entity
		/// </summary>
		/// <returns>A reference to an object</returns>
		ForceInline virtual const IThreadable& Join() const override {
			This->event.Wait();
			return *this;
		}

		/// <summary>
		/// Waiting for the executing of threadable entity with timeout
		/// </summary>
		/// <param name="timeOut">Maximal waiting time</param>
		/// <returns>A reference to an object</returns>
		ForceInline virtual const IThreadable& Join(Time::TimeSpan timeOut) const override {
			This->event.Wait(timeOut.Ticks);
			return *this;
		}

		/// <summary>
		/// Returns the context of specified object
		/// </summary>
		/// <returns>Context object</returns>
		virtual const ContextsBlock& GetContext() const;

	public:
		RefCountClass(ThreadableUnifiedThreadPoolScheduledTask, This);
	};
}