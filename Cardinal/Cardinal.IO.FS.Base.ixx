export module Cardinal.IO:FS.Base;

export import :Base;
export import :Exceptions;

export namespace Cardinal::IO::FS
{
#pragma region File path type
	/// <summary>
	/// The path delimiter in host system
	/// </summary>
	constexpr Char HostPathDelimiter = L'\\';

	/// <summary>
	/// The path object type in host system
	/// </summary>
	using FilePath = Text::Path<HostPathDelimiter>;
#pragma endregion

#pragma region Reparse Point
	/// <summary>
	/// Reparse points type
	/// </summary>
	enum class ReparsePointType : UInt8 {
		/// <summary>
		/// Symbolic link
		/// </summary>
		Symlink,

		/// <summary>
		/// Junction point
		/// </summary>
		Junction,

		/// <summary>
		/// Mount point
		/// </summary>
		MountPoint,

		/// <summary>
		/// Deduplicated data
		/// </summary>
		Dedup,

		/// <summary>
		/// Unknown type
		/// </summary>
		Unknown
	};

	EnumCttiDefinition(ReparsePointType,
		ReparsePointType::Symlink,
		ReparsePointType::Junction,
		ReparsePointType::MountPoint,
		ReparsePointType::Dedup,
		ReparsePointType::Unknown
	);

	/// <summary>
	/// Reparse points information
	/// </summary>
	struct RepasePointInformation {
		/// <summary>
		/// Reparse points type
		/// </summary>
		ReparsePointType Type;

		/// <summary>
		/// Reparse points raw data
		/// </summary>
		Byte ReparsePointData[(16Kb).Bytes];
	};
#pragma endregion

#pragma region FS object information
	/// <summary>
	/// File object type
	/// </summary>
	enum class Types : UInt8 {
		/// <summary>
		/// File
		/// </summary>
		File,

		/// <summary>
		/// Directory
		/// </summary>
		Directory,

		/// <summary>
		/// Unknown type
		/// </summary>
		Unknown
	};

	EnumCttiDefinition(Types,
		Types::File,
		Types::Directory,
		Types::Unknown
	);

	/// <summary>
	/// Information about file object
	/// </summary>
	struct FSObjectInformation {
		/// <summary>
		/// The object type
		/// </summary>
		Types Type;

		/// <summary>
		/// Is the given object a reparse point?
		/// </summary>
		bool IsReparsePoint;

		union {
			struct {
				/// <summary>
				/// Is archived
				/// </summary>
				Boolean IsArchived : 1;

				/// <summary>
				/// Is system
				/// </summary>
				Boolean IsSystem : 1;

				/// <summary>
				/// Is hiden
				/// </summary>
				Boolean IsHiden : 1;

				/// <summary>
				/// Is compressed
				/// </summary>
				Boolean IsCompresed : 1;

				/// <summary>
				/// Is readonly
				/// </summary>
				Boolean IsReadOnly : 1;

				/// <summary>
				/// Is temporary
				/// </summary>
				Boolean IsTemporary : 1;
			};

			/// <summary>
			/// Object flags
			/// </summary>
			UInt8 Flags;
		};
		/// <summary>
		/// Object size, 0 in directories
		/// </summary>
		Memory::DataSizeT Size;

		/// <summary>
		/// File object name
		/// </summary>
		String Name;

		/// <summary>
		/// File object constructor
		/// </summary>
		/// <param name="Type">Type</param>
		/// <param name="IsReparsePoint">Is reparse point flag</param>
		/// <param name="IsArchived">Is archived flag</param>
		/// <param name="IsSystem">Is system flag</param>
		/// <param name="IsHiden">Is hiden flag</param>
		/// <param name="IsCompresed">Is compressed flag</param>
		/// <param name="IsReadOnly">Is readonly flag</param>
		/// <param name="IsTemporary">Is temporary flag</param>
		/// <param name="Size">Size</param>
		/// <param name="Name">Object name</param>
		FSObjectInformation(
			Types             Type,
			Boolean           IsReparsePoint,
			Boolean           IsArchived,
			Boolean           IsSystem,
			Boolean           IsHiden,
			Boolean           IsCompresed,
			Boolean           IsReadOnly,
			Boolean           IsTemporary,
			Memory::DataSizeT Size,
			const String& Name
		)
			: Type(Type),
			IsReparsePoint(IsReparsePoint),
			IsArchived(IsArchived),
			IsSystem(IsSystem),
			IsHiden(IsHiden),
			IsCompresed(IsCompresed),
			IsReadOnly(IsReadOnly),
			IsTemporary(IsTemporary),
			Size(Size),
			Name(Name) {
			// HACK: clear all invalid flags
			Flags &= ~0b11000000;
		}

