export module Cardinal.Containers.Delegate;

export import Cardinal.Containers.Base;
export import Cardinal.Containers.Tuple;
export import Cardinal.Containers.Base;
export import Cardinal.Exceptions;

export namespace Cardinal
{
#pragma warning(push)
#pragma warning(disable: 4996)
    /// <summary>
    /// An attempt to gain result from a function that is not yet performed
    /// </summary>
    class CantGetResultNotInvokedFunction final : public Exception {
    public:
        /// <summary>
        /// The exception generation of attempt to gain result from a function that is not yet performed
        /// </summary>
        /// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
        /// <param name = 'FunctName'>The function name</param>
        ForceInline CantGetResultNotInvokedFunction(SourceLocation sourceLocation)
            : Exception(sourceLocation, L"Impossible to gain the result from the not yet executed function") {
        }

        /// <summary>
        /// Text representation of exception information
        /// </summary>
        virtual ExceptionString toString() const override final {
            return ExceptionString::Combine({
                ExceptionString(exceptionName), ExceptionString(L". "),
                Location.toString(), ExceptionString(L".")
                });
        }
    };

    /// <summary>
    /// An attempt to restart the already executed delegate or the delegate which is executing now
    /// </summary>
    class TryReStartDelegate final : public Exception {
    public:
        /// <summary>
        /// The exception generation of attempt to restart the already executed delegate or the delegate which is executing now
        /// </summary>
        /// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
        /// <param name = 'FunctName'>The function name</param>
        ForceInline TryReStartDelegate(SourceLocation sourceLocation)
            : Exception(sourceLocation, L"Impossible to restart the delegate for execution") {
        }

        /// <summary>
        /// Text representation of exception information
        /// </summary>
        virtual ExceptionString toString() const override final {
            return ExceptionString::Combine({
                ExceptionString(exceptionName), ExceptionString(L". "),
                Location.toString(), ExceptionString(L".")
                });
        }
    };

    /// <summary>
    /// The delegate during the execution process
    /// </summary>
    class AlredyExecute final : public Exception {
    public:
        /// <summary>
        /// The exception constructor
        /// </summary>
        /// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
        /// <param name = 'FunctName'>The function name</param>
        ForceInline AlredyExecute(SourceLocation sourceLocation)
            : Exception(sourceLocation, L"Impossible to gain the delegate result during the execution process") {
        }

        /// <summary>
        /// Text representation of exception information
        /// </summary>
        virtual ExceptionString toString() const override final {
            return ExceptionString::Combine({
                ExceptionString(exceptionName), ExceptionString(L". "),
                Location.toString(), ExceptionString(L".")
                });
        }
    };
#pragma warning(pop)
}

export namespace Cardinal
{
    /// <summary>
    /// The parent class for the function object with arguments
    /// </summary>
    class FunctionObj {
    protected:
        /// <summary>
        /// Returns the result (the realization in "TemplateFunctionT")
        /// </summary>
        /// <param name="type">Requested type</param>
        /// <returns>The pointer on data</returns>
        virtual void* __get_result(const TypeInfo& type) = 0;

    public:
        /// <summary>
        /// Executes the function (the realization in "TemplateFunctionT")
        /// </summary>
        virtual void Init() = 0;

        /// <summary>
        /// Returns "true" if the function is already launched (the realization in "TemplateFunctionT")
        /// </summary>
        /// <returns>Returns "true" if the function is already launched</returns>
        virtual bool IsStarted() const = 0;

        /// <summary>
        /// Returns "true" if the function has completed its work.
        /// If the exception is called during the execution the function will be considered as a complete,
        /// but there will be no result and during an attempt to gain it the "NullPointException" exception will be generated
        /// The realization is in "TemplateFunctionT"
        /// </summary>
        /// <returns>Returns "true" if the function has completed its work</returns>
        virtual bool IsEnded() const = 0;

        /// <summary>
        /// Returns "true" if the function is already launched
        /// </summary>
        /// <exception cref="Cardinal::Exceptions::NullPointException">If no result or the function has stopped its work due to exception generation</exception>
        /// <exception cref="Cardinal::Exceptions::InvalidType">If the result type is invalid</exception>
        template<typename T>
        ForceInline T GetResult() {
            return MoveRValue(*static_cast<T*>(__get_result(typeid(T))));
        }

        /// <summary>
        /// The destructor for the correct object deletion which inherits this class
        /// </summary>
        virtual ~FunctionObj() = 0 {};
    };

    namespace Details
    {
        /// <summary>
        /// Delegate realization internal class
        /// In fact is a function with arguments,
        /// as if the parent class is "FunctionObj" then it is possible to gain the pointer on a "FunctionObj",
        /// regardless of the type of the function
        /// </summary>
        template<typename TF, typename ... TArgs>
        class TemplateFunctionT final : public FunctionObj {
            // The function
            TF f;
            // The container for the result
            volatile void* result;
            // The container for the arguments
            Containers::Tuple<TArgs...> args;
            // The function execution completion flag
            volatile bool isEnded = false;
            // Function starting flag
            volatile bool isStarted = false;

