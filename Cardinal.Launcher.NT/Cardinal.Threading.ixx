export module Cardinal.Threading;

export import :LockPrimitives;

export namespace Cardinal::Threading
{
	Handle StartThread(void(*threadEntryPoint)(void*), void* arg);
}