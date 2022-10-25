export module Cardinal.IO:FS.File;

export import :FS.Base;
export import :FS.Exceptions;

export namespace Cardinal::IO::FS
{
	/// <summary>
	/// File object
	/// </summary>
	class File final : IDisposable {
	private:
		struct Data {
			Handle handle;
			SizeT readPosition;
			SizeT writePosition;
			String name;
			OpeningMode mode;
			OpenDisposition openPolicy;
			Threading::SRWLock syncLock;

			RefCountT RefCount;

			Data(Handle handle, String&& name, OpeningMode mode, OpenDisposition openPolicy) :
				handle(handle),
				readPosition(0),
				writePosition(0),
				name(MoveRValue(name)),
				mode(mode),
				openPolicy(openPolicy),
				syncLock(),
				RefCount(1) {
			}
		};

	private:
		Data* This;

	protected:
		virtual void Dispose() override;

	public:
		/// <summary>
		/// Creates the file object and opens it or creates file in system
		/// </summary>
		/// <param name="path">A path to file that can be in "NT" or "Win32" format</param>
		/// <param name="mode">File mode</param>
		/// <param name="policy">File opening policy</param>
		/// <param name="ShareWithOs">The file sharing flag with the rest of system</param>
		File(String&& path, OpeningMode mode, OpenDisposition policy, bool ShareWithOs);

		/// <summary>
		/// Creates the file object and opens it or creates file in system
		/// </summary>
		/// <param name="path">A path to file that can be in "NT" or "Win32" format</param>
		/// <param name="mode">File mode</param>
		/// <param name="policy">File opening policy</param>
		/// <param name="ShareWithOs">The file sharing flag with the rest of system</param>
		File(const String& path, OpeningMode mode, OpenDisposition policy, bool ShareWithOs);

	public:
		/// <summary>
		/// Reads binary data from file
		/// </summary>
		/// <param name="Position">Position in file</param>
		/// <param name="buffer">Array of data</param>
		void Read(SizeT Position, Containers::Blob& buffer);

		/// <summary>
		/// Writing the binary data in file on position
		/// </summary>
		/// <param name="Position">Position in file</param>
		/// <param name="buffer">The buffer in which will writes the data</param>
		void Write(SizeT Position, const Containers::Blob& buffer);

		/// <summary>
		/// Reads binary data from file on position which object controls
		/// </summary>
		/// <param name="buffer">The buffer in which will writes the data</param>
		void Read(Containers::Blob& buffer);

		/// <summary>
		/// Writes binary data from file on position which object controls
		/// </summary>
		/// <param name="buffer">The buffer in which will writes the data</param>
		void Write(const Containers::Blob& buffer);

		/// <summary>
		/// Writes binary data from file on position which object controls
		/// </summary>
		/// <param name="buffer">The buffer in which will writes the data</param>
		void Write(Containers::Blob&& buffer) {
			this->Write(buffer);
		}

		/// <summary>
		/// Writing the binary data in file on position
		/// </summary>
		/// <param name="Position">Position in file</param>
		/// <param name="buffer">The buffer in which will writes the data</param>
		void Write(SizeT Position, Containers::Blob&& buffer) {
			this->Write(Position, buffer);
		}

	public:
		/// <summary>
		/// Resetting data from the data cache
		/// </summary>
		void Flush();

	public:
		/// <summary>
		/// Returs the number of data in file
		/// </summary>
		SizeT GetSize() const;

	public:
		/// <summary>
		/// Returns the reading position
		/// </summary>
		ForceInline SizeT GetReadPosition() const {
			return This->readPosition;
		}

		/// <summary>
		/// Returns the writing position
		/// </summary>
		ForceInline SizeT GetWritePosition() const {
			return This->writePosition;
		}

	public:
		/// <summary>
		/// Returns the path in "NT" format
		/// </summary>
		/// <returns>A path in NT format</returns>
		const String& GetPath() const {
			return This->name;
		}

	public:
		/// <summary>
		/// Reading position
		/// </summary>
		SizeT Property(GetReadPosition) ReadPosition;

		/// <summary>
		/// Writing position
		/// </summary>
		SizeT Property(GetWritePosition) WritePosition;

		/// <summary>
		/// Path in "NT" format
		/// </summary>
		String Property(GetPath) Path;

		/// <summary>
		/// File size
		/// </summary>
		SizeT Property(GetSize) Size;

	public:
		/// <summary>
		/// File deleting
		/// </summary>
		void Delete()&&;

	public:
		RefCountClass(File, This);
	};

	/// <summary>
	/// Opens the existing format
	/// </summary>
	/// <param name="path">Path in "Win32" or "NT" format</param>
	/// <param name="mode">Opening mode</param>
	/// <returns>File object</returns>
	ForceInline static File RawOpenFile(String&& path, OpeningMode mode) {
		return File(
			MoveRValue(path),
			mode,
			OpenDisposition::Open,
			true);
	}

	/// <summary>
	/// Opens the existing format
	/// </summary>
	/// <param name="path">Path in "Win32" or "NT" format</param>
	/// <param name="mode">Opening mode</param>
	/// <returns>File object</returns>
	ForceInline static File RawOpenFile(const String& path, OpeningMode mode) {
		return File(
			path,
			mode,
			OpenDisposition::Open,
			true);
	}

	/// <summary>
	/// Creates new file
	/// </summary>
	/// <param name="path">Path in "Win32" or "NT" format</param>
	/// <param name="mode">Opening mode</param>
	/// <param name="OverrideIfExist">If "true" then the file overrides provided it already exists</param>
	/// <returns>File object</returns>
	ForceInline static File RawCreateFile(String&& path, OpeningMode mode, bool OverrideIfExist) {
		return File(
			MoveRValue(path),
			mode,
			(OverrideIfExist) ? OpenDisposition::OverwriteIf : OpenDisposition::Create,
			true);
	}

	/// <summary>
	/// Creates new file
	/// </summary>
	/// <param name="path">Path in "Win32" or "NT" format</param>
	/// <param name="mode">Opening mode</param>
	/// <param name="OverrideIfExist">If "true" then the file overrides provided it already exists</param>
	/// <returns>File object</returns>
	ForceInline static File RawCreateFile(const String& path, OpeningMode mode, bool OverrideIfExist) {
		return File(
			path,
			mode,
			(OverrideIfExist) ? OpenDisposition::OverwriteIf : OpenDisposition::Create,
			true);
	}

	/// <summary>
	/// Creates or opens the file
	/// </summary>
	/// <param name="path">Path in "Win32" or "NT" format</param>
	/// <param name="mode">Opening mode</param>
	/// <param name="policy">File opening policy (behavior in the presence or absence of a file)</param>
	/// <returns>File object</returns>
	ForceInline static File RawCreateFile(String&& path, OpeningMode mode, OpenDisposition policy) {
		return File(
			MoveRValue(path),
			mode,
			policy,
			true);
	}

	/// <summary>
	/// Creates or opens the file
	/// </summary>
	/// <param name="path">Path in "Win32" or "NT" format</param>
	/// <param name="mode">Opening mode</param>
	/// <param name="policy">File opening policy (behavior in the presence or absence of a file)</param>
	/// <returns>File object</returns>
	ForceInline static File RawCreateFile(const String& path, OpeningMode mode, OpenDisposition policy) {
		return File(
			path,
			mode,
			policy,
			true);
	}
}