export module Cardinal.Containers.BitMap;

export import Cardinal.Core;
export import Cardinal.Memory;
export import Cardinal.Containers.LINQ;
export import Cardinal.Containers.TypeTraits;

import Cardinal.Exceptions.Internals;

export namespace Cardinal::Containers
{
	/// <summary>
	/// A bit map with fixed size, a number of bits is indicated by "BitCount" argument
	/// BitCount cannot be higher than (2 * 1024 * 1024 * 1024 * 8) - 1, that is 2Gb - 1 byte
	/// </summary>
	template<typename THeap, SizeT BitCount, typename T>
	class BaseBitMapT : public Memory::Allocator<THeap> {
	public:
		static constexpr SizeT BitCountInFragment = sizeof(T) * 8ui64;
		static constexpr SizeT ActualSize = BitCount;

	private:
		static_assert(BitCount != 0, "BitMap bit count must not equal zero");
		static_assert(Math::RoundToUpper(BitCount, BitCountInFragment) < (2Gb).Bytes, "BitMap Size must not bigger then 2GB - 1b");

	public:
		static constexpr SizeT Size = Math::RoundToUpper(BitCount, BitCountInFragment);
		static constexpr SizeT FullClustersCount = BitCount / BitCountInFragment;
		static constexpr SizeT LastClusterBitsCount = BitCount % BitCountInFragment;

	public:
		/// <summary>
		/// A bitmap size
		/// </summary>
		/// <returns>A number of bits</returns>
		ForceInline constexpr SizeT GetCount() {
			return BitCount;
		}

		/// <summary>
		/// A number of bits
		/// </summary>
		SizeT Property(GetCount) Count;

	public:
		using BitMapDataType = T(&)[Size];

	private:
		T bitMap[Size] = { 0 };

	public:
		/// <summary>
		/// A constructor. If flag is set then bit map will be filled
		/// </summary>
		/// <param name="IsSetAll">If "true" then bit map is created already filled</param>
		constexpr BaseBitMapT(Boolean IsSetAll = false) {
			if (IsSetAll)
			{
				for (SizeT i = 0; i < FullClustersCount; i++)
				{
					bitMap[i] = Math::Max<T>();
				}
				if constexpr (FullClustersCount < Size)
				{
					for (SizeT i = 0; i < LastClusterBitsCount; i++)
					{
						Bits::BitSet<T>(bitMap[FullClustersCount], i);
					}
				}
			}
		}

		/// <summary>
		/// A solid area of a sertain size searching
		/// </summary>
		/// <param name="StartPosition">Starting position</param>
		/// <param name="RequireCount">The requested area size</param>
		/// <returns>A number of first element in found area. Or -1 if such area could not be found</returns>
		constexpr SizeT FindBitSequalFrom(SizeT StartPosition, SizeT RequireCount) {
			if (RequireCount == 0)
				return 0;

			if (StartPosition == 0)
			{
				return FindBitSequal(RequireCount);
			}
			else
			{
				SizeT RangeSize = 0;
				Boolean IsFound = false;

				SizeT bitInFirstCluster = StartPosition % BitCountInFragment;
				SizeT startCluster = StartPosition / BitCountInFragment;

				for (SizeT bit = bitInFirstCluster; bit < BitCountInFragment; bit++)
				{
					if (!Bits::BitCheck(bitMap[startCluster], bit))
						if (IsFound == false)
						{
							RangeSize = 0; IsFound = true;
						}
						else
						{
							RangeSize++;
							if (RangeSize == RequireCount)
								return ((StartPosition * BitCountInFragment) + bit) - RequireCount;
						}
					else
						if (IsFound == true)
							IsFound = false;
				}

				for (SizeT byte = startCluster + 1; byte < FullClustersCount; byte++)
				{
					if (bitMap[byte] == Math::Max<T>())
						continue;
					for (SizeT bit = 0; bit < BitCountInFragment; bit++)
					{
						if (!Bits::BitCheck(bitMap[byte], bit))
							if (IsFound == false)
							{
								RangeSize = 0; IsFound = true;
							}
							else
							{
								RangeSize++;
								if (RangeSize == RequireCount)
									return ((byte * BitCountInFragment) + bit) - RequireCount;
							}
						else
							if (IsFound == true)
								IsFound = false;
					}
				}

				if constexpr (LastClusterBitsCount != 0)
				{
					for (SizeT bit = 0; bit < LastClusterBitsCount; bit++)
					{
						if (!Bits::BitCheck(bitMap[FullClustersCount], bit))
							if (IsFound == false)
							{
								RangeSize = 0; IsFound = true;
							}
							else
							{
								RangeSize++;
								if (RangeSize == RequireCount)
									return ((FullClustersCount * BitCountInFragment) + bit) - RequireCount;
							}
						else
							if (IsFound == true)
								IsFound = false;
					}
				}
				return -1;
			}
		}

