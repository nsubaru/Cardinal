export module Cardinal.Threading:Context;

export import :Base;
export import Cardinal.Containers.Concurency.Array;
export import Cardinal.Containers.Concurency.List;
export import Cardinal.Containers.SmartPtr;
export import Cardinal.Containers.AtomicSmartPtr;
export import Cardinal.Exceptions;

export namespace Cardinal::Threading
{
	/// <summary>
	/// Context interface
	/// </summary>
	VirtualClass IContext : public IDisposable{
	public:
		virtual RTTI::Record::RttiTypeRecord GetContextType() const = 0;

	public:
		virtual ~IContext() override {}
	};

	/// <summary>
	/// Context record interface
	/// </summary>
	template<typename TContext>
	concept ThreadableContext =
		Concepts::IsBasedOn<IContext, TContext> &&
		Concepts::IsConstructibleWith<TContext, const TContext&> &&
		Concepts::IsConstructibleWith<TContext, TContext&&> &&
		Concepts::HasCloneMethod<TContext>;

	/// <summary>
	/// Information about context
	/// </summary>
	struct ContextInformation {
		struct ContextRecordInformation {
			/// <summary>
			/// Record identifier
			/// </summary>
			ID RecordId;

			/// <summary>
			/// Record type
			/// </summary>
			RTTI::Record::RttiTypeRecord RecordType;
		};

		/// <summary>
		/// Unique context identifier
		/// </summary>
		ID ContexUniqtId;

		/// <summary>
		/// Context thread type
		/// </summary>
		EThreadableTypes ThreadableType;

		/// <summary>
		/// "True" if the context is created manually
		/// </summary>
		Boolean IsManual;

		/// <summary>
		/// List of context records
		/// </summary>
		Containers::Array<ContextRecordInformation> ContextRecordsInformation;
	};

	/// <summary>
	/// The block of thread execution contexts
	/// </summary>
	class ContextsBlock final : public IDisposable {
	private:
		struct Data {
			SmartPtr<ContextsBlock> parrentContext;
			Containers::ConcurencyList<Containers::Pair<RTTI::Record::RttiTypeRecord, AtomicSmartPtr<IContext>>> thisContext;
			Boolean isManual;
			EThreadableTypes threadableType;
			RefCountT RefCount;

			Data(SmartPtr<ContextsBlock>&& parrentContext, EThreadableTypes threadableType, Boolean isManual) :
				parrentContext(MoveRValue(parrentContext)),
				threadableType(threadableType),
				isManual(isManual),
				thisContext(),
				RefCount(1) {
			}
		};

	private:
		Data* This;

	protected:
		virtual void Dispose() override {}

	public:
		/// <summary>
		/// Indepentent context constructor
		/// </summary>
		ContextsBlock();

	protected:
		/// <summary>
		/// Context constructor with the transition of parent context
		/// </summary>
		/// <param name="parrentContext">Parent context</param>
		ContextsBlock(SmartPtr<ContextsBlock>&& parrentContext, EThreadableTypes threadableType, Boolean isManual) :
			This(new Data(MoveRValue(parrentContext), threadableType, isManual)
			) {
			if (This->parrentContext == nullptr)
			{
				Throw(InvalidArgumentException(SourcePosition, L"parrentContext", L"parrentContext should be not empty"));
			}
		}

	private:
		friend ContextsBlock CreateContextBlock(SmartPtr<ContextsBlock>&& parrentContext, EThreadableTypes threadableType, Boolean isManual);

		friend void InitMainThreadContexts(ContextsBlock& contexts);

	public:
		/// <summary>
		/// Returns the context object by its type
		/// </summary>
		/// <param name="typeRecord">Context type</param>
		/// <param name="onlyFromCurrentContextBlock">If true then context search perform only in current context block ignoring parrents</param>
		/// <returns>Atomic pointer on a context</returns>
		AtomicSmartPtr<IContext> GetContext(RTTI::Record::RttiTypeRecord typeRecord, bool onlyFromCurrentContextBlock = false) const {
			if (This->thisContext.Any([&] (const auto& item) { return item.Key == typeRecord; }))
			{
				return This->thisContext.First([&] (const auto& item) { return item.Key == typeRecord; }).Value;
			}
			else if (!onlyFromCurrentContextBlock && This->parrentContext != nullptr)
			{
				return This->parrentContext->GetContext(typeRecord);
			}
			else
			{
				Throw(NotFoundException(SourcePosition));
			}
		}

		/// <summary>
		/// Setting the new context
		/// </summary>
		/// <param name="context">Atomic pointer on a context object</param>
		void SetContext(const AtomicSmartPtr<IContext>& context) {
			if (This->thisContext.Any([&] (const auto& item) { return item.Key == context->GetContextType(); }))
			{
				This->thisContext
					.First([&] (const auto& item) { return item.Key == context->GetContextType(); })
					.Value = context;
			}
			else
			{
				This->thisContext.AddLast({ context->GetContextType(), context });
			}
		}

