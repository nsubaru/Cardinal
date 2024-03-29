export module Cardinal.Settings.ConstSettings;

export import Cardinal.Core;
export import Cardinal.Memory.DataSizeT;

export namespace Cardinal::Settings
{
	/// <summary>
	/// Static settings
	/// </summary>
	struct StaticSettingsData {
		/// <summary>
		/// Max msg size in bytes which can be writed into core log file
		/// </summary>
		inline static constexpr auto MaxKernelLogMsgSize = 4Kb;
	};
}