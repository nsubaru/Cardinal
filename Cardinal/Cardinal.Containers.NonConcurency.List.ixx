export module Cardinal.Containers.NonConcurency.List;

export import Cardinal.Core;
export import Cardinal.Memory;
export import Cardinal.Containers.LINQ;
export import Cardinal.Containers.TypeTraits;
export import Cardinal.Containers.InitilizerList;

import Cardinal.Exceptions.Internals;

#pragma warning(push)
#pragma warning(disable: 4996)
#pragma warning(disable: 4595)
#pragma warning(disable: 28182)
#pragma warning(disable: 4172)

export namespace Cardinal::Containers
{
	template<class THeap, typename T>
	class BaseList final :
		public Memory::Allocator<THeap>,
		public LINQ::IList<BaseList, THeap, T, true>
	{
	private:
#pragma region Internal storage struct
		struct elem : public Memory::Allocator<THeap>
		{
		public:
			BaseList* father;

			elem* next;

			elem* prev;

			mutable RefCountT RefCount;

			T value;

			ForceInline elem() :
				father(nullptr), next(nullptr), prev(nullptr),
				RefCount(1), value()
			{};

			ForceInline elem(const T& obj, BaseList* This) :
				father(This), next(nullptr), prev(nullptr),
				RefCount(1), value(obj)
			{};

			ForceInline elem(T&& obj, BaseList* This) :
				father(This), next(nullptr), prev(nullptr),
				RefCount(1), value(MoveRValue(obj))
			{};

			ForceInline elem(const T& obj, BaseList* This, elem* prev) :
				father(This), next(nullptr), prev(prev),
				RefCount(1), value(obj)
			{};

			ForceInline elem(T&& obj, BaseList* This, elem* prev) :
				father(This), next(nullptr), prev(prev),
				RefCount(1), value(MoveRValue(obj))
			{};

			ForceInline elem(const T& obj, BaseList* This, elem* prev, elem* next) :
				father(This), next(next), prev(prev),
				RefCount(1), value(obj)
			{};

			ForceInline elem(T&& obj, BaseList* This, elem* prev, elem* next) :
				father(This), next(next), prev(prev),
				RefCount(1), value(MoveRValue(obj))
			{};
		};
		
	private:
		elem* root;
		elem* last;
		SizeT size;
		
	public:
		~BaseList() {
			while (this->root != nullptr)
			{
				elem* node = this->root;
				this->root = root->next;
				if (--node->RefCount == 0)
				{
					delete node;
					continue;
				}
				node->next = nullptr;
				node->father = nullptr;
				node->prev = nullptr;
			}
		}
		
	private:
		template<typename TLambda>
		elem* Merge(elem* first, elem* second, TLambda& lambda)
		{
			// If first linked list is empty
			if (!first)
				return second;

			// If second linked list is empty
			if (!second)
				return first;

			// Pick the smaller value
			if (lambda(first->value, second->value))
			{
				first->next = Merge(first->next, second, lambda);
				first->next->prev = first;
				first->prev = nullptr;
				return first;
			}
			else
			{
				second->next = Merge(first, second->next, lambda);
				second->next->prev = second;
				second->prev = nullptr;
				return second;
			}
		}

		template<typename TLambda>
		// Function to do merge sort
		elem* MergeSort(elem* head, TLambda& lambda)
		{
			if (!head || !head->next)
				return head;
			elem* second = Split(head, lambda);

			// Recur for left and right halves
			head = MergeSort(head, lambda);
			second = MergeSort(second, lambda);

			// Merge the two sorted halves
			return Merge(head, second, lambda);
		}

		template<typename TLambda>
		// Split a doubly linked list (DLL) into 2 DLLs of half sizes
		elem* Split(elem* head, TLambda& lambda)
		{
			elem* fast = head, * slow = head;
			while (fast->next && fast->next->next)
			{
				fast = fast->next->next;
				slow = slow->next;
			}
			elem* temp = slow->next;
			slow->next = nullptr;
			return temp;
		}
#pragma endregion

	public:
#pragma region Iterator
		class Iterator;

		/// <summary>
		/// The iterator
		/// <para>
		/// The feature of the iterator in list is that the elements will exist
		/// after destroying the list which contained that elements.
		/// Thats why references in list elements are counted, not the list at all
		/// </para>
		/// </summary>
		class ConstIterator final : public Memory::Allocator<THeap> {
			friend class BaseList;
			friend class Iterator;
			
		private:
			mutable elem* This;
			
		private:
			ForceInline ConstIterator(elem* This) :This(This) {
				if (This != nullptr) ++This->RefCount;
			}
			
		public:
			/// <summary>
			/// An empty constructor
			/// </summary>
			ForceInline ConstIterator() :This(nullptr) {};

#pragma region Operators

#pragma region Iterator Ariphmetic
			/// <summary>
			/// A constant postfix increment
			/// </summary>
			const ConstIterator operator++(int) const {
				ConstIterator node(*this);
				if (This == nullptr)
					Internals::Exceptions::ThrowEndNodeNotDeferencable(SourcePosition);

				--This->RefCount;
				if (this->This->father == nullptr)
				{
					// If an element does not have a parent, so the container is already destroyed and there are no adjecent elements
					this->This = nullptr;
					return node;
				}
				else
				{
					this->This = this->This->next;
					if (this->This != nullptr) ++This->RefCount;
					return node;
				}
			}

			/// <summary>
			/// A constant postfix decrement
			/// </summary>
			const ConstIterator operator--(int) const {
				ConstIterator node(*this);
				if (This == nullptr)
					Internals::Exceptions::ThrowEndNodeNotDeferencable(SourcePosition);

				--This->RefCount;
				if (This->father == nullptr)
				{
					// If an element does not have a parent, so the container is already destroyed and there are no adjecent elements
					this->This = nullptr;
					return node;
				}
				else
				{
					this->This = this->This->prev;
					if (this->This != nullptr) ++This->RefCount;
					return node;
				}
			}

			/// <summary>
			/// A prefix increment
			/// </summary>
			ConstIterator& operator++() {
				if (This == nullptr)
					Internals::Exceptions::ThrowEndNodeNotDeferencable(SourcePosition);

				--This->RefCount;
				if (This->father == nullptr)
				{
					// If an element does not have a parent, so the container is already destroyed and there are no adjecent elements
					if (This->RefCount == 0)
						delete This;
					This = nullptr;
				}
				else
				{
					this->This = this->This->next;
					if (this->This != nullptr) ++This->RefCount;
				}
				return *this;
			}

			/// <summary>
			/// A prefix decrement
			/// </summary>
			ConstIterator& operator--() {
				if (This == nullptr)
					Internals::Exceptions::ThrowEndNodeNotDeferencable(SourcePosition);

				--This->RefCount;
				if (this->This->father == nullptr)
				{
					// If an element does not have a parent, so the container is already destroyed and there are no adjecent elements
					this->This = nullptr;
					return *this;
				}
				else
				{
					this->This = this->This->prev;
					if (this->This != nullptr) ++This->RefCount;
					return *this;
				}
			}
#pragma endregion

#pragma region Comparing
			/// <summary>
			/// The equality operator of two iterators
			/// </summary>
			/// <param name = "seconde">The right iterator</param>
			ForceInline bool operator==(const ConstIterator& second) const {
				return reinterpret_cast<SizeT>(This) == reinterpret_cast<SizeT>(second.This);
			}

			/// <summary>
			/// The inequality operator of two iterators
			/// </summary>
			/// <param name = "seconde">The right iterator</param>
			ForceInline bool operator!=(const ConstIterator& second) const {
				return reinterpret_cast<SizeT>(This) != reinterpret_cast<SizeT>(second.This);
			}

			/// <summary>
			/// Equality operator with "nullptr"
			/// </summary>
			ForceInline bool operator==(const NullptrT) const {
				return reinterpret_cast<SizeT>(This) == reinterpret_cast<SizeT>(nullptr);
			}

			/// <summary>
			/// Inequality operator with "nullptr"
			/// </summary>
			ForceInline bool operator!=(const NullptrT) const {
				return This != nullptr;
			}
#pragma endregion

#pragma region Data access operators
			/// <summary>
			/// The iterator denomination operator
			/// </summary>
			/// <returns>A value of element</returns>
			ForceInline const T& operator*() {
				if (This == nullptr)
					Internals::Exceptions::ThrowEndNodeNotDeferencable(SourcePosition);

				return This->value;
			}

			/// <summary>
			/// The constant iterator denomination operator
			/// </summary>
			/// <returns>The constant value of element</returns>
			ForceInline const T& operator*() const {
				if (This == nullptr)
					Internals::Exceptions::ThrowEndNodeNotDeferencable(SourcePosition);

				return This->value;
			}

			/// <summary>
			/// The access operator to fields or structure/class methods
			/// </summary>
			/// <returns>A pointer to an element</returns>
			ForceInline const T* const operator->() {
				if (This == nullptr)
					Internals::Exceptions::ThrowEndNodeNotDeferencable(SourcePosition);

				return &This->value;
			}

			/// <summary>
			/// The constant access operator to fields or structure/class methods
			/// </summary>
			/// <returns>A constant pointer to an element</returns>
			ForceInline const T* const operator->() const {
				if (This == nullptr)
					Internals::Exceptions::ThrowEndNodeNotDeferencable(SourcePosition);

				return &This->value;
			}
#pragma endregion
#pragma endregion

#pragma region Iterator move
			/// <summary>
			/// Returns the next element
			/// </summary>
			ForceInline const ConstIterator Next() const {
				if (This == nullptr)
					Internals::Exceptions::ThrowEndNodeNotDeferencable(SourcePosition);

				if (This->father == nullptr)
					// If an element does not have a parent, so the container is already destroyed and there are no adjecent elements
					return ConstIterator(nullptr);
				else
					return ConstIterator(This->next);
			}

			/// <summary>
			/// Previous element
			/// </summary>
			ForceInline const ConstIterator Prev() const {
				if (This == nullptr)
					Internals::Exceptions::ThrowEndNodeNotDeferencable(SourcePosition);

				if (This->father == nullptr)
					// If an element does not have a parent, so the container is already destroyed and there are no adjecent elements
					return ConstIterator(nullptr);
				else
					return ConstIterator(This->prev);
			}
#pragma endregion

			template<typename U>
			friend void Cardinal::Swap(U& A, U& B);

#pragma region Ctor, Dtor, copy operators
			/// <summary>
			/// The iterator copying constructor
			/// </summary>
			/// <param name="iterator">Original</param>
			ForceInline ConstIterator(const ConstIterator& iterator) : This(iterator.This) {
				if (This != nullptr)
					++This->RefCount;
			}

			/// <summary>
			/// Moving constructor
			/// </summary>
			/// <param name="iterator">Original</param>
			ForceInline ConstIterator(ConstIterator&& iterator) : This(iterator.This) {
				iterator.This = nullptr;
			}

			/// <summary>
			/// Appropriation operator
			/// </summary>
			/// <param name="iterator">Original</param>
			/// <returns>A reference to a current object</returns>
			ForceInline ConstIterator& operator=(const ConstIterator& iterator) {
				this->~ConstIterator();
				this->ConstIterator::ConstIterator(iterator);
				return *this;
			}

			/// <summary>
			/// Moving operator
			/// </summary>
			/// <param name="iterator">Original</param>
			/// <returns>A reference to a current object</returns>
			ForceInline ConstIterator& operator=(ConstIterator&& iterator) {
				this->~ConstIterator();
				this->ConstIterator::ConstIterator(MoveRValue(iterator));
				return *this;
			}

			/// <summary>
			/// Destructor
			/// </summary>
			ForceInline ~ConstIterator() {
				if (This != nullptr)
				{
					if (--This->RefCount == 0)
					{
						delete This;
					}
					This = nullptr;
				}
			}
#pragma endregion
		};
		
	public:
		friend ConstIterator;
	public:

#pragma region Iterator
		/// <summary>
		/// The iterator
		/// <para>
		/// The feature of the iterator in list is that the elements will exist
		/// after destroying the list which contained that elements.
		/// Thats why references in list elements are counted, not the list at all
		/// </para>
		/// </summary>
		class Iterator final : public Memory::Allocator<THeap>
		{
			friend class BaseList;
			
		private:
			mutable elem* This;
			
		private:
			ForceInline Iterator(elem* This) :This(This)
			{
				if (This != nullptr) ++This->RefCount;
			}
			
		public:
			/// <summary>
			/// An empty constructor
			/// </summary>
			ForceInline Iterator() :This(nullptr)
			{};

#pragma region Operators

#pragma region Iterator Ariphmetic
			/// <summary>
			/// A constant postfix increment
			/// </summary>
			const Iterator operator++(int) const
			{
				Iterator node(*this);
				if (This == nullptr)
					Internals::Exceptions::ThrowEndNodeNotDeferencable(SourcePosition);

				--This->RefCount;
				if (this->This->father == nullptr)
				{
					// If an element does not have a parent, so the container is already destroyed and there are no adjecent elements
					this->This = nullptr;
					return node;
				}
				else
				{
					this->This = this->This->next;
					if (this->This != nullptr) ++This->RefCount;
					return node;
				}
			}

			/// <summary>
			/// A constant postfix decrement
			/// </summary>
			const Iterator operator--(int) const
			{
				Iterator node(*this);
				if (This == nullptr)
					Internals::Exceptions::ThrowEndNodeNotDeferencable(SourcePosition);

				--This->RefCount;
				if (This->father == nullptr)
				{
					// If an element does not have a parent, so the container is already destroyed and there are no adjecent elements
					this->This = nullptr;
					return node;
				}
				else
				{
					this->This = this->This->prev;
					if (this->This != nullptr) ++This->RefCount;
					return node;
				}
			}

			/// <summary>
			/// A prefix increment
			/// </summary>
			Iterator& operator++()
			{
				if (This == nullptr)
					Internals::Exceptions::ThrowEndNodeNotDeferencable(SourcePosition);

				--This->RefCount;
				if (This->father == nullptr)
				{
					// If an element does not have a parent, so the container is already destroyed and there are no adjecent elements
					if (This->RefCount == 0)
						delete This;
					This = nullptr;
				}
				else
				{
					this->This = this->This->next;
					if (this->This != nullptr) ++This->RefCount;
				}
				return *this;
			}

			/// <summary>
			/// A prefix decrement
			/// </summary>
			Iterator& operator--()
			{
				if (This == nullptr)
					Internals::Exceptions::ThrowEndNodeNotDeferencable(SourcePosition);

				--This->RefCount;
				if (this->This->father == nullptr)
				{
					// If an element does not have a parent, so the container is already destroyed and there are no adjecent elements
					this->This = nullptr;
					return *this;
				}
				else
				{
					this->This = this->This->prev;
					if (this->This != nullptr) ++This->RefCount;
					return *this;
				}
			}
#pragma endregion

#pragma region Comparing
			/// <summary>
			/// The equality operator of two iterators
			/// </summary>
			/// <param name = "seconde">The right iterator</param>
			ForceInline bool operator==(const Iterator& second) const
			{
				return reinterpret_cast<SizeT>(This) == reinterpret_cast<SizeT>(second.This);
			}

			/// <summary>
			/// The inequality operator of two iterators
			/// </summary>
			/// <param name = "seconde">The right iterator</param>
			ForceInline bool operator!=(const Iterator& second) const
			{
				return reinterpret_cast<SizeT>(This) != reinterpret_cast<SizeT>(second.This);
			}

			/// <summary>
			/// Equality operator with "nullptr"
			/// </summary>
			ForceInline bool operator==(const Cardinal::NullptrT) const
			{
				return reinterpret_cast<SizeT>(This) == reinterpret_cast<SizeT>(nullptr);
			}

			/// <summary>
			/// Inequality operator with "nullptr"
			/// </summary>
			ForceInline bool operator!=(const Cardinal::NullptrT) const
			{
				return This != nullptr;
			}

			/// <summary>
			/// Transforming operator into a constant iterator
			/// </summary>
			operator ConstIterator() const { return ConstIterator(This); }
#pragma endregion

#pragma region Data access operators
			/// <summary>
			/// The iterator denomination operator
			/// </summary>
			/// <returns>A value of element</returns>
			ForceInline T& operator*()
			{
				if (This == nullptr)
					Internals::Exceptions::ThrowEndNodeNotDeferencable(SourcePosition);

				return This->value;
			}

			/// <summary>
			/// The constant iterator denomination operator
			/// </summary>
			/// <returns>The constant value of element</returns>
			ForceInline const T& operator*() const
			{
				if (This == nullptr)
					Internals::Exceptions::ThrowEndNodeNotDeferencable(SourcePosition);

				return This->value;
			}

			/// <summary>
			/// The access operator to fields or structure/class methods
			/// </summary>
			/// <returns>A pointer to an element</returns>
			ForceInline T* operator->()
			{
				if (This == nullptr)
					Internals::Exceptions::ThrowEndNodeNotDeferencable(SourcePosition);

				return &This->value;
			}

			/// <summary>
			/// The constant access operator to fields or structure/class methods
			/// </summary>
			/// <returns>A constant pointer to an element</returns>
			ForceInline const T* operator->() const
			{
				if (This == nullptr)
					Internals::Exceptions::ThrowEndNodeNotDeferencable(SourcePosition);

				return &This->value;
			}
#pragma endregion
#pragma endregion

#pragma region Iterator move
			/// <summary>
			/// Returns the next element
			/// </summary>
			ForceInline const Iterator Next() const
			{
				if (This == nullptr)
					Internals::Exceptions::ThrowEndNodeNotDeferencable(SourcePosition);

				if (This->father == nullptr)
					// If an element does not have a parent, so the container is already destroyed and there are no adjecent elements
					return Iterator(nullptr);
				else
					return Iterator(This->next);
			}

			/// <summary>
			/// Previous element
			/// </summary>
			ForceInline const Iterator Prev() const
			{
				if (This == nullptr)
					Internals::Exceptions::ThrowEndNodeNotDeferencable(SourcePosition);

				if (This->father == nullptr)
					// If an element does not have a parent, so the container is already destroyed and there are no adjecent elements
					return Iterator(nullptr);
				else
					return Iterator(This->prev);
			}
#pragma endregion

#pragma region List element manipulations
			/// <summary>
			/// The iterator value deleting (iterator gains value of next element)
			/// </summary>
			Iterator Remove(bool isReciveNextItInstead = true)
			{
				if (this->This == nullptr)
					Internals::Exceptions::ThrowEndNodeNotDeferencable(SourcePosition);

				if (this->This->father == nullptr)
				{
					if (--This->RefCount == 0)
						delete This;
					This = nullptr;
					return *this;
				}
				else
				{
					__assume(this->This->father != nullptr); //-V547

					if (this->This->next == nullptr && this->This->prev == nullptr)
					{
						this->This->father->root = this->This->father->last = nullptr;
					}
					else if (this->This->next != nullptr && this->This->prev != nullptr)
					{
						this->This->prev->next = this->This->next;
						this->This->next->prev = this->This->prev;
					}
					else if (this->This->next == nullptr)
					{
						if (this->This->prev != nullptr)
							this->This->prev->next = nullptr;
						this->This->father->last = this->This->prev;
					}
					else if (this->This->prev == nullptr) //-V2516
					{
						this->This->next->prev = nullptr;
						this->This->father->root = this->This->next;
					}

					--((This)->father->size);


					if (--this->This->RefCount == 1)
					{
						auto newThis = this->This->next;
						delete this->This;
						if (isReciveNextItInstead) {
							this->This = newThis;
							if (this->This != nullptr)
								++this->This->RefCount;
						}
						else {
							this->This = nullptr;
						}
					}
					else
					{
						auto newThis = this->This->next;
						this->This->next = nullptr;
						this->This->prev = nullptr;
						if (isReciveNextItInstead) {
							this->This = newThis;
							if (this->This != nullptr)
								++this->This->RefCount;
						}
						else {
							this->This = nullptr;
						}
					}
				}
				return *this;
			}

#pragma region Adding element to list by iterator
			/// <summary>
			/// Element insertion after given element
			/// </summary>
			/// <param name = "Value">A value of new element</param>
			ForceInline Iterator Insert(const T& Value)
			{
				return AddAfter(Value);
			}

			/// <summary>
			/// Optimized element insertion after given element
			/// </summary>
			/// <param name = "Value">"RValue" reference to an element for perfect transition in newly created element</param>
			ForceInline Iterator Insert(T&& Value)
			{
				return AddAfter(MoveRValue(Value));
			}

			/// <summary>
			/// Element insertion after current element
			/// </summary>
			/// <param name = "value">A value of new element</param>
			Iterator AddAfter(const T& value)
			{

				if (This == nullptr)
					Internals::Exceptions::ThrowNullPointException(SourcePosition);

				if (This->father == nullptr)
					Internals::Exceptions::ThrowNullPointException(SourcePosition, reinterpret_cast<SizeT>(This->father), 0);

				// New element creation
				auto newAfter = new elem(value, This->father, This, This->next);
				if (This->next != nullptr)
				{
					This->next->prev = newAfter;
				}
				else
				{
					This->father->last = newAfter;
				}
				This->next = newAfter;
				This->father->size++;
				return newAfter;
			}

			/// <summary>
			/// Optimized element insertion after current element
			/// </summary>
			/// <param name = "value">"RValue" reference to an element for perfect transition in newly created element</param>
			Iterator AddAfter(T&& value)
			{
				if (This == nullptr)
					Internals::Exceptions::ThrowNullPointException(SourcePosition);

				if (This->father == nullptr)
					Internals::Exceptions::ThrowNullPointException(SourcePosition, reinterpret_cast<SizeT>(This->father), 0);

				// New element creation
				auto newAfter = new elem(MoveRValue(value), This->father, This, This->next);
				if (This->next != nullptr)
				{
					This->next->prev = newAfter;
				}
				else
				{
					This->father->last = newAfter;
				}
				This->next = newAfter;
				This->father->size++;
				return newAfter;
			}

			/// <summary>
			/// Element insertion before current element
			/// </summary>
			/// <param name = "value">A value of new element</param>
			Iterator AddBefore(const T& value)
			{
				if (This == nullptr)
					Internals::Exceptions::ThrowNullPointException(SourcePosition);

				if (This->father == nullptr)
					Internals::Exceptions::ThrowNullPointException(SourcePosition, reinterpret_cast<SizeT>(This->father), 0);

				// New element creation
				auto newBefore = new elem(value, This->father, This->prev, This);
				if (This->prev != nullptr)
				{
					This->prev->next = newBefore;
				}
				else
				{
					This->father->root = newBefore;
				}
				This->prev = newBefore;
				This->father->size++;
				return newBefore;
			}

			/// <summary>
			/// Optimized element insertion before current element
			/// </summary>
			/// <param name = "value">"RValue" reference to an element for perfect transition in newly created element</param>
			Iterator AddBefore(T&& value)
			{
				if (This == nullptr)
					Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);

				if (This->father == nullptr)
					Internals::Exceptions::ThrowNullPointException(SourcePosition, reinterpret_cast<SizeT>(This->father), 0);

				// New element creation
				auto newBefore = new elem(MoveRValue(value), This->father, This->prev, This);
				if (This->prev != nullptr)
				{
					This->prev->next = newBefore;
				}
				else
				{
					This->father->root = newBefore;
				}
				This->prev = newBefore;
				This->father->size++;
				return newBefore;
			}
#pragma endregion
#pragma endregion

			template<typename U>
			friend void Cardinal::Swap(U& A, U& B);

#pragma region Ctor, Dtor, copy operators
			/// <summary>
			/// The iterator copying constructor
			/// </summary>
			/// <param name="iterator">Original</param>
			ForceInline Iterator(const Iterator& iterator) : This(iterator.This) {
				if (This != nullptr)
					++This->RefCount;
			}

			/// <summary>
			/// Moving constructor
			/// </summary>
			/// <param name="iterator">Original</param>
			ForceInline Iterator(Iterator&& iterator) : This(iterator.This) {
				iterator.This = nullptr;
			}

			/// <summary>
			/// Appropriation operator
			/// </summary>
			/// <param name="iterator">Original</param>
			/// <returns>A reference to a current object</returns>
			ForceInline Iterator& operator=(const Iterator& iterator) {
				this->~Iterator();
				this->Iterator::Iterator(iterator);
				return *this;
			}

			/// <summary>
			/// Moving operator
			/// </summary>
			/// <param name="iterator">Original</param>
			/// <returns>A reference to a current object</returns>
			ForceInline Iterator& operator=(Iterator&& iterator) {
				this->~Iterator();
				this->Iterator::Iterator(MoveRValue(iterator));
				return *this;
			}

			/// <summary>
			/// Destructor
			/// </summary>
			ForceInline ~Iterator() {
				if (This != nullptr)
				{
					if (--This->RefCount == 0)
					{
						delete This;
					}
					This = nullptr;
				}
			}
#pragma endregion
		};
		
