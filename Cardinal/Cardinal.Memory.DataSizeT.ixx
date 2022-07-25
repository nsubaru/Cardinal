export module Cardinal.Memory.DataSizeT;

export import Cardinal.Core;

export namespace Cardinal::Memory
{
	/// <summary>
	/// Binary data size type
	/// </summary>
	class DataSizeT final {
	public:
		using ByteSizeT = UInt64;

	public:
		/// <summary>
		/// Kilobyte in bytes
		/// </summary>
		static constexpr ByteSizeT Kilobyte = 1024;

		/// <summary>
		/// Megabyte in bytes
		/// </summary>
		static constexpr ByteSizeT Megabyte = Kilobyte * 1024;

		/// <summary>
		/// Gigabyte in bytes
		/// </summary>
		static constexpr ByteSizeT Gigabyte = Megabyte * 1024;

		/// <summary>
		/// Terabyte in bytes
		/// </summary>
		static constexpr ByteSizeT Terabyte = Gigabyte * 1024;

		/// <summary>
		/// Petabyte in bytes
		/// </summary>
		static constexpr ByteSizeT Petabyte = Terabyte * 1024;

		/// <summary>
		/// Exabyte in bytes
		/// </summary>
		static constexpr ByteSizeT Exabyte = Petabyte * 1024;

		/// <summary>
		/// Max allowed bytes
		/// </summary>
		static constexpr ByteSizeT Maxbytes = Math::Max<ByteSizeT>();

	public:
		/// <summary>
		/// Bytes
		/// </summary>
		ByteSizeT Bytes;

	public:
		/// <summary>
		/// Empty ctor
		/// </summary>
		constexpr DataSizeT() : Bytes(0) {}

		/// <summary>
		/// Ctor from bytes
		/// </summary>
		/// <param name="bytes">Bytes count</param>
		constexpr explicit DataSizeT(SizeT bytes) : Bytes(bytes) {}

		/// <summary>
		/// Ctor from bytes
		/// </summary>
		/// <param name="bytes">Bytes count</param>
		constexpr explicit DataSizeT(Concepts::UnsignedInteger auto bytes) : Bytes(bytes) {}

	public:
		/// <summary>
		/// Add bytes
		/// </summary>
		/// <param name="bytes">Bytes count></param>
		constexpr void AddBytes(Concepts::UnsignedInteger auto bytes) { this->Bytes += bytes; }

	public:
		/// <summary>
		/// Get as kilobytes
		/// </summary>
		/// <returns></returns>
		constexpr ByteSizeT GetKilobytes() const { return Bytes / Kilobyte; }

		/// <summary>
		/// Set as kilobytes
		/// </summary>
		/// <param name="kilobytes">Kilobytes count</param>
		constexpr void SetKilobytes(Concepts::UnsignedInteger auto kilobytes) { this->Bytes = kilobytes * Kilobyte; }

		/// <summary>
		/// Add kilobytes
		/// </summary>
		/// <param name="kilobytes">Kilobytes count</param>
		/// <returns>Ref to this object</returns>
		constexpr DataSizeT& AddKilobytes(Concepts::UnsignedInteger auto kilobytes) { this->Bytes += kilobytes * Kilobyte; return *this; }

	public:
		/// <value>
		/// Kilobytes count
		/// </value>
		ByteSizeT Property(GetKilobytes, SetKilobytes) Kilobytes;

	public:
		/// <summary>
		/// Get as megabytes
		/// </summary>
		/// <returns>Count of megabytes</returns>
		constexpr ByteSizeT GetMegabytes() const { return Bytes / Megabyte; }

		/// <summary>
		/// Set as megabytes
		/// </summary>
		/// <param name="megabytes">Megabytes count</param>
		constexpr void SetMegabytes(Concepts::UnsignedInteger auto megabytes) { this->Bytes = megabytes * Megabyte; }

		/// <summary>
		/// Add megabytes
		/// </summary>
		/// <param name="megabytes">Megabytes count</param>
		/// <returns>Ref to this object</returns>
		constexpr DataSizeT& AddMegabytes(Concepts::UnsignedInteger auto megabytes) { this->Bytes += megabytes * Megabyte; return *this; }

	public:
		/// <summary>
		/// Megabytes count
		/// </summary>
		ByteSizeT Property(GetMegabytes, SetMegabytes) Megabytes;

