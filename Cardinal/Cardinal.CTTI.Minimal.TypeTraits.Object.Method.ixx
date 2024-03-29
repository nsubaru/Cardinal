export module Cardinal.CTTI.Minimal:TypeTraits.Object.Method;

export import :TypeTraits.Object.Base;
export import :TypeTraits.Enum;
export import Cardinal.Containers.SmartPtr;

export namespace Cardinal::CTTI::TypeTraits
{
	enum class ECvRefModificators : SizeT {
		Ref, RValueRef, Const, Volatile
	};

	EnumCttiDefinition(ECvRefModificators,
		ECvRefModificators::Ref,
		ECvRefModificators::RValueRef,
		ECvRefModificators::Const,
		ECvRefModificators::Volatile);

	template<typename TType>
	struct CompletedTypeT {
		using Type = TType;
	};

	template<typename TType> requires Concepts::IsAbstractCttiType<TType>
	struct CompletedTypeT<TType> {
		using Type = typename SmartPtr<TType>;
	};

	template<typename TType>
	struct OriginalTypeT {
		using Type = TType;
		constexpr static auto Flags = EnumFlags<ECvRefModificators>();
	};

	template<typename TType>
	struct OriginalTypeT<TType&> {
		using Type = TType;
		constexpr static auto Flags = EnumFlags<ECvRefModificators>(ECvRefModificators::Ref);
	};

	template<typename TType>
	struct OriginalTypeT<TType&&> {
		using Type = TType;
		constexpr static auto Flags = EnumFlags<ECvRefModificators>(ECvRefModificators::RValueRef);
	};

	template<typename TType>
	struct OriginalTypeT<const TType> {
		using Type = TType;
		constexpr static auto Flags = EnumFlags<ECvRefModificators>(ECvRefModificators::Const);
	};

	template<typename TType>
	struct OriginalTypeT<const TType&> {
		using Type = TType;
		constexpr static auto Flags = EnumFlags<ECvRefModificators>(ECvRefModificators::Const) | ECvRefModificators::Ref;
	};

	template<typename TType>
	struct OriginalTypeT<const TType&&> {
		using Type = TType;
		constexpr static auto Flags = EnumFlags<ECvRefModificators>(ECvRefModificators::Const) | ECvRefModificators::RValueRef;
	};

	template<typename TType>
	struct OriginalTypeT<volatile TType> {
		using Type = TType;
		constexpr static auto Flags = EnumFlags<ECvRefModificators>(ECvRefModificators::Volatile);
	};

	template<typename TType>
	struct OriginalTypeT<volatile TType&> {
		using Type = TType;
		constexpr static auto Flags = EnumFlags<ECvRefModificators>(ECvRefModificators::Volatile) | ECvRefModificators::Ref;
	};

	template<typename TType>
	struct OriginalTypeT<volatile TType&&> {
		using Type = TType;
		constexpr static auto Flags = EnumFlags<ECvRefModificators>(ECvRefModificators::Volatile) | ECvRefModificators::RValueRef;
	};

	template<typename TType>
	struct OriginalTypeT<const volatile TType> {
		using Type = TType;
		constexpr static auto Flags = EnumFlags<ECvRefModificators>(ECvRefModificators::Const) | ECvRefModificators::Volatile;
	};

	template<typename TType>
	struct OriginalTypeT<const volatile TType&> {
		using Type = TType;
		constexpr static auto Flags = EnumFlags<ECvRefModificators>(ECvRefModificators::Const) | ECvRefModificators::Volatile | ECvRefModificators::Ref;
	};

	template<typename TType>
	struct OriginalTypeT<const volatile TType&&> {
		using Type = TType;
		constexpr static auto Flags = EnumFlags<ECvRefModificators>(ECvRefModificators::Const) | ECvRefModificators::Volatile | ECvRefModificators::RValueRef;
	};

	template<
		typename TTargetClass,
		EnumFlags<ECvRefModificators>::ValueType TCVRefMofificators,
		typename TName,
		TName(*TVName)(),
		typename TMethod,
		TMethod TPMethod,
		typename TClass,
		typename TRet,
		typename ... TArgs
	>
	struct BaseMethodRecord {
		constexpr static EnumFlags<ECvRefModificators> Flags = [] () constexpr {
			EnumFlags<ECvRefModificators> value;

			if (Bits::BitCheck(TCVRefMofificators, static_cast<EnumFlags<ECvRefModificators>::ValueType>(ECvRefModificators::Ref)))
				value = value | ECvRefModificators::Ref;
			if (Bits::BitCheck(TCVRefMofificators, static_cast<EnumFlags<ECvRefModificators>::ValueType>(ECvRefModificators::RValueRef)))
				value = value | ECvRefModificators::RValueRef;
			if (Bits::BitCheck(TCVRefMofificators, static_cast<EnumFlags<ECvRefModificators>::ValueType>(ECvRefModificators::Const)))
				value = value | ECvRefModificators::Const;
			if (Bits::BitCheck(TCVRefMofificators, static_cast<EnumFlags<ECvRefModificators>::ValueType>(ECvRefModificators::Volatile)))
				value = value | ECvRefModificators::Volatile;

			return value;
		}();

