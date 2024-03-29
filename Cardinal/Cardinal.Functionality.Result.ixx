export module Cardinal.Functionality:Result;

import :Base;
import :UnionType;
import :Either;

export namespace Cardinal::Functionality
{
	/// <summary>
	/// Saves the result or exception
	/// </summary>
	/// <typeparam name="T">The result type</typeparam>
	template<typename T> requires (!Concepts::IsSame<T, ExceptionHolderType>)
	class Result
	{
	private:
		Either<ExceptionHolderType, T> data;
	
	public:
		/// <summary>
		/// Constructor with the reference to value
		/// </summary>
		/// <param name="value">Constant reference on result</param>
		/// <typeparam name="T">The result type</typeparam>
		ForceInline Result(const T& value) : data(value) {}

		/// <summary>
		/// "RValue" constructor with the reference to value
		/// </summary>
		/// <param name="value">"RValue" reference to value</param>
		/// <typeparam name="T">The result type</typeparam>
		ForceInline Result(T&& value) : data(MoveRValue(value)) {}

		/// <summary>
		/// "RValue" constructor with the pointer to exception
		/// </summary>
		/// <param name="ex">Constant reference to an exception</param>
		ForceInline Result(const ExceptionHolderType& ex) : data(ex) {}

		/// <summary>
		/// Constructor with the pointer to exception
		/// </summary>
		/// <param name="ex">"RValue" reference to exception</param>
		ForceInline Result(ExceptionHolderType&& ex) : data(MoveRValue(ex)) {}

		/// <summary>
		/// Constructor with the reference to exception
		/// </summary>
		/// <param name="ex">"RValue" reference to exception</param>
		ForceInline Result(const Exception& ex) : data(ex.Clone()) {}

		/// <summary>
		/// Constructor with the reference to exception
		/// </summary>
		/// <param name="ex">"RValue" reference to exception</param>
		ForceInline Result(Exception&& ex) : data(ex.Clone()) {}
	
	public:
		/// <summary>
		/// Indicates whether the object contains an exception
		/// </summary>
		/// <returns>The logical value</returns>
		bool GetIsFail() const { return data.HoldedTypeNumber == 0; }

		/// <summary>
		/// Indicates whether the object contains a value
		/// </summary>
		/// <returns>The logical value</returns>
		bool GetIsSuccess() const { return data.HoldedTypeNumber == 1; }
	
	public:
		/// <summary>
		/// Indicates whether the object contains an exception
		/// </summary>
		bool Property(GetIsFail) IsFail;

		/// <summary>
		/// Indicates whether the object contains a value
		/// </summary>
		bool Property(GetIsSuccess) IsSuccess;
	
	public:
		/// <summary>
		/// Executes the given lambda or functor (if the result is successful).
		/// If the functor throws an exception, then the exception will be catched
		/// into exception result;
		/// If functor returns void, the the result will have
		/// "TypeTraits::TypeTag<void>" type;
		/// If functor returns the "Result<T>", then this type will be returned, but
		/// not a "Result<Result<T>>" type.
		/// </summary>
		/// <typeparam name="TLambda">Lambda or functor type</typeparam>
		/// <param name="lambda">Lambda or functor object</param>
		/// <returns>New object "Result"</returns>
		template<typename TLambda>
		requires (Concepts::IsInvockable<TLambda, T>)
		NoDiscard ForceInline decltype(auto) OnSuccess(TLambda&& lambda) &&
		{
			using U = TypeTraits::ConditionV<
				TypeTraits::IsSameV<TypeTraits::InvockableResult<TLambda, T>, void>,
				TypeTraits::TypeTag<void>,
				TypeTraits::InvockableResult<TLambda, T>>;

			using Type = typename TypeTraits::IdentitV<typename IsResultT<U>::Type>;
			
			try
			{
				if (IsResultV<TypeTraits::InvockableResult<TLambda, T>>)
				{
					if (data.HoldedTypeNumber == 0)
					{
						return Type(MoveRValue(data).template GetAs<ExceptionHolderType>());
					}
					else
					{
						if constexpr (TypeTraits::IsSameV<TypeTraits::InvockableResult<TLambda, T>, void>)
						{
							lambda(MoveRValue(data).template GetAs<T>());
							return Type(U());
						}
						else
						{
							return Type(lambda(MoveRValue(data).template GetAs<T>()));
						}
					}
				}
				else
				{
					if (data.HoldedTypeNumber == 0)
					{
						return Type(MoveRValue(data).template GetAs<ExceptionHolderType>());
					}
					else
					{
						if constexpr (TypeTraits::IsSameV<TypeTraits::InvockableResult<TLambda, T>, void>)
						{
							lambda(MoveRValue(data).template GetAs<T>());
							return Type(U());
						}
						else
						{
							return Type(lambda(MoveRValue(data).template GetAs<T>()));
						}
					}
				}
			}
			catch (Exception& ex)
			{
				return Type(ex);
			}
		}