		/// <summary>
		/// File object constructor
		/// </summary>
		/// <param name="Type">Type</param>
		/// <param name="IsReparsePoint">Is reparse point flag</param>
		/// <param name="IsArchived">Is archived flag</param>
		/// <param name="IsSystem">Is system flag</param>
		/// <param name="IsHiden">Is hiden flag</param>
		/// <param name="IsCompresed">Is compressed flag</param>
		/// <param name="IsReadOnly">Is readonly flag</param>
		/// <param name="IsTemporary">Is temporary flag</param>
		/// <param name="Size">Size</param>
		/// <param name="Name">Object name</param>
		FSObjectInformation(
			Types             Type,
			Boolean           IsReparsePoint,
			Boolean           IsArchived,
			Boolean           IsSystem,
			Boolean           IsHiden,
			Boolean           IsCompresed,
			Boolean           IsReadOnly,
			Boolean           IsTemporary,
			Memory::DataSizeT Size,
			String&& Name
		)
			: Type(Type),
			IsReparsePoint(IsReparsePoint),
			IsArchived(IsArchived),
			IsSystem(IsSystem),
			IsHiden(IsHiden),
			IsCompresed(IsCompresed),
			IsReadOnly(IsReadOnly),
			IsTemporary(IsTemporary),
			Size(Size),
			Name(MoveRValue(Name)) {
			// HACK: clear all invalid flags
			Flags &= ~0b11000000;
		}

		/// <summary>
		/// An empty constructor, the initialization of invalid object
		/// </summary>
		FSObjectInformation()
			: Type(Types::Unknown),
			IsReparsePoint(false),
			Flags(0),
			Size(0),
			Name() {
		}

		StructCttiDef(FSObjectInformation,
			DefStructFieldData(FSObjectInformation, Type),
			DefStructFieldData(FSObjectInformation, IsReparsePoint),
			DefStructFieldData(FSObjectInformation, Flags),
			DefStructFieldData(FSObjectInformation, Size),
			DefStructFieldData(FSObjectInformation, Name)
		);
	};
#pragma endregion

#pragma region File opening mode and opening disposition
	/// <summary>
	/// Possible file opening modes
	/// </summary>
	enum class EOpeningMode {
		Read, Write, WriteThrought, Delete
	};

	EnumCttiDefinition(EOpeningMode,
		EOpeningMode::Read,
		EOpeningMode::Write,
		EOpeningMode::WriteThrought,
		EOpeningMode::Delete
	);

	/// <summary>
	/// File opening mode
	/// </summary>
	using OpeningMode = EnumFlags<EOpeningMode>;

	/// <summary>
	/// A constant for the reading mode
	/// </summary>
	const inline OpeningMode ReadMode = EOpeningMode::Read;

	/// <summary>
	/// A constant for the writing mode
	/// </summary>
	const inline OpeningMode WriteMode = EOpeningMode::Write;

	/// <summary>
	/// A constant for the writing mode without buffering
	/// </summary>
	const inline OpeningMode WriteThroughtMode = EOpeningMode::WriteThrought;

	/// <summary>
	/// A constant for the deleting mode
	/// </summary>
	const inline OpeningMode DeleteMode = EOpeningMode::Delete;

	/// <summary>
	/// File opening/creating policy
	/// </summary>
	enum class OpenDisposition : UInt8 {
		Superseed,
		Create,
		Open,
		OpenIf,
		Overwrite,
		OverwriteIf
	};
#pragma endregion

#pragma region Miscelenious functionі
	/// <summary>
	/// Returns "true" if path is in "NT" format
	/// </summary>
	/// <param name="FileAdress">Path</param>
	/// <returns>"True" if path is in "NT" format</returns>
	bool IsNtAdressFormat(const String& FileAdress);

	/// <summary>
	/// Converts the standard file/folder name and address in "NT" view
	/// </summary>
	/// <param name="Win32Name">"Win32" view name "C:\..." or relative path</param>
	/// <returns>"NT" path</returns>
	String GetNtFileName(const String& Win32Name);

	/// <summary>
	/// Returns the list of files and directories in given directory
	/// The directory can be specified relatively
	/// </summary>
	/// <param name="Directory">Path to directory</param>
	/// <returns>The list of objects in directory</returns>
	Containers::List<FSObjectInformation> QueryFilesInDirectory(const String& Directory);

	/// <summary>
	/// Returns the list of files and directories in given directory by filtering it by "WildCardPattern" mask
	/// The directory can be specified relatively
	/// </summary>
	/// <param name="Directory">Path to directory</param>
	/// <param name="WildCardPattern">Filtering mask</param>
	/// <returns>The list of objects in directory</returns>
	Containers::List<FSObjectInformation> QueryFilesInDirectory(const String& Directory, const String& WildCardPattern);

	/// <summary>
	/// Returns the list of files and directories in given directory
	/// The directory can be specified relatively
	/// </summary>
	/// <param name="hDirectory">Raw directory handle</param>
	/// <returns>The list of objects in directory</returns>
	InternalApiCall Containers::List<FSObjectInformation> QueryFilesInDirectoryUnsafe(Handle hDirectory);

	/// <summary>
	/// Returns the list of files and directories in given directory by filtering it by "WildCardPattern" mask
	/// The directory can be specified relatively
	/// </summary>
	/// <param name="hDirectory">Raw directory handle</param>
	/// <param name="WildCardPattern">Filtering mask</param>
	/// <returns>The list of objects in directory</returns>
	InternalApiCall Containers::List<FSObjectInformation> QueryFilesInDirectoryUnsafe(Handle hDirectory, const String& WildCardPattern);
#pragma endregion
}