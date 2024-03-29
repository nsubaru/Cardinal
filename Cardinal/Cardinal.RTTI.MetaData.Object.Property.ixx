export module Cardinal.RTTI:MetaData.Object.Property;

export import :MetaData.Object.Base;
export import :MetaData.Object.Method;

export namespace Cardinal::RTTI::MetaData
{
	/// <summary>
	/// Get the property from RTTI record
	/// </summary>
	/// <typeparam name="TProperty">Property record type</typeparam>
	template<typename TProperty>
	struct PropertyGetterT {
		/// <summary>
		/// Property getter
		/// </summary>
		/// <param name="src">Source</param>
		/// <param name="dest">Destination</param>
		static void Get(void* src, void* dest) {
			if constexpr (TProperty::Getters::Count > 0)
			{
				using GetterMethodRecord = typename TProperty::Getters::template ItemType<0>;
				using TClass = typename GetterMethodRecord::TargetClass;
				using T = typename TProperty::T::Type;

				constexpr auto ptr = GetterMethodRecord::RawMethodPtr;

				if constexpr (Cardinal::TypeTraits::IsAbstract<TClass>)
				{
					auto obj = &(**(SmartPtr<TClass>*) src);
					new (dest) T((obj->*ptr)());
				}
				else
				{
					auto obj = (TClass*) src;
					new (dest) T((obj->*ptr)());
				}
			}
			else
			{
				Throw(NotImplementedException(SourcePosition));
			}
		}
	};

	/// <summary>
	/// Get property type metadata from record
	/// </summary>
	/// <typeparam name="TProperties">Property record type</typeparam>
	/// <returns>Property type metadata</returns>
	template<typename TProperties, SizeT TIdx = 0>
	inline constexpr auto GetProperties() {
		if constexpr (TProperties::Count == TIdx)
		{
			return TypeTraits::TypeTag<void>{};
		}
		else
		{
			using TProperty = typename TProperties::template ItemType<TIdx>;
			using Type = typename TProperty::T::Type;
#pragma pack(push, 1)
			struct PropertyTypeMetaData {
				SizeT OffsetToNext;
				SizeT OffsetToGetters;
				SizeT OffsetToSetters;
				TUnsafeGet UnsafeGet;
				decltype(GetCTTIPropItemMetaData<Type>(TProperty::Name)) Type;
				decltype(AsRttiCollection<TProperty::Getters::Count>(GetCttiMethods<typename TProperty::Getters>())) Geters;
				decltype(AsRttiCollection<TProperty::Setters::Count>(GetCttiMethods<typename TProperty::Setters>())) Seters;
				decltype(GetProperties<TProperties, TIdx + 1>()) Rest;
			};
#pragma pack(pop)
			return PropertyTypeMetaData{
				.OffsetToNext = StructOffsetOfMember(PropertyTypeMetaData, Rest),
				.OffsetToGetters = StructOffsetOfMember(PropertyTypeMetaData, Geters),
				.OffsetToSetters = StructOffsetOfMember(PropertyTypeMetaData, Seters),
				.UnsafeGet = &PropertyGetterT<TProperty>::Get,
				.Type = GetCTTIPropItemMetaData<typename TProperty::T::Type>(TProperty::Name),
				.Geters = AsRttiCollection<TProperty::Getters::Count>(GetCttiMethods<typename TProperty::Getters>()),
				.Seters = AsRttiCollection<TProperty::Setters::Count>(GetCttiMethods<typename TProperty::Setters>()),
				.Rest = GetProperties<TProperties, TIdx + 1>()
			};
		}
	}
}