		/// <summary>
		/// A solid area of a sertain size searching
		/// </summary>
		/// <param name="RequireCount">The requested area size</param>
		/// <returns>A number of first element in found area. Or -1 if such area could not be found</returns>
		constexpr SizeT FindBitSequal(SizeT RequireCount) {
			if (RequireCount == 0)
				return 0;

			SizeT RangeSize = 0;
			Boolean IsFound = false;
			for (SizeT byte = 0; byte < FullClustersCount; byte++)
			{
				if (bitMap[byte] == Math::Max<T>())
					continue;
				for (SizeT bit = 0; bit < BitCountInFragment; bit++)
				{
					if (!Bits::BitCheck(bitMap[byte], bit))
						if (IsFound == false)
						{
							RangeSize = 0; IsFound = true;
						}
						else
						{
							RangeSize++;
							if (RangeSize == RequireCount)
								return ((byte * BitCountInFragment) + bit) - (RequireCount - 1);
						}
					else
						if (IsFound == true)
							IsFound = false;
				}
			}

			if constexpr (LastClusterBitsCount != 0)
			{
				for (SizeT bit = 0; bit < LastClusterBitsCount; bit++)
				{
					if (!Bits::BitCheck(bitMap[FullClustersCount], bit))
						if (IsFound == false)
						{
							RangeSize = 0; IsFound = true;
						}
						else
						{
							RangeSize++;
							if (RangeSize == RequireCount)
								return ((FullClustersCount * BitCountInFragment) + bit) - (RequireCount - 1);
						}
					else
						if (IsFound == true)
							IsFound = false;
				}
			}
			return -1;
		};

		/// <summary>
		/// A given area filling
		/// </summary>
		/// <param name="Position">Start index</param>
		/// <param name="BitsCount">Number of elements</param>
		constexpr void SetBitSequal(SizeT Position, SizeT BitsCount) {
			if (Position + BitsCount > BitCount)
				Internals::Exceptions::ThrowIndexOutOfRange(SourcePosition, Position, BitCount + BitsCount);

			SizeT StartCluster = Position / BitCountInFragment;
			SizeT StartBit = Position % BitCountInFragment;

			SizeT EndCluster = (Position + BitsCount) / BitCountInFragment;
			SizeT EndBit = (Position + BitsCount) % BitCountInFragment;

			if (StartCluster == EndCluster)
			{
				for (SizeT bit = StartBit; bit < EndBit; bit++)
					Bits::BitSet(bitMap[StartCluster], bit);
			}
			else
			{
				if (StartBit != 0)
				{
					for (SizeT bit = StartBit; bit < BitCountInFragment; bit++)
						Bits::BitSet(bitMap[StartCluster], bit);
				}
				else
				{
					bitMap[StartCluster] = Math::Max<T>();
				}

				if (EndBit != BitCountInFragment)
				{
					for (SizeT bit = 0; bit < EndBit; bit++)
						Bits::BitSet(bitMap[EndCluster], bit);
				}
				else
				{
					bitMap[EndCluster] = Math::Max<T>();
				}

				if (StartCluster - EndCluster > 1)
				{
					for (SizeT byte = StartCluster + 1; byte < EndCluster; byte++)
						bitMap[byte] = Math::Max<T>();
				}
			}
		};

		/// <summary>
		/// A given area freeing
		/// </summary>
		/// <param name="Position">An index of first element</param>
		/// <param name="BitsCount">A number of bits</param>
		constexpr void ResetBitSequal(SizeT Position, SizeT BitsCount) {
			if (Position + BitsCount > BitCount)
				Internals::Exceptions::ThrowIndexOutOfRange(SourcePosition, Position, BitCount + BitsCount);

			SizeT StartCluster = Position / BitCountInFragment;
			SizeT StartBit = Position % BitCountInFragment;

			SizeT EndCluster = (Position + BitsCount) / BitCountInFragment;
			SizeT EndBit = (Position + BitsCount) % BitCountInFragment;

			if (StartCluster == EndCluster)
			{
				for (SizeT bit = StartBit; bit < EndBit; bit++)
					Bits::BitReset(bitMap[StartCluster], bit);
			}
			else
			{
				if (StartBit != 0)
				{
					for (SizeT bit = StartBit; bit < BitCountInFragment; bit++)
						Bits::BitReset(bitMap[StartCluster], bit);
				}
				else
				{
					bitMap[StartCluster] = 0;
				}

				if (EndBit != BitCountInFragment)
				{
					for (SizeT bit = 0; bit < EndBit; bit++)
						Bits::BitReset(bitMap[EndCluster], bit);
				}
				else
				{
					bitMap[EndCluster] = 0;
				}

				if (StartCluster - EndCluster > 1)
				{
					for (SizeT byte = StartCluster + 1; byte < EndCluster; byte++)
						bitMap[byte] = 0;
				}
			}
		};

