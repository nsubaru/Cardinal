export module Cardinal.IO:FS.AsyncFile;

export import :FS.Base;
export import :FS.Exceptions;

import Cardinal.Threading;

#pragma warning(push)
// This internal implementation file which can use internal API functions
#pragma warning(disable: 4996)

export namespace Cardinal::IO::FS
{
	/// <summary>
	/// Asynchronous file object
	/// </summary>
	/// <typeparam name="TDataType">Packet data type</typeparam>
	class AsyncFile final : public IO::IBasePositionAsyncIO {
	private:
		class BaseAsyncFile final : public IDisposable {
		public:
			friend class AsyncFile;

		protected:
			Handle hFile;
			OpeningMode mode;
			OpenDisposition policy;
			bool ShareWithOs;
			String ntFileName;
			Threading::UnifiedThreadPoolIoTask* poolTask;

			RefCountT RefCount;

		protected:
			virtual void Dispose() override;

		private:
			void OpenFile();

		protected:
			BaseAsyncFile(String&& path, OpeningMode mode, OpenDisposition policy, bool ShareWithOs);

			BaseAsyncFile(const String& path, OpeningMode mode, OpenDisposition policy, bool ShareWithOs);

			BaseAsyncFile(FilePath&& path, OpeningMode mode, OpenDisposition policy, bool ShareWithOs)
				: BaseAsyncFile(MoveRValue(path).Value, mode, policy, ShareWithOs) {}

			BaseAsyncFile(const FilePath& path, OpeningMode mode, OpenDisposition policy, bool ShareWithOs)
				: BaseAsyncFile(path.Value, mode, policy, ShareWithOs) {}

		protected:
			void InitAsyncRead(IIOTask* task, void* buffer, SizeT position, Memory::DataSizeT size, void* ioStatusBlk);
			void InitAsyncWrite(IIOTask* task, void* buffer, SizeT position, Memory::DataSizeT size, void* ioStatusBlk);

		protected:
			void DeleteFile();

		protected:
			virtual ~BaseAsyncFile() override {}
		};

	private:
		BaseAsyncFile* This;

	private:
		AsyncFile(BaseAsyncFile* This) :
			This(This) {};