	public:
		/// <summary>
		/// Get as gigabytes
		/// </summary>
		/// <returns>Count of gigabytes</returns>
		constexpr ByteSizeT GetGigabytes() const { return Bytes / Gigabyte; }

		/// <summary>
		/// Set as gigabytes
		/// </summary>
		/// <param name="gigabytes">Gigabytes count</param>
		constexpr void SetGigabytes(Concepts::UnsignedInteger auto gigabytes) { this->Bytes = gigabytes * Gigabyte; }

		/// <summary>
		/// Add gigabytes
		/// </summary>
		/// <param name="gigabytes">Gigabytes count</param>
		/// <returns>Ref to this object</returns>
		constexpr DataSizeT& AddGigabytes(Concepts::UnsignedInteger auto gigabytes) { this->Bytes += gigabytes * Gigabyte; return *this; }

	public:
		/// <summary>
		/// Gigabytes count
		/// </summary>
		ByteSizeT Property(GetGigabytes, SetGigabytes) Gigabytes;

	public:
		/// <summary>
		/// Get as terabytes
		/// </summary>
		/// <returns>Count of terabytes</returns>
		constexpr ByteSizeT GetTerabytes() const { return Bytes / Terabyte; }

		/// <summary>
		/// Set as terabytes
		/// </summary>
		/// <param name="terabytes">Terabytes count</param>
		constexpr void SetTerabytes(Concepts::UnsignedInteger auto terabytes) { this->Bytes = terabytes * Terabyte; }

		/// <summary>
		/// Add terabytes
		/// </summary>
		/// <param name="terabytes">Terabytes count</param>
		/// <returns>Ref to this object</returns>
		constexpr DataSizeT& AddTerabytes(Concepts::UnsignedInteger auto terabytes) { this->Bytes += terabytes * Terabyte; return *this; }

	public:
		/// <summary>
		/// Terabytes count
		/// </summary>
		ByteSizeT Property(GetTerabytes, SetTerabytes) Terabytes;

	public:
		/// <summary>
		/// Get as petabytes
		/// </summary>
		/// <returns>Petabytes count</returns>
		constexpr ByteSizeT GetPetabytes() const { return Bytes / Petabyte; }

		/// <summary>
		/// Set as petabytes
		/// </summary>
		/// <param name="petabytes">Petabytes count</param>
		constexpr void SetPetabytes(Concepts::UnsignedInteger auto petabytes) { this->Bytes = petabytes * Petabyte; }

		/// <summary>
		/// Add petabytes
		/// </summary>
		/// <param name="petabytes">Petabytes count</param>
		/// <returns>Ref to this object</returns>
		constexpr DataSizeT& AddPetabytes(Concepts::UnsignedInteger auto petabytes) { this->Bytes += petabytes * Petabyte; return *this; }

	public:
		/// <summary>
		/// Petabytes count
		/// </summary>
		ByteSizeT Property(GetPetabytes, SetPetabytes) Petabytes;

	public:
		/// <summary>
		/// Get as exabytes
		/// </summary>
		/// <returns>Exabytes count</returns>
		constexpr ByteSizeT GetExabytes() const { return Bytes / Exabyte; }

		/// <summary>
		/// Set as exabytes
		/// </summary>
		/// <param name="exabytes">Exabytes count</param>
		constexpr void SetExabytes(Concepts::UnsignedInteger auto exabytes) { this->Bytes = exabytes * Exabyte; }

		/// <summary>
		/// Add exabytes
		/// </summary>
		/// <param name="exabytes">Exabytes count</param>
		constexpr DataSizeT& AddExabytes(Concepts::UnsignedInteger auto exabytes) { this->Bytes += exabytes * Exabyte; return *this; }

	public:
		/// <summary>
		/// Exabytes
		/// </summary>
		ByteSizeT Property(GetExabytes, SetExabytes) Exabytes;

	public:
		/// <summary>
		/// Add data sizes
		/// </summary>
		/// <param name="other">Second operand</param>
		/// <returns>Result value</returns>
		constexpr DataSizeT operator+(DataSizeT other) const { return DataSizeT(this->Bytes + other.Bytes); }

		/// <summary>
		/// Substract data sizes
		/// </summary>
		/// <param name="other">Second operand</param>
		/// <returns>Result value</returns>
		constexpr DataSizeT operator-(DataSizeT other) const { return DataSizeT(this->Bytes - other.Bytes); }