		/// <summary>
		/// Bit setting
		/// </summary>
		/// <param name="Position">Index of element</param>
		constexpr void SetBit(SizeT Position) {
			if (Position > BitCount)
				Internals::Exceptions::ThrowIndexOutOfRange(SourcePosition, Position, BitCount);
			Bits::BitSet(bitMap[Position / BitCountInFragment], Position % BitCountInFragment);
		};

		/// <summary>
		/// Bit resetting
		/// </summary>
		/// <param name="Position">Index of element</param>
		constexpr void ResetBit(SizeT Position) {
			if (Position > BitCount)
				Internals::Exceptions::ThrowIndexOutOfRange(SourcePosition, Position, BitCount);
			Bits::BitReset(bitMap[Position / BitCountInFragment], Position % BitCountInFragment);
		};

		/// <summary>
		/// Checking if the bit is free on a given position
		/// </summary>
		/// <param name="Position">Index of element</param>
		/// <returns>"True" if position is free</returns>
		constexpr Boolean IsZero(SizeT Position) {
			if (Position > BitCount)
				Internals::Exceptions::ThrowIndexOutOfRange(SourcePosition, Position, BitCount);
			return !Bits::BitCheck(bitMap[Position / BitCountInFragment], Position % BitCountInFragment);
		};

		/// <summary>
		/// Resetting all the bits (operation can be quite long)
		/// </summary>
		ForceInline constexpr void ResetAll() {
			Memory::MemBlkFill(bitMap, Size, 0);
		};

		/// <summary>
		/// A number of setted bits counting
		/// </summary>
		/// <returns>A number of setted bits</returns>
		constexpr SizeT SettedBitCount() {
			SizeT settedCount = 0;
			for (SizeT byte = 0; byte < FullClustersCount; byte++)
			{
				if (bitMap[byte] == 0)
					settedCount += BitCountInFragment;
				else
					for (SizeT j = 0; j < BitCountInFragment; j++)
						if (Bits::BitCheck(bitMap[byte], j))
							settedCount++;
			}
			if constexpr (FullClustersCount > Size)
			{
				for (SizeT j = 0; j < BitCountInFragment; j++)
					if (Bits::BitCheck(bitMap[FullClustersCount], j))
						settedCount++;
			}
			return settedCount;
		};

		/// <summary>
		/// A two bit maps unification using bit "AND"
		/// </summary>
		/// <param name="Second">A second bit map</param>
		/// <returns>A current bit map after operation</returns>
		constexpr BaseBitMapT& operator&=(const BaseBitMapT& Second) {
			for (SizeT i = 0; i < FullClustersCount; i++)
			{
				this->bitMap[i] &= Second.bitMap[i];
			}

			for (SizeT i = 0; i < LastClusterBitsCount; i++)
			{
				if (Bits::BitCheck(this->bitMap[FullClustersCount], i) && Bits::BitCheck(Second.bitMap[FullClustersCount], i))
					Bits::BitSet(this->bitMap[FullClustersCount], i);
				else
					Bits::BitReset(this->bitMap[FullClustersCount], i);
			}
		}

		/// <summary>
		/// A two bit maps unification using bit "OR"
		/// </summary>
		/// <param name="Second">A second bit map</param>
		/// <returns>A current bit map after the operation</returns>
		constexpr BaseBitMapT& operator|=(const BaseBitMapT& Second) {
			for (SizeT i = 0; i < FullClustersCount; i++)
			{
				this->bitMap[i] |= Second.bitMap[i];
			}

			for (SizeT i = 0; i < LastClusterBitsCount; i++)
			{
				if (Bits::BitCheck(this->bitMap[FullClustersCount], i) || Bits::BitCheck(Second.bitMap[FullClustersCount], i))
					Bits::BitSet(this->bitMap[FullClustersCount], i);
				else
					Bits::BitReset(this->bitMap[FullClustersCount], i);
			}
		}

