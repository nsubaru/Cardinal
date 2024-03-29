export module Cardinal.Threading:Async.Base;

export import Cardinal.Core;

export import :Base;
export import :IThreadable;
export import :Exceptions;
export import :Context;

export import :Task;

#pragma warning(push)
#pragma warning(disable: 4251)

// Intrinsics used in implementation of coroutine_handle
extern "C" size_t _coro_resume(void*);
extern "C" void _coro_destroy(void*);
extern "C" size_t _coro_done(void*);
#pragma intrinsic(_coro_resume)
#pragma intrinsic(_coro_destroy)
#pragma intrinsic(_coro_done)

extern "C" size_t _coro_frame_size();
extern "C" void* _coro_frame_ptr();
extern "C" void _coro_init_block();
extern "C" void* _coro_resume_addr();
extern "C" void _coro_init_frame(void*);
extern "C" void _coro_save(size_t);
extern "C" void _coro_suspend(size_t);
extern "C" void _coro_cancel();
extern "C" void _coro_resume_block();

#pragma intrinsic(_coro_frame_size)
#pragma intrinsic(_coro_frame_ptr)
#pragma intrinsic(_coro_init_block)
#pragma intrinsic(_coro_resume_addr)
#pragma intrinsic(_coro_init_frame)
#pragma intrinsic(_coro_save)
#pragma intrinsic(_coro_suspend)
#pragma intrinsic(_coro_cancel)
#pragma intrinsic(_coro_resume_block)

namespace std
{
    template <class _Ret, class = void>
    struct _Coroutine_traits_sfinae {};
    template <class _Ret>
    struct _Coroutine_traits_sfinae<_Ret, Cardinal::TypeTraits::VoidT<typename _Ret::promise_type>> {
        using promise_type = typename _Ret::promise_type;
    };

    template <typename _Ret, typename... _Ts>
    struct coroutine_traits : _Coroutine_traits_sfinae<_Ret> {};

    template <typename _PromiseT = void>
    struct coroutine_handle;

    template <>
    struct coroutine_handle<void> {
        coroutine_handle() noexcept = default;

        coroutine_handle(decltype(nullptr)) noexcept {}

        coroutine_handle& operator=(decltype(nullptr)) noexcept {
            _Ptr = nullptr;
            return *this;
        }

        static coroutine_handle from_address(void* _Addr) noexcept {
            coroutine_handle _Result;
            _Result._Ptr = static_cast<_Resumable_frame_prefix*>(_Addr);
            return _Result;
        }

        void* address() const noexcept {
            return _Ptr;
        }

        __declspec(
            deprecated("is deprecated. Use coroutine_handle::address() instead")) void* to_address() const noexcept {
            return _Ptr;
        }

        void operator()() const {
            resume();
        }

        explicit operator bool() const noexcept {
            return _Ptr != nullptr;
        }

        void resume() const {
            _coro_resume(_Ptr);
        }

        void destroy() {
            _coro_destroy(_Ptr);
        }

        bool done() const {
            return _Ptr->_Index == 0;
        }

        struct _Resumable_frame_prefix {
            using _Resume_fn = void(__cdecl*)(void*);

            _Resume_fn _Fn;
            Cardinal::UInt16 _Index;
            Cardinal::UInt16 _Flags;
        };

    protected:
        _Resumable_frame_prefix* _Ptr = nullptr;
    };

    template <typename _PromiseT>
    struct coroutine_handle : coroutine_handle<> {
        using coroutine_handle<>::coroutine_handle;

        __declspec(deprecated("with pointer parameter is deprecated. Use coroutine_handle::from_promise(T&) "
            "instead")) static coroutine_handle from_promise(_PromiseT* _Prom) noexcept {
            return from_promise(*_Prom);
        }

