export module Cardinal.DependencyInjection:DI;

import :Base;

export namespace Cardinal::DI
{
	class DependencyInjection;
	class IDI;

	/// <summary>
	/// The Dependency Injection class
	/// </summary>
	class DependencyInjection final : IDisposable {
	public:
		/// <summary>
		/// Dependency Injection schema
		/// </summary>
		/// <returns>Runtime service type description</returns>
		CttiClassRecord(
			DependencyInjection,
			NoCttiInterfaces(),
			NoCttiConstructors(DependencyInjection),
			CttiMethodGroup(
				/*CttiMethodRecord(DependencyInjection, GetServiceRecordByName),
				CttiExtendedMethodRecord(
					DependencyInjection,
					GetServiceDependenciesByServiceName,
					Containers::List<RTTI::Record::RttiTypeRecord>(DependencyInjection::*)(const String& serviceTypeName) const),
				CttiExtendedMethodRecord(
					DependencyInjection,
					GetServiceDependenciesByServiceName,
					Containers::List<RTTI::Record::RttiTypeRecord>(DependencyInjection::*)(String&& serviceTypeName) const)*/
			),
			CttiProperties(
				//CttiPropertyRecord(AllServicesNames, CttiMethodGroup(CttiMethodRecord(DependencyInjection, GetAllServicesNames)), CttiMethodGroup())
			)
		);

	private:
		using TResolverCtor = void* (*)(DependencyInjection& injector);
		using TServiceDtor = void(*)(void*);
		using TInterfaceCaster = void* (*)(void*);

	private:
		struct ServiceRecord {
			TResolverCtor							resolverCtor;
			TInterfaceCaster						interfaceCaster;
			TServiceDtor							serviceDtor;
			RTTI::Record::RttiTypeRecord             		rttiRecord;
			Containers::List<RTTI::Record::RttiTypeRecord>	dependencies;
			void* objPtr;

			ServiceRecord(
				TResolverCtor								resolverCtor,
				TServiceDtor								serviceDtor,
				TInterfaceCaster							interfaceCaster,
				RTTI::Record::RttiTypeRecord&& rttiRecord,
				Containers::List<RTTI::Record::RttiTypeRecord>&& dependencies
			) :
				resolverCtor(resolverCtor),
				interfaceCaster(interfaceCaster),
				serviceDtor(serviceDtor),
				rttiRecord(MoveRValue(rttiRecord)),
				dependencies(MoveRValue(dependencies)),
				objPtr(nullptr) {
			}

			~ServiceRecord() {
				if (objPtr != nullptr)
				{
					serviceDtor(objPtr);
					objPtr = nullptr;
				}
			}
		};

	private:
		struct Data {
			Containers::ConcurencyMap<RTTI::Record::RttiTypeRecord, SmartPtr<ServiceRecord>>			serviceTableForNativeLevel;
			Containers::ConcurencyMap<ConstString, SmartPtr<ServiceRecord>>						serviceTableForManagedLevel;
			Containers::ConcurencyList<SmartPtr<ServiceRecord>>									serviceStore;

			RefCountT																			RefCount;

			Data() :
				serviceTableForNativeLevel(),
				serviceTableForManagedLevel(),
				RefCount(1) {
			}
		};

	private:
		Data* data;

	protected:
		virtual void Dispose() override;

	private:
		DependencyInjection(Data* This) : data(This) { ++data->RefCount; }

	private:
		IDI GetIDI();

	public:
		/// <summary>
		/// The constructor
		/// </summary>
		DependencyInjection();

		/// <summary>
		/// Returns the service by type
		/// </summary>
		template<typename TService>
		TService Resolve() {
			using TTypeService = TypeTraits::RemoveRefV<TService>;

			if constexpr (TypeTraits::IsSameV<TService, IDI>)
			{
				return GetIDI();
			}
			else
			{
				auto type = TypeRecord<TTypeService>();
				if (data->serviceTableForNativeLevel.IsExist(type))
				{
					auto service = data->serviceTableForNativeLevel[type];
					if (service->objPtr == nullptr)
					{
						service->objPtr =
							service->resolverCtor(*this);
					}

					if (service->interfaceCaster != nullptr)
					{
						return static_cast<TService>(*reinterpret_cast<TTypeService*>(service
							->interfaceCaster(service->objPtr)));
					}
					else
					{
						return static_cast<TService>(*reinterpret_cast<TTypeService*>(service->objPtr));
					}
				}

				Throw(DIResolveException(SourcePosition, CTTI::TypeName<TTypeService>(), typeid(TTypeService)));
			}
		};

