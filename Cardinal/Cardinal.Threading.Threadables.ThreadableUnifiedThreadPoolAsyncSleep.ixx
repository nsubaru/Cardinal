//export module Cardinal.Threading:Threadables.ThreadableUnifiedThreadPoolAlpcTask;
//
//export import :Threadables.Base;
//export import :Async.Sleep;
//
//export namespace Cardinal::Threading::Threadables
//{
//	/// <summary>
//	/// Task object await processing of thread pool
//	/// </summary>
//	class ThreadableUnifiedThreadPoolAsyncSleep final : public Cardinal::Threading::IThreadable, public IDisposable {
//	private:
//		struct Data {
//			SynchronizeEvent event;
//			String threadName;
//			Handle hThread;
//			SleepTask trigger;
//
//			volatile EThreadabledStates state;
//			volatile EThreadabledStates secondState;
//
//			ID threadId;
//			ImplSpecificT instance;
//
//			RefCountT RefCount;
//
//			Data(Handle hThread, SleepTask&& trigger, EThreadabledStates state, EThreadabledStates secondState,
//				ID     threadId, ImplSpecificT instance)
//				: threadName(L"Event callback thread object"),
//				hThread(hThread),
//				trigger(MoveRValue(trigger)),
//				state(state),
//				secondState(secondState),
//				threadId(threadId),
//				instance(instance),
//				event(),
//				RefCount(1) {
//			}
//		};
//
//	private:
//		Data* This;
//
//	private:
//		virtual void Dispose() override {}
//
//	protected:
//		ThreadableUnifiedThreadPoolAsyncSleep(Handle hThread, SleepTask&& trigger, ID threadId, ImplSpecificT instance)
//			: This(new Data(hThread, MoveRValue(trigger), EThreadabledStates::Starting, EThreadabledStates::Paused, threadId, instance)) {}
//
//	private:
//		friend ThreadableRef RegisterPoolSleepThreadable(const SleepTask& task, ImplSpecificT instance, ContextsBlock& contexts);
//
//		friend void UnRegisterPoolSleepThreadable(ThreadableRef&& ref);
//
//	public:
//		/// <summary>
//		/// Returns the new reference
//		/// </summary>
//		/// <returns>New reference</returns>
//		NoDiscard virtual SmartPtr<IThreadable> MakeRef() const override { return new ThreadableUnifiedThreadPoolAsyncSleep(*this); }
//
//	public:
//		/// <summary>
//		/// Entity name receiving
//		/// </summary>
//		/// <returns>String representation of name</returns>
//		NoDiscard virtual const String& GetName() const override { return This->threadName; }
//
//		/// <summary>
//		/// Entity name setting
//		/// </summary>
//		/// <param name="newName">New entity name</param>
//		NoDiscard virtual void SetName(const String& newName) override {
//			This->threadName = newName;
//		}
//
//		/// <summary>
//		/// Entity name setting
//		/// </summary>
//		/// <param name="newName">New entity name</param>
//		NoDiscard virtual void SetName(String&& newName) override {
//			This->threadName = newName;
//		}
//
//		/// <summary>
//		/// Entity identifier
//		/// </summary>
//		/// <returns>Entity identifier</returns>
//		NoDiscard virtual ID GetID() const override { return This->threadId; }
//
//		/// <summary>
//		/// Type of threadable entity
//		/// </summary>
//		/// <returns>Type of threadable entity</returns>
//		NoDiscard virtual EThreadableTypes GetThreadableType() const override { return EThreadableTypes::AsyncSleep; }
//
//		/// <summary>
//		/// State of threadable entity
//		/// </summary>
//		/// <returns>State of threadable entity</returns>
//		NoDiscard virtual EThreadabledStates GetThreadabledState() const override { return This->state; }
//
//		/// <summary>
//		/// Suspending the executing of threadable entity
//		/// </summary>
//		/// <returns>A reference to an object</returns>
//		virtual IThreadable& Suspend() override;
//
//		/// <summary>
//		/// Resuming the executing of threadable entity
//		/// </summary>
//		/// <returns>A reference to an object</returns>
//		virtual IThreadable& Resume() override;
//
//		/// <summary>
//		/// Interrupting the executing of threadable entity
//		/// </summary>
//		/// <returns>A reference to an object</returns>
//		virtual IThreadable& Interupt() override;
//
//		/// <summary>
//		/// Waiting for the executing of threadable entity
//		/// </summary>
//		/// <returns>A reference to an object</returns>
//		virtual IThreadable& Join() override {
//			This->event.Wait();
//			return *this;
//		}
//
//		/// <summary>
//		/// Waiting for the executing of threadable entity with timeout
//		/// </summary>
//		/// <param name="timeOut">Maximal waiting time</param>
//		/// <returns>A reference to an object</returns>
//		virtual IThreadable& Join(Time::TimeSpan timeOut) override {
//			This->event.Wait(timeOut);
//			return *this;
//		}
//
//		/// <summary>
//		/// Waiting for the executing of threadable entity
//		/// </summary>
//		/// <returns>A reference to an object</returns>
//		NoDiscard  virtual const IThreadable& Join() const override {
//			This->event.Wait();
//			return *this;
//		}
//
//		/// <summary>
//		/// Waiting for the executing of threadable entity with timeout
//		/// </summary>
//		/// <param name="timeOut">Maximal waiting time</param>
//		/// <returns>A reference to an object</returns>
//		NoDiscard virtual const IThreadable& Join(Time::TimeSpan timeOut) const override {
//			This->event.Wait(timeOut);
//			return *this;
//		}
//
//		/// <summary>
//		/// Returns the context of specified object
//		/// </summary>
//		/// <returns>Context object</returns>
//		virtual const ContextsBlock& GetContext() const;
//
//	public:
//		RefCountClass(ThreadableUnifiedThreadPoolAsyncSleep, This)
//	};
//}