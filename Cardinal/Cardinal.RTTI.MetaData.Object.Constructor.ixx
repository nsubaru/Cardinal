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

#pragma pack(push, 1)
	template<typename TArgs>
	struct CtConstructMetaDataLastRecord {
		SizeT OffsetToNext;
		TGetRttiRecord GetRttiRecord;
		TDynamicCtorCaller DynamicCtorCaller;
		SizeT OffsetToArgs;
		decltype(AsRttiCollection<TArgs::Count>(GetMethodArgs<TArgs>())) Args;
	};

	template<typename TArgs, typename TRest>
	struct CtConstructMetaDataRecord {
		SizeT OffsetToNext;
		TGetRttiRecord GetRttiRecord;
		TDynamicCtorCaller DynamicCtorCaller;
		SizeT OffsetToArgs;
		decltype(AsRttiCollection<TArgs::Count>(GetMethodArgs<TArgs>())) Args;
		TRest Rest;
	};
#pragma pack(pop)

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

			using tCtConstructMetaDataRecord = CtConstructMetaDataLastRecord<typename T::Args>;

			return tCtConstructMetaDataRecord{
				.OffsetToNext = sizeof(tCtConstructMetaDataRecord),
				.GetRttiRecord = &GetRttiRecordGetter<typename T::Class>::Get,
				.DynamicCtorCaller = &CtorCallerGeneratorT<T>::DynamicCall,
				.OffsetToArgs = StructOffsetOfMember(tCtConstructMetaDataRecord, Args),
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
			using TRest = decltype(GetConstructors<TConstructors, TIdx + 1>());

			using tCtConstructMetaDataRecord = CtConstructMetaDataRecord<typename T::Args, TRest>;

			return tCtConstructMetaDataRecord{
				.OffsetToNext = StructOffsetOfMember(tCtConstructMetaDataRecord, Rest),
				.GetRttiRecord = &GetRttiRecordGetter<typename T::Class>::Get,
				.DynamicCtorCaller = &CtorCallerGeneratorT<T>::DynamicCall,
				.OffsetToArgs = StructOffsetOfMember(tCtConstructMetaDataRecord, Args),
				.Args = AsRttiCollection<T::Args::Count>(GetMethodArgs<typename T::Args>()),
				.Rest = GetConstructors<TConstructors, TIdx + 1>()
			};
		}
	}
}