		/// <summary>
		/// Execute ifTrue or ifFalse depending on condition and return Either which contain one of results
		/// </summary>
		/// <typeparam name="TConditionLambda">Type of condition lambda</typeparam>
		/// <typeparam name="TIfTrue">Type of lambda which execute if condition lambda return true</typeparam>
		/// <typeparam name="TIfFalse">Type of lambda which execute if condition lambda return false</typeparam>
		/// <param name="condition">Condition lambda</param>
		/// <param name="ifTrue">Lambda which execute if condition lambda return true</param>
		/// <param name="ifFalse">Lambda which execute if condition lambda return false</param>
		/// <returns>Either object which contain result of one of lambdas</returns>
		template<typename TConditionLambda, typename TIfTrue, typename TIfFalse>
		requires (Concepts::IsInvockableReturnSameAs<TConditionLambda, bool, const T&> || Concepts::IsInvockable<TIfTrue, T> || Concepts::IsInvockable<TIfFalse, T>)
		auto Match(TConditionLambda&& condition, TIfTrue&& ifTrue, TIfFalse&& ifFalse) &&
			-> Either<decltype(MoveRValue(*this).OnSuccess(MoveRValue(ifTrue))), decltype(MoveRValue(*this).OnSuccess(MoveRValue(ifFalse)))>
		{
			if (condition(data.template GetAs<T>()))
			{
				return MoveRValue(*this).OnSuccess(MoveRValue(ifTrue));
			}
			else
			{
				return MoveRValue(*this).OnSuccess(MoveRValue(ifFalse));
			}
		}
	
	public:
		/// <summary>
		/// Executes the lambda or functor (if object contains an exception and transmits
		/// the exception object inside it, regardless of the object state returns it)
		/// </summary>
		/// <typeparam name="TLambda">Lambda or functor type</typeparam>
		/// <param name="lambda">Lambda or functor object</param>
		/// <returns>Current object</returns>
		template<typename TLambda>
		requires (
			Concepts::IsInvockable<TLambda, ExceptionHolderType> || 
			Concepts::IsInvockable<TLambda, ExceptionHolderType&> ||
			Concepts::IsInvockable<TLambda, const ExceptionHolderType&>)
		NoDiscard ForceInline Result&& OnFail(TLambda&& lambda) &&
		{
			if (data.HoldedTypeNumber == 1)
			{
				return MoveRValue(*this);
			}
			else
			{
				lambda(data.template GetAs<ExceptionHolderType>());
				return MoveRValue(*this);
			}
		}

		/// <summary>
		/// Finalizing method, that does nothing
		/// </summary>
		void DoNothing()&& {};

		/// <summary>
		/// Executes the lambda or functor (if object contains an exception and transmits
		/// the exception object inside it, regardless of the object state returns it)
		/// </summary>
		/// <typeparam name="TLambda">Lambda or functor type</typeparam>
		/// <param name="lambda">Lambda or functor object</param>
		/// <returns>Current object</returns>
		template<typename TLambda>
		requires Concepts::IsInvockable<TLambda>
		NoDiscard ForceInline Result&& OnFail(TLambda&& lambda)&& {
			if (data.HoldedTypeNumber == 1)
			{
				return MoveRValue(*this);
			}
			else
			{
				lambda();
				return MoveRValue(*this);
			}
		}