        static coroutine_handle from_promise(_PromiseT& _Prom) noexcept {
            auto _FramePtr = reinterpret_cast<char*>(&_Prom) + _ALIGNED_SIZE;
            coroutine_handle<_PromiseT> _Result;
            _Result._Ptr = reinterpret_cast<_Resumable_frame_prefix*>(_FramePtr);
            return _Result;
        }

        static coroutine_handle from_address(void* _Addr) noexcept {
            coroutine_handle _Result;
            _Result._Ptr = static_cast<_Resumable_frame_prefix*>(_Addr);
            return _Result;
        }

        coroutine_handle& operator=(decltype(nullptr)) noexcept {
            _Ptr = nullptr;
            return *this;
        }

        static const size_t _ALIGN_REQ = sizeof(void*) * 2;

        static const size_t _ALIGNED_SIZE =
            Cardinal::TypeTraits::IsEmptyV<_PromiseT> ? 0 : ((sizeof(_PromiseT) + _ALIGN_REQ - 1) & ~(_ALIGN_REQ - 1));

        _PromiseT& promise() const noexcept {
            return *const_cast<_PromiseT*>(
                reinterpret_cast<const _PromiseT*>(reinterpret_cast<const char*>(_Ptr) - _ALIGNED_SIZE));
        }
    };

    NoDiscard inline bool operator==(coroutine_handle<> _Left, coroutine_handle<> _Right) noexcept {
        return _Left.address() == _Right.address();
    }

    NoDiscard inline bool operator<(coroutine_handle<> _Left, coroutine_handle<> _Right) noexcept {
        return _Left.address() < _Right.address();
    }

    NoDiscard inline bool operator!=(coroutine_handle<> _Left, coroutine_handle<> _Right) noexcept {
        return !(_Left == _Right);
    }

    NoDiscard inline bool operator>(coroutine_handle<> _Left, coroutine_handle<> _Right) noexcept {
        return _Right < _Left;
    }

    NoDiscard inline bool operator<=(coroutine_handle<> _Left, coroutine_handle<> _Right) noexcept {
        return !(_Left > _Right);
    }

    NoDiscard inline bool operator>=(coroutine_handle<> _Left, coroutine_handle<> _Right) noexcept {
        return !(_Left < _Right);
    }

    struct suspend_if {
        bool _Ready;

        explicit suspend_if(bool _Condition) noexcept : _Ready(!_Condition) {}

        bool await_ready() noexcept {
            return _Ready;
        }

        void await_suspend(coroutine_handle<>) noexcept {}

        void await_resume() noexcept {}
    };

    struct suspend_always {
        bool await_ready() noexcept {
            return false;
        }

        void await_suspend(coroutine_handle<>) noexcept {}

        void await_resume() noexcept {}
    };

    struct suspend_never {
        bool await_ready() noexcept {
            return true;
        }

        void await_suspend(coroutine_handle<>) noexcept {}

        void await_resume() noexcept {}
    };

    template <typename _Ret, typename... _Ts>
    struct _Resumable_helper_traits {
        using _Traits = coroutine_traits<_Ret, _Ts...>;
        using _PromiseT = typename _Traits::promise_type;
        using _Handle_type = coroutine_handle<_PromiseT>;

        static _PromiseT* _Promise_from_frame(void* _Addr) noexcept {
            return reinterpret_cast<_PromiseT*>(static_cast<char*>(_Addr) - _Handle_type::_ALIGNED_SIZE);
        }

        static _Handle_type _Handle_from_frame(void* _Addr) noexcept {
            return _Handle_type::from_promise(*_Promise_from_frame(_Addr));
        }

        static void _Set_exception(void* _Addr) {
            _Promise_from_frame(_Addr)->set_exception(Cardinal::Exception::StoreCurrentException());
        }

