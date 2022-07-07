export module Cardinal.Threading;

export import :LockPrimitives;

export namespace Cardinal::Core::Threading
{
	Handle StartThread(void(*threadEntryPoint)(void*), void* arg);
}