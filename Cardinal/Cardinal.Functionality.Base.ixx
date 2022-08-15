export module Cardinal.Functionality:Base;

import Cardinal.Core;
import Cardinal.Exceptions;

export namespace Cardinal::Functionality
{
	template<typename T> requires (!Concepts::IsSame<T, ExceptionHolderType>)
		class Result;

	template<typename T>
	struct IsResultT : TypeTraits::LogicFalse {
		using Type = Result<T>;
	};

	template<typename T>
	struct IsResultT<Result<T>> : TypeTraits::LogicTrue {
		using Type = Result<T>;
	};

	template<typename T>
	constexpr bool IsResultV = IsResultT<T>::State;
}