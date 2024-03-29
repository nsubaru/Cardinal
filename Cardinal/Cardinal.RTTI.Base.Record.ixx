export module Cardinal.RTTI:Base.Record;

export import :Base.Base;

export namespace Cardinal::RTTI::Record
{
#pragma region RttiCollection
	template<typename THeap, typename T>
	class BaseRttiCollection;

	/// <summary>
	/// Base RTTI collection with basic heap
	/// </summary>
	/// <typeparam name="T">Collection item type</typeparam>
	template<typename T>
	using RttiCollection = BaseRttiCollection<Memory::Heap, T>;

	/// <summary>
	/// Sensitive RTTI collection with basic heap
	/// </summary>
	/// <typeparam name="T">Collection item type</typeparam>
	template<typename T>
	using SensitiveRttiCollection = BaseRttiCollection<Memory::SensitiveDataHeap, T>;
#pragma endregion

#pragma region RttiIterator
	template<typename THeap, typename T>
	class BaseRttiIterator;

	/// <summary>
	/// Base RTTI iterator with basic heap
	/// </summary>
	/// <typeparam name="T">Iterator item type</typeparam>
	template<typename T>
	using RttiIterator = BaseRttiIterator<Memory::Heap, T>;

	/// <summary>
	/// Sensitive RTTI iterator with basic heap
	/// </summary>
	/// <typeparam name="T">Iterator item type</typeparam>
	template<typename T>
	using SensitiveRttiIterator = BaseRttiIterator<Memory::SensitiveDataHeap, T>;
#pragma endregion

#pragma region RttiField
	template<typename THeap>
	class BaseRttiField;

	/// <summary>
	/// Base RTTI field iterator with basic heap
	/// </summary>
	/// <typeparam name="THeap">Field iterator item type</typeparam>
	template<typename THeap>
	using BaseRttiFieldIterator = BaseRttiIterator<THeap, BaseRttiField<THeap>>;

	/// <summary>
	/// Base RTTI field with basic heap
	/// </summary>
	/// <typeparam name="THeap">Field item type</typeparam>
	template<typename THeap>
	using BaseRttiFieldsCollection = BaseRttiCollection<THeap, BaseRttiField<THeap>>;

	/// <summary>
	/// RTTI field with basic heap
	/// </summary>
	using RttiField = BaseRttiField<Memory::Heap>;

	/// <summary>
	/// Sensitive RTTI field with basic heap
	/// </summary>
	using SensitiveRttiField = BaseRttiField<Memory::SensitiveDataHeap>;

	/// <summary>
	/// RTTI field iterator with basic heap
	/// </summary>
	using RttiFieldIterator = BaseRttiFieldIterator<Memory::Heap>;

	/// <summary>
	/// Sensitive RTTI field iterator with basic heap
	/// </summary>
	using SensitiveRttiFieldIterator = BaseRttiFieldIterator<Memory::SensitiveDataHeap>;

	/// <summary>
	/// RTTI field collection with basic heap
	/// </summary>
	using RttiFieldsCollection = BaseRttiFieldsCollection<Memory::Heap>;

	/// <summary>
	/// Sensitive RTTI field collection with basic heap
	/// </summary>
	using SensitiveRttiFieldsCollection = BaseRttiFieldsCollection<Memory::SensitiveDataHeap>;
#pragma endregion

#pragma region RttiArgumentRecord
	template<typename THeap>
	class BaseRttiArgumentRecord;

	/// <summary>
	/// Base RTTI argument iterator with basic heap
	/// </summary>
	/// <typeparam name="THeap">Argument iterator item type</typeparam>
	template<typename THeap>
	using BaseRttiArgumentIterator = BaseRttiIterator<THeap, BaseRttiArgumentRecord<THeap>>;

	/// <summary>
	/// Base RTTI argument collection with basic heap
	/// </summary>
	/// <typeparam name="THeap">Argument collection item type</typeparam>
	template<typename THeap>
	using BaseRttiArgumentsCollection = BaseRttiCollection<THeap, BaseRttiArgumentRecord<THeap>>;

	/// <summary>
	/// RTTI argument record with basic heap
	/// </summary>
	using RttiArgumentRecord = BaseRttiArgumentRecord<Memory::Heap>;

	/// <summary>
	/// Sensitive RTTI argument record with basic heap
	/// </summary>
	using SensitiveRttiArgumentRecord = BaseRttiArgumentRecord<Memory::SensitiveDataHeap>;

	/// <summary>
	/// RTTI argument record iterator with basic heap
	/// </summary>
	using RttiArgumentRecordIterator = BaseRttiArgumentIterator<Memory::Heap>;

	/// <summary>
	/// Sensitive RTTI argument record iterator with basic heap
	/// </summary>
	using SensitiveRttiArgumentRecordIterator = BaseRttiArgumentIterator<Memory::SensitiveDataHeap>;