		using OriginalRet = OriginalTypeT<TRet>;
		using OriginalClass = typename CompletedTypeT<TTargetClass>;
		using TargetClass = TTargetClass;
		using OriginalArgsTuple = Containers::Tuple<OriginalTypeT<TArgs>...>;

		using DecayCallType = OriginalRet::Type(*)(OriginalClass::Type&& cls, OriginalTypeT<TArgs>::Type&&... args);

		constexpr static DecayCallType DecayCall = [] () constexpr {
			if constexpr (!Cardinal::TypeTraits::IsAbstract<TTargetClass>)
			{
				return [] (OriginalClass::Type&& cls, OriginalTypeT<TArgs>::Type&&... args) -> OriginalRet::Type {
					return (cls.*TPMethod)(ForwardReference<TArgs>(args)...);
				};
			}
			else
			{
				return [] (OriginalClass::Type&& cls, OriginalTypeT<TArgs>::Type&&... args) -> OriginalRet::Type {
					auto target = (&(*cls));
					return (target->*TPMethod)(ForwardReference<TArgs>(args)...);
				};
			}
		}();

		constexpr static TMethod RawMethodPtr = TPMethod;

		constexpr static auto MethodNameGetter = TVName;

		constexpr static TName MethodName = TVName();
	};

	template<typename TTargetClass, typename TName, TName(*VName)(), typename T, T TP>
	struct MethodRecord;

	template<typename TTargetClass, typename TName, TName(*VName)(), typename TClass, typename TRet, typename ... TArgs, TRet(TClass::* TP)(TArgs...) >
	struct MethodRecord<TTargetClass, TName, VName, TRet(TClass::*)(TArgs...), TP> :
		BaseMethodRecord<
		TTargetClass,
		EnumFlags<ECvRefModificators>().GetValue(),
		TName,
		VName,
		TRet(TClass::*)(TArgs...),
		TP,
		TClass,
		TRet,
		TArgs...> {
	};

	template<typename TTargetClass, typename TName, TName(*VName)(), typename TClass, typename TRet, typename ... TArgs, TRet(TClass::* TP)(TArgs...)&>
	struct MethodRecord<TTargetClass, TName, VName, TRet(TClass::*)(TArgs...)&, TP> :
		BaseMethodRecord<
		TTargetClass,
		(EnumFlags<ECvRefModificators>() | ECvRefModificators::Ref).GetValue(),
		TName,
		VName,
		TRet(TClass::*)(TArgs...)&,
		TP,
		TClass,
		TRet,
		TArgs...> {
	};

	template<typename TTargetClass, typename TName, TName(*VName)(), typename TClass, typename TRet, typename ... TArgs, TRet(TClass::* TP)(TArgs...)&&>
	struct MethodRecord<TTargetClass, TName, VName, TRet(TClass::*)(TArgs...)&&, TP> :
		BaseMethodRecord<
		TTargetClass,
		(EnumFlags<ECvRefModificators>() | ECvRefModificators::RValueRef).GetValue(),
		TName,
		VName,
		TRet(TClass::*)(TArgs...)&&,
		TP,
		TClass,
		TRet,
		TArgs...> {
	};

	template<typename TTargetClass, typename TName, TName(*VName)(), typename TClass, typename TRet, typename ... TArgs, TRet(TClass::* TP)(TArgs...) const>
	struct MethodRecord<TTargetClass, TName, VName, TRet(TClass::*)(TArgs...) const, TP> :
		BaseMethodRecord<
		TTargetClass,
		(EnumFlags<ECvRefModificators>() | ECvRefModificators::Const).GetValue(),
		TName,
		VName,
		TRet(TClass::*)(TArgs...) const,
		TP,
		TClass,
		TRet,
		TArgs...> {
	};

	template<typename TTargetClass, typename TName, TName(*VName)(), typename TClass, typename TRet, typename ... TArgs, TRet(TClass::* TP)(TArgs...) volatile>
	struct MethodRecord<TTargetClass, TName, VName, TRet(TClass::*)(TArgs...) volatile, TP> :
		BaseMethodRecord<
		TTargetClass,
		(EnumFlags<ECvRefModificators>() | ECvRefModificators::Volatile).GetValue(),
		TName,
		VName,
		TRet(TClass::*)(TArgs...) volatile,
		TP,
		TClass,
		TRet,
		TArgs...> {
	};

