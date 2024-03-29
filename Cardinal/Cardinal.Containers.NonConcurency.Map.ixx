export module Cardinal.Containers.NonConcurency.Map;

export import Cardinal.Core;
export import Cardinal.Memory;
export import Cardinal.Containers.LINQ;
export import Cardinal.Containers.TypeTraits;
export import Cardinal.Containers.InitilizerList;
export import Cardinal.Containers.Pair;

import Cardinal.Exceptions.Internals;

#pragma warning(push)
// This internal implementation file which can use internal API functions
#pragma warning(disable: 4996)

export namespace Cardinal::Containers
{
	template<typename THeap, typename TKey, typename TValue>
	class BaseMap :
		public LINQ::IAssociableCollection<BaseMap, THeap, TKey, TValue, BasePair<THeap, TKey, TValue>, true> {
		struct Node : public Memory::Allocator<THeap> {
		private:
			BasePair<THeap, TKey, TValue> data;

			ForceInline const TKey& GetKey() const { return data.Key; }
			ForceInline TValue& GetValue() { return data.Value; }
			ForceInline const TValue& GetValue() const { return data.Value; }
			ForceInline void SetValue(const TValue& newValue) { data.Value = newValue; }
			ForceInline void SetValue(TValue&& newValue) { data.Value = MoveRValue(newValue); }

			TKey Property(GetKey) key;
			TValue Property(GetValue, SetValue) value;

			UInt16 height;

			Node* father;
			Node* left;
			Node* right;

			BaseMap* This;

			RefCountT RefCount;

			ForceInline Node(const TKey& key, const TValue& value, Node* father, BaseMap* This) :
				data(BasePair<THeap, TKey, TValue>(key, value)),
				height(1),
				father(father),
				left(nullptr),
				right(nullptr),
				This(This),
				RefCount(1) {
			};

			ForceInline Node(TKey&& key, const TValue& value, Node* father, BaseMap* This) :
				data(BasePair<THeap, TKey, TValue>((MoveRValue(key)), value)),
				height(1),
				father(father),
				left(nullptr),
				right(nullptr),
				This(This),
				RefCount(1) {
			}

			ForceInline Node(const TKey& key, TValue&& value, Node* father, BaseMap* This) :
				data(BasePair<THeap, TKey, TValue>(key, MoveRValue(value))),
				height(1),
				father(father),
				left(nullptr),
				right(nullptr),
				This(This),
				RefCount(1) {
			};

			ForceInline Node(TKey&& key, TValue&& value, Node* father, BaseMap* This) :
				data(BasePair<THeap, TKey, TValue>(MoveRValue(key), MoveRValue(value))),
				height(1),
				father(father),
				left(nullptr),
				right(nullptr),
				This(This),
				RefCount(1) {
			};

			ForceInline Node(const BasePair<THeap, TKey, TValue>& data, Node* father, BaseMap* This) :
				data(data),
				height(1),
				father(father),
				left(nullptr),
				right(nullptr),
				This(This),
				RefCount(1) {
			};

			ForceInline Node(BasePair<THeap, TKey, TValue>&& data, Node* father, BaseMap* This) :
				data(MoveRValue(data)),
				height(1),
				father(father),
				left(nullptr),
				right(nullptr),
				This(This),
				RefCount(1) {
			};

			friend BaseMap;
			friend typename BaseMap::Iterator;
		};

	private:
		Node* root;
		SizeT count;

	public:
		~BaseMap() {
			DestroyTree(this->root);
		}

	private:
		/// <summary>
		/// Recursive element searching by key in the tree
		/// </summary>
		/// <param name = "node">The root in which is searching</param>
		/// <param name = "key">A key by which is searching</param>
		/// <returns>A tree element or <c>nullptr<c></returns>
		template<typename UKey>
		static Node* Find(Node* node, const UKey& key) {
			if (node == nullptr)
				return nullptr;

			if (node->key == key)
				return node;
			else
				if (node->key < key)
					return Find(node->right, key);
				else
					return Find(node->left, key);
		}

		/// <summary>
		/// The tree height calculation
		/// </summary>
		/// <param name = "node">An element for which is calculating</param>
		/// <returns>Height value</returns>
		static UInt16 Height(Node* node) {
			return node ? node->height : 0;
		}

		/// <summary>
		/// The height difference between right and left subelements calculation
		/// </summary>
		/// <param name = "node">A parent element</param>
		/// <returns>A negative value indicates that right has bigger height, the positive - the left has, 0 - equal by height</returns>
		static Int16 BFactor(Node* node) {
			return  Height(node->right) - Height(node->left);
		}

		/// <summary>
		/// Recalcutates the height of given element
		/// </summary>
		/// <param name = "node">An element for which is updating the height value</param>
		static void FixHeight(Node* node) {
			const UInt16 HLeft = Height(node->left);
			const UInt16 HRight = Height(node->right);

			node->height = Cardinal::Max(HLeft, HRight) + 1;
		}

		/// <summary>
		/// A right small tree rotation
		/// </summary>
		/// <param name = "node">The element relative to which the rotation occurs</param>
		/// <returns>A new parent element</returns>
		static Node* RotateRight(Node* node) {
			Node* left = node->left;
			Node* node_father = node->father;

			node->left = left->right;
			if (node->left != nullptr)
				node->left->father = node;

			left->right = node;
			if (left->right != nullptr)
				left->right->father = left;

			left->father = node_father;

			FixHeight(node);
			FixHeight(left);
			return left;
		}

		/// <summary>
		/// A left small tree rotation
		/// </summary>
		/// <param name = "node">The element relative to which the rotation occurs</param>
		/// <returns>A new parent element</returns>
		static Node* RotateLeft(Node* node) {
			Node* right = node->right;
			Node* node_father = node->father;

			node->right = right->left;
			if (node->right != nullptr)
				node->right->father = node;

			right->left = node;
			if (right->left != nullptr)
				right->left->father = right;

			right->father = node_father;

			FixHeight(node);
			FixHeight(right);
			return right;
		}

		/// <summary>
		/// Tree balancing
		/// It comes down to recalculating the height and calling the appropriate rotations
		/// Big rotations are realized through several small ones 
		/// </summary>
		/// <param name = "node">The element relative to which the balancing occurs</param>
		/// <returns>A new parent element</returns>
		static Node* Balance(Node* node) {
			FixHeight(node);
			if (BFactor(node) == 2)
			{
				if (BFactor(node->right) < 0)
					node->right = RotateRight(node->right);
				return RotateLeft(node);
			}
			if (BFactor(node) == -2)
			{
				if (BFactor(node->left) > 0)
					node->left = RotateLeft(node->left);
				return RotateRight(node);
			}
			return node;
		}

		/// <summary>
		/// A new element insertion
		/// </summary>
		/// <param name = "node">A parent element</param>
		/// <param name = "key">A new key</param>
		/// <param name = "value">A new value</param>
		/// <returns>A new parent element</returns>
		/// <exception cref = "Rem::KeyAllredyExistException">A key exists only in associative array</exception>
		Node* Insert(Node* node, const TKey& key, const TValue& value, Node* father) {
			if (!node) return new Node(key, value, father, this);

			if (key < node->key)
				node->left = Insert(node->left, key, value, node);
			else
				if (key != node->key)
					node->right = Insert(node->right, key, value, node);
				else
					Internals::Exceptions::ThrowKeyAlredyExist(SourcePosition);
			return Balance(node);
		}

		/// <summary>
		/// A new element insertion
		/// </summary>
		/// <param name = "node">A parent element</param>
		/// <param name = "key">A new key</param>
		/// <param name = "value">A new value</param>
		/// <returns>A new parent element</returns>
		/// <exception cref = "Rem::KeyAllredyExistException">A key exists only in associative array</exception>
		Node* Insert(Node* node, TKey&& key, const TValue& value, Node* father) {
			if (!node) return new Node(MoveRValue(key), value, father, this);

			if (key < node->key)
				node->left = Insert(node->left, MoveRValue(key), value, node);
			else
				if (key != node->key)
					node->right = Insert(node->right, MoveRValue(key), value, node);
				else
					Internals::Exceptions::ThrowKeyAlredyExist(SourcePosition);

			return Balance(node);
		}

		/// <summary>
		/// A new element insertion
		/// </summary>
		/// <param name = "node">A parent element</param>
		/// <param name = "key">A new key</param>
		/// <param name = "value">A new value</param>
		/// <returns>A new parent element</returns>
		/// <exception cref = "Rem::KeyAllredyExistException">A key exists only in associative array</exception>
		Node* Insert(Node* node, const TKey& key, TValue&& value, Node* father) {
			if (!node) return new Node(key, MoveRValue(value), father, this);

			if (key < node->key)
				node->left = Insert(node->left, key, MoveRValue(value), node);
			else
				if (key != node->key)
					node->right = Insert(node->right, key, MoveRValue(value), node);
				else
					Internals::Exceptions::ThrowKeyAlredyExist(SourcePosition);

			return Balance(node);
		}

		/// <summary>
		/// A new element insertion
		/// </summary>
		/// <param name = "node">A parent element</param>
		/// <param name = "key">A new key</param>
		/// <param name = "value">A new value</param>
		/// <returns>A new parent element</returns>
		/// <exception cref = "Rem::KeyAllredyExistException">A key exists only in associative array</exception>
		Node* Insert(Node* node, TKey&& key, TValue&& value, Node* father) {
			if (!node) return new Node(MoveRValue(key), MoveRValue(value), father, this);

			if (key < node->key)
				node->left = Insert(node->left, MoveRValue(key), MoveRValue(value), node);
			else
				if (key != node->key)
					node->right = Insert(node->right, MoveRValue(key), MoveRValue(value), node);
				else
					Internals::Exceptions::ThrowKeyAlredyExist(SourcePosition);

			return Balance(node);
		}

		/// <summary>
		/// A new element insertion
		/// </summary>
		/// <param name = "data">A key and value values</param>
		/// <param name = "value">A new value</param>
		/// <returns>A new parent element</returns>
		/// <exception cref = "Rem::KeyAllredyExistException">A key exists only in associative array</exception>
		Node* Insert(Node* node, const BasePair<THeap, TKey, TValue>& pair, Node* father) {
			if (!node) return new Node(pair, father, this);

			if (pair.Key < node->key)
				node->left = Insert(node->left, pair, node);
			else
				if (pair.Key != node->key)
					node->right = Insert(node->right, pair, node);
				else
					Internals::Exceptions::ThrowKeyAlredyExist(SourcePosition);

			return Balance(node);
		}

		/// <summary>
		/// A new element insertion
		/// </summary>
		/// <param name = "data">A key and value values</param>
		/// <param name = "value">A new value</param>
		/// <returns>A new parent element</returns>
		/// <exception cref = "Rem::KeyAllredyExistException">A key exists only in associative array</exception>
		Node* Insert(Node* node, BasePair<THeap, TKey, TValue>&& pair, Node* father) {
			if (!node) return new Node(MoveRValue(pair), father, this);

			if (pair.Key < node->key)
				node->left = Insert(node->left, MoveRValue(pair), node);
			else
				if (pair.Key != node->key)
					node->right = Insert(node->right, MoveRValue(pair), node);
				else
					Internals::Exceptions::ThrowKeyAlredyExist(SourcePosition);

			return Balance(node);
		}

		/// <summary>
		/// A recursive minimal element searching in tree
		/// </summary>
		/// <param name = "node">A root element</param>
		/// <returns>A minimal element</returns>
		static Node* FindMin(Node* node) {
			return node->left ? FindMin(node->left) : node;
		}

		/// <summary>
		/// A recursive maximal element searching in tree
		/// </summary>
		/// <param name = "node">A root element</param>
		/// <returns>A maximal element</returns>
		static Node* FindMax(Node* node) {
			return node->right ? FindMax(node->right) : node;
		}

		/// <summary>
		/// A recursive minimal element deletion
		/// </summary>
		/// <param name = "node">A parent element</param>
		/// <returns>A parent element</returns>
		Node* RemoveMin(Node* node) {
			if (node->left == nullptr)
				return node->right;

			node->left = RemoveMin(node->left);
			return Balance(node);
		}

		/// <summary>
		/// A recursive deletion by key
		/// </summary>
		/// <param name = "node">A parent element</param>
		/// <param name = "key">Key</param>
		/// <returns>A parent element</returns>
		template<typename UKey>
		Node* Remove(Node* node, const UKey& key) {
			if (!node) return nullptr;

			if (node->key > key)
				node->left = Remove(node->left, key);
			else if (node->key < key)
				node->right = Remove(node->right, key);
			else
			{
				Node* left = node->left;
				Node* right = node->right;
				delete node;
				if (!right) return left;
				Node* min = FindMin(right);
				min->right = RemoveMin(right);
				min->left = left;
				return Balance(min);
			}

			return Balance(node);
		}

		/// <summary>
		/// A recursive tree destroying
		/// </summary>
		/// <param name = "root">A parent element</param>
		Node* DestroyTree(Node* root) //-V688
		{
			if (root == nullptr)
				return nullptr;

			if (root->left != nullptr)
			{
				root->left = DestroyTree(root->left);
			}

			if (root->right != nullptr)
			{
				root->right = DestroyTree(root->right);
			}

			if (!root->RefCount.DecrementAndCheckIsZero())
			{
				root->father = root->left = root->right = nullptr;
				root->This = nullptr;
			}
			else
			{
				delete root;
			}

			return nullptr;
		}

		/// <summary>
		/// A recursive tree copying
		/// </summary>
		/// <param name = "node">Old tree element</param>
		/// <param name = "parrent">A parent element in a new tree</param>
		/// <param name = "owner">A reference to a new tree</param>
		/// <returns>A reference to a new element with subelements</returns>
		Node* copyTree(Node* node, Node* parrent, BaseMap* owner) const {
			Node* newNode = new Node(node->key, node->value, parrent, owner);
			newNode->height = node->height;

			if (node->left != nullptr)
				newNode->left = copyTree(node->left, newNode, owner);
			if (node->right != nullptr)
				newNode->right = copyTree(node->right, newNode, owner);

			return newNode;
		}

		/// <summary>
		/// A recursive tree cloning
		/// </summary>
		/// <param name = "node">Old tree element</param>
		/// <param name = "parrent">A parent element in a new tree</param>
		/// <param name = "owner">A reference to a new tree</param>
		/// <returns>A reference to a new element with subelements</returns>
		Node* cloneTree(Node* node, Node* parrent, BaseMap* owner) const {
			Node* newNode = new Node(Cardinal::Clone(node->key), Cardinal::Clone(node->value), parrent, owner);
			newNode->height = node->height;

			if (node->left != nullptr)
				newNode->left = cloneTree(node->left, newNode, owner);
			if (node->right != nullptr)
				newNode->right = cloneTree(node->right, newNode, owner);

			return newNode;
		}

		/// <summary>
		/// A recursive tree cloning
		/// </summary>
		/// <param name = "node">Old tree element</param>
		/// <param name = "parrent">A parent element in a new tree</param>
		/// <param name = "owner">A reference to a new tree</param>
		/// <returns>A reference to a new element with subelements</returns>
		template<typename TLambda>
		Node* cloneTree(Node* node, Node* parrent, BaseMap* owner, TLambda&& lambda) const {
			Node* newNode = new Node(lambda(node->data), parrent, owner);
			newNode->height = node->height;

			if (node->left != nullptr)
				newNode->left = cloneTree(node->left, newNode, owner, lambda);
			if (node->right != nullptr)
				newNode->right = cloneTree(node->right, newNode, owner, lambda);

			return newNode;
		}

	private:
		/// <summary>
		/// Next element
		/// </summary>
		/// <param name = "node">A current element</param>
		/// <returns>Next element or "<c>nullptr</c>"</returns>
		/// <exception cref = "Rem::NullPointException">If "<c>node == nullptr</c>"</exception>
		static Node* Next(Node* node) {
			if (node == nullptr)
				Internals::Exceptions::ThrowNullPointException(SourcePosition);

			if (node->This != nullptr)
			{
				--node->RefCount;
				if (node->right != nullptr)
				{
					Node* ret = FindMin(node->right);
					++ret->RefCount++;
					return ret;
				}

				Node* father = node->father;
				while (father != nullptr && node == father->right)
				{
					node = father;
					father = father->father;
				}

				if (father != nullptr)
					++father->RefCount;

				return father;
			}
			else
			{
				if (--node->RefCount == 0)
				{
					delete node;
				}
				return nullptr;
			}
		}

		/// <summary>
		/// Previous element
		/// </summary>
		/// <param name = "node">A current element</param>
		/// <returns>Previous element or "<c>nullptr</c>"</returns>
		/// <exception cref = "Rem::NullPointException">If "<c>node == nullptr</c>"</exception>
		static Node* Prev(Node* node) {
			if (node == nullptr)
				Internals::Exceptions::ThrowNullPointException(SourcePosition);

			if (node->This != nullptr)
			{
				--node->RefCount;
				if (node->left != nullptr)
				{
					Node* ret = FindMax(node->left);
					++ret->RefCount;
					return ret;
				}

				Node* father = node->father;
				while (father != nullptr && node == father->left)
				{
					node = father;
					father = father->father;
				}

				if (father != nullptr)
					++father->RefCount;

				return father;
			}
			else
			{
				if (--node->RefCount == 0)
				{
					delete node;
				}

				return nullptr;
			}
		}

		Node* moveTree(Node* node, BaseMap* owner) {
			if (node->left != nullptr)
				node->left = moveTree(node->left, owner);

			if (node->right != nullptr)
				node->right = moveTree(node->right, owner);

			node->This = owner;
			return node;
		}

	public:
		class Iterator;

		/// <summary>
		/// An iterator to an element in associative array
		/// </summary>
		class ConstIterator final {
		private:
			friend class BaseMap;
			friend class Iterator;

		private:
			Node* item;

		private:
			ForceInline ConstIterator(Node* item) : item(item) { if (item != nullptr) ++item->RefCount; };

		public:
			/// <summary>
			/// A constructor
			/// </summary>
			ForceInline ConstIterator() : item(nullptr) {};

			/// <summary>
			/// Moves to the next element
			/// </summary>
			/// <exception cref = "Rem::NullPointException">If there is null-iterator "</c>"</exception>
			/// <returns>A reference to this iterator</returns>
			ForceInline ConstIterator& operator++() {
				this->item = Next(item);
				return *this;
			}

			/// <summary>
			/// Moves to the previous element
			/// </summary>
			/// <exception cref = "Rem::NullPointException">If there is null-iterator "</c>"</exception>
			/// <returns>A reference to this iterator</returns>
			ForceInline ConstIterator& operator--() {
				this->item = Prev(item);
				return *this;
			}

			/// <summary>
			/// Moves to the next element
			/// </summary>
			/// <exception cref = "Rem::NullPointException">If there is null-iterator "</c>"</exception>
			/// <returns>A reference to this iterator</returns>
			ForceInline ConstIterator& operator++(int) {
				return ConstIterator(Next(item));
			}

			/// <summary>
			/// Moves to the previous element
			/// </summary>
			/// <exception cref = "Rem::NullPointException">If there is null-iterator "</c>"</exception>
			/// <returns>A reference to this iterator</returns>
			ForceInline ConstIterator& operator--(int) {
				return ConstIterator(Prev(item));
			}

			/// <summary>
			/// The iterators comparing
			/// </summary>
			/// <param name = "other">Left operand</param>
			/// <returns>"<c>true</c>" if not equal</returns>
			ForceInline bool operator!=(const ConstIterator& other) {
				if (this->item == nullptr && other.item == nullptr)
					return false;
				else if (this->item == nullptr || other.item == nullptr)
					return true;
				else
					return this->item->key != other.item->key;
			}

			/// <summary>
			/// Comparing with null-iterator
			/// </summary>
			/// <param name = "other">"<c>nullptr</c>"</param>
			/// <returns>"<c>true</c>" if there is null-iterator</returns>
			ForceInline bool operator!=(const NullptrT&) {
				return this->item != nullptr;
			}

			/// <summary>
			/// An element access operator
			/// </summary>
			/// <returns>Returns the reference to an element</returns>
			ForceInline const auto* const operator->() {
				return &this->item->data;
			};

			/// <summary>
			/// An element access operator
			/// </summary>
			/// <returns>Returns the reference to an element</returns>
			ForceInline const auto& operator*() {
				return this->item->data;
			}

			/// <summary>
			/// A constant element access operator
			/// </summary>
			/// <returns>Returns the reference to an element</returns>
			ForceInline const BasePair<THeap, TKey, TValue>* const operator->() const {
				return &this->item->data;
			};

			/// <summary>
			/// A constant element access operator
			/// </summary>
			/// <returns>Returns the reference to an element</returns>
			ForceInline const auto& operator*() const {
				return this->item->data;
			}

			template<typename U>
			friend void Cardinal::Swap(U& A, U& B);

#pragma region Ctor, Dtor, copy operators
			/// <summary>
			/// Copying constructor
			/// </summary>
			/// <param name="iterator">Original</param>
			ForceInline ConstIterator(const ConstIterator& iterator) : item(iterator.item) {
				if (item != nullptr)
					++item->RefCount;
			}

			/// <summary>
			/// Moving constructor
			/// </summary>
			/// <param name="iterator">Original</param>
			ForceInline ConstIterator(ConstIterator&& iterator) : item(iterator.item) {
				iterator.item = nullptr;
			}

			/// <summary>
			/// Copying operator
			/// </summary>
			/// <param name="iterator">Original</param>
			/// <returns>A reference to a current element</returns>
			ForceInline ConstIterator& operator=(const ConstIterator& iterator) {
				this->~ConstIterator();
				this->ConstIterator::ConstIterator(iterator);
				return *this;
			}

			/// <summary>
			/// Moving operator
			/// </summary>
			/// <param name="iterator">Original</param>
			/// <returns>A reference to a current element</returns>
			ForceInline ConstIterator& operator=(ConstIterator&& iterator) {
				this->~ConstIterator();
				this->ConstIterator::ConstIterator(MoveRValue(iterator));
				return *this;
			}

			/// <summary>
			/// Destructor
			/// </summary>
			ForceInline ~ConstIterator() {
				if (item != nullptr)
				{
					if (--item->RefCount == 0)
					{
						delete item;
					}
					item = nullptr;
				}
			}
#pragma endregion
		};

		friend ConstIterator;

		/// <summary>
		/// Returns the iterator to the first element
		/// </summary>
		/// <returns>The iterator to the first element</returns>
		ForceInline ConstIterator GetRoot() const {
			return ConstIterator(FindMin(this->root));
		}

		/// <summary>
		/// Returns the iterator to last element
		/// </summary>
		/// <returns>The iterator to last element</v>
		ForceInline ConstIterator GetLast() const {
			return ConstIterator(FindMax(this->root));
		}

		/// <summary>
		/// Returns the iterator to the first element
		/// </summary>
		/// <returns>The iterator to the first element</returns>
		ForceInline ConstIterator begin() const {
			return GetRoot();
		}

		/// <summary>
		/// Returns the iterator to last element
		/// </summary>
		/// <returns>The iterator to last element</returns>
		ForceInline const ConstIterator end() const {
			return ConstIterator(nullptr);
		}

	public:
		/// <summary>
		/// An iterator to an element in associative array
		/// </summary>
		class Iterator final {
		private:
			friend BaseMap;

		private:
			Node* item;

		private:
			ForceInline Iterator(Node* item) : item(item) { if (item != nullptr) ++item->RefCount; };

		public:
			/// <summary>
			/// A constructor
			/// </summary>
			ForceInline Iterator() : item(nullptr) {};

			/// <summary>
			/// Moves to the next element
			/// </summary>
			/// <exception cref = "Rem::NullPointException">If there is null-iterator "</c>"</exception>
			/// <returns>A reference to this iterator</returns>
			ForceInline Iterator& operator++() {
				this->item = Next(item);
				return *this;
			}

			/// <summary>
			/// Moves to the previous element
			/// </summary>
			/// <exception cref = "Rem::NullPointException">If there is null-iterator "</c>"</exception>
			/// <returns>A reference to this iterator</returns>
			ForceInline Iterator& operator--() {
				this->item = Prev(item);
				return *this;
			}

			/// <summary>
			/// Moves to the next element
			/// </summary>
			/// <exception cref = "Rem::NullPointException">If there is null-iterator "</c>"</exception>
			/// <returns>A reference to this iterator</returns>
			ForceInline Iterator& operator++(int) {
				return Iterator(Next(item));
			}

			/// <summary>
			/// Moves to the previous element
			/// </summary>
			/// <exception cref = "Rem::NullPointException">If there is null-iterator "</c>"</exception>
			/// <returns>A reference to this iterator</returns>
			ForceInline Iterator& operator--(int) {
				return Iterator(Prev(item));
			}

			/// <summary>
			/// The iterators comparing
			/// </summary>
			/// <param name = "other">Left operand</param>
			/// <returns>"<c>true</c>" if not equal</returns>
			ForceInline bool operator!=(const Iterator& other) {
				if (this->item == nullptr && other.item == nullptr)
					return false;
				else if (this->item == nullptr || other.item == nullptr)
					return true;
				else
					return this->item->key != other.item->key;
			}

			/// <summary>
			/// Comparing with null-iterator
			/// </summary>
			/// <param name = "other">"<c>nullptr</c>"</param>
			/// <returns>"<c>true</c>" if there is null-iterator</returns>
			ForceInline bool operator!=(const NullptrT&) {
				return this->item != nullptr;
			}

			/// <summary>
			/// An element access operator
			/// </summary>
			/// <returns>Returns the reference to an element</returns>
			ForceInline auto* operator->() {
				return &this->item->data;
			};

			/// <summary>
			/// An element access operator
			/// </summary>
			/// <returns>Returns the reference to an element</returns>
			ForceInline auto& operator*() {
				return this->item->data;
			}

			/// <summary>
			/// A constant element access operator
			/// </summary>
			/// <returns>Returns the reference to an element</returns>
			ForceInline const BasePair<THeap, TKey, TValue>* const operator->() const {
				return &this->item->data;
			};

			/// <summary>
			/// A constant element access operator
			/// </summary>
			/// <returns>Returns the reference to an element</returns>
			ForceInline const auto& operator*() const {
				return this->item->data;
			}

			/// <summary>
			/// Transforming operator into a constant iterator
			/// </summary>
			ForceInline operator ConstIterator() const { return ConstIterator(item); }

			template<typename U>
			friend void Cardinal::Swap(U& A, U& B);

#pragma region Ctor, Dtor, copy operators
			/// <summary>
			/// Copying constructor
			/// </summary>
			/// <param name="iterator">Original</param>
			ForceInline Iterator(const Iterator& iterator) : item(iterator.item) {
				if (item != nullptr)
					++item->RefCount;
			}

			/// <summary>
			/// Moving constructor
			/// </summary>
			/// <param name="iterator">Original</param>
			ForceInline Iterator(Iterator&& iterator) : item(iterator.item) {
				iterator.item = nullptr;
			}

			/// <summary>
			/// Copying operator
			/// </summary>
			/// <param name="iterator">Original</param>
			/// <returns>A reference to a current element</returns>
			ForceInline Iterator& operator=(const Iterator& iterator) {
				this->~Iterator();
				this->Iterator::Iterator(iterator);
				return *this;
			}

			/// <summary>
			/// Moving operator
			/// </summary>
			/// <param name="iterator">Original</param>
			/// <returns>A reference to a current element</returns>
			ForceInline Iterator& operator=(Iterator&& iterator) {
				this->~Iterator();
				this->Iterator::Iterator(MoveRValue(iterator));
				return *this;
			}

			/// <summary>
			/// Destructor
			/// </summary>
			ForceInline ~Iterator() {
				if (item != nullptr)
				{
					if (--item->RefCount == 0)
					{
						delete item;
					}
					item = nullptr;
				}
			}
#pragma endregion
		};

		friend Iterator;

		/// <summary>
		/// Returns the iterator to the first element
		/// </summary>
		/// <returns>The iterator to the first element</returns>
		ForceInline Iterator GetRoot() {
			return Iterator(FindMin(this->root));
		}

		/// <summary>
		/// Returns the iterator to last element
		/// </summary>
		/// <returns>The iterator to last element</returns>
		ForceInline Iterator GetLast() {
			return Iterator(FindMax(this->root));
		}

		/// <summary>
		/// Returns the iterator to the first element
		/// </summary>
		/// <returns>The iterator to the first element</returns>
		ForceInline Iterator begin() {
			return GetRoot();
		}

		/// <summary>
		/// Returns the iterator to last element
		/// </summary>
		/// <returns>The iterator to last element</returns>
		ForceInline Iterator end() {
			return Iterator(nullptr);
		}

	public:
		/// <summary>
		/// A constructor
		/// </summary>
		ForceInline BaseMap() : root(nullptr), count(0) {}

		/// <summary>
		/// A constructor
		/// </summary>
		ForceInline BaseMap(InitilizerList<BasePair<THeap, TKey, TValue>>&& pairs) : root(nullptr), count(0) {
			for (auto&& it : pairs)
			{
				Insert(MoveRValue(it));
			}
		}

		/// <summary>
		/// Copying constructor
		/// </summary>
		/// <param name="original">Original</param>
		BaseMap(const BaseMap& original) : root(nullptr), count(original.count) {
			if (count != 0)
			{
				root = copyTree(original.root, nullptr, this);
			}
		}

		/// <summary>
		/// Moving constructor
		/// </summary>
		/// <param name="original">Original</param>
		BaseMap(BaseMap&& original) : root(original.root), count(original.count) {
			original.root = nullptr;
			original.count = 0;

			if (root != nullptr)
				root = moveTree(root, this);
		}

		/// <summary>
		/// Copying operator
		/// </summary>
		/// <param name="original">Original</param>
		/// <returns>"LValue" reference to array</returns>
		BaseMap& operator=(const BaseMap& original) {
			this->~BaseMap();
			this->BaseMap::BaseMap(original);
			return *this;
		}

		/// <summary>
		/// Moving operator
		/// </summary>
		/// <param name="original">Original</param>
		/// <returns>"RValue" reference to array</returns>
		BaseMap& operator=(BaseMap&& original) {
			this->~BaseMap();
			this->BaseMap::BaseMap(MoveRValue(original));
			return *this;
		}

		template<typename U>
		friend void Cardinal::Swap(U& A, U& B);

		/// <summary>
		/// Returns the number of elements in associative array
		/// </summary>
		ForceInline SizeT GetCount() const { return this->count; }

		/// <value>A number of elements in associative array</value>
		SizeT Property(GetCount) Count;

		/// <summary>
		/// A new element insertion
		/// </summary>
		/// <param name = "pair">A key-value pair</param>
		/// <exception cref = "Rem::KeyAllredyExistException">A key exists only in associative array</exception>
		ForceInline void Insert(const BasePair<THeap, TKey, TValue>& pair) {
			this->root = Insert(this->root, pair, nullptr);
			++this->count;
		}

		/// <summary>
		/// A new element insertion
		/// </summary>
		/// <param name = "pair">A key-value pair</param>
		/// <exception cref = "Rem::KeyAllredyExistException">A key exists only in associative array</exception>
		ForceInline void Insert(BasePair<THeap, TKey, TValue>&& pair) {
			this->root = Insert(this->root, pair, nullptr);
			++this->count;
		}

		/// <summary>
		/// A new element insertion
		/// </summary>
		/// <param name = "key">A key of new element</param>
		/// <param name = "value">A value of new element</param>
		/// <exception cref = "Rem::KeyAllredyExistException">A key exists only in associative array</exception>
		ForceInline void Insert(const TKey& key, const TValue& value) {
			this->root = Insert(this->root, key, value, nullptr);
			++this->count;
		}

		/// <summary>
		/// A new element insertion
		/// </summary>
		/// <param name = "key">A key of new element</param>
		/// <param name = "value">A value of new element</param>
		/// <exception cref = "Rem::KeyAllredyExistException">A key exists only in associative array</exception>
		ForceInline void Insert(TKey&& key, const TValue& value) {
			this->root = Insert(this->root, MoveRValue(key), value, nullptr);
			++this->count;
		}

		/// <summary>
		/// A new element insertion
		/// </summary>
		/// <param name = "key">A key of new element</param>
		/// <param name = "value">A value of new element</param>
		/// <exception cref = "Rem::KeyAllredyExistException">A key exists only in associative array</exception>
		ForceInline void Insert(const TKey& key, TValue&& value) {
			this->root = Insert(this->root, key, MoveRValue(value), nullptr);
			++this->count;
		}

		/// <summary>
		/// A new element insertion
		/// </summary>
		/// <param name = "key">A key of new element</param>
		/// <param name = "value">A value of new element</param>
		/// <exception cref = "Rem::KeyAllredyExistException">A key exists only in associative array</exception>
		ForceInline void Insert(TKey&& key, TValue&& value) {
			this->root = Insert(this->root, MoveRValue(key), MoveRValue(value), nullptr);
			++this->count;
		}

		/// <summary>
		/// Elements deletion by value
		/// </summary>
		/// <param name = "key">A key relative to which the deletion will occur</param>
		template<typename UKey, typename V = TypeTraits::EnableIfV<TypeTraits::IsComparableV<TKey, UKey>, void>>
		ForceInline void Delete(const UKey& key) {
			--this->count;
			this->root = Remove(this->root, key);
		}

		/// <summary>
		/// Gaining value by key
		/// </summary>
		/// <param name = "key">A key relative to which the value searching will occur</param>
		/// <returns>A reference to value</returns>
		/// <exception cref = "Rem::NullPointException">There is no value by this key in array</exception>
		template<typename UKey, typename V = TypeTraits::EnableIfV<TypeTraits::IsComparableV<TKey, UKey>, void>>
		ForceInline TValue& operator[](const UKey& key) {
			Node* node = Find(this->root, key);

			if (node == nullptr)
				Internals::Exceptions::ThrowNotFound(SourcePosition);

			return node->value;
		}

		/// <summary>
		/// Gaining value by key
		/// </summary>
		/// <param name = "key">A key relative to which the value searching will occur</param>
		/// <returns>Constant reference to value</returns>
		/// <exception cref = "Rem::NullPointException">There is no value by this key in array</exception>
		template<typename UKey, typename V = TypeTraits::EnableIfV<TypeTraits::IsComparableV<TKey, UKey>, void>>
		ForceInline const TValue& operator[](const UKey& key) const {
			Node* node = Find(this->root, key);

			if (node == nullptr)
				Internals::Exceptions::ThrowNotFound(SourcePosition);

			return node->value;
		}

		/// <summary>
		/// Checking if element with given key exists
		/// </summary>
		/// <param name="key">Key</param>
		/// <returns>"True" if element exists</returns>
		template<typename UKey, typename V = TypeTraits::EnableIfV<TypeTraits::IsComparableV<TKey, UKey>, void>>
		ForceInline bool IsExist(const UKey& key) const {
			return Find(this->root, key) != nullptr;
		}

		/// <summary>
		/// Checking if element with given key exists
		/// </summary>
		/// <param name="key">Key</param>
		/// <returns>"True" if element exists</returns>
		template<typename UKey, typename V = TypeTraits::EnableIfV<TypeTraits::IsComparableV<TKey, UKey>, void>>
		ForceInline bool IsExist(UKey&& key) const {
			return Find(this->root, key) != nullptr;
		}

		/// <summary>
		/// An object cloning
		/// </summary>
		/// <returns>Returns new object with copy of all elements</returns>
		ForceInline BaseMap Clone() const
			requires (Concepts::IsClonable<TKey>&& Concepts::IsClonable<TValue>) {
			BaseMap result;
			result.count = this->count;
			result.root = cloneTree(this->root, nullptr, &result);
			return result;
		}

		/// <summary>
		/// An object cloning using custom lambda or functor
		/// </summary>
		/// <typeparam name="TLambda">Lambda type</typeparam>
		/// <returns>Returns new object with copy of all elements</returns>
		template<typename TLambda>
			requires (Concepts::IsInvockableWithResult<TLambda, BasePair<THeap, TKey, TValue>, const BasePair<THeap, TKey, TValue>&>)
		ForceInline BaseMap Clone() const {
			BaseMap result;
			result.count = this->count;
			result.root = cloneTree(this->root, nullptr, &result);
			return result;
		}

	protected:
		template<template<typename THeap, typename TKey, typename TValue> typename TCollection, typename THeap, typename TKey, typename TValue, typename TPair, bool IsConcurencySuportT>
		friend class LINQ::IAssociableCollection;

		template<template<typename THeap, typename TKey, typename TValue> typename TCollection, typename THeap, typename TKey, typename TValue, typename TPair, bool IsConcurencySuportT>
		friend class LINQ::IMap;

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
#pragma region ForEach Pair
		template<typename Lambda>
		static void ForEachImplWithIndex(Node* node, SizeT& Index, Lambda& lambda) {
			if (node->left != nullptr)
				ForEachImplWithIndex(node->left, Index, lambda);

			lambda(node->data, Index); Index++;

			if (node->right != nullptr)
				ForEachImplWithIndex(node->right, Index, lambda);
		}

		template<typename Lambda>
		static void ForEachImplWithoutIndex(Node* node, Lambda& lambda) {
			if (node->left != nullptr)
				ForEachImplWithoutIndex(node->left, lambda);

			lambda(node->data);

			if (node->right != nullptr)
				ForEachImplWithoutIndex(node->right, lambda);
		}

		template<typename TLambda>
		const BaseMap& ForEachImpl(TLambda&& lambda) const {
			if constexpr (TypeTraits::IsInvockable<TLambda, const BasePair<THeap, TKey, TValue>&>)
			{
				auto node = this->root;
				if (node != nullptr)
				{
					ForEachImplWithoutIndex(node, lambda);
				}
				return *this;
			}
			else if constexpr (TypeTraits::IsInvockable<TLambda, const BasePair<THeap, TKey, TValue>&, SizeT>)
			{
				auto node = this->root;
				if (node != nullptr)
				{
					SizeT index = 0;
					ForEachImplWithIndex(node, index, lambda);
				}
				return *this;
			}
			else
			{
				static_assert(TypeTraits::IsInvockable<TLambda, const BasePair<THeap, TKey, TValue>&> ||
					TypeTraits::IsInvockable<TLambda, const BasePair<THeap, TKey, TValue>&, SizeT>);
			}
		}

		template<typename TLambda>
		BaseMap& ForEachImpl(TLambda&& lambda) {
			if constexpr (TypeTraits::IsInvockable<TLambda, BasePair<THeap, TKey, TValue>&> ||
				TypeTraits::IsInvockable<TLambda, const BasePair<THeap, TKey, TValue>&>)
			{
				auto node = this->root;
				if (node != nullptr)
				{
					ForEachImplWithoutIndex(node, lambda);
				}
				return *this;
			}
			else if constexpr (TypeTraits::IsInvockable<TLambda, BasePair<THeap, TKey, TValue>&, SizeT> ||
				TypeTraits::IsInvockable<TLambda, const BasePair<THeap, TKey, TValue>&, SizeT>)
			{
				auto node = this->root;
				if (node != nullptr)
				{
					SizeT index = 0;
					ForEachImplWithIndex(node, index, lambda);
				}
				return *this;
			}
			else
			{
				static_assert(TypeTraits::IsInvockable<TLambda, BasePair<THeap, TKey, TValue>&> ||
					TypeTraits::IsInvockable<TLambda, const BasePair<THeap, TKey, TValue>&> ||
					TypeTraits::IsInvockable<TLambda, BasePair<THeap, TKey, TValue>&, SizeT> ||
					TypeTraits::IsInvockable<TLambda, const BasePair<THeap, TKey, TValue>&, SizeT>);
			}
		}
#pragma endregion

#pragma region For Each Key
		template<typename Lambda>
		static void ForEachKeyImplWithIndex(Node* node, SizeT& Index, Lambda& lambda) {
			if (node->left != nullptr)
				ForEachKeyImplWithIndex(node->left, Index, lambda);

			lambda(node->data.Key, Index); Index++;

			if (node->right != nullptr)
				ForEachKeyImplWithIndex(node->right, Index, lambda);
		}

		template<typename Lambda>
		static void ForEachKeyImplWithoutIndex(Node* node, Lambda& lambda) {
			if (node->left != nullptr)
				ForEachKeyImplWithoutIndex(node->left, lambda);

			lambda(node->data.Key);

			if (node->right != nullptr)
				ForEachKeyImplWithoutIndex(node->right, lambda);
		}

		template<typename TLambda>
		BaseMap& ForEachKeyImpl(TLambda&& lambda) {
			if constexpr (TypeTraits::IsInvockable<TLambda, const TKey&>)
			{
				auto node = this->root;
				if (node != nullptr)
				{
					ForEachKeyImplWithoutIndex(node, lambda);
				}
				return *this;
			}
			else if constexpr (TypeTraits::IsInvockable<TLambda, const TKey&, SizeT>)
			{
				auto node = this->root;
				if (node != nullptr)
				{
					SizeT index = 0;
					ForEachKeyImplWithIndex(node, index, lambda);
				}
				return *this;
			}
			else
			{
				static_assert(TypeTraits::IsInvockable<TLambda, const TKey&> ||
					TypeTraits::IsInvockable<TLambda, const TKey&, SizeT>);
			}
		}

		template<typename TLambda>
		const BaseMap& ForEachKeyImpl(TLambda&& lambda) const {
			if constexpr (TypeTraits::IsInvockable<TLambda, const TKey&>)
			{
				auto node = this->root;
				if (node != nullptr)
				{
					ForEachKeyImplWithoutIndex(node, lambda);
				}
				return *this;
			}
			else if constexpr (TypeTraits::IsInvockable<TLambda, const TKey&, SizeT>)
			{
				auto node = this->root;
				if (node != nullptr)
				{
					SizeT index = 0;
					ForEachKeyImplWithIndex(node, index, lambda);
				}
				return *this;
			}
			else
			{
				static_assert(TypeTraits::IsInvockable<TLambda, const TKey&> ||
					TypeTraits::IsInvockable<TLambda, const TKey&, SizeT>);
			}
		}
#pragma endregion

#pragma region For Each Value
		template<typename Lambda>
		static void ForEachValueImplWithIndex(Node* node, SizeT& Index, Lambda& lambda) {
			if (node->left != nullptr)
				ForEachValueImplWithIndex(node->left, Index, lambda);

			lambda(node->data.Value, Index); Index++;

			if (node->right != nullptr)
				ForEachValueImplWithIndex(node->right, Index, lambda);
		}

		template<typename Lambda>
		static void ForEachValueImplWithoutIndex(Node* node, Lambda& lambda) {
			if (node->left != nullptr)
				ForEachValueImplWithoutIndex(node->left, lambda);

			lambda(node->data.Value);

			if (node->right != nullptr)
				ForEachValueImplWithoutIndex(node->right, lambda);
		}

		template<typename TLambda>
		const BaseMap& ForEachValueImpl(TLambda&& lambda) const {
			if constexpr (TypeTraits::IsInvockable<TLambda, const TValue&>)
			{
				auto node = this->root;
				if (node != nullptr)
				{
					ForEachValueImplWithoutIndex(node, lambda);
				}
				return *this;
			}
			else if constexpr (TypeTraits::IsInvockable<TLambda, const TValue&, SizeT>)
			{
				auto node = this->root;
				if (node != nullptr)
				{
					SizeT index = 0;
					ForEachValueImplWithIndex(node, index, lambda);
				}
				return *this;
			}
			else
			{
				static_assert(TypeTraits::IsInvockable<TLambda, const TValue&> ||
					TypeTraits::IsInvockable<TLambda, const TValue&, SizeT>);
			}
		}

		template<typename TLambda>
		BaseMap& ForEachValueImpl(TLambda&& lambda) {
			if constexpr (TypeTraits::IsInvockable<TLambda, TValue&> ||
				TypeTraits::IsInvockable<TLambda, const TValue&>)
			{
				auto node = this->root;
				if (node != nullptr)
				{
					ForEachValueImplWithoutIndex(node, lambda);
				}
				return *this;
			}
			else if constexpr (TypeTraits::IsInvockable<TLambda, TValue&, SizeT> ||
				TypeTraits::IsInvockable<TLambda, const TValue&, SizeT>)
			{
				auto node = this->root;
				if (node != nullptr)
				{
					SizeT index = 0;
					ForEachValueImplWithIndex(node, index, lambda);
				}
				return *this;
			}
			else
			{
				static_assert(TypeTraits::IsInvockable<TLambda, TValue&> ||
					TypeTraits::IsInvockable<TLambda, const TValue&> ||
					TypeTraits::IsInvockable<TLambda, TValue&, SizeT> ||
					TypeTraits::IsInvockable<TLambda, const TValue&, SizeT>);
			}
		}

		ForceInline void UnsafeAdd(BasePair<THeap, TKey, TValue>&& pair) {
			Insert(MoveRValue(pair));
		}

		ForceInline void UnsafeAdd(const BasePair<THeap, TKey, TValue>& pair) {
			Insert(pair);
		}
#pragma endregion

	protected:
		/// <summary>A recursive tree destroying</summary>
		/// <param name = "root">A parent element</param>
		template<typename Lambda>
		Node* DestroyTreeWithLambdaPairValues(Node* root, Lambda& lambda) //-V688
		{
			if (root == nullptr)
				return nullptr;

			if (root->left != nullptr)
			{
				root->left = DestroyTreeWithLambdaPairValues(root->left, lambda);
			}

			lambda(MoveRValue(root->data));

			if (root->right != nullptr)
			{
				root->right = DestroyTreeWithLambdaPairValues(root->right, lambda);
			}

			if (!root->RefCount.DecrementAndCheckIsZero())
			{
				root->father = root->left = root->right = nullptr;
				root->This = nullptr;
			}
			else
			{
				delete root;
			}

			return nullptr;
		}

		BaseMap& RemoveAllImpl() {
			this->root = DestroyTree(this->root);
			this->count = 0;
			return *this;
		}

		template<typename Lambda>
		BaseMap& RemoveAllWithImpl(Lambda&& lambda) {
			this->root = DestroyTreeWithLambdaPairValues(this->root, lambda);
			this->count = 0;
			return *this;
		}
	};

