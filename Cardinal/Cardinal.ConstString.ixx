export module Cardinal.ConstString;

export import Cardinal.Core;
export import Cardinal.Memory.DataSizeT;

export namespace Cardinal
{
	/// <summary>
	/// Constant line which remembers its size
	/// </summary>
	class ConstString final {
	private:
		/// <summary>
		/// Pointer to a text
		/// </summary>
		const Char* str;

		/// <summary>
		/// Its size
		/// </summary>
		SizeT size;

	public:
		/// <summary>
		/// Constant string constructor
		/// </summary>
		/// <param name = "Text"String literal/param>
		/// <param name = "Lenght">Its size</param>
		template<SizeT TLenght>
		ForceInline constexpr ConstString(const wchar_t(&text)[TLenght]) : str(text), size(TLenght) {}

		/// <summary>
		/// Copying constructor
		/// </summary>
		/// <param name = "orig">Last string original</param>
		ForceInline constexpr ConstString(const ConstString& orig) : str(orig.str), size(orig.size) {} //-V730

		///<summary>
		/// Internal constructor from const raw array and lenght
		/// </summary>
		///<param name = "Text">String literal</param>
		///<param name = "length">Its size</param>
		InternalApiCall ForceInline constexpr ConstString(const Char* const text, SizeT length) : str(text), size(length) {}

		/// <summary>
		/// Copying operator
		/// </summary>
		/// <param name="orig">Left operand</param>
		/// <returns>A reference to a current object</returns>
		ForceInline constexpr ConstString& operator=(const ConstString& orig) {
			this->~ConstString();
			this->ConstString::ConstString(orig);
			return *this;
		}

		/// <summary>
		/// Constant string symbol access operator
		/// </summary>
		/// <param name = "Index">Symbol index in string</param>
		ForceInline Char operator[](SizeT index) const;

		/// <summary>
		/// Returns the constant string size
		/// </summary>
		ForceInline constexpr SizeT GetLenght() const { return (this->size == 0) ? 0 : this->size - 1; }

		/// <summary>
		/// Constant string size
		/// </summary>
		SizeT Property(GetLenght) Lenght;

		/// <summary>
		/// Returns the constant array of symbols of that constant string
		/// </summary>
		ForceInline constexpr ConstCharStr GetString() const { return this->str; }

		/// <summary>
		/// Constant array of symbols of that constant string
		/// </summary>
		ConstCharStr Property(GetString) Str;

		/// <summary>
		/// Actual memory size which used by text
		/// </summary>
		ForceInline constexpr Memory::DataSizeT GetSize() const { return Memory::DataSizeT(this->size * sizeof(Char)); }

		/// <summary>
		/// Actual memory size which used by text
		/// </summary>
		Memory::DataSizeT Property(GetSize) Size;

	public:
		/// <summary>
		/// Check equality of two ConstString
		/// </summary>
		/// <param name="str">Second operand</param>
		/// <returns>Logical value</returns>
		constexpr bool operator==(const ConstString& second) const {
			if (size != second.size)
				return false;

			for (SizeT i = 0; i < size; i++)
			{
				if (this->str[i] != second.str[i])
					return false;
			}

			return true;
		}

		/// <summary>
		/// Three way comparing with other ConstString
		/// </summary>
		/// <param name="right">Second operand</param>
		/// <returns>Strong order enumeration value</returns>
		constexpr auto operator<=>(const ConstString& right) const {
			auto szCmp = this->size <=> right.size;
			auto sz = Min(this->size, right.size);

			for (SizeT i = 0; i < sz; i++)
			{
				auto r = this->str[i] <=> right.str[i];
				if (r != 0)
				{
					return r;
				}
			}

			return szCmp;
		}

	public:
		/// <summary>
		/// Check equality with cstr
		/// </summary>
		/// <param name="str">Second operand</param>
		/// <returns>Logical value</returns>
		template<SizeT TN>
		constexpr bool operator==(StaticTextRef<TN> second) const {
			if (size != TN)
				return false;

			for (SizeT i = 0; i < TN; i++)
			{
				if (this->str[i] != second[i])
					return false;
			}

			return true;
		}

		/// <summary>
		/// Three way comparing with cstr
		/// </summary>
		/// <param name="right">Second operand</param>
		/// <returns>Strong order enumeration value</returns>
		template<SizeT TN>
		constexpr auto operator<=>(StaticTextRef<TN> right) const {
			auto szCmp = this->size <=> TN;
			auto sz = Min(this->size, TN);

			for (SizeT i = 0; i < sz; i++)
			{
				auto r = this->str[i] <=> right[i];
				if (r != 0)
				{
					return r;
				}
			}

			return szCmp;
		}
	};

