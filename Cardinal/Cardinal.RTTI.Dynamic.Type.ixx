export module Cardinal.RTTI:Dynamic.Type;

export import :Dynamic.Base;

export namespace Cardinal::RTTI
{
	class Dynamic;

	template<typename T>
	concept BlockedDynamicTemplateCtorTypes =
		Concepts::IsSame<TypeTraits::DecayV<T>, Dynamic> ||
		Concepts::IsSame<TypeTraits::DecayV<T>, Record::BaseRttiTypeRecord<Memory::Heap>> ||
		TypeTraits::IsReferenceV<T>;

	/// <summary>
	/// Class for dynamic data
	/// </summary>
	class Dynamic {
	private:
		Cardinal::RTTI::Record::BaseRttiTypeRecord<Memory::Heap> typeRecord;
		RawMemBlk data;

	private:
		Dynamic(void* rawData, Record::BaseRttiTypeRecord<Memory::Heap> type) : typeRecord(type), data(nullptr) {
			const auto* typeCttiData = &typeRecord.record->MetaDataForDynamic;

			data = new Byte[typeCttiData->BinarySize];

			typeCttiData->CopyCtor(data, rawData);
		}

	public:
		/// <summary>
		/// Empty constructor
		/// </summary>
		Dynamic() : typeRecord(Record::GetRttiTypeRecord<void>()), data(nullptr) {}

		/// <summary>
		/// Constructor
		/// </summary>
		/// <typeparam name="T">Dynamic item type</typeparam>
		template<typename T> requires (Concepts::IsConstructibleWith<T> && !BlockedDynamicTemplateCtorTypes<T>)
			Dynamic() : typeRecord(Record::GetRttiTypeRecord<T>()), data(nullptr) {
			const auto* typeCttiData = &typeRecord.record->MetaDataForDynamic;

			data = new Byte[typeCttiData->BinarySize];

			typeCttiData->EmptyConstruct(data);
		}

		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="typeRecord">RTTI type record</param>
		/// <param name="isType">Indicates if the given record is a type</param>
		Dynamic(Record::BaseRttiTypeRecord<Memory::Heap> typeRecord, bool isType = false) : typeRecord(typeRecord), data(nullptr) {
			if (isType == false)
			{
				this->typeRecord = Record::GetRttiTypeRecord<Record::RttiTypeRecord>();
			}

			const auto* typeCttiData = &this->typeRecord.record->MetaDataForDynamic;

			if (typeCttiData->EmptyConstruct == nullptr)
			{
				Throw(NullPointException(SourcePosition));
			}

			data = new Byte[typeCttiData->BinarySize];

			if (isType == false)
			{
				typeCttiData->CopyCtor(data, &typeRecord);
			}
			else
			{
				typeCttiData->EmptyConstruct(data);
			}
		}

		/// <summary>
		/// Constructor
		/// </summary>
		/// <typeparam name="T">Dynamic item type</typeparam>
		/// <param name="obj">Object of the specified type</param>
		template<typename T> requires (Concepts::IsConstructibleWith<T, const T&> && !BlockedDynamicTemplateCtorTypes<T>)
			Dynamic(const T& obj) : typeRecord(Record::GetRttiTypeRecord<T>()), data(nullptr) {
			const auto* typeCttiData = &typeRecord.record->MetaDataForDynamic;

			data = new Byte[typeCttiData->BinarySize];

			typeCttiData->CopyCtor(data, (RawMemBlk) &obj);
		}

		/// <summary>
		/// Constructor with LValue reference
		/// </summary>
		/// <typeparam name="T">Dynamic item type</typeparam>
		/// <param name="obj">Object of the specified type</param>
		template<typename T> requires (Concepts::IsConstructibleWith<T, const T&> && !BlockedDynamicTemplateCtorTypes<T>)
			Dynamic(T& obj) : typeRecord(Record::GetRttiTypeRecord<T>()), data(nullptr) {
			const auto* typeCttiData = &typeRecord.record->MetaDataForDynamic;

			data = new Byte[typeCttiData->BinarySize];

			typeCttiData->CopyCtor(data, (RawMemBlk) &obj);
		}