            using retType = TypeTraits::RemoveCvRefV<decltype(args.CallWithArgsFromTuple(f))>;

        protected:
            /// <summary>
            /// Returns the result and generates next exceptions
            /// </summary>
            /// <exception cref="Cardinal::Exceptions::NullPointException">If no result or the function has stopped its work due to exception generation</exception>
            /// <exception cref="Cardinal::Exceptions::InvalidType">If the result type is invalid</exception>
            ForceInline virtual void* __get_result(const TypeInfo& type) override final {
                if constexpr (TypeTraits::IsSameT<retType, void>::State)
                {
                    Throw(InvalidTypeException(SourcePosition, typeid(void), type));
                }
                else
                {
                    if (Interlocked::Load(result) == nullptr)
                        Throw(NullPointException(SourcePosition, reinterpret_cast<SizeT>(this->result), NullPointException::Types::Read));
                    else
                    {
                        if (typeid(retType) != type)
                            Throw(InvalidTypeException(SourcePosition, typeid(retType), type));
                        return Interlocked::Load(result);
                    }
                }
            }

        private:
            ForceInline void DestroyArgs(Containers::Tuple<TArgs...>&& argsTupple) {
                (void) argsTupple;
            }

        public:
            /// <summary>
            /// Executes the function, notes that the function has been started and completed,
            /// and if it is the result on a "void" function, stores the result
            ForceInline void Init() override final {
                isStarted = true;
                try
                {
                    // The compilation time condition which checks is the function returns "void"
                    if constexpr (!TypeTraits::IsSameT<retType, void>::State)
                    {
                        // If false then stores the result in the "res" pointer
                        // where the result will be moved by moving constructor
                        Interlocked::Store(result, new retType(args.CallWithArgsFromTuple(f)));
                    }
                    // If the function returns "void" then just call it
                    else
                    {
                        args.CallWithArgsFromTuple(f);
                    }
                    // NOTE: Arguments destroying (if you do not do this then cyclic links are possible in some cases)
                    // DestroyArgs(Rem::MoveRValue(args));
                }
                catch ([[maybe_unused]] InteruptException&)
                {
                    isEnded = true;
                    ReThrow;
                }
                // If the exception is catched then consider that the function has stopped its work
                catch ([[maybe_unused]] Exception&)
                {
                    isEnded = true;
                    ReThrow;
                }
                isEnded = true;
            }
            /// <summary>
            /// The constructor. It initializes the tuple with arguments, nullifies the pointer on result and writes the function
            /// </summary>
            template<typename TF, typename ... TArgs>
            ForceInline TemplateFunctionT(TF&& f, TArgs&&... args) : f(ForwardReference<TF>(f)), result(nullptr), args(ForwardReference<TArgs>(args)...) {}

            /// <summary>
            /// Returns "true" if the function was started
            /// </summary>
            ForceInline virtual bool IsStarted() const override final {
                return isStarted;
            }

            /// <summary>
            /// Returns "true" if the function was ended
            /// </summary>
            ForceInline virtual bool IsEnded() const override final {
                return isEnded;
            }

            /// <summary>
            /// The destructor. It deletes the result if it is exists
            /// </summary>
            ForceInline virtual ~TemplateFunctionT() override final {
                if constexpr (!TypeTraits::IsSameT<retType, void>::State)
                {
                    if (result != nullptr)
                    {
                        delete reinterpret_cast<TypeTraits::AddPointer<retType>>(Interlocked::Load(result));

                        Interlocked::Store(result, nullptr);
                    }
                }
            }
        };
    }

    /// <summary>
    /// The delegate - it is the object which incapsulates the function with arguments,
    /// thereby allowing to executes it even in different thread, the APC of some thread or a thread pool
    /// </summary>
    class Delegate final : public IDisposable {
    private:
        // Internal structure which has the delegate data
        struct delegate {
            FunctionObj* function;
            RefCountT RefCount;
            ForceInline delegate(FunctionObj* function) : function(function), RefCount(1) {}
        };
        // A pointer on the given delegate data
        delegate* data;

        // Inherited via IDisposable
        virtual void Dispose() override final {
            delete data->function;
        }

    public:
        /// <summary>
        /// The constructor with "F" function and the "Args" set of arguments (could be without arguments)
        /// </summary>
        /// <param name="f">The function (a pointer on it)</param>
        /// <param name="args">The function arguments</param>
        template<typename TF, typename ... TArgs>
            requires Concepts::IsInvockable<TF, TArgs...>
        ForceInline Delegate(TF&& f, TArgs&&... args) : data(new delegate(new Details::TemplateFunctionT<TF, TArgs...>(ForwardReference<TF>(f), ForwardReference<TArgs>(args)...))) {};