	public:
		friend Iterator;

		/// <summary>
		/// An empty constructor
		/// </summary>
		ForceInline BaseList() : root(nullptr), last(nullptr), size(0) {};

		/// <summary>
		/// Copying constructor
		/// </summary>
		/// <param name="original">Original</param>
		BaseList(const BaseList& original) :
			size(original.size) 
		{
			elem* node = original.root;
			if (node != nullptr)
			{
				this->root = this->last = new elem(node->value, this);
				node = node->next;
				while (node != nullptr)
				{
					this->last = this->last->next = new elem(node->value, this, this->last);
					node = node->next;
				}
			}
			else
			{
				this->root = this->last = nullptr;
			}
		}

		/// <summary>
		/// Moving constructor
		/// </summary>
		/// <param name="original">Original</param>
		BaseList(BaseList&& original) : 
			size(original.size), 
			root(original.root), 
			last(original.last)
		{
			if (original.root != nullptr)
			{
				original.root = nullptr;
				original.last = nullptr;
				original.size = 0;

				auto node = this->root;
				while (node != nullptr)
				{
					node->father = this;
					node = node->next;
				}
			}

			original.size = 0;
		}

		/// <summary>
		/// Copying operator
		/// </summary>
		/// <param name="original">Original</param>
		/// <returns>"LValue" reference to array</returns>
		BaseList& operator=(const BaseList& original) {
			this->~BaseList();
			this->BaseList::BaseList(original);
			return *this;
		}

