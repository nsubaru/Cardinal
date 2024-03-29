export module Cardinal.Crypt:Hash.SHA256;

import :Hash.Base;

#pragma warning(push)
// This internal implementation file which can use internal API functions
#pragma warning(disable: 4996)

export namespace Cardinal::Crypt::Hash
{
	/// <summary>
	/// The hash object by "SHA256" algoritm 
	/// </summary>
	class SHA256Hash final : public IHashProvider {
	public:
		/// <summary>
		/// The hash size in bits
		/// </summary>
		static constexpr SizeT HashBitsSize = 256;

		/// <summary>
		/// The hash size in bytes
		/// </summary>
		static constexpr SizeT HashSize = HashBitsSize / 8;

		/// <summary>
		/// The resulted hash type
		/// </summary>
		using THash = Containers::SensitiveFixedArray<HashSize, Byte>;

	private:
		struct Data {
			Byte data[64];
			UInt32 state[8];
			UInt64 bitlen;
			UInt64 datalen;

			RefCountT RefCount;

			Data() : data(), state(), bitlen(), datalen(), RefCount(1) {}
		};

	private:
		Data* This;

	protected:
		virtual void Dispose() {}

	public:
		/// <summary>
		/// An empty constructor (initializes the empty hash object)
		/// </summary>
		SHA256Hash();

		/// <summary>
		/// The constructor with data block (immediately calculates the hash value)
		/// </summary>
		/// <typeparam name="THeap">The array memory heap type</typeparam>
		/// <param name="block">The data block</param>
		template<typename THeap>
		SHA256Hash(const Cardinal::Containers::BaseArray<THeap, Byte>& block) : SHA256Hash() {
			this->AddBlock(block);
		}

		/// <summary>
		/// The constructor with data block (immediately calculates the hash value)
		/// </summary>
		/// <typeparam name="THeap">The array memory heap type</typeparam>
		/// <param name="block">The data block</param>
		template<typename THeap, SizeT TSize>
		SHA256Hash(const Containers::BaseFixedArray<THeap, TSize, Byte>& block) : SHA256Hash() {
			this->AddBlock(block);
		}

	public:
		/// <summary>
		/// Data block addition to hash
		/// </summary>
		/// <typeparam name="THeap">The data block memory heap type</typeparam>
		/// <param name="block">The data block</param>
		/// <returns>A reference to an object</returns>
		template<typename THeap>
		SHA256Hash& AddBlock(const Containers::BaseArray<THeap, Byte>& block) & {
			const Byte* const buffer = static_cast<const Byte* const>(block);
			SizeT bytesCount = block.Count;

			UpdateHash(buffer, bytesCount);

			return *this;
		}

		/// <summary>
		/// Data block addition to hash
		/// </summary>
		/// <typeparam name="THeap">The data block memory heap type</typeparam>
		/// <param name="block">The data block</param>
		/// <returns>A reference to an object</returns>
		template<typename THeap, SizeT TSize>
		SHA256Hash& AddBlock(const Containers::BaseFixedArray<THeap, TSize, Byte>& block) & {
			UpdateHash(block);

			return *this;
		}

	public:
		/// <summary>
		/// Data block addition to hash
		/// </summary>
		/// <typeparam name="THeap">The data block memory heap type</typeparam>
		/// <param name="block">The data block</param>
		/// <returns>A reference to an object</returns>
		template<typename THeap>
		SHA256Hash&& AddBlock(const Containers::BaseArray<THeap, Byte>& block)&& {
			const Byte* const buffer = static_cast<const Byte* const>(block);
			SizeT bytesCount = block.Count;

			UpdateHash(buffer, bytesCount);

			return MoveRValue(*this);
		}

		/// <summary>
		/// Data block addition to hash
		/// </summary>
		/// <typeparam name="THeap">The data block memory heap type</typeparam>
		/// <param name="block">The data block</param>
		/// <returns>A reference to an object</returns>
		template<typename THeap, SizeT TSize>
		SHA256Hash&& AddBlock(const Containers::BaseFixedArray<THeap, TSize, Byte>& block)&& {
			UpdateHash(block);

			return MoveRValue(*this);
		}

	private:
		template<typename THeap, SizeT TSize>
		void UpdateHash(const Containers::BaseFixedArray<THeap, TSize, Byte>& block) {
			decltype(auto) data = static_cast<const Byte(&)[TSize]>(block);

			for (SizeT i = 0; i < block.Count; ++i)
			{
				This->data[This->datalen] = data[i];
				This->datalen++;
				if (This->datalen == 64)
				{
					Transform();
					This->bitlen += 512;
					This->datalen = 0;
				}
			}
		}
	private:
		void Transform();
		void UpdateHash(const Byte* data, SizeT len);

	public:
		/// <summary>
		/// Returns the hash as a fixed array
		/// </summary>
		/// <returns>Hash value</returns>
		THash GetFixedHash() &;

		/// <summary>
		/// Returns the hash as a fixed array
		/// </summary>
		/// <returns>Hash value</returns>
		THash GetFixedHash() &&;

	public:
		/// <summary>
		/// Returns the hash as a dynamic array
		/// </summary>
		/// <returns>Hash value</returns>
		Containers::SensitiveBlob GetHash() & override
		{
			return GetFixedHash().CopyTo<Containers::SensitiveArray>();
		}

		/// <summary>
		/// Returns the hash as a dynamic array
		/// </summary>
		/// <returns>Hash value</returns>
		Containers::SensitiveBlob GetHash() && override
		{
			return MoveRValue(*this).GetFixedHash().CopyTo<Containers::SensitiveArray>();
		}

	public:
		RefCountClass(SHA256Hash, This)
	};
}

#pragma warning(pop)