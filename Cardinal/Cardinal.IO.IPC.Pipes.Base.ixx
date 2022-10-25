export module Cardinal.IO:IPC.Pipes.Base;

export import :IPC.Base;

export namespace Cardinal::IO::IPC::Pipes
{
	/// <summary>
	/// Pipe work mode
	/// </summary>
	enum class EPipeMode : UInt8 {
		/// <summary>
		/// Readonly
		/// </summary>
		In = 1,
		/// <summary>
		/// Write-only
		/// </summary>
		Out = 2
	};

	/// <summary>
	/// Combined pipe modes
	/// </summary>
	using PipeMode = EnumFlags<EPipeMode>;

	constexpr PipeMode InOutPipeMode = EPipeMode::In | EPipeMode::Out;
}