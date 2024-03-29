export module Cardinal.Text.Path;

export import Cardinal.String;
export import Cardinal.StringView;

import Cardinal.Exceptions.Internals;

#pragma warning(push)
#pragma warning(disable: 4996)
#pragma warning(disable: 4595)

export namespace Cardinal::Text
{
	/// <summary>
	/// Abstract path object
	/// </summary>
	/// <typeparam name="THeap">A heap of data</typeparam>
	/// <typeparam name="TPathDelimiter">Path components delimiter</typeparam>
	template<typename THeap, Char TPathDelimiter>
	class BasePath : public IDisposable {
	public:
		/// <summary>
		/// Path components delimiter
		/// </summary>
		constexpr static Char Delimiter = TPathDelimiter;

		/// <summary>
		/// Text path type
		/// </summary>
		using TString = BaseString<THeap>;

	private:
		constexpr static Char DelimiterStr[] = { TPathDelimiter , L'\0' };

	private:
		struct Data {
			TString path;
			RefCountT RefCount;

			ForceInline Data(const TString& path) : path(path), RefCount(1) {}
			ForceInline Data(TString&& path) : path(MoveRValue(path)), RefCount(1) {}
		};

	private:
		Data* This;

	protected:
		ForceInline virtual void Dispose() {}

	public:
		/// <summary>
		/// A path iterator that allows to walk alternately on each component of the path
		/// </summary>
		class PathIterator {
		private:
			SizeT offset;
			SizeT len;
			BasePath path;

		private:
			ForceInline PathIterator(const BasePath& path, SizeT offset, SizeT len) : path(path), offset(offset), len(len) {}
			ForceInline PathIterator(BasePath&& path, SizeT offset, SizeT len) : path(MoveRValue(path)), offset(offset), len(len) {}

		public:
			/// <summary>
			/// The iterator copying constructor
			/// </summary>
			/// <param name="orig">Original iterator</param>
			ForceInline PathIterator(const PathIterator& orig) : path(orig.path), offset(orig.offset), len(orig.len) {}

			/// <summary>
			/// The iterator moving constructor
			/// </summary>
			/// <param name="orig">Original iterator</param>
			ForceInline PathIterator(PathIterator&& orig) : path(MoveRValue(orig.path)), offset(orig.offset), len(orig.len) {}

		public:
			/// <summary>
			/// Object copying operator
			/// </summary>
			/// <param name="orig">Original</param>
			/// <returns>A reference to a current object</returns>
			ForceInline PathIterator& operator=(const PathIterator& orig) {
				this->~PathIterator();
				this->PathIterator::PathIterator(orig);
				return *this;
			}

			/// <summary>
			/// Object moving operator
			/// </summary>
			/// <param name="orig">Original</param>
			/// <returns>A reference to a current object</returns>
			ForceInline PathIterator& operator=(PathIterator&& orig) {
				this->~PathIterator();
				this->PathIterator::PathIterator(MoveRValue(orig));
				return *this;
			}

		public:
			/// <summary>
			/// Go to the next component of the path
			/// </summary>
			/// <returns>"RValue" reference to an object</returns>
			PathIterator&& Next()&& {
				if (IsPathEnd(true))
					Cardinal::Internals::Exceptions::ThrowIndexOutOfRange(SourcePosition, offset + len, path.Lenght);

				auto newPathEnd = path.Value.Find(Delimiter, offset + len + 1);

				if (newPathEnd == Math::Max<SizeT>())
				{
					newPathEnd = path.Value.Lenght;
				}

				offset = offset + len + 1;
				len = newPathEnd - offset;

				return MoveRValue(*this);
			}

