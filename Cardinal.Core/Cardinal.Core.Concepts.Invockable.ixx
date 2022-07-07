export module Cardinal.Core:Concepts.Invockable;

import :Concepts.Base;

export namespace Cardinal::Core::Concepts
{
	/// <summary>
	/// Checking if "TCallable" type can be called with "TArgs" list of arguments
	/// </summary>
	template<typename TCallable, typename ... TArgs>
	concept IsInvockable = TypeTraits::IsInvockable<TCallable, TArgs...>;

	/// <summary>
	///  Checking if template "TLambda" lambda can be called with "TArgs" 
	///  list of arguments and specific template type "TTemplate"
	/// </summary>
	template<typename TLambda, typename TTemplate, typename ... TArgs>
	concept IsInvockableTemplateLambda = requires(TLambda && lambda, TArgs&&... args) {
		{ lambda.template operator()<TTemplate>(Core::ForwardReference<TArgs>(args)...) };
	};

	/// <summary>
	///  Checking if template "TLambda" lambda can be called with "TArgs" 
	///  list of arguments and specific template type "TTemplate" with SizeT template as Index
	/// </summary>
	template<typename TLambda, typename TTemplate, SizeT TIdx, typename ... TArgs>
	concept IsInvockableTemplateLambdaWithTemplateIndex = requires(TLambda && lambda, TArgs&&... args) {
		{ lambda.template operator()<TTemplate, TIdx>(Core::ForwardReference<TArgs>(args)...) };
	};

	/// <summary>
	/// Checking if "TCallable" type can be called with "TArgs" list of arguments
	/// and returns given type "TRet"
	/// </summary>
	template<typename TCallable, typename TRet, typename ... TArgs>
	concept IsInvockableWithResult = TypeTraits::IsInvockableWithResult<TCallable, TRet, TArgs...>;

	/// <summary>
	/// Checking if "TCallable" type can be called with "TArgs" list of arguments
	/// and returns given type "TRet"
	/// </summary>
	template<typename TCallable, typename TRet, typename ... TArgs>
	concept IsInvockableReturnSameAs = TypeTraits::IsSameV<TypeTraits::InvockableResult<TCallable, TArgs...>, TRet>;

	/// <summary>
	/// Checking if "TCallable" type can be called with "TArgs" list of arguments
	/// and returns void
	/// </summary>
	template<typename TCallable, typename ... TArgs>
	concept IsInvockableReturnVoid = TypeTraits::IsVoidV<TypeTraits::InvockableResult<TCallable, TArgs...>>;
}