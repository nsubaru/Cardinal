export module Cardinal.RTTI:MetaData.Object.Interface;

export import :MetaData.Object.Base;

export namespace Cardinal::RTTI::MetaData
{
	/// <summary>
	/// 
	/// </summary>
	/// <typeparam name="TInterface"></typeparam>
	template<typename TInterface>
	struct ConverterT {
		using CompleteTypeInterface = typename Cardinal::CTTI::TypeTraits::CompletedTypeT<typename TInterface::Interface>::Type;
		using CompleteTypeClass = typename Cardinal::CTTI::TypeTraits::CompletedTypeT<typename TInterface::Class>::Type;

		using Interface = typename TInterface::Interface;
		using Class = typename TInterface::Class;

		/// <summary>
		/// An interface record conversion
		/// </summary>
		/// <param name="interfaceRecord">Interface record</param>
		/// <param name="from">A dynamic to which is need to convert</param>
		/// <returns>Conversion result</returns>
		static Dynamic ConvertTo(Record::RttiIntefaceRecord interfaceRecord, Dynamic& from) {
			if constexpr (Concepts::IsSame<CompleteTypeClass, Class>)
			{
				if (from.Type != Record::GetRttiTypeRecord<Class>())
				{
					Throw(Exceptions::DynamicInterfaceCastInvalidTargetException(SourcePosition, interfaceRecord, true, Record::GetRttiTypeRecord<Class>(), from.Type));
				}

				return Dynamic(CompleteTypeInterface{ new Class(from.GetValue<Class>()) });
			}
			else
			{
				Throw(Exceptions::DynamicInterfaceCastInvalidTargetException(SourcePosition, interfaceRecord, true, Record::GetRttiTypeRecord<Class>(), from.Type));
			}
		}

		/// <summary>
		/// An interface record conversion
		/// </summary>
		/// <param name="interfaceRecord">Interface record</param>
		/// <param name="from">A dynamic which is need to convert</param>
		/// <returns>Conversion result</returns>
		static Dynamic ConvertFrom(Record::RttiIntefaceRecord interfaceRecord, Dynamic& from) {
			if constexpr (Concepts::IsSame<CompleteTypeClass, Class>)
			{
				if (from.Type != Record::GetRttiTypeRecord<CompleteTypeInterface>())
				{
					Throw(Exceptions::DynamicInterfaceCastInvalidTargetException(SourcePosition, interfaceRecord, false, Record::GetRttiTypeRecord<CompleteTypeInterface>(), from.Type));
				}

				return Dynamic(Class(from.GetValue<CompleteTypeInterface>().As<Class>()));
			}
			else
			{
				Throw(Exceptions::DynamicInterfaceCastInvalidTargetException(SourcePosition, interfaceRecord, false, Record::GetRttiTypeRecord<CompleteTypeInterface>(), from.Type));
			}
		}
	};

#pragma pack(push, 1)
	template<SizeT TypeNameSz>
	struct CtInterfaceMetaDataLast {
		SizeT OffsetToNext;
		UniversalTypeIdentificator TypeId;
		MetaDataForDynamicRecord MetaDataForDynamic;
		DynamicConverterFunc DynamicConverterTo;
		DynamicConverterFunc DynamicConverterFrom;
		CttiString<TypeNameSz> TypeName;
	};
#pragma pack(pop)

	/// <summary>
	/// Get the interface from RTTI record
	/// </summary>
	/// <typeparam name="TInterface">Interface type</typeparam>
	/// <returns>Structure with information about interface in RTTI record</returns>
	template<typename TInterface, SizeT TIdx = 0>
	inline constexpr auto GetInterface() {
		if constexpr (TInterface::Count - 1 == TIdx)
		{
			using T = typename TInterface::template ItemType<TIdx>;

			return CtInterfaceMetaDataLast<Cardinal::CTTI::TypeName<typename T::Interface>().Count>{
				.OffsetToNext = sizeof(PropertyRecord),
				.TypeId = GetUniversalTypeIdentificatorByType<T>(),
				.MetaDataForDynamic = GetFieldMetaDataRecordForType<typename T::Interface>(),
				.DynamicConverterTo = ConverterT<T>::ConvertTo,
				.DynamicConverterFrom = ConverterT<T>::ConvertFrom,
				.TypeName = GetTextRecord(Cardinal::CTTI::TypeName<typename T::Interface>())
			};
		}
		else if constexpr (TInterface::Count == TIdx)
		{
			return TypeTraits::TypeTag<void>{};
		}
		else
		{
			using T = typename TInterface::template ItemType<TIdx>;
#pragma pack(push, 1)
			struct InterfaceMetaData {
				SizeT OffsetToNext;
				UniversalTypeIdentificator TypeId;
				MetaDataForDynamicRecord MetaDataForDynamic;
				DynamicConverterFunc DynamicConverterTo;
				DynamicConverterFunc DynamicConverterFrom;
				decltype(GetTextRecord(Cardinal::CTTI::TypeName<typename T::Interface>())) TypeName;
				decltype(GetInterface<TInterface, TIdx + 1>()) Rest;
			};
#pragma pack(pop)
			return InterfaceMetaData{
				.OffsetToNext = StructOffsetOfMember(InterfaceMetaData, Rest),
				.TypeId = GetUniversalTypeIdentificatorByType<T>(),
				.MetaDataForDynamic = GetFieldMetaDataRecordForType<typename T::Interface>(),
				.DynamicConverterTo = ConverterT<T>::ConvertTo,
				.DynamicConverterFrom = ConverterT<T>::ConvertFrom,
				.TypeName = GetTextRecord(Cardinal::CTTI::TypeName<typename T::Interface>()),
				.Rest = GetInterface<TInterface, TIdx + 1>()
			};
		}
	}
}