		/// <summary>
		/// Constructor with RValue reference
		/// </summary>
		/// <typeparam name="T">Dynamic item type</typeparam>
		/// <param name="obj">Object of the specified type</param>
		template<typename T> requires (Concepts::IsConstructibleWith<T, T&&> && !BlockedDynamicTemplateCtorTypes<T>)
			Dynamic(T&& obj) : typeRecord(Record::GetRttiTypeRecord<T>()), data(nullptr) {
			const auto* typeCttiData = &typeRecord.record->MetaDataForDynamic;

			data = new Byte[typeCttiData->BinarySize];

			typeCttiData->MoveCtor(data, (RawMemBlk) &obj);
		}

		/// <summary>
		/// Construct dynamic object using rtti constructor record and args to that constructor
		/// </summary>
		/// <param name="ctor">Rtti constructor record to some type</param>
		/// <param name="args">Arguments to constructor</param>
		Dynamic(Record::BaseRttiConstructorRecord<Memory::Heap> ctor, Containers::Array<Dynamic> args) : typeRecord(ctor.Type), data(nullptr) {
			const auto* typeCtorCttiData = ctor.GetRecord();

			*this = typeCtorCttiData->DynamicCtorCaller(ctor, args);
		}

	public:
		/// <summary>
		/// Constructor with LValue reference to an object
		/// </summary>
		/// <param name="dynamic">Dynamic record</param>
		Dynamic(const Dynamic& dynamic) : typeRecord(dynamic.typeRecord), data(nullptr) {
			if (dynamic.data == nullptr)
			{
				return;
			}

			const auto* typeCttiData = &typeRecord.record->MetaDataForDynamic;
			if (typeCttiData->CopyCtor == nullptr)
				Throw(NullPointException(SourcePosition));

			data = new Byte[typeCttiData->BinarySize];

			typeCttiData->CopyCtor(data, dynamic.data);
		}

		/// <summary>
		/// Constructor with RValue reference to an object
		/// </summary>
		/// <param name="dynamic">Dynamic record</param>
		Dynamic(Dynamic&& dynamic) : typeRecord(MoveRValue(dynamic.typeRecord)), data(nullptr) {
			if (dynamic.data == nullptr)
			{
				return;
			}

			const auto* typeCttiData = &typeRecord.record->MetaDataForDynamic;
			if (typeCttiData->MoveCtor == nullptr)
				Throw(NullPointException(SourcePosition));

			data = new Byte[typeCttiData->BinarySize];

			typeCttiData->MoveCtor(data, dynamic.data);
		}

	public:
		/// <summary>
		/// Assigning operator
		/// </summary>
		/// <param name="dynamic">LValue to an assigned value</param>
		/// <returns>A pointer to a new value</returns>
		Dynamic operator=(const Dynamic& dynamic) {
			this->~Dynamic();
			this->Dynamic::Dynamic(dynamic);
			return *this;
		}

		/// <summary>
		/// Assigning operator
		/// </summary>
		/// <param name="dynamic">RValue to an assigned value</param>
		/// <returns>A pointer to a new value</returns>
		Dynamic operator=(Dynamic&& dynamic) {
			this->~Dynamic();
			this->Dynamic::Dynamic(MoveRValue(dynamic));
			return *this;
		}

	public:
		/// <summary>
		/// Type getter for RTTI record
		/// </summary>
		/// <returns>Record type</returns>
		Record::BaseRttiTypeRecord<Memory::Heap> GetType() const {
			return typeRecord;
		}

		/// <summary>
		/// Checking the availability to use toString function
		/// </summary>
		/// <returns>"True" if type has toString function</returns>
		Boolean GetIsToStringAvailable() const {
			return typeRecord.record->MetaDataForDynamic.FToString != nullptr;
		}