		/// <summary>
		/// Moving operator
		/// </summary>
		/// <param name="original">Original</param>
		/// <returns>"RValue" reference to array</returns>
		BaseList& operator=(BaseList&& original) {
			this->~BaseList();
			this->BaseList::BaseList(MoveRValue(original));
			return *this;
		}

		/// <summary>
		/// A "list-initializer" constructor
		/// </summary>
		/// <param name = "list">Initialization list</param>
		BaseList(InitilizerList<T>&& list) : size(list.size())
		{
			if (list.size() == 0) return;
			auto it = list.begin();
			this->root = new elem(MoveRValue(*it), this);
			this->root->value = *it;
			this->last = this->root;
			++it;

			if (this->size != 1)
			{
				for (SizeT i = 1; i < this->size; i++)
				{
					this->last->next = new elem(MoveRValue(*it), this, this->last);
					this->last = this->last->next;
					++it;
				}
			}
		}

		/// <summary>
		/// A value addition operator to the end of list
		/// </summary>
		/// <param name = "Value">A value of new element</param>
		ForceInline BaseList& operator+=(const T& Value)
		{
			this->AddLast(Value); return *this;
		}

		/// <summary>
		/// An optimized value addition operator to the end of list
		/// </summary>
		/// <param name = "Value">"RValue" reference to a new element value</param>
		ForceInline BaseList& operator+=(T&& Value)
		{
			this->AddLast(MoveRValue(Value)); return *this;
		}

