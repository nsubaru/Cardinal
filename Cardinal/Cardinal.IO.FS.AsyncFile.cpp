// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// ReSharper disable CppZeroConstantCanBeReplacedWithNullptr
module;

#define CARDINAL_RT_ALLOW_INTERNAL_NT_API
#include "Cardinal.Core.NTAPI.h"

module Cardinal.IO;

using namespace Cardinal;
using namespace Cardinal::IO;
using namespace Cardinal::Threading;

void Cardinal::IO::FS::AsyncFile::BaseAsyncFile::Dispose() {
	if (hFile != nullptr)
	{
		NtClose(hFile);
		hFile = nullptr;
	}
}

void Cardinal::IO::FS::AsyncFile::BaseAsyncFile::OpenFile() {
	UNICODE_STRING str = { 0 };
	Unsafe(RtlInitUnicodeString(&str, const_cast<CharStr>(static_cast<ConstCharStr>(ntFileName))));

	OBJECT_ATTRIBUTES attrb;
	InitializeObjectAttributes(&attrb, &str, NULL, NULL, NULL);
	ULONG FileMode = 0;
	ULONG Disposition = 0;
	ULONG ShareMode = 0;
	ULONG Attributes = FILE_NON_DIRECTORY_FILE;

	// Configure file opening and share mode
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

	// Configure file opening policy
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

	IO_STATUS_BLOCK ioblk = { 0 };

	// Call NT service function
	NTSTATUS status = NtCreateFile(
		&hFile,
		FileMode,
		&attrb,
		&ioblk,
		NULL,
		FILE_ATTRIBUTE_NORMAL,
		ShareMode,
		Disposition,
		Attributes,
		NULL,
		NULL);

	// Analyze the NT service operation result code
	if (!NT_SUCCESS(status))
	{
		switch (status)
		{
			case STATUS_OBJECT_NAME_NOT_FOUND:
			case STATUS_NO_SUCH_FILE:
			Throw(FileNotExistException(SourcePosition, ntFileName));
			case STATUS_OBJECT_NAME_COLLISION:
			Throw(FileExistException(SourcePosition, ntFileName));
			case STATUS_SHARING_VIOLATION:
			Throw(FileBusyException(SourcePosition, ntFileName));
			case STATUS_FILE_IS_A_DIRECTORY:
			Throw(DirectoryIsNotFileException(SourcePosition, ntFileName));
			case STATUS_FILE_CORRUPT_ERROR:
			Throw(FileIsCoruptedException(SourcePosition, ntFileName));
			default:
			Throw(GenericFileException(SourcePosition, ntFileName, L"Generic exception"));
		}
	}
}

Cardinal::IO::FS::AsyncFile::BaseAsyncFile::BaseAsyncFile(String&& path, OpeningMode mode, OpenDisposition policy, bool ShareWithOs)
	: hFile(nullptr), mode(mode), policy(policy), ShareWithOs(ShareWithOs), ntFileName(), RefCount(1) {
	// Check is NT file path format, if not than try convert to NT path format
	if (!IsNtAdressFormat(path))
		ntFileName = GetNtFileName(path);
	else
		ntFileName = MoveRValue(path);

	OpenFile();
}

Cardinal::IO::FS::AsyncFile::BaseAsyncFile::BaseAsyncFile(const String& path, OpeningMode mode, OpenDisposition policy, bool ShareWithOs)
	: hFile(nullptr), mode(mode), policy(policy), ShareWithOs(ShareWithOs), ntFileName(), RefCount(1) {
	// Check is NT file path format, if not than try convert to NT path format
	if (!IsNtAdressFormat(path))
		ntFileName = GetNtFileName(path);
	else
		ntFileName = path;

	OpenFile();
}

void Cardinal::IO::FS::AsyncFile::BaseAsyncFile::InitAsyncRead(IIOTask* task, void* buffer, SizeT position, Memory::DataSizeT size, void* ioStatusBlk) {
	LARGE_INTEGER offset = { .QuadPart = static_cast<LONGLONG>(position) };

	NTSTATUS status = NtReadFile(
		this->hFile,
		NULL,
		NULL,
		task,
		static_cast<PIO_STATUS_BLOCK>(ioStatusBlk),
		buffer,
		static_cast<ULONG>(size.Bytes),
		&offset,
		NULL);

	if (!NT_SUCCESS(status))
		Throw(NtException(SourcePosition, status));
}

void Cardinal::IO::FS::AsyncFile::BaseAsyncFile::InitAsyncWrite(IIOTask* task, void* buffer, SizeT position, Memory::DataSizeT size, void* ioStatusBlk) {
	LARGE_INTEGER offset = { .QuadPart = static_cast<LONGLONG>(position) };

	NTSTATUS status = NtWriteFile(
		this->hFile,
		NULL,
		NULL,
		task,
		static_cast<PIO_STATUS_BLOCK>(ioStatusBlk),
		buffer,
		static_cast<ULONG>(size.Bytes),
		&offset,
		NULL);

	if (!NT_SUCCESS(status))
		Throw(NtException(SourcePosition, status));
}

void Cardinal::IO::FS::AsyncFile::BaseAsyncFile::DeleteFile() {
	if (!(mode & EOpeningMode::Delete))
		Throw(FileModeNoSupportedByOperationException(SourcePosition, ntFileName));
	IO_STATUS_BLOCK ioblk = { 0 };

	FILE_DISPOSITION_INFORMATION dispositionInformation{ TRUE };

	// Set deleting on file handle closing
	NTSTATUS status = NtSetInformationFile(
		hFile,
		&ioblk,
		&dispositionInformation,
		sizeof(FILE_DISPOSITION_INFORMATION),
		FileDispositionInformation
	);

	if (!NT_SUCCESS(status))
		Throw(GenericFileException(SourcePosition, ntFileName, L"Can't close file"));
}

Memory::DataSizeT Cardinal::IO::FS::AsyncFile::GetSize() const {
	FILE_STANDARD_INFORMATION  end_pos;
	IO_STATUS_BLOCK ioblk;
	NtQueryInformationFile(
		This->hFile,
		&ioblk,
		&end_pos,
		sizeof(FILE_STANDARD_INFORMATION),
		FileStandardInformation
	);
	return Memory::DataSizeT(end_pos.EndOfFile.QuadPart);
}