		/// <summary>
		/// The service registration
		/// </summary>
		/// <param name="rttiRecord">The service metainformation</param>
		template<typename TService, typename ... TCtorArgs>
			requires Concepts::IsConstructibleWith<TService, TCtorArgs...>
		DependencyInjection& Register() {
			static_assert(TypeTraits::IsConstructible<TService, TCtorArgs...>, "Cannot construct service with that types");

			static constexpr auto injectorCtor = [] (DependencyInjection& injector) -> void* {
				if constexpr (Containers::Tuple<TCtorArgs...>::Count == 0)
				{
					return new TService();
				}
				else
				{
					return Containers::Tuple<TCtorArgs...>{injector.Resolve<TCtorArgs>() ... }
					.CallWithArgsFromTuple([](TCtorArgs&& ... args) InlineLambda
						{
							return new TService(args...);
						});
				}
			};

			static constexpr auto serviceDtor = [] (void* pServiceObj)->void {
				delete static_cast<TService*>(pServiceObj);
			};

			auto serviceRecord = SmartPtr<ServiceRecord>{ new ServiceRecord(
				injectorCtor,
				serviceDtor,
				nullptr,
				::TypeRecord<TService>(),
				{ ::TypeRecord<TypeTraits::RemoveRefV<TCtorArgs>>() ... }
			) };

			this->data->serviceStore += serviceRecord;

			this->data->serviceTableForNativeLevel.Insert(TypeRecord<TService>(), serviceRecord);
			this->data->serviceTableForManagedLevel.Insert(serviceRecord->rttiRecord.TypeName, serviceRecord);

			return *this;
		}

		/// <summary>
		/// The service registration by the interface
		/// </summary>
		/// <param name="rttiRecord">The service metainformation</param>
		template<typename TServiceInterface, typename TService, typename ... TCtorArgs>
			requires Concepts::IsConstructibleWith<TService, TCtorArgs...>
		DependencyInjection& RegisterInterface() {
			static_assert(TypeTraits::IsConstructible<TService, TCtorArgs...>, "Cannot construct service with that types");
			static_assert(TypeTraits::IsBasedOnV<TServiceInterface, TService>, "Cannot register service with invalid interface");

			static constexpr auto injectorCtor = [] (DependencyInjection& injector) -> void* {
				if constexpr (Containers::Tuple<TCtorArgs...>::Count == 0)
				{
					return new TService();
				}
				else
				{
					return Containers::Tuple<TCtorArgs...>{injector.Resolve<TCtorArgs>() ... }
					.CallWithArgsFromTuple([](TCtorArgs&& ... args) InlineLambda
						{
							return new TService(args...);
						});
				}
			};

			static constexpr auto interfaceGetter = [] (void* obj) -> void* {
				return static_cast<TServiceInterface*>(static_cast<TService*>(obj));
			};

			if (!TypeRecord<TService>().Interfaces.Any(ArrowFunct(const& x, x.Type == TypeRecord<TServiceInterface>())))
			{
				Throw(DIRegisterService(SourcePosition, CTTI::TypeName<TService>(), typeid(TService)));
			}

			auto serviceRecord = SmartPtr<ServiceRecord>{ new ServiceRecord(
				injectorCtor,
				interfaceGetter,
				TypeRecord<TService>(),
				{ RTTI::Record::RttiTypeRecord(TypeTraits::TypeTag<TCtorArgs>{}) ... }
			) };

			this->data->serviceStore += serviceRecord;

			this->data->serviceTableForNativeLevel.Insert(TypeRecord<TService>(), serviceRecord);
			this->data->serviceTableForManagedLevel.Insert(serviceRecord->rttiRecord.TypeName, serviceRecord);

			return *this;
		}

		/// <summary>
		/// The service deregistration
		/// </summary>
		template<typename TService>
		DependencyInjection& UnRegisterService() {
			UnRegisterServiceByTypeRecord(TypeRecord<TService>());
			return *this;
		}

		/// <summary>
		/// Returns the service names list
		/// </summary>
		ForceInline Containers::List<ConstString> GetAllServicesNames() const {
			return data->serviceTableForManagedLevel.CopyKeyToCollection<Containers::List>();
		}

		/// <summary>
		/// Returns the metainformation about service by its name
		/// </summary>
		/// <param name="name">The service name</param>
		ForceInline RTTI::Record::RttiTypeRecord GetServiceRecordByName(const String& name) const {
			return data->serviceTableForManagedLevel[name]->rttiRecord;
		}

		/// <summary>
		/// Returns the service dependencies list by its name
		/// </summary>
		/// <param name="serviceTypeName">The service name</param>
		/// <returns>A reference to a current object</returns>
		Containers::List<RTTI::Record::RttiTypeRecord> GetServiceDependenciesByServiceName(const String& serviceTypeName) const;

		/// <summary>
		/// Returns the service dependencies list by its name
		/// </summary>
		/// <param name="serviceTypeName">The service name</param>
		Containers::List<RTTI::Record::RttiTypeRecord> GetServiceDependenciesByServiceName(String&& serviceTypeName) const {
			return GetServiceDependenciesByServiceName(serviceTypeName);
		}

	public:
		RefCountClass(DependencyInjection, data);

	public:
		friend class IDI;
	};
}