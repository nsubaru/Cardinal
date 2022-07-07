#pragma once

typedef NTSTATUS(__cdecl* PWNF_USER_CALLBACK) (
	_In_ WNF_STATE_NAME StateName,
	_In_ WNF_CHANGE_STAMP ChangeStamp,
	_In_opt_ PWNF_TYPE_ID TypeId,
	_In_opt_ PVOID CallbackContext,
	_In_ PVOID Buffer,
	_In_ ULONG BufferSize);

typedef struct _WNF_CONTEXT_HEADER {
	CSHORT                   NodeTypeCode;
	CSHORT                   NodeByteSize;
} WNF_CONTEXT_HEADER, * PWNF_CONTEXT_HEADER;

#define WNF_NODE_SUBSCRIPTION_TABLE  0x911
#define WNF_NODE_NAME_SUBSCRIPTION   0x912
#define WNF_NODE_SERIALIZATION_GROUP 0x913
#define WNF_NODE_USER_SUBSCRIPTION   0x914

#define WNF_STATE_KEY 0x41C64E6DA3BC0074

typedef struct _WNF_STATE_NAME_INTERNAL {
	ULONG64 Version : 4;
	ULONG64 NameLifetime : 2;
	ULONG64 DataScope : 4;
	ULONG64 PermanentData : 1;
	ULONG64 Unique : 53;
} WNF_STATE_NAME_INTERNAL, * PWNF_STATE_NAME_INTERNAL;

typedef struct _WNF_SUBSCRIPTION_TABLE {
	WNF_CONTEXT_HEADER                Header;
	SRWLOCK                           NamesTableLock;
	LIST_ENTRY                        NamesTableEntry;
	LIST_ENTRY                        SerializationGroupListHead;
	SRWLOCK                           SerializationGroupLock;
	DWORD                             Unknown1[2];
	DWORD                             SubscribedEventSet;
	DWORD                             Unknown2[2];
	PTP_TIMER                         Timer;
	ULONG64                           TimerDueTime;
} WNF_SUBSCRIPTION_TABLE, * PWNF_SUBSCRIPTION_TABLE;

typedef struct _WNF_SERIALIZATION_GROUP {
	WNF_CONTEXT_HEADER                Header;
	ULONG                             GroupId;
	LIST_ENTRY                        SerializationGroupList;
	ULONG64                           SerializationGroupValue;
	ULONG64                           SerializationGroupMemberCount;
} WNF_SERIALIZATION_GROUP, * PWNF_SERIALIZATION_GROUP;

typedef struct _WNF_NAME_SUBSCRIPTION {
	WNF_CONTEXT_HEADER                Header;
	ULONG64                           SubscriptionId;
	WNF_STATE_NAME_INTERNAL           StateName;
	WNF_CHANGE_STAMP                  CurrentChangeStamp;
	LIST_ENTRY                        NamesTableEntry;
	PWNF_TYPE_ID                      TypeId;
	SRWLOCK                           SubscriptionLock;
	LIST_ENTRY                        SubscriptionsListHead;
	ULONG                             NormalDeliverySubscriptions;
	ULONG                             NotificationTypeCount[5];
	PWNF_DELIVERY_DESCRIPTOR          RetryDescriptor;
	ULONG                             DeliveryState;
	ULONG64                           ReliableRetryTime;
} WNF_NAME_SUBSCRIPTION, * PWNF_NAME_SUBSCRIPTION;

typedef struct _WNF_USER_SUBSCRIPTION {
	WNF_CONTEXT_HEADER                Header;
	LIST_ENTRY                        SubscriptionsListEntry;
	PWNF_NAME_SUBSCRIPTION            NameSubscription;
	PWNF_USER_CALLBACK                Callback;
	PVOID                             CallbackContext;
	ULONG64                           SubProcessTag;
	ULONG                             CurrentChangeStamp;
	ULONG                             DeliveryOptions;
	ULONG                             SubscribedEventSet;
	PWNF_SERIALIZATION_GROUP          SerializationGroup;
	ULONG                             UserSubscriptionCount;
	ULONG64                           Unknown[10];
} WNF_USER_SUBSCRIPTION, * PWNF_USER_SUBSCRIPTION;

NTSYSCALLAPI
NTSTATUS
NTAPI
RtlSubscribeWnfStateChangeNotification(
	_Outptr_ PWNF_USER_SUBSCRIPTION * Subscription,
	_In_ WNF_STATE_NAME StateName,
	_In_ WNF_CHANGE_STAMP ChangeStamp,
	_In_ PWNF_USER_CALLBACK Callback,
	_In_opt_ PVOID CallbackContext,
	_In_opt_ PCWNF_TYPE_ID TypeId,
	_In_opt_ ULONG SerializationGroup,
	_In_opt_ ULONG Unknown
);

NTSYSCALLAPI
NTSTATUS
NTAPI
RtlUnsubscribeWnfStateChangeNotification(
	_Outptr_ PWNF_USER_SUBSCRIPTION Subscription);