		/// <summary>
		/// Type getter property
		/// </summary>
		Record::BaseRttiTypeRecord<Memory::Heap> Property(GetType) Type;

		/// <summary>
		/// ToString availability property
		/// </summary>
		Boolean Property(GetIsToStringAvailable) IsToStringAvailable;

	public:
		/// <summary>
		/// Get the value of dynamic record
		/// </summary>
		/// <typeparam name="T">Dynamic record item type</typeparam>
		/// <returnsValue of dynamic record></returns>
		template<typename T>
		T& GetValue()& {
			if (Record::GetRttiTypeRecord<T>() == typeRecord)
			{
				return *reinterpret_cast<T*>(data);
			}
			else
			{
				Throw(InvalidTypeException(SourcePosition, *typeRecord.CxxTypeInfo, *TypeRecord<T>().CxxTypeInfo));
			}
		}

		/// <summary>
		/// Get the value of dynamic record
		/// </summary>
		/// <typeparam name="T">Dynamic record item type</typeparam>
		/// <returnsValue of dynamic record></returns>
		template<typename T>
		const T& GetValue() const& {
			if (Record::GetRttiTypeRecord<T>() == typeRecord)
			{
				return *reinterpret_cast<T*>(data);
			}
			else
			{
				Throw(InvalidTypeException(SourcePosition, *typeRecord.CxxTypeInfo, *TypeRecord<T>().CxxTypeInfo));
			}
		}

		/// <summary>
		/// Get the value of dynamic record
		/// </summary>
		/// <typeparam name="T">Dynamic record item type</typeparam>
		/// <returnsRValue reference to a dynamic record></returns>
		template<typename T>
		T GetValue()&& {
			if (Record::GetRttiTypeRecord<T>() == typeRecord)
			{
				return MoveRValue(*reinterpret_cast<T*>(data));
			}
			else
			{
				Throw(InvalidTypeException(SourcePosition, *typeRecord.CxxTypeInfo, *TypeRecord<T>().CxxTypeInfo));
			}
		}

	public:
		/// <summary>
		/// Cast the dynamic record to a specified type
		/// </summary>
		/// <typeparam name="T">Dynamic record item type</typeparam>
		/// <returns>Cast result value</returns>
		template<typename T>
		T& As()& {
			if (Record::GetRttiTypeRecord<T>() == typeRecord)
			{
				return *reinterpret_cast<T*>(data);
			}

			Throw(InvalidTypeException(SourcePosition, *typeRecord.CxxTypeInfo, *TypeRecord<T>().CxxTypeInfo));
		}

		/// <summary>
		/// Cast the dynamic record to a specified type
		/// </summary>
		/// <typeparam name="T">Dynamic record item type</typeparam>
		/// <returns>RValue reference to a cast result value</returns>
		template<typename T>
		T As()&& {
			if (Record::GetRttiTypeRecord<T>() == typeRecord)
			{
				return MoveRValue(*reinterpret_cast<T*>(data));
			}

			Throw(InvalidTypeException(SourcePosition, *typeRecord.CxxTypeInfo, *TypeRecord<T>().CxxTypeInfo));
		}

		/// <summary>
		/// Cast the dynamic record to a specified type
		/// </summary>
		/// <typeparam name="T">Dynamic record item type</typeparam>
		/// <returns>Cast result value</returns>
		template<typename T>
		const T& As() const& {
			if (Record::GetRttiTypeRecord<T>() == typeRecord)
			{
				return *reinterpret_cast<T*>(data);
			}

			Throw(InvalidTypeException(SourcePosition, *typeRecord.CxxTypeInfo, *TypeRecord<T>().CxxTypeInfo));
		}

