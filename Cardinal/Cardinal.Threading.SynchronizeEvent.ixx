export module Cardinal.Threading:SynchronizeEvent;

export import :Base;
export import :Exceptions;
export import Cardinal.Threading.Internal;

#pragma warning(push)
// This internal implementation file which can use internal API functions
#pragma warning(disable: 4996)

export namespace Cardinal::Threading
{
	/// <summary> 
	/// NT event
	/// <para>Thread syncronization object</para>
	/// </summary>
	class SynchronizeEvent final : public IDisposable, public IAwaitable {
		struct Data {
			Boolean isManualReset;
			Handle handle;
			RefCountT RefCount;
			String Name;

			ForceInline Data(Boolean isManualReset, const String& name)
				:isManualReset(isManualReset), handle(nullptr), RefCount(1), Name(name) {
			};

			ForceInline Data(Boolean isManualReset, String&& name)
				:isManualReset(isManualReset), handle(nullptr), RefCount(1), Name(MoveRValue(name)) {
			};
		};

	private:
		Data* data;

	protected:
		virtual void Dispose() override;

		virtual Handle GetAwaitableHandle() const override {
			return data->handle;
		}

	public:
		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="isManualReset">Indicates whether the event is automatically reset during the waiting</param>
		SynchronizeEvent(Boolean isManualReset = false);

		/// <summary>
		/// Constructor with object name
		/// </summary>
		/// <param name="name">Object name (also it is the object name in NT kernel)</param>
		/// <param name="isManualReset">Indicates whether the event is automatically reset during the waiting</param>
		SynchronizeEvent(const String& name, Boolean isManualReset = false);

		/// <summary>
		/// Constructor with object name
		/// </summary>
		/// <param name="name">Object name (also it is the object name in NT kernel)</param>
		/// <param name="isManualReset">Indicates whether the event is automatically reset during the waiting</param>
		SynchronizeEvent(String&& name, Boolean isManualReset = false);

	public:
		/// <summary>
		/// Waiting for event
		/// </summary>
		void Wait() const;

		/// <summary>
		/// Waiting with timeout
		/// </summary>
		/// <param name="TimeOutMs">Timeout in milliseconds</param>
		void Wait(SizeT TimeOutMs) const;

		/// <summary>
		/// Waiting with timeout
		/// </summary>
		/// <param name="timeOut">Timeout</param>
		void Wait(Time::TimeSpan timeOut) const;

	public:
		/// <summary>
		/// Gives impulse to the event
		/// </summary>
		ForceInline void Signale() {
			Internals::PulseNtEvent(data->handle);
		}

		/// <summary>
		/// Sets "true" to event
		/// </summary>
		ForceInline void Set() {
			Internals::SetNtEvent(data->handle);
		}

		/// <summary>
		/// Sets "false" to event
		/// </summary>
		ForceInline void Reset() {
			Internals::ResetNtEvent(data->handle);
		}

	public:
		/// <summary>
		/// Check is event seted
		/// </summary>
		/// <returns>true if event setted</returns>
		bool IsSetted() const;

	public:
		/// <summary>
		/// Explicit conversion to the "OS" descriptor
		/// </summary>
		/// <returns>Returns the system descriptor which is associated with event object</returns>
		InternalApiCall explicit operator Handle() {
			return data->handle;
		}

	public:
		/// <summary>
		/// Event type (either manually resetting the event or OS features do it automatically)
		/// </summary>
		/// <returns>"True" if the event is automatically resets its state during the reading</returns>
		ForceInline Boolean GetIsManualReset() const {
			return data->isManualReset;
		}

		/// <summary>
		/// Name or empty line if the event is unnamed
		/// </summary>
		/// <returns> String with name or empty string if unnamed </returns>
		ForceInline const String& GetName() const {
			return data->Name;
		}

	public:
		/// <summary>
		/// Event type (either manually resetting the event or OS features do it automatically)
		/// </summary>
		Boolean Property(GetIsManualReset) isManualReset;

		/// <summary>
		/// Name or empty line if the event is unnamed
		/// </summary>
		String Property(GetName) Name;