		/// <summary>
		/// A "otherList" list elements addition to the end of this list
		/// </summary>
		/// <param name = "otherList">A second list</param>
		/// <returns>A reference to a current list</returns>
		BaseList& operator+=(const BaseList& otherList) {
			if (otherList.root == this->root)
				return *this;

			if (otherList.size == 0)
				return *this;

			auto node = otherList.root;
			while (node != nullptr) {
				this->last->next = new elem(node->value, this, this->last);
				this->last = this->last->next;
				node = node->next;
			}
			this->size = this->size + otherList.size;
			return *this;
		}

		/// <summary>
		/// An optimized elements addition to given list
		/// </summary>
		/// <param name = "otherList">A second list</param>
		/// <returns>A reference to a current list</returns>
		BaseList& operator+=(BaseList&& otherList) {
			if (otherList.root == this->root)
				return *this;

			if (otherList.size == 0)
				return *this;

			if (this->root == nullptr)
			{
				*this = MoveRValue(otherList);
				return *this;
			}

			//Ignore otherList.This->root == null as if it true than otherList.This->size == 0
			__assume(otherList.root != nullptr);
			auto item = otherList.root;
			while (item != nullptr)
			{
				item->father = this;
				item = item->next;
			}
			otherList.root->prev = this->last;
			this->last->next = otherList.root;
			this->last = otherList.last;
			this->size = this->size + otherList.size;
			otherList.root = otherList.last = nullptr;
			otherList.size = 0;
			return *this;
		}

		/// <summary>
		/// Equality operator
		/// </summary>
		/// <param name="second">Left list</param>
		/// <returns>"True" if all elements and its amount are equal</returns>
		Boolean operator==(const BaseList& second) requires Concepts::HaveEqual<T, T> {
			if (&this->size == &second.size)
				return true;

			if (this->Count == second.Count) {
				auto it1 = this->root;
				auto it2 = second.root;
				while (it1 != nullptr)
				{
					if (!(it1->value == it2->value))
						return false;
					it1 = it1->next;
					it2 = it2->next;
				}
				return true;
			}
			return false;
		}

		/// <summary>
		/// Inequality operator
		/// </summary>
		/// <param name="second">Left list</param>
		/// <returns>"False" if all elements and its amount are not equal</returns>
		Boolean operator!=(const BaseList& second) requires Concepts::HaveNotEqual<T, T> {
			if (&this->size == &second.size)
				return true;

			if (this->Count == second.Count)
			{
				auto it1 = this->root;
				auto it2 = second.root;
				while (it1 != nullptr)
				{
					if (!(it1->value != it2->value))
						return true;
					it1 = it1->next;
					it2 = it2->next;
				}
				return false;
			}
			return true;
		}
#pragma endregion

#pragma region Item count methods
		/// <summary>
		/// Returns "true" if list is empty
		/// </summary>
		ForceInline Boolean IsEmpty() const
		{
			return (this->root == nullptr || this->size == 0);
		}

		/// <summary>
		/// Returns the number of elements in list
		/// </summary>
		ForceInline virtual SizeT GetCount() const
		{
			return IsEmpty() ? 0ull : static_cast<SizeT>(this->size); //-V2517
		}

