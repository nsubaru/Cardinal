export module Cardinal.DependencyInjection:Base;

export import Cardinal.Exceptions;
export import Cardinal.String;
export import Cardinal.RTTI;
export import Cardinal.Containers;

export namespace Cardinal::DI
{
	/// <summary>
	/// Impossible to solve types dependencies for the class generation
	/// </summary>
	class DIResolveException : public Exception {
	private:
		/// <summary>
		/// The type which DI cannot found
		/// </summary>
		const TypeInfo& type;
		/// <summary>
		/// The not found  type name
		/// </summary>
		ExceptionString typeName;

	public:
		DIResolveException(const DIResolveException& orig) : Exception(orig), type(orig.type), typeName(orig.typeName) {}
		DIResolveException(DIResolveException&& from) : Exception(MoveRValue(from)), type(from.type), typeName(MoveRValue(from.typeName)) {}
		DIResolveException& operator=(const DIResolveException& from) { this->~DIResolveException(); this->DIResolveException::DIResolveException(from); return *this; }
		DIResolveException& operator=(DIResolveException&& from) { this->~DIResolveException(); this->DIResolveException::DIResolveException(MoveRValue(from)); return *this; }

	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name="File">The file in which the exception was generated</param>
		/// <param name="Line">The line number in which the exception was generated</param>
		/// <param name="typeName">The type which DI cannot found</param>
		/// <param name="type">The type which DI cannot found</param>
		DIResolveException(SourceLocation sourceLocation, ExceptionString&& typeName, const TypeInfo& type)
			:Exception(sourceLocation, L"Impossible to solve types dependencies for the class generation"), type(type), typeName(MoveRValue(typeName)) {}

		/// <summary>
		/// Text representation of exception information
		/// </summary>
		virtual ExceptionString toString() const override final {
			return ExceptionString::Combine({
				ExceptionString(exceptionName),
				ExceptionString(L". Type name: "), typeName, ExceptionString(L". "),
				location.toString(),
				(innerException == nullptr) ? ExceptionString(L".") : (L". Also there is a nested exception: " + InnerException->toString())
				});
		}

		/// <summary>
		/// Returns the copy of the exception object
		/// </summary>
		/// <returns>A "smart" pointer to an exception object</returns>
		virtual ExceptionHolderType Clone() const override {
			return ExceptionHolderType(new DIResolveException(*this));
		}

		virtual ~DIResolveException() {}
	};

	/// <summary>
	/// The service register exception
	/// </summary>
	class DIRegisterService : public Exception {
	private:
		/// <summary>
		/// The type which DI cannot found
		/// </summary>
		const TypeInfo& type;

		/// <summary>
		/// The not found  type name
		/// </summary>
		ExceptionString typeName;

	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name="file">The file in which the exception was generated</param>
		/// <param name="line">The line number in which the exception was generated</param>
		/// <param name="typeName">The type which DI cannot found</param>
		/// <param name="type">The type which DI cannot found</param>
		DIRegisterService(SourceLocation sourceLocation, ExceptionString&& typeName, const TypeInfo& type)
			:Exception(sourceLocation, L"The service register exception"), type(type), typeName(MoveRValue(typeName)) {}

		/// <summary>
		/// Text representation of exception information
		/// </summary>
		virtual ExceptionString toString() const override final {
			return ExceptionString::Combine({
				ExceptionString(exceptionName),
				ExceptionString(L". Service name: "), typeName, ExceptionString(L". "),
				location.toString(), ExceptionString(L". ")
				});
		}

		virtual ~DIRegisterService() {}
	};

	using DynamicObject = RTTI::Dynamic;
}