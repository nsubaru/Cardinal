export module Cardinal.CTTI.Minimal:TypeTraits.Object.RemoteDefinition;

export import :TypeTraits.Object.Base;
export import :TypeTraits.Object.Method;
export import :TypeTraits.Object.Property;
export import :TypeTraits.Object.ClassRecord;

export namespace Cardinal::CTTI::TypeTraits
{
	template<typename T> requires (Concepts::IsTypeHaveRemoteObjectCttiShema<T> && !Concepts::IsCustomCttiType<T>)
		struct CTTITypeDataSpecialization<T> : BaseCTTITypeDataImpl<
		T,
		Cardinal::TypeTraits::IsAbstract<T> ? EType::Abstract : EType::Object,
		false,
		Cardinal::TypeTraits::IsConstV<T>,
		Cardinal::TypeTraits::IsVolatileV<T>
		> {
		static constexpr bool IsTypeHaveClassCttiRecord = true;
		using ClassRecord = decltype(ObjectCttiSchema<T>::__identifier(class) ());
	};

	template<>
	struct ObjectCttiSchema<IDisposable> {
		CttiClassRecord(IDisposable,
			NoCttiInterfaces(),
			NoCttiConstructors(IDisposable),
			CttiMethodGroup(),
			CttiProperties()
		);
	};

	template<typename T>
	struct ObjectCttiSchema<IClonable<T>> {
		CttiClassRecord(IClonable<T>,
			NoCttiInterfaces(),
			NoCttiConstructors(IClonable<T>),
			CttiMethodGroup(CttiMethodRecord(IClonable<T>, Clone)),
			CttiProperties()
		);
	};
}