		/// <summary>
		/// A number of elements in list
		/// </summary>
		SizeT Property(GetCount) Count;
#pragma endregion

#pragma region Iterators

#pragma region Gettter
		/// <summary>
		/// Iterator gaining to the first element
		/// </summary>
		ForceInline Iterator GetRoot()
		{
			if (this->root == nullptr) return GetEnd();
				return Iterator(this->root);
		}

		/// <summary>
		/// Iterator gaining to the last element
		/// </summary>
		ForceInline Iterator GetLast()
		{
			if (this->last == nullptr) return GetEnd();
				return Iterator(this->last);
		}

		/// <summary>
		/// Constant iterator gaining to first element
		/// </summary>
		ForceInline ConstIterator GetRoot() const
		{
			if (this->root == nullptr) return GetEnd();
				return ConstIterator(this->root);
		}

		/// <summary>
		/// Constant iterator gaining to last element
		/// </summary>
		ForceInline ConstIterator GetLast() const
		{
			if (this->last == nullptr) return GetEnd();
				return ConstIterator(this->last);
		}

		/// <summary>
		/// Null-iterator gaining in the end of list
		/// </summary>
		ForceInline Iterator GetEnd() { return Iterator(nullptr); }

		/// <summary>
		/// Null-iterator gaining in the end of list
		/// </summary>
		ForceInline ConstIterator GetEnd() const { return ConstIterator(nullptr); }
#pragma endregion

#pragma region Add new value by iterator
		/// <summary>
		/// New element addition in the end of list
		/// </summary>
		/// <param name = "Value">A value of new element</param>
		Iterator AddLast(const T& Value)
		{
			if (this->root == nullptr)
			{
				this->last = this->root = new elem(Value, this);
				this->size = 1;
				return this->last;
			}
			this->last = this->last->next = new elem(Value, this, this->last);
			++this->size;
			return this->last;
		}

		/// <summary>
		/// Optimized element addition in list
		/// </summary>
		/// <param name = "Value">"RValue" reference to a new element value</param>
		Iterator AddLast(T&& Value)
		{
			if (this->root == nullptr)
			{
				this->last = this->root = new elem(MoveRValue(Value), this);
				this->size = 1;
				return this->last;
			}

			this->last = this->last->next = new elem(MoveRValue(Value), this, this->last);
			++this->size;
			return this->last;
		}

		/// <summary>
		/// New element addition after gained by iterator
		/// </summary>
		/// <param name = "Item">The iterator to an element</param>
		/// <param name = "Value">A value of new element</param>
		ForceInline Iterator AddAfter(Iterator Item, const T& Value)
		{
			return Item.AddAfter(Value);
		}

		/// <summary>
		/// Optimized element addition after gained by iterator
		/// </summary>
		/// <param name = "Item">The iterator to an element</param>
		/// <param name = "Value">"RValue" reference to a new element value</param>
		ForceInline Iterator AddAfter(Iterator Item, T&& Value)
		{
			return Item.AddAfter(MoveRValue(Value));
		}

		/// <summary>
		/// New element addition before gained by iterator
		/// </summary>
		/// <param name = "Item">The iterator to an element</param>
		/// <param name = "Value">A value of new element</param>
		ForceInline Iterator AddBefore(Iterator Item, const T& Value)
		{
			return Item.AddBefore(Value);
		}

		/// <summary>
		/// New element addition before gained by iterator
		/// </summary>
		/// <param name = "Item">The iterator to an element</param>
		/// <param name = "Value">"RValue" reference to a new element value</param>
		ForceInline Iterator AddBefore(Iterator Item, T&& Value)
		{
			return Item.AddBefore(MoveRValue(Value));
		}
#pragma endregion

	public:
#pragma region For each support methods
		/// <summary>
		/// Returns a constant iterator to the beginning for a busting cycle
		/// </summary>
		ForceInline ConstIterator begin() const
		{
			return GetRoot();
		}

		/// <summary>
		/// Returns a constant iterator to the end for a busting cycle
		/// </summary>
		ForceInline ConstIterator end() const
		{
			return nullptr;
		}

		/// <summary>
		/// Returns the iterator to the beginning for a busting cycle
		/// </summary>
		ForceInline Iterator begin()
		{
			return GetRoot();
		}

		/// <summary>
		/// Returns a constant iterator to the end for a busting cycle
		/// </summary>
		ForceInline Iterator end() 
		{
			return nullptr;
		}
#pragma endregion

	public:
		/// <summary>
		/// Element deletion by iterator (iterator resets)
		/// </summary>
		void Remove(Iterator& node)
		{
			if (node.This == nullptr)
				return;

			if (node.This->father == nullptr)
			{
				if (--node.This->RefCount == 0)
					delete node.This;
				node.This = nullptr;
			}
			else
			{
				if (node.This->next == nullptr && node.This->prev == nullptr)
				{
					node.This->father->root = node.This->father->last = nullptr;
				}
				else if (node.This->next != nullptr && node.This->prev != nullptr)
				{
					node.This->prev->next = node.This->next;
					node.This->next->prev = node.This->prev;
				}
				else if (node.This->next == nullptr)
				{
					if (node.This->prev != nullptr)
						node.This->prev->next = nullptr;
					node.This->father->last = node.This->prev;
				}
				else if (node.This->prev == nullptr) 
				{
					node.This->next->prev = nullptr;
					node.This->father->root = node.This->next;
				}

				--((node.This)->father->size);

				if (--node.This->RefCount == 1)
				{
					delete node.This;
					node.This = nullptr;
				}
				else
				{
					node.This->next = nullptr;
					node.This->prev = nullptr;
					node.This = nullptr;
				}
			}
		}

		/// <summary>
		/// Element deletion by iterator (iterator resets)
		/// </summary>
		void Remove(Iterator&& node) {
			Remove(node);
		}

		template<typename U>
		friend void Cardinal::Swap(U& A, U& B);
		
	public:
		/// <summary>
        /// List cloning
        /// </summary>
        /// <returns>Returns new list with copy of all elements</returns>
        template<typename = void> requires (Concepts::IsClonable<T>) 
        BaseList Clone() const {
			BaseList result;
			result.size = this->size;
			elem* node = this->root;
			if (node != nullptr) {
				result.root = result.last = new elem(Cardinal::Clone(node->value), &result);
				node = node->next;
				while (node != nullptr) {
					result.last = result.last->next = new elem(Cardinal::Clone(node->value), &result, result.last);
					node = node->next;
				}
			}
			return result;
        }

		/// <summary>
		/// List cloning using custom lambda or functor
		/// </summary>
		/// <typeparam name="TLambda">Lambda type</typeparam>
		/// <param name="lambda">A function with which occurs cloning</param>
		/// <returns>A lambda or functor which clones all elements</returns>
		template<typename TLambda> requires (Concepts::IsInvockableWithResult<TLambda, T, const T&>)
        BaseList Clone(TLambda&& lambda) const {
			BaseList result;
			result.size = this->size;
			elem* node = this->root;
			if (node != nullptr) {
				result.root = result.last = new elem(lambda(node->value), result);
				node = node->next;
				while (node != nullptr) {
					result.last = result.last->next = new elem(lambda(node->value), result, result.last);
					node = node->next;
				}
			}
			return result;
        }
		
	protected:
		template<template<typename, typename> typename, typename, typename, Boolean, Boolean, Boolean, Boolean>
		friend class LINQ::ICollection;

		template<template<typename, typename> typename, typename, typename, Boolean>
		friend class LINQ::IList;

		template<template<typename, SizeT, typename T> typename TCollection, typename THeap, typename T, SizeT TSize, Boolean TIsConcurencySuport>
		friend class LINQ::IFixedCollectionBase;

		template<template<typename THeap, typename T>typename TList, typename THeap, typename T, bool TIsDynamic, bool TIsConcurencySuport>
		friend class LINQ::IOrderable;

        template<typename T>
        friend struct Cardinal::Serialization::SerializationHandler;