	/// <summary>
	/// Constant "ASCII" string which remembers its size
	/// </summary>
	class ConstAString final { 
	private:
		/// <summary>
		/// Pointer to a text
		/// </summary>
		const AChar* str;

		/// <summary>
		/// Its size
		/// </summary>
		SizeT size;

	public:
		/// <summary>
		/// Constant "ASCII" string constructor
		/// </summary>
		/// <param name = "Text">String literal of "ASCII" symbols</param>
		/// <param name = "Lenght">Its size</param>
		template<SizeT TLenght>
		ForceInline constexpr ConstAString(const char(&Text)[TLenght]) : str(Text), size(TLenght) {}

		/// <summary>
		/// Copying constructor
		/// </summary>
		/// <param name = "orig">Last string original</param>
		ForceInline constexpr ConstAString(const ConstAString& orig) : str(orig.str), size(orig.size) {} //-V730

		/// <summary>
		/// Copying operator
		/// </summary>
		/// <param name="orig">Left operand</param>
		/// <returns>A reference to a current object</returns>
		ForceInline constexpr ConstAString& operator=(const ConstAString& orig) {
			this->~ConstAString();
			this->ConstAString::ConstAString(orig);
			return *this;
		}

		/// <summary>
		/// Access operator to symbols of a constant "ASCII" string
		/// </summary>
		/// <param name = "Index">Symbol index in string</param>
		ForceInline AChar operator[](SizeT Index) const;

		/// <summary>
		/// Returns the constant "ASCII" string size
		/// </summary>
		ForceInline constexpr SizeT GetLenght() const { return (this->size == 0) ? 0 : this->size - 1; }

		/// <summary>
		/// Constant "ASCII" string size
		/// </summary>
		SizeT Property(GetLenght) Lenght;

		/// <summary>
		/// Returns the constant array of symbols of this constant "ASCII" string
		/// </summary>
		ForceInline constexpr ConstACharStr GetString() const { return this->str; }

		/// <summary>
		/// Constant array of symbols of this constant "ASCII" string
		/// </summary>
		ConstACharStr Property(GetString) Str;

		/// <summary>
		/// Actual memory size which used by text
		/// </summary>
		ForceInline constexpr Memory::DataSizeT GetSize() const { return Memory::DataSizeT(this->size * sizeof(AChar)); }

		/// <summary>
		/// Actual memory size which used by text
		/// </summary>
		Memory::DataSizeT Property(GetSize) Size;

	public:
		/// <summary>
		/// Check equality of two ConstString
		/// </summary>
		/// <param name="str">Second operand</param>
		/// <returns>Logical value</returns>
		constexpr bool operator==(const ConstAString& second) const {
			if (size != second.size)
				return false;

			for (SizeT i = 0; i < size; i++)
			{
				if (this->str[i] != second.str[i])
					return false;
			}

			return true;
		}

		/// <summary>
		/// Three way comparing with other ConstString
		/// </summary>
		/// <param name="right">Second operand</param>
		/// <returns>Strong order enumeration value</returns>
		constexpr auto operator<=>(const ConstAString& right) const {
			auto szCmp = this->size <=> right.size;
			auto sz = Min(this->size, right.size);

			for (SizeT i = 0; i < sz; i++)
			{
				auto r = this->str[i] <=> right.str[i];
				if (r != 0)
				{
					return r;
				}
			}

			return szCmp;
		}

	public:
		/// <summary>
		/// Check equality with cstr
		/// </summary>
		/// <param name="str">Second operand</param>
		/// <returns>Logical value</returns>
		template<SizeT TN>
		constexpr bool operator==(StaticATextRef<TN> second) const {
			if (size != TN)
				return false;

			for (SizeT i = 0; i < TN; i++)
			{
				if (this->str[i] != second[i])
					return false;
			}

			return true;
		}

		/// <summary>
		/// Three way comparing with cstr
		/// </summary>
		/// <param name="right">Second operand</param>
		/// <returns>Strong order enumeration value</returns>
		template<SizeT TN>
		constexpr auto operator<=>(StaticATextRef<TN> right) const {
			auto szCmp = this->size <=> TN;
			auto sz = Min(this->size, TN);

			for (SizeT i = 0; i < sz; i++)
			{
				auto r = this->str[i] <=> right[i];
				if (r != 0)
				{
					return r;
				}
			}

			return szCmp;
		}
	};

	/// <summary>
	/// Location in source file
	/// </summary>
	export struct SourceLocation {
		ConstString FileName;
		SizeT Line;
	};
}