	/// <summary>
	/// RTTI argument collection with basic heap
	/// </summary>
	using RttiArgumentsCollection = BaseRttiArgumentsCollection<Memory::Heap>;

	/// <summary>
	/// Sensitive RTTI argument collection with basic heap
	/// </summary>
	using SensitiveRttiArgumentsCollection = BaseRttiArgumentsCollection<Memory::SensitiveDataHeap>;
#pragma endregion

#pragma region RttiIntefaceRecord
	template<typename THeap>
	class BaseRttiIntefaceRecord;

	/// <summary>
	/// Base RTTI interface iterator with basic heap
	/// </summary>
	/// <typeparam name="THeap">Interface iterator item type</typeparam>
	template<typename THeap>
	using BaseRttiInterfaceIterator = BaseRttiIterator<THeap, BaseRttiIntefaceRecord<THeap>>;

	/// <summary>
	/// Base RTTI interface collection with basic heap
	/// </summary>
	/// <typeparam name="THeap">Interface collection item type</typeparam>
	template<typename THeap>
	using BaseRttiInterfacesCollection = BaseRttiCollection<THeap, BaseRttiIntefaceRecord<THeap>>;

	/// <summary>
	/// RTTI interface record with basic heap
	/// </summary>
	using RttiIntefaceRecord = BaseRttiIntefaceRecord<Memory::Heap>;

	/// <summary>
	/// Sensitive RTTI interface record with basic heap
	/// </summary>
	using SensitiveRttiIntefaceRecord = BaseRttiIntefaceRecord<Memory::SensitiveDataHeap>;

	/// <summary>
	/// RTTI interface record iterator with basic heap
	/// </summary>
	using RttiIntefaceRecordIterator = BaseRttiInterfaceIterator<Memory::Heap>;

	/// <summary>
	/// Sensitive RTTI interface record iterator with basic heap
	/// </summary>
	using SensitiveRttiIntefaceRecordIterator = BaseRttiInterfaceIterator<Memory::SensitiveDataHeap>;

	/// <summary>
	/// RTTI interface collection with basic heap
	/// </summary>
	using RttiInterfacesCollection = BaseRttiInterfacesCollection<Memory::Heap>;

	/// <summary>
	/// Sensitive RTTI interface record with basic heap
	/// </summary>
	using SensitiveRttiInterfacesCollection = BaseRttiInterfacesCollection<Memory::SensitiveDataHeap>;
#pragma endregion

#pragma region RttiConstructorRecord
	template<typename THeap>
	class BaseRttiConstructorRecord;

	/// <summary>
	/// Base RTTI constructor iterator with basic heap
	/// </summary>
	/// <typeparam name="THeap">Constructor iterator item type</typeparam>
	template<typename THeap>
	using BaseRttiConstructorIterator = BaseRttiIterator<THeap, BaseRttiConstructorRecord<THeap>>;

	/// <summary>
	/// Base RTTI constructor collection with basic heap
	/// </summary>
	/// <typeparam name="THeap">Constructor collection item heap</typeparam>
	template<typename THeap>
	using BaseRttiConstructorsCollection = BaseRttiCollection<THeap, BaseRttiConstructorRecord<THeap>>;

	/// <summary>
	/// RTTI constructor record with basic heap
	/// </summary>
	using RttiConstructorRecord = BaseRttiConstructorRecord<Memory::Heap>;

	/// <summary>
	/// Sensitive RTTI constructor record with basic heap
	/// </summary>
	using SensitiveRttiConstructorRecord = BaseRttiConstructorRecord<Memory::SensitiveDataHeap>;

	/// <summary>
	/// RTTI constructor iterator with basic heap
	/// </summary>
	using RttiConstructorIterator = BaseRttiConstructorIterator<Memory::Heap>;

	/// <summary>
	/// Sensitive RTTI constructor iterator with basic heap
	/// </summary>
	using SensitiveRttiConstructorIterator = BaseRttiConstructorIterator<Memory::SensitiveDataHeap>;

	/// <summary>
	/// RTTI constructor collection with basic heap
	/// </summary>
	using RttiConstructorsCollection = BaseRttiConstructorsCollection<Memory::Heap>;

	/// <summary>
	/// Sensitive RTTI constructor collection with basic heap
	/// </summary>
	using SensitiveRttiConstructorsCollection = BaseRttiConstructorsCollection<Memory::SensitiveDataHeap>;
#pragma endregion

#pragma region RttiMethodRecord
	template<typename THeap>
	class BaseRttiMethodRecord;