			/// <summary>
			/// Go to the next component of the path
			/// </summary>
			/// <returns>The iterator to a next path component</returns>
			PathIterator Next()& {
				if (IsPathEnd(true))
					Cardinal::Internals::Exceptions::ThrowIndexOutOfRange(SourcePosition, offset + len, path.Lenght);

				auto cp = PathIterator(*this);

				auto newPathEnd = path.Value.Find(Delimiter, offset + len + 1);

				if (newPathEnd == Math::Max<SizeT>())
				{
					newPathEnd = path.Value.Lenght;
				}

				cp.offset = offset + len + 1;
				cp.len = newPathEnd - cp.offset;

				return cp;
			}

			/// <summary>
			/// Go to the next component of the path
			/// </summary>
			/// <returns>The iterator to a next path component</returns>
			PathIterator Next() const& {
				if (IsPathEnd(true))
					Cardinal::Internals::Exceptions::ThrowIndexOutOfRange(SourcePosition, offset + len, path.Lenght);

				auto cp = PathIterator(*this);

				auto newPathEnd = path.Value.Find(Delimiter, offset + len + 1);

				if (newPathEnd == Math::Max<SizeT>())
				{
					newPathEnd = path.Value.Lenght;
				}

				cp.offset = offset + len + 1;
				cp.len = newPathEnd - cp.offset;

				return cp;
			}

		public:
			/// <summary>
			/// The transition operator to the next component of the path to copy the path
			/// </summary>
			/// <returns>The iterator to a next path component</returns>
			ForceInline PathIterator operator++() const& { return Next(); }

			/// <summary>
			/// The transition operator to the next component of the path without copying the path
			/// </summary>
			/// <returns>The iterator to a next path component</returns>
			PathIterator operator++()& {
				if (IsPathEnd(true))
					Cardinal::Internals::Exceptions::ThrowIndexOutOfRange(SourcePosition, offset + len, path.Lenght);

				auto newPathEnd = path.Value.Find(Delimiter, offset + len + 1);

				if (newPathEnd == Math::Max<SizeT>())
				{
					newPathEnd = path.Value.Lenght;
				}

				offset = offset + len + 1;
				len = newPathEnd - offset;

				return *this;
			}

			/// <summary>
			/// The transition operator to the next component of the path without copying the path
			/// </summary>
			/// <returns>The iterator to a next path component</returns>
			ForceInline PathIterator&& operator++()&& { return MoveRValue(*this).Next(); }

			/// <summary>
			/// Equality operator with the end of path which is presented as a zero address
			/// </summary>
			/// <param name="_">Zero address</param>
			/// <returns>The logical value</returns>
			ForceInline bool operator==(NullptrT _) { return IsPathEnd(); }

			/// <summary>
			/// Inequality operator with the end of path which is presented as a zero address
			/// </summary>
			/// <param name="_">Zero address</param>
			/// <returns>The logical value</returns>
			ForceInline bool operator!=(NullptrT) { return !IsPathEnd(); }

			/// <summary>
			/// Equality operator with other iterator
			/// </summary>
			/// <param name="other">Second operand</param>
			/// <returns>The logical value</returns>
			bool operator==(const PathIterator& other) const {
				return this->path == other.path && this->len == other.len && this->offset == other.offset;
			}

			/// <summary>
			/// Inequality operator with other iterator
			/// </summary>
			/// <param name="other">Second operand</param>
			/// <returns>The logical value</returns>
			bool operator!=(const PathIterator& other) const {
				return !(*this == other);
			}

		public:
			/// <summary>
			/// Denomination operator, returns the string representation of current path component
			/// </summary>
			/// <returns>A string with path</returns>
			BaseStringView<THeap> operator*() const {
				return GetValue();
			}

		public:
			/// <summary>
			/// Returns the relative path (default without object name)
			/// </summary>
			/// <param name="withItemName">Boolean value if extract object name</param>
			/// <returns>Relative path</returns>
			ForceInline BaseStringView<THeap> ExtractRelativePath(bool withItemName = false) {
				if (withItemName)
				{
					return BaseStringView<THeap>(Value.GetFullText(), offset);
				}
				else
				{
					auto newlen = Value.GetFullText().FindLeft(Delimiter);
					// If the position is last than searching, once again excluding this position
					newlen = (newlen == Value.GetFullText().Lenght - 1) ? Value.GetFullText().FindLeft(Delimiter, 0, newlen - 1) : newlen;
					newlen = (newlen < offset) ? 0 : newlen - offset;

					return BaseStringView<THeap>(Value.GetFullText(), offset, newlen);
				}
			}