		/// <summary>
		/// Add bytes to data size
		/// </summary>
		/// <param name="other">Second operand</param>
		/// <returns>Result value></returns>
		constexpr DataSizeT operator+(Concepts::UnsignedInteger auto other) const { return DataSizeT(this->Bytes + Bytes); }

		/// <summary>
		/// Substract bytes to data size
		/// </summary>
		/// <param name="other">Second operand</param>
		/// <returns>Result value></returns>
		constexpr DataSizeT operator-(Concepts::UnsignedInteger auto other) const { return DataSizeT(this->Bytes - Bytes); }

		/// <summary>
		/// Add bytes to data size
		/// </summary>
		/// <param name="first">First operand, bytes count</param>
		/// <param name="second">Second operand, data size</param>
		/// <returns>Result value</returns>
		constexpr friend DataSizeT operator+(Concepts::UnsignedInteger auto first, DataSizeT second) { return DataSizeT(first + second.Bytes); }

		/// <summary>
		/// Substract bytes to data size
		/// </summary>
		/// <param name="first">First operand, bytes count</param>
		/// <param name="second">Second operand, data size</param>
		/// <returns>Result value</returns>
		constexpr friend DataSizeT operator-(Concepts::UnsignedInteger auto first, DataSizeT second) { return DataSizeT(first - second.Bytes); }

	public:
		/// <summary>
		/// Add to this value data size
		/// </summary>
		/// <param name="other">Second operand</param>
		/// <returns>Ref to this object</returns>
		constexpr DataSizeT& operator+=(DataSizeT other) { this->Bytes += other.Bytes; return *this; }

		/// <summary>
		/// Substract to this value data size
		/// </summary>
		/// <param name="other">Second operand</param>
		/// <returns>Ref to this object</returns>
		constexpr DataSizeT& operator-=(DataSizeT other) { this->Bytes -= other.Bytes; return *this; }

		/// <summary>
		/// Add to this value bytes
		/// </summary>
		/// <param name="other">Second operand, byte count</param>
		/// <returns>Ref to this object</returns>
		constexpr DataSizeT& operator+=(Concepts::UnsignedInteger auto other) { this->Bytes += other; return *this; }

		/// <summary>
		/// Add to this value bytes
		/// </summary>
		/// <param name="other">Second operand, byte count</param>
		/// <returns>Ref to this object</returns>
		constexpr DataSizeT& operator-=(Concepts::UnsignedInteger auto other) { this->Bytes -= other; return *this; }

	public:
		/// <summary>
		/// Three way comparing
		/// </summary>
		/// <param name="other">Second operand</param>
		/// <returns>Comparing result</returns>
		constexpr auto operator<=>(const DataSizeT& other) const { return this->Bytes <=> other.Bytes; }

		/// <summary>
		/// Equality operator
		/// </summary>
		/// <param name="other">Second operand</param>
		/// <returns>Equality result</returns>
		constexpr auto operator==(const DataSizeT& other) const { return this->Bytes == other.Bytes; }

		/// <summary>
		/// Non equality operator
		/// </summary>
		/// <param name="other">Second operand</param>
		/// <returns>Non equality result</returns>
		constexpr auto operator!=(const DataSizeT& other) const { return this->Bytes != other.Bytes; }

	public:
		constexpr friend DataSizeT operator/(DataSizeT first, DataSizeT second) { return DataSizeT(first.Bytes / second.Bytes); }

		constexpr friend DataSizeT operator/(DataSizeT first, Concepts::UnsignedInteger auto second) { return DataSizeT(first.Bytes / second); }

		constexpr friend DataSizeT operator*(DataSizeT first, DataSizeT second) { return DataSizeT(first.Bytes * second.Bytes); }

		constexpr friend DataSizeT operator*(DataSizeT first, Concepts::UnsignedInteger auto second) { return DataSizeT(first.Bytes * second); }

	public:
		/// <summary>
		/// Explicit convertion to byte count operator
		/// </summary>
		constexpr explicit operator ByteSizeT() { return this->Bytes; }
	};

#pragma region Miscenelious memory function
	/// <summary>
	/// Copying data from source to destination
	/// </summary>
	/// <param name='source'>Copying area of memory</param>
	/// <param name='destination'>Area of memory in which in copying</param>
	/// <param name='size'>Number of bytes which is need to copy</param>
	DeprecatedCall(L"Low-level work with memory") void Copy(
		const void* Restrict source,
		void* Restrict destination,
		DataSizeT size);