		template<template<typename THeap, typename TKey, typename TValue> typename TCollection, typename THeap, typename TKey, typename TValue, typename TPair, bool IsConcurencySuportT>
        friend class LINQ::IAssociableCollection;

		template<template<typename, SizeT, typename> typename TCollection, typename THeap, typename T, SizeT TSize, Boolean TIsConcurencySuport>
		friend class LINQ::IFixedCollection;

	protected:
		template<typename TLambda>
		BaseList& ForEachImpl(TLambda&& lambda) {
			if constexpr (TypeTraits::IsInvockable<TLambda, const T&> ||
                            TypeTraits::IsInvockable<TLambda, T&>) 
			{
				auto node = this->root;
				while (node != nullptr) {
					lambda(node->value);
					node = node->next;
				}
				return *this;
			}
			else if constexpr (TypeTraits::IsInvockable<TLambda, const T&, SizeT> ||
                                TypeTraits::IsInvockable<TLambda, T&, SizeT>) 
			{
				auto node = this->root;
				SizeT index = 0;
				while (node != nullptr) {
					lambda(node->value, index);
					node = node->next;
					index++;
				}
				return *this;
			}
			else 
			{
				static_assert(
					TypeTraits::IsInvockable<TLambda, const T&> ||
					TypeTraits::IsInvockable<TLambda, T&> ||
					TypeTraits::IsInvockable<TLambda, const T&, SizeT> ||
					TypeTraits::IsInvockable<TLambda, T&, SizeT>);
			}
		}

		template<typename TLambda>
		const BaseList& ForEachImpl(TLambda&& lambda) const {
			if constexpr (TypeTraits::IsInvockable<TLambda, const T&>) 
			{
				auto node = this->root;
				while (node != nullptr) {
					lambda(node->value);
					node = node->next;
				}
				return *this;
			}
			else if constexpr (TypeTraits::IsInvockable<TLambda, const T&, SizeT>) 
			{
				auto node = this->root;
				SizeT index = 0;
				while (node != nullptr) {
					lambda(node->value, index);
					node = node->next;
					index++;
				}
				return *this;
			}
			else 
			{
				static_assert(
					TypeTraits::IsInvockable<TLambda, const T&> || 
					TypeTraits::IsInvockable<TLambda, const T&, SizeT>
					);
			}
		}

		template<typename TLambda>
		BaseList& ForEachImpl(TLambda&& lambda, SizeT count) {
			if constexpr (TypeTraits::IsInvockable<TLambda, const T&> ||
                            TypeTraits::IsInvockable<TLambda, T&>) 
			{
				if (count > this->size)
				{
					auto node = this->root;
					while (node != nullptr) {
						lambda(node->value);
						node = node->next;
					}
					return *this;
				}
				else {
					SizeT i = 0;
					auto node = this->root;
					while (node != nullptr) {
						lambda(node->value);
						node = node->next;
						i++;
						if (i == count)
							return *this;
					}
					return *this;
				}
			}
			else if constexpr (TypeTraits::IsInvockable<TLambda, const T&, SizeT> ||
                                TypeTraits::IsInvockable<TLambda, T&, SizeT>) 
			{
				if (count > this->size)
				{
					auto node = this->root;
					SizeT index = 0;
					while (node != nullptr) {
						lambda(node->value, index);
						node = node->next;
						index++;
					}
					return *this;
				}
				else {
					SizeT index = 0;
					auto node = this->root;
					while (node != nullptr) {
						lambda(node->value, index);
						node = node->next;
						index++;
						if (index == count)
							return *this;
					}
					return *this;
				}
			}
			else 
			{
				static_assert(
					TypeTraits::IsInvockable<TLambda, const T&> ||
					TypeTraits::IsInvockable<TLambda, T&> ||
					TypeTraits::IsInvockable<TLambda, const T&, SizeT> ||
					TypeTraits::IsInvockable<TLambda, T&, SizeT>);
			}
		}

		template<typename TLambda>
		const BaseList& ForEachImpl(TLambda&& lambda, SizeT count) const {
			if constexpr (TypeTraits::IsInvockable<TLambda, const T&>) 
			{
				if (count > this->size)
				{
					auto node = this->root;
					while (node != nullptr) {
						lambda(node->value);
						node = node->next;
					}
					return *this;
				}
				else {
					SizeT i = 0;
					auto node = this->root;
					while (node != nullptr) {
						lambda(node->value);
						node = node->next;
						i++;
						if (i == count)
							return *this;
					}
					return *this;
				}
			}
			else if constexpr (TypeTraits::IsInvockable<TLambda, const T&, SizeT>) 
			{
				if (count > this->size)
				{
					auto node = this->root;
					SizeT index = 0;
					while (node != nullptr) {
						lambda(node->value, index);
						node = node->next;
						index++;
					}
					return *this;
				}
				else {
					SizeT index = 0;
					auto node = this->root;
					while (node != nullptr) {
						lambda(node->value, index);
						node = node->next;
						index++;
						if (index == count)
							return *this;
					}
					return *this;
				}
			}
			else 
			{
				static_assert(
					TypeTraits::IsInvockable<TLambda, const T&> ||
					TypeTraits::IsInvockable<TLambda, const T&, SizeT>
					);
			}
		}

	protected:
		template<typename U, typename TLambda>
		U AggregateImpl(TLambda&& lambda, const U& value) const {
			auto node = this->root;
			U result = value;
			U& data = result;
			SizeT index = 0;

			while (node != nullptr) {
				if constexpr (
					Concepts::IsInvockableWithResult<TLambda, U&, U&, const T&> ||
					Concepts::IsInvockableWithResult<TLambda, U, U&, const T&>)
				{
					data = lambda(data, node->value);
				}
				else
				{
					lambda(data, node->value);
				}
						
				node = node->next;
				index++;
			}
			return MoveRValue(result);
		}

		template<typename U, typename TLambda>
		U AggregateImpl(TLambda&& lambda, U&& value) const {
			auto node = this->root;
			SizeT index = 0;

			while (node != nullptr) {
				if constexpr (
					Concepts::IsInvockableWithResult<TLambda, U&, U&, const T&> ||
					Concepts::IsInvockableWithResult<TLambda, U, U&, const T&>)
				{
					value = lambda(value, node->value);
				}
				else
				{
					lambda(value, node->value);
				}

				node = node->next;
				index++;
			}
			return MoveRValue(value);
		}

		ForceInline bool AnyImpl() const { return this->size > 0; }

		template<typename TLambda>
		bool AnyImpl(TLambda lambda) const {
			auto node = this->root;
			SizeT index = 0;
			while (node != nullptr) {
				if (lambda(node->value))
					return true;
				node = node->next;
				index++;
			}
			return false;
		}

		template<typename TLambda>
		bool AllImpl(TLambda lambda) const {
			auto node = this->root;
			SizeT index = 0;
			while (node != nullptr) {
				if (!lambda(node->value))
					return false;
				node = node->next;
				index++;
			}
			return true;
		}

		BaseList& SortByImpl(bool IsDescending = true) requires Concepts::IsComparableWith<T, T> {
			if (!IsDescending) 
			{
				auto l = [](const T& V1, const T& V2) { return V1 < V2; };
				this->root = MergeSort(this->root, l);
			}
			else 
			{
				auto l = [](const T& V1, const T& V2) { return V1 > V2; };
				this->root = MergeSort(this->root, l);
			}

			this->last = [&]() {
				elem* node = this->root;
				while (node->next != nullptr)
					node = node->next;
				return node;
			}();

			return *this;
		}

		template<typename TLambda>
		BaseList& SortByLambdaImpl(TLambda&& lambda) {
			if (this->size == 0)
				return *this;

			this->root = MergeSort(this->root, lambda);

			this->last = [&]() {
				elem* node = this->root;
				while (node->next != nullptr)
					node = node->next;
				return node;
			}();

			return *this;
		}

