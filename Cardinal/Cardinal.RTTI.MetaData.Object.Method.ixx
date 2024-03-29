export module Cardinal.RTTI:MetaData.Object.Method;

export import :MetaData.Object.Base;

export namespace Cardinal::RTTI::MetaData
{
	/// <summary>
	/// Method caller generator
	/// </summary>
	/// <typeparam name="TMethodRecord">Method record type</typeparam>
	template<typename TMethodRecord>
	struct MethodCallerGeneratorT {
		/// <summary>
		/// Receiving the type from dynamic array by index
		/// </summary>
		/// <param name="targetDef">Target definition</param>
		/// <param name="args">Array with arguments</param>
		/// <returns>Type from dynamic array</returns>
		template<SizeT TIdx>
		static auto GetTypeFromDynamicArrayByIndex(Record::RttiMethodRecord targetDef, Containers::Array<Dynamic>& args) {
			using ArgType = typename TMethodRecord::OriginalArgsTuple::template ItemType<TIdx>::Type;

			if (args[TIdx].Type != Record::GetRttiTypeRecord<ArgType>())
			{
				Throw(Exceptions::DynamicCallInvalidArgumentTypeException(
					SourcePosition,
					targetDef,
					args[TIdx].Type,
					Record::GetRttiTypeRecord<ArgType>(),
					TIdx));
			}

			return ArgType(args[TIdx].GetValue<ArgType>());
		}

		template<typename>
		struct MethodCallerGeneratorImpl;

		template<SizeT ... TIdx>
		struct MethodCallerGeneratorImpl<TypeTraits::Indexator<TIdx...>> {
			/// <summary>
			/// Dynamic call implementation
			/// </summary>
			/// <param name="target">Target</param>
			/// <param name="targetDef">Target definition</param>
			/// <param name="args">Arguments</param>
			/// <returns>Dynamic object</returns>
			static ForceInline Dynamic DynamicCallImpl(Dynamic target, Record::RttiMethodRecord targetDef, Containers::Array<Dynamic>& args) {
				using Target = typename TMethodRecord::OriginalClass::Type;

				if constexpr (Cardinal::TypeTraits::IsVoidV<typename TMethodRecord::OriginalRet::Type>)
				{
					TMethodRecord::DecayCall(
						MoveRValue(Target(target.GetValue<Target>())),
						MoveRValue(
							GetTypeFromDynamicArrayByIndex<(TMethodRecord::OriginalArgsTuple::Count - 1) - TIdx>(targetDef, args))...);

					return Dynamic();
				}
				else
				{
					return Dynamic(TMethodRecord::DecayCall(
						MoveRValue(Target(target.GetValue<Target>())),
						MoveRValue(
							GetTypeFromDynamicArrayByIndex<(TMethodRecord::OriginalArgsTuple::Count - 1) - TIdx>(targetDef, args))...));
				}
			}
		};

		/// <summary>
		/// Dynamic call function
		/// </summary>
		/// <param name="target">Target</param>
		/// <param name="targetDef">Target definition</param>
		/// <param name="args">Arguments</param>
		/// <returns>Dynamic object</returns>
		static Dynamic DynamicCall(Dynamic target, Record::RttiMethodRecord targetDef, Containers::Array<Dynamic>& args) {
			using Target = typename TMethodRecord::OriginalClass::Type;

			if (Record::GetRttiTypeRecord<Target>() != target.Type)
			{
				Throw(Exceptions::DynamicCallInvalidTargetException(SourcePosition, targetDef, Record::GetRttiTypeRecord<Target>(), target.Type));
			}

			if (TMethodRecord::OriginalArgsTuple::Count != args.Count)
			{
				Throw(Exceptions::DynamicCallInvalidArgumentsCountException(SourcePosition, targetDef, args.Count));
			}

			return MethodCallerGeneratorImpl<typename TypeTraits::Indexer<TMethodRecord::OriginalArgsTuple::Count>::Type>::DynamicCallImpl(target, targetDef, args);
		}
	};

	/// <summary>
	/// Receiving the method arguments
	/// </summary>
	/// <typeparam name="TTuple">Tuple nested item types</typeparam>
	/// <returns>Method arguments</returns>
	template<typename TTuple, SizeT TIdx = 0>
	inline constexpr auto GetMethodArgs() {
		if constexpr (TTuple::Count == TIdx)
		{
			return TypeTraits::TypeTag<void>{};
		}
		else
		{
			using T = typename TTuple::template ItemType<TIdx>;
			constexpr auto item = GetMethodTypeRecord<T>();
#pragma pack(push, 1)
			struct MetaDataField {
				SizeT OffsetToNext;
				decltype(item) Record;
				decltype(GetMethodArgs<TTuple, TIdx + 1>()) Rest;
			};
#pragma pack(pop)
			return MetaDataField{
				.OffsetToNext = StructOffsetOfMember(MetaDataField, Rest),
				.Record = item,
				.Rest = GetMethodArgs<TTuple, TIdx + 1>()
			};
		}
	}

