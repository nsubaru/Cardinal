export module Cardinal.Text.Tokenizer;

export import Cardinal.Exceptions;
export import Cardinal.Containers.Tuple;
export import Cardinal.String;
export import Cardinal.StringView;

#pragma warning(push)
#pragma warning(disable: 4996)

export namespace Cardinal
{
	/// <summary>
	/// Tokenizer which allow separate text into tokens, can be created from part of another tokenizer
	/// </summary>
	/// <typeparam name="THeap">Memory heap</typeparam>
	template<typename THeap>
	class BaseTokenizer final : public Memory::Allocator<THeap>, public IDisposable {
	private:
		struct Data;

	public:
		class Token;

		/// <summary>
		/// Token view which allow get token text
		/// </summary>
		class TokenView final : public Memory::Allocator<THeap> {
			const Token* This;

		public:
			/// <summary>
			/// Empty ctor
			/// </summary>
			TokenView() : This(nullptr) {}

			/// <summary>
			/// Ctor from token
			/// </summary>
			/// <param name="This"></param>
			TokenView(const Token& This) : This(&This) {}

			/// <summary>
			/// Copy ctor
			/// </summary>
			/// <param name="from">Original</param>
			TokenView(const TokenView& from) : This(from.This) {}

			/// <summary>
			/// Move ctor
			/// </summary>
			/// <param name="from">Original</param>
			TokenView(TokenView&& from) : This(from.This) {}

		public:
			/// <summary>
			/// Copy operator
			/// </summary>
			/// <param name="from">Original</param>
			/// <returns>Refernce to current object</returns>
			TokenView& operator=(const TokenView& from) {
				this->~TokenView();
				this->TokenView::TokenView(from);
			}

			/// <summary>
			/// Move operator
			/// </summary>
			/// <param name="from">Original</param>
			/// <returns>Refernce to current object</returns>
			TokenView& operator=(TokenView&& from) {
				this->~TokenView();
				this->TokenView::TokenView(MoveRValue(from));
			}

		public:
			/// <summary>
			/// Comparing operator which compare token text
			/// </summary>
			/// <param name="second">Second operand</param>
			/// <returns>Equality value</returns>
			auto operator<=>(const TokenView& second) const {
				return BaseStringView<THeap>(this->This->Text->GetCharStr() + this->This->OffSet, this->This->Size) <=> BaseStringView<THeap>(second.This->Text->GetCharStr() + second.This->OffSet, second.This->Size);
			}

		public:
			/// <summary>
			/// Tokwn character access operator
			/// </summary>
			/// <param name="Index">Character index</param>
			/// <returns>Characer value</returns>
			Char operator[](SizeT Index) {
				if (This->Size < Index)
					return *(This->Text->GetCharStr() + This->OffSet + Index);
				else
					Throw(IndexOutOfRange(SourcePosition, Index, This->Size));
			}

			/// <summary>
			/// Tokwn character access operator
			/// </summary>
			/// <param name="Index">Character index</param>
			/// <returns>Characer value</returns>
			Char operator[](SizeT Index) const {
				if (This->Size < Index)
					return *(This->Text->GetCharStr() + This->OffSet + Index);
				else
					Throw(IndexOutOfRange(SourcePosition, Index, This->Size));
			}

		public:
			/// <summary>
			/// Cast to string operator
			/// </summary>
			operator BaseString<THeap>() const {
				return this->GetValue();
			}

			/// <summary>
			/// Return string value of token
			/// </summary>
			/// <returns>String value of token</returns>
			BaseString<THeap> GetValue() const {
				return String(This->Text->GetCharStr() + This->OffSet, This->Size);
			}

			/// <summary>
			/// Return delimiter between that and previous token, if they don't exist exception will be throw
			/// </summary>
			/// <returns>Const reference to delimiter</returns>
			const BaseString<THeap>& GetDelimiter() const {
				if (This->DelimIndex == Math::Max<SizeT>())
					Throw(NullPointException(SourcePosition));

				return This->Text->data->Delims[This->DelimIndex];
			}

			/// <summary>
			/// Size of token value
			/// </summary>
			/// <returns>Token size</returns>
			SizeT GetValueSize() const {
				return This->Size;
			}

