export module Cardinal.Containers.Concurency.Dictionary;

export import Cardinal.Core;
export import Cardinal.Memory;
export import Cardinal.Containers.LINQ;
export import Cardinal.Containers.TypeTraits;
export import Cardinal.Containers.Pair;
export import Cardinal.Containers.Concurency.Map;
export import Cardinal.Containers.Base;
export import Cardinal.String;
export import Cardinal.Threading.SRWLock;

import Cardinal.Exceptions.Internals;

namespace Cardinal::Containers
{
	template<typename THeap, typename TString, typename TData, TCharacterComparator CharacterComparator>
	class BaseConcurencyDictionary;
}

namespace Cardinal::Containers::Details
{
	template<TCharacterComparator CharacterComparator>
	struct BaseConcurencyDictionaryWithCharterComparator {
		template<typename THeap, typename TString, typename TData>
		using Type = BaseConcurencyDictionary<THeap, TString, TData, CharacterComparator>;
	};
}

export namespace Cardinal::Containers
{
	/// <summary>
	/// A word dictionary template with multithreading support which performs fast word searching
	/// </summary>
	template<typename THeap, typename TString, typename TData, TCharacterComparator CharacterComparator>
	class BaseConcurencyDictionary :
		public Memory::Allocator<THeap>,
		public LINQ::IDictionary<
		typename Details::BaseConcurencyDictionaryWithCharterComparator<CharacterComparator>::Type, THeap, TString, TData, BasePair<THeap, TString, TData>, true> {
		static_assert(TypeTraits::IsSameV<TString, BaseString<THeap>>, "BaseConcurencyDictionary key must be BaseString<THeap>");

	private:
		using Pair = BasePair<THeap, TString, TData>;
		// A letter structure as part of "AVL-tree"
		struct NodeSym;

		// A subtree structure
		struct NodeLex : public Memory::Allocator<THeap> {
			RefCountT count;
			NodeSym* node_root;
			BasePair<THeap, TString, TData>* data;

			ForceInline NodeLex(SizeT count = 0) : count(count), node_root(nullptr), data(nullptr) {};
		};

		// A letter structure as part of "AVL-tree"
		struct NodeSym : public Memory::Allocator<THeap> {
			UInt8 height;
			Char key;
			NodeLex* data;
			NodeSym* left;
			NodeSym* right;

			ForceInline NodeSym(Char key) : height(1), key(key), data(nullptr), left(nullptr), right(nullptr) {}
		};

		// Tree height
		UInt8 height(NodeSym* node) {
			return node ? node->height : 0;
		}

		// A left and right subbranches heights difference
		Int8 bfactor(NodeSym* node) {
			return height(node->right) - height(node->left);
		}

		// Height recalculation
		void fixheight(NodeSym* node) {
			auto h_left = height(node->left);
			auto h_right = height(node->right);
			node->height = ((h_left < h_right) ? h_right : h_left) + 1;
		}

		// Subtree right rotation
		NodeSym* rotateright(NodeSym* p) {
			NodeSym* q = p->left;
			p->left = q->right;
			q->right = p;
			fixheight(p);
			fixheight(q);
			return q;
		}

		// Subtree left rotation
		NodeSym* rotateleft(NodeSym* q) {
			NodeSym* p = q->right;
			q->right = p->left;
			p->left = q;
			fixheight(q);
			fixheight(p);
			return p;
		}

		// Subtree balancing
		NodeSym* balance(NodeSym* p) {
			fixheight(p);
			if (bfactor(p) == 2)
			{
				if (bfactor(p->right) < 0)
					p->right = rotateright(p->right);
				return rotateleft(p);
			}
			if (bfactor(p) == -2)
			{
				if (bfactor(p->left) > 0)
					p->left = rotateleft(p->left);
				return rotateright(p);
			}
			return p;
		}

		// Element insertion in tree
		NodeSym* insert(NodeSym* p, wchar_t key) {
			if (!p) return new NodeSym(key);
			if (key == p->key) return p;
			if (key < p->key)
				p->left = insert(p->left, key);
			else
				p->right = insert(p->right, key);
			return balance(p);
		}

		// Minimal element searching in tree
		NodeSym* findmin(NodeSym* p) {
			return p->left ? findmin(p->left) : p;
		}

		// Minimal element deleting in tree
		NodeSym* removemin(NodeSym* p) {
			if (p->left == 0)
				return p->right;
			p->left = removemin(p->left);
			return balance(p);
		}

		// Element deleting in subtree by key
		NodeSym* remove(NodeSym* p, Char key) {
			if (!p) return nullptr;

			if (CharacterComparator(key, p->key) < 0)
				p->left = remove(p->left, key);
			else if (CharacterComparator(key, p->key) > 0)
				p->right = remove(p->right, key);
			else
			{
				NodeSym* q = p->left;
				NodeSym* r = p->right;
				delete p;
				if (!r) return q;
				NodeSym* min = findmin(r);
				min->right = removemin(r);
				min->left = q;
				return balance(min);
			}
			return balance(p);
		}

		// Element searching in tree
		NodeSym* find(NodeSym* root, Char key) const { //-V688
			if (!root) return nullptr;

			while (CharacterComparator(root->key, key) != 0)
			{
				if (CharacterComparator(root->key, key) > 0)
					root = root->left;
				else
					root = root->right;

				if (!root)
					return nullptr;
			}

			return root;
		}

		// Element insertion in tree system
		NodeLex* insert(
			NodeLex* rootLex, //-V688
			Pair&& data,
			size_t index = 1
		) {
			if (!rootLex)
			{
				rootLex = new NodeLex;
			}
			rootLex->node_root = insert(rootLex->node_root, data.Key[index - 1]);

			auto subnode = find(rootLex->node_root, data.Key[index - 1]);
			if (data.Key.Lenght != index)
			{
				subnode->data = insert(subnode->data, MoveRValue(data), index + 1);
			}
			else
			{
				if (subnode->data == nullptr) subnode->data = new NodeLex(1);

				if (subnode->data->data != nullptr)
					Internals::Exceptions::ThrowNullPointException(SourcePosition);
				subnode->data->data = new Pair(MoveRValue(data));
			}

			++rootLex->count;
			return rootLex;
		}

		// Element searching in tree system
		template<typename UString>
		NodeLex* find(
			NodeLex* This, //-V688
			const UString& data,
			size_t index = 0
		) const {
			if (!This) return nullptr;
			auto subnode = find(This->node_root, data[index]);
			if (!subnode) return nullptr;
			if (data.Lenght - 1 != index)
			{
				return find(subnode->data, data, index + 1);
			}
			else
			{
				return subnode->data;
			}
		}

		// Element deleting in tree system
		NodeLex* remove_node(NodeLex* This, //-V688
			const TString& key,
			bool& removed,
			size_t index = 1
		) {
			if (!This) return nullptr;

			if (key.Lenght != index - 1)
			{
				auto subnode = find(This->node_root, key[index - 1]);
				if (!subnode)
					return This;

				subnode->data = remove_node(subnode->data, key, removed, index + 1);
				if (!subnode->data)
					return This;

				if (removed)
				{
					--subnode->data->count;
					if (subnode->data->count == 0)
					{
						delete subnode->data;
						subnode->data = nullptr;
						This->node_root = remove(This->node_root, key[index - 1]);
					}
				}
			}
			else
			{
				if (This->data != nullptr)
				{
					delete This->data;
					removed = true;
				}
			}
			return This;
		}

		// Tree element destroying
		NodeLex* destroy_node(NodeLex* This) {
			if (This != nullptr)
			{
				delete This->data;

				if (This->node_root != nullptr)
					This->node_root = destroy_tree(This->node_root);
			}
			delete This;
			return nullptr;
		}

		// Tree destroying
		NodeSym* destroy_tree(NodeSym* This) {
			if (This != nullptr)
			{
				if (This->left != nullptr)
				{
					This->left = destroy_tree(This->left);
				}

				if (This->data != nullptr)
				{
					This->data = destroy_node(This->data);
				}

				if (This->right != nullptr)
				{
					This->right = destroy_tree(This->right);
				}

				delete This;
			}
			return nullptr;
		}

	private:
		NodeLex* node;
		Threading::SRWLock mutable lock;
		RefCountT count;

	private:
		NodeSym* create_lex_copy(NodeSym*& orig) const {
			NodeSym* copy = new NodeSym(orig->key);

			if (orig->data != nullptr) copy->data = create_lex_copy(orig->data);
			if (orig->left != nullptr) copy->left = create_lex_copy(orig->left);
			if (orig->right != nullptr) copy->right = create_lex_copy(orig->right);

			copy->height = orig->height;

			return copy;
		}

		NodeLex* create_lex_copy(NodeLex*& orig) const {
			NodeLex* copy = new NodeLex(orig->count);
			if (orig->node_root != nullptr) copy->node_root = create_lex_copy(orig->node_root);

			if (orig->data != nullptr)
			{
				if constexpr (TypeTraits::IsPodV<TData>)
				{
					copy->data = new BasePair<THeap, TString, TData>(*orig->data);
				}
				else if constexpr (Concepts::HasCloneMethod<TData>)
				{
					copy->data = new BasePair<THeap, TString, TData>(orig->data->Clone());
				}
				else
				{
					copy->data = new BasePair<THeap, TString, TData>(*orig->data);
				}
			}

			return copy;
		}

		BaseConcurencyDictionary* create_lex_copy(BaseConcurencyDictionary* This) const {
			if (This == nullptr)
				return nullptr;

			BaseConcurencyDictionary* copy = new BaseConcurencyDictionary();

			if (This->node != nullptr)
				copy->node = create_lex_copy(This->node);

			return copy;
		}

	private:
		template<typename = void> requires Concepts::IsClonable<TString>&& Concepts::IsClonable<TData>
		NodeSym* create_lex_clone_copy(NodeSym* orig) const {
			NodeSym* copy = new NodeSym(orig->key);

			if (orig->data != nullptr) copy->data = create_lex_clone_copy(orig->data);
			if (orig->left != nullptr) copy->left = create_lex_clone_copy(orig->left);
			if (orig->right != nullptr) copy->right = create_lex_clone_copy(orig->right);

			copy->height = orig->height;

			return copy;
		}

		template<typename = void> requires Concepts::IsClonable<TString>&& Concepts::IsClonable<TData>
		NodeLex* create_lex_clone_copy(NodeLex* orig) const {
			NodeLex* copy = new NodeLex(orig->count);

			if (orig->node_root != nullptr)
				copy->node_root = create_lex_clone_copy(orig->node_root);

			if (orig->data != nullptr)
			{
				copy->data = new BasePair<THeap, TString, TData>(Clone(*orig->data));
			}
			return copy;
		}

	private:
		template<typename TLambda>
		NodeSym* create_lex_clone_copy(NodeSym* orig, TLambda& lambda) const {
			NodeSym* copy = new NodeSym(orig->key);

			if (orig->data != nullptr) copy->data = create_lex_clone_copy(orig->data, lambda);
			if (orig->left != nullptr) copy->left = create_lex_clone_copy(orig->left, lambda);
			if (orig->right != nullptr) copy->right = create_lex_clone_copy(orig->right, lambda);

			copy->height = orig->height;

			return copy;
		}

		template<typename TLambda>
		NodeLex* create_lex_clone_copy(NodeLex* orig, TLambda& lambda) const {
			NodeLex* copy = new NodeLex(orig->count);
			if (orig->node_root != nullptr)
				copy->node_root = create_lex_clone_copy(orig->node_root);

			if (orig->data != nullptr)
			{
				copy->data = new BasePair<THeap, TString, TData>(lambda(orig->data));
			}

			return copy;
		}

	private:
		void ToMap(
			NodeSym* node,
			BaseConcurencyMap<THeap, TString, TData>& list) {
			if (node->left != nullptr) ToMap(node->left, list);
			if (node->data != nullptr) ToMap(node->data, list);
			if (node->right != nullptr) ToMap(node->right, list);
		}

		void ToMap(
			NodeLex* node,
			BaseConcurencyMap<THeap, TString, TData>& list) {
			if (node->data != nullptr)
				list.Insert(*node->data);

			if (node->node_root != nullptr)
				ToMap(node->node_root, list);
		}

		void ToMap(
			BaseConcurencyDictionary* This,
			BaseConcurencyMap<THeap, TString, TData>& list) {
			if (This != nullptr)
				if (This->node != nullptr)
					ToMap(This->node, list);
		}

	public:
		~BaseConcurencyDictionary() {
			this->lock.ExclusiveLock([&] () {
				this->node = destroy_node(this->node);
				this->count = 0;
				});
		}

	public:
		/// <summary>
		/// An empty constructor which initialize an empty dictionary
		/// </summary>
		ForceInline BaseConcurencyDictionary() : node(nullptr), lock(), count(0) {};

		/// <summary>
		/// A key-value pairs list constructor
		/// </summary>
		/// <param name="pairs">A key-value pairs list</param>
		BaseConcurencyDictionary(InitilizerList<BasePair<THeap, TString, TData>>&& pairs) : node(nullptr), lock(), count(0) {
			for (auto&& it : pairs)
			{
				Insert(MoveRValue(it));
			}
		}

		/// <summary>
		/// Copying constructor from original dictionary
		/// </summary>
		/// <param name="dictionary">Original dictionary</param>
		BaseConcurencyDictionary(const BaseConcurencyDictionary& dictionary) : lock() {
			this->lock.SharedLock([&]() InlineLambda{
				this->node = create_lex_clone_copy(dictionary.node);
				this->count = dictionary.count;
				});
		}

		/// <summary>
		/// Moving constructor from original dictionary
		/// </summary>
		/// <param name="dictionary">Original dictionary</param>
		BaseConcurencyDictionary(BaseConcurencyDictionary&& dictionary) : lock() {
			this->lock.ExclusiveLock([&]() InlineLambda{
				this->node = MoveRValue(dictionary.node);
				dictionary.node = nullptr;
				this->count = MoveRValue(dictionary.count);
				dictionary.count = 0;
				});
		}

		/// <summary>
		/// Copying operator
		/// </summary>
		/// <param name="original">Original</param>
		/// <returns>"LValue" reference to array</returns>
		BaseConcurencyDictionary& operator=(const BaseConcurencyDictionary& original) {
			this->~BaseConcurencyDictionary();
			this->BaseConcurencyDictionary::BaseConcurencyDictionary(original);
			return *this;
		}

		/// <summary>
		/// Moving operator
		/// </summary>
		/// <param name="original">Original</param>
		/// <returns>"RValue" reference to array</returns>
		BaseConcurencyDictionary& operator=(BaseConcurencyDictionary&& original) {
			this->~BaseConcurencyDictionary();
			this->BaseConcurencyDictionary::BaseConcurencyDictionary(MoveRValue(original));
			return *this;
		}

		/// <summary>
		/// Element insertion by key and value in dictionary
		/// </summary>
		/// <param name="key">Key</param>
		/// <param name="value">Value</param>
		/// <returns>A reference to a current object</returns>
		ForceInline BaseConcurencyDictionary& Insert(const TString& key, const TData& value) {
			return Insert({ key, value });
		}

		/// <summary>
		/// Element insertion by key and value in dictionary
		/// </summary>
		/// <param name="key">Key</param>
		/// <param name="value">Value</param>
		/// <returns>A reference to a current object</returns>
		ForceInline BaseConcurencyDictionary& Insert(TString&& key, const TData& value) {
			return Insert({ MoveRValue(key), value });
		}

		/// <summary>
		/// Element insertion by key and value in dictionary
		/// </summary>
		/// <param name="key">Key</param>
		/// <param name="value">Value</param>
		/// <returns>A reference to a current object</returns>
		ForceInline BaseConcurencyDictionary& Insert(const TString& key, TData&& value) {
			return Insert({ key, MoveRValue(value) });
		}

		/// <summary>
		/// Element insertion by key and value in dictionary
		/// </summary>
		/// <param name="key">Key</param>
		/// <param name="value">Value</param>
		/// <returns>A reference to a current object</returns>
		ForceInline BaseConcurencyDictionary& Insert(TString&& key, TData&& value) {
			return Insert({ MoveRValue(key), MoveRValue(value) });
		}

		/// <summary>
		/// Element insertion by key and value in dictionary
		/// </summary>
		/// <param name="data">A key-value pair</param>
		/// <returns>A reference to a current object</returns>
		ForceInline BaseConcurencyDictionary& Insert(Pair&& data) {
			return this->lock.ExclusiveLock([&, data = MoveRValue(data)]() mutable->BaseConcurencyDictionary & InlineLambda{
				this->node = insert(this->node, MoveRValue(data));
				++this->count;
				return *this;
				});
		}

		/// <summary>
		/// Element insertion by key and value in dictionary
		/// </summary>
		/// <param name="data">A key-value pair</param>
		/// <returns>A reference to a current object</returns>
		ForceInline BaseConcurencyDictionary& Insert(const Pair& data) {
			return this->lock.ExclusiveLock([&]() mutable->BaseConcurencyDictionary & InlineLambda{
				this->node = insert(this->node, MoveRValue(Pair(data)));
				++this->count;
				return *this;
				});
		}

		/// <summary>
		/// Searching a value by key
		/// </summary>
		/// <param name="Key">Key</param>
		/// <returns>A constant reference to value</returns>
		template<typename UString> requires (requires(const UString& str) {
			{str[0]}->Concepts::IsConvertible<Char>;
		})
			ForceInline const TData& Find(const UString& Key) const {
			return this->lock.SharedLock([&]() -> const TData & InlineLambda{
				if (Key.Lenght == 0)
				Internals::Exceptions::ThrowNullPointException(SourcePosition);
				if (this->node == nullptr)
					Internals::Exceptions::ThrowNotFoundException(SourcePosition, L"Invalid Key");

				NodeLex* node = find(this->node, Key);

				if (node == nullptr)
					Internals::Exceptions::ThrowNotFoundException(SourcePosition, L"Invalid Key");

				return node->data->Value;
				});
		}

		/// <summary>
		/// Searching a value by key
		/// </summary>
		/// <param name="Key">Key</param>
		/// <returns>A reference to value</returns>
		template<typename UString> requires (requires(const UString& str) {
			{str[0]}->Concepts::IsConvertible<Char>;
		})
			ForceInline TData& Find(const UString& Key) {
			return this->lock.SharedLock([&]()->TData & InlineLambda{
				if (Key.Lenght == 0)
				Internals::Exceptions::ThrowNullPointException(SourcePosition);
				if (this->node == nullptr)
					Internals::Exceptions::ThrowNotFoundException(SourcePosition, L"Invalid Key");

				NodeLex* node = find(this->node, Key);
				if (node == nullptr)
					Internals::Exceptions::ThrowNotFoundException(SourcePosition, L"Invalid Key");

				return node->data->Value;
				});
		}

		/// <summary>
		/// Searching a value by key
		/// </summary>
		/// <param name="Key">Key</param>
		/// <returns>A reference to value</returns>
		template<SizeT TN>
		TData& Find(StaticTextRef<TN> Key) {
			return Find(ConstString(Key));
		}

		/// <summary>
		/// Searching a value by key
		/// </summary>
		/// <param name="Key">Key</param>
		/// <returns>A reference to value</returns>
		template<SizeT TN>
		const TData& Find(StaticTextRef<TN> Key) const {
			return Find(ConstString(Key));
		}

		/// <summary>
		/// The element sampling operator by key
		/// </summary>
		/// <param name="key">Key</param>
		/// <returns>A reference to value</returns>
		template<typename UString> requires (requires(const UString& str) {
			{str[0]}->Concepts::IsConvertible<Char>;
		})
			ForceInline TData& operator[](const UString& key) {
			return Find(key);
		}

		/// <summary>
		/// The element sampling operator by key
		/// </summary>
		/// <param name="key">Key</param>
		/// <returns>A reference to value</returns>
		template<typename UString> requires (requires(const UString& str) {
			{str[0]}->Concepts::IsConvertible<Char>;
		})
			ForceInline TData& operator[](UString&& key) {
			return Find(key);
		}

		/// <summary>
		/// The constant element sampling operator by key
		/// </summary>
		/// <param name="key">Key</param>
		/// <returns>A constant reference to value</returns>
		template<typename UString> requires (requires(const UString& str) {
			{str[0]}->Concepts::IsConvertible<Char>;
		})
			ForceInline const TData& operator[](const UString& key) const {
			return Find(key);
		}

		/// <summary>
		/// The constant element sampling operator by key
		/// </summary>
		/// <param name="key">Key</param>
		/// <returns>A constant reference to value</returns>
		template<typename UString> requires (requires(const UString& str) {
			{str[0]}->Concepts::IsConvertible<Char>;
		})
			ForceInline const TData& operator[](UString&& key) const {
			return Find(key);
		}

		/// <summary>
		/// The constant element sampling operator by key
		/// </summary>
		/// <param name="Key">Key</param>
		/// <returns>A constant reference to value</returns>
		template<SizeT TN>
		TData& operator[](StaticTextRef<TN> Key) {
			return Find(ConstString(Key));
		}

		/// <summary>
		/// The constant element sampling operator by key
		/// </summary>
		/// <param name="Key">Key</param>
		/// <returns>A constant reference to value</returns>
		template<SizeT TN>
		const TData& operator[](StaticTextRef<TN> Key) const {
			return Find(ConstString(Key));
		}

		/// <summary>
		/// Checking if dictionary in empty
		/// </summary>
		/// <returns>"True" if dictionary is empty</returns>
		ForceInline Boolean IsEmpty() const { return this->count == 0; }

		/// <summary>
		/// Checking if an element with given key is in the dictionary
		/// </summary>
		/// <param name="Key">A key by which searches the element</param>
		/// <returns>"True" if element exists</returns>
		template<typename UString> requires (requires(const UString& str) {
			{str[0]}->Concepts::IsConvertible<Char>;
		})
			ForceInline Boolean IsExist(const UString& Key) const {
			return this->lock.SharedLock([&]() -> bool InlineLambda{
				if (this->node == nullptr) return false;

				NodeLex* node = find(this->node, Key);
				return (node == nullptr || node->data == nullptr) ? false : true;
				});
		}

		/// <summary>
		/// Checking if an element with given key is in the dictionary
		/// </summary>
		/// <param name="Key">A key by which searches the element</param>
		/// <returns>"True" if element exists</returns>
		template<typename UString> requires (requires(const UString& str) {
			{str[0]}->Concepts::IsConvertible<Char>;
		})
			ForceInline Boolean IsExist(UString&& Key) const {
			return this->lock.SharedLock([&]() -> bool InlineLambda{
				if (this->node == nullptr) return false;

				NodeLex* node = find(this->node, Key);
				return (node == nullptr || node->data == nullptr) ? false : true;
				});
		}

		/// <summary>
		/// Checking if an element with given key is in the dictionary
		/// </summary>
		/// <param name="Key">A key by which searches the element</param>
		/// <returns>"True" if element exists</returns>
		template<SizeT TN>
		ForceInline Boolean IsExist(StaticTextRef<TN> Key) const {
			return IsExist(ConstString(Key));
		}

		/// <summary>
		/// Element deleting by key
		/// </summary>
		/// <param name="Key">A key by which searches the element</param>
		/// <returns>A reference to a current object</returns>
		BaseConcurencyDictionary& remove(const TString& Key) {
			return this->lock.ExclusiveLock([&]() mutable->BaseConcurencyDictionary & InlineLambda{
				bool flag = false;
				this->node = remove_node(this->node, Key, flag);

				if (flag)
					--this->count;

				return *this;
				});
		}

		/// <summary>
		/// A number of elements in dictionary
		/// </summary>
		/// <returns>A current number of elements in dictionary</returns>
		ForceInline SizeT GetCount() const {
			return this->count;
		}

		/// <summary>
		/// A current number of elements in dictionary
		/// </summary>
		SizeT Property(GetCount) Count;

		/// <summary>
		/// Elements copying in associative array
		/// </summary>
		/// <returns>An associative array with elements from a dictionary</returns>
		BaseConcurencyMap<THeap, TString, TData> ToMap() {
			return this->lock.SharedLock([&] () -> BaseConcurencyMap<THeap, TString, TData> {
				BaseConcurencyMap<THeap, TString, TData> retVal;
				ToMap(this, retVal);
				return retVal;
				});
		}

		template<typename U>
		friend void Cardinal::Swap(U& A, U& B);

		/// <summary>
		/// Dictonary object cloning
		/// </summary>
		/// <returns>Returns a new dictionary with copies of elements</returns>
		template<typename = void> requires Concepts::IsClonable<TString>&& Concepts::IsClonable<TData>
		BaseConcurencyDictionary Clone() const {
			return this->lock.SharedLock([&]() InlineLambda{
				BaseConcurencyDictionary result;
				result.node = create_lex_clone_copy(this->node);
				result.count = this->count;
				return result;
				});
		}

		/// <summary>
		/// Dictonary object cloning using lambda
		/// </summary>
		/// <typeparam name="TLambda">Lambda type</typeparam>
		/// <param name="lambda">A function with which occurs cloning</param>
		/// <returns>Returns a new dictionary with copies of elements</returns>
		template<typename TLambda> requires (Concepts::IsInvockableWithResult<TLambda, TData, const TData&>)
			BaseConcurencyDictionary Clone(TLambda&& lambda) const {
			return this->lock.SharedLock([&]() InlineLambda{
				BaseConcurencyDictionary result;
				result.node = create_lex_clone_copy(this->node, lambda);
				result.count = this->count;
				return result;
				});
		}

	protected:
#pragma region LINQ
		template<template<typename UHeap, typename TKey, typename TValue> typename TCollection, typename UHeap, typename TKey, typename TValue, typename TPair, bool IsConcurencySuportT>
		friend class LINQ::IAssociableCollection;

		template<template<typename UHeap, typename TKey, typename TValue> typename TCollection, typename UHeap, typename TKey, typename TValue, typename TPair, bool IsConcurencySuportT>
		friend class LINQ::IDictionary;

		template<template<typename, SizeT, typename> typename TCollection, typename THeap, typename T, SizeT TSize, Boolean TIsConcurencySuport>
		friend class LINQ::IFixedCollection;

		template<typename U>
		friend struct Cardinal::Serialization::SerializationHandler;

		template<
			template<typename, typename> typename TCollection,
			typename THeap,
			typename T,
			Boolean TIsDynamic,
			Boolean TIsConcurencySuport,
			Boolean TIsReadonly,
			Boolean TIsCollectionCanReturnRef
		>
		friend class LINQ::ICollection;

	protected:
#pragma region For Each
		template<typename TLambda>
		ForceInline static void ForEachImplWithoutIndex(
			NodeSym* node,
			TLambda& lambda) {
			if (node->left != nullptr) ForEachImplWithoutIndex(node->left, lambda);
			if (node->data != nullptr) ForEachImplWithoutIndex(node->data, lambda);
			if (node->right != nullptr) ForEachImplWithoutIndex(node->right, lambda);
		}

		template<typename TLambda>
		static void ForEachImplWithoutIndex(
			NodeLex* node,
			TLambda& lambda) {
			if (node->data != nullptr)
				lambda(*node->data);

			if (node->node_root != nullptr)
				ForEachImplWithoutIndex(node->node_root, lambda);
		}

		template<typename TLambda>
		ForceInline static void ForEachImplWithoutIndex(
			const BaseConcurencyDictionary* const This,
			TLambda& lambda) {
			if (This != nullptr)
				if (This->node != nullptr)
					ForEachImplWithoutIndex(This->node, lambda);
		}

		template<typename TLambda>
		ForceInline static void ForEachImplWithIndex(
			NodeSym* node,
			TLambda& lambda,
			SizeT& Index) {
			if (node->left != nullptr) ForEachImplWithIndex(node->left, lambda, Index);
			if (node->data != nullptr) ForEachImplWithIndex(node->data, lambda, Index);
			if (node->right != nullptr) ForEachImplWithIndex(node->right, lambda, Index);
		}

		template<typename TLambda>
		static void ForEachImplWithIndex(
			NodeLex* node,
			TLambda& lambda,
			SizeT& Index) {
			if (node->data != nullptr)
			{
				lambda(*node->data, Index);
				Index++;
			}

			if (node->node_root != nullptr)
				ForEachImplWithIndex(node->node_root, lambda, Index);
		}

		template<typename TLambda>
		ForceInline static void ForEachImplWithIndex(
			const BaseConcurencyDictionary* const This,
			TLambda& lambda,
			SizeT& Index) {
			if (This != nullptr)
				if (This->node != nullptr)
					ForEachImplWithIndex(This->node, lambda, Index);
		}

		template<typename LambdaT>
		ForceInline const BaseConcurencyDictionary& ForEachImpl(LambdaT&& lambda) const {
			if constexpr (TypeTraits::IsInvockable<LambdaT, const BasePair<THeap, TString, TData>&>)
			{
				return this->lock.SharedLock([&]() -> const BaseConcurencyDictionary & InlineLambda{
					ForEachImplWithoutIndex(this, lambda);
					return *this;
					});
			}
			else if constexpr (TypeTraits::IsInvockable<LambdaT, const BasePair<THeap, TString, TData>&, SizeT>)
			{
				return this->lock.SharedLock([&]() -> const BaseConcurencyDictionary & InlineLambda{
					SizeT index = 0;
					ForEachImplWithIndex(this, lambda, index);
					return *this;
					});
			}
			else
			{
				static_assert(TypeTraits::IsInvockable<LambdaT, const BasePair<THeap, TString, TData>&> ||
					TypeTraits::IsInvockable<LambdaT, const BasePair<THeap, TString, TData>&, SizeT>);
				__assume(0);
			}
		}

		template<typename LambdaT>
		ForceInline BaseConcurencyDictionary& ForEachImpl(LambdaT&& lambda) {
			if constexpr (TypeTraits::IsInvockable<LambdaT, BasePair<THeap, TString, TData>&> ||
				TypeTraits::IsInvockable<LambdaT, const BasePair<THeap, TString, TData>&>)
			{
				return this->lock.SharedLock([&]()->BaseConcurencyDictionary & InlineLambda{
					ForEachImplWithoutIndex(this, lambda);
					return *this;
					});
			}
			else if constexpr (TypeTraits::IsInvockable<LambdaT, BasePair<THeap, TString, TData>&, SizeT> ||
				TypeTraits::IsInvockable<LambdaT, const BasePair<THeap, TString, TData>&, SizeT>)
			{
				return this->lock.SharedLock([&]()->BaseConcurencyDictionary & InlineLambda{
					SizeT index = 0;
					ForEachImplWithIndex(this, lambda, index);
					return *this;
					});
			}
			else
			{
				static_assert(TypeTraits::IsInvockable<LambdaT, BasePair<THeap, TString, TData>&> ||
					TypeTraits::IsInvockable<LambdaT, const BasePair<THeap, TString, TData>&> ||
					TypeTraits::IsInvockable<LambdaT, BasePair<THeap, TString, TData>&, SizeT> ||
					TypeTraits::IsInvockable<LambdaT, const BasePair<THeap, TString, TData>&, SizeT>);
				__assume(0);
			}
		}
#pragma endregion

#pragma region For Each Key
		template<typename TLambda>
		ForceInline static void ForEachKeyImplWithoutIndex(
			NodeSym* node,
			TLambda& lambda) {
			if (node->left != nullptr) ForEachKeyImplWithoutIndex(node->left, lambda);
			if (node->data != nullptr) ForEachKeyImplWithoutIndex(node->data, lambda);
			if (node->right != nullptr) ForEachKeyImplWithoutIndex(node->right, lambda);
		}

		template<typename TLambda>
		static void ForEachKeyImplWithoutIndex(
			NodeLex* node,
			TLambda& lambda) {
			if (node->data != nullptr)
				lambda(node->data->Key);

			if (node->node_root != nullptr)
				ForEachKeyImplWithoutIndex(node->node_root, lambda);
		}

		template<typename TLambda>
		ForceInline static void ForEachKeyImplWithoutIndex(
			const BaseConcurencyDictionary* const This,
			TLambda& lambda) {
			if (This != nullptr)
				if (This->node != nullptr)
					ForEachKeyImplWithoutIndex(This->node, lambda);
		}

		template<typename TLambda>
		ForceInline static void ForEachKeyImplWithIndex(
			NodeSym* node,
			TLambda& lambda,
			SizeT& Index) {
			if (node->left != nullptr) ForEachKeyImplWithIndex(node->left, lambda, Index);
			if (node->data != nullptr) ForEachKeyImplWithIndex(node->data, lambda, Index);
			if (node->right != nullptr) ForEachKeyImplWithIndex(node->right, lambda, Index);
		}

		template<typename TLambda>
		static void ForEachKeyImplWithIndex(
			NodeLex* node,
			TLambda& lambda,
			SizeT& Index) {
			if (node->data != nullptr)
			{
				lambda(node->data->Key, Index);
				Index++;
			}

			if (node->node_root != nullptr)
				ForEachKeyImplWithIndex(node->node_root, lambda, Index);
		}

		template<typename TLambda>
		ForceInline static void ForEachKeyImplWithIndex(
			const BaseConcurencyDictionary* const This,
			TLambda& lambda,
			SizeT& Index) {
			if (This != nullptr)
				if (This->node != nullptr)
					ForEachImplWithIndex(This->node, lambda, Index);
		}

		template<typename LambdaT>
		ForceInline const BaseConcurencyDictionary& ForEachKeyImpl(LambdaT&& lambda) const {
			if constexpr (TypeTraits::IsInvockable<LambdaT, const TString&>)
			{
				return this->lock.SharedLock([&]() -> const BaseConcurencyDictionary & InlineLambda{
					ForEachKeyImplWithoutIndex(this, lambda);
					return *this;
					});
			}
			else if constexpr (TypeTraits::IsInvockable<LambdaT, const TString&, SizeT>)
			{
				return this->lock.SharedLock([&]() -> const BaseConcurencyDictionary & InlineLambda{
					SizeT index = 0;
					ForEachKeyImplWithIndex(this, lambda, index);
					return *this;
					});
			}
			else
			{
				static_assert(TypeTraits::IsInvockable<LambdaT, const TString&> ||
					TypeTraits::IsInvockable<LambdaT, const TString&, SizeT>);
				__assume(0);
			}
		}

		template<typename LambdaT>
		ForceInline BaseConcurencyDictionary& ForEachKeyImpl(LambdaT&& lambda) {
			if constexpr (TypeTraits::IsInvockable<LambdaT, const TString&>)
			{
				return this->lock.SharedLock([&]() mutable->BaseConcurencyDictionary & InlineLambda{
					ForEachKeyImplWithoutIndex(this, lambda);
					return *this;
					});
			}
			else if constexpr (TypeTraits::IsInvockable<LambdaT, const TString&, SizeT>)
			{
				return this->lock.SharedLock([&]() mutable->BaseConcurencyDictionary & InlineLambda{
					SizeT index = 0;
					ForEachKeyImplWithIndex(this, lambda, index);
					return *this;
					});
			}
			else
			{
				static_assert(TypeTraits::IsInvockable<LambdaT, const TString&> ||
					TypeTraits::IsInvockable<LambdaT, const TString&, SizeT>);
				__assume(0);
			}
		}
#pragma endregion

#pragma region For Each Value
		template<typename TLambda>
		ForceInline static void ForEachValueImplWithoutIndex(
			NodeSym* node,
			TLambda& lambda) {
			if (node->left != nullptr) ForEachValueImplWithoutIndex(node->left, lambda);
			if (node->data != nullptr) ForEachValueImplWithoutIndex(node->data, lambda);
			if (node->right != nullptr) ForEachValueImplWithoutIndex(node->right, lambda);
		}

		template<typename TLambda>
		static void ForEachValueImplWithoutIndex(
			NodeLex* node,
			TLambda& lambda) {
			if (node->data != nullptr)
				lambda(node->data->Value);

			if (node->node_root != nullptr)
				ForEachValueImplWithoutIndex(node->node_root, lambda);
		}

		template<typename TLambda>
		ForceInline static void ForEachValueImplWithoutIndex(
			const BaseConcurencyDictionary* const This,
			TLambda& lambda) {
			if (This != nullptr)
				if (This->node != nullptr)
					ForEachValueImplWithoutIndex(This->node, lambda);
		}

		template<typename TLambda>
		ForceInline static void ForEachValueImplWithIndex(
			NodeSym* node,
			TLambda& lambda,
			SizeT& Index) {
			if (node->left != nullptr) ForEachValueImplWithIndex(node->left, lambda, Index);
			if (node->data != nullptr) ForEachValueImplWithIndex(node->data, lambda, Index);
			if (node->right != nullptr) ForEachValueImplWithIndex(node->right, lambda, Index);
		}

		template<typename TLambda>
		static void ForEachValueImplWithIndex(
			NodeLex* node,
			TLambda& lambda,
			SizeT& Index) {
			if (node->data != nullptr)
			{
				lambda(node->data->Value, Index);
				Index++;
			}

			if (node->node_root != nullptr)
				ForEachValueImplWithIndex(node->node_root, lambda, Index);
		}

		template<typename TLambda>
		ForceInline static void ForEachValueImplWithIndex(
			const BaseConcurencyDictionary* const This,
			TLambda& lambda,
			SizeT& index) {
			if (This != nullptr)
				if (This->node != nullptr)
					ForEachValueImplWithIndex(This->node, lambda, index);
		}

		template<typename LambdaT>
		ForceInline const BaseConcurencyDictionary& ForEachValueImpl(LambdaT&& lambda) const {
			if constexpr (TypeTraits::IsInvockable<LambdaT, const TData&>)
			{
				return this->lock.SharedLock([&]() -> const BaseConcurencyDictionary & InlineLambda{
					ForEachValueImplWithoutIndex(this, lambda);
					return *this;
					});
			}
			else if constexpr (TypeTraits::IsInvockable<LambdaT, const TData&, SizeT>)
			{
				return this->lock.SharedLock([&]() -> const BaseConcurencyDictionary & InlineLambda{
					SizeT index = 0;
					ForEachValueImplWithIndex(this, lambda, index);
					return *this;
					});
			}
			else
			{
				static_assert(TypeTraits::IsInvockable<LambdaT, const TData&> ||
					TypeTraits::IsInvockable<LambdaT, const TData&, SizeT>);
				__assume(0);
			}
		}

		template<typename LambdaT>
		ForceInline BaseConcurencyDictionary& ForEachValueImpl(LambdaT&& lambda) {
			if constexpr (TypeTraits::IsInvockable<LambdaT, TData&> ||
				TypeTraits::IsInvockable<LambdaT, const TData&>)
			{
				return this->lock.SharedLock([&]()->BaseConcurencyDictionary & InlineLambda{
					ForEachValueImplWithoutIndex(this, lambda);
					return *this;
					});
			}
			else if constexpr (TypeTraits::IsInvockable<LambdaT, TData&, SizeT> ||
				TypeTraits::IsInvockable<LambdaT, const TData&, SizeT>)
			{
				return this->lock.SharedLock([&]()->BaseConcurencyDictionary & InlineLambda{
					SizeT index = 0;
					ForEachValueImplWithIndex(this, lambda, index);
					return *this;
					});
			}
			else
			{
				static_assert(TypeTraits::IsInvockable<LambdaT, TData&> ||
					TypeTraits::IsInvockable<LambdaT, const TData&> ||
					TypeTraits::IsInvockable<LambdaT, TData&, SizeT> ||
					TypeTraits::IsInvockable<LambdaT, const TData&, SizeT>);
				__assume(0);
			}
		}

		ForceInline void UnsafeAdd(Pair&& pair) {
			Insert(MoveRValue(pair));
		}

		ForceInline void UnsafeAdd(const Pair& pair) {
			Insert(pair);
		}
#pragma endregion

	protected:
		// Tree element destroying
		template<typename TLambda>
		NodeLex* destroy_node(NodeLex* This, TLambda& lambda) {
			if (This == nullptr)
				return nullptr;

			if (This->node_root != nullptr)
				This->node_root = destroy_tree(This->node_root, lambda);

			if (This->data != nullptr)
			{
				lambda(MoveRValue(*This->data));
				delete This->data;
			}

			delete This;
			return nullptr;
		}

		// Tree destroying
		template<typename TLambda>
		NodeSym* destroy_tree(NodeSym* This, TLambda& lambda) {
			if (This == nullptr)
				return nullptr;

			if (This->left != nullptr)
			{
				This->left = destroy_tree(This->left, lambda);
			}

			if (This->data != nullptr)
			{
				This->data = destroy_node(This->data, lambda);
			}

			if (This->right != nullptr)
			{
				This->right = destroy_tree(This->right, lambda);
			}

			delete This;
			return nullptr;
		}

		ForceInline BaseConcurencyDictionary& RemoveAllImpl() {
			return this->lock.ExclusiveLock([&]()->BaseConcurencyDictionary & InlineLambda{
				this->node = destroy_node(this->node);
				this->count = 0;
				return *this;
				});
		}

		template<typename TLambda>
		ForceInline BaseConcurencyDictionary& RemoveAllWithImpl(TLambda&& lambda) {
			return this->lock.SharedLock([&]()->BaseConcurencyDictionary & InlineLambda{
				this->node = destroy_node(this->node, lambda);
				this->count = 0;
				return *this;
				});
		}
#pragma endregion
	};

