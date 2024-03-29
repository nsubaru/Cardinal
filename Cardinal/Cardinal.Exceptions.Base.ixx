export module Cardinal.Exceptions:Base;

export import Cardinal.Core;
export import Cardinal.Time;
export import Cardinal.ConstString;
export import Cardinal.String;
export import Cardinal.Containers.SmartPtr;

#pragma warning(push)
#pragma warning(disable: 4996)

export namespace Cardinal::Memory
{
	VirtualClass ExceptionHeap final : public BasicHeap {
	public:
		/// <summary>
		/// Standard constructor
		/// </summary>
		ForceInline ExceptionHeap() = default;

		/// <summary>
		/// Standard copying operator
		/// </summary>
		ForceInline ExceptionHeap& operator=(const ExceptionHeap&) = default;

		/// <summary>
		/// Standard destructor
		/// </summary>
		ForceInline ~ExceptionHeap() override {}

	public:
		/// <summary>
		/// Memory allocation
		/// </summary>
		/// <param name='Size'>The area size in bytes</param>
		/// <returns>Allocated area</returns>
		virtual __declspec(restrict) DeprecatedCall(L"Low-level work with memory") void* Allocate(DataSizeT Size) final;

		/// <summary>
		/// Aligned memory allocation
		/// </summary>
		/// <param name='Size'>The area size in bytes</param>
		/// <param name='AllocationReq'>Alignment borders</param>
		/// <returns>Allocated area</returns>
		virtual __declspec(restrict) DeprecatedCall(L"Low-level work with memory") void* AllignedAllocate(DataSizeT Size, DataSizeT AllocationReq) final;

		/// <summary>
		/// Memory freeing
		/// </summary>
		/// <param name='memBlk'>The freeing memory block</param>
		virtual DeprecatedCall(L"Low-level work with memory") void* Free(void* Restrict memBlk) final;

		/// <summary>
		/// Memory freeing which was allocated on the border
		/// </summary>
		/// <param name='memBlk'>The freeing memory block</param>
		/// <param name='AllocationReq'>Alignment borders</param>
		virtual DeprecatedCall(L"Low-level work with memory") void* AllignedFree(void* Restrict memBlk, DataSizeT AllocationReq) final;

		/// <summary>
		/// The number of allocated memory blocks
		/// </summary>
		virtual SizeT CurrentMemBlkCount() final;

		/// <summary>
		/// The size of allocated memory in bytes
		/// </summary>
		virtual DataSizeT CurrentAllocatedMemory() final;

		/// <summary>
		/// The actual size of allocated memory in bytes
		/// </summary>
		virtual DataSizeT CurrentHeapUsage() final;
	};
}

export namespace Cardinal
{
	/// <summary>
	/// Base exception class
	/// </summary>
	class Exception;

	/// <summary>
	/// A string for saving text of exceptions
	/// </summary>
	using ExceptionString = BaseString<Memory::ExceptionHeap>;

	/// <summary>
	/// The object for saving the exception
	/// </summary>
	using ExceptionHolderType = BaseSmartPtr<Memory::ExceptionHeap, Exception>;

	/// <summary>
	/// Base exception class
	/// </summary>
	class Exception : public Memory::Allocator<Memory::ExceptionHeap>, public IClonable<ExceptionHolderType> {
	public:
		/// <summary>
		/// Call stack storage
		/// </summary>
		struct StackTraceRecord {
			/// <summary>
			/// The maximal number of functions adresses which can be saved in the storage
			/// </summary>
			static inline constexpr SizeT MaxStackTraceItemCount = (2Kb).Bytes / sizeof(SizeT);

			/// <summary>
			/// The actual number of function adresses
			/// </summary>
			SizeT Count;

			/// <summary>
			/// Function adresses
			/// </summary>
			SizeT FunctionAddress[MaxStackTraceItemCount];
		};

		/// <summary>
		/// Exception generation location
		/// </summary>
		struct ExeptionLocation {
			/// <summary>
			/// Source location
			/// </summary>
			SourceLocation SourceLocation;

			/// <summary>
			/// Thread identifier
			/// </summary>
			ID ThreadID;