		/// <summary>
		/// Equality operator of two bit maps
		/// </summary>
		/// <param name="Second">A second bit map</param>
		/// <returns>"True" if maps are equal</returns>
		constexpr bool operator==(BaseBitMapT& Second) {
			for (SizeT i = 0; i < FullClustersCount; i++)
			{
				if (this->bitMap[i] != Second.bitMap[i])
					return false;
			}

			for (SizeT i = 0; i < LastClusterBitsCount; i++)
			{
				if (Bits::BitCheck(this->bitMap[FullClustersCount], i) == Bits::BitCheck(Second.bitMap[FullClustersCount], i))
					return false;
			}
			return true;
		}

		/// <summary>
		/// Inequality operator of two bit maps
		/// </summary>
		/// <param name="Second">A second bit map</param>
		/// <returns>"False" if maps are not equal</returns>
		constexpr bool operator!=(BaseBitMapT& Second) {
			for (SizeT i = 0; i < FullClustersCount; i++)
			{
				if (this->bitMap[i] != Second.bitMap[i])
					return true;
			}

			for (SizeT i = 0; i < LastClusterBitsCount; i++)
			{
				if (Bits::BitCheck(this->bitMap[FullClustersCount], i) == Bits::BitCheck(Second.bitMap[FullClustersCount], i))
					return true;
			}
			return false;
		}

		/// <summary>
		/// An access operator to cluster of bits
		/// </summary>
		/// <param name="Index">A cluster number</param>
		/// <returns>A reference to cluster</returns>
		constexpr T& operator[](SizeT Index) {
			if (Index < Size)
				return this->bitMap[Index];
			else
				Internals::Exceptions::ThrowIndexOutOfRange(SourcePosition, Index, Size);
		}

		/// <summary>
		/// An access operator to cluster of bits
		/// </summary>
		/// <param name="Index">A cluster number</param>
		/// <returns>A reference to cluster</returns>
		constexpr const T& operator[](SizeT Index) const {
			if (Index < Size)
				return this->bitMap[Index];
			else
				Internals::Exceptions::ThrowIndexOutOfRange(SourcePosition, Index, Size);
		}

	public:
		/// <summary>
		/// Return raw bitmap
		/// </summary>
		/// <returns>Raw bitmap</returns>
		operator const BitMapDataType() const& {
			return (const BitMapDataType) bitMap;
		}

		/// <summary>
		/// Return raw bitmap
		/// </summary>
		/// <returns>Raw bitmap</returns>
		operator BitMapDataType()& {
			return bitMap;
		}

	protected:
		template<template<typename, typename> typename, typename, typename, Boolean, Boolean, Boolean, Boolean>
		friend class LINQ::ICollection;

		template<template<typename, SizeT, typename> typename TCollection, typename THeap, typename T, SizeT Size, Boolean TIsConcurencySuport>
		friend class LINQ::IFixedCollection;

		template<template<typename, typename> typename, typename, typename, Boolean, Boolean, Boolean, Boolean>
		friend class LINQ::ICollectionBase;

		template<typename U>
		friend struct Cardinal::Serialization::SerializationHandler;

	protected:
#pragma warning(push)
#pragma warning(disable: 4172)

		template<typename LambdaT>
		BaseBitMapT& ForEachImpl(LambdaT&& lambda) {
			if constexpr (TypeTraits::IsLambdaConvertibleV<LambdaT, void, T&>)
			{
				for (SizeT i = 0; i < Size; i++)
				{
					lambda(bitMap[i]);
				}
				return *this;
			}
			else if constexpr (TypeTraits::IsLambdaConvertibleV<LambdaT, void, T&, SizeT>)
			{
				for (SizeT i = 0; i < Size; i++)
				{
					lambda(bitMap[i], i);
				}
				return *this;
			}
			else
			{
				static_assert(TypeTraits::IsLambdaConvertibleV<LambdaT, void, T&> ||
					TypeTraits::IsLambdaConvertibleV<LambdaT, void, T&, SizeT>);
			}
		}

		template<typename LambdaT>
		const BaseBitMapT& ForEachImpl(LambdaT&& lambda) const {
			if constexpr (TypeTraits::IsLambdaConvertibleV<LambdaT, void, const T&>)
			{
				for (SizeT i = 0; i < Size; i++)
				{
					lambda(bitMap[i]);
				}
				return *this;
			}
			else if constexpr (TypeTraits::IsLambdaConvertibleV<LambdaT, void, const T&, SizeT>)
			{
				for (SizeT i = 0; i < Size; i++)
				{
					lambda(bitMap[i], i);
				}
				return *this;
			}
			else
			{
				static_assert(TypeTraits::IsLambdaConvertibleV<LambdaT, void, const T&> ||
					TypeTraits::IsLambdaConvertibleV<LambdaT, void, const T&, SizeT>);
			}
		}