	/// <summary>
	/// Fast dictionary with the standard memory allocator and multithreading support
	/// </summary>
	template<typename T>
	using ConcurencyDictionary = BaseConcurencyDictionary<Memory::Heap, BaseString<Memory::Heap>, T, DefaultCharacterComparator>;

	/// <summary>
	/// Fast dictionary with the protected memory allocator and multithreading support
	/// </summary>
	template<typename T>
	using ConcurencySensitiveDictionary = BaseConcurencyDictionary<Memory::SensitiveDataHeap, BaseString<Memory::Heap>, T, DefaultCharacterComparator>;

	/// <summary>
	/// Fast case insesitive dictionary with the standard memory allocator and multithreading support
	/// </summary>
	template<typename T>
	using ConcurencyInsesitiveDictionary = BaseConcurencyDictionary<Memory::Heap, BaseString<Memory::Heap>, T, CaseInsesitiveCharacterComparator>;

	/// <summary>
	/// Fast case insesitive dictionary with the protected memory allocator and multithreading support
	/// </summary>
	template<typename T>
	using ConcurencyInsesitiveSensitiveDictionary = BaseConcurencyDictionary<Memory::SensitiveDataHeap, BaseString<Memory::Heap>, T, CaseInsesitiveCharacterComparator>;
}

export namespace Cardinal::TypeTraits::Collection
{
	template<typename THeap, typename TString, typename T, Cardinal::Containers::TCharacterComparator CharacterComparator>
	struct IsDictionaryT<Cardinal::Containers::BaseConcurencyDictionary<THeap, TString, T, CharacterComparator>> : LogicTrue {};
}

export namespace Cardinal
{
	template<typename THeap, typename TString, typename T, Cardinal::Containers::TCharacterComparator CharacterComparator>
	ForceInline void Swap(Containers::BaseConcurencyDictionary<THeap, TString, T, CharacterComparator>& a, Containers::BaseConcurencyDictionary<THeap, TString, T, CharacterComparator>& b) {
		a.lock.ExclusiveLock([&] () {
			b.lock.ExclusiveLock([&] () {
				Swap(a.count, b.count);
				Swap(a.node, b.node);
				});
			});
	}
}