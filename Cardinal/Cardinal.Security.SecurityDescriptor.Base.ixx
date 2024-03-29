export module Cardinal.Security.SecurityDescriptor:Base;

export import Cardinal.Core;
export import Cardinal.Memory;
export import Cardinal.String;
export import Cardinal.CTTI;
export import Cardinal.RTTI;
export import Cardinal.Functionality;
export import Cardinal.Exceptions;

export namespace Cardinal::Security
{
	template<typename T>
	using AccessRights = EnumFlags<T>;

	/// <summary>
	/// Base interface of security object
	/// </summary>
	class IBaseSecurableObject {
	public:
		/// <summary>
		/// Returns the name of an object of type
		/// </summary>
		virtual const String& GetTypeName() = 0;

		virtual ~IBaseSecurableObject() {}
	};

	/// <summary>
	/// Interface of security object
	/// </summary>
	/// <typeparam name="T">The object type</typeparam>
	/// <typeparam name="ERights">A list of object rights</typeparam>
	template<typename T, typename ERights> // Requires SecurableObject<T>
	class ISecurableObject : public IBaseSecurableObject {
	private: 
		using Rights = ERights;

	public:
		/// <summary>
		/// Returns the object type
		/// </summary>
		/// <returns>The type record</returns>
		auto GetType() {
			return TypeRecord<T>();
		}

	public:
		constexpr static auto GenericAll = [] () constexpr -> AccessRights<Rights> {
			auto value = AccessRights<Rights>();

			CTTI::TypeTraits::CTTITypeMetaData<Rights>::ItemsMap
				.ForEach(
					[&] (const auto& v) {
						value |= static_cast<Rights>(v.template GetByIdx<0>());
					}
			);

			return value;
		}();
	};

	template<typename T>
	concept SecurableObject =
		Concepts::IsEnum<typename T::ERights>;
}