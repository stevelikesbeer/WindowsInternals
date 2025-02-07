#include <ntdef.h>

struct _DISPATCHER_HEADER {
    union {
        volatile LONG Lock;                                                 //0x0
        LONG LockNV;                                                        //0x0
        struct {
            UCHAR Type;                                                     //0x0
            UCHAR Signalling;                                               //0x1
            UCHAR Size;                                                     //0x2
            UCHAR Reserved1;                                                //0x3
        };
        struct {
            UCHAR TimerType;                                                //0x0
            union {
                UCHAR TimerControlFlags;                                    //0x1
                struct {
                    UCHAR Absolute : 1;                                       //0x1
                    UCHAR Wake : 1;                                           //0x1
                    UCHAR EncodedTolerableDelay : 6;                          //0x1
                };
            };
            UCHAR Hand;                                                     //0x2
            union {
                UCHAR TimerMiscFlags;                                       //0x3
                struct {
                    UCHAR Index : 6;                                          //0x3
                    UCHAR Inserted : 1;                                       //0x3
                    volatile UCHAR Expired : 1;                               //0x3
                };
            };
        };
        struct {
            UCHAR Timer2Type;                                               //0x0
            union {
                UCHAR Timer2Flags;                                          //0x1
                struct {
                    UCHAR Timer2Inserted : 1;                                 //0x1
                    UCHAR Timer2Expiring : 1;                                 //0x1
                    UCHAR Timer2CancelPending : 1;                            //0x1
                    UCHAR Timer2SetPending : 1;                               //0x1
                    UCHAR Timer2Running : 1;                                  //0x1
                    UCHAR Timer2Disabled : 1;                                 //0x1
                    UCHAR Timer2ReservedFlags : 2;                            //0x1
                };
            };
            UCHAR Timer2ComponentId;                                        //0x2
            UCHAR Timer2RelativeId;                                         //0x3
        };
        struct {
            UCHAR QueueType;                                                //0x0
            union {
                UCHAR QueueControlFlags;                                    //0x1
                struct {
                    UCHAR Abandoned : 1;                                      //0x1
                    UCHAR DisableIncrement : 1;                               //0x1
                    UCHAR QueueReservedControlFlags : 6;                      //0x1
                };
            };
            UCHAR QueueSize;                                                //0x2
            UCHAR QueueReserved;                                            //0x3
        };
        struct {
            UCHAR ThreadType;                                               //0x0
            UCHAR ThreadReserved;                                           //0x1
            union {
                UCHAR ThreadControlFlags;                                   //0x2
                struct {
                    UCHAR CycleProfiling : 1;                                 //0x2
                    UCHAR CounterProfiling : 1;                               //0x2
                    UCHAR GroupScheduling : 1;                                //0x2
                    UCHAR AffinitySet : 1;                                    //0x2
                    UCHAR Tagged : 1;                                         //0x2
                    UCHAR EnergyProfiling : 1;                                //0x2
                    UCHAR SchedulerAssist : 1;                                //0x2
                    UCHAR ThreadReservedControlFlags : 1;                     //0x2
                };
            };
            union {
                UCHAR DebugActive;                                          //0x3
                struct {
                    UCHAR ActiveDR7 : 1;                                      //0x3
                    UCHAR Instrumented : 1;                                   //0x3
                    UCHAR Minimal : 1;                                        //0x3
                    UCHAR Reserved4 : 2;                                      //0x3
                    UCHAR AltSyscall : 1;                                     //0x3
                    UCHAR Emulation : 1;                                      //0x3
                    UCHAR Reserved5 : 1;                                      //0x3
                };
            };
        };
        struct {
            UCHAR MutantType;                                               //0x0
            UCHAR MutantSize;                                               //0x1
            UCHAR DpcActive;                                                //0x2
            UCHAR MutantReserved;                                           //0x3
        };
    };
    LONG SignalState;                                                       //0x4
    struct _LIST_ENTRY WaitListHead;                                        //0x8
};

struct _KAB_UM_PROCESS_CONTEXT {
    struct _KAB_UM_PROCESS_TREE *Trees;                                     //0x0
    ULONG TreeCount;                                                        //0x8
};

union _KEXECUTE_OPTIONS {
    UCHAR ExecuteDisable : 1;                                                 //0x0
    UCHAR ExecuteEnable : 1;                                                  //0x0
    UCHAR DisableThunkEmulation : 1;                                          //0x0
    UCHAR Permanent : 1;                                                      //0x0
    UCHAR ExecuteDispatchEnable : 1;                                          //0x0
    UCHAR ImageDispatchEnable : 1;                                            //0x0
    UCHAR DisableExceptionChainValidation : 1;                                //0x0
    UCHAR Spare : 1;                                                          //0x0
    volatile UCHAR ExecuteOptions;                                          //0x0
    UCHAR ExecuteOptionsNV;                                                 //0x0
};

