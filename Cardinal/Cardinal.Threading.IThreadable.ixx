export module Cardinal.Threading:IThreadable;

export import Cardinal.Containers.Concurency.Array;
export import Cardinal.Containers.Concurency.List;
export import Cardinal.Containers.SmartPtr;
export import Cardinal.String;

export import :Base;
export import :Context;

export namespace Cardinal::Threading
{
	/// <summary>
	/// Threadable entities interface
	/// </summary>
	VirtualClass IThreadable{
	public:
		/// <summary>
		/// Returns the new reference
		/// </summary>
		/// <returns>New reference</returns>
		virtual SmartPtr<IThreadable> MakeRef() const = 0;

	public:
		/// <summary>
		/// Entity name receiving
		/// </summary>
		/// <returns>String representation of name</returns>
		virtual const String& GetName() const = 0;

		/// <summary>
		/// Entity name setting
		/// </summary>
		/// <param name="newName">New entity name</param>
		virtual void SetName(const String& newName) = 0;

		/// <summary>
		/// Entity name setting
		/// </summary>
		/// <param name="newName">New entity name</param>
		virtual void SetName(String&& newName) = 0;

		/// <summary>
		/// Entity identifier
		/// </summary>
		/// <returns>Entity identifier</returns>
		virtual ID GetID() const = 0;

		/// <summary>
		/// Type of threadable entity
		/// </summary>
		/// <returns>Type of threadable entity</returns>
		virtual EThreadableTypes GetThreadableType() const = 0;

		/// <summary>
		/// State of threadable entity
		/// </summary>
		/// <returns>State of threadable entity</returns>
		virtual EThreadabledStates GetThreadabledState() const = 0;

		/// <summary>
		/// Suspending the executing of threadable entity
		/// </summary>
		/// <returns>A reference to an object</returns>
		virtual IThreadable& Suspend() = 0;

		/// <summary>
		/// Resuming the executing of threadable entity
		/// </summary>
		/// <returns>A reference to an object</returns>
		virtual IThreadable& Resume() = 0;

		/// <summary>
		/// Interrupting the executing of threadable entity
		/// </summary>
		/// <returns>A reference to an object</returns>
		virtual IThreadable& Interupt() = 0;

		/// <summary>
		/// Waiting for the executing of threadable entity
		/// </summary>
		/// <returns>A reference to an object</returns>
		virtual IThreadable& Join() = 0;

		/// <summary>
		/// Waiting for the executing of threadable entity with timeout
		/// </summary>
		/// <param name="timeOut">Maximal waiting time</param>
		/// <returns>A reference to an object</returns>
		virtual IThreadable& Join(Time::TimeSpan timeOut) = 0;

		/// <summary>
		/// Waiting for the executing of threadable entity
		/// </summary>
		/// <returns>A reference to an object</returns>
		virtual const IThreadable& Join() const = 0;

		/// <summary>
		/// Waiting for the executing of threadable entity with timeout
		/// </summary>
		/// <param name="timeOut">Maximal waiting time</param>
		/// <returns>A reference to an object</returns>
		virtual const IThreadable& Join(Time::TimeSpan timeOut) const = 0;

		/// <summary>
		/// Returns the context of specified object
		/// </summary>
		virtual const ContextsBlock& GetContext() const = 0;

	public:
		virtual ~IThreadable() /*= 0*/ {}
	};

	namespace Threadables
	{
		/// <summary>
		/// Reference to a thread
		/// </summary>
		using ThreadableRef = Cardinal::Containers::ConcurencySensitiveList<Cardinal::SmartPtr<Cardinal::Threading::IThreadable>>::Iterator;
	}

	/// <summary>
	/// Returns the number of active threadable entities
	/// </summary>
	/// <param name="WithPseudoThreadable">If "true" then in addition to threads, other threadable entities will be counted</param>
	SizeT GetThreadableCount(Boolean WithPseudoThreadable = false);

	/// <summary>
	/// Returns the list of all threadable entities
	/// </summary>
	Containers::List<SmartPtr<IThreadable>> GetAllThreadable();

	/// <summary>
	/// Returns the reference to a threadable entity within which the threadable code is running
	/// </summary>
	IThreadable& GetThis();
}