	/// <summary>
	/// Receiving the method return type
	/// </summary>
	/// <typeparam name="T">Method type</typeparam>
	/// <returns>Method return type</returns>
	template<typename T>
	inline constexpr auto GetMethodRet() {
		constexpr auto item = GetMethodTypeRecord<T>();
#pragma pack(push, 1)
		struct MetaDataField {
			decltype(item) Record;
		};
#pragma pack(pop)
		return MetaDataField{
			.Record = item
		};
	}

	/// <summary>
	/// Receiving the collection of method arguments
	/// </summary>
	/// <typeparam name="Method">Given method</typeparam>
	/// <returns>Collection of method arguments</returns>
	template<typename Method>
	inline constexpr auto GetMethodArgsCollection() {
#pragma pack(push, 1)
		struct MethodArgsCollection {
			SizeT OffsetToNext;
			decltype(GetMethodRet<typename Method::OriginalRet>()) Ret;
			decltype(GetMethodArgs<typename Method::OriginalArgsTuple>()) Args;
		};
#pragma pack(pop)
		return MethodArgsCollection{
			.OffsetToNext = StructOffsetOfMember(MethodArgsCollection, Args),
			.Ret = GetMethodRet<typename Method::OriginalRet>(),
			.Args = GetMethodArgs<typename Method::OriginalArgsTuple>()
		};
	}

#pragma pack(push, 1)
	template<typename Method>
	struct CtMethodMetadata {
		SizeT MethodArgsOffset;
		SizeT Flags;
		DynamicCallerFunc DynamicCaller;
		decltype(GetTextRecord(Method::MethodName)) MethodName;
		decltype(AsRttiCollection<1 + Method::OriginalArgsTuple::Count>(GetMethodArgsCollection<Method>())) MethodArgsCollection;
	};
#pragma pack(pop)

	/// <summary>
	/// Receiving the compile-time method record
	/// </summary>
	/// <typeparam name="Method">Given method</typeparam>
	/// <returns>Compile-time method record</returns>
	template<typename Method>
	inline constexpr auto GetCttiMethodRecord() {
		using tCtMethodMetadata = CtMethodMetadata<Method>;

		return tCtMethodMetadata{
			.MethodArgsOffset = StructOffsetOfMember(tCtMethodMetadata, MethodArgsCollection),
			.Flags = Method::Flags.GetValue(),
			.DynamicCaller = &MethodCallerGeneratorT<Method>::DynamicCall,
			.MethodName = GetTextRecord(Method::MethodNameGetter()),
			.MethodArgsCollection = AsRttiCollection<1 + Method::OriginalArgsTuple::Count>(GetMethodArgsCollection<Method>())
		};
	}

	/// <summary>
	/// Receiving the compile-time methods
	/// </summary>
	/// <typeparam name="TMethods">Methods type</typeparam>
	/// <returns>Compile-time methods</returns>
	template<typename TMethods, SizeT TIdx = 0>
	inline constexpr auto GetCttiMethods() {
		if constexpr (TMethods::Count == TIdx)
		{
			return TypeTraits::TypeTag<void>{};
		}
		else
		{
			using Method = TMethods::template ItemType<TIdx>;

#pragma pack(push, 1)
			struct MethodsMetaData {
				SizeT OffsetToNext;
				SizeT MethodArgsOffset;
				SizeT Flags;
				DynamicCallerFunc DynamicCaller;
				decltype(GetTextRecord(Method::MethodName)) MethodName;
				decltype(AsRttiCollection<1 + Method::OriginalArgsTuple::Count>(GetMethodArgsCollection<Method>())) MethodArgsCollection;
				decltype(GetCttiMethods<TMethods, TIdx + 1>()) Rest;
			};
#pragma pack(pop)
			return MethodsMetaData{
				.OffsetToNext = StructOffsetOfMember(MethodsMetaData, Rest),
				.MethodArgsOffset = StructOffsetOfMember(MethodsMetaData, MethodArgsCollection),
				.Flags = Method::Flags.GetValue(),
				.DynamicCaller = &MethodCallerGeneratorT<Method>::DynamicCall,
				.MethodName = GetTextRecord(Method::MethodNameGetter()),
				.MethodArgsCollection = AsRttiCollection<1 + Method::OriginalArgsTuple::Count>(GetMethodArgsCollection<Method>()),
				.Rest = GetCttiMethods<TMethods, TIdx + 1>()
			};
		}
	}
}