		public:
			/// <summary>
			/// Returns "true" if it is the last part of the path, "false" if it is the end of the line
			/// </summary>
			/// <param name="IgnoreDirectoryDelimiterInEnd">Ignore the path delimiter at the end of the path line</param>
			/// <returns>"True" if it is the last part of the fragment</returns>
			ForceInline bool IsLastPathPart(bool IgnoreDirectoryDelimiterInEnd = false) const {
				if (IsPathEnd(IgnoreDirectoryDelimiterInEnd))
					return false;

				auto newOffset = path.Value.Find(Delimiter, offset);

				if (newOffset != Math::Max<SizeT>()) newOffset += 1;

				if (IgnoreDirectoryDelimiterInEnd)
					return newOffset == Math::Max<SizeT>() || newOffset >= path.Value.Lenght;
				else
					return newOffset == Math::Max<SizeT>() || newOffset >= path.Value.Lenght + 1;
			}

			/// <summary>
			/// Returns "true" if this is the end of the path
			/// </summary>
			/// <returns>The logical value</returns>
			ForceInline bool IsPathEnd(bool IgnoreDirectoryDelimiterInEnd = false) const {
				if (IgnoreDirectoryDelimiterInEnd)
					return offset == Math::Max<SizeT>() || offset >= path.Value.Lenght;
				else
					return offset == Math::Max<SizeT>() || offset >= path.Value.Lenght + 1;
			}

		public:
			/// <summary>
			/// Returns the value of current path component
			/// </summary>
			/// <returns>Part of string variable</returns>
			ForceInline BaseStringView<THeap> GetValue() const {
				return BaseStringView<THeap>(path.This->path, offset, len);
			}

			/// <summary>
			/// Returns the path object
			/// </summary>
			/// <returns>Invariant reference to the path object</returns>
			ForceInline const BasePath& GetFullPath() const {
				return path;
			}

			/// <summary>
			/// Indicates whether this path component is a directory
			/// </summary>
			/// <returns>The logical value</returns>
			ForceInline bool GetIsDirectory() const {
				if (offset == Math::Max<SizeT>() || len == Math::Max<SizeT>())
				{
					return false;
				}

				if (offset + len > path.Lenght)
				{
					return false;
				}
				else if (offset + len == path.Lenght)
				{
					return path.IsDirectory;
				}

				return path.Value[len + offset] == Delimiter;
			}

			/// <summary>
			/// Returns name indent of the current path component relative to the entire path name
			/// </summary>
			/// <returns>Numeric value</returns>
			ForceInline SizeT GetOffset() const {
				return offset;
			}

		public:
			/// <summary>
			/// Returns the path to the current component along with its name
			/// </summary>
			/// <returns>Returns the string path</returns>
			ForceInline BaseStringView<THeap> GetCurrentPath() const {
				if (offset > Math::Max<SizeT>() || offset > path.Lenght)
				{
					return BaseStringView<THeap>(path.This->path);
				}
				else if (offset + len > path.Lenght)
				{
					return BaseStringView<THeap>(path.This->path);
				}
				else
				{
					return BaseStringView<THeap>(path.This->path, 0, offset + len);
				}
			}

		public:
			/// <summary>
			/// String representation of the current path component
			/// </summary>
			BaseStringView<THeap> Property(GetValue) Value;

			/// <summary>
			/// String representation of the path to current component with it
			/// </summary>
			BaseStringView<THeap> Property(GetCurrentPath) CurrentPath;

