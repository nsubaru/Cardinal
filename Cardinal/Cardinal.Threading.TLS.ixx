export module Cardinal.Threading.TLS;

export import :Base;
export import :Exceptions;

import Cardinal.Exceptions.Internals;

#pragma warning(push)
#pragma warning(disable: 4996)

export namespace Cardinal::Threading
{
	class IThreadable;
	enum EThreadabledStates;
	class ContextsBlock;

	/// <summary>
	/// Main thread local storage of kernel per thread data, required for Threading API and Exception ABI
	/// </summary>
	inline __declspec(thread) struct TLS {
		IThreadable* Threadable = nullptr;
		volatile EThreadabledStates* volatile SecondState;
		volatile EThreadabledStates* volatile State;
		TlsExceptionData ExceptionInfo;
		ContextsBlock* volatile contexts;
		bool IsDebugThread;
	} TLSData = { nullptr };

	/// <summary>
	/// Returns the main thread local storage of kernel per thread data
	/// </summary>
	InternalApiCall ForceInline TLS* GetTLS() { return &TLSData; }

	/// <summary>
	/// Initialize the current thread kernel data
	/// </summary>
	InternalApiCall void ConfigureTls(
		IThreadable* threadable,
		volatile EThreadabledStates* volatile state,
		volatile EThreadabledStates* volatile secondState,
		ContextsBlock* contexts
	);

	/// <summary>
	/// Returns is thread for debug
	/// </summary>
	/// <returns>Debug status</returns>
	inline bool GetIsDebugThread() {
		return TLSData.IsDebugThread;
	}

	/// <summary>
	/// Set new thread debug state
	/// </summary>
	/// <param name="newValue">New debug status</param>
	/// <returns>Old value</returns>
	inline bool SetIsDebugThread(bool newValue) {
		Interlocked::Swap(TLSData.IsDebugThread, newValue);
		return newValue;
	}

	/// <summary>
	/// Perform action in debug thread mode
	/// </summary>
	/// <param name="Funct">Action lambda</param>
	/// <returns>Lambda result</returns>
	template<typename T> requires Concepts::IsInvockable<T>
	inline constexpr ForceInline decltype(auto) DebugThread(T&& Funct) {
		if (TypeTraits::IsCompileTimeEvaluating())
		{
			if constexpr (TypeTraits::IsSameT<decltype(Funct()), void>())
			{
				Funct();
				return;
			}
			else
			{
				return Funct();
			}
		}
		else if constexpr (TypeTraits::IsSameT<decltype(Funct()), void>())
		{
			auto oldValue = SetIsDebugThread(true);
			try
			{
				Funct();
			}
			catch (...)
			{
				SetIsDebugThread(oldValue);
				Internals::Exceptions::__ReThrow();
			}
			SetIsDebugThread(oldValue);
		}
		else
		{
			auto oldValue = SetIsDebugThread(true);
			try
			{
				if constexpr (TypeTraits::IsLValueReferenceV<decltype(Funct())>)
				{
					decltype(Funct()) ret(Funct());
					SetIsDebugThread(oldValue);
					return ret;
				}
				else
				{
					decltype(Funct()) ret(MoveRValue(Funct()));
					SetIsDebugThread(oldValue);
					return ret;
				}
			}
			catch (...)
			{
				SetIsDebugThread(oldValue);
				Internals::Exceptions::__ReThrow();
			}
		}
	}

	/// <summary>
	/// Deletes the current thread kernel data
	/// </summary>
	InternalApiCall void DestroyTls();
}

#pragma warning(pop)