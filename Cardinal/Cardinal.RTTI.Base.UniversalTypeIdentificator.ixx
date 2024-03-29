export module Cardinal.RTTI:Base.UniversalTypeIdentificator;

export import :Base.Base;

export namespace Cardinal::RTTI
{
	/// <summary>
	/// C++ type information
	/// </summary>
	using CxxTypeInfoT = const TypeInfo*;

#pragma pack(push, 1)
	/// <summary>
	/// Type UID structure
	/// </summary>
	struct UniversalTypeIdentificator {
		union {
			UUID Id;
			struct {
				Settings::KernelVersionT KernelVersion;
				union {
					ID TypeId;
					CTTI::TypeTraits::CTTITypeRttiIdGetter TypeRttiIdGetter;
				};
			};
		};

		StructCttiDef(UniversalTypeIdentificator,
			DefStructFieldData(UniversalTypeIdentificator, KernelVersion),
			DefStructFieldData(UniversalTypeIdentificator, TypeId));

		/// <summary>
		/// Empty constructor
		/// </summary>
		constexpr UniversalTypeIdentificator() {}

		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="id">UID</param>
		constexpr UniversalTypeIdentificator(UUID id) :
			Id(id) {}

		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="kernelVersion">Kernel version</param>
		/// <param name="typeId">Type ID</param>
		constexpr UniversalTypeIdentificator(Settings::KernelVersionT kernelVersion, ID typeId) :
			KernelVersion(kernelVersion), TypeId(typeId) {}

		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="kernelVersion">Kernel version</param>
		/// <param name="typeRttiIdGetter">Type information</param>
		constexpr UniversalTypeIdentificator(Settings::KernelVersionT kernelVersion, CTTI::TypeTraits::CTTITypeRttiIdGetter typeRttiIdGetter) :
			KernelVersion(kernelVersion), TypeRttiIdGetter(typeRttiIdGetter) {}

		/// <summary>
		/// Universal comparing operator
		/// </summary>
		/// <param name="other">Second ID to compare</param>
		/// <returns>"True" if given IDs is equal</returns>
		constexpr auto operator<=>(const UniversalTypeIdentificator& other) const {
			return this->Id <=> other.Id;
		}

		/// <summary>
		/// Equality operator
		/// </summary>
		/// <param name="other">Second ID to compare</param>
		/// <returns>"True" if given IDs is equal</returns>
		constexpr bool operator==(const UniversalTypeIdentificator& other) const {
			return (this->Id <=> other.Id) == 0;
		}

		/// <summary>
		/// Inequality operator
		/// </summary>
		/// <param name="other">Second ID to compare</param>
		/// <returns>"True" if given IDs is not equal</returns>
		constexpr bool operator!=(const UniversalTypeIdentificator& other) const {
			return (this->Id <=> other.Id) != 0;
		}

		/// <summary>
		/// Convert UniversalTypeIdentificator to string as UUID
		/// </summary>
		/// <typeparam name="THeap">String heap</typeparam>
		/// <returns>String representation of type id</returns>
		template<typename THeap>
		BaseString<THeap> toString() const {
			return BaseString<THeap>::toString(Id);
		}
	};

#pragma pack(pop)

	/// <summary>
	/// Returns PE executable start address in memory
	/// </summary>
	/// <returns>PE executable start address</returns>
	SizeT GetPeImageBase();
}