	/// <summary>
	/// Moving data from source to destination
	/// </summary>
	/// <param name='source'>Copying area of memory</param>
	/// <param name='destination'>Area of memory in which in copying</param>
	/// <param name='size'>Number of bytes which is need to copy</param>
	DeprecatedCall(L"Low-level work with memory") void Move(
		const void* source,
		void* destination,
		DataSizeT size);

	/// <summary>
	/// Fills the memory block by specified values
	/// </summary>
	/// <param name='memBlk'>Memory block</param>
	/// <param name='size'>The memory area size in bytes</param>
	DeprecatedCall(L"Low-level work with memory") void MemBlkFill(
		void* memBlk,
		DataSizeT Size,
		const Byte Filler);

	/// <summary>
	/// Fills the memory block by specified values
	/// </summary>
	/// <param name='memBlk'>Memory block</param>
	/// <param name='size'>The memory area size in bytes</param>
	/// <param name='filler'>A value which fills the memory area</param>
	DeprecatedCall(L"Low-level work with memory") void SecureMemBlkZero(
		void* memBlk,
		DataSizeT Size);
#pragma endregion
}

#pragma warning(push)
#pragma warning(disable: 4455)
#pragma warning(disable: 4114)
/// <summary>
/// Bytes literal
/// </summary>
/// <param name="size">Bytes count</param>
/// <returns>Data size value</returns>
export ForceInline constexpr Cardinal::Memory::DataSizeT operator "" b(const unsigned long long size) {
	using namespace Cardinal::Memory;
	return DataSizeT(size);
}

/// <summary>
/// Kilobytes literal
/// </summary>
/// <param name="size">Kilobytes count</param>
/// <returns>Data size value</returns>
export ForceInline constexpr Cardinal::Memory::DataSizeT operator "" Kb(const unsigned long long size) {
	using namespace Cardinal::Memory;
	return DataSizeT(size * DataSizeT::Kilobyte);
}

/// <summary>
/// Megabytes literal
/// </summary>
/// <param name="size">Megabytes count</param>
/// <returns>Data size value</returns>
export ForceInline constexpr Cardinal::Memory::DataSizeT operator "" Mb(const unsigned long long size) {
	using namespace Cardinal::Memory;
	return DataSizeT(size * DataSizeT::Megabyte);
}

/// <summary>
/// Gigabytes literal
/// </summary>
/// <param name="size">Gigabytes count</param>
/// <returns>Data size value</returns>
export ForceInline constexpr Cardinal::Memory::DataSizeT operator "" Gb(const unsigned long long size) {
	using namespace Cardinal::Memory;
	return DataSizeT(size * DataSizeT::Gigabyte);
}

/// <summary>
/// Terabytes literal
/// </summary>
/// <param name="size">Terabytes count</param>
/// <returns>Data size value</returns>
export ForceInline constexpr Cardinal::Memory::DataSizeT operator "" Tb(const unsigned long long size) {
	using namespace Cardinal::Memory;
	return DataSizeT(size * DataSizeT::Terabyte);
}

/// <summary>
/// Petabytes literal
/// </summary>
/// <param name="size">Petabytes count</param>
/// <returns>Data size value</returns>
export ForceInline constexpr Cardinal::Memory::DataSizeT operator "" Pb(const unsigned long long size) {
	using namespace Cardinal::Memory;
	return DataSizeT(size * DataSizeT::Petabyte);
}

/// <summary>
/// Exabytes literal
/// </summary>
/// <param name="size">Exabytes count</param>
/// <returns>Data size value</returns>
export ForceInline constexpr Cardinal::Memory::DataSizeT operator "" Eb(const unsigned long long size) {
	using namespace Cardinal::Memory;
	return DataSizeT(size * DataSizeT::Exabyte);
}

/// <summary>
/// Return sizeof as Memory::DataSizeT object
/// </summary>
/// <typeparam name="T">Type for which size will get</typeparam>
/// <returns>DataSizeT object with sizeof for item</returns>
export template<typename T>
ForceInline constexpr Cardinal::Memory::DataSizeT MemDataSizeOf() {
	using namespace Cardinal::Memory;
	return DataSizeT(sizeof(T));
}

#pragma warning(pop)