        static void _ConstructPromise(void* _Addr, void* _Resume_addr, int _HeapElision) {
            *static_cast<void**>(_Addr) = _Resume_addr;
            *reinterpret_cast<Cardinal::UInt32*>(reinterpret_cast<Cardinal::UInt64>(_Addr) + sizeof(void*)) =
                2u + (_HeapElision ? 0u : 0x10000u);
            auto _Prom = _Promise_from_frame(_Addr);
            ::new (static_cast<void*>(_Prom)) _PromiseT();
        }

        static void _DestructPromise(void* _Addr) {
            _Promise_from_frame(_Addr)->~_PromiseT();
        }
    };

    template<typename TRet, typename TPayload, typename ... TArgs>
    struct coroutine_traits<Cardinal::Threading::BaseTask<TRet, TPayload>, TArgs...> {
        template<typename T>
        using Task = Cardinal::Threading::BaseTask<T, TPayload>;
        struct promise_type {
            Task<TRet> task;

            Task<TRet> get_return_object() {
                return task;
            }

            static constexpr suspend_never initial_suspend() noexcept {
                return {};
            }

            static constexpr suspend_never final_suspend() noexcept {
                return {};
            }

            template <class U>
            void return_value(U&& value) {
                task.SetValue(Cardinal::ForwardReference<U>(value));
                task.MarkFinished();
            }

            void unhandled_exception() {
                task.SetException(Cardinal::Exception::StoreCurrentException());
                task.MarkFinished();
            }
        };
    };

    template<typename TPayload, typename ... TArgs>
    struct coroutine_traits<Cardinal::Threading::BaseTask<void, TPayload>, TArgs...> {
        template<typename T>
        using Task = Cardinal::Threading::BaseTask<T, TPayload>;
        struct promise_type {
            Task<void> task;

            Task<void> get_return_object() {
                return task;
            }

            static constexpr suspend_never initial_suspend() noexcept {
                return {};
            }

            static constexpr suspend_never final_suspend() noexcept {
                return {};
            }

            void unhandled_exception() {
                task.SetException(Cardinal::Exception::StoreCurrentException());
                task.MarkFinished();
            }

            void return_void() {
                task.MarkFinished();
            }
        };
    };
}

namespace Cardinal::Threading
{
    /// <summary>
    /// Task state machine type
    /// </summary>
    /// <typeparam name="T">Task result type</typeparam>
    /// <typeparam name="TPayload">Task additional payload type</typeparam>
    template<typename T, typename TPayload>
    struct TaskAwaiter {
        BaseTask<T, TPayload> task;

        /// <summary>
        /// Called when before init async operation
        /// </summary>
        /// <returns>If true then result alredy available and async operation not neaded, for general case always false</returns>
        bool await_ready() const {
            return false;
        }

        /// <summary>
        /// Called for init async operation, performe additional payload init if payload require it
        /// </summary>
        /// <param name="resume_cb">Function context(Data stack)</param>
        void await_suspend(std::coroutine_handle<> resume_cb) {
            task.SetContext(resume_cb.address());

            if constexpr (!Concepts::IsSame<TPayload, void> && IsTaskPayloadRequireInitAndStop<TPayload>)
            {
                static_cast<ITaskPayload<TPayload, true>&>(task.GetPayload()).InitTask(task);
            }
        }

        /// <summary>
        /// Called for complete async operation
        /// </summary>
        /// <returns>Task result</returns>
        decltype(auto) await_resume() {
            return MoveRValue(task).GetValue();
        }
    };
}

/// <summary>
/// Performe async operation with result T and TPayload additional task payload
/// </summary>
/// <typeparam name="T">The result type</typeparam>
/// <typeparam name="TPayload">Additional data type</typeparam>
/// <param name="task">Task object</param>
/// <returns>Task result</returns>
template<typename T, typename TPayload>
inline decltype(auto) operator co_await(Cardinal::Threading::BaseTask<T, TPayload>&& task) {
    return Cardinal::Threading::TaskAwaiter<T, TPayload>{ Cardinal::MoveRValue(task) };
}

#pragma warning(pop)