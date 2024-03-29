export module Cardinal.DependencyInjection:IDI;

import :Base;
import :DI;

export namespace Cardinal::DI
{
	/// <summary>
	/// The Dependency Injection service class
	/// </summary>
	class IDI final {
	public:
		/// <summary>
		/// Service schema
		/// </summary>
		/// <returns>Runtime service type description</returns>
		CttiClassRecord(
			IDI,
			NoCttiInterfaces(),
			NoCttiConstructors(IDI),
			CttiMethodGroup(
				/*CttiMethodRecord(IDI, GetServiceRecordByName),
				CttiExtendedMethodRecord(
					IDI,
					GetServiceDependenciesByServiceName,
					Containers::List<RTTI::Record::RttiTypeRecord>(IDI::*)(const String& serviceTypeName) const),
				CttiExtendedMethodRecord(
					IDI,
					GetServiceDependenciesByServiceName,
					Containers::List<RTTI::Record::RttiTypeRecord>(IDI::*)(String&& serviceTypeName) const)*/
			),
			CttiProperties(
				//CttiPropertyRecord(AllServicesNames, CttiMethodGroup(CttiMethodRecord(IDI, GetAllServicesNames)), CttiMethodGroup())
			)
		);

	private:
		DependencyInjection& di;

	private:
		IDI(DependencyInjection& di) : di(di) {}

	public:
#pragma region Getters
		ForceInline Containers::List<ConstString> GetAllServicesNames() const {
			return di.GetAllServicesNames();
		}

		ForceInline RTTI::Record::RttiTypeRecord GetServiceRecordByName(const String& name) const {
			return di.GetServiceRecordByName(name);
		}

		/// <summary>
		/// Returns the service dependencies list by its name
		/// </summary>
		/// <param name="serviceTypeName">The service name</param>
		Containers::List<RTTI::Record::RttiTypeRecord> GetServiceDependenciesByServiceName(const String& serviceTypeName) const {
			return di.GetServiceDependenciesByServiceName(serviceTypeName);
		}

		/// <summary>
		/// Returns the service dependencies list by its name
		/// </summary>
		/// <param name="serviceTypeName">The service name</param>
		Containers::List<RTTI::Record::RttiTypeRecord> GetServiceDependenciesByServiceName(String&& serviceTypeName) const {
			return di.GetServiceDependenciesByServiceName(MoveRValue(serviceTypeName));
		}
#pragma endregion

	public:
		/// <summary>
		/// Returns the service by type
		/// </summary>
		template<typename TService>
		TService Resolve() {
			return di.Resolve<TService>();
		}

		/// <summary>
		/// The service registration
		/// </summary>
		/// <param name="rttiRecord">The service metainformation</param>
		template<typename TService, typename ... TCtorArgs>
			requires Concepts::IsConstructibleWith<TService, TCtorArgs...>
		IDI& Register() {
			di.Register<TService, TCtorArgs...>();
			return *this;
		}

		/// <summary>
		/// The service registration by the interface
		/// </summary>
		/// <param name="rttiRecord">The service metainformation</param>
		template<typename TServiceInterface, typename TService, typename ... TCtorArgs>
			requires Concepts::IsConstructibleWith<TService, TCtorArgs...>
		IDI& RegisterInterface() {
			di.RegisterInterface<TServiceInterface, TService, TCtorArgs...>();
			return *this;
		}

	private:
		friend class DependencyInjection;
	};
}