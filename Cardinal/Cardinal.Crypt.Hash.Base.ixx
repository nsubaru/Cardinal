export module Cardinal.Crypt:Hash.Base;

import :Base;

export namespace Cardinal::Crypt::Hash
{
	/// <summary>
	/// The hash algoritm provider interface
	/// </summary>
	class IHashProvider : public IDisposable {
	public:
		/// <summary>
		/// Returns the hash and allows to continue data hashing
		/// </summary>
		/// <returns>Hash value</returns>
		virtual Containers::SensitiveBlob GetHash() & = 0;

		/// <summary>
		/// Returns the hash without the possibility to continue data hashing
		/// </summary>
		/// <returns>Hash value</returns>
		virtual Containers::SensitiveBlob GetHash() && = 0;

	public:
		virtual ~IHashProvider() {}
	};
}