		/// <summary>
		/// Cast the dynamic record to a specified type
		/// </summary>
		/// <typeparam name="T">Dynamic record item type</typeparam>
		/// <returns>Const RValue reference to a cast result value</returns>
		template<typename T>
		const T& As() const&& {
			if (Record::GetRttiTypeRecord<T>() == typeRecord)
			{
				return *reinterpret_cast<T*>(data);
			}

			Throw(InvalidTypeException(SourcePosition, *typeRecord.CxxTypeInfo, *TypeRecord<T>().CxxTypeInfo));
		}

	private:
		const MetaData::MetaDataForDynamicRecord& GetMetaDataForDynamicRecordForDynamic() const {
			return this->typeRecord.record->MetaDataForDynamic;
		}

	public:
		/// <summary>
		/// Executing a specified method for a dynamic record
		/// </summary>
		/// <param name="methodInfo">Information about called method</param>
		/// <param name="args">Method arguments</param>
		/// <returns>Result of method executing</returns>
		Dynamic ExecuteMethod(Record::BaseRttiMethodRecord<Memory::Heap> methodInfo, Containers::Array<Dynamic> args) {
			return methodInfo.GetRecord()->DynamicCaller(*this, methodInfo, args);
		}

		/// <summary>
		/// Receiving the getter of property
		/// </summary>
		/// <param name="propertyInfo">Property information</param>
		/// <returns>Getter of property</returns>
		Dynamic GetProperty(Record::BaseRttiProperty<Memory::Heap> propertyInfo) {
			auto getterRecord = propertyInfo.Getters.At(0);
			return ExecuteMethod(getterRecord, {});
		}

		/// <summary>
		/// Receiving the setter of property
		/// </summary>
		/// <param name="propertyInfo">Property information</param>
		/// <returns>Setter of property</returns>
		void SetProperty(Record::BaseRttiProperty<Memory::Heap> propertyInfo, Dynamic value) {
			auto setterRecord = propertyInfo.Setters.At(0);
			ExecuteMethod(setterRecord, { value });
		}

		/// <summary>
		/// Cast to a specified type
		/// </summary>
		/// <param name="interfaceRecord">Interface record</param>
		/// <returns>Casted value of record</returns>
		Dynamic CastTo(Record::BaseRttiIntefaceRecord<Memory::Heap> interfaceRecord) {
			return interfaceRecord.GetRecord()->DynamicConverterTo(interfaceRecord, *this);
		}

		/// <summary>
		/// Cast from a specified type
		/// </summary>
		/// <param name="interfaceRecord">Interface record</param>
		/// <returns>Casted value of record</returns>
		Dynamic CastFrom(Record::BaseRttiIntefaceRecord<Memory::Heap> interfaceRecord) {
			return interfaceRecord.GetRecord()->DynamicConverterFrom(interfaceRecord, *this);
		}

		/// <summary>
		/// Cast dynamic data to string
		/// </summary>
		/// <returns>String value representation of dynamic data</returns>
		String ToString() {
			const auto* typeCttiData = &typeRecord.record->MetaDataForDynamic;

			if (typeCttiData->FToString == nullptr)
				Throw(NotImplementedException(SourcePosition));

			return typeCttiData->FToString(data);
		}

		/// <summary>
		/// Deserialize data to dynamic by rtti type record
		/// It is not same as deserialize Dynamic it deserialize object and put it data to dynamic obj
		/// </summary>
		/// <param name="type">Target type record</param>
		/// <param name="data">Serialized data</param>
		/// <returns>Dynamic object with deserialized data</returns>
		static Dynamic Deserialize(Record::BaseRttiTypeRecord<Memory::Heap> type, Containers::Blob& data);

	public:
		~Dynamic() {
			if (data != nullptr)
			{
				const auto* typeCttiData = &typeRecord.record->MetaDataForDynamic;
				typeCttiData->Dtor(data);

				delete[](Byte*)data;
				data = nullptr;
			}
		}

	public:
		template<typename U>
		friend struct Cardinal::Serialization::SerializationHandler;

		template<typename>
		friend class BaseTraveler;
	};
}