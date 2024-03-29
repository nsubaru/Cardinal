export module Cardinal.Serialization:Base;

export import Cardinal.Core;
export import Cardinal.Memory;
export import Cardinal.ConstString;
export import Cardinal.CTTI;
export import Cardinal.String;
export import Cardinal.Text.Tokenizer;
export import Cardinal.Exceptions;

export namespace Cardinal::Serialization
{
	template<typename TSerializeable>
	using ISerializable = CTTI::TypeTraits::ISerializable<TSerializeable>;

	/// <summary>
	/// Unable to serialize or deserialize a given type
	/// </summary>
	class NonSerializableType : public Exception {
	private:
		/// <summary>
		/// The not found type name
		/// </summary>
		ExceptionString typeName;

	public:
		ForceInline NonSerializableType(const NonSerializableType& orig) : Exception(orig) {}
		ForceInline NonSerializableType(NonSerializableType&& from) : Exception(MoveRValue(from)) {}
		ForceInline NonSerializableType& operator=(const NonSerializableType& from) { this->~NonSerializableType(); this->NonSerializableType::NonSerializableType(from); return *this; }
		ForceInline NonSerializableType& operator=(NonSerializableType&& from) { this->~NonSerializableType(); this->NonSerializableType::NonSerializableType(MoveRValue(from)); return *this; }

	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
		/// <param name = 'FunctName'>The function name</param>
		/// <param Name = 'Type'>The type which DI cannot found</param>
		NonSerializableType(SourceLocation sourceLocation, ExceptionString&& typeName)
			:Exception(sourceLocation, L"Unable to serialize a given type"), typeName(MoveRValue(typeName)) {}

		/// <summary>
		/// Text representation of exception information
		/// </summary>
		virtual ExceptionString toString() const override final {
			return ExceptionString::Combine({
				ExceptionString(exceptionName),
				ExceptionString(L". Type name: "), typeName, ExceptionString(L". "),
				location.toString(),
				(innerException == nullptr) ? ExceptionString(L".") : (L". Also there is a nested exception: " + InnerException->toString())
				});
		}

		/// <summary>
		/// Returns the copy of the exception object
		/// </summary>
		/// <returns>A "smart" pointer to an exception object</returns>
		ForceInline virtual ExceptionHolderType Clone() const override {
			return ExceptionHolderType(new NonSerializableType(*this));
		}

		ForceInline virtual ~NonSerializableType() {}
	};
}