        /// <summary>
        /// Returns "true" if the delegate is working right now
        /// Generates "NullPointException" if the delegate is invalid
        /// </summary>
        /// <exception cref="Cardinal::NullPointException">If no result or the function has stopped its work due to exception generation</exception>
        /// <returns>Returns "true" if the delegate is working right now</returns>
        ForceInline Boolean GetIsStarted() const {
            if (this->data == nullptr)
                Throw(NullPointException(SourcePosition, reinterpret_cast<SizeT>(this->data), NullPointException::Types::Read));
            return this->data->function->IsStarted();
        }

        /// <summary>
        /// Returns "true" if the delegates work has been finished.
        /// Generates "NullPointException" if the delegate is invalid
        /// </summary>
        /// <exception cref="Cardinal::NullPointException">If no result or the function has stopped its work due to exception generation</exception>
        /// <returns>Returns "true" if the delegates work has been finished</returns>
        ForceInline Boolean GetIsEnded() const {
            if (this->data == nullptr)
                Throw(NullPointException(SourcePosition, reinterpret_cast<SizeT>(this->data), NullPointException::Types::Read));
            return this->data->function->IsEnded();
        }

        /// <summary>
        /// The function execution, can occur only once. For getting the result there is a function "GetResult"
        /// </summary>
        /// <exception cref="Cardinal::NullPointException">If the delegate data refers to "nullptr"</exception>
        /// <exception cref="Cardinal::TryReStartDelegate">If the delegate was or is being executed </exception>
        ForceInline void Init() volatile {
            if (this->data == nullptr)
                Throw(NullPointException(SourcePosition, reinterpret_cast<SizeT>(this->data), NullPointException::Types::Read));
            if (this->data->function->IsEnded() || this->data->function->IsStarted())
                Throw(TryReStartDelegate(SourcePosition));
            this->data->function->Init();
        }

        /// <summary>
        /// Returns the delegate execution result
        /// </summary>
        /// <exception cref="Cardinal::NullPointException">If the delegate is without a function (invalid), or the function is not started yet,
        /// or the function has not been completed because the exception was generated
        /// </exception>
        /// <exception cref=""Cardinal::AlredyExecute>If the function's work is in progress</exception>
        /// <exception cref="InvalidType">If the "T" type does not match the function result type</exception>
        /// <returns>The function result value</returns>
        template<typename T>
        ForceInline T GetResult() {
            if (this->data == nullptr)
                Throw(NullPointException(SourcePosition, reinterpret_cast<SizeT>(this->data), NullPointException::Types::Read));
            if (!this->data->function->IsStarted())
                Throw(NullPointException(SourcePosition, reinterpret_cast<SizeT>(this->data->function), NullPointException::Types::Execute));
            if (this->data->function->IsStarted() && !this->data->function->IsEnded())
                Throw(AlredyExecute(SourcePosition));
            return this->data->function->GetResult<T>();
        }

        /// <summary>
        /// Returns the delegate execution result
        /// </summary>
        /// <exception cref="Cardinal::NullPointException">If the delegate is without a function (invalid), or the function is not started yet,
        /// or the function has not been completed because the exception was generated
        /// </exception>
        /// <exception cref=""Cardinal::AlredyExecute>If the function's work is in progress</exception>
        /// <exception cref="InvalidType">If the "T" type does not match the function result type</exception>
        /// <returns>The function result value</returns>
        template<typename T>
        ForceInline T GetResult() volatile {
            if (this->data == nullptr)
                Throw(NullPointException(SourcePosition, reinterpret_cast<SizeT>(this->data), NullPointException::Types::Read));
            if (!this->data->function->IsStarted())
                Throw(NullPointException(SourcePosition, reinterpret_cast<SizeT>(this->data->function), NullPointException::Types::Execute));
            if (this->data->function->IsStarted() && !this->data->function->IsEnded())
                Throw(AlredyExecute(SourcePosition));
            return this->data->function->GetResult<T>();
        }

        /// <summary>
        /// The operator-functor (executes the function)
        /// </summary>
        /// <returns>Returns the reference to a given object</returns>
        ForceInline volatile Delegate& operator()() volatile {
            this->Init();
            return *this;
        }

        /// <summary>
        /// The operator-functor (executes the function)
        /// </summary>
        /// <returns>Returns the reference to a given object</returns>
        ForceInline Delegate& operator()() {
            this->Init();
            return *this;
        }

        RefCountClass(Delegate, data);
    };

    /// <summary>
    /// Creates the delegate with the "F" function and "Args" arguments (could be without arguments)
    /// </summary>
    /// <param name="f">The function (a pointer on it)</param>
    /// <param name="args">The function arguments</param>
    /// <returns>The delegate object</returns>
    template<typename TF, typename ... TArgs>
        requires Concepts::IsInvockable<TF, TArgs...>
    ForceInline Delegate CreateAndPackFunctionObj(TF&& f, TArgs&&... args) {
        return Delegate(Cardinal::ForwardReference<TF>(f), Cardinal::ForwardReference<TArgs>(args)...);
    }
};