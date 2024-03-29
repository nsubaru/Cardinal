export module Cardinal.Threading:Threadables.Internal;

export import :Threadables.Base;

export namespace Cardinal::Threading::Threadables
{
	// Thread interface registration
	ThreadableRef RegisterIThreadable(
		IThreadable*&& threadable,
		volatile EThreadabledStates* volatile state,
		volatile EThreadabledStates* volatile secondState,
		ContextsBlock* context);

	// Thread interface deregistration
	void UnRegisterIThreadable(ThreadableRef&& ref);

	// Thread name setting
	void SetName(const String& NewName, Handle hThread);

	// Thread name resetting
	void ReSetName(Handle hThread);

	// New context creation using the current optimization for some types of threads
	ContextsBlock CreateInnerContextsBlock(EThreadableTypes newThreadableType, Boolean isManual = false);
}