	/// <summary>
	/// Base RTTI method iterator with basic heap
	/// </summary>
	/// <typeparam name="THeap">Method iterator item type</typeparam>
	template<typename THeap>
	using BaseRttiMethodIterator = BaseRttiIterator<THeap, BaseRttiMethodRecord<THeap>>;

	/// <summary>
	/// Base RTTI method collection with basic heap
	/// </summary>
	/// <typeparam name="THeap">Method collection item type</typeparam>
	template<typename THeap>
	using BaseRttiMethodsCollection = BaseRttiCollection<THeap, BaseRttiMethodRecord<THeap>>;

	/// <summary>
	/// RTTI method record with basic heap
	/// </summary>
	using RttiMethodRecord = BaseRttiMethodRecord<Memory::Heap>;

	/// <summary>
	/// Sensitive RTTI method record with basic heap
	/// </summary>
	using SensitiveRttiMethodRecord = BaseRttiMethodRecord<Memory::SensitiveDataHeap>;

	/// <summary>
	/// RTTI method record iterator with basic heap
	/// </summary>
	using RttiMethodRecordIterator = BaseRttiMethodIterator<Memory::Heap>;

	/// <summary>
	/// Sensitive RTTI method record iterator with basic heap
	/// </summary>
	using SensitiveRttiMethodRecordIterator = BaseRttiMethodIterator<Memory::SensitiveDataHeap>;

	/// <summary>
	/// RTTI method collection with basic heap
	/// </summary>
	using RttiMethodsCollection = BaseRttiMethodsCollection<Memory::Heap>;

	/// <summary>
	/// Sensitive RTTI method collection with basic heap
	/// </summary>
	using SensitiveRttiMethodsCollection = BaseRttiMethodsCollection<Memory::SensitiveDataHeap>;
#pragma endregion

#pragma region BaseRttiProperty
	template<typename THeap>
	class BaseRttiProperty;

	/// <summary>
	/// Base RTTI property iterator with basic heap
	/// </summary>
	/// <typeparam name="THeap">Property iterator item type</typeparam>
	template<typename THeap>
	using BaseRttiPropertyIterator = BaseRttiIterator<THeap, BaseRttiProperty<THeap>>;

	/// <summary>
	/// Base RTTI property collection with basic heap
	/// </summary>
	/// <typeparam name="THeap">Property collection item type</typeparam>
	template<typename THeap>
	using BaseRttiPropertiesCollection = BaseRttiCollection<THeap, BaseRttiProperty<THeap>>;

	/// <summary>
	/// RTTI property with basic heap
	/// </summary>
	using RttiProperty = BaseRttiProperty<Memory::Heap>;

	/// <summary>
	/// Sensitive RTTI property with basic heap
	/// </summary>
	using SensitiveRttiProperty = BaseRttiProperty<Memory::SensitiveDataHeap>;

	/// <summary>
	/// RTTI property iterator with basic heap
	/// </summary>
	using RttiPropertyIterator = BaseRttiPropertyIterator<Memory::Heap>;

	/// <summary>
	/// Sensitive RTTI property iterator with basic heap
	/// </summary>
	using SensitiveRttiPropertyIterator = BaseRttiPropertyIterator<Memory::SensitiveDataHeap>;

	/// <summary>
	/// RTTI property collection with basic heap
	/// </summary>
	using RttiPropertiesCollection = BaseRttiPropertiesCollection<Memory::Heap>;

	/// <summary>
	/// Sensitive RTTI property collection with basic heap
	/// </summary>
	using SensitiveRttiPropertiesCollection = BaseRttiPropertiesCollection<Memory::SensitiveDataHeap>;
#pragma endregion

#pragma region RttiTypeRecord
	template<typename THeap>
	class BaseRttiTypeRecord;

	/// <summary>
	/// RTTI type record with basic heap
	/// </summary>
	using RttiTypeRecord = BaseRttiTypeRecord<Memory::Heap>;

	/// <summary>
	/// Sensitive RTTI type record with basic heap
	/// </summary>
	using SensitiveRttiTypeRecord = BaseRttiTypeRecord<Memory::SensitiveDataHeap>;
#pragma endregion

#pragma region GetRttiTypeRecord
	/// <summary>
	/// Base RTTI type record
	/// </summary>
	/// <typeparam name="T">RTTI record item type</typeparam>
	/// <returns>RTTI type record</returns>
	template<typename T>
	BaseRttiTypeRecord<Memory::Heap> GetRttiTypeRecord();

	/// <summary>
	/// Base RTTI type record
	/// </summary>
	/// <typeparam name="UHeap">Heap type</typeparam>
	/// <typeparam name="T">RTTI record item type</typeparam>
	/// <returns>RTTI type record</returns>
	template<typename UHeap, typename T>
	BaseRttiTypeRecord<UHeap> GetRttiTypeRecord();
#pragma endregion
}