		template<typename U, typename LambdaT>
		U AggregateImpl(LambdaT&& lambda) const {
			U val = U();
			for (SizeT i = 0; i < Size; i++)
			{
				lambda(val, bitMap[i]);
			}
			return val;
		}

		template<typename U, typename LambdaT>
		T AggregateImpl(LambdaT&& lambda, const U& Value) const {
			U result = Value;
			for (SizeT i = 0; i < Size; i++)
			{
				lambda(result, bitMap[i]);
			}
			return MoveRValue(result);
		}

		template<typename U, typename LambdaT>
		U AggregateImpl(LambdaT&& lambda, U&& Value) const {
			for (SizeT i = 0; i < Size; i++)
			{
				lambda(Value, bitMap[i]);
			}
			return MoveRValue(Value);
		}

		bool AnyImpl() const {
			return Size > 0;
		}

		template<typename Lambda>
		bool AnyImpl(Lambda&& lambda) const {
			for (SizeT i = 0; i < Size; i++)
			{
				if (lambda(bitMap[i]))
					return true;
			}
			return false;
		}

		template<typename Lambda>
		bool AllImpl(Lambda&& lambda) const {
			for (SizeT i = 0; i < Size; i++)
			{
				if (!lambda(bitMap[i]))
					return false;
			}
			return true;
		}

		template<typename Lambda>
		T& FirstImpl(Lambda&& lambda) {
			for (SizeT i = 0; i < Size; i++)
				if (lambda(bitMap[i]))
					return static_cast<T&>(bitMap[i]);

			Internals::Exceptions::ThrowNullPointException(SourcePosition);
		}

		template<typename Lambda>
		T& LastImpl(Lambda&& lambda) {
			SizeT Index = -1;
			for (SizeT i = 0; i < Size; i++)
				if (lambda(bitMap[i]))
					Index = i;

			if (Index != -1)
				return static_cast<T&>(bitMap[Index]);

			Internals::Exceptions::ThrowNullPointException(SourcePosition);
		}

		template<typename Lambda>
		const T& FirstImpl(Lambda&& lambda) const {
			for (SizeT i = 0; i < Size; i++)
				if (lambda(bitMap[i]))
					return static_cast<T&>(bitMap[i]);

			Internals::Exceptions::ThrowNullPointException(SourcePosition);
		}

		template<typename Lambda>
		const T& LastImpl(Lambda&& lambda) const {
			SizeT Index = -1;
			for (SizeT i = 0; i < Size; i++)
				if (lambda(bitMap[i]))
					Index = i;

			if (Index != -1)
				return static_cast<T&>(bitMap[Index]);

			Internals::Exceptions::ThrowNullPointException(SourcePosition);
		}

		ForceInline BaseBitMapT& ReverseImpl() {
			for (SizeT i = 0; i < Size / 2; i++)
			{
				Swap(bitMap[i], bitMap[(Size - 1) - i]);
			}
			return *this;
		}

		ForceInline void UnsafeSet(const T& Value, SizeT Index) {
			bitMap[Index] = Value;
		}

		ForceInline void UnsafeSet(T&& Value, SizeT Index) {
			bitMap[Index] = MoveRValue(Value);
		}

		ForceInline BaseBitMapT& RemoveAllImpl() {
			this->ResetAll();
			return *this;
		}

		template<typename Lambda>
		BaseBitMapT& RemoveAllImpl(Lambda&& lambda) {
			for (SizeT i = 0; i < Size; i++)
			{
				lambda(MoveRValue(bitMap[i]));
				bitMap[i] = 0;
			}
			return *this;
		}
#pragma warning(pop)
	};

	/// <summary>
	/// A bit map with fixed size, a number of bits is indicated by "BitCount" argument.
	/// BitCount cannot be higher than (2 * 1024 * 1024 * 1024 * 8) - 1, that is 2Gb - 1 byte
	/// </summary>
	template<SizeT BitCount, typename T = Byte>
	using BitMapT = BaseBitMapT<Memory::Heap, BitCount, T>;

	/// <summary>
	/// A bit map with fixed size, a number of bits is indicated by "BitCount" argument.
	/// BitCount cannot be higher than (2 * 1024 * 1024 * 1024 * 8) - 1, that is 2Gb - 1 byte
	/// </summary>
	template<SizeT BitCount, typename T = Byte>
	using SensitiveBitMapT = BaseBitMapT<Memory::SensitiveDataHeap, BitCount, T>;
}

export namespace Cardinal::TypeTraits::Collection
{
	template<typename THeap, SizeT BitCount, typename T>
	struct IsBitMapT<Cardinal::Containers::BaseBitMapT<THeap, BitCount, T>> : LogicTrue {};
}