export module Cardinal.Crypt:HMAC;

import :Base;
import :Hash;

export namespace Cardinal::Crypt
{
	/// <summary>
	/// The message signature generation algoritm using hash algoritm
	/// </summary>
	/// <typeparam name="THashProvider">Hash provider</typeparam>
	template<typename THashProvider> requires Concepts::IsBasedOn<Hash::IHashProvider, THashProvider>
	class HMAC {
	public:
		/// <summary>
		/// The size of resulted identifier
		/// </summary>
		static constexpr SizeT HashSize = THashProvider::HashSize;

		/// <summary>
		/// The type of resulted hashing
		/// </summary>
		using THash = typename THashProvider::THash;

	private:
		/// <summary>
		/// Hash provider
		/// </summary>
		THashProvider hashProvider;

		/// <summary>
		/// Hash key
		/// </summary>
		THash key;

	public:
		/// <summary>
		/// The constructor with key initialization
		/// </summary>
		/// <param name="key">The key in a binary appearance</param>
		HMAC(THash&& key) : 
			hashProvider(), 
			key(MoveRValue(key))
		{ 
			// Inner padded key
			auto ikeypad = key.CopyToFixed<Containers::SensitiveFixedArray>([] (const Byte& x) { return (Byte) (x ^ 0x36); });

			hashProvider.AddBlock(ikeypad);
		}

		/// <summary>
		/// The constructor with key initialization
		/// </summary>
		/// <param name="key">The key in a binary appearance</param>
		HMAC(const THash& key) : hashProvider(), key(key) {
			// Inner padded key
			auto ikeypad = key.CopyToFixed<Containers::SensitiveFixedArray>([] (const Byte& x) { return (Byte) (x ^ 0x36); });

			hashProvider.AddBlock(ikeypad);
		}

	public:
		/// <summary>
		/// Data block addition
		/// </summary>
		/// <typeparam name="THeap">The heap type in data block</typeparam>
		/// <param name="blob">The data block</param>
		/// <returns>A reference to an object</returns>
		template<typename THeap>
		HMAC& AddBlock(const Containers::BaseArray<THeap, Byte>& blob) & {
			hashProvider.AddBlock(blob);
			return *this;
		}

		/// <summary>
		/// Fixed sized data block addition
		/// </summary>
		/// <typeparam name="THeap">The heap type in data block</typeparam>
		/// <param name="blob">The data block</param>
		/// <returns>A reference to an object</returns>
		template<typename THeap, SizeT TSize>
		HMAC& AddBlock(const Containers::BaseFixedArray<THeap, TSize, Byte>& blob) & {
			hashProvider.AddBlock(blob);
			return *this;
		}

	public:
		/// <summary>
		/// Data block addition
		/// </summary>
		/// <typeparam name="THeap">The heap type in data block</typeparam>
		/// <param name="blob">The data block</param>
		/// <returns>A reference to an object</returns>
		template<typename THeap>
		HMAC&& AddBlock(const Containers::BaseArray<THeap, Byte>& blob)&& {
			hashProvider.AddBlock(blob);
			return MoveRValue(*this);
		}

		/// <summary>
		/// Fixed sized data block addition
		/// </summary>
		/// <typeparam name="THeap">The heap type in data block</typeparam>
		/// <param name="blob">The data block</param>
		/// <returns>A reference to an object</returns>
		template<typename THeap, SizeT TSize>
		HMAC&& AddBlock(const Containers::BaseFixedArray<THeap, TSize, Byte>& blob)&& {
			hashProvider.AddBlock(blob);
			return MoveRValue(*this);
		}

	public:
		/// <summary>
		/// The "HMAC" signature generation
		/// </summary>
		/// <returns>The data block with hash which is used for signature check</returns>
		THash Generate() & {
			// Generate hash as pseudocoded function:
			//  "hash(outer_key || hash(inner_key || data))"
			//	where '||' operator combine data
			//
			// Inner key we combine in ctor by adding it before any other data
			// Outer key we combine in this method as it's require data hash
			return THashProvider().AddBlock(Containers::Combine(
				// Outer padded key
				key.CopyToFixed<Containers::SensitiveFixedArray>([] (const Byte& x) -> Byte InlineLambda { return x ^ 0x5c; }),
				// Data hash
				hashProvider.GetFixedHash()
			)).GetFixedHash();
		}

		/// <summary>
		/// The "HMAC" signature generation
		/// </summary>
		/// <returns>The data block with hash which is used for signature check</returns>
		THash Generate()&& {
			// Generate hash as pseudocoded function:
			//  "hash(outer_key || hash(inner_key || data))"
			//	where '||' operator combine data
			//
			// Inner key we combine in ctor by adding it before any other data
			// Outer key we combine in this method as it's require data hash
			return THashProvider().AddBlock(Containers::Combine(
				//Outer padded key
				key.CopyToFixed<Containers::SensitiveFixedArray>([] (const Byte& x) -> Byte InlineLambda { return x ^ 0x5c; }),
				//Data hash
				MoveRValue(hashProvider).GetFixedHash()
			)).GetFixedHash();
		}
	};

	/// <summary>
	/// A standard "HMAC" algoritm realization using "SHA-256" hash
	/// </summary>
	using HmacSha256 = HMAC<Hash::SHA256Hash>;
}