			/// <summary>
			/// String representation of full path
			/// </summary>
			BasePath Property(GetFullPath) FullPath;

			/// <summary>
			/// Indicates whether the curretn component is a directory
			/// </summary>
			Boolean Property(GetIsDirectory) IsDirectory;

			/// <summary>
			/// Indent to the current component of the path relative to the beginning of the path
			/// </summary>
			SizeT Property(GetOffset) Offset;

		public:
			friend class BasePath;
		};

	public:
		/// <summary>
		/// Copying constructor with string representation of path
		/// </summary>
		/// <param name="path">String representation of path</param>
		ForceInline BasePath(const TString& path) : This(new Data(path)) {}

		/// <summary>
		/// Moving constructor with string representation of path
		/// </summary>
		/// <param name="path">String representation of path</param>
		ForceInline BasePath(TString&& path) : This(new Data(MoveRValue(path))) {}

	public:
		/// <summary>
		/// Concatenation of a path with a string, if at the end of the path there is no directory delimiter, it will be automatically added
		/// </summary>
		/// <param name="pathPart">Second path of string</param>
		/// <returns>Current object</returns>
		BasePath& operator+=(const TString& pathPart) {
			if (This->path.Lenght != 0)
			{
				if (This->path[This->path.Lenght - 1] == Delimiter)
				{
					if (pathPart[0] != Delimiter)
					{
						This->path.Reserve(This->path.Lenght + pathPart.Lenght);
						This->path += pathPart;
					}
					else
					{
						This->path.Shrink(This->path.Lenght);
						This->path.Reserve(This->path.Lenght + pathPart.Lenght);
						This->path += pathPart;
					}
					return *this;
				}
			}

			if (pathPart[0] != Delimiter)
			{
				This->path.Reserve(This->path.Lenght + 1 + pathPart.Lenght);
				This->path += Delimiter;
				This->path += pathPart;
			}
			else
			{
				This->path.Reserve(This->path.Lenght + pathPart.Lenght);
				This->path += pathPart;
			}

			return *this;
		}

		/// <summary>
		/// Concatenation of a path with a string, if at the end of the path there is no directory delimiter, it will be automatically added
		/// </summary>
		/// <param name="pathPart">Second path of string</param>
		/// <returns>Current object</returns>
		BasePath& operator+=(TString&& pathPart) {
			if (This->path.Lenght != 0)
			{
				if (This->path[This->path.Lenght - 1] == Delimiter)
				{
					if (pathPart[0] != Delimiter)
					{
						This->path.Reserve(This->path.Lenght + pathPart.Lenght);
						This->path += pathPart;
					}
					else
					{
						This->path.Shrink(This->path.Lenght);
						This->path.Reserve(This->path.Lenght + pathPart.Lenght);
						This->path += pathPart;
					}
					return *this;
				}
			}

			if (pathPart[0] != Delimiter)
			{
				This->path.Reserve(This->path.Lenght + 1 + pathPart.Lenght);
				This->path += Delimiter;
				This->path += pathPart;
			}
			else
			{
				This->path.Reserve(This->path.Lenght + pathPart.Lenght);
				This->path += pathPart;
			}

			return *this;
		}

		/// <summary>
		/// Concatenation of a path with a string, if at the end of the path there is no directory delimiter, it will be automatically added
		/// </summary>
		/// <param name="pathPart">Second path of string</param>
		/// <typeparam name="pathPartSize">Number of symbols in second part of the path</typeparam>
		/// <returns>Current object</returns>
		template<SizeT pathPartSize>
		BasePath& operator+=(StaticTextRef<pathPartSize> pathPart) {
			if (This->path.Lenght != 0)
			{
				if (This->path[This->path.Lenght - 1] == Delimiter)
				{
					if (pathPart[0] != Delimiter)
					{
						This->path.Reserve(This->path.Lenght + pathPartSize);
						This->path += pathPart;
					}
					else
					{
						This->path.Shrink(This->path.Lenght);
						This->path.Reserve(This->path.Lenght + pathPartSize);
						This->path += pathPart;
					}
					return *this;
				}
			}

			if (pathPart[0] != Delimiter)
			{
				This->path.Reserve(This->path.Lenght + 1 + pathPartSize);
				This->path += Delimiter;
				This->path += pathPart;
			}
			else
			{
				This->path.Reserve(This->path.Lenght + pathPartSize);
				This->path += pathPart;
			}

			return *this;
		}

