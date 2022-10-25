// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

// ReSharper disable CppZeroConstantCanBeReplacedWithNullptr
#include "Cardinal.IO.FS.File.h"

#define Cardinal_RT_ALLOW_INTERNAL_NT_API
#include #include "Cardinal.Core.NTAPI.h"

#pragma warning(push)
// This internal implementation file which can use internal API functions
#pragma warning(disable: 4996)

using namespace Cardinal;
using namespace Cardinal::IO::FS;

void Cardinal::IO::FS::File::Dispose() {
	NtClose(This->handle);
}

Rem::Core::IO::FS::File::File(String&& path, OpeningMode mode, OpenDisposition policy, bool ShareWithOs) : This(nullptr) {
	String fullPath;

	if (!IsNtAdressFormat(path))
		fullPath = GetNtFileName(path);
	else
		fullPath = MoveRValue(path);

	UNICODE_STRING str;
	RtlInitUnicodeString(&str, const_cast<CharStr>(static_cast<ConstCharStr>(fullPath)));

	OBJECT_ATTRIBUTES attrb;
	InitializeObjectAttributes(&attrb, &str, NULL, NULL, NULL);
	ULONG FileMode = 0;
	ULONG Disposition = 0;
	ULONG ShareMode = 0;
	ULONG Attributes = FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_ALERT;

	if (mode & EOpeningMode::Read)
	{
		FileMode |= GENERIC_READ;
		ShareMode |= (ShareWithOs) ? (FILE_SHARE_WRITE | FILE_SHARE_READ) : NULL;
	}
	if (mode & EOpeningMode::Write)
	{
		FileMode |= GENERIC_WRITE;
		ShareMode |= (ShareWithOs) ? (FILE_SHARE_READ) : NULL;
	}
	if (mode & EOpeningMode::WriteThrought)
	{
		Attributes |= FILE_WRITE_THROUGH;
	}
	if (mode & EOpeningMode::Delete)
	{
		FileMode |= DELETE;
	}

	switch (policy)
	{
		case OpenDisposition::Superseed:
		Disposition = FILE_SUPERSEDE;
		break;
		case OpenDisposition::Create:
		Disposition = FILE_CREATE;
		break;
		case OpenDisposition::Open:
		Disposition = FILE_OPEN;
		break;
		case OpenDisposition::OpenIf:
		Disposition = FILE_OPEN_IF;
		break;
		case OpenDisposition::Overwrite:
		Disposition = FILE_OVERWRITE;
		break;
		case OpenDisposition::OverwriteIf:
		Disposition = FILE_OVERWRITE_IF;
		break;
		default:
		break;
	}

	HANDLE hFile = nullptr;
	IO_STATUS_BLOCK ioblk = { 0 };
	NTSTATUS status = NtCreateFile(
		&hFile,
		SYNCHRONIZE | FileMode,
		&attrb,
		&ioblk,
		NULL,
		FILE_ATTRIBUTE_NORMAL,
		ShareMode,
		Disposition,
		Attributes,
		NULL,
		NULL);

	if (!NT_SUCCESS(status))
	{
		switch (status)
		{
			case STATUS_OBJECT_NAME_NOT_FOUND:
			case STATUS_NO_SUCH_FILE:
			Throw(FileNotExistException(SourcePosition, fullPath));
			case STATUS_OBJECT_NAME_COLLISION:
			Throw(FileExistException(SourcePosition, fullPath));
			case STATUS_SHARING_VIOLATION:
			Throw(FileBusyException(SourcePosition, fullPath));
			case STATUS_FILE_IS_A_DIRECTORY:
			Throw(DirectoryIsNotFileException(SourcePosition, fullPath));
			case STATUS_FILE_CORRUPT_ERROR:
			Throw(FileIsCoruptedException(SourcePosition, fullPath));
			default:
			Throw(GenericFileException(SourcePosition, fullPath, L"Generic exception"));
		}
	}

	This = new Data(hFile, MoveRValue(fullPath), mode, policy);
}