		template<typename TLambda>
		T& FirstImpl(TLambda&& lambda) {
			elem* node = this->root;
			while (node != nullptr)
				if (lambda(node->value))
					return node->value;
				else
					node = node->next;

			Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);
		}

		template<typename TLambda>
		T& LastImpl(TLambda&& lambda) {
			elem* node = this->root;
			elem* selected = nullptr;
			while (node != nullptr) {
				if (lambda(node->value))
					selected = node;
				node = node->next;
			}

			if (selected != nullptr)
				return selected->value;

			Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);
		}

		template<typename TLambda>
		const T& FirstImpl(TLambda&& lambda) const {
			elem* node = this->root;
			while (node != nullptr)
				if (lambda(node->value))
					return node->value;
				else
					node = node->next;

			Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);
		}

		template<typename TLambda>
		const T& LastImpl(TLambda&& lambda) const {
			elem* node = this->root;
			elem* selected = nullptr;
			while (node != nullptr) {
				if (lambda(node->value))
					selected = node;
				node = node->next;
			}

			if (selected != nullptr)
				return selected->value;

			Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);
		}

		template<typename TLambda>
		Iterator FirstItImpl(TLambda&& lambda) {
			elem* node = this->root;
			while (node != nullptr)
				if (lambda(node->value))
					return Iterator(node);
				else
					node = node->next;

			return Iterator(nullptr);
		}

		template<typename TLambda>
		Iterator LastItImpl(TLambda&& lambda) {
			elem* node = this->root;
			elem* selected = nullptr;
			while (node != nullptr)
			{
				if (lambda(node->value))
					selected = node;
				node = node->next;
			}

			if (selected != nullptr)
				return Iterator(selected);

			return Iterator(nullptr);
		}

		template<typename TLambda>
		ConstIterator FirstItImpl(TLambda&& lambda) const {
			elem* node = this->root;
			while (node != nullptr)
				if (lambda(node->value))
					return ConstIterator(node);
				else
					node = node->next;

			return ConstIterator(nullptr);
		}

		template<typename TLambda>
		ConstIterator LastItImpl(TLambda&& lambda) const {
			elem* node = this->root;
			elem* selected = nullptr;
			while (node != nullptr)
			{
				if (lambda(node->value))
					selected = node;
				node = node->next;
			}

			if (selected != nullptr)
				return ConstIterator(selected);

			return ConstIterator(nullptr);
		}

		template<typename TLambda>
		ForceInline T FirstOrImpl(TLambda&& lambda, T&& item) const {
			if (size == 0)
				return MoveRValue(item);

			elem* node = this->root;
			while (node != nullptr)
				if (lambda(node->value))
					return node->value;
				else
					node = node->next;

			return MoveRValue(item);
		}

		template<typename TLambda>
		ForceInline T LastOrImpl(TLambda&& lambda, T&& item) const {
			if (size == 0)
				return MoveRValue(item);

			elem* node = this->root;
			elem* selected = nullptr;
			while (node != nullptr)
			{
				if (lambda(node->value))
					selected = node;
				node = node->next;
			}

			if (selected != nullptr)
				return selected->value;

			return MoveRValue(item);
		}

		template<typename TLambda>
		ForceInline T FirstOrImpl(TLambda&& lambda, const T& item) const {
			if (size == 0)
				return item;

			elem* node = this->root;
			while (node != nullptr)
				if (lambda(node->value))
					return node->value;
				else
					node = node->next;

			return item;
		}

		template<typename TLambda>
		ForceInline T LastOrImpl(TLambda&& lambda, const T& item) const {
			if (size == 0)
				return item;

			elem* node = this->root;
			elem* selected = nullptr;
			while (node != nullptr)
			{
				if (lambda(node->value))
					selected = node;
				node = node->next;
			}

			if (selected != nullptr)
				return selected->value;

			return item;
		}

		template<typename TLambda, typename TGetterLambda>
		ForceInline T FirstOrImpl(TLambda&& lambda, TGetterLambda&& otherLambda) const {
			if (size == 0)
				return otherLambda();

			elem* node = this->root;
			while (node != nullptr)
				if (lambda(node->value))
					return node->value;
				else
					node = node->next;

			return otherLambda();
		}

		template<typename TLambda, typename TGetterLambda>
		ForceInline T LastOrImpl(TLambda&& lambda, TGetterLambda&& otherLambda) const {
			if (size == 0)
				return otherLambda();

			elem* node = this->root;
			elem* selected = nullptr;
			while (node != nullptr)
			{
				if (lambda(node->value))
					selected = node;
				node = node->next;
			}

			if (selected != nullptr)
				return selected->value;

			return otherLambda();
		}

		BaseList& ReverseImpl() {
			elem* temp = nullptr;
			elem* current = this->root;

			while (current != nullptr) {
				temp = current->prev;
				current->prev = current->next;
				current->next = temp;
				current = current->prev;
			}

			Cardinal::Swap(this->root, this->last);
			return *this;
		}

		ForceInline void UnsafeAdd(const T& value) {
			if (this->root == nullptr) 
			{
				this->root = this->last = new elem(value, this);
			}
			else 
			{
				this->last->prev->next = this->last = new elem(value, this, this->last);
			}

			++this->size;
		}

		ForceInline void UnsafeAdd(T&& value) {
			if (this->root == nullptr) 
			{
				this->root = this->last = new elem(MoveRValue(value), this);
			}
			else 
			{
				this->last->prev->next = this->last = new elem(MoveRValue(value), this, this->last);
			}

			++this->size;
		}

		BaseList& RemoveAllImpl() {
			while (this->root != nullptr)
			{
				elem* node = this->root;
				this->root = this->root->next;
				node->next = nullptr;
				node->father = nullptr;
				node->prev = nullptr;
				if (--node->RefCount == 0)
				{
					delete node;
					continue;
				}
			}

			this->root = this->last = nullptr;
			this->size = 0;
			return *this;
		}

		template<typename TLambda>
		BaseList& RemoveAllImpl(TLambda&& lambda) {
			elem* node;
			while (this->root != nullptr)
			{
				node = this->root;
				this->root = root->next;
				node->next = nullptr;
				node->father = nullptr;
				node->prev = nullptr;
				lambda(MoveRValue(node->value));
				if (--node->RefCount == 0)
				{
					delete node;
					continue;
				}
			}

			this->root = this->last = nullptr;
			this->size = 0;
			return *this;
		}

		template<typename TFilterLambda, typename TLambda>
		BaseList& RemoveIfFilterAndLambdaWithIndex(TFilterLambda&& filter, TLambda&& lambda) {
			if (this->root == nullptr)
				return *this;

			elem* it = this->root;
			elem* tmp;
			SizeT Index = 0;
			while (it != nullptr)
			{
				if (filter(it->value, Index)) 
				{
					lambda(it->value, Index);

					if (it->next == nullptr && it->prev == nullptr)
					{
						this->root = nullptr;
						this->last = nullptr;
						tmp = nullptr;
					}
					else if (it->next == nullptr)
					{
						tmp = nullptr;
						if (it->prev != nullptr)
							it->prev->next = nullptr;
						this->last = it->prev;
					}
					else if (it->prev == nullptr)
					{
						tmp = it->next;
						it->next->prev = nullptr;
						this->root = it->next;
					}
					else
					{
						tmp = it->next;
						tmp->prev = it->prev;
						it->prev->next = tmp;
					}
					--it->RefCount;
					if (it->RefCount == 0)
					{
						delete it;
					}
					--this->size;
					it = tmp;

				}
				else  it = it->next;
			}
			return *this;
		}

		template<typename TFilterLambda, typename TLambda>
		BaseList& RemoveIfFilterWithIndex(TFilterLambda&& filter, TLambda&& lambda) {
			if (this->root == nullptr)
				return *this;

			elem* it = this->root;
			elem* tmp;
			SizeT Index = 0;
			while (it != nullptr)
			{
				if (filter(it->value, Index)) 
				{
					lambda(it->value);

					if (it->next == nullptr && it->prev == nullptr)
					{
						this->root = nullptr;
						this->last = nullptr;
						tmp = nullptr;
					}
					else if (it->next == nullptr)
					{
						tmp = nullptr;
						if (it->prev != nullptr)
							it->prev->next = nullptr;
						this->last = it->prev;
					}
					else if (it->prev == nullptr)
					{
						tmp = it->next;
						it->next->prev = nullptr;
						this->root = it->next;
					}
					else
					{
						tmp = it->next;
						tmp->prev = it->prev;
						it->prev->next = tmp;
					}
					--it->RefCount;
					if (it->RefCount == 0)
					{
						delete it;
					}
					--this->size;
					it = tmp;

				}
				else  it = it->next;
			}
			return *this;
		}

		template<typename TFilterLambda, typename TLambda>
		BaseList& RemoveIfLambdaWithIndex(TFilterLambda&& filter, TLambda&& lambda) {
			if (this->root == nullptr)
				return *this;

			elem* it = this->root;
			elem* tmp;
			SizeT Index = 0;
			while (it != nullptr)
			{
				if (filter(it->value)) 
				{
					lambda(it->value, Index);

					if (it->next == nullptr && it->prev == nullptr)
					{
						this->root = nullptr;
						this->last = nullptr;
						tmp = nullptr;
					}
					else if (it->next == nullptr)
					{
						tmp = nullptr;
						if (it->prev != nullptr)
							it->prev->next = nullptr;
						this->last = it->prev;
					}
					else if (it->prev == nullptr)
					{
						tmp = it->next;
						it->next->prev = nullptr;
						this->root = it->next;
					}
					else
					{
						tmp = it->next;
						tmp->prev = it->prev;
						it->prev->next = tmp;
					}
					--it->RefCount;
					if (it->RefCount == 0)
					{
						delete it;
					}
					--this->size;
					it = tmp;

				}
				else  it = it->next;
			}
			return *this;
		}

		template<typename TFilterLambda, typename TLambda>
		BaseList& RemoveIfFilterAndLambdaWithoutIndex(TFilterLambda&& filter, TLambda&& lambda) {
			if (this->root == nullptr)
				return *this;

			elem* it = this->root;
			elem* tmp;
			SizeT Index = 0;
			while (it != nullptr)
			{
				if (filter(it->value)) {
					lambda(MoveRValue(it->value));

					if (it->next == nullptr && it->prev == nullptr)
					{
						this->root = nullptr;
						this->last = nullptr;
						tmp = nullptr;
					}
					else if (it->next == nullptr)
					{
						tmp = nullptr;
						if (it->prev != nullptr)
							it->prev->next = nullptr;
						this->last = it->prev;
					}
					else if (it->prev == nullptr)
					{
						tmp = it->next;
						it->next->prev = nullptr;
						this->root = it->next;
					}
					else
					{
						tmp = it->next;
						tmp->prev = it->prev;
						it->prev->next = tmp;
					}
					--it->RefCount;
					if (it->RefCount == 0)
					{
						delete it;
					}
					--this->size;
					it = tmp;

				}
				else  it = it->next;
			}
			return *this;
		}

		template<typename TFilterLambda, typename TLambda>
		ForceInline BaseList& RemoveIfImpl(TFilterLambda&& filter, TLambda&& lambda) {
			if constexpr (TypeTraits::IsInvockable<TLambda, T&&>&&
				TypeTraits::IsInvockableWithResult<TFilterLambda, Boolean, const T&>) 
			{
				return RemoveIfFilterAndLambdaWithoutIndex(MoveRValue(filter), MoveRValue(lambda));
			}
			else if constexpr (TypeTraits::IsInvockable<TLambda, void, T&&>&&
				TypeTraits::IsInvockableWithResult<TFilterLambda, Boolean, const T&, SizeT>) 
			{
				return RemoveIfFilterWithIndex(MoveRValue(filter), MoveRValue(lambda));
			}
			else if constexpr (TypeTraits::IsInvockable<TLambda, void, T&&, SizeT>&&
				TypeTraits::IsInvockableWithResult<TFilterLambda, Boolean, const T&>) 
			{
				return RemoveIfLambdaWithIndex(MoveRValue(filter), MoveRValue(lambda));
			}
			else if constexpr (TypeTraits::IsInvockable<TLambda, void, T&&, SizeT>&&
				TypeTraits::IsInvockableWithResult<TFilterLambda, Boolean, const T&, SizeT>) 
			{
				return RemoveIfFilterAndLambdaWithIndex(MoveRValue(filter), MoveRValue(lambda));
			}
			else
				static_assert((TypeTraits::IsInvockable<TLambda, T&&> &&
					TypeTraits::IsInvockableWithResult<TFilterLambda, Boolean, const T&>) ||
					(TypeTraits::IsInvockable<TLambda, void, T&&> &&
						TypeTraits::IsInvockableWithResult<TFilterLambda, Boolean, const T&, SizeT>) ||
					(TypeTraits::IsInvockable<TLambda, void, T&&, SizeT> &&
						TypeTraits::IsInvockableWithResult<TFilterLambda, Boolean, const T&>) ||
					(TypeTraits::IsInvockable<TLambda, void, T&&, SizeT> &&
						TypeTraits::IsInvockableWithResult<TFilterLambda, Boolean, const T&, SizeT>));
		}

		template<typename TLambda>
		SizeT GetCountIfImpl(TLambda&& lambda) const {
			SizeT selectedCount = 0;

			auto node = this->root;
			while (node != nullptr) {
				if (lambda(node->value))
					selectedCount++;
				node = node->next;
			}

			return selectedCount;
		}
	};