	public:
		/// <summary>
		/// Combining path components into a path object
		/// </summary>
		/// <param name="parts">Parts of path in a string view</param>
		/// <param name="isStartWithDir">Indicates whether set the component delimiter on a beginning of path</param>
		/// <returns>Path object</returns>
		static BasePath Combine(Containers::BaseArray<THeap, TString>&& parts, bool isStartWithDir = false) {
			bool previousDirEnd = !isStartWithDir;

			return TString::Combine(
				parts.MoveTo<Containers::Array>([&](TString&& part)  InlineLambda->TString
					{
						if (part.Lenght == 0)
						{
							return MoveRValue(part);
						}

						bool currrentDirEnd = part[part.Lenght - 1] == Delimiter;

						if (!previousDirEnd && !part.IsStartWith(DelimiterStr))
						{
							part = DelimiterStr + part;
						}

						if (previousDirEnd && part[0] == Delimiter)
						{
							part.LeftResize(part.Lenght - 1);
						}

						previousDirEnd = currrentDirEnd;
						return MoveRValue(part);
					})
			);
		}

	public:
		/// <summary>
		/// Returns the iterator to a first path component
		/// </summary>
		/// <returns>Iterator on a first path component</returns>
		ForceInline PathIterator GetIterator() const& {
			auto offset = 0;
			auto len = This->path.Find(Delimiter);

			if (len == 0)
			{
				offset = 1; len = This->path.Find(Delimiter, offset);
			}
			else if (len == Math::Max<SizeT>())
			{
				len = Value.Lenght;
			}

			return PathIterator(*this, offset, len - offset);
		}

		/// <summary>
		/// Returns the iterator to a first path component
		/// </summary>
		/// <returns>Iterator on a first path component</returns>
		ForceInline PathIterator GetIterator()& {
			auto offset = 0;
			auto len = This->path.Find(Delimiter);

			if (len == 0)
			{
				offset = 1; len = This->path.Find(Delimiter, offset);
			}
			else if (len == Math::Max<SizeT>())
			{
				len = This->path.Lenght;
			}

			return PathIterator(*this, offset, len - offset);
		}

		/// <summary>
		/// Returns the iterator to a first path component
		/// </summary>
		/// <returns>Iterator on a first path component</returns>
		PathIterator GetIterator()&& {
			auto offset = 0;
			auto len = This->path.Find(Delimiter);

			if (len == 0)
			{
				offset = 1; len = This->path.Find(Delimiter, offset);
			}
			else if (len == Math::Max<SizeT>())
			{
				len = This->path.Lenght;
			}

			return PathIterator(MoveRValue(*this), offset, len - offset);
		}

	public:
		/// <summary>
		/// Iterator on a first path component
		/// </summary>
		PathIterator Property(GetIterator) Iterator;

	public:
		/// <summary>
		/// Returns the iterator to a last path component
		/// </summary>
		/// <returns>Iterator on a last path component</returns>
		ForceInline PathIterator GetLast() const& {
			auto offset = This->path.FindLeft(Delimiter) + 1;

			return PathIterator(*this, offset, This->path.Lenght - offset);
		}

		/// <summary>
		/// Returns the iterator to a last path component
		/// </summary>
		/// <returns>Iterator on a last path component</returns>
		ForceInline PathIterator GetLast()&& {
			auto offset = This->path.FindLeft(Delimiter) + 1;

			return PathIterator(MoveRValue(*this), offset, This->path.Lenght - offset);
		}