	template<typename TTargetClass, typename TName, TName(*VName)(), typename TClass, typename TRet, typename ... TArgs, TRet(TClass::* TP)(TArgs...) const volatile>
	struct MethodRecord<TTargetClass, TName, VName, TRet(TClass::*)(TArgs...) const volatile, TP> :
		BaseMethodRecord<
		TTargetClass,
		(ECvRefModificators::Const | ECvRefModificators::Volatile).GetValue(),
		TName,
		VName,
		TRet(TClass::*)(TArgs...) const volatile,
		TP,
		TClass,
		TRet,
		TArgs...> {
	};

	template<typename TTargetClass, typename TName, TName(*VName)(), typename TClass, typename TRet, typename ... TArgs, TRet(TClass::* TP)(TArgs...) const&>
	struct MethodRecord<TTargetClass, TName, VName, TRet(TClass::*)(TArgs...) const&, TP> :
		BaseMethodRecord<
		TTargetClass,
		(ECvRefModificators::Ref | ECvRefModificators::Const).GetValue(),
		TName,
		VName,
		TRet(TClass::*)(TArgs...) const&,
		TP,
		TClass,
		TRet,
		TArgs...> {
	};

	template<typename TTargetClass, typename TName, TName(*VName)(), typename TClass, typename TRet, typename ... TArgs, TRet(TClass::* TP)(TArgs...) volatile&>
	struct MethodRecord<TTargetClass, TName, VName, TRet(TClass::*)(TArgs...) volatile&, TP> :
		BaseMethodRecord<
		TTargetClass,
		(ECvRefModificators::Ref | ECvRefModificators::Volatile).GetValue(),
		TName,
		VName,
		TRet(TClass::*)(TArgs...) volatile&,
		TP,
		TClass,
		TRet,
		TArgs...> {
	};

	template<typename TTargetClass, typename TName, TName(*VName)(), typename TClass, typename TRet, typename ... TArgs, TRet(TClass::* TP)(TArgs...) const volatile&>
	struct MethodRecord<TTargetClass, TName, VName, TRet(TClass::*)(TArgs...) const volatile&, TP> :
		BaseMethodRecord<
		TTargetClass,
		(ECvRefModificators::Ref | ECvRefModificators::Const | ECvRefModificators::Volatile).GetValue(),
		TName,
		VName,
		TRet(TClass::*)(TArgs...) const volatile&,
		TP,
		TClass,
		TRet,
		TArgs...> {
	};

	template<typename TTargetClass, typename TName, TName(*VName)(), typename TClass, typename TRet, typename ... TArgs, TRet(TClass::* TP)(TArgs...) const&&>
	struct MethodRecord<TTargetClass, TName, VName, TRet(TClass::*)(TArgs...) const&&, TP> :
		BaseMethodRecord<
		TTargetClass,
		(ECvRefModificators::RValueRef | ECvRefModificators::Const).GetValue(),
		TName,
		VName,
		TRet(TClass::*)(TArgs...) const&&,
		TP,
		TClass,
		TRet,
		TArgs...> {
	};

	template<typename TTargetClass, typename TName, TName(*VName)(), typename TClass, typename TRet, typename ... TArgs, TRet(TClass::* TP)(TArgs...) volatile&&>
	struct MethodRecord<TTargetClass, TName, VName, TRet(TClass::*)(TArgs...) volatile&&, TP> :
		BaseMethodRecord<
		TTargetClass,
		(ECvRefModificators::RValueRef | ECvRefModificators::Volatile).GetValue(),
		TName,
		VName,
		TRet(TClass::*)(TArgs...) volatile&&,
		TP,
		TClass,
		TRet,
		TArgs...> {
	};

	template<typename TTargetClass, typename TName, TName(*VName)(), typename TClass, typename TRet, typename ... TArgs, TRet(TClass::* TP)(TArgs...) const volatile&&>
	struct MethodRecord<TTargetClass, TName, VName, TRet(TClass::*)(TArgs...) const volatile&&, TP> :
		BaseMethodRecord<
		TTargetClass,
		(ECvRefModificators::RValueRef | ECvRefModificators::Const | ECvRefModificators::Volatile).GetValue(),
		TName,
		VName,
		TRet(TClass::*)(TArgs...) const volatile&&,
		TP,
		TClass,
		TRet,
		TArgs...> {
	};
}