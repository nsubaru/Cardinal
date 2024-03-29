export module Cardinal.Security.SecurityDescriptor:ProcessRights;

import :Base;

export namespace Cardinal::Security
{
	/// <summary>
	/// Process object rights
	/// </summary>
	enum class EProcessObjectRights : UInt64 {
		ProcessTerminate = 1,
		ProcessCreateThread = 2,
		ProcessSetSessionId = 3,
		ProcessVmOperation = 4,
		ProcessVmRead = 5,
		ProcessVmWrite = 6,
		ProcessDupHandle = 7,
		ProcessCreateProcess = 8,
		ProcessSetQuota = 9,
		ProcessSetInformation = 10,
		ProcessQueryInformation = 11,
		ProcessSuspendResume = 12,
		ProcessQueryLimitedInformation = 13,
		ProcessSetLimitedInformation = 14,
		ProcessReserved1 = 15,
		ProcessReserved2 = 16,
		Delete = 17,
		ReadControl = 18,
		WriteDac = 19,
		WriteOwner = 20,
		Synchronize = 21
	};

	EnumCttiDefinition(EProcessObjectRights,
		EProcessObjectRights::ProcessTerminate,
		EProcessObjectRights::ProcessCreateThread,
		EProcessObjectRights::ProcessSetSessionId,
		EProcessObjectRights::ProcessVmOperation,
		EProcessObjectRights::ProcessVmRead,
		EProcessObjectRights::ProcessVmWrite,
		EProcessObjectRights::ProcessDupHandle,
		EProcessObjectRights::ProcessCreateProcess,
		EProcessObjectRights::ProcessSetQuota,
		EProcessObjectRights::ProcessSetInformation,
		EProcessObjectRights::ProcessQueryInformation,
		EProcessObjectRights::ProcessSuspendResume,
		EProcessObjectRights::ProcessQueryLimitedInformation,
		EProcessObjectRights::ProcessSetLimitedInformation,
		EProcessObjectRights::ProcessReserved1,
		EProcessObjectRights::ProcessReserved2,
		EProcessObjectRights::Delete,
		EProcessObjectRights::ReadControl,
		EProcessObjectRights::WriteDac,
		EProcessObjectRights::WriteOwner,
		EProcessObjectRights::Synchronize
	);

	/// <summary>
	/// Securable process data object
	/// </summary>
	class ProcessSecurableObject : public ISecurableObject<ProcessSecurableObject, EProcessObjectRights> {
	public:
		constexpr static auto GenericRead =
			EProcessObjectRights::ProcessVmRead |
			EProcessObjectRights::ProcessQueryInformation |
			EProcessObjectRights::ReadControl;

		constexpr static auto GenericWrite =
			EProcessObjectRights::ProcessCreateThread |
			EProcessObjectRights::ProcessVmOperation |
			EProcessObjectRights::ProcessVmWrite |
			EProcessObjectRights::ProcessDupHandle |
			EProcessObjectRights::ProcessCreateProcess |
			EProcessObjectRights::ProcessSetQuota |
			EProcessObjectRights::ProcessSetInformation |
			EProcessObjectRights::ProcessSuspendResume |
			EProcessObjectRights::ReadControl;

		constexpr static auto GenericExecute =
			EProcessObjectRights::ProcessTerminate |
			EProcessObjectRights::ProcessQueryLimitedInformation |
			EProcessObjectRights::ReadControl |
			EProcessObjectRights::Synchronize;
	};
}