	public:
		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="ownerPool">A pool that will handle "I / O" requests</param>
		/// <param name="path">A path to file</param>
		/// <param name="mode">File opening mode</param>
		/// <param name="policy">File opening policy</param>
		/// <param name="ShareWithOs">If "true", the file will be opened in shared mode</param>
		AsyncFile(const Threading::UnifiedThreadPool& ownerPool, String&& path, OpeningMode mode, OpenDisposition policy, bool ShareWithOs) :
			This(new BaseAsyncFile(MoveRValue(path), mode, policy, ShareWithOs)
			) {
			This->poolTask = new Threading::UnifiedThreadPoolIoTask(this->RegisterNtIoObjectInThreadPool(ownerPool, This->hFile));
		}

		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="ownerPool">A pool that will handle "I / O" requests</param>
		/// <param name="path">A path to file</param>
		/// <param name="mode">File opening mode</param>
		/// <param name="policy">File opening policy</param>
		/// <param name="ShareWithOs">If "true", the file will be opened in shared mode</param>
		AsyncFile(const Threading::UnifiedThreadPool& ownerPool, const String& path, OpeningMode mode, OpenDisposition policy, bool ShareWithOs) :
			This(new BaseAsyncFile(path, mode, policy, ShareWithOs)
			) {
			This->poolTask = new Threading::UnifiedThreadPoolIoTask(this->RegisterNtIoObjectInThreadPool(ownerPool, This->hFile));
		}

		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="ownerPool">A pool that will handle "I / O" requests</param>
		/// <param name="path">A path to file</param>
		/// <param name="mode">File opening mode</param>
		/// <param name="policy">File opening policy</param>
		/// <param name="ShareWithOs">If "true", the file will be opened in shared mode</param>
		AsyncFile(const Threading::UnifiedThreadPool& ownerPool, FilePath&& path, OpeningMode mode, OpenDisposition policy, bool ShareWithOs) :
			This(new BaseAsyncFile(MoveRValue(path), mode, policy, ShareWithOs)
			) {
			This->poolTask = new Threading::UnifiedThreadPoolIoTask(this->RegisterNtIoObjectInThreadPool(ownerPool, This->hFile));
		}

		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="ownerPool">A pool that will handle "I / O" requests</param>
		/// <param name="path">A path to file</param>
		/// <param name="mode">File opening mode</param>
		/// <param name="policy">File opening policy</param>
		/// <param name="ShareWithOs">If "true", the file will be opened in shared mode</param>
		AsyncFile(const Threading::UnifiedThreadPool& ownerPool, const FilePath& path, OpeningMode mode, OpenDisposition policy, bool ShareWithOs) :
			This(new BaseAsyncFile(path, mode, policy, ShareWithOs)
			) {
			This->poolTask = new Threading::UnifiedThreadPoolIoTask(this->RegisterNtIoObjectInThreadPool(ownerPool, This->hFile));
		}

		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="path">A path to file</param>
		/// <param name="mode">File opening mode</param>
		/// <param name="policy">File opening policy</param>
		/// <param name="ShareWithOs">If "true", the file will be opened in shared mode</param>
		AsyncFile(String&& path, OpeningMode mode, OpenDisposition policy, bool ShareWithOs) :
			This(new BaseAsyncFile(MoveRValue(path), mode, policy, ShareWithOs)
			) {
			This->poolTask = new Threading::UnifiedThreadPoolIoTask(this->RegisterNtIoObjectInThreadPool(
				Threading::Contexts::GetContext<Threading::ThreadPoolThreadContext>().ThreadPool,
				This->hFile));
		}

		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="ownerPool">A pool that will handle "I / O" requests</param>
		/// <param name="path">A path to file</param>
		/// <param name="mode">File opening mode</param>
		/// <param name="policy">File opening policy</param>
		/// <param name="ShareWithOs">If "true", the file will be opened in shared mode</param>
		AsyncFile(const String& path, OpeningMode mode, OpenDisposition policy, bool ShareWithOs) :
			This(new BaseAsyncFile(path, mode, policy, ShareWithOs)
			) {
			This->poolTask = new Threading::UnifiedThreadPoolIoTask(this->RegisterNtIoObjectInThreadPool(
				Threading::Contexts::GetContext<Threading::ThreadPoolThreadContext>().ThreadPool,
				This->hFile));
		}

		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="ownerPool">A pool that will handle "I / O" requests</param>
		/// <param name="path">A path to file</param>
		/// <param name="mode">File opening mode</param>
		/// <param name="policy">File opening policy</param>
		/// <param name="ShareWithOs">If "true", the file will be opened in shared mode</param>
		AsyncFile(FilePath&& path, OpeningMode mode, OpenDisposition policy, bool ShareWithOs) :
			This(new BaseAsyncFile(MoveRValue(path), mode, policy, ShareWithOs)
			) {
			This->poolTask = new Threading::UnifiedThreadPoolIoTask(this->RegisterNtIoObjectInThreadPool(
				Threading::Contexts::GetContext<Threading::ThreadPoolThreadContext>().ThreadPool,
				This->hFile));
		}

		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="ownerPool">A pool that will handle "I / O" requests</param>
		/// <param name="path">A path to file</param>
		/// <param name="mode">File opening mode</param>
		/// <param name="policy">File opening policy</param>
		/// <param name="ShareWithOs">If "true", the file will be opened in shared mode</param>
		AsyncFile(const FilePath& path, OpeningMode mode, OpenDisposition policy, bool ShareWithOs) :
			This(new BaseAsyncFile(path, mode, policy, ShareWithOs)
			) {
			This->poolTask = new Threading::UnifiedThreadPoolIoTask(this->RegisterNtIoObjectInThreadPool(
				Threading::Contexts::GetContext<Threading::ThreadPoolThreadContext>().ThreadPool,
				This->hFile));
		}

	protected:
		virtual void Dispose() override {
			if (This->poolTask != nullptr)
			{
				this->UnRegisterNtIoObjectInThreadPool(*This->poolTask);
				delete This->poolTask;
				This->poolTask = nullptr;
			}
			This->Dispose();
		}

	protected:
		virtual Handle GetHandle() override { return This->hFile; }

		virtual const Threading::UnifiedThreadPoolIoTask& GetIoPoolTask() const override {
			return *This->poolTask;
		}

	public:
		/// <summary>
		/// Returns "true" if object is valid
		/// </summary>
		/// <returns>"True" if object is valid</returns>
		virtual bool GetIsValid() const override { return This->hFile != nullptr; }