#pragma endregion

#pragma region Partical implementation for default and sensitive heaps
	/// <summary>Double-linked list with standard allocator 
	/// <para>
	/// The reference counting performs for every element
	/// which allows to properly destroy it, even 
	/// if the iterator to the element is using somewhere
	/// </para>
	/// </summary>
	/// <param name = "T">Values type</param>
	template<typename T>
	using List = BaseList<Cardinal::Memory::Heap, T>;

	/// <summary>Double-linked list with protected allocator 
	/// <para>
	/// The reference counting performs for every element
	/// which allows to properly destroy it, even 
	/// if the iterator to the element is using somewhere
	/// </para>
	/// </summary>
	/// <param name = "T">Values type</param>
	template<typename T>
	using SensitiveList = BaseList<Cardinal::Memory::SensitiveDataHeap, T>;
#pragma endregion
}

export namespace Cardinal::TypeTraits::Collection
{
	template<typename THeap, typename T>
	struct IsListT<Cardinal::Containers::BaseList<THeap, T>> : LogicTrue {};
}

export namespace Cardinal
{
	template<typename THeap, typename T>
	inline void Swap(Containers::BaseList<THeap, T>& A, Containers::BaseList<THeap, T>& B) {
		Swap(A.root, B.root);

		if (A.root != nullptr)
		{
			auto node = A.root;
			while (node != nullptr)
			{
				node->father = &A;
				node = node->next;
			}
		}

		if (B.root != nullptr)
		{
			auto node = B.root;
			while (node != nullptr)
			{
				node->father = &B;
				node = node->next;
			}
		}

		Swap(A.last, B.last);
		Swap(A.size, B.size);
	}

	template<typename THeap, typename T>
	ForceInline void Swap(
		typename Containers::BaseList<THeap, T>::Iterator& A, 
		typename Containers::BaseList<THeap, T>::Iterator& B
	) {
		Swap(A.This, B.This);
	}

	template<typename THeap, typename T>
	ForceInline void Swap(
		typename Containers::BaseList<THeap, T>::ConstIterator& A, 
		typename Containers::BaseList<THeap, T>::ConstIterator& B
	) {
		Swap(A.This, B.This);
	}
}

#pragma warning(pop)