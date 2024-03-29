export module Cardinal.Functionality:Optional;

import :Base;

export namespace Cardinal::Functionality
{
	/// <summary>
	/// The type with optional presence of value
	/// </summary>
	/// <typeparam name="T">A value type</typeparam>
	template<typename T>
	class Optional final {
	private:
		Boolean isExist;
		Byte data[sizeof(T)] = { 0 };
	
	public:
		/// <summary>
		/// An empty constructor, initializes the object without a value
		/// </summary>
		Optional() : isExist(false) {}

		/// <summary>
		/// Costructor with nullpointer (initializes the object without a value)
		/// </summary>
		Optional(NullptrT) : isExist(false) {}

		/// <summary>
		/// Costructor with the reference to value (initializes the object by the value copy)
		/// </summary>
		/// <param name="data">Constant reference</param>
		Optional(const T& data) : Optional() { SetValue(data); }

		/// <summary>
		/// Constructor with "RValue" reference to the value
		/// </summary>
		/// <param name="data">"RValue" reference to the object</param>
		Optional(T&& data) : Optional() { SetValue(MoveRValue(data)); }
	
	public:
		/// <summary>
		/// Copy ctor
		/// </summary>
		/// <param name="orig">Original</param>
		Optional(const Optional& orig) : Optional() {
			if (orig.HasValue)
			{
				this->SetValue(orig.GetValue());
			}
		}

		/// <summary>
		/// Move ctor
		/// </summary>
		/// <param name="orig">Original</param>
		Optional(Optional&& orig) : Optional() {
			if (orig.HasValue)
			{
				this->SetValue(MoveRValue(MoveRValue(orig).GetValue()));
			}
		}
	
	public:
		/// <summary>
		/// Copy operator
		/// </summary>
		/// <param name="orig">Original</param>
		/// <returns>Reference to current object</returns>
		Optional& operator=(const Optional& orig) {
			this->~Optional();
			this->Optional::Optional(orig);
			return *this;
		}

		/// <summary>
		/// Move operator
		/// </summary>
		/// <param name="orig">Original</param>
		/// <returns>Reference to current object</returns>
		Optional& operator=(Optional&& orig) {
			this->~Optional();
			this->Optional::Optional(MoveRValue(orig));
			return *this;
		}
	
	public:
		/// <summary>
		/// Returns the logical value which indicates if the value exists in object
		/// </summary>
		/// <returns>Indicates whether the value exists in object</returns>
		NoDiscard bool GetHasValue() const { return isExist; }

		/// <summary>
		/// Returns the value of object if it exists
		/// </summary>
		/// <returns>Constant reference to an object</returns>
		const T& GetValue() const& {
			if (!isExist)
				Throw(NullPointException(SourcePosition));

			return *reinterpret_cast<const T* const>(this->data);
		}

		/// <summary>
		/// Returns the value of object if it exists
		/// </summary>
		/// <returns>A reference to an object</returns>
		T& GetValue()& {
			if (!isExist)
				Throw(NullPointException(SourcePosition));

			return *reinterpret_cast<T*>(this->data);
		}

		/// <summary>
		/// Returns the value of object if it exists
		/// </summary>
		/// <returns>"RValue" reference to an object</returns>
		T GetValue()&& {
			if (!isExist)
				Throw(NullPointException(SourcePosition));

			auto value = MoveRValue(*reinterpret_cast<T*>(this->data));
			reinterpret_cast<T*>(this->data)->~T();
			isExist = false;
			return MoveRValue(value);
		}

		/// <summary>
		/// Sets a new object value
		/// </summary>
		/// <param name="data">A new value</param>
		void SetValue(const T& data) {
			if (isExist)
			{
				reinterpret_cast<T*>(this->data)->~T();
			}

			new (this->data) T(data);
			isExist = true;
		}

		/// <summary>
		/// Sets a new object value
		/// </summary>
		/// <param name="data">A new value</param>
		void SetValue(T&& data) {
			if (isExist)
			{
				reinterpret_cast<T*>(this->data)->~T();
			}

			new (this->data) T(MoveRValue(data));
			isExist = true;
		}
	
	public:
		/// <summary>
		/// Indicates whether value exists in object
		/// </summary>
		bool Property(GetHasValue) HasValue;

		/// <summary>
		/// The object value if it exists
		/// </summary>
		T Property(GetValue, SetValue) Value;
	
	public:
		/// <summary>
		/// Equality operator for Optional
		/// </summary>
		/// <param name="second">Right operand</param>
		/// <returns>Equality result</returns>
		bool operator==(const Optional& second) const requires Concepts::HaveEqual<T, T> {
			if (this->isExist != second.isExist)
			{
				return false;
			}
			else if (this->isExist == true)
			{
				return this->Value == second.Value;
			}
			else
			{
				return true;
			}
		}
	
	public:
		~Optional() {
			if (isExist)
			{
				reinterpret_cast<T*>(this->data)->~T();
				isExist = false;
			}
		}
	};
}