			/// <summary>
			/// Offset of tokenizer start
			/// </summary>
			/// <returns>Offset value</returns>
			SizeT GetOffset() const {
				return This->OffSet;
			}

			/// <summary>
			/// Delimiter index
			/// </summary>
			/// <returns>Delimiter index</returns>
			UInt64 GetDelimiterIndex() const {
				return This->DelimIndex;
			}

			/// <summary>
			/// Return availability of delimiter
			/// </summary>
			/// <returns>True, if delimiter available</returns>
			bool GetIsDelimiterAvalilable() const {
				return This->DelimIndex == Math::Max<SizeT>();
			}

		public:
			/// <summary>
			/// String value of token
			/// </summary>
			BaseString<THeap> Property(GetValue) Value;

			/// <summary>
			/// Delimiter string value of token
			/// </summary>
			BaseString<THeap> Property(GetDelimiter) Delimiter;

			/// <summary>
			/// Size of token value in character count
			/// </summary>
			SizeT Property(GetValueSize) ValueSize;

			/// <summary>
			/// Token lenght
			/// </summary>
			SizeT Property(GetValueSize) Lenght;

			/// <summary>
			/// Token start offset
			/// </summary>
			SizeT Property(GetOffset) OffSet;

			/// <summary>
			/// Delimiter index, if it's Max<UInt64> than delimiter not available
			/// </summary>
			UInt64 Property(GetDelimiterIndex) DelimiterIndex;

			/// <summary>
			/// True, if delimiter available
			/// </summary>
			bool Property(GetIsDelimiterAvalilable) IsDelimiterAvalilable;
		};

		/// <summary>
		/// Token object
		/// </summary>
		class Token final : public Memory::Allocator<THeap> {
			BaseTokenizer* Text;
			SizeT OffSet;
			SizeT Size;
			SizeT DelimIndex;

		public:
			friend class BaseTokenizer;
			friend class TokenView;
			friend struct BaseTokenizer::Data;

		private:
			Token(BaseTokenizer* Text, SizeT OffSet, SizeT Size, SizeT DelimIndex)
				:Text(Text), OffSet(OffSet), Size(Size), DelimIndex(DelimIndex) {
			}

		public:
			/// <summary>
			/// Copy ctor
			/// </summary>
			/// <param name="orig">Original</param>
			Token(const Token& orig) : Text(new BaseTokenizer(*orig.Text)), OffSet(orig.OffSet), Size(orig.Size), DelimIndex(orig.DelimIndex) {};

			/// <summary>
			/// Move ctor
			/// </summary>
			/// <param name="from">Original</param>
			Token(Token&& from) : Text(from.Text), OffSet(from.OffSet), Size(from.Size), DelimIndex(from.DelimIndex) { from.Text = nullptr; from.OffSet = from.Size = from.DelimIndex = 0; };

			/// <summary>
			/// Copy operator
			/// </summary>
			/// <param name="orig">Original</param>
			/// <returns>Reference to current object</returns>
			Token& operator=(const Token& orig) {
				this->~Token();
				this->Token::Token(orig);
				return *this;
			}

			/// <summary>
			/// Move operator
			/// </summary>
			/// <param name="orig">Original</param>
			/// <returns>Reference to current object</returns>
			Token& operator=(Token&& from) {
				this->~Token();
				this->Token::Token(MoveRValue(from));
				return *this;
			}

		public:
			/// <summary>
			/// Equality operator
			/// </summary>
			/// <param name="other">Second operand</param>
			/// <returns>True, if tokens equal</returns>
			bool operator==(const Token& other) const {
				return this->Text == other.Text &&
					this->OffSet == other.OffSet &&
					this->Size == other.Size;
			}

			/// <summary>
			/// Nonequality operator
			/// </summary>
			/// <param name="other">Second operand</param>
			/// <returns>True, if tokens not equal</returns>
			bool operator!=(const Token& other) const {
				return this->Text != other.Text ||
					this->OffSet != other.OffSet ||
					this->Size != other.Size;
			}

		public:
			/// <summary>
			/// Token view accessor operator
			/// </summary>
			/// <returns></returns>
			TokenView operator*() {
				return GetView();
			}

