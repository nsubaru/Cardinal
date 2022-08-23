export module Cardinal.RTTI:MetaData.Object.Constructor;

export import :MetaData.Object.Base;

export namespace Cardinal::RTTI::MetaData
{
	template<typename TCtorRecord>
	struct CtorCallerGeneratorT {
		template<SizeT TIdx>
		static auto GetTypeFromDynamicArrayByIndex(Record::RttiConstructorRecord targetCtorDef, Containers::Array<Dynamic>& args) {
			using ArgType = typename TCtorRecord::Args::template ItemType<TIdx>::Type;

			if (args[TIdx].Type != Record::GetRttiTypeRecord<ArgType>())
			{
				Throw(Exceptions::DynamicCtorCallInvalidTargetException(
					SourcePosition,
					targetCtorDef,
					args[TIdx].Type,
					Record::GetRttiTypeRecord<ArgType>(),
					TIdx));
			}

			return ArgType(args[TIdx].GetValue<ArgType>());
		}

		template<typename>
		struct CtorCallerGeneratorImpl;

		template<SizeT ... TIdx>
		struct CtorCallerGeneratorImpl<TypeTraits::Indexator<TIdx...>> {
			static ForceInline Dynamic DynamicCallImpl(Record::RttiConstructorRecord targetCtorDef, Containers::Array<Dynamic>& args) {
				using Target = typename TCtorRecord::Class;

				return Dynamic(Target(MoveRValue(
					GetTypeFromDynamicArrayByIndex<(TCtorRecord::Args::Count - 1) - TIdx>(targetCtorDef, args))...));
			}
		};

		static Dynamic DynamicCall(Record::RttiConstructorRecord targetCtorDef, Containers::Array<Dynamic>& args) {
			using Target = typename TCtorRecord::Class;

			if (TCtorRecord::Args::Count != args.Count)
			{
				Throw(Exceptions::DynamicCtorCallInvalidArgumentsCountException(SourcePosition, targetCtorDef, args.Count));
			}

			return CtorCallerGeneratorImpl<typename TypeTraits::Indexer<TCtorRecord::Args::Count>::Type>::DynamicCallImpl(targetCtorDef, args);
		}
	};

	/// <summary>
	/// Get the constructors from RTTI record
	/// </summary>
	/// <typeparam name="TConstructors">Constructor type</typeparam>
	/// <returns>Structure with information about constructors in RTTI record</returns>
	template<typename TConstructors, SizeT TIdx = 0>
	inline constexpr auto GetConstructors() {
		if constexpr (TConstructors::Count - 1 == TIdx)
		{
			using T = typename TConstructors::template ItemType<TIdx>;
#pragma pack(push, 1)
			struct PropertyMetaData {
				SizeT OffsetToNext;
				TGetRttiRecord GetRttiRecord;
				TDynamicCtorCaller DynamicCtorCaller;
				SizeT OffsetToArgs;
				decltype(AsRttiCollection<T::Args::Count>(GetMethodArgs<typename T::Args>())) Args;
			};
#pragma pack(pop)
			return PropertyMetaData{
				.OffsetToNext = sizeof(PropertyMetaData),
				.GetRttiRecord = &GetRttiRecordGetter<typename T::Class>::Get,
				.DynamicCtorCaller = &CtorCallerGeneratorT<T>::DynamicCall,
				.OffsetToArgs = StructOffsetOfMember(PropertyMetaData, Args),
				.Args = AsRttiCollection<T::Args::Count>(GetMethodArgs<typename T::Args>())
			};
		}
		else if constexpr (TConstructors::Count == TIdx)
		{
			return TypeTraits::TypeTag<void>{};
		}
		else
		{
			using T = typename TConstructors::template ItemType<TIdx>;
#pragma pack(push, 1)
			struct PropertyMetaData {
				SizeT OffsetToNext;
				TGetRttiRecord GetRttiRecord;
				TDynamicCtorCaller DynamicCtorCaller;
				SizeT OffsetToArgs;
				decltype(AsRttiCollection<T::Args::Count>(GetMethodArgs<typename T::Args>())) Args;
				decltype(GetConstructors<TConstructors, TIdx + 1>()) Rest;
			};
#pragma pack(pop)
			return PropertyMetaData{
				.OffsetToNext = StructOffsetOfMember(PropertyMetaData, Rest),
				.GetRttiRecord = &GetRttiRecordGetter<typename T::Class>::Get,
				.DynamicCtorCaller = &CtorCallerGeneratorT<T>::DynamicCall,
				.OffsetToArgs = StructOffsetOfMember(PropertyMetaData, Args),
				.Args = AsRttiCollection<T::Args::Count>(GetMethodArgs<typename T::Args>()),
				.Rest = GetConstructors<TConstructors, TIdx + 1>()
			};
		}
	}
}