	public:
		/// <summary>
		/// Asynchronous reading operation initialization
		/// </summary>
		/// <param name="data">Data buffer with its size</param>
		/// <param name="position">Reading position start</param>
		/// <returns>"I/O" operation object</returns>
		template<typename TDataType = Containers::Blob>
		IOTask<TDataType> AsyncRead(TDataType&& data, SizeT position) {
			if (!IsValid)
				Throw(InvalidHandleUsage(SourcePosition));

			if (!(This->mode & EOpeningMode::Read))
				Throw(FileModeNoSupportedByOperationException(SourcePosition, This->ntFileName));

			// Create task object
			IOTask<TDataType> task = IOTask<TDataType>(
				// Create irp packet with callbacks for init and end I/O request
				this->MakeIrp(
					*This->poolTask,
					new AsyncFile(*this),
					[] (IIOTask& task, IRP& irp, SmartPtr<IAsyncIOBase>& ioObject) {
						IOTask<TDataType>* taskPtr = nullptr;

						// Get request object from interface
						auto& target = ioObject.As<AsyncFile>();

						try
						{
							// Allocate task pointer for thread
							taskPtr = new IOTask<TDataType>(dynamic_cast<IOTask<TDataType>&>(task));

							// Get buffer and its size
							auto [buffer, size] = target.GetRawBufferAndSize(*taskPtr);

							// Notify about arrived io operation
							target.NotifyIoOperation(*target.This->poolTask);

							// Perfom os specific read operations
							target.This->InitAsyncRead(
								taskPtr,
								buffer,
								reinterpret_cast<SizeT>(irp.GetRequestSpecificData()),
								size,
								target.GetReservedPlaceInIrp(&task.GetPayload()));
						}
						catch (...)
						{
							// If reading is failed cancel notification
							target.CancelNotifyIoOperation(*target.This->poolTask);
							// and delete task ptr as it not used
							delete taskPtr;
							// and rethrow exception as we don't how handle it
							ReThrow;
						}
					},
					nullptr,
						reinterpret_cast<ImplSpecificT>(position)),
				MoveRValue(data));

			// Return task object
			return task;
		}

		/// <summary>
		/// Asynchronous writing operation initialization
		/// </summary>
		/// <param name="data">Data buffer with its size</param>
		/// <param name="position">Writing position start</param>
		/// <returns>"I/O" operation object</returns>
		template<typename TDataType = Containers::Blob>
		IOTask<TDataType> AsyncWrite(const TDataType&& data, SizeT position) {
			if (!IsValid)
				Throw(InvalidHandleUsage(SourcePosition));

			if (!(This->mode & EOpeningMode::Write || This->mode & EOpeningMode::WriteThrought))
				Throw(FileModeNoSupportedByOperationException(SourcePosition, This->ntFileName));

			// Create task object
			IOTask<TDataType> task = IOTask<TDataType>(
				// Create irp packet with callbacks for init and end I/O request
				this->MakeIrp(
					*This->poolTask,
					new AsyncFile(*this),
					[] (IIOTask& task, IRP& irp, SmartPtr<IAsyncIOBase>& ioObject) {
						IOTask<TDataType>* taskPtr = nullptr;

						// Get request object from interface
						auto& target = ioObject.As<AsyncFile>();

						try
						{
							// Allocate task pointer for thread
							taskPtr = new IOTask<TDataType>(dynamic_cast<IOTask<TDataType>&>(task));

							// Get buffer and its size
							auto [buffer, size] = target.GetRawBufferAndSize(*taskPtr);

							// Notify about arrived io operation
							target.NotifyIoOperation(*target.This->poolTask);

							// Perfom os specific read operations
							target.This->InitAsyncWrite(
								taskPtr,
								buffer,
								reinterpret_cast<SizeT>(irp.GetRequestSpecificData()),
								size,
								target.GetReservedPlaceInIrp(&task.GetPayload()));
						}
						catch (...)
						{
							// If reading is failed cancel notification
							target.CancelNotifyIoOperation(*target.This->poolTask);
							// and delete task ptr as it not used
							delete taskPtr;
							// and rethrow exception as we don't how handle it
							ReThrow;
						}
					},
					nullptr,
						reinterpret_cast<ImplSpecificT>(position)),
				MoveRValue(data));

			// Return task object
			return task;
		}

	public:
		/// <summary>
		/// File object deleting
		/// </summary>
		void DeleteFile()&& {
			if (!IsValid)
				Throw(InvalidHandleUsage(SourcePosition));

			This->DeleteFile();

			this->UnRegisterNtIoObjectInThreadPool(*This->poolTask);

			delete This->poolTask;
			This->poolTask = nullptr;

			This->Dispose();
		}

	public:
		/// <summary>
		/// Returs the number of data in file
		/// </summary>
		Memory::DataSizeT GetSize() const;

	public:
		/// <summary>
		/// Returns the "NT" file name
		/// </summary>
		/// <returns>A string with the "NT" file name</returns>
		const String& GetFileName() const { return This->ntFileName; }

		/// <summary>
		/// Returns the file opening mode
		/// </summary>
		/// <returns>File opening mode</returns>
		OpeningMode GetOpenMode() const { return This->mode; }

	public:
		/// <summary>
		/// "NT" file name
		/// </summary>
		String Property(GetFileName) FileName;

		/// <summary>
		/// File opening mode
		/// </summary>
		OpeningMode Property(GetOpenMode) OpenMode;

		/// <summary>
		/// "True" if object is valid
		/// </summary>
		Boolean Property(GetIsValid) IsValid;

	public:
		RefCountClass(AsyncFile, This);
	};
}

#pragma warning(pop)