			/// <summary>
			/// Return token view
			/// </summary>
			/// <returns>Token view object</returns>
			TokenView GetView() {
				if (Text != nullptr)
					return TokenView(*this);
				else
					Throw(NullPointException(SourcePosition, 0, NullPointException::Types::Read));
			}

		public:
			/// <summary>
			/// Move to next token
			/// </summary>
			/// <returns>Reference to current token</returns>
			Token& operator++() {
				if (this->OffSet + this->Size == this->Text->GetCharStrSize())
				{
					delete this->Text;
					this->Text = nullptr;
					this->DelimIndex = this->OffSet = this->Size = -1;
					return *this;
				}

				auto [NewOffSet, NewSize, NewDelimIndex] =
					BaseTokenizer::GetNextToken(
						this->Text->GetCharStr(),
						this->Text->GetCharStrSize(),
						(CharStr*) (const CharStr*) this->Text->data->DelimsRaw,
						(SizeT*) (const SizeT*) this->Text->data->DelimsSize,
						this->Text->data->Delims.Count,
						this->OffSet + this->Size +
						((this->DelimIndex == Math::Max<SizeT>())
							? 1
							: this->Text->data->DelimsSize[this->DelimIndex])
					);

				if (NewOffSet == 0 && NewSize == 0)
				{
					delete this->Text;
					this->Text = nullptr;
					this->DelimIndex = this->OffSet = this->Size = -1;
				}
				else
				{
					this->OffSet = NewOffSet;
					this->Size = NewSize;
					this->DelimIndex = NewDelimIndex;
				}

				return *this;
			}

		public:
			~Token() {
				delete Text;
				Text = nullptr;
				OffSet = Size = DelimIndex = 0;
			}

		public:
			TokenView Property(GetView) View;
		};

	public:
		friend class Token;
		friend class TokenView;

	private:
		struct Data : Memory::Allocator<THeap> {
			union {
				struct ParentTokenData {
					Token ParentToken;
					CharStr RawText;
					SizeT RawTextSize;

					ParentTokenData(const Token& ParentToken, CharStr RawText, SizeT RawTextSize)
						: ParentToken(ParentToken), RawText(RawText), RawTextSize(RawTextSize) {}
				};

				ParentTokenData ParentToken;
				BaseString<THeap> Text;
			};

			enum class DataTypes {
				ParentToken, String
			};

			DataTypes DataType;

			Containers::Array<BaseString<THeap>> Delims;
			Containers::Array<CharStr> DelimsRaw;
			Containers::Array<SizeT> DelimsSize;

			RefCountT RefCount;

			Data(const String& Text, const Containers::Array<BaseString<THeap>>& Delims) :
				DataType(DataTypes::String),
				Text(Text), Delims(Delims),
				DelimsRaw(Delims.Count),
				DelimsSize(Delims.Count),
				RefCount(1) {
				LoadCachesData();
			}

			Data(String&& Text, const Containers::Array<BaseString<THeap>>& Delims) :
				DataType(DataTypes::String),
				Text(MoveRValue(Text)), Delims(Delims),
				DelimsRaw(Delims.Count),
				DelimsSize(Delims.Count),
				RefCount(1) {
				LoadCachesData();
			}

			Data(const Token& ParentToken, const Containers::Array<BaseString<THeap>>& Delims) :
				DataType(DataTypes::ParentToken),
				ParentToken(ParentToken, ParentToken.Text->GetCharStr(), ParentToken.Text->GetCharStrSize()),
				Delims(Delims),
				DelimsRaw(Delims.Count),
				DelimsSize(Delims.Count),
				RefCount(1) {
				LoadCachesData();
			}

			void LoadCachesData() {
				for (SizeT i = 0; i < Delims.Count; i++)
				{
					DelimsRaw[i] = const_cast<CharStr>(static_cast<ConstCharStr>(Delims[i]));
					DelimsSize[i] = Delims[i].Lenght;
				}
			}

			~Data() {
				switch (DataType)
				{
					case DataTypes::ParentToken:
					this->ParentToken.~ParentTokenData();
					break;
					case DataTypes::String:
					this->Text.~BaseString();
					break;
				}
			}
		};

		Data* data;

	protected:
		virtual void Dispose() override {}

