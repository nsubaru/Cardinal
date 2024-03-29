export module Cardinal.CTTI.Minimal:TypeTraits.Object.SpecialMethods;

export import :TypeTraits.Object.Method;

export namespace Cardinal::CTTI::TypeTraits
{
	template<typename TClass, typename ... TArgs>
	struct ConstructorInstanceRecord {
		constexpr static auto Construct = [] (TArgs&&... args) constexpr { return TClass(ForwardReference<TArgs>(args)...); };

		using Class = TClass;

		using Args = typename Cardinal::Containers::Tuple<OriginalTypeT<TArgs>...>;
	};

	template<typename Class>
	struct DestructorRecord {
		constexpr static auto Destructor = [] (Class&& obj) constexpr { obj.~Class(); };
	};

	template<typename Class, typename TConstructors>
	struct ConstructorsRecord {
		using Constructors = TConstructors;
		using Destructor = DestructorRecord<Class>;
	};
}