			/// <summary>
			/// Text representation
			/// </summary>
			ExceptionString toString() const {
				return ExceptionString::Combine({
					ExceptionString(L"Generated in line ") , ExceptionString::toString(SourceLocation.Line),
					ExceptionString(L" in file ") , ExceptionString(SourceLocation.FileName),
					ExceptionString(L" in thread with ID ") , ExceptionString::toString(ThreadID)
					});
			}
		};

	protected:
		/// <summary>
		/// The given exception generation location
		/// </summary>
		ExeptionLocation location;

		/// <summary>
		/// Exception name
		/// </summary>
		ConstString exceptionName;

		/// <summary>
		/// Exception stack
		/// </summary>
		StackTraceRecord stackTrace;

		/// <summary>
		/// Inner exception
		/// </summary>
		ExceptionHolderType innerException;

	private:
		static ID GetTargetThreadID();
		friend void GetStackBackTrace(Exception::StackTraceRecord& stackTrace);
		friend ExceptionHolderType* GetInnerExceptionObjectPtr(Exception* exception);

	public:
		/// <summary>
		/// Copying constructor 
		/// </summary>
		/// <param name="from"></param>
		Exception(const Exception& from) :
			location(from.location),
			exceptionName(from.exceptionName),
			stackTrace(from.stackTrace),
			innerException(from.innerException) {
		}

		/// <summary>
		/// Moving constructor
		/// </summary>
		/// <param name="from"></param>
		Exception(Exception&& from) :
			location(from.location),
			exceptionName(MoveRValue(from.exceptionName)),
			stackTrace(from.stackTrace),
			innerException(MoveRValue(from.innerException)) {
		}

		/// <summary>
		/// Copying operator
		/// </summary>
		/// <param name="ex">Original</param>
		/// <returns>A reference to a current object</returns>
		Exception& operator=(const Exception& ex) {
			this->~Exception();
			this->Exception::Exception(ex);
			return *this;
		}

		/// <summary>
		/// Moving operator
		/// </summary>
		/// <param name="ex">Original</param>
		/// <returns>A reference to a current object</returns>
		Exception& operator=(Exception&& ex) {
			this->~Exception();
			this->Exception::Exception(MoveRValue(ex));
			return *this;
		}

	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
		/// <param name = 'FunctName'>The function name</param>
		/// <param name = 'Name'>Exception name</param>
		ForceInline Exception(SourceLocation sourceLocation, const ConstString& ExeptionName) noexcept
			:location({ sourceLocation, GetTargetThreadID() }), exceptionName(ExeptionName), stackTrace({ 0 }), innerException(nullptr) {
			GetStackBackTrace(static_cast<Exception::StackTraceRecord&>(this->stackTrace));
		}

		/// <summary>
		/// Returns the custom exception name
		/// </summary>
		ForceInline const ConstString& GetExceptioName() const { return exceptionName; }

		/// <summary>
		/// Returns the exception generation location
		/// </summary>
		ForceInline const ExeptionLocation& GetLocation() const { return location; }

		/// <summary>
		/// Returns the call stack during exception generation
		/// </summary>
		ForceInline const StackTraceRecord& GetStackTrace() const { return stackTrace; }

		/// <summary>
		/// Returns the object which contains the inner exception (the object can be empty)
		/// </summary>
		ForceInline const ExceptionHolderType& GetInnerException() const { return innerException; }

		/// <summary>
		/// Sets the object which contains the inner exception (the object can be empty)
		/// </summary>
		/// <param name="newInnerException">The object which contains a new exception</param>
		ForceInline void SetInnerException(const ExceptionHolderType& newInnerException) { innerException = newInnerException; }

		/// <summary>
		/// Sets the object which contains the inner exception (the object can be empty)
		/// </summary>
		/// <param name="newInnerException">The object which contains a new exception</param>
		ForceInline void SetInnerException(ExceptionHolderType&& newInnerException) { innerException = MoveRValue(newInnerException); }

		/// <summary>
		/// Sets the object which contains the inner exception (the object can be empty)
		/// </summary>
		/// <param name="newInnerException">The pointer to an inner exception</param>
		ForceInline void SetInnerException(Exception*&& newInnerException) { innerException = ExceptionHolderType(MoveRValue(newInnerException)); }

