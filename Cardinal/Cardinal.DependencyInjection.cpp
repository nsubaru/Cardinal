module Cardinal.DependencyInjection;

import Cardinal.Time;

using namespace Cardinal;
using namespace Cardinal::DI;

void Cardinal::DI::DependencyInjection::Dispose() {
	// The service deletion from registry
	data->serviceTableForManagedLevel.RemoveAll();
	data->serviceTableForNativeLevel.RemoveAll();
	// The service deletion
	data->serviceStore.Reverse().RemoveAll();
};

Cardinal::DI::DependencyInjection::DependencyInjection() : data(new Data()) {}

Cardinal::DI::IDI Cardinal::DI::DependencyInjection::GetIDI() {
	return Cardinal::DI::IDI(static_cast<DependencyInjection&>(*this));
}

Cardinal::Containers::List<Cardinal::RTTI::Record::RttiTypeRecord>
Cardinal::DI::DependencyInjection::GetServiceDependenciesByServiceName(const String& serviceTypeName) const {
	if (serviceTypeName == TypeRecord<IDI>().TypeName)
	{
		return {};
	}

	if (!this->data->serviceTableForManagedLevel.IsExist(serviceTypeName))
		Throw(InvalidArgumentException(SourcePosition, L"serviceTypeName", L"service not found"));

	return this->data->serviceTableForManagedLevel[serviceTypeName]->dependencies.CopyTo<Containers::List>(
		[&](const auto& item) InlineLambda->Cardinal::RTTI::Record::RttiTypeRecord{
			if (item == TypeRecord<IDI>())
				return TypeRecord<IDI>();

			return this->data->serviceTableForNativeLevel[item]->rttiRecord;
		});
}