IO::FS::File::File(const String& path, OpeningMode mode, OpenDisposition policy, bool ShareWithOs) : This(nullptr) {
	String fullPath;

	if (!IsNtAdressFormat(path))
		fullPath = GetNtFileName(path);
	else
		fullPath = path;

	UNICODE_STRING str;
	RtlInitUnicodeString(&str, const_cast<CharStr>(static_cast<ConstCharStr>(fullPath)));

	OBJECT_ATTRIBUTES attrb;
	InitializeObjectAttributes(&attrb, &str, NULL, NULL, NULL);
	ULONG FileMode = 0;
	ULONG Disposition = 0;
	ULONG ShareMode = 0;
	ULONG Attributes = FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_ALERT;

	if (mode & EOpeningMode::Read)
	{
		FileMode |= GENERIC_READ;
		ShareMode |= (ShareWithOs) ? (FILE_SHARE_WRITE | FILE_SHARE_READ) : NULL;
	}
	if (mode & EOpeningMode::Write)
	{
		FileMode |= GENERIC_WRITE;
		ShareMode |= (ShareWithOs) ? (FILE_SHARE_READ) : NULL;
	}
	if (mode & EOpeningMode::WriteThrought)
	{
		Attributes |= FILE_WRITE_THROUGH;
	}
	if (mode & EOpeningMode::Delete)
	{
		FileMode |= DELETE;
	}

	switch (policy)
	{
		case OpenDisposition::Superseed:
		Disposition = FILE_SUPERSEDE;
		break;
		case OpenDisposition::Create:
		Disposition = FILE_CREATE;
		break;
		case OpenDisposition::Open:
		Disposition = FILE_OPEN;
		break;
		case OpenDisposition::OpenIf:
		Disposition = FILE_OPEN_IF;
		break;
		case OpenDisposition::Overwrite:
		Disposition = FILE_OVERWRITE;
		break;
		case OpenDisposition::OverwriteIf:
		Disposition = FILE_OVERWRITE_IF;
		break;
		default:
		break;
	}

	HANDLE hFile;
	IO_STATUS_BLOCK ioblk;
	NTSTATUS status = NtCreateFile(
		&hFile,
		SYNCHRONIZE | FileMode,
		&attrb,
		&ioblk,
		NULL,
		FILE_ATTRIBUTE_NORMAL,
		ShareMode,
		Disposition,
		Attributes,
		NULL,
		NULL);

	if (!NT_SUCCESS(status))
	{
		switch (status)
		{
			case STATUS_OBJECT_NAME_NOT_FOUND:
			case STATUS_NO_SUCH_FILE:
			Throw(FileNotExistException(SourcePosition, fullPath));
			case STATUS_OBJECT_NAME_COLLISION:
			Throw(FileExistException(SourcePosition, fullPath));
			case STATUS_SHARING_VIOLATION:
			Throw(FileBusyException(SourcePosition, fullPath));
			case STATUS_FILE_IS_A_DIRECTORY:
			Throw(DirectoryIsNotFileException(SourcePosition, fullPath));
			case STATUS_FILE_CORRUPT_ERROR:
			Throw(FileIsCoruptedException(SourcePosition, fullPath));
			default:
			Throw(GenericFileException(SourcePosition, fullPath, L"Generic exception"));
		}
	}

	This = new Data(hFile, MoveRValue(fullPath), mode, policy);
}

void IO::FS::File::Read(SizeT Position, Containers::Blob& buffer) {
	if (!(This->mode & EOpeningMode::Read))
		Throw(FileModeNoSupportedByOperationException(SourcePosition, This->name));

	if (this->GetSize() - Position < buffer.Count)
		Throw(CantReadMoreDataThanInExistInFileException(SourcePosition, This->name));

	LARGE_INTEGER pos = { .QuadPart = static_cast<LONGLONG>(Position) };
	IO_STATUS_BLOCK ioblk;

	NTSTATUS status = NtReadFile(
		This->handle,
		NULL,
		NULL,
		NULL,
		&ioblk,
		static_cast<void*>(const_cast<Byte*>(static_cast<const Byte* const>(buffer))),
		static_cast<ULONG>(buffer.Count), //-V202
		&pos,
		NULL
	);

	if (!NT_SUCCESS(status))
		Throw(GenericFileException(SourcePosition, This->name, L"Can't read from file"));
}