		/// <summary>
		/// Throw captured exception if they exist
		/// </summary>
		ForceInline Result&& ThrowOnFail()&& {
			if (data.HoldedTypeNumber != 1)
			{
				Throw(*data.template GetAs<ExceptionHolderType>());
			}
			else
			{
				return MoveRValue(*this);
			}
		}
	
	public:
		/// <summary>
		/// Copying constructor
		/// </summary>
		ForceInline Result(const Result& from) : data(from.data) {}

		/// <summary>
		/// Moving constructor
		/// </summary>
		ForceInline Result(Result&& from) : data(MoveRValue(from.data)) {}

		/// <summary>
		/// Copying operator
		/// </summary>
		/// <param name="from">Original</param>
		/// <returns>A reference to a current object</returns>
		ForceInline Result& operator=(const Result& from)
		{
			this->~Result();
			this->Result::Result(from);
			return *this;
		}

		/// <summary>
		/// Moving operator
		/// </summary>
		/// <param name="from">Original</param>
		/// <returns>A reference to a current object</returns>
		ForceInline Result& operator=(Result&& from) {
			this->~Result();
			this->Result::Result(MoveRValue(from));
			return *this;
		}
	
	public:
		/// <summary>
		/// Returns the successful result or generates the exception if no result
		/// </summary>
		/// <returns>The reference to result</returns>
		NoDiscard ForceInline const T& GetSuccessResult() const &
		{
			return data.template GetAs<T>();
		}

		/// <summary>
		/// Returns the successful result or generates the exception if no result
		/// </summary>
		/// <returns>The reference to result</returns>
		NoDiscard ForceInline const T& GetSuccessResult() &
		{
			return data.template GetAs<T>();
		}

		/// <summary>
		/// Returns the successful result or generates the exception if no result
		/// </summary>
		/// <returns>RValue reference on result</returns>
		NoDiscard ForceInline T GetSuccessResult() && {
			return MoveRValue(MoveRValue(data).template GetAs<T>());
		}
	
	public:
		/// <summary>
		/// Returns the reference to exception or generates the exception if no exception
		/// </summary>
		/// <returns>The reference to exception</returns>
		NoDiscard ForceInline const ExceptionHolderType& GetFailResult() const& {
			return data.template GetAs<ExceptionHolderType>();
		}

		/// <summary>
		/// Returns the reference to exception or generates the exception if no exception
		/// </summary>
		/// <returns>The reference to exception</returns>
		NoDiscard ForceInline const ExceptionHolderType& GetFailResult()& {
			return data.template GetAs<ExceptionHolderType>();
		}

		/// <summary>
		/// Returns the reference to exception or generates the exception if no exception
		/// </summary>
		/// <returns>"RValue" reference to an exception</returns>
		NoDiscard ForceInline ExceptionHolderType GetFailResult()&& {
			return MoveRValue(MoveRValue(data).template GetAs<ExceptionHolderType>());
		}
	};

	/// <summary>
	/// Generates the successful result
	/// </summary>
	/// <typeparam name="T">The result type</typeparam>
	/// <param name="data">The reference to result</param>
	/// <returns>The result object</returns>
	template<typename T>
	NoDiscard ForceInline Result<T> SuccessResult(const T& data) { return Result<T>(data); }

	/// <summary>
	/// Generates the successful result
	/// </summary>
	/// <typeparam name="T">The result type</typeparam>
	/// <param name="data">"RValue" reference to the result</param>
	/// <returns>The result object</returns>
	template<typename T>
	NoDiscard ForceInline Result<T> SuccessResult(T&& data) { return Result<T>(MoveRValue(data));  }

	/// <summary>
	/// Generates the non successful result
	/// </summary>
	/// <typeparam name="T">The result type</typeparam>
	/// <param name="ex">The reference to exception</param>
	/// <returns>The result object</returns>
	template<typename T>
	NoDiscard ForceInline Result<T> FailResult(const ExceptionHolderType& ex) { return Result<T>(ex); }

	/// <summary>
	/// Generates the non successful result
	/// </summary>
	/// <typeparam name="T">The result type</typeparam>
	/// <param name="ex">"RValue" reference to an exception</param>
	/// <returns>The result object</returns>
	template<typename T>
	NoDiscard ForceInline Result<T> FailResilt(ExceptionHolderType&& ex) { return Result<T>(MoveRValue(ex));  }
}