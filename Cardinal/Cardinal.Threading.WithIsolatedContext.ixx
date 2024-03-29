export module Cardinal.Threading:WithIsolatedContext;

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
	/// Manual thread context object
	/// </summary>
	class IsolatedContext {
	private:
		ContextsBlock contexts;

	public:
		/// <summary>
		/// Constructor which creates the new context by using the current as a parent
		/// </summary>
		IsolatedContext();

	public:
		/// <summary>
		/// Returns the context which contains this object
		/// </summary>
		/// <returns>Context object</returns>
		ContextsBlock& GetContexts() { return contexts; }

		/// <summary>
		/// Returns the context which contains this object
		/// </summary>
		/// <returns>Context object</returns>
		const ContextsBlock& GetContexts() const { return contexts; }

	private:
		ContextsBlock* volatile SetNewConxtextsBlock(ContextsBlock& contexts);
		void ResetContextsBlock(ContextsBlock* volatile oldContextsBlock);

	public:
		/// <summary>
		/// Executes the <see cref="Funct"/> with new thread contexts block
		/// </summary>
		/// <param name="Funct">A function or lambda which executes with new thread contexts block</param>
		/// <returns>Same as "Funct"</returns>
		template<typename T> requires Concepts::IsInvockable<T>
		auto WithContextBlock(T&& funct) -> decltype(funct()) {
			if constexpr (TypeTraits::IsSameV<decltype(funct()), void>)
			{
				decltype(auto) oldContextBlock = SetNewConxtextsBlock(contexts);
				try
				{
					funct();
				}
				catch (...)
				{
					ResetContextsBlock(oldContextBlock);
					Internals::__CxxRethrowImpl();
				}
				ResetContextsBlock(oldContextBlock);
			}
			else
			{
				decltype(auto) oldContextBlock = SetNewConxtextsBlock(contexts);
				try
				{
					if constexpr (TypeTraits::IsLValueReferenceV<decltype(funct())>)
					{
						decltype(auto) ret(funct());
						ResetContextsBlock(oldContextBlock);
						return ret;
					}
					else
					{
						decltype(auto) ret(MoveRValue(funct()));
						ResetContextsBlock(oldContextBlock);
						return ret;
					}
				}
				catch (...)
				{
					ResetContextsBlock(oldContextBlock);
					Internals::__CxxRethrowImpl();
				}
			}
		}
	};
}

#pragma warning(pop)