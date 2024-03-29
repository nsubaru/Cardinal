export module Cardinal.Threading:Threadables.ThreadableUnifiedThreadPoolAlpcTask;

export import :Threadables.Base;

export namespace Cardinal::Threading::Threadables
{
	/// <summary>
	/// Task object of ALPC pool during the execution process
	/// </summary>
	class ThreadableUnifiedThreadPoolAlpcTask final : public Cardinal::Threading::IThreadable, public IDisposable {
	private:
		friend class IO::IPC::Alpc::IAlpcBase;
	private:
		struct Data {
			AlpcPoolJobContainer task;
			SynchronizeEvent event;
			volatile EThreadabledStates state;
			volatile EThreadabledStates secondState;
			Handle hThread;
			ID threadId;
			ImplSpecificT instance;
			ContextsBlock contexts;

			RefCountT RefCount;

			ForceInline Data(
				const AlpcPoolJobContainer& task,
				const SynchronizeEvent& event,
				ContextsBlock& contexts,
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
				contexts(contexts),
				RefCount(1) {
			}
		};

		Data* This;

	protected:
		ForceInline virtual void Dispose() override {}

		ForceInline ThreadableUnifiedThreadPoolAlpcTask(
			const AlpcPoolJobContainer& task,
			const SynchronizeEvent& event,
			ContextsBlock& constexts,
			Handle hThread,
			ID threadId,
			ImplSpecificT instance
		) :
			This(new Data(task, event, constexts, hThread, threadId, instance)) {
		}

	protected:
		friend ThreadableRef RegisterPoolJobThreadable(
			const AlpcPoolJobContainer& task,
			const SynchronizeEvent& event,
			ImplSpecificT instance,
			ContextsBlock& contexts);

		friend void UnRegisterPoolAlpcThreadable(ThreadableRef&& ref);

	private:
		ContextsBlock* GetCurrentContext() const {
			return &This->contexts;
		}

	public:
		/// <summary>
		/// Returns the new reference
		/// </summary>
		/// <returns>New reference</returns>
		ForceInline virtual SmartPtr<IThreadable> MakeRef() const override {
			return new ThreadableUnifiedThreadPoolAlpcTask(*this);
		}

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
		/// <param name="newName">New entity name</param>
		ForceInline virtual void SetName(const String& NewName) override {
			This->task.Name = NewName;
		}

		/// <summary>
		/// Entity name setting
		/// </summary>
		/// <param name="newName">New entity name</param>
		ForceInline virtual void SetName(String&& NewName) override {
			This->task.Name = MoveRValue(NewName);
		}

		/// <summary>
		/// Entity identifier
		/// </summary>
		/// <returns>Entity identifier</returns>
		ForceInline virtual ID GetID() const override {
			return This->threadId;
		}

		/// <summary>
		/// Type of threadable entity
		/// </summary>
		/// <returns>Type of threadable entity</returns>
		ForceInline virtual EThreadableTypes GetThreadableType() const override {
			return EThreadableTypes::IoTask;
		}

		/// <summary>
		/// State of threadable entity
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
			This->event.Wait(timeOut);
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
			This->event.Wait(timeOut);
			return *this;
		}

		/// <summary>
		/// Returns true if this thread process task
		/// </summary>
		ForceInline bool IsSameTask(const AlpcPoolJobContainer& task) {
			return This->task.IsSameAs(task);
		}

		/// <summary>
		/// Return current thread context
		/// </summary>
		/// <returns>Об'єкт контексту</returns>
		virtual const ContextsBlock& GetContext() const;

	public:
		RefCountClass(ThreadableUnifiedThreadPoolAlpcTask, This);
	};
}