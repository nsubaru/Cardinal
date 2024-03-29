export module Cardinal.StringView;

export import :Base;
export import :Convertors;
export import :LINQ;

import Cardinal.Core.Internals;
import Cardinal.Exceptions.Internals;
import Cardinal.Time.Internals;

#pragma warning(push)
// This internal implementation file which can use internal API functions
#pragma warning(disable: 4996)

export namespace Cardinal
{
	/// <summary>
	/// Reference to a constant string (allows to work abstractly with part of the string)
	/// </summary>
	/// <typeparam name="Heap">Memory heap type</typeparam>
	template<typename Heap>
	class BaseStringView : 
		public Memory::Allocator<Heap>, 
		public Details::TConvertorsStringViewPart<Heap, BaseStringView>,
		public Details::TLinqStringViewPart<Heap, BaseStringView>
	{
	private:
		/// <summary>
		/// Reference to a string
		/// </summary>
		const BaseString<Heap>* refStr;

		/// <summary>
		/// Offset from the actual beginning of the text in the number of symbols
		/// </summary>
		SizeT offset;

		/// <summary>
		/// String part lenght that is visible in this object
		/// </summary>
		SizeT len;
	
	public:
		/// <summary>
		/// Empty ctore create invalid string view
		/// </summary>
		ForceInline BaseStringView() : refStr(nullptr), offset(0), len(0) {}

		/// <summary>
		/// Constructor with reference to a a string and possibly on the beggining and end of given part
		/// </summary>
		/// <param name="refStr">Reference to a string</param>
		/// <param name="offset">Offset</param>
		/// <param name="len">Part length</param>
		ForceInline BaseStringView(const BaseString<Heap>& refStr, SizeT offset = 0, SizeT len = Math::Max<SizeT>()) 
			: refStr(&refStr), offset(offset), len(len) {}

		/// <summary>
		/// Constructor
		/// </summary>
		ForceInline BaseStringView(const BaseStringView& orig) : refStr(orig.refStr), offset(orig.offset), len(orig.len) {}

		/// <summary>
		/// Assigning operator
		/// </summary>
		ForceInline BaseStringView& operator=(const BaseStringView& from) {
			this->~BaseStringView();
			this->BaseStringView::BaseStringView(from);
			return *this;
		}

		/// <summary>
		/// Move data from a reference to a new object
		/// </summary>
		/// <param name="from">Original</param>
		ForceInline BaseStringView(BaseStringView&& from) : refStr(from.refStr), offset(from.offset), len(from.len) {}

		/// <summary>
		/// Moving operator from the reference to a new object
		/// </summary>
		/// <param name="from">Original</param>
		/// <returns>A reference to a current object</returns>
		ForceInline BaseStringView& operator=(BaseStringView&& from) {
			this->~BaseStringView();
			this->BaseStringView::BaseStringView(MoveRValue(from));
		}
	
	public:
		/// <summary>
		/// Comparing with a string
		/// </summary>
		/// <param name="other">Line</param>
		/// <returns>"True" if equal</returns>
		ForceInline bool operator==(const BaseString<Heap>& other) const {
			return (*this <=> other) == 0;
		}

		/// <summary>
		/// Comparing with a string
		/// </summary>
		/// <param name="other">Line</param>
		/// <returns>"False" if equal</returns>
		ForceInline bool operator!=(const BaseString<Heap>& other) const {
			return !(*this == other);
		}

		/// <summary>
		/// Comparing with a string
		/// </summary>
		/// <param name="first">String representation</param>
		/// <param name="second">Line</param>
		/// <returns>"True" if equal</returns>
		ForceInline friend bool operator==(const BaseString<Heap>& first, const BaseStringView& second) {
			return second == first;
		}

		/// <summary>
		/// Comparing with a string
		/// </summary>
		/// <param name="first">String representation</param>
		/// <param name="second">Line</param>
		/// <returns>"True" if not equal</returns>
		ForceInline friend bool operator!=(const BaseString<Heap>& first, const BaseStringView& second) {
			return second != first;
		}
	
	public:
		/// <summary>
		/// Comparing with a string
		/// </summary>
		/// <param name="first">String representation</param>
		/// <param name="second">Line</param>
		/// <returns>"True" if equal</returns>
		template<typename TOtherHeap>
		ForceInline friend bool operator==(const BaseString<Heap>& first, const BaseStringView<TOtherHeap>& second) {
			return (second <=> first) == 0;
		}

		/// <summary>
		/// Comparing with a string
		/// </summary>
		/// <param name="first">String representation</param>
		/// <param name="second">Line</param>
		/// <returns>"True" if not equal</returns>
		template<typename TOtherHeap>
		ForceInline friend bool operator!=(const BaseString<Heap>& first, const BaseStringView<TOtherHeap>& second) {
			return (second != first) == 0;
		}

		/// <summary>
		/// Universal comparing
		/// </summary>
		/// <param name="first">First element</param>
		/// <param name="second">Second element</param>
		/// <returns>Comparing result</returns>
		ForceInline friend StrongOrdering operator<=>(const BaseString<Heap>& first, const BaseStringView& second) {
			auto comparableLen = Min(first.Lenght, second.ActualViewLen);
			for (SizeT i = 0; i < comparableLen; i++)
			{
				if (first[i] != (*second.refStr)[second.offset + i])
					return first[i] <=> (*second.refStr)[second.offset + i];
			}
			return first.Lenght <=> second.ActualViewLen;
		}

		/// <summary>
		/// Universal comparing
		/// </summary>
		/// <param name="first">First element</param>
		/// <param name="second">Second element</param>
		/// <returns>Comparing result</returns>
		ForceInline friend StrongOrdering operator<=>(const BaseStringView& first, const BaseString<Heap>& second) {
			auto comparableLen = Min(first.ActualViewLen, second.Lenght);
			for (SizeT i = 0; i < comparableLen; i++)
			{
				if ((*first.refStr)[first.offset + i] != second[i])
					return (*first.refStr)[first.offset + i] <=> second[i];
			}
			return first.ActualViewLen <=> second.Lenght;
		}

	public:
		/// <summary>
		/// Universal comparing
		/// </summary>
		/// <param name="first">First element</param>
		/// <param name="second">Second element</param>
		/// <returns>Comparing result</returns>
		template<typename TOtherHeap>
		friend StrongOrdering operator<=>(const BaseStringView& first, const BaseStringView<TOtherHeap>& second) {
			auto comparableLen = Min(first.ActualViewLen, second.ActualViewLen);
			for (SizeT i = 0; i < comparableLen; i++)
			{
				if ((*first.refStr)[first.offset + i] != (*second.refStr)[second.offset + i])
					return (*first.refStr)[first.offset + i] <=> (*second.refStr)[second.offset + i];
			}
			return first.ActualViewLen <=> second.ActualViewLen;
		}

		/// <summary>
		/// Universal comparing
		/// </summary>
		/// <param name="first">First element</param>
		/// <param name="second">Second element</param>
		/// <returns>Comparing result</returns>
		friend StrongOrdering operator<=>(const BaseStringView& first, const BaseStringView& second) {
			auto comparableLen = Min(first.ActualViewLen, second.ActualViewLen);
			for (SizeT i = 0; i < comparableLen; i++)
			{
				if ((*first.refStr)[first.offset + i] != (*second.refStr)[second.offset + i])
					return (*first.refStr)[first.offset + i] <=> (*second.refStr)[second.offset + i];
			}
			return first.ActualViewLen <=> second.ActualViewLen;
		}

	public:
		/// <summary>
		/// Comparing with a string
		/// </summary>
		/// <param name="other">Line</param>
		/// <returns>"True" if equal</returns>
		ForceInline bool operator==(const BaseStringView& other) const {
			return (*this <=> other) == 0;
		}

		/// <summary>
		/// Comparing with a string
		/// </summary>
		/// <param name="other">Line</param>
		/// <returns>"False" if equal</returns>
		ForceInline bool operator!=(const BaseStringView& other) const {
			return !(*this == other);
		}

	public:
		/// <summary>
		/// Symbol receiving operator within "Offset - Offset + ViewLen". If the index is equal to "Offset + ViewLen", then "zero terminator" is returned
		/// </summary>
		/// <param name="Index">Symbol index</param>
		/// <returns>The letter that is on the position</returns>
		ForceInline Char operator[](SizeT Index) const {
			if (offset + Index > offset + len && offset + Index > refStr->Lenght)
			{
				Cardinal::Internals::Exceptions::ThrowIndexOutOfRange(SourcePosition, offset + Index, refStr->Lenght - len);
			}
			else if(offset + Index == refStr->Lenght + offset)
			{
				return '\0';
			}

			return (*refStr)[offset + Index];
		}

		/// <summary>
		/// Returns a reference to the string on which the reference is based
		/// </summary>
		/// <returns>Reference to a string</returns>
		ForceInline const BaseString<Heap>& GetFullText() const {
			return *refStr;
		}

		/// <summary>
		/// Returns the offset
		/// </summary>
		/// <returns>Offset</returns>
		ForceInline SizeT GetOffset() const {
			return offset;
		}

		/// <summary>
		/// Sets new offset
		/// </summary>
		/// <param name="newOffset">New offset</param>
		ForceInline void SetOffset(SizeT newOffset) {
			offset = newOffset; if (offset > refStr->Lenght) len = 0;
		}

		/// <summary>
		/// Returns the reference limit
		/// </summary>
		/// <returns>String part length</returns>
		ForceInline SizeT GetViewLen() const {
			return len;
		}

		/// <summary>
		/// Sets new reference limit
		/// </summary>
		/// <param name="newViewLen">New reference limit</param>
		ForceInline void SetViewLen(SizeT newViewLen) {
			len = newViewLen;
		}

		/// <summary>
		/// String receiving from the display
		/// </summary>
		/// <returns>String converted to string</returns>
		template<typename UHeap = Heap> 
		ForceInline BaseString<UHeap> ToString() const {
			// Use internal string ctor
#pragma warning(push)
#pragma warning(disable: 4996)
			return BaseString<UHeap>(((const CharStr) refStr->operator ConstCharStr()) + offset, ActualViewLen);
#pragma warning(pop)
		}

		/// <summary>
		/// String receiving from the display
		/// </summary>
		/// <returns>String text</returns>
		ForceInline BaseString<Heap> GetText() const {
			return this->ToString();
		}

		/// <summary>
		/// Actual text
		/// </summary>
		/// <returns>String length</returns>
		ForceInline SizeT GetActualViewLen() const {
			if (refStr == nullptr)
			{
				return 0;
			} 
			else if (offset > Math::Max<SizeT>() || offset >= refStr->Lenght)
			{
				return 0;
			}
			else if(len == Math::Max<SizeT>())
			{
				return refStr->Lenght - offset;
			}
			else if(len > refStr->Lenght || len + offset > refStr->Lenght)
			{
				return refStr->Lenght - offset;
			}
			else
			{
				return len;
			}
		}

		/// <summary>
		/// Display string
		/// </summary>
		BaseString<Heap> Property(GetText) Text;

		/// <summary>
		/// Refered string
		/// </summary>
		BaseString<Heap> Property(GetFullText) FullText;

		/// <summary>
		/// Offset
		/// </summary>
		SizeT Property(GetOffset, SetOffset) Offset;

		/// <summary>
		/// Reference limit
		/// </summary>
		SizeT Property(GetViewLen, SetViewLen) ViewLen;

		/// <summary>
		/// Lenght of viewable string
		/// </summary>
		SizeT Property(GetActualViewLen) Lenght;

		/// <summary>
		/// Actual display size
		/// </summary>
		SizeT Property(GetActualViewLen) ActualViewLen;
		
	public:
		/// <summary>
		/// Returns if visible string is empty
		/// </summary>
		bool GetIsEmpty() { return ActualViewLen == 0; }

		/// <summary>
		/// "True" if visible string is empty
		/// </summary>
		bool Property(GetIsEmpty) IsEmpty;

	public:
		~BaseStringView() {
			this->refStr = nullptr;
			this->len = 0;
			this->offset = 0;
		}

	public:
		template<typename TOtherHeap>
		friend class BaseStringView;
			
		template<typename THeap, template<typename THeap> typename TStringView>
		friend class Details::TConvertorsStringViewPart;

		template<typename THeap, template<typename THeap> typename TStringView>
		friend class Details::TLinqStringViewPart;
	};

	/// <summary>
	/// Base reference to a a string
	/// </summary>
    using StringView = BaseStringView<Memory::Heap>;

    /// <summary>
	/// Reference to a string with confidential information
	/// </summary>
    using SensitiveStringView = BaseStringView<Memory::SensitiveDataHeap>;
}

#pragma warning(pop)