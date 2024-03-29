export module Cardinal.Threading:UnifiedThreadPool.ThreadPoolThreadContext;

export import :UnifiedThreadPool.Base;
export import :UnifiedThreadPool.Job;
export import :UnifiedThreadPool.ScheduledTask;
export import :UnifiedThreadPool.IO;
export import :UnifiedThreadPool.Pool;

export namespace Cardinal::Threading
{
	/// <summary>
	/// Default thread pool threadble context. 
	/// Hold default thread pool for threadable. 
	/// It use for async operations if no other thread pool provided.
	/// </summary>
	class ThreadPoolThreadContext final : public IContext, public IClonable<ThreadPoolThreadContext> {
	private:
		UnifiedThreadPool pool;

	protected:
		virtual void Dispose() override {}

	public:
		/// <summary>
		/// Constructot from thread pool
		/// </summary>
		/// <param name="pool">Thread pool object</param>
		ThreadPoolThreadContext(const UnifiedThreadPool& pool) : pool(pool) {}

		/// <summary>
		/// Constructot from thread pool
		/// </summary>
		/// <param name="pool">Thread pool object</param>
		ThreadPoolThreadContext(UnifiedThreadPool&& pool) : pool(MoveRValue(pool)) {}

	public:
		ThreadPoolThreadContext(const ThreadPoolThreadContext& orig) : pool(orig.pool) {}
		ThreadPoolThreadContext(ThreadPoolThreadContext&& orig) : pool(MoveRValue(orig.pool)) {}

	public:
		ThreadPoolThreadContext& operator=(const ThreadPoolThreadContext& orig) {
			this->~ThreadPoolThreadContext();
			this->ThreadPoolThreadContext::ThreadPoolThreadContext(orig);
			return *this;
		}

		ThreadPoolThreadContext& operator=(ThreadPoolThreadContext&& orig) {
			this->~ThreadPoolThreadContext();
			this->ThreadPoolThreadContext::ThreadPoolThreadContext(MoveRValue(orig));
			return *this;
		}

	public:
		/// <summary>
		/// Create object copy
		/// </summary>
		/// <returns>Object copy</returns>
		virtual ThreadPoolThreadContext Clone() const override { return ThreadPoolThreadContext(*this); }

	public:
		/// <summary>
		/// Return threadable context type
		/// </summary>
		/// <returns>RTTI type record for this type</returns>
		virtual RTTI::Record::RttiTypeRecord GetContextType() const override { return RTTI::Record::RttiTypeRecord() /*TypeRecord<ThreadPoolThreadContext>()*/; } //Note: this line causes over 4GB issue

	public:
		/// <summary>
		/// Return thread pool object
		/// </summary>
		/// <returns>Thread pool object</returns>
		UnifiedThreadPool GetThreadPool() {
			return this->pool;
		}

		/// <summary>
		/// Thread pool object
		/// </summary>
		UnifiedThreadPool Property(GetThreadPool) ThreadPool;

	public:
		virtual ~ThreadPoolThreadContext() {}
	};
}