#include "Cardinal.IO.FS.Base.h"

#define Cardinal_RT_ALLOW_INTERNAL_NT_API
#include #include "Cardinal.Core.NTAPI.h"

#pragma warning(push)
// This internal implementation file which can use internal API functions
#pragma warning(disable: 4996)

using namespace Cardinal;
using namespace Cardinal::IO::FS;

Containers::List<IO::FS::FSObjectInformation> IO::FS::QueryFilesInDirectory(const String& Directory) {
	Containers::List<FSObjectInformation> files;
	OBJECT_ATTRIBUTES attrbs;
	IO_STATUS_BLOCK io_blk = { 0 };
	UNICODE_STRING str;

	String fullDirectoryName;
	if (!IsNtAdressFormat(Directory))
		fullDirectoryName = GetNtFileName(Directory);
	else
		fullDirectoryName = Directory;

	RtlInitUnicodeStringEx(&str, const_cast<CharStr>(static_cast<ConstCharStr>(fullDirectoryName)));
	InitializeObjectAttributes(&attrbs, &str, OBJ_CASE_INSENSITIVE, NULL, NULL);
	HANDLE directoryHandle;

	NTSTATUS status = NtCreateFile(
		&directoryHandle,
		FILE_LIST_DIRECTORY | FILE_TRAVERSE | SYNCHRONIZE,
		&attrbs,
		&io_blk,
		NULL,
		NULL,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		FILE_OPEN,
		FILE_SYNCHRONOUS_IO_NONALERT | FILE_DIRECTORY_FILE,
		NULL,
		0
	);

	if (!NT_SUCCESS(status))
	{
		Throw(NtException(SourcePosition, status));
	}

	auto data = static_cast<PFILE_DIRECTORY_INFORMATION>(Memory::Heap().AllignedAllocate(1Mb, Memory::DataSizeT(8)));

	try
	{
		SizeT TryAllocating = 1;
		do
		{
			status = NtQueryDirectoryFile(
				directoryHandle,
				NULL,
				NULL,
				NULL,
				&io_blk,
				data,
				10240,
				FileDirectoryInformation,
				FALSE,
				NULL,
				FALSE
			);

			if (status == STATUS_NO_MORE_FILES)
				break;

			if ((status == STATUS_SUCCESS && io_blk.Information == 0)
				|| status == STATUS_BUFFER_OVERFLOW
				|| status == STATUS_BUFFER_TOO_SMALL)
			{
				Memory::Heap().AllignedFree(data, Memory::DataSizeT(8));
				TryAllocating++;
				data = static_cast<PFILE_DIRECTORY_INFORMATION>(Memory::Heap().AllignedAllocate(
					Memory::DataSizeT((1Mb).Bytes * TryAllocating), Memory::DataSizeT(8)));
				continue;
			}

			if (!NT_SUCCESS(status))
			{
				break;
			}

			PFILE_DIRECTORY_INFORMATION ptr = data;
			do
			{
				Types type;

				if ((ptr->FileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
				{
					type = Types::Directory;
				}
				else
				{
					type = Types::File;
				}

				files += FSObjectInformation(
					type,
					((ptr->FileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0),
					(ptr->FileAttributes & FILE_ATTRIBUTE_ARCHIVE) != 0,
					(ptr->FileAttributes & FILE_ATTRIBUTE_SYSTEM) != 0,
					(ptr->FileAttributes & FILE_ATTRIBUTE_HIDDEN) != 0,
					(ptr->FileAttributes & FILE_ATTRIBUTE_COMPRESSED) != 0,
					(ptr->FileAttributes & FILE_ATTRIBUTE_READONLY) != 0,
					(ptr->FileAttributes & FILE_ATTRIBUTE_TEMPORARY) != 0,

					Memory::DataSizeT(static_cast<SizeT>(ptr->EndOfFile.QuadPart)),
					String(reinterpret_cast<PWCHAR>(&ptr->FileName), ptr->FileNameLength / 2) //-V106
				);

				if (ptr->NextEntryOffset == 0)
				{
					break;
				}

				ptr = reinterpret_cast<PFILE_DIRECTORY_INFORMATION>(reinterpret_cast<Byte*>(ptr) + ptr->NextEntryOffset); //-V104
			}
			while (true);
		}
		while (true);
	}
	catch (...)
	{
		Memory::Heap().AllignedFree(data, Memory::DataSizeT(8));
		NtClose(directoryHandle);
		ReThrow;
	}
	Memory::Heap().AllignedFree(data, Memory::DataSizeT(8));
	NtClose(directoryHandle);
	return files;
}

Containers::List<IO::FS::FSObjectInformation> IO::FS::QueryFilesInDirectory(const String& Directory, const String& WildCardPattern) {
	Containers::List<FSObjectInformation> files;
	OBJECT_ATTRIBUTES attrbs;
	IO_STATUS_BLOCK io_blk = { 0 };
	UNICODE_STRING str;
	UNICODE_STRING wildcard;

	String fullDirectoryName;
	if (!IsNtAdressFormat(Directory))
		fullDirectoryName = GetNtFileName(Directory);
	else
		fullDirectoryName = Directory;

	RtlInitUnicodeStringEx(&str, const_cast<CharStr>(static_cast<ConstCharStr>(fullDirectoryName)));
	RtlInitUnicodeStringEx(&wildcard, const_cast<CharStr>(static_cast<ConstCharStr>(WildCardPattern)));
	InitializeObjectAttributes(&attrbs, &str, OBJ_CASE_INSENSITIVE, NULL, NULL);
	HANDLE directoryHandle;

	NTSTATUS status = NtCreateFile(
		&directoryHandle,
		FILE_LIST_DIRECTORY | FILE_TRAVERSE | SYNCHRONIZE,
		&attrbs,
		&io_blk,
		NULL,
		NULL,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		FILE_OPEN,
		FILE_SYNCHRONOUS_IO_NONALERT | FILE_DIRECTORY_FILE,
		NULL,
		0
	);

	if (!NT_SUCCESS(status))
	{
		Throw(NtException(SourcePosition, status));
	}

	auto data = static_cast<PFILE_DIRECTORY_INFORMATION>(Memory::Heap().AllignedAllocate(1Mb, Memory::DataSizeT(8)));

	try
	{
		SizeT TryAllocating = 1;
		do
		{
			status = NtQueryDirectoryFile(
				directoryHandle,
				NULL,
				NULL,
				NULL,
				&io_blk,
				data,
				10240,
				FileDirectoryInformation,
				FALSE,
				&wildcard,
				FALSE
			);

			if (status == STATUS_NO_MORE_FILES)
				break;

			if ((status == STATUS_SUCCESS && io_blk.Information == 0)
				|| status == STATUS_BUFFER_OVERFLOW
				|| status == STATUS_BUFFER_TOO_SMALL)
			{
				Memory::Heap().AllignedFree(data, Memory::DataSizeT(8));
				TryAllocating++;
				data = static_cast<PFILE_DIRECTORY_INFORMATION>(Memory::Heap().AllignedAllocate(
					Memory::DataSizeT((1Mb).Bytes * TryAllocating), Memory::DataSizeT(8)));
				continue;
			}

			if (status == STATUS_NO_SUCH_FILE)
			{
				break;
			}

			if (!NT_SUCCESS(status))
			{
				break;
			}

			PFILE_DIRECTORY_INFORMATION ptr = data;
			do
			{
				Types type;

				if ((ptr->FileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
				{
					type = Types::Directory;
				}
				else
				{
					type = Types::File;
				}

				files += FSObjectInformation(
					type,
					((ptr->FileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0),
					(ptr->FileAttributes & FILE_ATTRIBUTE_ARCHIVE) != 0,
					(ptr->FileAttributes & FILE_ATTRIBUTE_SYSTEM) != 0,
					(ptr->FileAttributes & FILE_ATTRIBUTE_HIDDEN) != 0,
					(ptr->FileAttributes & FILE_ATTRIBUTE_COMPRESSED) != 0,
					(ptr->FileAttributes & FILE_ATTRIBUTE_READONLY) != 0,
					(ptr->FileAttributes & FILE_ATTRIBUTE_TEMPORARY) != 0,

					Memory::DataSizeT(static_cast<SizeT>(ptr->EndOfFile.QuadPart)),
					String(reinterpret_cast<PWCHAR>(&ptr->FileName), ptr->FileNameLength / 2) //-V106
				);

				if (ptr->NextEntryOffset == 0)
				{
					break;
				}

				ptr = reinterpret_cast<PFILE_DIRECTORY_INFORMATION>(reinterpret_cast<Byte*>(ptr) + ptr->NextEntryOffset); //-V104
			}
			while (ptr->NextEntryOffset != 0);
		}
		while (true);
	}
	catch (...)
	{
		Memory::Heap().AllignedFree(data, Memory::DataSizeT(8));
		NtClose(directoryHandle);
		ReThrow;
	}
	Memory::Heap().AllignedFree(data, Memory::DataSizeT(8));
	NtClose(directoryHandle);
	return files;
}

Containers::List<FSObjectInformation> Rem::Core::IO::FS::QueryFilesInDirectoryUnsafe(Handle directoryHandle) {
	Containers::List<FSObjectInformation> files;
	IO_STATUS_BLOCK io_blk = { 0 };

	OBJECT_TYPE_INFORMATION obTypeInfo{};
	ULONG retSz = 0;
	NTSTATUS status = NtQueryObject(directoryHandle, ObjectTypeInformation, &obTypeInfo, sizeof(obTypeInfo), &retSz);

	if (!NT_SUCCESS(status))
	{
		Throw(NtException(SourcePosition, status));
	}

	auto data = static_cast<PFILE_DIRECTORY_INFORMATION>(Memory::Heap().AllignedAllocate(1Mb, Memory::DataSizeT(8)));

	try
	{
		SizeT TryAllocating = 1;
		do
		{
			status = NtQueryDirectoryFile(
				directoryHandle,
				NULL,
				NULL,
				NULL,
				&io_blk,
				data,
				10240,
				FileDirectoryInformation,
				FALSE,
				NULL,
				FALSE
			);

			if (status == STATUS_NO_MORE_FILES)
				break;

			if ((status == STATUS_SUCCESS && io_blk.Information == 0)
				|| status == STATUS_BUFFER_OVERFLOW
				|| status == STATUS_BUFFER_TOO_SMALL)
			{
				Memory::Heap().AllignedFree(data, Memory::DataSizeT(8));
				TryAllocating++;
				data = static_cast<PFILE_DIRECTORY_INFORMATION>(Memory::Heap().AllignedAllocate(
					Memory::DataSizeT((1Mb).Bytes * TryAllocating), Memory::DataSizeT(8)));
				continue;
			}

			if (!NT_SUCCESS(status))
			{
				break;
			}

			PFILE_DIRECTORY_INFORMATION ptr = data;
			do
			{
				Types type;

				if ((ptr->FileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
				{
					type = Types::Directory;
				}
				else
				{
					type = Types::File;
				}

				files += FSObjectInformation(
					type,
					((ptr->FileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0),
					(ptr->FileAttributes & FILE_ATTRIBUTE_ARCHIVE) != 0,
					(ptr->FileAttributes & FILE_ATTRIBUTE_SYSTEM) != 0,
					(ptr->FileAttributes & FILE_ATTRIBUTE_HIDDEN) != 0,
					(ptr->FileAttributes & FILE_ATTRIBUTE_COMPRESSED) != 0,
					(ptr->FileAttributes & FILE_ATTRIBUTE_READONLY) != 0,
					(ptr->FileAttributes & FILE_ATTRIBUTE_TEMPORARY) != 0,

					Memory::DataSizeT(static_cast<SizeT>(ptr->EndOfFile.QuadPart)),
					String(reinterpret_cast<PWCHAR>(&ptr->FileName), ptr->FileNameLength / 2) //-V106
				);

				if (ptr->NextEntryOffset == 0)
				{
					break;
				}

				ptr = reinterpret_cast<PFILE_DIRECTORY_INFORMATION>(reinterpret_cast<Byte*>(ptr) + ptr->NextEntryOffset); //-V104
			}
			while (true);
		}
		while (true);
	}
	catch (...)
	{
		Memory::Heap().AllignedFree(data, Memory::DataSizeT(8));
		ReThrow;
	}
	Memory::Heap().AllignedFree(data, Memory::DataSizeT(8));
	return files;
}

Containers::List<FSObjectInformation> Rem::Core::IO::FS::QueryFilesInDirectoryUnsafe(Handle directoryHandle, const String& WildCardPattern) {
	Containers::List<FSObjectInformation> files;
	IO_STATUS_BLOCK io_blk = { 0 };
	UNICODE_STRING wildcard;

	RtlInitUnicodeStringEx(&wildcard, const_cast<CharStr>(static_cast<ConstCharStr>(WildCardPattern)));

	OBJECT_TYPE_INFORMATION obTypeInfo{};
	ULONG retSz = 0;
	NTSTATUS status = NtQueryObject(directoryHandle, ObjectTypeInformation, &obTypeInfo, sizeof(obTypeInfo), &retSz);

	if (!NT_SUCCESS(status))
	{
		Throw(NtException(SourcePosition, status));
	}

	auto data = static_cast<PFILE_DIRECTORY_INFORMATION>(Memory::Heap().AllignedAllocate(1Mb, Memory::DataSizeT(8)));

	try
	{
		SizeT TryAllocating = 1;
		do
		{
			status = NtQueryDirectoryFile(
				directoryHandle,
				NULL,
				NULL,
				NULL,
				&io_blk,
				data,
				10240,
				FileDirectoryInformation,
				FALSE,
				&wildcard,
				FALSE
			);

			if (status == STATUS_NO_MORE_FILES)
				break;

			if ((status == STATUS_SUCCESS && io_blk.Information == 0)
				|| status == STATUS_BUFFER_OVERFLOW
				|| status == STATUS_BUFFER_TOO_SMALL)
			{
				Memory::Heap().AllignedFree(data, Memory::DataSizeT(8));
				TryAllocating++;
				data = static_cast<PFILE_DIRECTORY_INFORMATION>(Memory::Heap().AllignedAllocate(
					Memory::DataSizeT((1Mb).Bytes * TryAllocating), Memory::DataSizeT(8)));
				continue;
			}

			if (status == STATUS_NO_SUCH_FILE)
			{
				break;
			}

			if (!NT_SUCCESS(status))
			{
				break;
			}

			PFILE_DIRECTORY_INFORMATION ptr = data;
			do
			{
				Types type;

				if ((ptr->FileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
				{
					type = Types::Directory;
				}
				else
				{
					type = Types::File;
				}

				files += FSObjectInformation(
					type,
					((ptr->FileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0),
					(ptr->FileAttributes & FILE_ATTRIBUTE_ARCHIVE) != 0,
					(ptr->FileAttributes & FILE_ATTRIBUTE_SYSTEM) != 0,
					(ptr->FileAttributes & FILE_ATTRIBUTE_HIDDEN) != 0,
					(ptr->FileAttributes & FILE_ATTRIBUTE_COMPRESSED) != 0,
					(ptr->FileAttributes & FILE_ATTRIBUTE_READONLY) != 0,
					(ptr->FileAttributes & FILE_ATTRIBUTE_TEMPORARY) != 0,

					Memory::DataSizeT(static_cast<SizeT>(ptr->EndOfFile.QuadPart)),
					String(reinterpret_cast<PWCHAR>(&ptr->FileName), ptr->FileNameLength / 2) //-V106
				);

				if (ptr->NextEntryOffset == 0)
				{
					break;
				}

				ptr = reinterpret_cast<PFILE_DIRECTORY_INFORMATION>(reinterpret_cast<Byte*>(ptr) + ptr->NextEntryOffset); //-V104
			}
			while (ptr->NextEntryOffset != 0);
		}
		while (true);
	}
	catch (...)
	{
		Memory::Heap().AllignedFree(data, Memory::DataSizeT(8));
		ReThrow;
	}
	Memory::Heap().AllignedFree(data, Memory::DataSizeT(8));
	return files;
}

String IO::FS::GetNtFileName(const String& Win32Name) {
	auto buf = new Char[32767];
	UNICODE_STRING str = { 0 };
	RtlGetFullPathName_U(const_cast<CharStr>(static_cast<ConstCharStr>(Win32Name)), 32767, buf, NULL);
	RtlDosPathNameToNtPathName_U(buf, &str, NULL, NULL);
	String result = str.Buffer;
	RtlFreeUnicodeString(&str);
	delete[] buf;
	return result;
}

bool Rem::Core::IO::FS::IsNtAdressFormat(const String& FileAdress) {
	return
		FileAdress.IsStartWith(L"\\Device\\") ||
		FileAdress.IsStartWith(L"\\DosDevice\\") ||
		FileAdress.IsStartWith(L"\\SystemRoot");
}