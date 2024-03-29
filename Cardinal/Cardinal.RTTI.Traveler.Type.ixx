module;
#define CARDINAL_RT_ALLOW_INTERNAL_NT_API
#include "Cardinal.Core.NTAPI.h"

export module Cardinal.RTTI:Traveler.Type;

export import :Traveler.Base;

export namespace Cardinal::RTTI
{
	/// <summary>
	/// A base traveler class
	/// </summary>
	/// <typeparam name="THeap">Heap type</typeparam>
	template<typename THeap>
	class BaseTraveler : public Memory::Allocator<THeap>, public IDisposable {
	private:
		template<typename T>
		using Array = Containers::BaseArray<THeap, T>;

	private:
		enum ETraverseMode {
			Offset, Call
		};

		struct TravelRecord : public Memory::Allocator<THeap> {
			ETraverseMode mode;
			SizeT offset;
			MetaData::TUnsafeGet caller;
			SizeT requiredSz;
			MetaData::TDtor dtor;
			Record::RttiTypeRecord type;

			TravelRecord() :
				mode(),
				offset(),
				caller(),
				requiredSz(),
				dtor(),
				type() {
			}

			TravelRecord(
				ETraverseMode mode,
				SizeT offset,
				MetaData::TUnsafeGet caller,
				SizeT requiredSz,
				MetaData::TDtor dtor,
				Record::RttiTypeRecord type
			) :
				mode(mode),
				offset(offset),
				caller(caller),
				requiredSz(requiredSz),
				dtor(dtor),
				type(type) {
			}

			TravelRecord(const TravelRecord& from) :
				mode(from.mode),
				offset(from.offset),
				caller(from.caller),
				requiredSz(from.requiredSz),
				dtor(from.dtor),
				type(from.type) {
			}

			TravelRecord& operator=(const TravelRecord& from) {
				this->~TravelRecord();
				this->TravelRecord::TravelRecord(from);
				return *this;
			}
		};

		struct Data : public Memory::Allocator<THeap> {
			Containers::BaseMap<THeap, ID, Containers::BaseArray<THeap, TravelRecord>> travelMap;
			Containers::BaseArray<THeap, BaseString<THeap>> route;

			RefCountT RefCount;

			Data(Containers::BaseArray<THeap, BaseString<THeap>>&& route) : travelMap(), route(MoveRValue(route)), RefCount(1) {}
		};

		Data* This;

	protected:
		virtual void Dispose() override {}

	public:
		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="route">Route</param>
		BaseTraveler(const BaseString<THeap>& route) : This(new Data(
			route.Split(L'.').MoveTo<Array>()
		)) {
		}

		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="route">RValue reference to a route</param>
		BaseTraveler(BaseString<THeap>&& route) : This(new Data(
			route.Split(L'.').MoveTo<Array>()
		)) {
		}

	private:
		TravelRecord BuildTravelInStructForPathPart(Record::RttiTypeRecord type, SizeT idx) {
			const MetaData::StructMetaData* const structRecord = (const MetaData::StructMetaData* const) (((Byte*) type.record) + type.record->OffsetToCttiMetaData);

			Record::RttiField fieldRecord = type
				.Fields
				.First(ArrowFunctWithCapture(const& x, x.FieldName == This->route[idx]));

			return TravelRecord{
				ETraverseMode::Offset,
				fieldRecord.GetRecord()->Offset,
				structRecord->BaseGetter,
				fieldRecord.GetRecord()->MetaDataForDynamic.BinarySize,
				fieldRecord.GetRecord()->MetaDataForDynamic.Dtor,
				fieldRecord.FieldTypeRecord
			};
		}

		TravelRecord BuildTravelInObjectForPathPart(Record::RttiTypeRecord type, SizeT idx) {
			Record::RttiProperty propertyRecord = type
				.Properties
				.First(ArrowFunctWithCapture(const& x, x.Name == This->route[idx]));

			return TravelRecord{
				ETraverseMode::Call,
				0,
				propertyRecord.GetRecord()->UnsafeGet,
				propertyRecord.GetRecord()->PropertyType.MetaDataForDynamic.BinarySize,
				propertyRecord.GetRecord()->PropertyType.MetaDataForDynamic.Dtor,
				propertyRecord.PropertyType
			};
		}