	public:
		friend UInt64 WaitForAll(Containers::Array<SynchronizeEvent>&& events);
		friend UInt64 WaitForAny(Containers::Array<SynchronizeEvent>&& events);
		friend UInt64 WaitForAll(Containers::Array<SynchronizeEvent>&& events, SizeT timeOutMs);
		friend UInt64 WaitForAny(Containers::Array<SynchronizeEvent>&& events, SizeT timeOutMs);
		friend UInt64 WaitForAll(const Containers::Array<SynchronizeEvent>& events);
		friend UInt64 WaitForAny(const Containers::Array<SynchronizeEvent>& events);
		friend UInt64 WaitForAll(const Containers::Array<SynchronizeEvent>& events, SizeT timeOutMs);
		friend UInt64 WaitForAny(const Containers::Array<SynchronizeEvent>& events, SizeT timeOutMs);

	public:
		/// <summary>
		/// Realization of waiting for several events
		/// </summary>
		/// <param name="events">Array of events</param>
		/// <param name="isWaitAll">Whether to expect everything. If "false" then the waiting occurs on any</param>
		/// <param name="timeOutMs">Waiting time or "SizeT max" if need to wait without the timeout</param>
		friend UInt64 WaitForMany(const Containers::Array<SynchronizeEvent>& events, Boolean isWaitAll, SizeT timeOutMs = Math::Max<SizeT>());

	public:
		RefCountClass(SynchronizeEvent, data)
	};

	/// <summary>
	/// Waiting on all the events in array of events
	/// </summary>
	/// <param name="events">Array of events</param>
	ForceInline UInt64 WaitForAll(Containers::Array<SynchronizeEvent>&& events) {
		return WaitForMany(events, true);
	}

	/// <summary>
	/// Waiting on every event in array of events
	/// </summary>
	/// <param name="events">Array of events</param>
	ForceInline UInt64 WaitForAny(Containers::Array<SynchronizeEvent>&& events) {
		return WaitForMany(events, false);
	}

	/// <summary>
	/// Waiting on all the events in array of events with the maximal waiting time
	/// </summary>
	/// <param name="events">Array of events</param>
	/// <param name="timeOutMs">Waiting time or "SizeT max" if need to wait without the timeout</param>
	ForceInline UInt64 WaitForAll(Containers::Array<SynchronizeEvent>&& events, SizeT timeOutMs) {
		return WaitForMany(events, true, timeOutMs);
	}

	/// <summary>
	/// Waiting on every event in array of events with the maximal waiting time
	/// </summary>
	/// <param name="events">Array of events</param>
	/// <param name="timeOutMs">Waiting time or "SizeT max" if need to wait without the timeout</param>
	ForceInline UInt64 WaitForAny(Containers::Array<SynchronizeEvent>&& events, SizeT timeOutMs) {
		return WaitForMany(events, false, timeOutMs);
	}

	/// <summary>
	/// Waiting on all the events in array of events
	/// </summary>
	/// <param name="events">Array of events</param>
	ForceInline UInt64 WaitForAll(const Containers::Array<SynchronizeEvent>& events) {
		return WaitForMany(events, true);
	}

	/// <summary>
	/// Waiting on every event in array of events
	/// </summary>
	/// <param name="events">Array of events</param>
	ForceInline UInt64 WaitForAny(const Containers::Array<SynchronizeEvent>& events) {
		return WaitForMany(events, false);
	}

	/// <summary>
	/// Waiting on all the events in array of events with the maximal waiting time
	/// </summary>
	/// <param name="events">Array of events</param>
	/// <param name="timeOutMs">Waiting time or "SizeT max" if need to wait without the timeout</param>
	ForceInline UInt64 WaitForAll(const Containers::Array<SynchronizeEvent>& events, SizeT timeOutMs) {
		return WaitForMany(events, true, timeOutMs);
	}

	/// <summary>
	/// Waiting on every event in array of events with the maximal waiting time
	/// </summary>
	/// <param name="events">Array of events</param>
	/// <param name="timeOutMs">Waiting time or "SizeT max" if need to wait without the timeout</param>
	ForceInline UInt64 WaitForAny(const Containers::Array<SynchronizeEvent>& events, SizeT timeOutMs) {
		return WaitForMany(events, false, timeOutMs);
	}
}

#pragma warning(pop)