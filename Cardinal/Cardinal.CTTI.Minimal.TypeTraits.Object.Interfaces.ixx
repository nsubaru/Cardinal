export module Cardinal.CTTI.Minimal:TypeTraits.Object.Interfaces;

export import :TypeTraits.Object.Base;

export namespace Cardinal::CTTI::TypeTraits
{
	template<typename TClass, typename TInterface>
	struct InterfaceRecord {
		static_assert(IsBasedOnV<TInterface, TClass>);

		using Interface = TInterface;
		using Class = TClass;
	};

	template<typename TClass, typename ... TInterfaces>
	constexpr auto BuildInterfaceRecords() {
		return Containers::Tuple{ InterfaceRecord<TClass, TInterfaces>{}... };
	}
}