		/// <summary>
		/// Setting the new context
		/// </summary>
		/// <param name="context">Atomic pointer on a context object</param>
		void SetContext(AtomicSmartPtr<IContext>&& context) {
			if (This->thisContext.Any([&] (const auto& item) { return item.Key == context->GetContextType(); }))
			{
				This->thisContext
					.First([&] (const auto& item) { return item.Key == context->GetContextType(); })
					.Value = MoveRValue(context);
			}
			else
			{
				This->thisContext.AddLast({ context->GetContextType(), MoveRValue(context) });
			}
		}

		/// <summary>
		/// Returns "true" if context with te specified type already exists
		/// </summary>
		/// <param name="typeRecord">Requested context type</param>
		/// <param name="onlyFromCurrentContextBlock">If true then context search perform only in current context block ignoring parrents</param>
		/// <returns>The logical value</returns>
		bool IsContextExist(RTTI::Record::RttiTypeRecord typeRecord, bool onlyFromCurrentContextBlock = false) {
			if (This->thisContext.Any([&] (const auto& item) { return item.Key == typeRecord; }))
			{
				return true;
			}
			else if (!onlyFromCurrentContextBlock && This->parrentContext != nullptr)
			{
				return This->parrentContext->IsContextExist(typeRecord);
			}
			else
			{
				return false;
			}
		}

		/// <summary>
		/// Remove context from current block by type
		/// </summary>
		/// <param name="typeRecord">Type information of context</param>
		void RemoveContext(RTTI::Record::RttiTypeRecord typeRecord) {
			This->thisContext.FirstIt(ArrowFunctWithCapture(const& x, x.Key == typeRecord)).Remove();
		}

	public:
		/// <summary>
		/// Returns the information about context and its parent contexts
		/// </summary>
		/// <returns>Array of records where the first record is the record about ths context, and next is about parent contexts</returns>
		Containers::Array<ContextInformation> QueryFullContextInformation() const;

		/// <summary>
		/// Returns the information about this context
		/// </summary>
		/// <returns>Information about this context</returns>
		ContextInformation QueryContextInformation() const;

	public:
		RefCountClass(ContextsBlock, This)
	};

	/// <summary>
	/// Static object which contains the contexts of current execution code threads
	/// </summary>
	class Contexts final {
		static AtomicSmartPtr<IContext> GetContextImpl(RTTI::Record::RttiTypeRecord typeRecord, bool onlyFromCurrentContextBlock);
		static void SetContextImpl(AtomicSmartPtr<IContext>&& context);
		static bool IsContextExistImpl(RTTI::Record::RttiTypeRecord typeRecord, bool onlyFromCurrentContextBlock);
		static void RemoveContextImpl(RTTI::Record::RttiTypeRecord typeRecord);

	public:
		Contexts() = delete;
		Contexts(const Contexts&) = delete;
		Contexts(Contexts&&) = delete;
		Contexts& operator=(const Contexts&) = delete;
		Contexts& operator=(Contexts&&) = delete;

	public:
		/// <summary>
		/// Returns the context of thread/task
		/// </summary>
		/// <param name="onlyFromCurrentContextBlock">If true then context search perform only in current context block ignoring parrents</param>
		/// <returns>Context object</returns>
		template<ThreadableContext TContext>
		static TContext GetContext(bool onlyFromCurrentContextBlock = false) {
			return const_cast<TContext&>(GetContextImpl(TypeRecord<TContext>(), onlyFromCurrentContextBlock).As<TContext>());
		}

		/// <summary>
		/// Sets the value of new context
		/// </summary>
		/// <param name="newContext">New context value</param>
		template<ThreadableContext TContext>
		static void SetContext(const TContext& newContext) {
			SetContextImpl(new TContext(newContext.Clone()));
		}

		/// <summary>
		/// Sets the value of new context
		/// </summary>
		/// <param name="newContext">New context value</param>
		template<ThreadableContext TContext>
		static void SetContext(TContext&& newContext) {
			SetContextImpl(new TContext(newContext.Clone()));
		}

		/// <summary>
		/// Perform removing context record from current context block by TContext type
		/// </summary>
		template<ThreadableContext TContext>
		static void RemoveContext() {
			RemoveContextImpl(TypeRecord<TContext>());
		}

		/// <summary>
		/// Returns "true" if the context of specified type already exists
		/// </summary>
		/// <param name="onlyFromCurrentContextBlock">If true then context search perform only in current context block ignoring parrents</param>
		/// <returns>"True" if the context of specified type already exists</returns>
		template<ThreadableContext TContext>
		static bool IsContextExist(bool onlyFromCurrentContextBlock = false) {
			return IsContextExistImpl(TypeRecord<TContext>(), onlyFromCurrentContextBlock);
		}
	};
}