void IO::FS::File::Write(SizeT Position, const Containers::Blob& buffer) {
	if (!(This->mode & EOpeningMode::Write))
		Throw(FileModeNoSupportedByOperationException(SourcePosition, This->name));

	if (this->GetSize() - Position < buffer.Count)
		Throw(CantReadMoreDataThanInExistInFileException(SourcePosition, This->name));

	LARGE_INTEGER pos = { .QuadPart = static_cast<LONGLONG>(Position) };
	IO_STATUS_BLOCK ioblk;

	NTSTATUS status = NtWriteFile(
		This->handle,
		NULL,
		NULL,
		NULL,
		&ioblk,
		static_cast<void*>(const_cast<Byte*>(static_cast<const Byte* const>(buffer))),
		static_cast<ULONG>(buffer.Count), //-V202
		&pos,
		NULL
	);

	if (!NT_SUCCESS(status))
		Throw(GenericFileException(SourcePosition, This->name, L"Can't write to file"));
}

void IO::FS::File::Read(Containers::Blob& buffer) {
	This->syncLock.ExclusiveLock([&] () mutable {
		if (!(This->mode & EOpeningMode::Read))
			Throw(FileModeNoSupportedByOperationException(SourcePosition, This->name));

		if (this->GetSize() - This->readPosition < buffer.Count)
			Throw(CantReadMoreDataThanInExistInFileException(SourcePosition, This->name));

		LARGE_INTEGER pos = { .QuadPart = static_cast<LONGLONG>(This->readPosition) };
		IO_STATUS_BLOCK ioblk;

		NTSTATUS status = NtReadFile(
			This->handle,
			NULL,
			NULL,
			NULL,
			&ioblk,
			static_cast<void*>(const_cast<Byte*>(static_cast<const Byte* const>(buffer))),
			static_cast<ULONG>(buffer.Count), //-V202
			&pos,
			NULL
		);

		if (!NT_SUCCESS(status))
			Throw(GenericFileException(SourcePosition, This->name, L"Can't read from file"));

		This->readPosition += buffer.Count;
		});
}

void IO::FS::File::Write(const Containers::Blob& buffer) {
	This->syncLock.ExclusiveLock([&] () mutable {
		if (!(This->mode & EOpeningMode::Write || This->mode & EOpeningMode::WriteThrought))
			Throw(FileModeNoSupportedByOperationException(SourcePosition, This->name));

		if (this->GetSize() < This->writePosition)
			Throw(CantReadMoreDataThanInExistInFileException(SourcePosition, This->name));

		LARGE_INTEGER pos = { .QuadPart = static_cast<LONGLONG>(This->writePosition) };
		IO_STATUS_BLOCK ioblk;

		NTSTATUS status = NtWriteFile(
			This->handle,
			NULL,
			NULL,
			NULL,
			&ioblk,
			static_cast<void*>(const_cast<Byte*>(static_cast<const Byte* const>(buffer))),
			static_cast<ULONG>(buffer.Count), //-V202
			&pos,
			NULL
		);

		if (!NT_SUCCESS(status))
			Throw(GenericFileException(SourcePosition, This->name, L"Can't read from file"));

		This->writePosition += buffer.Count;
		});
}

void IO::FS::File::Flush() {
	IO_STATUS_BLOCK ioblk;
	NTSTATUS status = NtFlushBuffersFile(This->handle, &ioblk);

	if (!NT_SUCCESS(status))
		Throw(GenericFileException(SourcePosition, This->name, L"Can't flush file"));
}

SizeT IO::FS::File::GetSize() const {
	FILE_STANDARD_INFORMATION  end_pos;
	IO_STATUS_BLOCK ioblk;
	NtQueryInformationFile(
		This->handle,
		&ioblk,
		&end_pos,
		sizeof(FILE_STANDARD_INFORMATION),
		FileStandardInformation
	);
	return end_pos.EndOfFile.QuadPart;
}

void IO::FS::File::Delete()&& {
	if (!(This->mode & EOpeningMode::Delete))
		Throw(FileModeNoSupportedByOperationException(SourcePosition, This->name));

	FILE_DISPOSITION_INFORMATION dispositionInformation{ TRUE };
	IO_STATUS_BLOCK ioblk;

	NTSTATUS status = NtSetInformationFile(
		This->handle,
		&ioblk,
		&dispositionInformation,
		sizeof(FILE_DISPOSITION_INFORMATION),
		FileDispositionInformation
	);

	if (!NT_SUCCESS(status))
		Throw(GenericFileException(SourcePosition, This->name, L"Can't close file"));
}

#pragma warning(pop)