	private:
		static Containers::Tuple<SizeT, SizeT, SizeT> GetNextToken(const CharStr Text, SizeT Size, CharStr* Delims, SizeT* DelimSizes, SizeT DelimsCount, SizeT StartSize) {
			for (SizeT i = StartSize; i < Size; i++)
			{
				for (SizeT j = 0; j < DelimsCount; j++)
				{
					if (Text[i] == Delims[j][0] && Size - i >= DelimSizes[j])
					{
						SizeT Pos = i;
						Boolean Found = true;

						if (DelimSizes[j] > 1)
						{
							for (i; i < Pos + DelimSizes[j]; i++)
							{
								if (Text[i] != Delims[j][i - Pos])
								{
									Found = false;
									break;
								}
							}
						}

						if (Found)
							return Containers::Tuple<SizeT, SizeT, SizeT>(StartSize, i - StartSize, j);
						else
							i = Pos;
					}
				}
			}
			return Containers::Tuple<SizeT, SizeT, SizeT>(StartSize, Size - StartSize, Cardinal::Math::Max<SizeT>());
		}

	private:
		CharStr GetCharStr() {
			switch (data->DataType)
			{
				case Data::DataTypes::String:
				return const_cast<CharStr>(static_cast<ConstCharStr>(data->Text));
				case Data::DataTypes::ParentToken:
				return data->ParentToken.RawText;
			}
			__assume(false);
		}

		SizeT GetCharStrSize() {
			switch (data->DataType)
			{
				case Data::DataTypes::String:
				return data->Text.Lenght;
				case Data::DataTypes::ParentToken:
				return data->ParentToken.RawTextSize;
			}
			__assume(false);
		}

	public:
		/// <summary>
		/// Constructor from text and delimiters
		/// </summary>
		/// <param name="Text">String with tokenizer text</param>
		/// <param name="Delims">Array of delimiters</param>
		BaseTokenizer(const String& Text, const Containers::Array<BaseString<THeap>>& Delims) : data(new Data(Text, Delims)) {}

		/// <summary>
		/// Constructor from text and delimiters
		/// </summary>
		/// <param name="Text">String with tokenizer text</param>
		/// <param name="Delims">Array of delimiters</param>
		BaseTokenizer(String&& Text, const Containers::Array<BaseString<THeap>>& Delims) : data(new Data(Text, Delims)) {}

		/// <summary>
		/// Constructor from parrent token and delimiters
		/// </summary>
		/// <param name="Text">Token from another tokenizer</param>
		/// <param name="Delims">Array of delimiters</param>
		BaseTokenizer(const Token& ParentToken, const Containers::Array<BaseString<THeap>>& Delims) : data(new Data(ParentToken, Delims)) {}

	public:
		/// <summary>
		/// Return first token
		/// </summary>
		/// <returns>First token</returns>
		Token begin() {
			auto [OffSet, Size, DelimIndex] =
				GetNextToken(
					GetCharStr(),
					GetCharStrSize(),
					(CharStr*) (const CharStr*) data->DelimsRaw,
					(SizeT*) (const SizeT*) data->DelimsSize,
					data->Delims.Count, 0
				);
			if (OffSet == 0 && Size == 0 && DelimIndex == 0)
				return Token(nullptr, -1, -1, -1);
			else
				return Token(new BaseTokenizer(*this), OffSet, Size, DelimIndex);
		}

		/// <summary>
		/// Return invalid token
		/// </summary>
		/// <returns>Invalid token</returns>
		Token end() {
			return Token(nullptr, Math::Max<SizeT>(), Math::Max<SizeT>(), Math::Max<SizeT>());
		}

	public:
		RefCountClass(BaseTokenizer, data);

	public:
		friend class Token;
	};

	/// <summary>
	/// Tokenizer which allow separate text into tokens, can be created from part of another tokenizer
	/// </summary>
	using Tokenizer = BaseTokenizer<Memory::Heap>;

	/// <summary>
	/// Sensitive text tokenizer which allow separate text into tokens, can be created from part of another tokenizer
	/// </summary>
	using SensitiveTokenizer = BaseTokenizer<Memory::SensitiveDataHeap>;
}

#pragma warning(pop)