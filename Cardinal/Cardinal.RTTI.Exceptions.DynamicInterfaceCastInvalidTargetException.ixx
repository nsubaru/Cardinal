export module Cardinal.RTTI:Exceptions.DynamicInterfaceCastInvalidTargetException;

export import :Exceptions.Base;
export import :Exceptions.GenericRttiException;

export namespace Cardinal::RTTI::Exceptions
{
	/// <summary>
	/// Dynamic interface cast invalid target exception
	/// </summary>
	class DynamicInterfaceCastInvalidTargetException : public GenericRttiException {
	public:
		ForceInline DynamicInterfaceCastInvalidTargetException(const DynamicInterfaceCastInvalidTargetException& orig)
			: GenericRttiException(orig), target(orig.target), castType(orig.castType), actualType(orig.actualType), expectedlType(orig.expectedlType) {}

		ForceInline DynamicInterfaceCastInvalidTargetException(DynamicInterfaceCastInvalidTargetException&& from)
			: GenericRttiException(MoveRValue(from)), target(from.target), castType(from.castType), actualType(from.actualType), expectedlType(from.expectedlType) {}

		ForceInline DynamicInterfaceCastInvalidTargetException& operator=(const DynamicInterfaceCastInvalidTargetException& from) {
			this->~DynamicInterfaceCastInvalidTargetException();
			this->DynamicInterfaceCastInvalidTargetException::DynamicInterfaceCastInvalidTargetException(from);
			return *this;
		}

		ForceInline DynamicInterfaceCastInvalidTargetException& operator=(DynamicInterfaceCastInvalidTargetException&& from) {
			this->~DynamicInterfaceCastInvalidTargetException();
			this->DynamicInterfaceCastInvalidTargetException::DynamicInterfaceCastInvalidTargetException(MoveRValue(from));
			return *this;
		}

	private:
		Cardinal::RTTI::Record::BaseRttiIntefaceRecord<Memory::Heap> target;
		bool castType;
		Cardinal::RTTI::Record::BaseRttiTypeRecord<Memory::Heap> actualType;
		Cardinal::RTTI::Record::BaseRttiTypeRecord<Memory::Heap> expectedlType;

	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
		/// <param Name = 'target'>Target method record</param>
		DynamicInterfaceCastInvalidTargetException(
			SourceLocation sourceLocation,
			Record::BaseRttiIntefaceRecord<Memory::Heap> target,
			bool castType,
			Record::BaseRttiTypeRecord<Memory::Heap> actualType,
			Record::BaseRttiTypeRecord<Memory::Heap> expectedlType
		) :
			GenericRttiException(sourceLocation, L"Dynamic interface cast invalid target object type"),
			target(target),
			castType(castType),
			actualType(actualType),
			expectedlType(expectedlType) {
		}

	public:
		/// <summary>
		/// Retutn target interface record
		/// </summary>
		Record::RttiIntefaceRecord GetTarget() const { return target; }

		/// <summary>
		/// Return cast type(if true then it cast to interface else it cast from interace)
		/// </summary>
		Boolean GetCastType() const { return castType; }

		/// <summary>
		/// Return actual type
		/// </summary>
		Record::RttiTypeRecord  GetActualType() const { return actualType; }

	public:
		/// <summary>
		/// Target interface record
		/// </summary>
		Record::RttiIntefaceRecord Property(GetTarget) Target;

		/// <summary>
		/// Cast type(if true then it cast to interface else it cast from interace)
		/// </summary>
		Boolean Property(GetCastType) CastType;

		/// <summary>
		/// Actual type
		/// </summary>
		Record::RttiTypeRecord  Property(GetActualType) ActualCount;

	public:
		/// <summary>
		/// Text representation of exception information
		/// </summary>
		virtual ExceptionString toString() const override final {
			return ExceptionString::Combine({
				ExceptionString(exceptionName), ExceptionString(L". "),
				(castType ? ExceptionString(L"Durring casting to: ")
						  : ExceptionString(L"Durring casting from: ")), target.TypeName,
				ExceptionString(L"Expected type: "), ExceptionString(expectedlType.TypeName), ExceptionString(L". "),
				ExceptionString(L"Actual type: "), ExceptionString(actualType.TypeName), ExceptionString(L". "),
				location.toString(),
				(innerException == nullptr) ? ExceptionString(L".") : (L". Also there is a nested exception: " + InnerException->toString())
				});
		}

		/// <summary>
		/// Returns the copy of the exception object
		/// </summary>
		/// <returns>A "smart" pointer to an exception object</returns>
		ForceInline virtual ExceptionHolderType Clone() const override {
			return ExceptionHolderType(new DynamicInterfaceCastInvalidTargetException(*this));
		}

	public:
		ForceInline virtual ~DynamicInterfaceCastInvalidTargetException() {}
	};
}