		/// <summary>
		/// Custom exception name
		/// </summary>
		ConstString Property(GetExceptioName) Name;

		/// <summary>Exception generation location</summary>
		ExeptionLocation Property(GetLocation) Location;

		/// <summary>
		/// The call stack during exception generation
		/// </summary>
		StackTraceRecord Property(GetStackTrace) StackTrace;

		/// <summary>
		/// Inner exception (if exists)
		/// </summary>
		ExceptionHolderType Property(GetInnerException, SetInnerException) InnerException;

		/// <summary>
		/// Text representation of exception information
		/// </summary>
		virtual ExceptionString toString() const {
			return ExceptionString::Combine({
				ExceptionString(L"Unknown error with message "), ExceptionString(exceptionName), ExceptionString(L". "),
				location.toString(),
				(innerException == nullptr) ? ExceptionString(L".") : (L". Also there is a nested exception: " + InnerException->toString())
				});
		}

		/// <summary>
		/// Destructor which is declared for right inherited object destroying
		/// </summary>
		virtual ~Exception() {}

	public:
		/// <summary>
		/// Raw exception
		/// </summary>
		class RawException final : IDisposable {
		private:
			struct Data : Memory::Allocator<Memory::ExceptionHeap> {
				void* ptr;
				RefCountT RefCount;

				Data(void* ptr) : ptr(ptr), RefCount(1) {}
			};

		private:
			Data* This;

		private:
			RawException(void* ptr) : This(new Data(ptr)) {};

		protected:
			virtual void Dispose() override final;

		public:
			/// <summary>
			/// An empty constructor
			/// </summary>
			RawException() :This(new Data(nullptr)) {};

			/// <summary>
			/// Generates saved exception
			/// </summary>
			__declspec(noreturn) void ThrowThis() {
				ThrowRawException(*this);
			}

			RefCountClass(RawException, This);

		public:
			friend Exception;
		};

	public:
		/// <summary>
		/// Saves the current exception object
		/// </summary>
		static RawException StoreCurrentException();

		/// <summary>
		/// Generates saved exception
		/// </summary>
		/// <param name="exception">Saved exception</param>
		__declspec(noreturn) static void ThrowRawException(RawException& exception);

	public:
		/// <summary>
		/// Clones the exception
		/// </summary>
		virtual ExceptionHolderType Clone() const override {
			return ExceptionHolderType(new Exception(*this));
		}
	};

	class InteruptException;
}

export namespace Cardinal::Concepts
{
	/// <summary>
	/// Checking if given type is an exception type (Rem::Core::Exception or derived from it class)
	/// </summary>
	template<typename T>
	concept IsException = IsSame<T, Exception> || IsBasedOn<Exception, T> || IsSame<T, InteruptException>;
}

#undef throw

/// <summary>
/// Returns the type or function/method result type
/// <\summary>
export template<typename Type>
using __ThrowableObj = typename Cardinal::TypeTraits::ConditionT <
	Cardinal::TypeTraits::IsFunction<Type>::State,
	typename Cardinal::TypeTraits::ResultOfT<Type>::Type,
	Type
>::Result;

/// <summary>
/// Returns "<c>True</c>" if this object is an eception or returns the exception object as a result
/// </summary>
export template<typename Type>
struct __IsThrowable :
	Cardinal::TypeTraits::IsBasedOn<
	Cardinal::Exception,
	Cardinal::TypeTraits::RemoveRefV<__ThrowableObj<Type>>
	> {
};

/// <summary>
/// Internal exception throwing function
/// </summary>
/// <param name = 'ex'>The exception object</param>
export template<typename E> InternalApiCall ForceInline void __declspec(noreturn) ___cxxthrow(E&& ex) { throw Cardinal::ForwardReference<E&&>(ex); }

/// <summary>
/// Internal exception rethrowing function
/// </summary>
export InternalApiCall ForceInline void __declspec(noreturn) __cxxrethrow() { throw; } //-V667

#pragma warning(pop)