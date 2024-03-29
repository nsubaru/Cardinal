export module Cardinal.CTTI.Minimal:TypeTraits.Object;

export import :TypeTraits.Object.Base;
export import :TypeTraits.Object.Method;
export import :TypeTraits.Object.Property;
export import :TypeTraits.Object.ClassRecord;
export import :TypeTraits.Object.RemoteDefinition;

export namespace Cardinal::CTTI::TypeTraits
{
	template<typename T> requires (Concepts::IsTypeHaveClassCttiRecord<T> && !Concepts::IsTypeHaveRemoteObjectCttiShema<T> && !Concepts::IsCustomCttiType<T>)
		struct CTTITypeDataSpecialization<T> : BaseCTTITypeDataImpl<
		T,
		Cardinal::TypeTraits::IsAbstract<T> ? EType::Abstract : EType::Object,
		false,
		Cardinal::TypeTraits::IsConstV<T>,
		Cardinal::TypeTraits::IsVolatileV<T>
		> {
		static constexpr bool IsTypeHaveClassCttiRecord = true;
		using ClassRecord = decltype(T::__identifier(class) ());
	};
}