	/// <summary>A flow protected associative array template with nullifying allocator
	/// <para>
	/// Internally realized as a red-and-black tree.
	/// The reference counting performs for every element
	/// which allows to properly destroy it, even 
	/// if the iterator to the element is using somewhere
	/// </para>
	/// </summary>
	/// <param name = "TKey">A key type</param>
	/// <param name = "TValue">A value type</param>
	template<typename TKey, typename TVal>
	using SensitiveMap = BaseMap<Cardinal::Memory::SensitiveDataHeap, TKey, TVal>;

	/// <summary>A flow protected associative array template
	/// <para>
	/// Internally realized as a red-and-black tree.
	/// The reference counting performs for every element
	/// which allows to properly destroy it, even 
	/// if the iterator to the element is using somewhere
	/// </para>
	/// </summary>
	/// <param name = "TKey">A key type</param>
	/// <param name = "TValue">A value type</param>
	template<typename TKey, typename TVal>
	using Map = BaseMap<Cardinal::Memory::Heap, TKey, TVal>;
}

export namespace Cardinal::TypeTraits::Collection
{
	template<typename THeap, typename TKey, typename TVal>
	struct IsMapT<Cardinal::Containers::BaseMap<THeap, TKey, TVal>> : LogicTrue {};
}

export namespace Cardinal
{
	template<typename THeap, typename TKey, typename TVal>
	inline void Swap(Containers::BaseMap<THeap, TKey, TVal>& A, Containers::BaseMap<THeap, TKey, TVal>& B) {
		Swap(A.root, B.root);

		if (A.root != nullptr)
			A.root = moveTree(A.root, &A);
		if (B.root != nullptr)
			B.root = moveTree(B.root, &B);

		Swap(A.count, B.count);
	}

	template<typename THeap, typename TKey, typename TVal>
	ForceInline void Swap(
		typename Containers::BaseMap<THeap, TKey, TVal>::Iterator& A,
		typename Containers::BaseMap<THeap, TKey, TVal>::Iterator& B
	) {
		Swap(A.This, B.This);
	}

	template<typename THeap, typename TKey, typename TVal>
	ForceInline void Swap(
		typename Containers::BaseMap<THeap, TKey, TVal>::ConstIterator& A,
		typename Containers::BaseMap<THeap, TKey, TVal>::ConstIterator& B
	) {
		Swap(A.This, B.This);
	}
}

#pragma warning(pop)