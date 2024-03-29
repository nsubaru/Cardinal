export module Cardinal.Security.SecurityDescriptor:WellKnownSids;

import :Base;

export namespace Cardinal::Security
{
	// All versions of Windows
	constexpr Char NullAuthority[] = L"S-1-0";
	constexpr Char Nobody[] = L"S-1-0-0";
	constexpr Char WorldAuthority[] = L"S-1-1";
	constexpr Char Everyone[] = L"S-1-1-0";
	constexpr Char LocalAuthority[] = L"S-1-2";
	constexpr Char Local[] = L"S-1-2-0";
	constexpr Char CreatorAuthority[] = L"S-1-3";
	constexpr Char CreatorOwner[] = L"S-1-3-0";
	constexpr Char CreatorGroup[] = L"S-1-3-1";
	constexpr Char OwnerRights[] = L"S-1-3-4";
	constexpr Char NonUniqueAuthority[] = L"S-1-4";
	constexpr Char NTAuthority[] = L"S-1-5";
	constexpr Char Dialup[] = L"S-1-5-1";
	constexpr Char Network[] = L"S-1-5-2";
	constexpr Char Batch[] = L"S-1-5-3";
	constexpr Char Interactive[] = L"S-1-5-4";
	constexpr Char Service[] = L"S-1-5-6";
	constexpr Char Anonymous[] = L"S-1-5-7";
	constexpr Char EnterpriseDomainControllers[] = L"S-1-5-9";
	constexpr Char PrincipalSelf[] = L"S-1-5-10";
	constexpr Char AuthenticatedUsers[] = L"S-1-5-11";
	constexpr Char RestrictedCode[] = L"S-1-5-12";
	constexpr Char TerminalServerUsers[] = L"S-1-5-13";
	constexpr Char RemoteInteractiveLogon[] = L"S-1-5-14";
	constexpr Char IisThisOrganization[] = L"S-1-5-17";
	constexpr Char LocalSystem[] = L"S-1-5-18";
	constexpr Char LocalNTAuthority[] = L"S-1-5-19";
	constexpr Char NetworkNTAuthority[] = L"S-1-5-20";
	constexpr Char Administrators[] = L"S-1-5-32-544";
	constexpr Char Users[] = L"S-1-5-32-545";
	constexpr Char Guests[] = L"S-1-5-32-546";
	constexpr Char PowerUsers[] = L"S-1-5-32-547";
	constexpr Char AccountOperators[] = L"S-1-5-32-548";
	constexpr Char ServerOperators[] = L"S-1-5-32-549";
	constexpr Char PrintOperators[] = L"S-1-5-32-550";
	constexpr Char BackupOperators[] = L"S-1-5-32-551";
	constexpr Char Replicators[] = L"S-1-5-32-552";
	constexpr Char StorageReplicaAdministrators[] = L"S-1-5-32-582";
	constexpr Char NTLMAuthentication[] = L"S-1-5-64-10";
	constexpr Char SChannelAuthentication[] = L"S-1-5-64-14";
	constexpr Char DigestAuthentication[] = L"S-1-5-64-21";
	constexpr Char NTService[] = L"S-1-5-80";

	// Windows Server 2003 and later versions
	constexpr Char CreatorOwnerServer[] = L"S-1-3-2";
	constexpr Char CreatorGroupServer[] = L"S-1-3-3";
	constexpr Char Proxy[] = L"S-1-5-8";
	constexpr Char ThisOrganization[] = L"S-1-5-15";
	constexpr Char BuiltinPreWindows2000CompatibleAccess[] = L"S-1-5-32-554";
	constexpr Char BuiltinRemoteDesktopUsers[] = L"S-1-5-32-555";
	constexpr Char BuiltinNetworkConfigurationOperators[] = L"S-1-5-32-556";
	constexpr Char BuiltinIncomingForestTrustBuilders[] = L"S-1-5-32-557";
	constexpr Char BuiltinPerformanceMonitorUsers[] = L"S-1-5-32-558";
	constexpr Char BuiltinPerformanceLogUsers[] = L"S-1-5-32-559";
	constexpr Char BuiltinWindowsAuthorizationAccessGroup[] = L"S-1-5-32-560";
	constexpr Char BuiltinTerminalServerLicenseServers[] = L"S-1-5-32-561";
	constexpr Char BuiltinDistributedCOMUsers[] = L"S-1-5-32-562";

	// Windows Server 2008 and later versions
	constexpr Char ConsoleLogon[] = L"S-1-2-1";
	constexpr Char BuiltinCryptographicOperators[] = L"S-1-5-32-569";
	constexpr Char BuiltinEventLogReaders[] = L"S-1-5-32-573";
	constexpr Char BuiltinCertificateServiceDCOMAccess[] = L"S-1-5-32-574";
	constexpr Char NTServicesAllServices[] = L"S-1-5-80-0";
	constexpr Char AllServices[] = L"S-1-5-80-0";
	constexpr Char NTVirtualMachineVirtualMachines[] = L"S-1-5-83-0";
	constexpr Char WindowsManagerWindowsManagerGroup[] = L"S-1-5-90-0";
	constexpr Char UntrustedMandatoryLevel[] = L"S-1-16-0";
	constexpr Char LowMandatoryLevel[] = L"S-1-16-4096";
	constexpr Char MediumMandatoryLevel[] = L"S-1-16-8192";
	constexpr Char MediumPlusMandatoryLevel[] = L"S-1-16-8448";
	constexpr Char HighMandatoryLevel[] = L"S-1-16-12288";
	constexpr Char SystemMandatoryLevel[] = L"S-1-16-16384";
	constexpr Char ProtectedProcessMandatoryLevel[] = L"S-1-16-20480";
	constexpr Char SecureProcessMandatoryLevel[] = L"S-1-16-28672";

	// Windows Server 2012 and later versions
	constexpr Char BuiltinRDSRemoteAccessServers[] = L"S-1-5-32-575";
	constexpr Char BuiltinRDSEndpointServers[] = L"S-1-5-32-576";
	constexpr Char BuiltinRDSManagementServers[] = L"S-1-5-32-577";
	constexpr Char BuiltinHyperVAdministrators[] = L"S-1-5-32-578";
	constexpr Char BuiltinAccessControlAssistanceOperators[] = L"S-1-5-32-579";
	constexpr Char BuiltinRemoteManagementUsers[] = L"S-1-5-32-580";

	const SensitiveString& GetWellKnownSidByName(const SensitiveString& name);
}