	public:
		/// <summary>
		/// The length of string representation of the path
		/// </summary>
		/// <returns>Numeric value</returns>
		ForceInline SizeT GetLenght() const {
			return This->path.GetLenght();
		}

		/// <summary>
		/// Indicates whether path is ends in a directoty (contains the component delimiter in the end of path)
		/// </summary>
		/// <returns>The logical value</returns>
		ForceInline bool GetIsDirectory() const {
			return This->path.Lenght != 0 && (This->path[This->path.Lenght - 1] == Delimiter);
		}

		/// <summary>
		/// Indicates whether path is empty
		/// </summary>
		/// <returns>The logical value</returns>
		ForceInline bool GetIsEmpty() const {
			return This->path.Lenght == 0;
		}

		/// <summary>
		/// Returns the string representation of path
		/// </summary>
		/// <returns>String representation of path</returns>
		ForceInline const TString& GetValue() const {
			return This->path;
		}

		/// <summary>
		/// Returns the number of path components
		/// </summary>
		/// <returns>Number of path components</returns>
		ForceInline SizeT GetComponentsCount() const {
			if (This->path.Lenght == 0)
				return 0;

			SizeT count = 0;
			This->path.ForEach([&](const auto& ch) InlineLambda{ if (ch == Delimiter) count++; });
			count += -((This->path[0] == Delimiter) ? 0 : 1);

			return count;
		}

	public:
		bool operator==(const BasePath& other) const {
			return This->path == other.This->path;
		}

		bool operator!=(const BasePath& other) const {
			return This->path != other.This->path;
		}

	public:
		/// <summary>
		/// The length of the text representation of the path
		/// </summary>
		SizeT Property(GetLenght) Lenght;

		/// <summary>
		/// Indicates whether the path ends by the directory
		/// </summary>
		bool Property(GetIsDirectory) IsDirectory;

		/// <summary>
		/// Indicates whether path is empty
		/// </summary>
		bool Property(GetIsEmpty) IsEmpty;

		/// <summary>
		/// String representation of path
		/// </summary>
		TString Property(GetValue) Value;

		/// <summary>
		/// Number of path components
		/// </summary>
		SizeT Property(GetComponentsCount) ComponentsCount;

		/// <summary>
		/// Iterator on a last path component
		/// </summary>
		PathIterator Property(GetLast) Last;

	public:
		/// <summary>
		/// Returns the iterator to the beginning of the path
		/// </summary>
		/// <returns>Iterator to the beginning of the path</returns>
		ForceInline PathIterator begin() const& { return this->GetIterator(); }

		/// <summary>
		/// Returns the iterator to the beginning of the path
		/// </summary>
		/// <returns>Iterator to the beginning of the path</returns>
		ForceInline PathIterator begin()& { return this->GetIterator(); }

		/// <summary>
		/// Returns the iterator to the beginning of the path
		/// </summary>
		/// <returns>Iterator to the beginning of the path</returns>
		ForceInline PathIterator begin()&& { return MoveRValue(*this).GetIterator(); }

		/// <summary>
		/// Iterator at the end of the representation as a null address
		/// </summary>
		/// <returns>Null address</returns>
		ForceInline NullptrT end()& { return nullptr; }

		/// <summary>
		/// Iterator at the end of the representation as a null address
		/// </summary>
		/// <returns>Null address</returns>
		ForceInline NullptrT end() const& { return nullptr; }

	public:
		RefCountClass(BasePath, This);
	};

	/// <summary>
	/// A path with using the standard path allocator
	/// </summary>
	template<Char TPathDelimiter>
	using Path = BasePath<Memory::Heap, TPathDelimiter>;

	/// <summary>
	/// A path with using the sensitive path allocator
	/// </summary>
	template<Char TPathDelimiter>
	using SensitivePath = BasePath<Memory::SensitiveDataHeap, TPathDelimiter>;
}

#pragma warning(pop)