		TravelRecord BuildTravelInTupleForPathPart(Record::RttiTypeRecord type, SizeT idx) {
			Record::RttiField fieldRecord = type
				.Fields
				.First(ArrowFunctWithCapture(const& x, x.FieldName == This->route[idx]));

			return TravelRecord{
				ETraverseMode::Call,
				0,
				(MetaData::TUnsafeGet) fieldRecord.GetRecord()->Offset,
				fieldRecord.GetRecord()->MetaDataForDynamic.BinarySize,
				fieldRecord.GetRecord()->MetaDataForDynamic.Dtor,
				fieldRecord.FieldTypeRecord
			};
		}

		TravelRecord BuildTravelForPathPart(Record::RttiTypeRecord type, SizeT idx) {
			if (type.TypeClass == EType::Struct)
			{
				return BuildTravelInStructForPathPart(type, idx);
			}
			else if (type.TypeClass == EType::Object || type.TypeClass == EType::Abstract)
			{
				return BuildTravelInObjectForPathPart(type, idx);
			}
			else if (type.TypeClass == EType::Tuple)
			{
				return BuildTravelInTupleForPathPart(type, idx);
			}
			else
			{
				Throw(NullPointException(SourcePosition));
			}
		}

		Array<TravelRecord> OptimizeTravelInstruction(Containers::BaseList<THeap, TravelRecord>& travelInstruction) {
			SizeT count = 1;
			auto prev = travelInstruction.begin();
			auto it = prev; it++;

			while (it != nullptr)
			{
				if (!(prev->mode == ETraverseMode::Offset && it->mode == ETraverseMode::Offset))
				{
					count++;
				}

				prev = it; ++it;
			}

			auto optimizedInstruction = Array<TravelRecord>(count);
			prev = travelInstruction.begin();
			it = prev; it++;

			optimizedInstruction[0] = *prev;
			count = 0;

			while (it != nullptr)
			{
				if (prev->mode == ETraverseMode::Offset && it->mode == ETraverseMode::Offset)
				{
					it->offset += optimizedInstruction[count].offset;
					optimizedInstruction[count] = *it;
				}
				else
				{
					count++;
					optimizedInstruction[count] = *it;
				}

				prev = it; ++it;
			}

			return optimizedInstruction;
		}

		Array<TravelRecord> BuildTravel(Record::RttiTypeRecord type) {
			Containers::BaseList<THeap, TravelRecord> travelInstruction;

			for (SizeT i = 0; i < This->route.Count; i++)
			{
				auto stepInstruction = BuildTravelForPathPart(type, i);
				type = stepInstruction.type;
				travelInstruction += MoveRValue(stepInstruction);
			}

			return OptimizeTravelInstruction(travelInstruction);
		}

		Dynamic TravelUsingTravelRecordArray(void* obj, const Array<TravelRecord>& travelInstruction, SizeT idx = 0) {
			if (idx == travelInstruction.Count)
			{
				return Dynamic(obj, travelInstruction[idx - 1].type);
			}

			switch (travelInstruction[idx].mode)
			{
				case ETraverseMode::Offset:
				{
					if (travelInstruction[idx].caller == nullptr)
					{
						return TravelUsingTravelRecordArray(((Byte*) obj) + travelInstruction[idx].offset, travelInstruction, idx + 1);
					}
					else
					{
						void* buffer = _alloca((int) travelInstruction[idx].requiredSz);
						travelInstruction[idx].caller(obj, buffer);
						auto res = TravelUsingTravelRecordArray(((Byte*) buffer) + travelInstruction[idx].offset, travelInstruction, idx + 1);
						travelInstruction[idx].dtor(buffer);

						return res;
					}
				}

				case ETraverseMode::Call:
				{
					void* buffer = _alloca((int) travelInstruction[idx].requiredSz);
					travelInstruction[idx].caller(obj, buffer);
					auto res = TravelUsingTravelRecordArray(((Byte*) buffer), travelInstruction, idx + 1);
					travelInstruction[idx].dtor(buffer);

					return res;
				}

				default:
				Throw(InvalidArgumentException(SourcePosition, L"travelInstruction[idx].mode", L"Invalid enum value"));
			}
		}

	public:
		/// <summary>
		/// Dynamic travel function
		/// </summary>
		/// <param name="data">Dynamic data</param>
		/// <returns>Travel route</returns>
		Dynamic Travel(const Dynamic& data) {
			if (!This->travelMap.IsExist(data.typeRecord.TypeId))
			{
				This->travelMap.Insert(data.typeRecord.TypeId, BuildTravel(data.typeRecord));
			}

			return TravelUsingTravelRecordArray(data.data, This->travelMap[data.typeRecord.TypeId]);
		}

	public:
		RefCountClass(BaseTraveler, This)
	};
}