struct _KGROUP_MASK {
    ULONGLONG Masks[2];                                                     //0x0
};

union _KSTACK_COUNT {
    LONG Value;                                                    //0x0
    ULONG State : 3;                                                 //0x0
    ULONG StackCount : 29;                                                    //0x0
};

// Can't find thread seed, 
struct _KPROCESS {
/**/struct _DISPATCHER_HEADER Header;                                       //0x0
    struct _LIST_ENTRY ProfileListHead;                                     //0x18
/**/ULONGLONG DirectoryTableBase;// is this process page directory base?    //0x28
/**/struct _LIST_ENTRY ThreadListHead;                                      //0x30
/**/ULONG ProcessLock;                                                      //0x40
    ULONG ProcessTimerDelay;                                                //0x44
    ULONGLONG DeepFreezeStartTime;                                          //0x48
/**/struct _KAFFINITY_EX *Affinity;                                         //0x50
    struct _KAB_UM_PROCESS_CONTEXT AutoBoostState;                          //0x58
    struct _LIST_ENTRY ReadyListHead;                                       //0x68
    struct _SINGLE_LIST_ENTRY SwapListEntry;                                //0x78
    struct _KAFFINITY_EX *ActiveProcessors;                                 //0x80
/**/union { // process flags?
        struct {
            ULONG AutoAlignment : 1;                                          //0x88
            ULONG DisableBoost : 1;                                           //0x88
            ULONG DisableQuantum : 1;                                         //0x88
            ULONG DeepFreeze : 1;                                             //0x88
            ULONG TimerVirtualization : 1;                                    //0x88
            ULONG CheckStackExtents : 1;                                      //0x88
            ULONG CacheIsolationEnabled : 1;                                  //0x88
            ULONG PpmPolicy : 4;                                              //0x88
            ULONG VaSpaceDeleted : 1;                                         //0x88
            ULONG MultiGroup : 1;                                             //0x88
            ULONG ForegroundProcess : 1;                                      //0x88
            ULONG ReservedFlags : 18;                                         //0x88
        };
        volatile LONG ProcessFlags;                                         //0x88
    };
    ULONG Spare0c;                                                          //0x8c
/**/CHAR BasePriority;                                                      //0x90
    CHAR QuantumReset;                                                      //0x91
    CHAR Visited;                                                           //0x92
/**/union _KEXECUTE_OPTIONS Flags;  // is this the flags I want instead?    //0x93
    struct _KGROUP_MASK ActiveGroupsMask;                                   //0x98
    ULONGLONG ActiveGroupPadding[2];                                        //0xa8
    struct _KI_IDEAL_PROCESSOR_ASSIGNMENT_BLOCK *IdealProcessorAssignmentBlock; //0xb8
    ULONGLONG Padding[8];                                                   //0xc0
    ULONG Spare0d;                                                          //0x100
/**/USHORT IdealGlobalNode;                                                 //0x104
    USHORT Spare1;                                                          //0x106
    union _KSTACK_COUNT StackCount;                                 //0x108
/**/struct _LIST_ENTRY ProcessListEntry;                                    //0x110
/**/ULONGLONG CycleTime;                                                    //0x120
/**/ULONGLONG ContextSwitches;                                              //0x128
    struct _KSCHEDULING_GROUP *SchedulingGroup;                             //0x130
/**/ULONGLONG KernelTime;                                                   //0x138
/**/ULONGLONG UserTime;                                                     //0x140
    ULONGLONG ReadyTime;                                                    //0x148
    ULONG FreezeCount;                                                      //0x150
    ULONG Spare4;                                                           //0x154
    ULONGLONG UserDirectoryTableBase;                                       //0x158
    UCHAR AddressPolicy;                                                    //0x160
    UCHAR Spare2[7];                                                        //0x161
/**/VOID *InstrumentationCallback;                                          //0x168
/**/union {
        ULONGLONG SecureHandle;                                             //0x170
        struct {
            ULONGLONG SecureProcess : 1;                                      //0x170
            ULONGLONG TrustedApp : 1;                                         //0x170
        } Flags;                                                            //0x170
    } SecureState;                                                          //0x170
    ULONGLONG KernelWaitTime;                                               //0x178
    ULONGLONG UserWaitTime;                                                 //0x180
    ULONGLONG LastRebalanceQpc;                                             //0x188
    VOID *PerProcessorCycleTimes;                                           //0x190
    ULONGLONG ExtendedFeatureDisableMask;                                   //0x198
    USHORT PrimaryGroup;                                                    //0x1a0
    USHORT Spare3[3];                                                       //0x1a2
    VOID *UserCetLogging;                                                   //0x1a8
    struct _LIST_ENTRY CpuPartitionList;                                    //0x1b0
    struct _KPROCESS_AVAILABLE_CPU_STATE *AvailableCpuState;                //0x1c0
};