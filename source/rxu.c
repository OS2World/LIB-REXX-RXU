#if defined(__HOS_OS2__) && defined(__TOS_OS2__) && defined(__OS2__)
  #define __BLD4OS2__
#endif  // building for OS/2

#if defined(_MSC_VER) && defined(_WIN32)
  #define __BLD4NT__
  #define _MSVC_
#endif
#if defined(__HOS_WIN__) && defined(__TOS_WIN__)
  #define __BLD4NT__
#endif  // building for WindowsNT

// Include files
#define  INCL_REXXSAA
#define  INCL_RXQUEUE
#if defined(__BLD4OS2__)
  #define  INCL_DOS
  #define  INCL_DOSPROCESS
  #define  INCL_ERRORS
  #define  INCL_WIN
  #define  INCL_WINSWITCHLIST
  #define  INCL_VIO
  #define  INCL_KBD
  #include <os2.h>
  #include <rexxsaa.h>
  #include <rexxque.h>

  #define PURE_32
  #include <upm.h>
  // undefine macros used by UPM
  #undef  LSFAR
  #undef  LSPAS
  #undef  LSPTR
  #undef  LSINT
  #undef  FARPASCAL

  #define EXPORTSPEC
#endif  // BLD4OS2

#if defined(__BLD4NT__)
  #include <windows.h>
  #include <process.h>
  #if defined(__W32OREXX__)
    #include <rexx.h>
  #else
    #include <wrexx32.h>
  #endif
  #include <sqlenv.h>
  #define HMTX         HANDLE
  #define HEV          HANDLE
  //#define PFN          LPTHREAD_START_ROUTINE
  //#define PFN          FARPROC
  #define _Optlink
  #define _System
  typedef PVOID*       PPVOID;
  //#define PSZ          LPCSTR
  //#define PSZ          PUCHAR
  #define EXPORTSPEC   __declspec(dllexport)
  #define TID          ULONG
  #define PID          ULONG
  #define DATETIME     SYSTEMTIME
#endif  // BLD4NT

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#if defined(__BLD4OS2__)
  #define INCL_32
  #include <sqlenv.h>
  #define  E32TO16
  #include <lan_7_c.h>
  #include <netb_1_c.h>
  #include <netb_2_c.h>
  #include <netb_4_c.h>
  #include <netcons.h>
  #include <access.h>
  #include <use.h>
  #include <shares.h>
#endif  // BLD4OS2

#if defined(__BLD4OS2__)
  #define _THIS_DLL_NAME_               "RXU"
#endif  // BLD4OS2
#if defined(__BLD4NT__)
  #define _THIS_DLL_NAME_               "RXUW32"
#endif  // BLD4NT
#define _VERSION_INFO_LIT_            "RXU, v1.b, 25 October, 1996:"
#define _REXX_VARIABLE_NAME_MAX_LEN_  250

// GLOBAL STATIC variables
#if defined(__BLD4OS2__)
HMTX   hmtxRxGlobalVar=(HMTX)0;
UCHAR  RxGlobalVarSemName[] = "\\SEM32\\RXU\\RXGLOBALVAR";
#endif  // BLD4OS2
#if defined(__BLD4NT__)
HANDLE   hmtxRxGlobalVar=(HANDLE)0;
UCHAR  RxGlobalVarSemName[] = "SEM32RXURXGLOBALVAR";
#endif  // BLD4OS2

#if defined(__BLD4OS2__)
  ERR    pExceptionExit=(ERR)NULL;
  _ERR   callFunctionExcpHandler;
#endif  // BLD4OS2

#define XCPT_CALL_FUNCTION_EXCPNUMB 0xe0042781
#define XCPT_CALL_FUNCTION_EXCPNUMA 0xe0042782
typedef void ( _System _CALLFUNCADDRESS)(void);
typedef _CALLFUNCADDRESS    *PCALLFUNCADDRESS;
typedef struct _funcCallTypeInfo
  {
  ULONG   argtype;
  ULONG   arglen;
  BOOL    argbyref;
  } FUNCCALLTYPEINFO, *PFUNCCALLTYPEINFO;
typedef struct _funcCallInfo
  {
  PVOID            pfunc;
  ULONG            origesp;
  FUNCCALLTYPEINFO ret;
  ULONG            uretcode;
  long             retcode;
  ULONG            linkage;
  ULONG            numfuncargs;
  ULONG            stackforargs;
  FUNCCALLTYPEINFO arg[1];
  } FUNCCALLINFO, *PFUNCCALLINFO;

ULONG  ulPassByNameBufSize=0x8000;  // 32k by default

// RxPassByName->initExitHandler argument structs
typedef struct _varnamelist
  {
  char                * pvar;
  struct _varnamelist * pnextvar;
  BOOL                  byref;
  } VARNAMELIST, *PVARNAMELIST;
typedef struct _initexithandlerargs
  {
  PVARNAMELIST                  pvars;
  BOOL                          somevarbyref;
  PVARNAMELIST                  pprefs;
  BOOL                          someprefbyref;
  PSHVBLOCK                     pbase;
  struct _initexithandlerargs * pnextarg;
  } INITEXITHANDLERARGS, *PINITEXITHANDLERARGS;
typedef struct _RexxThreadCallArgs
  {
  unsigned long	numargs;
  RXSTRING		args[1];
  } RTCARGS, *PRTCARGS;
typedef struct _passByNameThreadArgs
  {
  PRTCARGS				ptargs;
  PINITEXITHANDLERARGS	pehargs;
  } PBNTTARGS, *PPBNTTARGS;
long passByNameInvokeInterpreter(ULONG,PRXSTRING,PRXSTRING,PINITEXITHANDLERARGS,BOOL);
void __cdecl passByNameToThread(void*);
void extractVariableSubPool(PINITEXITHANDLERARGS,BOOL);
BOOL getEhargsTail(PINITEXITHANDLERARGS*,PRXSYSEXIT*,ULONG*);
void addToListOfPassedVariables(PINITEXITHANDLERARGS,PCH,BOOL);
BOOL getDataType(PFUNCCALLTYPEINFO,char*);
BOOL getLinkageType(ULONG*,char*);

// RXRSOE2Q definitions
static UCHAR _RXRSOE2Q_END_OF_FILE_MARKER_[] = ";RxRsOe2q;EndOfFile;RxRsOe2q;\x1a";
typedef struct _rxrsoe2qargs
  {
  HFILE hread;
  HFILE hwrite;
  PSZ   pqname;
  HEV   hevDoneWithPipe;
  } RXRSOE2QARGS, * PRXRSOE2QARGS;

#if defined(__BLD4OS2__)
// DosQProcStatus definitions
#define   CONVERT(fp) (*((unsigned far* &(fp) + 1) = QSsel)
#define   QPROCSTATBUFFSIZE 0xffff
typedef struct qsS16Headrec_s
  {
  ULONG            SRecType;    // semaphore rectype = 3
  ULONG            Reserved1;   // overlays NextRec of 1st qsS16rec_t
  ULONG            Reserved2;
  ULONG            S16TblOff;   // index of first semaphore, SEE PSTAT OUTPUT
  }qsS16Headrec_t, *PqsS16Headrec_t;
#pragma pack(1)
typedef struct qsS16rec_s
  {
  struct qsS16rec_s *pNextRec;      // offset to next record in buffer
  //UINT             s_SysSemOwner;   // thread owning this semaphore
  //UCHAR            s_SysSemFlag;    // system semaphore flag bit field
  //UCHAR            s_SysSemRefCnt;  // number of references to this system sem
  //UCHAR            s_SysSemProcCnt; // number of requests by sem owner
  //UCHAR            Reserved1;
  //ULONG            Reserved2;
  UCHAR            s_SysSemProcCnt; // number of requests by sem owner
  UCHAR            s_SysSemFlag[2]; // system semaphore flag bit field
  UCHAR            s_SysSemRefCnt;  // number of references to this system sem
  ULONG            reserved1;
  USHORT           semindex;
  USHORT           reserved2;
  CHAR             SemName[1];  // start of semaphore name string
  }qsS16rec_t, *PqsS16rec_t;
#pragma pack()
typedef struct qsLrec_s
  {
  struct qsLrec_s *pNextRec;    // ptr to next record in buffer
  USHORT           hmte;        // handle for this mte
  USHORT           Reserved1;
  ULONG            ctImpMod;    // number of imported modules in table
  ULONG            Reserved2;
  //qsLObjrec_t FAR *Reserved3;
  PVOID            pReserved3;
  PSZ              pName;       // ptr to name string following struct
  USHORT           imphmte[1];  // array of handles of imported modules
  }qsLrec_t, *PqsLrec_t;
typedef struct qsMrec_s
  {
  struct qsMrec_s *pNextRec;    // ptr to next record in buffer
  USHORT           hmem;        // handle for shared memory
  USHORT           sel;         // shared memory selector
  USHORT           refcnt;      // reference count
  CHAR             Memname[1];  // start of shared memory name string
  }qsMrec_t, *PqsMrec_t;
typedef struct qsGrec_s
  {
  ULONG            cThrds;
  ULONG            Reserved1;
  ULONG            Reserved2;
  }qsGrec_t, *PqsGrec_t;
typedef struct qsTrec_s
  {
  ULONG            RecType;     // type of record (thread rectype = 100)
  USHORT           tid;         // thread-id
  USHORT           slot;        // unique thread slot number
  ULONG            sleepid;     // sleep-id thread is sleeping on
  ULONG            priority;    // thread priority
  ULONG            systime;     // thread system time
  ULONG            usertime;    // thread user time
  UCHAR            state;       // thread state
                                /*
                                   1  - Ready to run
                                   2  - Blocked
                                   3  - ?
                                   4  - waiting on another thread
                                        which is in a critical section
                                   5  - Running
                                */
  CHAR             Reserved1;
  SHORT            Reserved2;
  }qsTrec_t, *PqsTrec_t;
typedef struct qsPrec_s
  {
  ULONG            RecType;     // type of record (process rectype = 1)
  PqsTrec_t        pThrdRec;    // ptr to first thread rec for this process
  USHORT           pid;         // process-id
  USHORT           ppid;        // parent process-id
  ULONG            type;        // process-type
  ULONG            stat;        // process-status
                                /*
                                   0x01 - Doing ExitList Processing
                                   0x02 - Exiting thread 1
                                   0x04 - The whole process is exiting
                                   0x10 - Parent cares about termination
                                   0x20 - Parent did an exec-and-wait
                                   0x40 - Process is dying
                                   0x80 - Process in embryonic state
                                */
  ULONG            sgid;        // screen group
  USHORT           hMte;        // program module handle
  USHORT           cTCB;        // number of TCBs in use in process
  ULONG            Reserved1;
  PVOID            pReserved2;
  USHORT           c16Sem;      // number of 16-bit system sems in use by proces
  USHORT           cLib;        // number of runtime linked libraries
  USHORT           cShrMem;     // number of shared memory handles
  USHORT           Reserved3;
  PUSHORT          p16SemRec;   // ptr to head of 16-bit sem info for this proces
  PUSHORT          pLibRec;     // ptr to list of runtime libraries in use
  PUSHORT          pShrMemRec;  // ptr to list of shared mem handles
  PUSHORT          pReserved4;
  }qsPrec_t, *PqsPrec_t;
typedef struct qsPtrRec_s
  {
  PqsGrec_t        pGlobalRec;  // ptr to global data section
  PqsPrec_t        pProcRec;    // ptr to process record section
  PqsS16Headrec_t  p16SemRec;   // ptr to 16 bit sem section
  PVOID            Reserved1;   // ptr to god-knows-where
  PqsMrec_t        pShrMemRec;  // ptr to shared memory section
  PqsLrec_t        pLibRec;     // ptr to exe module record section
  } qsPtrRec_t, *PqsPtrRec_t;

APIRET16  APIENTRY16 DosQProcStatus(PULONG,USHORT);
APIRET    APIENTRY DosReplaceModule(PSZ,PSZ,PSZ);
PSZ       GetModName(USHORT,PqsLrec_t,USHORT);

// Exception handlers
#pragma handler(RexxFunctionCall)
#pragma handler(pipeToQueue)
#pragma map(_Exception,"__SecondaryThreadHandler")

#endif  // BLD4OS2

#define  INVALID_ROUTINE 40            // Raise Rexx error
#define  VALID_ROUTINE    0            // Successful completion
#define  PROCESS_LOGON    1
#define  USER_LOGON       2
#define  PROCESS_LOGOFF   3
#define  USER_LOGOFF      4
#define  GET_LOCAL_LOGON  5
#define  LOGON_USER_LOCAL 6
#define  GET_LOGON_LIST   7
#define  USER_PW_SET      8
#define  USER_ADD         9
#define  USER_DEL         10
#define  USER_VAL         11
#define  USER_ENUM        12
#define  LOGON_ENUM       13
#define  FILE_ENUM        14
#define  USE_ADD          15
#define  USE_DEL          16
#define  USE_ENUM         17
#define  USER_GETINFO     18
#define  NUM_UINFO_BLOCKS 100
#define PRINTFQNAME   "\\QUEUES\\PRINTF32"
#define QUEUEPREFIX   "\\QUEUES\\"
#define STRUCT_ELEM_TYPE_LONG    1
#define STRUCT_ELEM_TYPE_ULONG   2
#define STRUCT_ELEM_TYPE_SHORT   3
#define STRUCT_ELEM_TYPE_USHORT  4
#define STRUCT_ELEM_TYPE_DOUBLE  5
#define STRUCT_ELEM_TYPE_CHAR    6
#define ARG_TYPE_LONG    1
#define ARG_TYPE_ULONG   2
#define ARG_TYPE_SHORT   3
#define ARG_TYPE_USHORT  4
#define ARG_TYPE_CHAR    5
#define ARG_TYPE_UCHAR   6
#define ARG_TYPE_FLOAT   7
#define ARG_TYPE_DOUBLE  8
#define ARG_TYPE_STRUCT  9
#define ARG_TYPE_PVOID   10
#define LINK_TYPE_SYSTEM  1
#define LINK_TYPE_OPTLINK 2
#define LINK_TYPE_CDECL   3

#if defined(__BLD4OS2__)

// Function Pointer prototypes for 16-bit generic function calls
typedef APIRET16 ( _Far16 _Pascal _GENERIC16ARG0)(void);
typedef _GENERIC16ARG0    *PGENERIC16ARG0;
typedef APIRET16 ( _Far16 _Pascal _GENERIC16ARG1)(PVOID _Seg16);
typedef _GENERIC16ARG1    *PGENERIC16ARG1;
typedef APIRET16 ( _Far16 _Pascal _GENERIC16ARG2)(PVOID _Seg16,PVOID _Seg16);
typedef _GENERIC16ARG2    *PGENERIC16ARG2;
typedef APIRET16 ( _Far16 _Pascal _GENERIC16ARG3)(PVOID _Seg16,PVOID _Seg16,PVOID _Seg16);
typedef _GENERIC16ARG3    *PGENERIC16ARG3;
typedef APIRET16 ( _Far16 _Pascal _GENERIC16ARG4)(PVOID _Seg16,PVOID _Seg16,PVOID _Seg16,PVOID _Seg16);
typedef _GENERIC16ARG4    *PGENERIC16ARG4;
typedef APIRET16 ( _Far16 _Pascal _GENERIC16ARG5)(PVOID _Seg16,PVOID _Seg16,PVOID _Seg16,PVOID _Seg16,PVOID _Seg16);
typedef _GENERIC16ARG5    *PGENERIC16ARG5;

// Function Pointer prototypes for 32-bit _System generic function calls
typedef APIRET ( APIENTRY _GENERIC32SYSARG0)(void);
typedef _GENERIC32SYSARG0    *PGENERIC32SYSARG0;
typedef APIRET ( APIENTRY _GENERIC32SYSARG1)(PVOID);
typedef _GENERIC32SYSARG1    *PGENERIC32SYSARG1;
typedef APIRET ( APIENTRY _GENERIC32SYSARG2)(PVOID,PVOID);
typedef _GENERIC32SYSARG2    *PGENERIC32SYSARG2;
typedef APIRET ( APIENTRY _GENERIC32SYSARG3)(PVOID,PVOID,PVOID);
typedef _GENERIC32SYSARG3    *PGENERIC32SYSARG3;
typedef APIRET ( APIENTRY _GENERIC32SYSARG4)(PVOID,PVOID,PVOID,PVOID);
typedef _GENERIC32SYSARG4    *PGENERIC32SYSARG4;
typedef APIRET ( APIENTRY _GENERIC32SYSARG5)(PVOID,PVOID,PVOID,PVOID,PVOID);
typedef _GENERIC32SYSARG5    *PGENERIC32SYSARG5;
typedef APIRET ( APIENTRY _GENERIC32SYSARG6)(PVOID,PVOID,PVOID,PVOID,PVOID,PVOID);
typedef _GENERIC32SYSARG6    *PGENERIC32SYSARG6;
typedef APIRET ( APIENTRY _GENERIC32SYSARG7)(PVOID,PVOID,PVOID,PVOID,PVOID,PVOID,PVOID);
typedef _GENERIC32SYSARG7    *PGENERIC32SYSARG7;
typedef APIRET ( APIENTRY _GENERIC32SYSARG8)(PVOID,PVOID,PVOID,PVOID,PVOID,PVOID,PVOID,PVOID);
typedef _GENERIC32SYSARG8    *PGENERIC32SYSARG8;
typedef APIRET ( APIENTRY _GENERIC32SYSARG9)(PVOID,PVOID,PVOID,PVOID,PVOID,PVOID,PVOID,PVOID,PVOID);
typedef _GENERIC32SYSARG9    *PGENERIC32SYSARG9;
typedef APIRET ( APIENTRY _GENERIC32SYSARG10)(PVOID,PVOID,PVOID,PVOID,PVOID,PVOID,PVOID,PVOID,PVOID,PVOID);
typedef _GENERIC32SYSARG10   *PGENERIC32SYSARG10;

// Function Pointer prototypes for "UPM" APIs
typedef APIRET ( _Far16 _Pascal _UPMGLGNFN)(USHORT,USHORT,USHORT,USHORT,USHORT,PUCHAR,PUCHAR,PUCHAR);
typedef _UPMGLGNFN    *PUPMGLGNFN;
typedef APIRET ( _Far16 _Pascal _UPMGULGNFN)(USHORT,USHORT,USHORT,USHORT,USHORT,PUCHAR,PUCHAR,PUCHAR);
typedef _UPMGULGNFN    *PUPMGULGNFN;
typedef APIRET ( _Far16 _Pascal _UPMGLGFFFN)(USHORT,USHORT,USHORT,PUCHAR,PUCHAR);
typedef _UPMGLGFFFN    *PUPMGLGFFFN;
typedef APIRET ( _Far16 _Pascal _UPMGULGFFN)(USHORT,USHORT,USHORT,PUCHAR,PUCHAR);
typedef _UPMGULGFFN    *PUPMGULGFFN;
typedef APIRET ( _Far16 _Pascal _UPMGLOCUFN)(PUSHORT,PUCHAR,PUSHORT);
typedef _UPMGLOCUFN    *PUPMGLOCUFN;
typedef APIRET ( _Far16 _Pascal _UPMGLOCLFN)(PUSHORT,PUCHAR,PUSHORT);
typedef _UPMGLOCLFN    *PUPMGLOCLFN;
typedef APIRET ( _Far16 _Pascal _UPMGUSRLFN)(USHORT,USHORT,USHORT,PUSHORT,PUSHORT,PUCHAR,PUCHAR);
typedef _UPMGUSRLFN    *PUPMGUSRLFN;

// Function Pointer prototypes for "SQL" APIs
typedef APIRET ( _Far16 _Pascal _SQLGDOPSFN)(UCHAR,PUSHORT,PUSHORT,struct sqlca *);
typedef _SQLGDOPSFN    *PSQLGDOPSFN;
typedef APIRET ( _Far16 _Pascal _SQLGDGNEFN)(USHORT,struct sqledinfo * _Seg16 *,struct sqlca *);
typedef _SQLGDGNEFN    *PSQLGDGNEFN;
typedef APIRET ( _Far16 _Pascal _SQLGDCLSFN)(USHORT,struct sqlca *);
typedef _SQLGDCLSFN    *PSQLGDCLSFN;

// Function Pointer prototypes for "Net" APIs
typedef APIRET ( _Far16 _Pascal _NETUSEADDFN)(char *,short,char *,USHORT);
typedef _NETUSEADDFN    *PNETUSEADDFN;
typedef APIRET ( _Far16 _Pascal _NETUSEDELFN)(char *,char *,USHORT);
typedef _NETUSEDELFN    *PNETUSEDELFN;
typedef APIRET ( _Far16 _Pascal _NETUSEENUMFN)(char *,short,char *,USHORT,PUSHORT,PUSHORT);
typedef _NETUSEENUMFN    *PNETUSEENUMFN;
typedef APIRET ( _Far16 _Pascal _NETUSEGETINFOFN)(char *,short,char *,USHORT);
typedef _NETUSEGETINFOFN    *PNETUSEGETINFOFN;
typedef APIRET ( _Far16 _Pascal _NETUSERADDFN)(char *,short,char *,USHORT);
typedef _NETUSERADDFN    *PNETUSERADDFN;
typedef APIRET ( _Far16 _Pascal _NETUSERPWSETFN)(char *,char *,char *,char *);
typedef _NETUSERPWSETFN  *PNETUSERPWSETFN;
typedef APIRET ( _Far16 _Pascal _NETUSERDELFN)(char *,char *);
typedef _NETUSERDELFN    *PNETUSERDELFN;
typedef APIRET ( _Far16 _Pascal _NETUSERVAL2FN)(char *,short,char *,USHORT,USHORT,PUSHORT);
typedef _NETUSERVAL2FN   *PNETUSERVAL2FN;
typedef APIRET ( _Far16 _Pascal _NETUSERENUMFN)(char *,short,char *,USHORT,PUSHORT,PUSHORT);
typedef _NETUSERENUMFN   *PNETUSERENUMFN;
typedef APIRET ( _Far16 _Pascal _NETUSERGETINFOFN)(char *,char *,short,char *,USHORT,PUSHORT);
typedef _NETUSERGETINFOFN *PNETUSERGETINFOFN;
typedef APIRET ( _Far16 _Pascal _NETLOGONENUMFN)(char *,short,char *,USHORT,PUSHORT,PUSHORT);
typedef _NETLOGONENUMFN  *PNETLOGONENUMFN;
typedef APIRET ( _Far16 _Pascal _NETFILEENUM2FN)(char *,char *,char *,short,char *,USHORT,PUSHORT,PUSHORT,void *);
typedef _NETFILEENUM2FN  *PNETFILEENUM2FN;

// Function Pointer prototypes for "NETBIOS" APIs
typedef APIRET ( _Far16 _Pascal _NETBIOSFN)(char *);
typedef _NETBIOSFN    *PNETBIOSFN;

#endif  // BLD4OS2

// Structure declaration
typedef struct _structMapPrefix
  {
  ULONG      beginbraket;
  ULONG      numel;
  ULONG      packing;
  } STRUCTMAPPREFIX, * PSTRUCTMAPPREFIX;
typedef struct _structMapElem
  {
  ULONG      elemtype;
  ULONG      elemoffset;
  ULONG      elemlen;
  } STRUCTMAPELEM, * PSTRUCTMAPELEM;
typedef struct _structMapSuffix
  {
  ULONG      totlen;
  ULONG      endbraket;
  } STRUCTMAPSUFFIX, * PSTRUCTMAPSUFFIX;

// Function Prototypes
//BOOL         string2long(ULONG, PCH, LONG *);
ULONG        loadAPI(PSZ dllname, PSZ procname, ULONG ordinal, PFN* pprocaddr);
#if defined(__BLD4OS2__)
  BOOL         searchSwitchList4PID(SWCNTRL * pswctl,HAB hab,ULONG sessid);
#endif  // BLD4OS2
void _Optlink CdeclSetExceptionExit(PPVOID);
void _Optlink RexxFunctionCall(void *);
void _Optlink pipeToQueue(void *);
int  _System pfnProcAddr(PVOID);
int  RexxQueueAddElement(PSZ, PRXSTRING, BOOL);

// Rexx general function prototypes

// UPM specific function prototypes
#if defined(__BLD4OS2__)
EXPORTSPEC RexxFunctionHandler  RxUpm_u32elgn;
EXPORTSPEC RexxFunctionHandler  RxUpm_u32elgff;
EXPORTSPEC RexxFunctionHandler  RxUpm_u32elocu;
EXPORTSPEC RexxFunctionHandler  RxUpm_u32elocl;
EXPORTSPEC RexxFunctionHandler  RxUpm_u32eulgn;
EXPORTSPEC RexxFunctionHandler  RxUpm_u32eulgf;
EXPORTSPEC RexxFunctionHandler  RxUpm_u32eusrl;
EXPORTSPEC RexxFunctionHandler  RxUpm_u32esetp;
EXPORTSPEC RexxFunctionHandler  RxUpm_u32egetp;
EXPORTSPEC RexxFunctionHandler  RxUpm;
#endif
EXPORTSPEC RexxFunctionHandler  RxProcId;
EXPORTSPEC RexxFunctionHandler  RxVlist;
#if defined(__BLD4OS2__)
EXPORTSPEC RexxFunctionHandler  RxStorage;
#endif
EXPORTSPEC RexxFunctionHandler  RxScount;
#if defined(__BLD4OS2__)
EXPORTSPEC RexxFunctionHandler  RxRsoe2f;
EXPORTSPEC RexxFunctionHandler  RxRsoe2q;
EXPORTSPEC RexxFunctionHandler  RxKillProcess;
EXPORTSPEC RexxFunctionHandler  RxCreateNPipe;
EXPORTSPEC RexxFunctionHandler  RxSetNPHState;
#endif
EXPORTSPEC RexxFunctionHandler  RxCreateRexxThread;
#if defined(__BLD4OS2__)
EXPORTSPEC RexxFunctionHandler  RxSetExceptionExit;
EXPORTSPEC RexxFunctionHandler  RxCreateThread;
EXPORTSPEC RexxFunctionHandler  RxConnectNPipe;
EXPORTSPEC RexxFunctionHandler  RxDisConnectNPipe;
EXPORTSPEC RexxFunctionHandler  RxNbSessionStatus;
EXPORTSPEC RexxFunctionHandler  RxCreatePipe;
EXPORTSPEC RexxFunctionHandler  RxSoSe2H;
EXPORTSPEC RexxFunctionHandler  RxSi2H;
EXPORTSPEC RexxFunctionHandler  RxRSi2F;
EXPORTSPEC RexxFunctionHandler  RxLineInH;
EXPORTSPEC RexxFunctionHandler  RxAllocMem;
EXPORTSPEC RexxFunctionHandler  RxFreeMem;
EXPORTSPEC RexxFunctionHandler  RxDestroyPipe;
EXPORTSPEC RexxFunctionHandler  RxAllocSharedMem;
EXPORTSPEC RexxFunctionHandler  RxGetSharedMem;
EXPORTSPEC RexxFunctionHandler  RxGiveSharedMem;
EXPORTSPEC RexxFunctionHandler  RxGetNamedSharedMem;
EXPORTSPEC RexxFunctionHandler  RxCreateEventSem;
EXPORTSPEC RexxFunctionHandler  RxCloseEventSem;
EXPORTSPEC RexxFunctionHandler  RxOpenEventSem;
EXPORTSPEC RexxFunctionHandler  RxPostEventSem;
EXPORTSPEC RexxFunctionHandler  RxQueryEventSem;
EXPORTSPEC RexxFunctionHandler  RxResetEventSem;
EXPORTSPEC RexxFunctionHandler  RxWaitEventSem;
#endif
EXPORTSPEC RexxFunctionHandler  RxuTerm;
EXPORTSPEC RexxFunctionHandler  RxuInit;
EXPORTSPEC RexxFunctionHandler  RxuQuery;
#if defined(__BLD4OS2__)
EXPORTSPEC RexxFunctionHandler  RxLoadModule;
EXPORTSPEC RexxFunctionHandler  RxFreeModule;
EXPORTSPEC RexxFunctionHandler  RxQueryModuleName;
EXPORTSPEC RexxFunctionHandler  RxQueryModuleHandle;
EXPORTSPEC RexxFunctionHandler  RxQueryProcType;
EXPORTSPEC RexxFunctionHandler  RxQueryProcAddr;
EXPORTSPEC RexxFunctionHandler  RxCallEntryPoint;
EXPORTSPEC RexxFunctionHandler  RxCallProcAddr;
EXPORTSPEC RexxFunctionHandler  RxAddMacro;
EXPORTSPEC RexxFunctionHandler  RxDropMacro;
EXPORTSPEC RexxFunctionHandler  RxClearMacroSpace;
EXPORTSPEC RexxFunctionHandler  RxSaveMacroSpace;
EXPORTSPEC RexxFunctionHandler  RxLoadMacroSpace;
EXPORTSPEC RexxFunctionHandler  RxQueryMacro;
EXPORTSPEC RexxFunctionHandler  RxReorderMacro;
EXPORTSPEC RexxFunctionHandler  RxSetMem;
EXPORTSPEC RexxFunctionHandler  RxQueryMem;
EXPORTSPEC RexxFunctionHandler  RxSubAllocMem;
EXPORTSPEC RexxFunctionHandler  RxSubFreeMem;
EXPORTSPEC RexxFunctionHandler  RxSubSetMem;
EXPORTSPEC RexxFunctionHandler  RxSubUnsetMem;
EXPORTSPEC RexxFunctionHandler  RxOpen;
EXPORTSPEC RexxFunctionHandler  RxRead;
EXPORTSPEC RexxFunctionHandler  RxWrite;
EXPORTSPEC RexxFunctionHandler  RxDosRead;
EXPORTSPEC RexxFunctionHandler  RxDosWrite;
EXPORTSPEC RexxFunctionHandler  RxCloseH;
EXPORTSPEC RexxFunctionHandler  RxQuerySysInfo;
EXPORTSPEC RexxFunctionHandler  RxTmrQueryFreq;
EXPORTSPEC RexxFunctionHandler  RxTmrQueryTime;
EXPORTSPEC RexxFunctionHandler  RxDevConfig;
EXPORTSPEC RexxFunctionHandler  RxDevIOCtl;
EXPORTSPEC RexxFunctionHandler  RxPhysicalDisk;
EXPORTSPEC RexxFunctionHandler  RxExecPgm;
EXPORTSPEC RexxFunctionHandler  RxWaitChild;
EXPORTSPEC RexxFunctionHandler  RxKillThread;
EXPORTSPEC RexxFunctionHandler  RxResumeThread;
EXPORTSPEC RexxFunctionHandler  RxSuspendThread;
EXPORTSPEC RexxFunctionHandler  RxStartSession;
EXPORTSPEC RexxFunctionHandler  RxStartRexxSession;
EXPORTSPEC RexxFunctionHandler  RxDetachRexxPgm;
EXPORTSPEC RexxFunctionHandler  RxWinQueryObject;
EXPORTSPEC RexxFunctionHandler  RxWinSetPresParam;
EXPORTSPEC RexxFunctionHandler  RxWinDestroyObject;
EXPORTSPEC RexxFunctionHandler  RxCreateMutexSem;
EXPORTSPEC RexxFunctionHandler  RxOpenMutexSem;
EXPORTSPEC RexxFunctionHandler  RxCloseMutexSem;
EXPORTSPEC RexxFunctionHandler  RxQueryMutexSem;
EXPORTSPEC RexxFunctionHandler  RxReleaseMutexSem;
EXPORTSPEC RexxFunctionHandler  RxRequestMutexSem;
EXPORTSPEC RexxFunctionHandler  RxAddMuxWaitSem;
EXPORTSPEC RexxFunctionHandler  RxDeleteMuxWaitSem;
EXPORTSPEC RexxFunctionHandler  RxCloseMuxWaitSem;
EXPORTSPEC RexxFunctionHandler  RxCreateMuxWaitSem;
EXPORTSPEC RexxFunctionHandler  RxOpenMuxWaitSem;
EXPORTSPEC RexxFunctionHandler  RxQueryMuxWaitSem;
EXPORTSPEC RexxFunctionHandler  RxWaitMuxWaitSem;
EXPORTSPEC RexxFunctionHandler  RxNet;
EXPORTSPEC RexxFunctionHandler  RxPullQueue;
EXPORTSPEC RexxFunctionHandler  RxAddQueue;
EXPORTSPEC RexxFunctionHandler  RxQueued;
EXPORTSPEC RexxFunctionHandler  RxPmPrintf;
#endif
EXPORTSPEC RexxFunctionHandler  RxCallInStore;
#if defined(__BLD4OS2__)
EXPORTSPEC RexxFunctionHandler  RxTokenize;
EXPORTSPEC RexxFunctionHandler  RxQProcStatus;
EXPORTSPEC RexxFunctionHandler  RxSetPriority;
EXPORTSPEC RexxFunctionHandler  RxExecI;
EXPORTSPEC RexxFunctionHandler  RxExecO;
EXPORTSPEC RexxFunctionHandler  RxQExists;
EXPORTSPEC RexxFunctionHandler  RxAdd2Ptr;
EXPORTSPEC RexxFunctionHandler  RxThunkAddr;
EXPORTSPEC RexxFunctionHandler  RxOpenQueue;
EXPORTSPEC RexxFunctionHandler  RxPeekQueue;
EXPORTSPEC RexxFunctionHandler  RxCloseQueue;
EXPORTSPEC RexxFunctionHandler  RxCreateQueue;
EXPORTSPEC RexxFunctionHandler  RxReadQueue;
EXPORTSPEC RexxFunctionHandler  RxReadQueueStr;
EXPORTSPEC RexxFunctionHandler  RxWriteQueue;
EXPORTSPEC RexxFunctionHandler  RxPurgeQueue;
EXPORTSPEC RexxFunctionHandler  RxQueryQueue;
EXPORTSPEC RexxFunctionHandler  RxSetError;
EXPORTSPEC RexxFunctionHandler  RxSearchPath;
EXPORTSPEC RexxFunctionHandler  RxReplaceModule;
EXPORTSPEC RexxFunctionHandler  RxExitList;
EXPORTSPEC RexxFunctionHandler  RxStructMap;
EXPORTSPEC RexxFunctionHandler  RxStruct2Stem;
EXPORTSPEC RexxFunctionHandler  RxStem2Struct;
EXPORTSPEC RexxFunctionHandler  RxGetInfoBlocks;
EXPORTSPEC RexxFunctionHandler  RxQueryAppType;
EXPORTSPEC RexxFunctionHandler  RxVioPopUp;
EXPORTSPEC RexxFunctionHandler  RxVioEndPopUp;
EXPORTSPEC RexxFunctionHandler  RxVioWrtCharStrAtt;
EXPORTSPEC RexxFunctionHandler  RxKbdCharIn;
EXPORTSPEC RexxFunctionHandler  RxDupHandle;
EXPORTSPEC RexxFunctionHandler  RxSetMaxFH;
EXPORTSPEC RexxFunctionHandler  RxSetRelMaxFH;
EXPORTSPEC RexxFunctionHandler  RxSetFHState;
EXPORTSPEC RexxFunctionHandler  RxQueryFHState;
#endif
EXPORTSPEC RexxFunctionHandler  RxRegisterFunctionExe;
EXPORTSPEC RexxFunctionHandler  RxRegisterExitDll;
EXPORTSPEC RexxFunctionHandler  RxRegisterExitExe;
EXPORTSPEC RexxFunctionHandler  RxQueryExit;
EXPORTSPEC RexxFunctionHandler  RxDeregisterExit;
#if defined(__BLD4OS2__)
EXPORTSPEC RexxFunctionHandler  RxWinSetSelf;
EXPORTSPEC RexxFunctionHandler  RxGlobalVar;
#endif
EXPORTSPEC RexxFunctionHandler  RxPassByName;
EXPORTSPEC RexxFunctionHandler  RxReturnByName;
EXPORTSPEC RexxFunctionHandler  RxPBNBufSize;
#if defined(__BLD4OS2__)
EXPORTSPEC RexxFunctionHandler  RxSetExtLibPath;
EXPORTSPEC RexxFunctionHandler  RxQueryExtLibPath;
EXPORTSPEC RexxFunctionHandler  RxRegisterFuncAddress;
EXPORTSPEC RexxFunctionHandler  RxCallFuncAddress;
#endif
EXPORTSPEC RexxFunctionHandler  RxMalloc;
EXPORTSPEC RexxFunctionHandler  RxFree;
EXPORTSPEC RexxFunctionHandler  RxF2C;
EXPORTSPEC RexxFunctionHandler  RxC2F;

// Math library passthrough function prototypes
EXPORTSPEC RexxFunctionHandler  RxuMthceil;
EXPORTSPEC RexxFunctionHandler  RxuMthcosh;
EXPORTSPEC RexxFunctionHandler  RxuMthexp;
EXPORTSPEC RexxFunctionHandler  RxuMthfloor;
EXPORTSPEC RexxFunctionHandler  RxuMthfmod;
EXPORTSPEC RexxFunctionHandler  RxuMthfrexp;
EXPORTSPEC RexxFunctionHandler  RxuMthldexp;
EXPORTSPEC RexxFunctionHandler  RxuMthlog;
EXPORTSPEC RexxFunctionHandler  RxuMthlog10;
EXPORTSPEC RexxFunctionHandler  RxuMthmodf;
EXPORTSPEC RexxFunctionHandler  RxuMthpow;
EXPORTSPEC RexxFunctionHandler  RxuMthsinh;
EXPORTSPEC RexxFunctionHandler  RxuMthtanh;
EXPORTSPEC RexxFunctionHandler  RxuMthasin;
EXPORTSPEC RexxFunctionHandler  RxuMthacos;
EXPORTSPEC RexxFunctionHandler  RxuMthatan2;
EXPORTSPEC RexxFunctionHandler  RxuMthatan;
EXPORTSPEC RexxFunctionHandler  RxuMthsin;
EXPORTSPEC RexxFunctionHandler  RxuMthcos;
EXPORTSPEC RexxFunctionHandler  RxuMthtan;
EXPORTSPEC RexxFunctionHandler  RxuMthsqrt;
EXPORTSPEC RexxFunctionHandler  RxuMthfabs;
EXPORTSPEC RexxFunctionHandler  RxuMtherf;
EXPORTSPEC RexxFunctionHandler  RxuMtherfc;
EXPORTSPEC RexxFunctionHandler  RxuMthgamma;
EXPORTSPEC RexxFunctionHandler  RxuMthhypot;

// Rexx Exit Handler prototypes
RexxExitHandler      passByNameInitTermExitHandler;

// Global variable (function names table)
PSZ  RxFncTable[] =
  {
#if defined(__BLD4OS2__)
  "RxUpm_u32elgn",
  "RxUpm_u32elgff",
  "RxUpm_u32elocu",
  "RxUpm_u32elocl",
  "RxUpm_u32eulgn",
  "RxUpm_u32eulgf",
  "RxUpm_u32eusrl",
  "RxUpm_u32esetp",
  "RxUpm_u32egetp",
  "RxUpm",
#endif
  "RxProcId",
  "RxVlist",
  "RxStorage",
  "RxScount",
  "RxRsoe2f",
  "RxRsoe2q",
  "RxKillProcess",
  "RxCreateNPipe",
  "RxSetNPHState",
  "RxCreateRexxThread",
  "RxSetExceptionExit",
  "RxCreateThread",
  "RxConnectNPipe",
  "RxDisConnectNPipe",
  "RxNbSessionStatus",
  "RxCreatePipe",
  "RxSoSe2H",
  "RxSi2H",
  "RxRSi2F",
  "RxLineInH",
  "RxAllocMem",
  "RxFreeMem",
  "RxDestroyPipe",
  "RxAllocSharedMem",
  "RxGetSharedMem",
  "RxGiveSharedMem",
  "RxGetNamedSharedMem",
  "RxCreateEventSem",
  "RxCloseEventSem",
  "RxOpenEventSem",
  "RxPostEventSem",
  "RxQueryEventSem",
  "RxResetEventSem",
  "RxWaitEventSem",
  "RxuTerm",
  "RxuQuery",
  "RxLoadModule",
  "RxFreeModule",
  "RxQueryModuleName",
  "RxQueryModuleHandle",
  "RxQueryProcType",
  "RxQueryProcAddr",
  "RxCallEntryPoint",
  "RxCallProcAddr",
  "RxAddMacro",
  "RxDropMacro",
  "RxClearMacroSpace",
  "RxSaveMacroSpace",
  "RxLoadMacroSpace",
  "RxQueryMacro",
  "RxReorderMacro",
  "RxSetMem",
  "RxQueryMem",
  "RxSubAllocMem",
  "RxSubFreeMem",
  "RxSubSetMem",
  "RxSubUnsetMem",
  "RxOpen",
  "RxRead",
  "RxWrite",
  "RxDosRead",
  "RxDosWrite",
  "RxCloseH",
  "RxQuerySysInfo",
  "RxTmrQueryFreq",
  "RxTmrQueryTime",
  "RxDevConfig",
  "RxDevIOCtl",
  "RxPhysicalDisk",
  "RxExecPgm",
  "RxWaitChild",
  "RxKillThread",
  "RxResumeThread",
  "RxSuspendThread",
  "RxStartSession",
  "RxStartRexxSession",
  "RxDetachRexxPgm",
  "RxWinQueryObject",
  "RxWinSetPresParam",
  "RxWinDestroyObject",
  "RxCreateMutexSem",
  "RxOpenMutexSem",
  "RxCloseMutexSem",
  "RxQueryMutexSem",
  "RxReleaseMutexSem",
  "RxRequestMutexSem",
  "RxAddMuxWaitSem",
  "RxDeleteMuxWaitSem",
  "RxCloseMuxWaitSem",
  "RxCreateMuxWaitSem",
  "RxOpenMuxWaitSem",
  "RxQueryMuxWaitSem",
  "RxWaitMuxWaitSem",
  "RxNet",
  "RxPullQueue",
  "RxAddQueue",
  "RxQueued",
  "RxPmPrintf",
  "RxCallInStore",
  "RxTokenize",
  "RxQProcStatus",
  "RxSetPriority",
  "RxExecI",
  "RxExecO",
  "RxQExists",
  "RxAdd2Ptr",
  "RxThunkAddr",
  "RxOpenQueue",
  "RxPeekQueue",
  "RxCloseQueue",
  "RxCreateQueue",
  "RxReadQueue",
  "RxReadQueueStr",
  "RxWriteQueue",
  "RxPurgeQueue",
  "RxQueryQueue",
  "RxSetError",
  "RxSearchPath",
  "RxReplaceModule",
  "RxExitList",
  "RxStructMap",
  "RxStruct2Stem",
  "RxStem2Struct",
  "RxGetInfoBlocks",
  "RxQueryAppType",
  "RxVioPopUp",
  "RxVioEndPopUp",
  "RxVioWrtCharStrAtt",
  "RxKbdCharIn",
  "RxDupHandle",
  "RxSetMaxFH",
  "RxSetRelMaxFH",
  "RxSetFHState",
  "RxQueryFHState",
  "RxRegisterFunctionExe",
  "RxRegisterExitDll",
  "RxRegisterExitExe",
  "RxQueryExit",
  "RxDeregisterExit",
  "RxWinSetSelf",
  "RxGlobalVar",
  "RxPassByName",
  "RxReturnByName",
  "RxPBNBufSize",
  "RxSetExtLibPath",
  "RxQueryExtLibPath",
  "RxRegisterFuncAddress",
  "RxCallFuncAddress",
  "RxuMthceil",
  "RxuMthcosh",
  "RxuMthexp",
  "RxuMthfloor",
  "RxuMthfmod",
  "RxuMthfrexp",
  "RxuMthldexp",
  "RxuMthlog",
  "RxuMthlog10",
  "RxuMthmodf",
  "RxuMthpow",
  "RxuMthsinh",
  "RxuMthtanh",
  "RxuMthasin",
  "RxuMthacos",
  "RxuMthatan2",
  "RxuMthatan",
  "RxuMthsin",
  "RxuMthcos",
  "RxuMthtan",
  "RxuMthsqrt",
  "RxuMthfabs",
  "RxuMtherf",
  "RxuMtherfc",
  "RxuMthgamma",
  "RxuMthhypot",
  "RxMalloc",
  "RxFree",
  "RxF2C",
  "RxC2F",
  };

// RxMalloc external function
EXPORTSPEC ULONG APIENTRY RxMalloc(PUCHAR name, ULONG numargs, RXSTRING args[], PSZ queuename, PRXSTRING retstr)
{
  PVOID  ptr=NULL;

  if (numargs < 1 || !args[0].strlength)
    return INVALID_ROUTINE;

  ptr = (PVOID) malloc(atoi(args[0].strptr));
  if (!ptr)
    return INVALID_ROUTINE;

  retstr->strlength = sizeof(PVOID);
  memcpy(retstr->strptr,&ptr,sizeof(PVOID));
  return VALID_ROUTINE;
}

// RxFree external function
EXPORTSPEC ULONG APIENTRY RxFree(PUCHAR name, ULONG numargs, RXSTRING args[], PSZ queuename, PRXSTRING retstr)
{
  PVOID  ptr=NULL;

  if (numargs < 1 || args[0].strlength != sizeof(PVOID))
    return INVALID_ROUTINE;

  memcpy(&ptr,args[0].strptr,sizeof(PVOID));
  free(ptr);

  retstr->strlength = 1;
  *retstr->strptr = '1';
  return VALID_ROUTINE;
}

// RxF2C external function
EXPORTSPEC ULONG APIENTRY  RxF2C(PUCHAR name, ULONG numargs, RXSTRING args[], PSZ queuename, PRXSTRING retstr)
{
  float       f;
  double      df;
#if !defined(_MSVC_)
  long double ldf;
#endif

  // Arg 1 = string representation of floating point number
  // Arg 2 = length of floating point to be created (32, 64, 80)

  if (numargs < 1 || !args[0].strlength)
    return INVALID_ROUTINE;

  if (numargs >= 2 && args[1].strlength)
    {
    switch(atoi(args[1].strptr))
      {
      case 32:
        f = atof(args[0].strptr);
        memcpy(retstr->strptr,&f,sizeof(f));
        retstr->strlength = sizeof(f);
        return VALID_ROUTINE;
        break;
      case 64:
        df = strtod(args[0].strptr,(char**)NULL);
        memcpy(retstr->strptr,&df,sizeof(df));
        retstr->strlength = sizeof(df);
        return VALID_ROUTINE;
        break;
#if !defined(_MSVC_)
      case 80:
        ldf = strtold(args[0].strptr,(char**)NULL);
        memcpy(retstr->strptr,&ldf,sizeof(ldf));
        retstr->strlength = sizeof(ldf);
        return VALID_ROUTINE;
        break;
#endif
      default:
        return INVALID_ROUTINE;
        break;
      }
    }
  else
    {
    f = atof(args[0].strptr);
    memcpy(retstr->strptr,&f,sizeof(f));
    retstr->strlength = sizeof(f);
    return VALID_ROUTINE;
    }
  return INVALID_ROUTINE;
}

// RxC2F external function
EXPORTSPEC ULONG APIENTRY  RxC2F(PUCHAR name, ULONG numargs, RXSTRING args[], PSZ queuename, PRXSTRING retstr)
{
  char *cp=NULL;
  int   numdec=16, decpos=0, sign=0;
  char  expbuf[10];

  // Arg 1 = binary representation of floating point number
  // Arg 2 = number of digits to specify (default = 16)

  if (numargs < 1)
    return INVALID_ROUTINE;

  if (numargs >= 2 && args[1].strlength)
    numdec = atoi(args[1].strptr);

  if (args[0].strlength == sizeof(float))
    {
    cp = _fcvt((double)*((float*)args[0].strptr),numdec,&decpos,&sign);
    }
  else if (args[0].strlength == sizeof(double))
    {
    cp = _fcvt(*((double*)args[0].strptr),numdec,&decpos,&sign);
    }
  else if (args[0].strlength == sizeof(long double))
    {
    return INVALID_ROUTINE;
    cp = _fcvt((double)*((long double*)args[0].strptr),numdec,&decpos,&sign);
    }
  else
    return INVALID_ROUTINE;

  // Set sign of number
  strcpy(retstr->strptr,(sign?"-":"+"));
  // Put decimal point in
  if (decpos > 0)
    {
    strncat(retstr->strptr,cp,decpos);
    strcat(retstr->strptr,".");
    strcat(retstr->strptr,&cp[decpos]);
    }
  else if (decpos == 0)
    {
    strcat(retstr->strptr,"0.");
    strcat(retstr->strptr,(*cp?cp:"0"));
    }
  else if (decpos < 0)
    {
    strcat(retstr->strptr,"0.");
    strcat(retstr->strptr,(*cp?cp:"0"));
    sprintf(expbuf,"e%ld",decpos);
    strcat(retstr->strptr,expbuf);
    }

  retstr->strlength = strlen(retstr->strptr);
  return VALID_ROUTINE;

}

// RxuMthceil external function
EXPORTSPEC ULONG APIENTRY  RxuMthceil(PUCHAR name, ULONG numargs, RXSTRING args[], PSZ queuename, PRXSTRING retstr)
{
  if (numargs < 1 || !args[0].strlength)
    return INVALID_ROUTINE;
  retstr->strlength = sprintf(retstr->strptr,"%g",ceil(atof(args[0].strptr)));
  return VALID_ROUTINE;
}

// RxuMthcosh external function
EXPORTSPEC ULONG APIENTRY  RxuMthcosh(PUCHAR name, ULONG numargs, RXSTRING args[], PSZ queuename, PRXSTRING retstr)
{
  if (numargs < 1 || !args[0].strlength)
    return INVALID_ROUTINE;
  retstr->strlength = sprintf(retstr->strptr,"%g",cosh(atof(args[0].strptr)));
  return VALID_ROUTINE;
}

// RxuMthexp external function
EXPORTSPEC ULONG APIENTRY  RxuMthexp(PUCHAR name, ULONG numargs, RXSTRING args[], PSZ queuename, PRXSTRING retstr)
{
  if (numargs < 1 || !args[0].strlength)
    return INVALID_ROUTINE;
  retstr->strlength = sprintf(retstr->strptr,"%g",exp(atof(args[0].strptr)));
  return VALID_ROUTINE;
}

// RxuMthfloor external function
EXPORTSPEC ULONG APIENTRY  RxuMthfloor(PUCHAR name, ULONG numargs, RXSTRING args[], PSZ queuename, PRXSTRING retstr)
{
  if (numargs < 1 || !args[0].strlength)
    return INVALID_ROUTINE;
  retstr->strlength = sprintf(retstr->strptr,"%g",floor(atof(args[0].strptr)));
  return VALID_ROUTINE;
}

// RxuMthfmod external function
EXPORTSPEC ULONG APIENTRY  RxuMthfmod(PUCHAR name, ULONG numargs, RXSTRING args[], PSZ queuename, PRXSTRING retstr)
{
  if (numargs < 2 || !args[0].strlength || !args[1].strlength)
    return INVALID_ROUTINE;
  retstr->strlength = sprintf(retstr->strptr,"%g",fmod(atof(args[0].strptr),atof(args[1].strptr)));
  return VALID_ROUTINE;
}

// RxuMthfrexp external function
EXPORTSPEC ULONG APIENTRY  RxuMthfrexp(PUCHAR name, ULONG numargs, RXSTRING args[], PSZ queuename, PRXSTRING retstr)
{
  int exponent=0;
  if (numargs < 1 || !args[0].strlength)
    return INVALID_ROUTINE;
  retstr->strlength = sprintf(retstr->strptr,"%g %ld",frexp(atof(args[0].strptr),&exponent),exponent);
  return VALID_ROUTINE;
}

// RxuMthldexp external function
EXPORTSPEC ULONG APIENTRY  RxuMthldexp(PUCHAR name, ULONG numargs, RXSTRING args[], PSZ queuename, PRXSTRING retstr)
{
  if (numargs < 2 || !args[0].strlength || !args[1].strlength)
    return INVALID_ROUTINE;
  retstr->strlength = sprintf(retstr->strptr,"%g",ldexp(atof(args[0].strptr),atoi(args[1].strptr)));
  return VALID_ROUTINE;
}

// RxuMthlog external function
EXPORTSPEC ULONG APIENTRY  RxuMthlog(PUCHAR name, ULONG numargs, RXSTRING args[], PSZ queuename, PRXSTRING retstr)
{
  if (numargs < 1 || !args[0].strlength)
    return INVALID_ROUTINE;
  retstr->strlength = sprintf(retstr->strptr,"%g",log(atof(args[0].strptr)));
  return VALID_ROUTINE;
}

// RxuMthlog10 external function
EXPORTSPEC ULONG APIENTRY  RxuMthlog10(PUCHAR name, ULONG numargs, RXSTRING args[], PSZ queuename, PRXSTRING retstr)
{
  if (numargs < 1 || !args[0].strlength)
    return INVALID_ROUTINE;
  retstr->strlength = sprintf(retstr->strptr,"%g",log10(atof(args[0].strptr)));
  return VALID_ROUTINE;
}

// RxuMthmodf external function
EXPORTSPEC ULONG APIENTRY  RxuMthmodf(PUCHAR name, ULONG numargs, RXSTRING args[], PSZ queuename, PRXSTRING retstr)
{
  double intpart=0;
  if (numargs < 2 || !args[0].strlength || !args[1].strlength)
    return INVALID_ROUTINE;
  retstr->strlength = sprintf(retstr->strptr,"%g %g",modf(atof(args[0].strptr),&intpart),intpart);
  return VALID_ROUTINE;
}

// RxuMthpow external function
EXPORTSPEC ULONG APIENTRY  RxuMthpow(PUCHAR name, ULONG numargs, RXSTRING args[], PSZ queuename, PRXSTRING retstr)
{
  if (numargs < 2 || !args[0].strlength || !args[1].strlength)
    return INVALID_ROUTINE;
  retstr->strlength = sprintf(retstr->strptr,"%g",pow(atof(args[0].strptr),atof(args[1].strptr)));
  return VALID_ROUTINE;
}

// RxuMthsinh external function
EXPORTSPEC ULONG APIENTRY  RxuMthsinh(PUCHAR name, ULONG numargs, RXSTRING args[], PSZ queuename, PRXSTRING retstr)
{
  if (numargs < 1 || !args[0].strlength)
    return INVALID_ROUTINE;
  retstr->strlength = sprintf(retstr->strptr,"%g",sinh(atof(args[0].strptr)));
  return VALID_ROUTINE;
}

// RxuMthtanh external function
EXPORTSPEC ULONG APIENTRY  RxuMthtanh(PUCHAR name, ULONG numargs, RXSTRING args[], PSZ queuename, PRXSTRING retstr)
{
  if (numargs < 1 || !args[0].strlength)
    return INVALID_ROUTINE;
  retstr->strlength = sprintf(retstr->strptr,"%g",tanh(atof(args[0].strptr)));
  return VALID_ROUTINE;
}

// RxuMthasin external function
EXPORTSPEC ULONG APIENTRY  RxuMthasin(PUCHAR name, ULONG numargs, RXSTRING args[], PSZ queuename, PRXSTRING retstr)
{
  if (numargs < 1 || !args[0].strlength)
    return INVALID_ROUTINE;
  retstr->strlength = sprintf(retstr->strptr,"%g",asin(atof(args[0].strptr)));
  return VALID_ROUTINE;
}

// RxuMthacos external function
EXPORTSPEC ULONG APIENTRY  RxuMthacos(PUCHAR name, ULONG numargs, RXSTRING args[], PSZ queuename, PRXSTRING retstr)
{
  if (numargs < 1 || !args[0].strlength)
    return INVALID_ROUTINE;
  retstr->strlength = sprintf(retstr->strptr,"%g",acos(atof(args[0].strptr)));
  return VALID_ROUTINE;
}

// RxuMthatan2 external function
EXPORTSPEC ULONG APIENTRY  RxuMthatan2(PUCHAR name, ULONG numargs, RXSTRING args[], PSZ queuename, PRXSTRING retstr)
{
  if (numargs < 2 || !args[0].strlength || !args[1].strlength)
    return INVALID_ROUTINE;
  retstr->strlength = sprintf(retstr->strptr,"%g",atan2(atof(args[0].strptr),atof(args[1].strptr)));
  return VALID_ROUTINE;
}

// RxuMthatan external function
EXPORTSPEC ULONG APIENTRY  RxuMthatan(PUCHAR name, ULONG numargs, RXSTRING args[], PSZ queuename, PRXSTRING retstr)
{
  if (numargs < 1 || !args[0].strlength)
    return INVALID_ROUTINE;
  retstr->strlength = sprintf(retstr->strptr,"%g",atan(atof(args[0].strptr)));
  return VALID_ROUTINE;
}

// RxuMthsin external function
EXPORTSPEC ULONG APIENTRY  RxuMthsin(PUCHAR name, ULONG numargs, RXSTRING args[], PSZ queuename, PRXSTRING retstr)
{
  if (numargs < 1 || !args[0].strlength)
    return INVALID_ROUTINE;
  retstr->strlength = sprintf(retstr->strptr,"%g",sin(atof(args[0].strptr)));
  return VALID_ROUTINE;
}

// RxuMthcos external function
EXPORTSPEC ULONG APIENTRY  RxuMthcos(PUCHAR name, ULONG numargs, RXSTRING args[], PSZ queuename, PRXSTRING retstr)
{
  if (numargs < 1 || !args[0].strlength)
    return INVALID_ROUTINE;
  retstr->strlength = sprintf(retstr->strptr,"%g",cos(atof(args[0].strptr)));
  return VALID_ROUTINE;
}

// RxuMthtan external function
EXPORTSPEC ULONG APIENTRY  RxuMthtan(PUCHAR name, ULONG numargs, RXSTRING args[], PSZ queuename, PRXSTRING retstr)
{
  if (numargs < 1 || !args[0].strlength)
    return INVALID_ROUTINE;
  retstr->strlength = sprintf(retstr->strptr,"%g",tan(atof(args[0].strptr)));
  return VALID_ROUTINE;
}

// RxuMthsqrt external function
EXPORTSPEC ULONG APIENTRY  RxuMthsqrt(PUCHAR name, ULONG numargs, RXSTRING args[], PSZ queuename, PRXSTRING retstr)
{
  if (numargs < 1 || !args[0].strlength)
    return INVALID_ROUTINE;
  retstr->strlength = sprintf(retstr->strptr,"%g",sqrt(atof(args[0].strptr)));
  return VALID_ROUTINE;
}

// RxuMthfabs external function
EXPORTSPEC ULONG APIENTRY  RxuMthfabs(PUCHAR name, ULONG numargs, RXSTRING args[], PSZ queuename, PRXSTRING retstr)
{
  if (numargs < 1 || !args[0].strlength)
    return INVALID_ROUTINE;
  retstr->strlength = sprintf(retstr->strptr,"%g",fabs(atof(args[0].strptr)));
  return VALID_ROUTINE;
}

#if !defined(_MSC_VER)

// RxuMtherf external function
EXPORTSPEC ULONG APIENTRY  RxuMtherf(PUCHAR name, ULONG numargs, RXSTRING args[], PSZ queuename, PRXSTRING retstr)
{
  if (numargs < 1 || !args[0].strlength)
    return INVALID_ROUTINE;
  retstr->strlength = sprintf(retstr->strptr,"%g",erf(atof(args[0].strptr)));
  return VALID_ROUTINE;
}

// RxuMtherfc external function
EXPORTSPEC ULONG APIENTRY  RxuMtherfc(PUCHAR name, ULONG numargs, RXSTRING args[], PSZ queuename, PRXSTRING retstr)
{
  if (numargs < 1 || !args[0].strlength)
    return INVALID_ROUTINE;
  retstr->strlength = sprintf(retstr->strptr,"%g",erfc(atof(args[0].strptr)));
  return VALID_ROUTINE;
}

// RxuMthhypot external function
EXPORTSPEC ULONG APIENTRY  RxuMthhypot(PUCHAR name, ULONG numargs, RXSTRING args[], PSZ queuename, PRXSTRING retstr)
{
  if (numargs < 2 || !args[0].strlength || !args[1].strlength)
    return INVALID_ROUTINE;
  retstr->strlength = sprintf(retstr->strptr,"%g",hypot(atof(args[0].strptr),atof(args[1].strptr)));
  return VALID_ROUTINE;
}

// RxuMthgamma external function
EXPORTSPEC ULONG APIENTRY  RxuMthgamma(PUCHAR name, ULONG numargs, RXSTRING args[], PSZ queuename, PRXSTRING retstr)
{
  if (numargs < 1 || !args[0].strlength)
    return INVALID_ROUTINE;
  retstr->strlength = sprintf(retstr->strptr,"%g",gamma(atof(args[0].strptr)));
  return VALID_ROUTINE;
}
#endif

#if defined(__BLD4OS2__)

// RxUpm_u32elgn external function
EXPORTSPEC ULONG APIENTRY  RxUpm_u32elgn(PUCHAR name, ULONG numargs, RXSTRING args[], PSZ queuename, PRXSTRING retstr)
{
  typedef short (_System _U32ELGN) (UCHAR*,UCHAR*,UCHAR*,ULONG,ULONG);
  typedef _U32ELGN *PU32ELGN;
  PU32ELGN  pu32elgn;
  short     upmrc=0;

  if (numargs < 5)
    return INVALID_ROUTINE;

  if (!loadAPI("UPM32","U32ELGN",0,(PFN*)&pu32elgn))
    return INVALID_ROUTINE;

  upmrc = (*pu32elgn)(args[0].strptr,args[1].strptr,args[2].strptr,atoi(args[3].strptr),atoi(args[4].strptr));
  retstr->strlength = sprintf(retstr->strptr,"%hx",upmrc);

  return VALID_ROUTINE;
}

// RxUpm_u32elgff external function
EXPORTSPEC ULONG APIENTRY  RxUpm_u32elgff(PUCHAR name, ULONG numargs, RXSTRING args[], PSZ queuename, PRXSTRING retstr)
{
  typedef short (_System _U32ELGFF) (UCHAR*,UCHAR*,ULONG);
  typedef _U32ELGFF *PU32ELGFF;
  PU32ELGFF  pu32elgff;
  short     upmrc=0;

  if (numargs < 3)
    return INVALID_ROUTINE;

  if (!loadAPI("UPM32","U32ELGFF",0,(PFN*)&pu32elgff))
    return INVALID_ROUTINE;

  upmrc = (*pu32elgff)(args[0].strptr,args[1].strptr,atoi(args[2].strptr));
  retstr->strlength = sprintf(retstr->strptr,"%hx",upmrc);

  return VALID_ROUTINE;
}

// RxUpm_u32elocu external function
EXPORTSPEC ULONG APIENTRY  RxUpm_u32elocu(PUCHAR name, ULONG numargs, RXSTRING args[], PSZ queuename, PRXSTRING retstr)
{
  typedef short (_System _U32ELOCU) (UCHAR*,ULONG*);
  typedef _U32ELOCU *PU32ELOCU;
  PU32ELOCU  pu32elocu;
  ULONG  type=0;
  short     upmrc=0;

  if (numargs < 1)
    return INVALID_ROUTINE;

  if (!loadAPI("UPM32","U32ELOCU",0,(PFN*)&pu32elocu))
    return INVALID_ROUTINE;

  upmrc = (*pu32elocu)(args[0].strptr,&type);
  retstr->strlength = sprintf(retstr->strptr,"%hx %ld",upmrc,type);

  return VALID_ROUTINE;
}

// RxUpm_u32elocl external function
EXPORTSPEC ULONG APIENTRY  RxUpm_u32elocl(PUCHAR name, ULONG numargs, RXSTRING args[], PSZ queuename, PRXSTRING retstr)
{
  typedef short (_System _U32ELOCL) (UCHAR*,ULONG*);
  typedef _U32ELOCL *PU32ELOCL;
  PU32ELOCL  pu32elocl;
  ULONG  type=0;
  short     upmrc=0;

  if (numargs < 1)
    return INVALID_ROUTINE;

  if (!loadAPI("UPM32","U32ELOCL",0,(PFN*)&pu32elocl))
    return INVALID_ROUTINE;

  upmrc = (*pu32elocl)(args[0].strptr,&type);
  retstr->strlength = sprintf(retstr->strptr,"%hx %ld",upmrc,type);

  return VALID_ROUTINE;
}

// RxUpm_u32eulgn external function
EXPORTSPEC ULONG APIENTRY  RxUpm_u32eulgn(PUCHAR name, ULONG numargs, RXSTRING args[], PSZ queuename, PRXSTRING retstr)
{
  typedef short (_System _U32EULGN) (UCHAR*,UCHAR*,UCHAR*,ULONG,ULONG);
  typedef _U32EULGN *PU32EULGN;
  PU32EULGN  pu32eulgn;
  short     upmrc=0;

  if (numargs < 5)
    return INVALID_ROUTINE;

  if (!loadAPI("UPM32","U32EULGN",0,(PFN*)&pu32eulgn))
    return INVALID_ROUTINE;

  upmrc = (*pu32eulgn)(args[0].strptr,args[1].strptr,args[2].strptr,atoi(args[3].strptr),atoi(args[4].strptr));
  retstr->strlength = sprintf(retstr->strptr,"%hx",upmrc);

  return VALID_ROUTINE;
}

// RxUpm_u32eulgf external function
EXPORTSPEC ULONG APIENTRY  RxUpm_u32eulgf(PUCHAR name, ULONG numargs, RXSTRING args[], PSZ queuename, PRXSTRING retstr)
{
  typedef short (_System _U32EULGF) (UCHAR*,UCHAR*,ULONG);
  typedef _U32EULGF *PU32EULGF;
  PU32EULGF  pu32eulgf;
  short     upmrc=0;

  if (numargs < 3)
    return INVALID_ROUTINE;

  if (!loadAPI("UPM32","U32EULGF",0,(PFN*)&pu32eulgf))
    return INVALID_ROUTINE;

  upmrc = (*pu32eulgf)(args[0].strptr,args[1].strptr,atoi(args[2].strptr));
  retstr->strlength = sprintf(retstr->strptr,"%hx",upmrc);

  return VALID_ROUTINE;
}

// RxUpm_u32eusrl external function
EXPORTSPEC ULONG APIENTRY  RxUpm_u32eusrl(PUCHAR name, ULONG numargs, RXSTRING args[], PSZ queuename, PRXSTRING retstr)
{
  return INVALID_ROUTINE;
}

// RxUpm_u32esetp external function
EXPORTSPEC ULONG APIENTRY  RxUpm_u32esetp(PUCHAR name, ULONG numargs, RXSTRING args[], PSZ queuename, PRXSTRING retstr)
{
  return INVALID_ROUTINE;
}

// RxUpm_u32egetp external function
EXPORTSPEC ULONG APIENTRY  RxUpm_u32egetp(PUCHAR name, ULONG numargs, RXSTRING args[], PSZ queuename, PRXSTRING retstr)
{
  return INVALID_ROUTINE;
}

// RxUpm external function
EXPORTSPEC ULONG APIENTRY  RxUpm(PUCHAR name, ULONG numargs, RXSTRING args[],
             PSZ queuename, PRXSTRING retstr)
{

  ULONG Get_DB_Node(PCH, USHORT *, USHORT *, PCH, USHORT *, long *);

  USHORT    idlen = 0, pwlen = 0, rnlen = 0, rtype = 0;
  USHORT    authcheck = 0, action;
  USHORT    cctr, idread, totalid, upmrc, dbnodelen;
  PCH       uid=NULL, pw=NULL, remname=NULL, cptr;
  UCHAR     dbnode[8];
  long      retsqlcode;
  USHORT    upmmaintrc;
  APIRET    rc;
  PRXSTRING pargs;
  PUPMGLGNFN   pUpmglgnFn;
  PUPMGULGNFN  pUpmgulgnFn;
  PUPMGLGFFFN  pUpmglgffFn;
  PUPMGULGFFN  pUpmgulgfFn;
  PUPMGLOCUFN  pUpmglocuFn;
  PUPMGLOCLFN  pUpmgloclFn;
  PUPMGUSRLFN  pUpmgusrlFn;

  struct UPM_G_USER_LOGON uinfo[NUM_UINFO_BLOCKS], *bufptr;
  bufptr = uinfo;

  // Handle FUNCTION parameter (arg 1)
  if (numargs >= 1)
    {
    if (args[0].strlength == 0)
      return INVALID_ROUTINE;
    strupr(args[0].strptr);
    if (!strncmp("LOGON",args[0].strptr,args[0].strlength)
       || !strncmp("LOGONP",args[0].strptr,args[0].strlength))
      action = PROCESS_LOGON;
    else if (!strncmp("LOGONU",args[0].strptr,args[0].strlength))
      action = USER_LOGON;
    else if (!strncmp("LOGOFF",args[0].strptr,args[0].strlength)
            || !strncmp("LOGOFFP",args[0].strptr,args[0].strlength))
      action = PROCESS_LOGOFF;
    else if (!strncmp("LOGOFFU",args[0].strptr,args[0].strlength))
      action = USER_LOGOFF;
    else if (!strncmp("GLL",args[0].strptr,args[0].strlength))
      action = GET_LOCAL_LOGON;
    else if (!strncmp("UONL",args[0].strptr,args[0].strlength))
      action = LOGON_USER_LOCAL;
    else if (!strncmp("LOGONLIST",args[0].strptr,args[0].strlength))
      action = GET_LOGON_LIST;
    else
      return INVALID_ROUTINE;
    }
  else
    return INVALID_ROUTINE;

  // Handle USERID parameter (arg 2)
  if (numargs >= 2)
    {
    if (args[1].strlength > 0)
      strupr(args[1].strptr);
    idlen = args[1].strlength;
    uid = args[1].strptr;
    }
  else
    idlen = 0;

  // Handle PASSWORD/OLD-PASSWORD parameter (arg 3)
  if (numargs >= 3)
    {
    if (args[2].strlength > 0)
      strupr(args[2].strptr);
    pwlen = args[2].strlength;
    pw = args[2].strptr;
    }
  else
    {
    pwlen = 0;
    if (action == PROCESS_LOGON || action == USER_LOGON)
      return INVALID_ROUTINE;
    }

  // Handle TYPE/NEW-PASSWORD parameter (arg 4)
  if (numargs >= 4)
    {
    *args[3].strptr = toupper(*args[3].strptr);
    if (*args[3].strptr == 'L' || args[3].strlength == 0)
      rtype = UPM_LOCAL;
    else if (*args[3].strptr == 'D')
      {
      rtype = UPM_DOMAIN;
      if ( (numargs < 5 || args[4].strlength == 0) && action != GET_LOGON_LIST)
        return (INVALID_ROUTINE);
      }
    else if (*args[3].strptr == 'N')
      {
      rtype = UPM_DNODE;
      if ( (numargs < 5 || args[4].strlength == 0) && action != GET_LOGON_LIST)
        return (INVALID_ROUTINE);
      }
    else if (*args[3].strptr == 'B')
      {
      rtype = UPM_DNODE;
      if ( (numargs < 5 || args[4].strlength == 0) && action != GET_LOGON_LIST)
        return (INVALID_ROUTINE);
      }
    else
      return INVALID_ROUTINE;
    }
  else
    rtype = UPM_LOCAL;

  // Handle LOCATION parameter (arg 5)
  if (numargs >= 5)
    {
    if (args[4].strlength > 0)
      strupr(args[4].strptr);
    rnlen = args[4].strlength;
    remname = args[4].strptr;
    if (*args[3].strptr == 'B')
      {
      if (Get_DB_Node(dbnode,&dbnodelen,
                      &rnlen,remname,&rtype,&retsqlcode) == INVALID_ROUTINE)
        {
        retstr->strlength = sprintf(retstr->strptr,"%ld",retsqlcode);
        return VALID_ROUTINE;
        }
      remname = dbnode;
      rnlen = dbnodelen;
      }
    }
  else
    rnlen = 0;

  /* -----  Just ignore a remote name for a local logon/logoff ----
  if (rnlen > 0 && rtype == UPM_LOCAL)
    return INVALID_ROUTINE;
  */

  // Handle AUTHCHECK parameter (arg 6)
  if (numargs >= 6)
    {
    if (args[5].strlength > 0)
      {
      *args[5].strptr = toupper(*args[5].strptr);
      if (*args[5].strptr == 'U' || args[5].strlength == 0)
        authcheck = UPM_USER;
      else if (*args[5].strptr == 'A')
        authcheck = UPM_ADMIN;
      else if (*args[5].strptr == 'C')
        authcheck = UPM_CONFIG;
      else
        return INVALID_ROUTINE;
      }
    else
      return INVALID_ROUTINE;
    }
  else
    authcheck = UPM_USER;

  switch(action)
    {
    case PROCESS_LOGON:
      if ( !loadAPI("UPM","UPMGLGN",0,(PFN *)&pUpmglgnFn) )
        return INVALID_ROUTINE;
      upmrc = (*pUpmglgnFn)(idlen,pwlen,rnlen,rtype,authcheck,uid,pw,remname);
      //upmrc = upmglgn(idlen, pwlen, rnlen, rtype,
      //                authcheck, uid, pw, remname);
      break;
    case USER_LOGON:
      if ( !loadAPI("UPM","UPMGULGN",0,(PFN *)&pUpmgulgnFn) )
        return INVALID_ROUTINE;
      authcheck = 0;
      upmrc = (*pUpmgulgnFn)(idlen,pwlen,rnlen,rtype,authcheck,uid,pw,remname);
      //upmrc = upmgulgn(idlen, pwlen, rnlen, rtype,
      //                 authcheck, uid, pw, remname);
      break;
    case PROCESS_LOGOFF:
      if ( !loadAPI("UPM","UPMGLGFF",0,(PFN *)&pUpmglgffFn) )
        return INVALID_ROUTINE;
      upmrc = (*pUpmglgffFn)(idlen,rnlen,rtype,uid,remname);
      //upmrc = upmglgff(idlen, rnlen, rtype, uid, remname);
      break;
    case USER_LOGOFF:
      if ( !loadAPI("UPM","UPMGULGF",0,(PFN *)&pUpmgulgfFn) )
        return INVALID_ROUTINE;
      upmrc = (*pUpmgulgfFn)(idlen,rnlen,rtype,uid,remname);
      //upmrc = upmgulgf(idlen, rnlen, rtype, uid, remname);
      break;
    case GET_LOCAL_LOGON:
      if ( !loadAPI("UPM","UPMGLOCU",0,(PFN *)&pUpmglocuFn) )
        return INVALID_ROUTINE;
      upmrc = (*pUpmglocuFn)(&idlen,uinfo[0].userid,&authcheck);
      //upmrc = upmglocu(&idlen, uinfo[0].userid, &authcheck);
      printf ("Logged on user is '%s' (%ld)\n",uinfo[0].userid,authcheck);
      break;
    case LOGON_USER_LOCAL:
      if ( !loadAPI("UPM","UPMGLOCL",0,(PFN *)&pUpmgloclFn) )
        return INVALID_ROUTINE;
      authcheck = 0;
      upmrc = (*pUpmgloclFn)(&idlen,uinfo[0].userid,&authcheck);
      //upmrc = upmglocl(&idlen, uid, &authcheck);
      break;
    case GET_LOGON_LIST:
      {
      UCHAR varnamebuf[250], dbuf[10];
      SHVBLOCK shvb;

      if ( !loadAPI("UPM","UPMGUSRL",0,(PFN *)&pUpmgusrlFn) )
        return INVALID_ROUTINE;

      // If no stem name was supplied
      if (numargs < 2)
        return INVALID_ROUTINE;
      if (args[1].strlength == 0)
        return INVALID_ROUTINE;

      upmrc = (*pUpmgusrlFn)(rnlen,rtype,(USHORT)sizeof(uinfo) * NUM_UINFO_BLOCKS,
                             &idread,&totalid,remname,(PCH)bufptr);
      //upmrc = upmgusrl(rnlen, rtype,
      //                 (USHORT) sizeof(uinfo) * NUM_UINFO_BLOCKS,
      //                 &idread, &totalid, remname, (PCH) bufptr);

      // Setup for RexxVariablePool access to build stem structure
      shvb.shvnext = (PSHVBLOCK)NULL;

      // If name supplied was a stem, drop the structure first
      if ( *((args[1].strptr) + (args[1].strlength) - 1) == '.')
        {
        shvb.shvcode = RXSHV_DROPV;
        shvb.shvname.strptr = args[1].strptr;
        shvb.shvname.strlength = args[1].strlength;
        if (RexxVariablePool(&shvb) == RXSHV_BADN)
          return INVALID_ROUTINE;
        }

      shvb.shvcode = RXSHV_SET;
      shvb.shvname.strptr = varnamebuf;
      shvb.shvvalue.strptr = dbuf;

      // Set stem.0
      sprintf (varnamebuf,"%.*s0",args[1].strlength,args[1].strptr);
      shvb.shvname.strlength = strlen(varnamebuf);
      sprintf (dbuf,"%ld",idread);
      shvb.shvvalue.strlength = strlen(dbuf);
      if (RexxVariablePool(&shvb) == RXSHV_BADN)
        return INVALID_ROUTINE;

      for (cctr=0;cctr<idread;cctr++)
        {

        // Build userid value
        sprintf (varnamebuf,"%.*s%ld.1",args[1].strlength,args[1].strptr,
                 cctr + 1);
        shvb.shvname.strlength = strlen(varnamebuf);
        shvb.shvvalue.strptr = uinfo[cctr].userid;
        shvb.shvvalue.strlength = uinfo[cctr].useridlen;
        if (RexxVariablePool(&shvb) == RXSHV_BADN)
          return INVALID_ROUTINE;

        // Build remote name value
        sprintf (varnamebuf,"%.*s%ld.2",args[1].strlength,args[1].strptr,
                 cctr + 1);
        shvb.shvname.strlength = strlen(varnamebuf);
        shvb.shvvalue.strptr = uinfo[cctr].remotename;
        shvb.shvvalue.strlength = uinfo[cctr].remotelen;
        if (RexxVariablePool(&shvb) == RXSHV_BADN)
          return INVALID_ROUTINE;

        // Build session-id value
        sprintf (varnamebuf,"%.*s%ld.3",args[1].strlength,args[1].strptr,
                 cctr + 1);
        shvb.shvname.strlength = strlen(varnamebuf);
        sprintf (dbuf,"%ld",uinfo[cctr].sessionid);
        shvb.shvvalue.strptr = dbuf;
        shvb.shvvalue.strlength = strlen(dbuf);
        if (RexxVariablePool(&shvb) == RXSHV_BADN)
          return INVALID_ROUTINE;

        // Build remote type value
        sprintf (varnamebuf,"%.*s%ld.4",args[1].strlength,args[1].strptr,
                 cctr + 1);
        shvb.shvname.strlength = strlen(varnamebuf);
        switch (uinfo[cctr].remotetype)
          {
          case UPM_LOCAL:
            sprintf(dbuf,"Local");
            break;
          case UPM_DNODE:
            sprintf(dbuf,"Node");
            break;
          case UPM_DOMAIN:
            sprintf(dbuf,"Domain");
            break;
          default:
            sprintf(dbuf,"?");
            break;
          }
        shvb.shvvalue.strptr = dbuf;
        shvb.shvvalue.strlength = strlen(dbuf);
        if (RexxVariablePool(&shvb) == RXSHV_BADN)
          return INVALID_ROUTINE;
        }
      break;
      }
    default:
      return INVALID_ROUTINE;
      break;
    }

  retstr->strlength = sprintf(retstr->strptr,"%X",upmrc);
  return VALID_ROUTINE;
}

// Get Database Node Name function
ULONG Get_DB_Node(PCH dbnode, USHORT *dbnlen, USHORT *rnlen,
                  PCH remname,USHORT *rtype, long *retsqlcode)

{

  typedef struct sqledinfo * _Seg16 PSQLEDINFO;
  USHORT hdir, hcnt, hctr, cctr;
  struct sqlca sqlca;
  PSQLEDINFO psqledinfo;
  PCH cptr;
  PSQLGDCLSFN pSqlgdclsFn;
  PSQLGDGNEFN pSqlgdgneFn;
  PSQLGDOPSFN pSqlgdopsFn;

  sqlca.sqlcabc = sizeof(sqlca);

  if ( !loadAPI("SQLAPI","SQLGDCLS",0,(PFN *)&pSqlgdclsFn) )
    return INVALID_ROUTINE;
  if ( !loadAPI("SQLAPI","SQLGDGNE",0,(PFN *)&pSqlgdgneFn) )
    return INVALID_ROUTINE;
  if ( !loadAPI("SQLAPI","SQLGDOPS",0,(PFN *)&pSqlgdopsFn) )
    return INVALID_ROUTINE;

  *dbnlen = 0;
  *rtype = UPM_LOCAL;

  (*pSqlgdopsFn)('0',&hdir,&hcnt,&sqlca);
  //SQLGDOPS('0',&hdir, &hcnt, &sqlca);
  if (sqlca.sqlcode != 0)
    {
    printf ("SQLGDOPS sqlcode = %ld\n",sqlca.sqlcode);
    *retsqlcode = sqlca.sqlcode;
    return INVALID_ROUTINE;
    }

  for (hctr=1;hctr <= hcnt;hctr++)
    {
    (*pSqlgdgneFn)(hdir,&psqledinfo,&sqlca);
    //SQLGDGNE(hdir, &psqledinfo, &sqlca);
    if (sqlca.sqlcode != 0)
      {
      printf ("SQLGDGNE sqlcode = %ld\n",sqlca.sqlcode);
      *retsqlcode = sqlca.sqlcode;
      (*pSqlgdclsFn)(hdir,&sqlca);
      //SQLGDCLS(hdir, &sqlca);
      return INVALID_ROUTINE;
      }
    if (!strncmp(psqledinfo->alias,remname,*rnlen))
      {
      // Check to see that names are same length too
      cptr = psqledinfo->alias;
      for (cctr=0;cctr < sizeof(psqledinfo->alias);cctr++)
        {
        if (*cptr == ' ')  // When space is found, that's length of dbalias
          break;
        cptr++;
        }
      if (cctr != *rnlen)
        continue;
      if (psqledinfo->type == SQL_REMOTE)
        {
        strncpy(dbnode,psqledinfo->nodename,sizeof(psqledinfo->nodename));
        cptr=dbnode;
        for(*dbnlen=0;*dbnlen<sizeof(psqledinfo->nodename);)
          {
          if (*cptr == ' ')  // When space is found, that's length
            break;
          cptr++;
          (*dbnlen)++;
          }
        *rtype = UPM_DNODE;
        }
      break;
      }
    }

  (*pSqlgdclsFn)(hdir,&sqlca);
  //SQLGDCLS(hdir, &sqlca);

  if (hctr > hcnt)
    {
    printf("Database Name '%.*s' not found\n",*rnlen,remname);
    *retsqlcode = -9999;
    return INVALID_ROUTINE;
    }

  return VALID_ROUTINE;
}

// RxNet external function
EXPORTSPEC ULONG APIENTRY  RxNet(PUCHAR name, ULONG numargs, RXSTRING args[],
             PSZ queuename, PRXSTRING retstr)

{

  USHORT            ctr, maintrc, action;
  char  *           pservername = NULL;
  PNETUSEADDFN      pNetUseAddFn;
  PNETUSEDELFN      pNetUseDelFn;
  PNETUSEENUMFN     pNetUseEnumFn;
  PNETUSERADDFN     pNetUserAddFn;
  PNETUSERPWSETFN   pNetUserPwSetFn;
  PNETUSERDELFN     pNetUserDelFn;
  PNETUSERVAL2FN    pNetUserVal2Fn;
  PNETUSERENUMFN    pNetUserEnumFn;
  PNETUSERGETINFOFN pNetUserGetInfoFn;
  PNETLOGONENUMFN   pNetLogonEnumFn;
  PNETFILEENUM2FN   pNetFileEnum2Fn;

  // Handle FUNCTION parameter (arg 1)
  if (numargs >= 1)
    {
    if (args[0].strlength == 0)
      return INVALID_ROUTINE;
    strupr(args[0].strptr);
    if (!strncmp("USERPWSET",args[0].strptr,args[0].strlength))
      action = USER_PW_SET;
    else if (!strncmp("USEADD",args[0].strptr,args[0].strlength))
      action = USE_ADD;
    else if (!strncmp("USEDEL",args[0].strptr,args[0].strlength))
      action = USE_DEL;
    else if (!strncmp("USEENUM",args[0].strptr,args[0].strlength))
      action = USE_ENUM;
    else if (!strncmp("USERADD",args[0].strptr,args[0].strlength))
      action = USER_ADD;
    else if (!strncmp("USERDEL",args[0].strptr,args[0].strlength))
      action = USER_DEL;
    else if (!strncmp("USERVAL",args[0].strptr,args[0].strlength))
      action = USER_VAL;
    else if (!strncmp("USERENUM",args[0].strptr,args[0].strlength))
      action = USER_ENUM;
    else if (!strncmp("USERGETINFO",args[0].strptr,args[0].strlength))
      action = USER_GETINFO;
    else if (!strncmp("LOGONENUM",args[0].strptr,args[0].strlength))
      action = LOGON_ENUM;
    else if (!strncmp("FILEENUM",args[0].strptr,args[0].strlength))
      action = FILE_ENUM;
    else
      return INVALID_ROUTINE;
    }
  else
    return INVALID_ROUTINE;

  switch (action)
    {
    case USER_PW_SET:
      /* Argument list:
         arg[0] - function code
         arg[1] - server name
         arg[2] - userid
         arg[3] - oldpw
         arg[4] - newpw
      */
      if (numargs < 5)
        return INVALID_ROUTINE;

      if ( !loadAPI("NETAPI","NETUSERPASSWORDSET",0,(PFN *)&pNetUserPwSetFn) )
        return INVALID_ROUTINE;

      for (ctr=1;ctr <= 4;ctr++)
        {
        if (args[ctr].strlength == 0)
          return INVALID_ROUTINE;
        strupr(args[ctr].strptr);
        }
      //printf("Changing password at node '%s' for userid '%s'\n",
      //       args[1].strptr,args[2].strptr);
      //printf("from '%s' to '%s'\n",args[3].strptr,args[4].strptr);
      maintrc = (*pNetUserPwSetFn)(args[1].strptr,args[2].strptr,
                                   args[3].strptr,args[4].strptr);
      //maintrc = NetUserPasswordSet(args[1].strptr,args[2].strptr,
      //                             args[3].strptr,args[4].strptr);
      retstr->strlength = sprintf(retstr->strptr,"%ld",maintrc);
      return VALID_ROUTINE;
      break;
    case USER_ADD:
      {
      /* Argument list:
         arg[0] - function code
         arg[1] - server name
         arg[2] - userid
         arg[3] - password
         arg[4] - user_level
         arg[5] - user_comment
      */
      struct user_info_1 ui1;

      if (numargs < 6)
        return INVALID_ROUTINE;
      if (args[2].strlength > UNLEN+1)
        return INVALID_ROUTINE;
      if (args[3].strlength > ENCRYPTED_PWLEN)
        return INVALID_ROUTINE;

      if ( !loadAPI("NETAPI","NETUSERADD",0,(PFN *)&pNetUserAddFn) )
        return INVALID_ROUTINE;

      for (ctr=1;ctr <= 5;ctr++)
        {
        if (args[ctr].strlength == 0)
          return INVALID_ROUTINE;
        strupr(args[ctr].strptr);
        }
      memset(&ui1,0x00,sizeof(ui1));
      strcpy(ui1.usri1_name,args[2].strptr);
      strcpy(ui1.usri1_password,args[3].strptr);
      switch (*(args[4].strptr))
        {
        case 'g':
        case 'G':
          ui1.usri1_priv = USER_PRIV_GUEST;
          break;
        case 'u':
        case 'U':
          ui1.usri1_priv = USER_PRIV_USER;
          break;
        case 'a':
        case 'A':
          ui1.usri1_priv = USER_PRIV_ADMIN;
          break;
        default:
          return INVALID_ROUTINE;
          break;
        }
      ui1.usri1_comment = args[5].strptr;
      pservername = args[1].strptr;
      ui1.usri1_flags = UF_SCRIPT;
      //printf("Adding user '%s' with password '%s' of privilege '%d'\n",
      //       ui1.usri1_name,ui1.usri1_password,ui1.usri1_priv);
      //printf("with comment '%s'\n",args[5].strptr);
      //printf("at server '%s'\n",args[1].strptr);
      maintrc = (*pNetUserAddFn)(pservername,1,(PCH) &ui1,sizeof(ui1));
      //maintrc = NetUserAdd(pservername,1,(PCH) &ui1,sizeof(ui1));
      retstr->strlength = sprintf(retstr->strptr,"%ld",maintrc);
      return VALID_ROUTINE;
      break;
      }
    case USER_DEL:
      /* Argument list:
         arg[0] - function code
         arg[1] - server name
         arg[2] - userid
      */
      if (numargs < 3)
        return INVALID_ROUTINE;

      if ( !loadAPI("NETAPI","NETUSERDEL",0,(PFN *)&pNetUserDelFn) )
        return INVALID_ROUTINE;

      for (ctr=1;ctr <= 2;ctr++)
        {
        if (args[ctr].strlength == 0)
          return INVALID_ROUTINE;
        strupr(args[ctr].strptr);
        }
      //printf("Deleting user '%s'\n",args[2].strptr);
      //printf("at server '%s'\n",args[1].strptr);
      maintrc = (*pNetUserDelFn)(args[1].strptr,args[2].strptr);
      //maintrc = NetUserDel(args[1].strptr,args[2].strptr);
      retstr->strlength = sprintf(retstr->strptr,"%ld",maintrc);
      return VALID_ROUTINE;
      break;
    case USER_VAL:
      {
      /* Argument list:
         arg[0] - function code
         arg[1] - server name
         arg[2] - userid
         arg[3] - password
      */
      struct user_logon_req_1  *pulr1;
      struct user_logon_info_1 *puli1;
      PCH    pbuf;
      USHORT totavail = 0;
      ULONG  buflen = 0;

      if (numargs < 4)
        return INVALID_ROUTINE;

      if ( !loadAPI("NETAPI","NETUSERVALIDATE2",0,(PFN *)&pNetUserVal2Fn) )
        return INVALID_ROUTINE;

      for (ctr=1;ctr <= 3;ctr++)
        {
        if (args[ctr].strlength == 0)
          return INVALID_ROUTINE;
        strupr(args[ctr].strptr);
        }

      if (args[2].strlength > UNLEN + 1)
        return INVALID_ROUTINE;
      if (args[3].strlength > SESSION_PWLEN)
        return INVALID_ROUTINE;

      buflen =   sizeof(struct user_logon_req_1)
               + sizeof(struct user_logon_info_1);
      pbuf = (PCH) malloc(buflen);

      memset(pbuf,0x00,buflen);
      pulr1 = (struct user_logon_req_1  *) pbuf;
      strcpy(pulr1->usrreq1_name,args[2].strptr);
      strcpy(pulr1->usrreq1_password,args[3].strptr);
      pulr1->usrreq1_workstation = args[1].strptr;
      //printf("Validating user '%s'\n",args[2].strptr);
      //printf("with password '%s'\n",args[3].strptr);
      //printf("from workstation '%s'\n",args[1].strptr);
      maintrc = (*pNetUserVal2Fn)(NULL, 1, pbuf, buflen, 0, &totavail);
      //maintrc = NetUserValidate2(NULL, 1, pbuf, buflen, 0, &totavail);
      puli1 = (struct user_logon_info_1 *) pbuf;
      retstr->strlength = sprintf(retstr->strptr,"%d %d %s %d %x %d %d %ld %ld %ld %ld %ld"
                                 ,maintrc
                                 ,puli1->usrlog1_code
                                 ,puli1->usrlog1_eff_name
                                 ,puli1->usrlog1_priv
                                 ,puli1->usrlog1_auth_flags
                                 ,puli1->usrlog1_num_logons
                                 ,puli1->usrlog1_bad_pw_count
                                 ,puli1->usrlog1_last_logon
                                 ,puli1->usrlog1_last_logoff
                                 ,puli1->usrlog1_logoff_time
                                 ,puli1->usrlog1_kickoff_time
                                 ,puli1->usrlog1_password_age);
      free(pbuf);
      return VALID_ROUTINE;
      break;
      }
    case USER_ENUM:
      {
      /* Argument list:
         arg[0] - function code
         arg[1] - server name
         arg[2] - rexx_stem_name
         arg[3] - userid filter (optional)
         arg[4] - information type (full/username)
      */
      struct user_info_2  *pui2;
      struct user_info_0  *pui0;
      PCH    pbuf;
      USHORT readentries = 0, totentries = 0;
      ULONG  buflen = 0, matched=0;
      SHVBLOCK shvb;
      UCHAR  varname[250], varval[500];
      APIRET rc;
      BOOL   fullinfo=TRUE;
      short  infotype;

      if (numargs < 3)
        return INVALID_ROUTINE;

      if ( !loadAPI("NETAPI","NETUSERENUM",0,(PFN *)&pNetUserEnumFn) )
        return INVALID_ROUTINE;

      for (ctr=1;ctr <= 2;ctr++)
        {
        if (args[ctr].strlength == 0)
          return INVALID_ROUTINE;
        strupr(args[ctr].strptr);
        }

      if (args[2].strlength > 0)
        strupr(args[2].strptr);    // Fold stem name to upper case

      if (numargs > 3)
        {
        if (args[3].strlength)
          strupr(args[3].strptr);
        }

      if (numargs > 4)
        {
        if (args[4].strlength)
          {
          strupr(args[4].strptr);
          switch (*(args[4].strptr))
            {
            case 'F':
              fullinfo = TRUE;
              break;
            case 'U':
              fullinfo = FALSE;
              break;
            default:
              return INVALID_ROUTINE;
              break;
            }
          }
        }

      buflen = 0xffff;
      pbuf = (PCH) malloc(buflen);
      memset(pbuf,0x00,buflen);
      if (fullinfo)
        {
        pui2 = (struct user_info_2 *) pbuf;
        infotype = 2;
        }
      else
        {
        pui0 = (struct user_info_0 *) pbuf;
        infotype = 0;
        }

      maintrc = (*pNetUserEnumFn)(args[1].strptr,infotype,(PCH)pbuf,buflen,
                                  &readentries,&totentries);
      //maintrc = NetUserEnum(args[1].strptr,infotype,(PCH)pbuf,buflen,
      //                      &readentries,&totentries);
      if (maintrc != 0 && maintrc != ERROR_MORE_DATA)
        {
        free(pbuf);
        retstr->strlength = sprintf(retstr->strptr,"%d",maintrc);
        return VALID_ROUTINE;
        }

      shvb.shvnext = (PSHVBLOCK) NULL;

      // Drop stem first
      shvb.shvcode = RXSHV_DROPV;
      shvb.shvname.strptr = args[2].strptr;
      shvb.shvname.strlength = args[2].strlength;
      RexxVariablePool(&shvb);

      // Set stem.0 value
      shvb.shvcode = RXSHV_SET;
      shvb.shvname.strptr = varname;
      shvb.shvvalue.strptr = varval;

      for (ctr=1;ctr <= readentries;ctr++)
        {

        // If userid filter supplied, and this entry does not match
        // the filter, then skip to next entry
        if (numargs > 3 && args[3].strlength > 0)
          {
          if (fullinfo)
            {
            strupr(pui2->usri2_name);
            if ( strncmp(args[3].strptr,pui2->usri2_name,min(strlen(pui2->usri2_name),args[3].strlength)) )
              {
              pui2++;
              continue;
              } // end-if userid doesn't match filter
            } // end-if fullinfo requested
          else
            {
            strupr(pui0->usri0_name);
            if ( strncmp(args[3].strptr,pui0->usri0_name,min(strlen(pui0->usri0_name),args[3].strlength)) )
              {
              pui0++;
              continue;
              } // end-if userid doesn't match filter
            } // end-else fullinfo requested
          } // end-if filter supplied
        matched++;

        // Set userid
        sprintf(varname,"%s%ld.1",args[2].strptr,ctr);
        shvb.shvname.strlength = strlen(varname);
        if (fullinfo)
          shvb.shvvalue.strptr = (PCH) pui2->usri2_name;
        else
          shvb.shvvalue.strptr = (PCH) pui0->usri0_name;
        shvb.shvvalue.strlength = strlen(shvb.shvvalue.strptr);
        RexxVariablePool(&shvb);

        // If only username was requested, go to next user
        if (!fullinfo)
          {
          pui0++;
          continue;
          }

        // Set Privilege Level
        sprintf(varname,"%s%ld.2",args[2].strptr,ctr);
        shvb.shvname.strlength = strlen(varname);
        shvb.shvvalue.strptr = varval;
        switch (pui2->usri2_priv)
          {
          case USER_PRIV_GUEST:
            sprintf(varval,"Guest");
            break;
          case USER_PRIV_USER:
            sprintf(varval,"User");
            break;
          case USER_PRIV_ADMIN:
            sprintf(varval,"Admin");
            break;
          default:
            sprintf(varval,"?");
            break;
          }
        shvb.shvvalue.strlength = strlen(shvb.shvvalue.strptr);
        RexxVariablePool(&shvb);

        // Set Password Age
        sprintf(varname,"%s%ld.3",args[2].strptr,ctr);
        shvb.shvname.strlength = strlen(varname);
        shvb.shvvalue.strptr = varval;
        sprintf(varval,"%ld",pui2->usri2_password_age);
        shvb.shvvalue.strlength = strlen(shvb.shvvalue.strptr);
        RexxVariablePool(&shvb);

        // Set Comment
        sprintf(varname,"%s%ld.4",args[2].strptr,ctr);
        shvb.shvname.strlength = strlen(varname);
        shvb.shvvalue.strptr = pui2->usri2_comment;
        shvb.shvvalue.strlength = strlen(shvb.shvvalue.strptr);
        RexxVariablePool(&shvb);

        // Set Script path
        sprintf(varname,"%s%ld.5",args[2].strptr,ctr);
        shvb.shvname.strlength = strlen(varname);
        shvb.shvvalue.strptr = pui2->usri2_script_path;
        shvb.shvvalue.strlength = strlen(shvb.shvvalue.strptr);
        RexxVariablePool(&shvb);

        // Set fullname
        sprintf(varname,"%s%ld.6",args[2].strptr,ctr);
        shvb.shvname.strlength = strlen(varname);
        shvb.shvvalue.strptr = pui2->usri2_full_name;
        shvb.shvvalue.strlength = strlen(shvb.shvvalue.strptr);
        RexxVariablePool(&shvb);

        // Set user comment
        sprintf(varname,"%s%ld.7",args[2].strptr,ctr);
        shvb.shvname.strlength = strlen(varname);
        shvb.shvvalue.strptr = pui2->usri2_usr_comment;
        shvb.shvvalue.strlength = strlen(shvb.shvvalue.strptr);
        RexxVariablePool(&shvb);

        // Set parms
        sprintf(varname,"%s%ld.8",args[2].strptr,ctr);
        shvb.shvname.strlength = strlen(varname);
        shvb.shvvalue.strptr = pui2->usri2_parms;
        shvb.shvvalue.strlength = strlen(shvb.shvvalue.strptr);
        RexxVariablePool(&shvb);

        // Set logon server
        sprintf(varname,"%s%ld.9",args[2].strptr,ctr);
        shvb.shvname.strlength = strlen(varname);
        shvb.shvvalue.strptr = pui2->usri2_logon_server;
        shvb.shvvalue.strlength = strlen(shvb.shvvalue.strptr);
        RexxVariablePool(&shvb);

        pui2++;
        } // end-for all user entries

      // Set stem.0 value
      shvb.shvvalue.strptr = varval;
      sprintf(varname,"%s0",args[2].strptr);
      shvb.shvname.strlength = strlen(varname);
      if (numargs > 3)
        sprintf(varval,"%ld %d",matched,readentries);
      else
        sprintf(varval,"%d",readentries);
      shvb.shvvalue.strlength = strlen(varval);
      RexxVariablePool(&shvb);

      retstr->strlength = sprintf(retstr->strptr,"%d",maintrc);
      free(pbuf);
      return VALID_ROUTINE;

      break;
      }
    case USER_GETINFO:
      {
      /* Argument list:
         arg[0] - function code
         arg[1] - server name
         arg[2] - rexx_stem_name
         arg[3] - username
      */
      struct user_info_2  *pui2;
      PCH    pbuf;
      USHORT totavail = 0;
      ULONG  buflen = 0;
      SHVBLOCK shvb;
      UCHAR  varname[250], varval[500];
      APIRET rc;

      if (numargs < 4)
        return INVALID_ROUTINE;

      if ( !loadAPI("NETAPI","NETUSERGETINFO",0,(PFN *)&pNetUserGetInfoFn) )
        return INVALID_ROUTINE;

      for (ctr=1;ctr <= 3;ctr++)
        {
        if (args[ctr].strlength == 0)
          return INVALID_ROUTINE;
        strupr(args[ctr].strptr);
        }

      // Fold stem name to uppercase
      strupr(args[2].strptr);

      // Fold username to uppercase
      strupr(args[3].strptr);

      buflen = 0xffff;
      pbuf = (PCH) malloc(buflen);
      memset(pbuf,0x00,buflen);
      pui2 = (struct user_info_2 *) pbuf;

      maintrc = (*pNetUserGetInfoFn)(args[1].strptr,args[3].strptr,2,
                                     (PCH)pui2,buflen,&totavail);
      //maintrc = NetUserGetInfo(args[1].strptr,args[3].strptr,2,(PCH)pui2,buflen,
      //                         &totavail);
      if (maintrc != 0 && maintrc != ERROR_MORE_DATA)
        {
        free(pbuf);
        retstr->strlength = sprintf(retstr->strptr,"%d",maintrc);
        return VALID_ROUTINE;
        }

      shvb.shvnext = (PSHVBLOCK) NULL;

      // Drop stem first
      shvb.shvcode = RXSHV_DROPV;
      shvb.shvname.strptr = args[2].strptr;
      shvb.shvname.strlength = args[2].strlength;
      RexxVariablePool(&shvb);

      // Set stem.0 value
      shvb.shvcode = RXSHV_SET;
      shvb.shvname.strptr = varname;
      shvb.shvvalue.strptr = varval;

      // Set userid
      sprintf(varname,"%s.1",args[2].strptr);
      shvb.shvname.strlength = strlen(varname);
      shvb.shvvalue.strptr = (PCH) pui2->usri2_name;
      shvb.shvvalue.strlength = strlen(shvb.shvvalue.strptr);
      RexxVariablePool(&shvb);

      // Set Privilege Level
      sprintf(varname,"%s.2",args[2].strptr);
      shvb.shvname.strlength = strlen(varname);
      shvb.shvvalue.strptr = varval;
      switch (pui2->usri2_priv)
        {
        case USER_PRIV_GUEST:
          sprintf(varval,"Guest");
          break;
        case USER_PRIV_USER:
          sprintf(varval,"User");
          break;
        case USER_PRIV_ADMIN:
          sprintf(varval,"Admin");
          break;
        default:
          sprintf(varval,"?");
          break;
        }
      shvb.shvvalue.strlength = strlen(shvb.shvvalue.strptr);
      RexxVariablePool(&shvb);

      // Set Password Age
      sprintf(varname,"%s.3",args[2].strptr);
      shvb.shvname.strlength = strlen(varname);
      shvb.shvvalue.strptr = varval;
      sprintf(varval,"%ld",pui2->usri2_password_age);
      shvb.shvvalue.strlength = strlen(shvb.shvvalue.strptr);
      RexxVariablePool(&shvb);

      // Set Comment
      sprintf(varname,"%s.4",args[2].strptr);
      shvb.shvname.strlength = strlen(varname);
      shvb.shvvalue.strptr = pui2->usri2_comment;
      shvb.shvvalue.strlength = strlen(shvb.shvvalue.strptr);
      RexxVariablePool(&shvb);

      // Set Script path
      sprintf(varname,"%s.5",args[2].strptr);
      shvb.shvname.strlength = strlen(varname);
      shvb.shvvalue.strptr = pui2->usri2_script_path;
      shvb.shvvalue.strlength = strlen(shvb.shvvalue.strptr);
      RexxVariablePool(&shvb);

      // Set fullname
      sprintf(varname,"%s.6",args[2].strptr);
      shvb.shvname.strlength = strlen(varname);
      shvb.shvvalue.strptr = pui2->usri2_full_name;
      shvb.shvvalue.strlength = strlen(shvb.shvvalue.strptr);
      RexxVariablePool(&shvb);

      // Set user comment
      sprintf(varname,"%s.7",args[2].strptr);
      shvb.shvname.strlength = strlen(varname);
      shvb.shvvalue.strptr = pui2->usri2_usr_comment;
      shvb.shvvalue.strlength = strlen(shvb.shvvalue.strptr);
      RexxVariablePool(&shvb);

      // Set parms
      sprintf(varname,"%s.8",args[2].strptr);
      shvb.shvname.strlength = strlen(varname);
      shvb.shvvalue.strptr = pui2->usri2_parms;
      shvb.shvvalue.strlength = strlen(shvb.shvvalue.strptr);
      RexxVariablePool(&shvb);

      // Set logon server
      sprintf(varname,"%s.9",args[2].strptr);
      shvb.shvname.strlength = strlen(varname);
      shvb.shvvalue.strptr = pui2->usri2_logon_server;
      shvb.shvvalue.strlength = strlen(shvb.shvvalue.strptr);
      RexxVariablePool(&shvb);

      retstr->strlength = sprintf(retstr->strptr,"%d",maintrc);
      free(pbuf);
      return VALID_ROUTINE;

      break;
      }
    case LOGON_ENUM:
      {
      /* Argument list:
         arg[0] - function code
         arg[1] - server name
         arg[2] - rexx_stem_name
      */
      struct user_logon_info_0  *puli0;
      PCH    pbuf;
      USHORT readentries = 0, totentries = 0;
      ULONG  buflen = 0;
      SHVBLOCK shvb;
      UCHAR  varname[250], varval[250];
      APIRET rc;

      if (numargs < 3)
        return INVALID_ROUTINE;

      if ( !loadAPI("NETAPI","NETLOGONENUM",0,(PFN *)&pNetLogonEnumFn) )
        return INVALID_ROUTINE;

      for (ctr=1;ctr <= 2;ctr++)
        {
        if (args[ctr].strlength == 0)
          return INVALID_ROUTINE;
        strupr(args[ctr].strptr);
        }

      if (args[2].strlength > 0)
        strupr(args[2].strptr);       // Fold Rexx stem name to upper case

      buflen = 65500;
      pbuf = (PCH) malloc(buflen);
      memset(pbuf,0x00,buflen);
      puli0 = (struct user_logon_info_0 *) pbuf;

      maintrc = (*pNetLogonEnumFn)(args[1].strptr,0,(PCH) puli0,buflen,
                                   &readentries,&totentries);
      //maintrc = NetLogonEnum(args[1].strptr,0,(PCH) puli0,buflen,
      //                       &readentries,&totentries);
      if (maintrc != 0 && maintrc != ERROR_MORE_DATA)
        {
        free(pbuf);
        retstr->strlength = sprintf(retstr->strptr,"%d",maintrc);
        return VALID_ROUTINE;
        }

      shvb.shvnext = (PSHVBLOCK) NULL;

      // Drop stem first
      shvb.shvcode = RXSHV_DROPV;
      shvb.shvname.strptr = args[2].strptr;
      shvb.shvname.strlength = args[2].strlength;
      RexxVariablePool(&shvb);

      // Set stem.0 value
      shvb.shvcode = RXSHV_SET;
      shvb.shvname.strptr = varname;
      sprintf(varname,"%s0",args[2].strptr);
      shvb.shvname.strlength = strlen(varname);
      shvb.shvvalue.strptr = varval;
      sprintf(varval,"%d",readentries);
      shvb.shvvalue.strlength = strlen(varval);
      rc = RexxVariablePool(&shvb);
      if (rc != 0 && shvb.shvret != RXSHV_NEWV)
        {
        free(pbuf);
        return INVALID_ROUTINE;
        }

      for (ctr=1;ctr <= readentries;ctr++)
        {
        // Set userid
        sprintf(varname,"%s%ld.1",args[2].strptr,ctr);
        shvb.shvname.strlength = strlen(varname);
        shvb.shvvalue.strptr = (PCH) puli0->usrlog0_eff_name;
        shvb.shvvalue.strlength = strlen(shvb.shvvalue.strptr);
        rc = RexxVariablePool(&shvb);

        puli0++;
        }
      retstr->strlength = sprintf(retstr->strptr,"%d",maintrc);
      free(pbuf);
      return VALID_ROUTINE;
      break;
      } // logon_enum
    case FILE_ENUM:
      {
      /* Argument list:
         arg[0] - function code
         arg[1] - server name
         arg[2] - basepath
         arg[3] - username
         arg[4] - rexx_stem_name
      */
      struct file_info_3  *pfi3;
      PCH    pbuf, pbasepath, pusername;
      USHORT readentries = 0, totentries = 0;
      ULONG  buflen = 0, ctr2;
      SHVBLOCK shvb;
      UCHAR  varname[250], varval[250];
      FRK    reskey;
      APIRET rc;

      if (numargs < 5)
        return INVALID_ROUTINE;

      // Fix up servername and rexxstem
      if (args[1].strlength == 0 || args[4].strlength == 0)
        return INVALID_ROUTINE;

      if ( !loadAPI("NETAPI","NETFILEENUM2",0,(PFN *)&pNetFileEnum2Fn) )
        return INVALID_ROUTINE;

      strupr(args[1].strptr);
      strupr(args[4].strptr);

      // Fix up basepath and username
      pbasepath = pusername = NULL;
      if (args[2].strlength > 0)
        pbasepath = args[2].strptr;
      if (args[3].strlength > 0)
        pusername = args[3].strptr;

      buflen = 65500;
      pbuf = (PCH) malloc(buflen);
      memset(pbuf,0x00,buflen);
      pfi3 = (struct file_info_3 *) pbuf;
      FRK_INIT(reskey);

      maintrc = (*pNetFileEnum2Fn)(args[1].strptr,
                                   pbasepath,
                                   pusername,
                                   (short) 3,
                                   (PCH) pfi3, buflen,
                                   &readentries,&totentries,&reskey);
      //maintrc = NetFileEnum2(args[1].strptr,
      //                       pbasepath,
      //                       pusername,
      //                       (short) 3,
      //                       (PCH) pfi3, buflen,
      //                       &readentries,&totentries,&reskey);
      if (maintrc != 0 && maintrc != ERROR_MORE_DATA)
        {
        free(pbuf);
        retstr->strlength = sprintf(retstr->strptr,"%d",maintrc);
        return VALID_ROUTINE;
        }

      shvb.shvnext = (PSHVBLOCK) NULL;

      // Drop stem first
      shvb.shvcode = RXSHV_DROPV;
      shvb.shvname.strptr = args[4].strptr;
      shvb.shvname.strlength = args[4].strlength;
      RexxVariablePool(&shvb);

      // Set stem.0 value
      shvb.shvcode = RXSHV_SET;
      shvb.shvname.strptr = varname;
      sprintf(varname,"%s0",args[4].strptr);
      shvb.shvname.strlength = strlen(varname);
      shvb.shvvalue.strptr = varval;
      sprintf(varval,"%d",readentries);
      shvb.shvvalue.strlength = strlen(varval);
      rc = RexxVariablePool(&shvb);
      if (rc != 0 && shvb.shvret != RXSHV_NEWV)
        {
        free(pbuf);
        return INVALID_ROUTINE;
        }

      ctr = 1;
      //printf("Total entries(%ld) = %ld\n",ctr,totentries);
      //printf("Read  entries(%ld) = %ld\n",ctr,readentries);
      while ( ctr <= readentries )
        {
        //printf("Total entries(%ld) = %ld\n",ctr,totentries);
        //printf("Read  entries(%ld) = %ld\n",ctr,readentries);
        for (ctr2=1;ctr2 <= readentries;ctr2++)
          {

          // Set file_id
          sprintf(varname,"%s%ld.1",args[4].strptr,ctr);
          shvb.shvname.strlength = strlen(varname);
          shvb.shvvalue.strptr = varval;
          sprintf(varval,"%d",pfi3->fi3_id);
          shvb.shvvalue.strlength = strlen(shvb.shvvalue.strptr);
          rc = RexxVariablePool(&shvb);

          // Set permissions
          sprintf(varname,"%s%ld.2",args[4].strptr,ctr);
          shvb.shvname.strlength = strlen(varname);
          shvb.shvvalue.strptr = varval;
          memset(varval,0x00,sizeof(varval));
          if (pfi3->fi3_permissions & PERM_FILE_READ)
            strcat(varval,"R");
          if (pfi3->fi3_permissions & PERM_FILE_WRITE)
            strcat(varval,"W");
          if (pfi3->fi3_permissions & PERM_FILE_CREATE)
            strcat(varval,"C");
          shvb.shvvalue.strlength = strlen(shvb.shvvalue.strptr);
          rc = RexxVariablePool(&shvb);

          // Set num_locks
          sprintf(varname,"%s%ld.3",args[4].strptr,ctr);
          shvb.shvname.strlength = strlen(varname);
          shvb.shvvalue.strptr = varval;
          sprintf(varval,"%d",pfi3->fi3_num_locks);
          shvb.shvvalue.strlength = strlen(shvb.shvvalue.strptr);
          rc = RexxVariablePool(&shvb);

          // Set pathname
          sprintf(varname,"%s%ld.4",args[4].strptr,ctr);
          shvb.shvname.strlength = strlen(varname);
          shvb.shvvalue.strptr = pfi3->fi3_pathname;
          shvb.shvvalue.strlength = strlen(shvb.shvvalue.strptr);
          rc = RexxVariablePool(&shvb);

          // Set username
          sprintf(varname,"%s%ld.5",args[4].strptr,ctr);
          shvb.shvname.strlength = strlen(varname);
          shvb.shvvalue.strptr = pfi3->fi3_username;
          shvb.shvvalue.strlength = strlen(shvb.shvvalue.strptr);
          rc = RexxVariablePool(&shvb);

          ctr++;
          pfi3++;
          } // end for

        memset(pbuf,0x00,buflen);
        pfi3 = (struct file_info_3 *) pbuf;
        maintrc = (*pNetFileEnum2Fn)(args[1].strptr,
                                     pbasepath,
                                     pusername,
                                     (short) 3,
                                     (PCH) pfi3, buflen,
                                     &readentries,&totentries,&reskey);
        //maintrc = NetFileEnum2(args[1].strptr,
        //                       pbasepath,
        //                       pusername,
        //                       (short) 3,
        //                       (PCH) pfi3, buflen,
        //                       &readentries,&totentries,&reskey);
        if (maintrc != 0 && maintrc != ERROR_MORE_DATA)
          {
          free(pbuf);
          retstr->strlength = sprintf(retstr->strptr,"%d",maintrc);
          return VALID_ROUTINE;
          } // end if
        } // end while
      retstr->strlength = sprintf(retstr->strptr,"%d",maintrc);
      free(pbuf);
      return VALID_ROUTINE;
      break;
      } // end case
    case USE_ADD:
      {
      /* Argument list:
         arg[0] - function code
         arg[1] - server name
         arg[2] - local device name
         arg[3] - remote name
         arg[4] - password
      */
      struct use_info_1 ui1;

      if (numargs < 4)
        return INVALID_ROUTINE;
      if (args[2].strlength > DEVLEN+1)
        return INVALID_ROUTINE;
      if (numargs >= 5)
        {
        if (args[4].strlength > PWLEN)
          return INVALID_ROUTINE;
        }

      if ( !loadAPI("NETAPI","NETUSEADD",0,(PFN *)&pNetUseAddFn) )
        return INVALID_ROUTINE;

      for (ctr=1;ctr <= 3;ctr++)
        {
        if (args[ctr].strlength == 0)
          return INVALID_ROUTINE;
        strupr(args[ctr].strptr);
        }
      memset(&ui1,0x00,sizeof(ui1));
      strcpy(ui1.ui1_local,args[2].strptr);
      ui1.ui1_remote = args[3].strptr;
      if (numargs >= 5)
        ui1.ui1_password = args[4].strptr;
      pservername = args[1].strptr;
      maintrc = (*pNetUseAddFn)(pservername,1,(PCH) &ui1,sizeof(ui1));
      //maintrc = NetUseAdd(pservername,1,(PCH) &ui1,sizeof(ui1));
      retstr->strlength = sprintf(retstr->strptr,"%d %d %d %d %d",maintrc,ui1.ui1_status,
                                  ui1.ui1_asg_type,ui1.ui1_refcount,ui1.ui1_usecount);
      return VALID_ROUTINE;
      break;
      } // use-add
    case USE_DEL:
      {
      /* Argument list:
         arg[0] - function code
         arg[1] - server name
         arg[2] - local device name
         arg[3] - force flag ('F'orce, 'N'oforce, 'M'ax Force)
      */
      USHORT       usForceFlag;

      if (numargs < 4)
        return INVALID_ROUTINE;
      if (args[2].strlength > DEVLEN+1)
        return INVALID_ROUTINE;

      if ( !loadAPI("NETAPI","NETUSEDEL",0,(PFN *)&pNetUseDelFn) )
        return INVALID_ROUTINE;

      for (ctr=1;ctr <= 3;ctr++)
        {
        if (args[ctr].strlength == 0)
          return INVALID_ROUTINE;
        strupr(args[ctr].strptr);
        }
      switch (*args[3].strptr)
        {
        case 'F':
          usForceFlag = USE_FORCE;
          break;
        case 'N':
          usForceFlag = USE_NOFORCE;
          break;
        case 'M':
          usForceFlag = USE_LOTS_OF_FORCE;
          break;
        default:
          return INVALID_ROUTINE;
          break;
        }
      pservername = args[1].strptr;
      //printf("Deleting device '%s'\n",args[2].strptr);
      //printf("at server '%s'\n",args[1].strptr);
      maintrc = (*pNetUseDelFn)(pservername,args[2].strptr,usForceFlag);
      //maintrc = NetUseDel(pservername,args[2].strptr,usForceFlag);
      retstr->strlength = sprintf(retstr->strptr,"%d",maintrc);
      return VALID_ROUTINE;
      break;
      } // use-del
    case USE_ENUM:
      {
      /* Argument list:
         arg[0] - function code
         arg[1] - server name
         arg[2] - rexx_stem_name
      */
      struct use_info_1  *pui1;
      PCH    pbuf;
      USHORT readentries = 0, totentries = 0;
      ULONG  buflen = 0;
      SHVBLOCK shvb;
      UCHAR  varname[250], varval[250];
      APIRET rc;

      if (numargs < 3)
        return INVALID_ROUTINE;

      if ( !loadAPI("NETAPI","NETUSEENUM",0,(PFN *)&pNetUseEnumFn) )
        return INVALID_ROUTINE;

      for (ctr=1;ctr <= 2;ctr++)
        {
        if (args[ctr].strlength == 0)
          return INVALID_ROUTINE;
        strupr(args[ctr].strptr);
        }

      if (args[2].strlength > 0)
        strupr(args[2].strptr);       // Fold Rexx stem name to upper case

      buflen = 65500;
      pbuf = (PCH) malloc(buflen);
      memset(pbuf,0x00,buflen);
      pui1 = (struct use_info_1 *) pbuf;

      maintrc = (*pNetUseEnumFn)(args[1].strptr,1,(PCH)pui1,buflen,
                                 &readentries,&totentries);
      //maintrc = NetUseEnum(args[1].strptr,1,(PCH) pui1,buflen,
      //                     &readentries,&totentries);
      if (maintrc != 0 && maintrc != ERROR_MORE_DATA)
        {
        free(pbuf);
        retstr->strlength = sprintf(retstr->strptr,"%d",maintrc);
        return VALID_ROUTINE;
        }

      shvb.shvnext = (PSHVBLOCK) NULL;

      // Drop stem first
      shvb.shvcode = RXSHV_DROPV;
      shvb.shvname.strptr = args[2].strptr;
      shvb.shvname.strlength = args[2].strlength;
      RexxVariablePool(&shvb);

      // Set stem.0 value
      shvb.shvcode = RXSHV_SET;
      shvb.shvname.strptr = varname;
      sprintf(varname,"%s0",args[2].strptr);
      shvb.shvname.strlength = strlen(varname);
      shvb.shvvalue.strptr = varval;
      sprintf(varval,"%d",readentries);
      shvb.shvvalue.strlength = strlen(varval);
      rc = RexxVariablePool(&shvb);
      if (rc != 0 && shvb.shvret != RXSHV_NEWV)
        {
        free(pbuf);
        return INVALID_ROUTINE;
        }

      for (ctr=1;ctr <= readentries;ctr++)
        {
        // Set device name
        sprintf(varname,"%s%ld.1",args[2].strptr,ctr);
        shvb.shvname.strlength = strlen(varname);
        shvb.shvvalue.strptr = (PCH)pui1->ui1_local;
        shvb.shvvalue.strlength = strlen(shvb.shvvalue.strptr);
        rc = RexxVariablePool(&shvb);

        // Set remote name
        sprintf(varname,"%s%ld.2",args[2].strptr,ctr);
        shvb.shvname.strlength = strlen(varname);
        shvb.shvvalue.strptr = (PCH)pui1->ui1_remote;
        shvb.shvvalue.strlength = strlen(shvb.shvvalue.strptr);
        rc = RexxVariablePool(&shvb);

        // Set status
        sprintf(varname,"%s%ld.3",args[2].strptr,ctr);
        shvb.shvname.strlength = strlen(varname);
        shvb.shvvalue.strptr = varval;
        switch (pui1->ui1_status)
          {
          case USE_OK:
            sprintf(varval,"Ok");
            break;
          case USE_PAUSED:
            sprintf(varval,"Paused");
            break;
          case USE_DISCONN:
            // (a.k.a. "case USE_SESSLOST:")
            sprintf(varval,"Disconnected/SessionLost");
            break;
          case USE_NETERR:
            sprintf(varval,"NetError");
            break;
          case USE_CONN:
            sprintf(varval,"Connecting");
            break;
          case USE_RECONN:
            sprintf(varval,"Reconnecting");
            break;
          default:
            sprintf(varval,"?");
            break;
          }
        shvb.shvvalue.strlength = strlen(shvb.shvvalue.strptr);
        rc = RexxVariablePool(&shvb);

        // Set asg_type
        sprintf(varname,"%s%ld.4",args[2].strptr,ctr);
        shvb.shvname.strlength = strlen(varname);
        shvb.shvvalue.strptr = varval;
        switch (pui1->ui1_asg_type)
          {
          case USE_WILDCARD:
            sprintf(varval,"WildCard");
            break;
          case USE_DISKDEV:
            sprintf(varval,"DiskDevice");
            break;
          case USE_SPOOLDEV:
            sprintf(varval,"SpooledPrinter");
            break;
          case USE_CHARDEV:
            sprintf(varval,"SerialDevice");
            break;
          case USE_IPC:
            sprintf(varval,"IPC");
            break;
          default:
            sprintf(varval,"?");
            break;
          }
        shvb.shvvalue.strlength = strlen(shvb.shvvalue.strptr);
        rc = RexxVariablePool(&shvb);

        // Set refcount
        sprintf(varname,"%s%ld.5",args[2].strptr,ctr);
        shvb.shvname.strlength = strlen(varname);
        shvb.shvvalue.strptr = varval;
        sprintf(varval,"%d",pui1->ui1_refcount);
        shvb.shvvalue.strlength = strlen(shvb.shvvalue.strptr);
        rc = RexxVariablePool(&shvb);

        // Set usecount
        sprintf(varname,"%s%ld.6",args[2].strptr,ctr);
        shvb.shvname.strlength = strlen(varname);
        shvb.shvvalue.strptr = varval;
        sprintf(varval,"%d",pui1->ui1_usecount);
        shvb.shvvalue.strlength = strlen(shvb.shvvalue.strptr);
        rc = RexxVariablePool(&shvb);

        pui1++;
        }
      retstr->strlength = sprintf(retstr->strptr,"%d",maintrc);
      free(pbuf);
      return VALID_ROUTINE;
      break;
      } // use_enum
    default:
      return INVALID_ROUTINE;
      break;
    } // end switch

  return INVALID_ROUTINE;

}

// Rexx RxWinQueryObject external function
EXPORTSPEC ULONG APIENTRY  RxWinQueryObject(PUCHAR name, ULONG numargs, RXSTRING args[],
                        PSZ queuename, PRXSTRING retstr)
{

  HOBJECT hobject;

  if (numargs < 1)
    return INVALID_ROUTINE;
  if (args[0].strlength == 0)
    return INVALID_ROUTINE;

  hobject = WinQueryObject(args[0].strptr);
  if (hobject == NULLHANDLE)
    {
    retstr->strlength = 0;
    return VALID_ROUTINE;
    }

  memcpy(retstr->strptr,&hobject,sizeof(hobject));
  retstr->strlength = sizeof(hobject);
  return VALID_ROUTINE;
}

// Rexx RxWinSetPresParam external function
EXPORTSPEC ULONG APIENTRY  RxWinSetPresParam(PUCHAR name, ULONG numargs, RXSTRING args[],
                         PSZ queuename, PRXSTRING retstr)
{

  ULONG  ulAttr;
  ULONG  ulValue;
  HWND   hwnd;
  APIRET rc;

  if (numargs < 3)
    return INVALID_ROUTINE;
  if (args[0].strlength != sizeof(HWND))
    return INVALID_ROUTINE;
  if (args[1].strlength == 0 ||
      args[2].strlength == 0)
    return INVALID_ROUTINE;

  memcpy(&hwnd,args[0].strptr,sizeof(HWND));
  ulAttr  = atoi(args[0].strptr);
  ulValue = atoi(args[1].strptr);

  rc=WinSetPresParam(hwnd,ulAttr,sizeof(ulValue),&ulValue);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);
  return VALID_ROUTINE;

}

// Rexx RxWinDestroyObject external function
EXPORTSPEC ULONG APIENTRY  RxWinDestroyObject(PUCHAR name, ULONG numargs, RXSTRING args[],
                          PSZ queuename, PRXSTRING retstr)
{

  HOBJECT hobject;
  BOOL    fSuccess;

  if (numargs < 1)
    return INVALID_ROUTINE;
  if (args[0].strlength != sizeof(hobject))
    return INVALID_ROUTINE;
  memcpy(&hobject,args[0].strptr,args[0].strlength);

  fSuccess = WinDestroyObject(hobject);
  if (fSuccess)
    *retstr->strptr = '1';
  else
    *retstr->strptr = '0';
  retstr->strlength = 1;
  return VALID_ROUTINE;
}

#endif  // BLD4OS2

// Rexx RXPROCID external function
EXPORTSPEC ULONG APIENTRY  RxProcId(PUCHAR name, ULONG numargs, RXSTRING args[],
                PSZ queuename, PRXSTRING retstr)
{
#if defined(__BLD4OS2__)
  PTIB  ptib;  // Address of pointer to TIB
  PPIB  ppib;  // Address of pointer to PIB
  APIRET  rc;  // Return code
  rc = DosGetInfoBlocks(&ptib, &ppib);
  retstr->strlength = sprintf(retstr->strptr,"%ld %ld %ld",
                              ppib->pib_ulpid,
                              ppib->pib_ulppid,
                              ptib->tib_ptib2->tib2_ultid);
#endif  // BLD4OS2

#if defined(__BLD4NT__)
  retstr->strlength = sprintf(retstr->strptr,"%ld %ld %ld",
                              GetCurrentProcessId(),
                              0,
                              GetCurrentThreadId());
#endif

  return VALID_ROUTINE;
}

// Rexx RXREGISTERFUNCTIONEXE external function
EXPORTSPEC ULONG APIENTRY  RxRegisterFunctionExe(PUCHAR name, ULONG numargs, RXSTRING args[],
                             PSZ queuename, PRXSTRING retstr)
{

  APIRET   rc;
  PFN      pfunc;

  // Arg #1 - Function Name
  // Arg #2 - Function Address

  if (numargs < 2)
    return INVALID_ROUTINE;

  if ( args[1].strlength != sizeof(PFN) )
    return INVALID_ROUTINE;

  strupr(args[0].strptr);
  memcpy(&pfunc,args[1].strptr,args[1].strlength);

  rc = RexxRegisterFunctionExe(args[0].strptr,pfunc);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;

}

// Rexx RXREGISTEREXITDLL external function
EXPORTSPEC ULONG APIENTRY  RxRegisterExitDll(PUCHAR name, ULONG numargs, RXSTRING args[],
                         PSZ queuename, PRXSTRING retstr)
{

  APIRET   rc;
  PCH      pudata=(PCH)NULL;
  UCHAR    udata[8];
  ULONG    ctr;
  ULONG    dropflag = RXEXIT_DROPPABLE;

  // Arg #1 - ExitHandler Name
  // Arg #2 - ExitHandler DLL name
  // Arg #3 - ExitHandler Procedure name
  // Arg #4 - User data
  // Arg #5 - Drop Flag

  if (numargs < 3)
    return INVALID_ROUTINE;

  for (ctr=0;ctr<3;ctr++)
    strupr(args[ctr].strptr);

  if (numargs >= 4)
    {
    if (args[3].strlength > sizeof(udata))
      return INVALID_ROUTINE;
    memset(udata,0x00,sizeof(udata));
    memcpy(udata,args[3].strptr,args[3].strlength);
    pudata = udata;
    }

  if (numargs >= 5)
    {
    strupr(args[4].strptr);
    switch (*args[4].strptr)
      {
      case 'D':
        dropflag = RXEXIT_DROPPABLE;
        break;
      case 'N':
        dropflag = RXEXIT_NONDROP;
        break;
      default:
        return INVALID_ROUTINE;
        break;
      }
    }

  rc = RexxRegisterExitDll(args[0].strptr,args[1].strptr,
                           args[2].strptr,pudata,dropflag);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;

}

// Rexx RXREGISTEREXITEXE external function
EXPORTSPEC ULONG APIENTRY  RxRegisterExitExe(PUCHAR name, ULONG numargs, RXSTRING args[],
                         PSZ queuename, PRXSTRING retstr)
{

  APIRET   rc;
  PFN      pfunc;
  PCH      pudata=(PCH)NULL;
  UCHAR    udata[8];

  // Arg #1 - ExitHandler Name
  // Arg #2 - ExitHandler Address
  // Arg #3 - User data

  if (numargs < 2)
    return INVALID_ROUTINE;

  if ( args[1].strlength != sizeof(PFN) )
    return INVALID_ROUTINE;

  strupr(args[0].strptr);
  memcpy(&pfunc,args[1].strptr,args[1].strlength);
  if (numargs >= 3)
    {
    memset(udata,0x00,sizeof(udata));
    if (args[2].strlength > sizeof(udata))
      return INVALID_ROUTINE;
    memcpy(udata,args[2].strptr,args[2].strlength);
    pudata = udata;
    }

  rc = RexxRegisterExitExe(args[0].strptr,pfunc,pudata);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;

}

// Rexx RXDEREGISTEREXIT external function
EXPORTSPEC ULONG APIENTRY  RxDeregisterExit(PUCHAR name, ULONG numargs, RXSTRING args[],
                        PSZ queuename, PRXSTRING retstr)
{

  APIRET   rc;
  PCH      pmodname=(PCH)NULL;

  // Arg #1 - ExitHandler Name
  // Arg #2 - ExitHandler Module name

  if (numargs < 1)
    return INVALID_ROUTINE;

  strupr(args[0].strptr);

  if (numargs >=2 )
    {
    pmodname = args[1].strptr;
    strupr(pmodname);
    }

  rc = RexxDeregisterExit(args[0].strptr,pmodname);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;

}

// Rexx RXQUERYEXIT external function
EXPORTSPEC ULONG APIENTRY  RxQueryExit(PUCHAR name, ULONG numargs, RXSTRING args[],
                   PSZ queuename, PRXSTRING retstr)
{

  APIRET   rc;
  USHORT   flags=0;
  UCHAR    udata[8];
  PCH      pmodname=(PCH)NULL;

  // Arg #1 - ExitHandler Name
  // Arg #2 - ExitHandler Module Name

  if (numargs < 1)
    return INVALID_ROUTINE;

  strupr(args[0].strptr);

  if ( numargs >= 2 )
    {
    strupr(args[1].strptr);
    pmodname = args[1].strptr;
    }

  rc = RexxQueryExit(args[0].strptr,pmodname,&flags,udata);

  if (rc)
    retstr->strlength = sprintf(retstr->strptr,"%ld",rc);
  else
    retstr->strlength = sprintf(retstr->strptr,"%ld %04x %16x",rc,flags,udata);

  return VALID_ROUTINE;

}

#if defined(__BLD4OS2__)

// Rexx RXPULLQUEUE external function
EXPORTSPEC ULONG APIENTRY  RxPullQueue(PUCHAR name, ULONG numargs, RXSTRING args[],
                   PSZ queuename, PRXSTRING retstr)
{

  APIRET   rc;
  RXSTRING retval;
  DATETIME rxdatetime;
  PCH      qname;
  BOOL     type, gettime;
  SHVBLOCK shvb;
  UCHAR    dtbuf[100];

  // Validate first argument (queue name)
  qname = queuename;
  if (numargs >= 1)
    {
    if (args[0].strlength > 0)
      qname = args[0].strptr;
    }

  // Validate second argument (type)
  type = RXQUEUE_WAIT;
  if (numargs >= 2)
    if (args[1].strlength > 0)
      {
      switch(*(args[1].strptr))
        {
        case 'w':
        case 'W':
          type = RXQUEUE_WAIT;
          break;
        case 'n':
        case 'N':
          type = RXQUEUE_NOWAIT;
          break;
        default:
          return INVALID_ROUTINE;
          break;
        }
      }

  // Validate third argument (rexx-var for time)
  gettime = FALSE;
  if (numargs >= 3)
    {
    if (args[2].strlength > 0)
      {
      strupr(args[2].strptr);
      gettime = TRUE;
      }
    }

  MAKERXSTRING(retval,NULL,0);
  rc = RexxPullQueue(qname,(PRXSTRING) &retval, &rxdatetime, type);
  if (rc)
    {
    retstr->strlength = 0;
    return VALID_ROUTINE;
    }

  if (gettime)
    {
    // Put queue-element-insertion-timestamp into variable
    sprintf(dtbuf,"%.4d-%.2d-%.2d-%.2d.%.2d.%.2d.%.2d0000"
                 ,rxdatetime.year
                 ,rxdatetime.month
                 ,rxdatetime.day
                 ,rxdatetime.hours
                 ,rxdatetime.minutes
                 ,rxdatetime.seconds
                 ,rxdatetime.hundredths);
    shvb.shvnext = (PSHVBLOCK) NULL;
    shvb.shvcode = RXSHV_SET;
    shvb.shvname.strptr    = args[2].strptr;
    shvb.shvname.strlength = args[2].strlength;
    shvb.shvvalue.strptr = dtbuf;
    shvb.shvvalue.strlength = strlen(dtbuf);
    RexxVariablePool(&shvb);
    }

  DosFreeMem(retstr->strptr);
  retstr->strptr = retval.strptr;
  retstr->strlength = retval.strlength;
  //if (retval.strlength > 250)
  //  {
  //  DosFreeMem(retstr->strptr);
  //  rc = DosAllocMem((PVOID) &(retstr->strptr), retval.strlength,
  //                   PAG_COMMIT | PAG_WRITE);
  //  if (rc != 0)
  //    return INVALID_ROUTINE;
  //  }
  //memcpy(retstr->strptr,retval.strptr,retval.strlength);
  //retstr->strlength = retval.strlength;

  return VALID_ROUTINE;

}

// Rexx RXADDQUEUE external function
EXPORTSPEC ULONG APIENTRY  RxAddQueue(PUCHAR name, ULONG numargs, RXSTRING args[],
                  PSZ queuename, PRXSTRING retstr)
{

  APIRET rc;
  RXSTRING retval;
  PCH      qname;
  BOOL     type;

  if (numargs < 1)
    return INVALID_ROUTINE;

  // Validate second argument (queue name)
  qname = queuename;
  if (numargs >= 2)
    {
    if (args[1].strlength > 0)
      qname = args[1].strptr;
    }

  // Validate third argument (Queue or Push)
  type = RXQUEUE_FIFO;
  if (numargs >= 3)
    if (args[2].strlength > 0)
      {
      switch(*(args[2].strptr))
        {
        case 'q':
        case 'Q':
          type = RXQUEUE_FIFO;
          break;
        case 'p':
        case 'P':
          type = RXQUEUE_LIFO;
          break;
        default:
          return INVALID_ROUTINE;
          break;
        }
      }

  rc = RexxAddQueue(qname, (PRXSTRING)&args[0], (BOOL)type);
  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;

}

// Rexx RXQUEUED external function
EXPORTSPEC ULONG APIENTRY  RxQueued(PUCHAR name, ULONG numargs, RXSTRING args[],
                PSZ queuename, PRXSTRING retstr)
{

  APIRET rc;
  PCH    qname;
  ULONG  qcount = 0;

  // Validate first argument (queue name)
  qname = queuename;
  if (numargs >= 1)
    {
    if (args[0].strlength > 0)
      qname = args[0].strptr;
    }

  rc = RexxQueryQueue(qname, (PULONG) &qcount);
  if (rc != 0)
    qcount = 0;
  retstr->strlength = sprintf(retstr->strptr,"%ld",qcount);

  return VALID_ROUTINE;

}

// Rexx RXQEXISTS external function
EXPORTSPEC ULONG APIENTRY  RxQExists(PUCHAR name, ULONG numargs, RXSTRING args[],
                 PSZ queuename, PRXSTRING retstr)
{

  APIRET rc;
  PCH    qname;
  ULONG  qcount = 0;

  // Validate first argument (queue name)
  qname = queuename;
  if (numargs >= 1)
    {
    if (args[0].strlength > 0)
      qname = args[0].strptr;
    }

  rc = RexxQueryQueue(qname, (PULONG) &qcount);
  if (rc)
    *retstr->strptr = '0';
  else
    *retstr->strptr = '1';
  retstr->strlength = 1;

  return VALID_ROUTINE;

}

// Rexx RXGLOBALVAR external function
EXPORTSPEC ULONG APIENTRY  RxGlobalVar(PUCHAR name, ULONG numargs, RXSTRING args[],
                   PSZ queuename, PRXSTRING retstr)
{

  APIRET   rc;
  UCHAR    varqname[251];
  ULONG    qcount=0;
  BOOL     qok=FALSE;
  ULONG    dupname=0;
  UCHAR    createdname[251];
  DATETIME dt;
  RXSTRING qelem;

  // Arg#1 - 'Get', 'Put', 'Delete'
  // Arg#2 - Variable name (240 chars. max)
  // Arg#3 - Variable value (for put only)

  if (numargs < 2)
    return INVALID_ROUTINE;
  if (!args[0].strlength)
    return INVALID_ROUTINE;
  if (!args[1].strlength || args[1].strlength > 240)
    return INVALID_ROUTINE;
  retstr->strlength=0;
  strupr(args[1].strptr);
  sprintf(varqname,"RXUGV%sRXUGV",args[1].strptr);

  // First, serialize
  if (hmtxRxGlobalVar)
    {
#if defined(__BLD4OS2__)
    rc=DosRequestMutexSem(hmtxRxGlobalVar,SEM_INDEFINITE_WAIT);
#endif  // BLD4OS2
    if (rc)
      return VALID_ROUTINE;
    }
  else
    {
#if defined(__BLD4OS2__)
    rc=DosOpenMutexSem(RxGlobalVarSemName,&hmtxRxGlobalVar);
#endif  // BLD4OS2
#if defined(__BLD4NT__)
    hmtxRxGlobalVar = OpenMutex(MUTEX_ALL_ACCESS,FALSE,RxGlobalVarSemName);
#endif  // BLD4NT
    switch (rc)
      {
      case NO_ERROR:
#if defined(__BLD4OS2__)
        rc=DosRequestMutexSem(hmtxRxGlobalVar,SEM_INDEFINITE_WAIT);
        if (rc)
#endif  // BLD4OS2
          return VALID_ROUTINE;
        break;
      case ERROR_SEM_NOT_FOUND:
#if defined(__BLD4OS2__)
        rc=DosCreateMutexSem(RxGlobalVarSemName,&hmtxRxGlobalVar,0,TRUE);
#endif  // BLD4OS2
#if defined(__BLD4NT__)
        hmtxRxGlobalVar = CreateMutex(NULL,TRUE,RxGlobalVarSemName);
#endif  // BLD4NT
        switch (rc)
          {
          case NO_ERROR:
            break;
          case ERROR_DUPLICATE_NAME:
            rc=DosOpenMutexSem(RxGlobalVarSemName,&hmtxRxGlobalVar);
            if (rc)
              return VALID_ROUTINE;
            rc=DosRequestMutexSem(hmtxRxGlobalVar,SEM_INDEFINITE_WAIT);
            if (rc)
              return VALID_ROUTINE;
            break;
          default:
            return VALID_ROUTINE;
          }
        break;
      default:
        return VALID_ROUTINE;
        break;
      }
    }

  // Check to see if variable queue exists
  rc=RexxQueryQueue(varqname,(PULONG)&qcount);
  if (!rc)
    qok=TRUE;

  switch (*args[0].strptr)
    {
    case 'G':
    case 'g':
      if (qok)
        {
        if (qcount)
          {
          DosFreeMem(retstr->strptr);
          retstr->strptr=(PCH)NULL;
          rc=RexxPullQueue(varqname,retstr,&dt,RXQUEUE_WAIT);
          rc=RexxAddQueue(varqname,retstr,RXQUEUE_LIFO);
          }
        }
      else
        rc=RexxCreateQueue(createdname,sizeof(createdname)-1,varqname,&dupname);
      break;
    case 'P':
    case 'p':
      if (!qok)
        rc=RexxCreateQueue(createdname,sizeof(createdname)-1,varqname,&dupname);
      if (qcount)
        {
        MAKERXSTRING(qelem,NULL,0);
        rc=RexxPullQueue(varqname,&qelem,&dt,RXQUEUE_WAIT);
        DosFreeMem(qelem.strptr);
        }
      rc=RexxAddQueue(varqname,&args[2],RXQUEUE_LIFO);
      break;
    case 'D':
    case 'd':
      if (qok)
        RexxDeleteQueue(varqname);
      break;
    default:
      rc=DosReleaseMutexSem(hmtxRxGlobalVar);
      return INVALID_ROUTINE;
      break;
    }

  rc=DosReleaseMutexSem(hmtxRxGlobalVar);
  return VALID_ROUTINE;

}

// Rexx RxExecI external function
EXPORTSPEC ULONG APIENTRY  RxExecI(PUCHAR name, ULONG numargs, RXSTRING args[],
               PSZ queuename, PRXSTRING retstr)
{

  FILE    *stream;
  PVOID    pBuf;
  ULONG    ulBufLen = 65535;
  SHVBLOCK shvb;
  ULONG    bytesread, totalbytesread, linesread;
  ULONG    ulstemctr;
  ULONG    npl;
  APIRET   rc;
  RXSTRING qelem;
  PSZ      qname = NULL;
  PSZ      pszFileName = NULL;
  HFILE    hfFileHandle;
  ULONG    ActionTaken;
  ULONG    ulFileSize = 0;
  ULONG    ulFileAttribute = FILE_NORMAL;
  ULONG    ulOpenFlag;
  ULONG    ulOpenMode;
  PEAOP2   pEABuf = NULL;
  BOOL     bToQueue;
  BOOL     bQueueType;
  UCHAR    varname[250], varval[50];
  PCH      pstr;
  ULONG    linelen;

  // Get file name, if supplied
  hfFileHandle = 0x00000000;  // Stdin by default
  stream = stdin;
  ulFileSize = 80;
  if (numargs >= 1)
    {
    if (args[0].strlength > 0)
      {
      pszFileName = args[0].strptr;
      // Default action is to replace file
      ulOpenFlag = OPEN_ACTION_FAIL_IF_NEW |
                   OPEN_ACTION_OPEN_IF_EXISTS;
      ulOpenMode = OPEN_FLAGS_SEQUENTIAL |
                   OPEN_FLAGS_NOINHERIT  |
                   OPEN_SHARE_DENYNONE   |
                   OPEN_ACCESS_READONLY;
      rc = DosOpen(pszFileName, &hfFileHandle,
                   &ActionTaken, ulFileSize,
                   ulFileAttribute, ulOpenFlag,
                   ulOpenMode, pEABuf);
      if (rc)
        {
        retstr->strlength = sprintf(retstr->strptr,"0 0");
        return VALID_ROUTINE;
        }
      // Position to end of file (to find length)
      rc = DosSetFilePtr(hfFileHandle,(ULONG)0,FILE_END,(PULONG)&npl);
      if (rc)
        {
        DosClose(hfFileHandle);
        return INVALID_ROUTINE;
        } // end-if
      ulFileSize = npl;
      DosClose(hfFileHandle);
      stream = fopen(pszFileName,"r");
      if ( !stream )
        {
        retstr->strlength = sprintf(retstr->strptr,"0 0");
        return VALID_ROUTINE;
        } // end-if
      } // end-if
    } // end-if

  // Get queuing order (if queue specified for arg 3)
  bQueueType = RXQUEUE_FIFO;    // Queue elements by default
  if (numargs >= 2)
    {
    if (args[1].strlength > 0)
      {
      switch (*(args[1].strptr))
        {
        case 'l':
        case 'L':
          bQueueType = RXQUEUE_LIFO;
          break;
        case 'f':
        case 'F':
          bQueueType = RXQUEUE_FIFO;
          break;
        default:
          return INVALID_ROUTINE;
          break;
        } // end-switch
      } // end-if
    } // end-if

  // Get name of queue/stem to use (use current queue by default,
  // if name is not specified)
  qname = queuename;
  if (numargs >= 3)
    {
    if (args[2].strlength > 0)
      {
      strupr(args[2].strptr);
      qname = args[2].strptr;
      }
    }

  // Get type of source (queue or stem)
  bToQueue = TRUE;
  if (numargs >= 4)
    {
    if (args[3].strlength > 0)
      {
      switch (*(args[3].strptr))
        {
        case 's':
        case 'S':
          // If no stem name was supplied, that's an error
          if (args[2].strlength == 0)
            return INVALID_ROUTINE;
          bToQueue = FALSE;
          break;
        case 'q':
        case 'Q':
          bToQueue = TRUE;
          break;
        default:
          return INVALID_ROUTINE;
          break;
        } // end-switch
      } // end-if
    } // end-if

  linesread = 0;
  ulstemctr = 0;
  totalbytesread = 0;

  memset(&shvb,0x00,sizeof(shvb));
  shvb.shvnext = (PSHVBLOCK)NULL;
  if ( !bToQueue )
    {
    shvb.shvcode = RXSHV_DROPV;
    sprintf(varname,"%.*s",args[2].strlength,args[2].strptr);
    MAKERXSTRING(shvb.shvname,varname,strlen(varname));
    rc = RexxVariablePool(&shvb);
    shvb.shvcode = RXSHV_SET;
    }

  pBuf = (PVOID) malloc(ulFileSize);
  if ( !pBuf )
    {
    if ( pszFileName )
      fclose(stream);
    return INVALID_ROUTINE;
    }

  // Process file, putting lines into queue or stem
  rc = 0;
  while ( fgets(pBuf,ulBufLen,stream) && ( !rc ) )
    {
    // Determine exact line length (strip trailing CRLF and EOF marks)
    linelen = min(strlen(pBuf),ulBufLen);
    pstr = ((PCH) pBuf) + linelen;
    while (   ( *pstr == 0x0d || *pstr == 0x0a || *pstr == 0x1a )
           && (pstr >= (PCH) pBuf) )
      {
      pstr--;
      linelen--;
      }
    linelen--;
    if (bToQueue)
      {
      MAKERXSTRING(qelem,pBuf,linelen);
      rc = RexxQueueAddElement(qname,&qelem,bQueueType);
      }
    else
      {
      ulstemctr++;
      sprintf(varname,"%.*s%ld",args[2].strlength,args[2].strptr,
              ulstemctr);
      MAKERXSTRING(shvb.shvname,varname,strlen(varname));
      MAKERXSTRING(shvb.shvvalue,pBuf,linelen);
      rc = RexxVariablePool(&shvb);
      if (rc == RXSHV_NEWV)
        rc = 0;
      } // end-if
    if ( !rc )
      {
      totalbytesread += linelen;
      linesread++;
      }
    } // end-while

  if ( !bToQueue )
    {
    // Set stem.0 value
    sprintf(varname,"%.*s0",args[2].strlength,args[2].strptr);
    MAKERXSTRING(shvb.shvname,varname,strlen(varname));
    sprintf(varval,"%ld",ulstemctr);
    MAKERXSTRING(shvb.shvvalue,varval,strlen(varval));
    rc = RexxVariablePool(&shvb);
    }

  if (pszFileName)
    fclose(stream);

  free(pBuf);

  retstr->strlength = sprintf(retstr->strptr,"%ld %ld",linesread,totalbytesread);
  return VALID_ROUTINE;

}

// Rexx RxExecO external function
EXPORTSPEC ULONG APIENTRY  RxExecO(PUCHAR name, ULONG numargs, RXSTRING args[],
               PSZ queuename, PRXSTRING retstr)
{

  SHVBLOCK shvb;
  ULONG    byteswritten, totalbyteswritten, lineswritten;
  ULONG    ulstemctr;
  ULONG    npl;
  APIRET   rc;
  RXSTRING retval;
  DATETIME rxdatetime;
  PSZ      qname = NULL;
  PSZ      pszFileName = NULL;
  HFILE    hfFileHandle;
  ULONG    ActionTaken;
  ULONG    ulFileSize = 0;
  ULONG    ulFileAttribute = FILE_NORMAL;
  ULONG    ulOpenFlag;
  ULONG    ulOpenMode;
  PEAOP2   pEABuf = NULL;
  BOOL     bFromQueue;
  UCHAR    litbuf[10], varname[250];

  // Get file name, if supplied
  hfFileHandle = 0x00000001;  // Stdout by default
  if (numargs >= 1)
    {
    if (args[0].strlength > 0)
      {
      pszFileName = args[0].strptr;
      // Default action is to replace file
      ulOpenFlag = OPEN_ACTION_CREATE_IF_NEW |
                   OPEN_ACTION_REPLACE_IF_EXISTS;
      if (numargs >= 2)
        {
        if (args[1].strlength > 0)
          {
          switch (*(args[1].strptr))
            {
            case 'a':
            case 'A':
              ulOpenFlag = OPEN_ACTION_CREATE_IF_NEW |
                           OPEN_ACTION_OPEN_IF_EXISTS;
              break;
            case 'r':
            case 'R':
              ulOpenFlag = OPEN_ACTION_CREATE_IF_NEW |
                           OPEN_ACTION_REPLACE_IF_EXISTS;
              break;
            default:
              return INVALID_ROUTINE;
              break;
            } // end-switch
          } // end-if
        } // end-if
      ulOpenMode = OPEN_FLAGS_SEQUENTIAL |
                   OPEN_FLAGS_NOINHERIT  |
                   OPEN_SHARE_DENYNONE   |
                   OPEN_ACCESS_WRITEONLY;
      rc = DosOpen(pszFileName, &hfFileHandle,
                   &ActionTaken, ulFileSize,
                   ulFileAttribute, ulOpenFlag,
                   ulOpenMode, pEABuf);
      if (rc)
        {
        printf("DosOpen failed with rc = %ld\n",rc);
        return INVALID_ROUTINE;
        }
      // Position to end of file
      rc = DosSetFilePtr(hfFileHandle,(ULONG)0,FILE_END,(PULONG)&npl);
      if (rc)
        {
        DosClose(hfFileHandle);
        printf("DosSetFilePtr failed with rc = %ld\n",rc);
        return INVALID_ROUTINE;
        } // end-if
      } // end-if
    } // end-if

  // Get name of queue/stem to use (use current queue by default,
  // if name is not specified)
  qname = queuename;
  if (numargs >= 3)
    {
    if (args[2].strlength > 0)
      {
      strupr(args[2].strptr);
      qname = args[2].strptr;
      }
    }

  // Get type of source (queue or stem)
  bFromQueue = TRUE;
  if (numargs >= 4)
    {
    if (args[3].strlength > 0)
      {
      switch (*(args[3].strptr))
        {
        case 's':
        case 'S':
          // If no stem name was supplied, that's an error
          if (args[2].strlength == 0)
            return INVALID_ROUTINE;
          bFromQueue = FALSE;
          break;
        case 'q':
        case 'Q':
          bFromQueue = TRUE;
          break;
        default:
          return INVALID_ROUTINE;
          break;
        } // end-switch
      } // end-if
    } // end-if

  lineswritten = 0;
  ulstemctr = 0;
  totalbyteswritten = 0;
  strcpy(litbuf,"\r\n");

  // Process queue or stem, putting lines into file

  // Get first line (from queue or stem)
  if (bFromQueue)
    {
    MAKERXSTRING(retval,NULL,0);
    rc = RexxPullQueue(qname,&retval,&rxdatetime, RXQUEUE_NOWAIT);
    }
  else
    {
    memset(&shvb,0x00,sizeof(shvb));
    shvb.shvnext = (PSHVBLOCK)NULL;
    shvb.shvcode = RXSHV_FETCH;
    shvb.shvname.strptr = varname;
    ulstemctr++;
    sprintf(varname,"%.*s%ld",args[2].strlength,args[2].strptr,ulstemctr);
    shvb.shvname.strlength = strlen(varname);
    MAKERXSTRING(shvb.shvvalue,NULL,0);
    rc = RexxVariablePool(&shvb);
    } // end-if
  while (!rc)
    {
    byteswritten = 0;
    if ( !bFromQueue )
      {
      retval.strptr = shvb.shvvalue.strptr;
      retval.strlength = shvb.shvvaluelen;
      }
    rc = DosWrite((HFILE)  hfFileHandle,
                  (PVOID)  retval.strptr,
                  (ULONG)  retval.strlength,
                  (PULONG) &byteswritten);
    if (rc)
      {
      printf("DosWrite failed with rc = %ld\n",rc);
      if (pszFileName)
        DosClose(hfFileHandle);
      }
    // Clean up memory given to us by Rexx interpreter
    if ( bFromQueue )
      DosFreeMem(retval.strptr);
    else
      DosFreeMem(shvb.shvvalue.strptr);
    totalbyteswritten += byteswritten;
    rc = DosWrite((HFILE) hfFileHandle,
                  (PVOID) litbuf,
                  (ULONG) strlen(litbuf),
                  (PULONG) &byteswritten);
    if (rc)
      {
      printf("DosWrite failed with rc = %ld\n",rc);
      if (pszFileName)
        DosClose(hfFileHandle);
      }
    lineswritten++;
    if (bFromQueue)
      {
      MAKERXSTRING(retval,NULL,0);
      rc = RexxPullQueue(qname,&retval,&rxdatetime, RXQUEUE_NOWAIT);
      }
    else
      {
      ulstemctr++;
      sprintf(varname,"%.*s%ld",args[2].strlength,args[2].strptr,ulstemctr);
      shvb.shvname.strlength = strlen(varname);
      MAKERXSTRING(shvb.shvvalue,NULL,0);
      rc = RexxVariablePool(&shvb);
      } // end-if
    } // end-while

  if (pszFileName)
    DosClose(hfFileHandle);

  retstr->strlength = sprintf(retstr->strptr,"%ld %ld",lineswritten,totalbyteswritten);
  return VALID_ROUTINE;

}

#endif  // BLD4OS2

// Rexx RXVLIST external function
EXPORTSPEC ULONG APIENTRY  RxVlist(PUCHAR name, ULONG numargs, RXSTRING args[],
               PSZ queuename, PRXSTRING retstr)
{

  UCHAR varnamebuf[250], vnpat[250], opt, *qname, litbuf[10];
  SHVBLOCK shvb;
  USHORT vctr = 0, cctr;
  ULONG  vnlen = 0;
  ULONG  numqueued;
  ULONG  byteswritten;
  HFILE  hfStdout = 0x01;
  APIRET rc;
  RXSTRING retval;
#if defined(__BLD4OS2__) || defined(__BLD4NT__)
  DATETIME rxdatetime;
#endif  // BLD4OS2
#if defined(__BLD4NT__)
  HANDLE   hStdout=GetStdHandle(STD_OUTPUT_HANDLE);
#endif  // BLD4NT

  // setvbuf(&hfStdout,NULL,_IOLBF,32765);

  // Validate argument number one
  if (numargs >= 1)
    {
    if (args[0].strlength > 0)
      strupr(args[0].strptr);
    vnlen = args[0].strlength;
    strncpy(vnpat,args[0].strptr,vnlen);
    }

  // Validate argument number two
  if (numargs >= 2)
    {
    if (args[1].strlength == 0)
      opt = 'D';
    else
      {
      opt = *(args[1].strptr);
      switch (opt)
        {
        case 'd':
        case 'D':
          opt = 'D';     // Display variables' values
          break;
        case 'n':
        case 'N':
          opt = 'N';     // Put names on queue
          break;
        case 'v':
        case 'V':
          opt = 'V';     // Put values on queue
          break;
        case 'g':
        case 'G':
          opt = 'G';     // Get values from queue
          break;
        default:
          return INVALID_ROUTINE;
          break;
        }
      }
    }
  else
    opt = 'D';

  // Validate arg 3 (name of queue to use)
  if (numargs >= 3)
    {
    if (args[2].strlength > 0)
      qname = args[2].strptr;
    else
      qname = (char*)queuename;
    }
  else
    qname = (char*)queuename;

  // Process requests that require spinning through Rexx Queue
  if (opt == 'G')
    {
    // Find out how many are queued() on relevant Rexx Queue
    rc = RexxQueryQueue(qname,&numqueued);
    if (rc)
      {
      printf("RexxQueryQueue failed to get NumQueued, rc=%ld\n",rc);
      return INVALID_ROUTINE;
      } // end-if
    if (numqueued%2 != 0)
      {
      printf("Odd number of elements (%ld) on Rexx Queue.  There must be an\n",
             numqueued);
      printf("even number of elements on the Rexx Queue\n");
      return INVALID_ROUTINE;
      } // end-if
    shvb.shvnext = (PSHVBLOCK)NULL;
    shvb.shvcode = RXSHV_SET;
    for (cctr=1,vctr=0;cctr<=numqueued;)
      {
      // Get variable name
      MAKERXSTRING(retval,NULL,0);
      rc = RexxPullQueue(qname,(PRXSTRING)&retval,&rxdatetime, RXQUEUE_WAIT);
      if (rc)
        {
        printf("RexxPullQueue failed to get variable name, rc=%ld\n",rc);
        return INVALID_ROUTINE;
        }
      cctr++;
      memcpy(varnamebuf,retval.strptr,retval.strlength);
      shvb.shvname.strptr = varnamebuf;
      shvb.shvname.strlength = retval.strlength;
#if defined(__BLD4OS2__)
      DosFreeMem(retval.strptr);
#endif  // BLD4OS2
#if defined(__BLD4NT__)
      GlobalFree(GlobalHandle(retval.strptr));
#endif  // BLD4NT
      // Get variable value
      MAKERXSTRING(retval,NULL,0);
      rc = RexxPullQueue(qname,(PRXSTRING)&retval,&rxdatetime,RXQUEUE_WAIT);
      if (rc)
        {
        printf("RexxPullQueue failed to get variable value, rc=%ld\n",rc);
        return INVALID_ROUTINE;
        }
      cctr++;
      shvb.shvvalue.strptr = retval.strptr;
      shvb.shvvalue.strlength = retval.strlength;
      if (shvb.shvname.strlength >= vnlen)
        {
        if (vnlen > 0)
          {
          if ( !strncmp(shvb.shvname.strptr,vnpat,vnlen) )
            {
            vctr++;
            rc = RexxVariablePool(&shvb);
            if (rc > 1)
              {
              printf("RexxVariablePool service failed with rc = %ld\n",rc);
              if (rc & 0x08)
                printf("(Invalid variable name)\n");
              if (rc & 0x10)
                printf("(Out of memory)\n");
              printf("Trying to put variable name  '%.*s'\n",
                     shvb.shvname.strlength,shvb.shvname.strptr);
              printf("(of length %ld)\n",shvb.shvname.strlength);
              printf("              with the value '%.*s'\n",
                     shvb.shvvalue.strlength,shvb.shvvalue.strptr);
              printf("(of length %ld)\n",shvb.shvvalue.strlength);

#if defined(__BLD4OS2__)
              DosFreeMem(retval.strptr);
#endif  // BLD4OS2
#if defined(__BLD4NT__)
              GlobalFree(GlobalHandle(retval.strptr));
#endif  // BLD4NT
              return INVALID_ROUTINE;
              } // end-if
#if defined(__BLD4OS2__)
            DosFreeMem(retval.strptr);
#endif  // BLD4OS2
#if defined(__BLD4NT__)
            GlobalFree(GlobalHandle(retval.strptr));
#endif  // BLD4NT
            } // end-if
          } // end-if
        else
          {
          vctr++;
          rc = RexxVariablePool(&shvb);
          // printf("RexxVariablePool rc = %ld\n",rc);
          if (rc > 1)
            {
            printf("RexxVariablePool service failed with rc = %ld\n",rc);
            if (rc & 0x08)
              printf("(Invalid variable name)\n");
            if (rc & 0x10)
              printf("(Out of memory)\n");
            printf("Trying to put variable name  '%.*s'\n",
                   shvb.shvname.strlength,shvb.shvname.strptr);
            printf("(of length %ld)\n",shvb.shvname.strlength);
            printf("              with the value '%.*s'\n",
                   shvb.shvvalue.strlength,shvb.shvvalue.strptr);
            printf("(of length %ld)\n",shvb.shvvalue.strlength);
#if defined(__BLD4OS2__)
            DosFreeMem(retval.strptr);
#endif  // BLD4OS2
#if defined(__BLD4NT__)
            GlobalFree(GlobalHandle(retval.strptr));
#endif  // BLD4NT
            return INVALID_ROUTINE;
            } // end-if
#if defined(__BLD4OS2__)
          DosFreeMem(retval.strptr);
#endif  // BLD4OS2
#if defined(__BLD4NT__)
          GlobalFree(GlobalHandle(retval.strptr));
#endif  // BLD4NT
          } // end-else
        } // end-if
      } // end-for
    } // end else
  // Process requests that require spinning through variable pool
  else
    {
    // Setup for RexxVariablePool to access all variables
    shvb.shvnext = (PSHVBLOCK)NULL;
    shvb.shvcode = RXSHV_NEXTV;
    MAKERXSTRING(shvb.shvname,NULL,0);
    shvb.shvnamelen = 0;
    MAKERXSTRING(shvb.shvvalue,NULL,0);
    shvb.shvvaluelen = 0;
    if (opt == 'D')
      printf("\nRexx Variable Pool:\n\n");
    while (! ( RexxVariablePool(&shvb) & RXSHV_LVAR) )
      {
      if (shvb.shvnamelen >= vnlen)
        {
        if (vnlen > 0)
          {
          if ( !strncmp(shvb.shvname.strptr,vnpat,vnlen) )
            {
            vctr++;
            switch (opt)
              {
              case 'D':
#if defined(__BLD4OS2__)
                rc = DosWrite(
                              (HFILE) 0x00000001,
#endif  // BLD4OS2
#if defined(__BLD4NT__)
                rc = WriteFile(
							  hStdout,
#endif  // BLD4NT
                              (PVOID) shvb.shvname.strptr,
                              (ULONG) shvb.shvnamelen,
                              (PULONG) &byteswritten
#if defined(__BLD4NT__)
                             ,(LPOVERLAPPED)NULL
#endif  // BLD4NT
							  );
#if defined(__BLD4NT__)
                if (!rc) rc=GetLastError();
                //printf("WriteFile rc=%ld\n",rc);
#endif  // BLD4NT
                strcpy(litbuf," = '");
#if defined(__BLD4OS2__)
                rc = DosWrite(
                              (HFILE) 0x00000001,
#endif  // BLD4OS2
#if defined(__BLD4NT__)
                rc = WriteFile(
							  hStdout,
#endif  // BLD4NT
                              (PVOID) litbuf,
                              (ULONG) strlen(litbuf),
                              (PULONG) &byteswritten
#if defined(__BLD4NT__)
                             ,(LPOVERLAPPED)NULL
#endif  // BLD4NT
							  );
#if defined(__BLD4NT__)
				if (!rc) rc=GetLastError();
                //printf("WriteFile rc=%ld\n",rc);
#endif  // BLD4NT
#if defined(__BLD4OS2__)
                rc = DosWrite(
                              (HFILE) 0x00000001,
#endif  // BLD4OS2
#if defined(__BLD4NT__)
                rc = WriteFile(
							  hStdout,
#endif  // BLD4NT
                              (PVOID) shvb.shvvalue.strptr,
                              (ULONG) shvb.shvvaluelen,
                              (PULONG) &byteswritten
#if defined(__BLD4NT__)
                             ,(LPOVERLAPPED)NULL
#endif  // BLD4NT
							  );
#if defined(__BLD4NT__)
				if (!rc) rc=GetLastError();
                //printf("WriteFile rc=%ld\n",rc);
#endif  // BLD4NT
                strcpy(litbuf,"'\r\n");
#if defined(__BLD4OS2__)
                rc = DosWrite(
                              (HFILE) 0x00000001,
#endif  // BLD4OS2
#if defined(__BLD4NT__)
                rc = WriteFile(
							  hStdout,
#endif  // BLD4NT
                              (PVOID) litbuf,
                              (ULONG) strlen(litbuf),
                              (PULONG) &byteswritten
#if defined(__BLD4NT__)
                             ,(LPOVERLAPPED)NULL
#endif  // BLD4NT
							  );
#if defined(__BLD4NT__)
				if (!rc) rc=GetLastError();
                //printf("WriteFile rc=%ld\n",rc);
#endif  // BLD4NT
                break;
              case 'N':
                shvb.shvname.strlength = shvb.shvnamelen;
                rc = RexxQueueAddElement(qname,(PRXSTRING) &shvb.shvname,RXQUEUE_FIFO);
                if (rc)
                  {
                  printf("RexxQueueAddElement of varname failed with rc = %ld\n",rc);
                  return INVALID_ROUTINE;
                  }
                break;
              case 'V':
                shvb.shvname.strlength = shvb.shvnamelen;
                rc = RexxQueueAddElement(qname,(PRXSTRING) &shvb.shvname, RXQUEUE_FIFO);
                if (rc)
                  {
                  printf("RexxQueueAddElement of varname failed with rc = %ld\n",rc);
                  return INVALID_ROUTINE;
                  }
                shvb.shvvalue.strlength = shvb.shvvaluelen;
                rc = RexxQueueAddElement(qname,(PRXSTRING) &shvb.shvvalue, RXQUEUE_FIFO);
                if (rc)
                  {
                  printf("RexxQueueAddElement of varval failed with rc = %ld\n",rc);
                  return INVALID_ROUTINE;
                  }
                break;
              default:
                break;
              } // end switch
            } // end if
          } // end if
        else
          {
          vctr++;
          switch (opt)
            {
            case 'D':
              byteswritten = 0;
#if defined(__BLD4OS2__)
              rc = DosWrite(
                            (HFILE) 0x00000001,
#endif  // BLD4OS2
#if defined(__BLD4NT__)
              rc = WriteFile(
                            hStdout,
#endif  // BLD4NT
                            (PVOID) shvb.shvname.strptr,
                            (ULONG) shvb.shvnamelen,
                            (PULONG) &byteswritten
#if defined(__BLD4NT__)
                           ,(LPOVERLAPPED)NULL
#endif  // BLD4NT
                            );
#if defined(__BLD4NT__)
			  if (!rc) rc=GetLastError();
              //printf("WriteFile rc=%ld\n",rc);
#endif  // BLD4NT
              strcpy(litbuf," = '");
#if defined(__BLD4OS2__)
              rc = DosWrite(
                            (HFILE) 0x00000001,
#endif  // BLD4OS2
#if defined(__BLD4NT__)
              rc = WriteFile(
                            hStdout,
#endif  // BLD4NT
                            (PVOID) litbuf,
                            (ULONG) strlen(litbuf),
                            (PULONG) &byteswritten
#if defined(__BLD4NT__)
                           ,(LPOVERLAPPED)NULL
#endif  // BLD4NT
                            );
#if defined(__BLD4NT__)
			  if (!rc) rc=GetLastError();
              //printf("WriteFile rc=%ld\n",rc);
#endif  // BLD4NT
#if defined(__BLD4OS2__)
              rc = DosWrite(
                            (HFILE) 0x00000001,
#endif  // BLD4OS2
#if defined(__BLD4NT__)
              rc = WriteFile(
                            hStdout,
#endif  // BLD4NT
                            (PVOID) shvb.shvvalue.strptr,
                            (ULONG) shvb.shvvaluelen,
                            (PULONG) &byteswritten
#if defined(__BLD4NT__)
                           ,(LPOVERLAPPED)NULL
#endif  // BLD4NT
                            );
#if defined(__BLD4NT__)
			  if (!rc) rc=GetLastError();
              //printf("WriteFile rc=%ld\n",rc);
#endif  // BLD4NT
              strcpy(litbuf,"'\r\n");
#if defined(__BLD4OS2__)
              rc = DosWrite(
                            (HFILE) 0x00000001,
#endif  // BLD4OS2
#if defined(__BLD4NT__)
              rc = WriteFile(
                            hStdout,
#endif  // BLD4NT
                            (PVOID) litbuf,
                            (ULONG) strlen(litbuf),
                            (PULONG) &byteswritten
#if defined(__BLD4NT__)
                           ,(LPOVERLAPPED)NULL
#endif  // BLD4NT
                            );
#if defined(__BLD4NT__)
			  if (!rc) rc=GetLastError();
              //printf("WriteFile rc=%ld\n",rc);
#endif  // BLD4NT
              break;
            case 'N':
              shvb.shvname.strlength = shvb.shvnamelen;
              rc = RexxQueueAddElement(qname,(PRXSTRING) &shvb.shvname, RXQUEUE_FIFO);
              if (rc)
                {
                printf("RexxQueueAddElement of varname failed with rc = %ld\n",rc);
                return INVALID_ROUTINE;
                } // end-if
              break;
            case 'V':
              shvb.shvname.strlength = shvb.shvnamelen;
              rc = RexxQueueAddElement(qname,(PRXSTRING) &shvb.shvname, RXQUEUE_FIFO);
              if (rc)
                {
                printf("RexxQueueAddElement of varname failed with rc = %ld\n",rc);
                return INVALID_ROUTINE;
                } // end-if
              shvb.shvvalue.strlength = shvb.shvvaluelen;
              rc = RexxQueueAddElement(qname,(PRXSTRING) &shvb.shvvalue, RXQUEUE_FIFO);
              if (rc)
                {
                printf("RexxQueueAddElement of varval failed with rc = %ld\n",rc);
                return INVALID_ROUTINE;
                } // end-if
              break;
            default:
              break;
            } // end switch
          } // end else
        } // end if
#if defined(__BLD4OS2__)
      DosFreeMem(shvb.shvname.strptr);
#endif  // BLD4OS2
#if defined(__BLD4NT__)
      GlobalFree(GlobalHandle(shvb.shvname.strptr));
#endif  // BLD4NT
#if defined(__BLD4OS2__)
      DosFreeMem(shvb.shvvalue.strptr);
#endif  // BLD4OS2
#if defined(__BLD4NT__)
      GlobalFree(GlobalHandle(shvb.shvvalue.strptr));
#endif  // BLD4NT
      shvb.shvnamelen = 0;
      shvb.shvvaluelen = 0;
      MAKERXSTRING(shvb.shvname,NULL,0);
      MAKERXSTRING(shvb.shvvalue,NULL,0);
      }  // end while
    } // end if

  retstr->strlength = sprintf(retstr->strptr,"%ld",vctr);

  return VALID_ROUTINE;
}

// Rexx RxAdd2Ptr external function (Add a number to a pointer)
EXPORTSPEC ULONG APIENTRY  RxAdd2Ptr(PUCHAR name, ULONG numargs, RXSTRING args[],
                 PSZ queuename, PRXSTRING retstr)
{

  LONG   incr;
  PCH    memaddr;

  if (numargs < 1)
    return INVALID_ROUTINE;

  if (args[0].strlength != sizeof(memaddr))
    return INVALID_ROUTINE;

  incr = 0;
  if (numargs >= 2)
    {
    if (args[1].strlength > 0)
      {
      if ( !(incr=atoi(args[1].strptr)) )
        return INVALID_ROUTINE;
      }
    }

  // Copy address from argument number one to pointer
  memcpy(&memaddr,args[0].strptr,args[0].strlength);

  memaddr += incr;

  memcpy(retstr->strptr,&memaddr,sizeof(memaddr));
  retstr->strlength = sizeof(memaddr);

  return VALID_ROUTINE;

}

#if defined(__BLD4OS2__)

// Rexx RxThunkAddr external function (Thunk an address)
EXPORTSPEC ULONG APIENTRY  RxThunkAddr(PUCHAR name, ULONG numargs, RXSTRING args[],
                   PSZ queuename, PRXSTRING retstr)
{

  PCH     p32;
  APIRET  rc;
  PCHAR16 p16;

  if (numargs < 2)
    return INVALID_ROUTINE;

  if (args[0].strlength != sizeof(p32))
    return INVALID_ROUTINE;

  if (args[1].strlength == 0)
    return INVALID_ROUTINE;

  strupr(args[1].strptr);
  switch (*args[1].strptr)
    {
    case 'S':
      memcpy(&p32,args[0].strptr,sizeof(p32));
      p16 = (PCHAR16)p32;
      memcpy(retstr->strptr,(void*)&p16,sizeof(p16));
      break;
    case 'F':
      memcpy((void*)&p16,args[0].strptr,sizeof(p16));
      p32 = (PCH)p16;
      memcpy(retstr->strptr,(void*)&p32,sizeof(p32));
      break;
    default:
      return INVALID_ROUTINE;
      break;
    }

  retstr->strlength = sizeof(p32);

  return VALID_ROUTINE;

}

// Rexx RxSetError external function
EXPORTSPEC ULONG APIENTRY  RxSetError(PUCHAR name, ULONG numargs, RXSTRING args[],
                  PSZ queuename, PRXSTRING retstr)
{

  ULONG  ulFlags = 0;
  ULONG  cctr;
  APIRET rc;
  PCH    cptr;

  if (numargs >= 1)
    {
    if (args[0].strlength > 0)
      {
      cptr = args[0].strptr;
      for (cctr=1;cctr <= args[0].strlength;cctr++)
        {
        switch (*cptr)
          {
          case 'h':
            ulFlags |= FERR_DISABLEHARDERR;
            break;
          case 'H':
            ulFlags |= FERR_ENABLEHARDERR;
            break;
          case 'e':
            ulFlags |= FERR_DISABLEEXCEPTION;
            break;
          case 'E':
            ulFlags |= FERR_ENABLEEXCEPTION;
            break;
          default:
            return INVALID_ROUTINE;
            break;
          } // end-switch
        cptr++;
        } // end-for
      }
    else
      ulFlags = FERR_ENABLEHARDERR | FERR_ENABLEEXCEPTION;
    }
  else
    ulFlags = FERR_ENABLEHARDERR | FERR_ENABLEEXCEPTION;

  rc = DosError(ulFlags);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;

}

// Rexx RxSearchPath external function
EXPORTSPEC ULONG APIENTRY  RxSearchPath(PUCHAR name, ULONG numargs, RXSTRING args[],
                    PSZ queuename, PRXSTRING retstr)
{

  ULONG    ulFlags = 0;
  ULONG    cctr;
  APIRET   rc;
  PCH      cptr;
  UCHAR    buf[256];

  // Arg 1 = File name
  // Arg 2 = Path string or environment variable name
  // Arg 3 = flags:
  //         I - ignore net errors
  //         V - arg 2 is an environment variable name
  //         C - search current directory first

  if (numargs < 2)
    return INVALID_ROUTINE;

  if (numargs >= 3)
    {
    if (args[2].strlength > 0)
      {
      strupr(args[2].strptr);
      for (cptr=args[2].strptr,cctr=1;cctr<=args[2].strlength;cptr++,cctr++)
        switch (*cptr)
          {
          case 'I':
            ulFlags |= SEARCH_IGNORENETERRS;
            break;
          case 'V':
            ulFlags |= SEARCH_ENVIRONMENT;
            break;
          case 'C':
            ulFlags |= SEARCH_CUR_DIRECTORY;
            break;
          default:
            return INVALID_ROUTINE;
            break;
          }
      }
    }

  DosFreeMem(retstr->strptr);
  rc = DosAllocMem((PVOID) &(retstr->strptr), 4096,
                   PAG_COMMIT | PAG_WRITE);
  if (rc)
    {
    printf("DosAllocMem failed with rc = %ld\n",rc);
    return INVALID_ROUTINE;
    }

  memset(buf,0x00,sizeof(buf));
  rc = DosSearchPath((ULONG) ulFlags,
                     (PSZ)args[1].strptr,
                     (PSZ)args[0].strptr,
                     (PSZ)buf,
                     (ULONG)sizeof(buf)-1);

  if (rc)
    retstr->strlength = sprintf(retstr->strptr,"%ld",rc);
  else
    retstr->strlength = sprintf(retstr->strptr,"%ld %s",rc,buf);

  return VALID_ROUTINE;

}

// Rexx RxReplaceModule external function
EXPORTSPEC ULONG APIENTRY  RxReplaceModule(PUCHAR name, ULONG numargs, RXSTRING args[],
                       PSZ queuename, PRXSTRING retstr)
{

  APIRET   rc;
  PSZ      pszOldModule = NULL;
  PSZ      pszNewModule = NULL;
  PSZ      pszBackupModule = NULL;

  // Arg 1 = Old Module
  // Arg 2 = New Module
  // Arg 3 = Backup Module

  if (numargs < 1)
    return INVALID_ROUTINE;
  if (args[0].strlength == 0)
    return INVALID_ROUTINE;
  pszOldModule = args[0].strptr;

  if (numargs >= 2)
    {
    if (args[1].strlength > 0)
      pszNewModule = args[1].strptr;
    }

  if (numargs >= 3)
    {
    if (args[2].strlength > 0)
      pszBackupModule = args[2].strptr;
    }

  rc = DosReplaceModule((PSZ)pszOldModule,
                        (PSZ)pszNewModule,
                        (PSZ)pszBackupModule);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;

}

// Rexx RxExitList external function
EXPORTSPEC ULONG APIENTRY  RxExitList(PUCHAR name, ULONG numargs, RXSTRING args[],
                  PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  ULONG       ulFuncOrder = EXLST_ADD;
  PFNEXITLIST pfxAddr;

  if (numargs < 1)
    return INVALID_ROUTINE;
  if (args[0].strlength != sizeof(PFNEXITLIST))
    return INVALID_ROUTINE;
  memcpy(&pfxAddr,args[0].strptr,sizeof(PFNEXITLIST));

  // Operation
  if (numargs >= 2)
    {
    if (args[1].strlength > 0)
      {
      switch (*args[1].strptr)
        {
        case 'A':
        case 'a':
          ulFuncOrder = EXLST_ADD;
          break;
        case 'R':
        case 'r':
          ulFuncOrder = EXLST_REMOVE;
          break;
        case 'E':
        case 'e':
          ulFuncOrder = EXLST_EXIT;
          break;
        default:
          break;
        }
      }
    }

  // Add Order (meaningful only if arg#2 is "Add")
  if (ulFuncOrder == EXLST_ADD)
    {
    if (numargs >= 3)
      {
      if (args[2].strlength > 0)
        {
        UCHAR uchOrder[4];
        if (args[2].strlength != 1)
          return INVALID_ROUTINE;
        memset(uchOrder,0x00,sizeof(uchOrder));
        uchOrder[2] = *args[2].strptr;
        ulFuncOrder |= (ULONG)(*uchOrder);
        }
      }
    }

  rc = DosExitList((ULONG)ulFuncOrder,(PFNEXITLIST)pfxAddr);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;

}



// Rexx RxStorage external function
EXPORTSPEC ULONG APIENTRY  RxStorage(PUCHAR name, ULONG numargs, RXSTRING args[],
                 PSZ queuename, PRXSTRING retstr)
{

  ULONG  ulRegionSize, ulAllocFlags;
  LONG   memlen;
  PCH    memaddr;
  APIRET rc;
  BOOL   goodmem;

  if (numargs < 1)
    return INVALID_ROUTINE;

  if (args[0].strlength != sizeof(memaddr))
    return INVALID_ROUTINE;

  if (numargs >= 2)
    {
    if (args[1].strlength == 0)
      memlen = 1;
    else
      {
      if (!(memlen=atoi(args[1].strptr)))
        return INVALID_ROUTINE;
      if (memlen < 0)
        return INVALID_ROUTINE;
      }
    }
  else
    memlen = 1;

  // Copy address from argument number one to pointer
  memcpy(&memaddr,args[0].strptr,args[0].strlength);

  ulRegionSize = 0xffffffff;      // 2**32 = 4 gigabytes
  ulAllocFlags = 0;
  goodmem = TRUE;
  rc = DosQueryMem(memaddr, &ulRegionSize, &ulAllocFlags);
  if (rc != 0)
    {
    memlen = 0;
    goodmem = FALSE;
    }
  else
    {
    if (ulAllocFlags & PAG_FREE)
      {
      memlen = 0;
      goodmem = FALSE;
      }
    if (! (ulAllocFlags & PAG_COMMIT))
      {
      memlen = 0;
      goodmem = FALSE;
      }
    }

  // Get some memory for a copy of storage to return to Rexx interpreter
  // (The Rexx interpreter will free this memory)
  if (memlen > 0)
    {

    DosFreeMem(retstr->strptr);

    if (memlen > ulRegionSize)
      memlen = ulRegionSize;

    rc = DosAllocMem((PPVOID) &(retstr->strptr),memlen,PAG_COMMIT | PAG_WRITE);
    if (rc != 0)
      {
      printf("DosAllocMem failed with rc=%ld\n",rc);
      return INVALID_ROUTINE;
      }

    // Copy memory at specified address (for specified length) to temp storage
    memcpy(retstr->strptr,memaddr,memlen);
    }

  // Check to see if we are to set contents of memory
  if (numargs >= 3 && goodmem)
    {
    if (args[2].strlength > 0)
      {
      if (args[2].strlength > ulRegionSize)
        memcpy((PCH) memaddr, args[2].strptr, ulRegionSize);
      else
        memcpy((PCH) memaddr, args[2].strptr, args[2].strlength);
      }
    }

  retstr->strlength = memlen;

  return VALID_ROUTINE;
}

// Rexx RxStructMap external function
EXPORTSPEC ULONG APIENTRY  RxStructMap(PUCHAR name, ULONG numargs, RXSTRING args[],
                   PSZ queuename, PRXSTRING retstr)
{

  PCH              pbuf;
  APIRET           rc;
  UCHAR            varname[251];
  ULONG            numel;
  ULONG            ctr;
  ULONG            pack = 1;
  SHVBLOCK         shvb;
  UCHAR            vartype[5];
  UCHAR            varlen[20];
  ULONG            cumlen;
  ULONG            brakval;
  PSTRUCTMAPELEM   pelem;
  PSTRUCTMAPPREFIX ppref;
  PSTRUCTMAPSUFFIX psuff;

  // Arg 1 = Stem Name

  if (numargs < 1)
    return INVALID_ROUTINE;

  if (args[0].strlength == 0)
    return INVALID_ROUTINE;
  strupr(args[0].strptr);

  // Get stem.0 to find out how many elements in structure
  memset(&shvb,0x00,sizeof(shvb));
  shvb.shvnext = (PSHVBLOCK)NULL;
  shvb.shvcode = RXSHV_FETCH;
  shvb.shvname.strptr = varname;
  sprintf(varname,"%s0",args[0].strptr);
  shvb.shvname.strlength = strlen(varname);
  MAKERXSTRING(shvb.shvvalue,NULL,0);
  rc = RexxVariablePool(&shvb);
  if (rc)
    {
    printf("RexxVariablePool failed with rc = %08x\n",rc);
    DosFreeMem(shvb.shvvalue.strptr);
    return INVALID_ROUTINE;
    }
  if ( !string2long(shvb.shvvaluelen,shvb.shvvalue.strptr,(PLONG)&numel) )
    {
    DosFreeMem(shvb.shvvalue.strptr);
    return INVALID_ROUTINE;
    }
  DosFreeMem(shvb.shvvalue.strptr);
  if ( numel == 0)
    return INVALID_ROUTINE;

  // Get stem.P to find out packing of structure
  sprintf(varname,"%sP",args[0].strptr);
  shvb.shvname.strlength = strlen(varname);
  MAKERXSTRING(shvb.shvvalue,NULL,0);
  rc = RexxVariablePool(&shvb);
  if (rc)
    pack = 1;
  else
    {
    switch (*shvb.shvvalue.strptr)
      {
      case '1':
        pack = 1;
        break;
      default:
        return INVALID_ROUTINE;
        break;
      }
    }
  DosFreeMem(shvb.shvvalue.strptr);

  // Allocate return buffer storage
  DosFreeMem(retstr->strptr);
  retstr->strlength =   sizeof(STRUCTMAPPREFIX)
                      + (numel * sizeof(STRUCTMAPELEM))
                      + sizeof(STRUCTMAPSUFFIX);
  rc = DosAllocMem((PPVOID)&retstr->strptr,retstr->strlength,PAG_COMMIT | PAG_WRITE);
  if (rc)
    {
    printf("\nDosAllocMem failed with rc = %ld\n",rc);
    return INVALID_ROUTINE;
    }
  pbuf = retstr->strptr;

  // Build map prefix
  ppref = (PSTRUCTMAPPREFIX)retstr->strptr;
  ppref->beginbraket = rand();
  brakval = 0xffffffff - ppref->beginbraket;
  ppref->numel = numel;
  ppref->packing = pack;

  // Set up SHVBLOCK for varnames
  MAKERXSTRING(shvb.shvvalue,vartype,sizeof(vartype)-1);

  // Process each element of structure
  pelem = (PSTRUCTMAPELEM)(pbuf + sizeof(STRUCTMAPPREFIX));
  for (ctr=1,cumlen=0;ctr <= numel;ctr++,pelem++)
    {

    // Set this element's offset
    pelem->elemoffset = cumlen;

    // Get stem.n.T to find out data type of this element
    sprintf(varname,"%s%ld.T",args[0].strptr,ctr);
    shvb.shvname.strlength = strlen(varname);
    MAKERXSTRING(shvb.shvvalue,vartype,sizeof(vartype)-1);
    memset(vartype,0x00,sizeof(vartype));
    rc = RexxVariablePool(&shvb);
    if (rc)
      return INVALID_ROUTINE;
    switch (vartype[0])
      {
      case 'l':
        // Signed Long integer
        pelem->elemtype = STRUCT_ELEM_TYPE_LONG;
        pelem->elemlen = sizeof(LONG);
        break;
      case 'L':
        // Unsigned Long integer
        pelem->elemtype = STRUCT_ELEM_TYPE_ULONG;
        pelem->elemlen = sizeof(ULONG);
        break;
      case 's':
        // Signed Short integer
        pelem->elemtype = STRUCT_ELEM_TYPE_SHORT;
        pelem->elemlen = sizeof(SHORT);
        break;
      case 'S':
        // Unsigned Short integer
        pelem->elemtype = STRUCT_ELEM_TYPE_USHORT;
        pelem->elemlen = sizeof(USHORT);
        break;
      case 'd':
      case 'D':
        // Double
        pelem->elemtype = STRUCT_ELEM_TYPE_DOUBLE;
        pelem->elemlen = sizeof(double);
        break;
      case 'C':
      case 'c':
        // Character array of one or more bytes
        // Get stem.n.L to find out length of this character array.
        sprintf(varname,"%s%ld.L",args[0].strptr,ctr);
        shvb.shvname.strlength = strlen(varname);
        MAKERXSTRING(shvb.shvvalue,varlen,sizeof(varlen));
        rc = RexxVariablePool(&shvb);
        if (rc)
          return INVALID_ROUTINE;
        if ( !string2long(shvb.shvvaluelen,varlen,(PLONG)&pelem->elemlen) )
          return INVALID_ROUTINE;
        pelem->elemtype = STRUCT_ELEM_TYPE_CHAR;
        break;
      default:
        return INVALID_ROUTINE;
        break;
      } // end-switch

    cumlen += pelem->elemlen;

    } // end-for

  psuff = (PSTRUCTMAPSUFFIX)pelem;
  psuff->totlen = cumlen;
  psuff->endbraket = brakval;

  return VALID_ROUTINE;
}

// Rexx RxStruct2Stem external function
EXPORTSPEC ULONG APIENTRY  RxStruct2Stem(PUCHAR name, ULONG numargs, RXSTRING args[],
                     PSZ queuename, PRXSTRING retstr)
{

  ULONG    ulRegionSize, ulAllocFlags;
  PCH      memaddr, pbuf, npbuf;
  APIRET   rc;
  BOOL     goodmem, gotpointer=FALSE;
  UCHAR    varname[251];
  ULONG    numel;
  ULONG    ctr;
  ULONG    pack = 1;
  SHVBLOCK shvb;
  UCHAR    vartype[5];
  UCHAR    varval[50];
  UCHAR    varlen[20];
  PCH      memlim;
  ULONG    cumlen = 0;
  PSTRUCTMAPELEM   pelem;
  PSTRUCTMAPPREFIX ppref;
  PSTRUCTMAPSUFFIX psuff;
  ULONG            mapsize;

  // Arg 1 = Stem Name
  // Arg 2 = Address or actual structure contents
  // Arg 3 = StructMap
  // Arg 4 = 'S' to indicate that arg#2 is the memory itself rather than a pointer

  if (numargs < 3)
    return INVALID_ROUTINE;

  if (args[0].strlength == 0)
    return INVALID_ROUTINE;
  strupr(args[0].strptr);

  if (numargs >= 4 && args[3].strlength && toupper(*args[3].strptr)=='S')
    {
    memaddr = args[1].strptr;
    gotpointer = FALSE;
    }
  else
    {
    if (args[1].strlength != sizeof(memaddr))
      return INVALID_ROUTINE;
    memcpy(&memaddr,args[1].strptr,args[1].strlength);
    gotpointer = TRUE;
    }

  // Check structure map to make sure it's "intact".
  // It must be at least as long as prefix and suffix
  if ( args[2].strlength < sizeof(STRUCTMAPPREFIX) + sizeof(STRUCTMAPSUFFIX) )
    {
    printf("Map is shorter than minimum length\n");
    return INVALID_ROUTINE;
    }
  ppref = (PSTRUCTMAPPREFIX)args[2].strptr;
  numel = ppref->numel;
  // The map size must be consistent with the number of elements described
  mapsize =  sizeof(STRUCTMAPPREFIX)
           + (numel * sizeof(STRUCTMAPELEM))
           + sizeof(STRUCTMAPSUFFIX);
  if ( args[2].strlength != mapsize )
    {
    printf("Supplied map length (%ld) doesn't match calculated map length (%ld)\n",
           args[2].strlength,mapsize);
    return INVALID_ROUTINE;
    }
  // The beginning and ending brakets must be consistent
  pbuf = args[2].strptr + sizeof(STRUCTMAPPREFIX) + (numel * sizeof(STRUCTMAPELEM));
  psuff = (PSTRUCTMAPSUFFIX)pbuf;
  if ( ppref->beginbraket + psuff->endbraket != 0xffffffff )
    {
    printf("Map signature is corrupted\n");
    return INVALID_ROUTINE;
    }

  // Point to first element
  pelem = (PSTRUCTMAPELEM)(args[2].strptr + sizeof(STRUCTMAPPREFIX));

  // Check to see if memory is addressable
  ulRegionSize = 0xffffffff;      // 2**32 = 4 gigabytes
  ulAllocFlags = 0;
  goodmem = TRUE;
  if (gotpointer)
    {
    rc = DosQueryMem(memaddr,&ulRegionSize,&ulAllocFlags);
    if (rc)
      goodmem = FALSE;
    else
      {
      if (ulAllocFlags & PAG_FREE)
        goodmem = FALSE;
      if (! (ulAllocFlags & PAG_COMMIT))
        goodmem = FALSE;
      }
    if ( !goodmem )
      return INVALID_ROUTINE;
    }
  else
    {
    ulRegionSize = args[1].strlength;
    }
  memlim = memaddr + ulRegionSize;
  if ( psuff->totlen > ulRegionSize )
    return INVALID_ROUTINE;

  // Set up SHVBLOCK for value variable names
  memset(&shvb,0x00,sizeof(shvb));
  shvb.shvnext = (PSHVBLOCK)NULL;
  shvb.shvcode = RXSHV_SET;
  shvb.shvname.strptr = varname;

  // Set stem.0 to indicate how many elements in structure
  shvb.shvvalue.strptr = varval;
  sprintf(varval,"%ld",numel);
  shvb.shvvalue.strlength = strlen(varval);
  sprintf(varname,"%s0",args[0].strptr);
  shvb.shvname.strlength = strlen(varname);
  rc = RexxVariablePool(&shvb);

  // Process each element of structure
  for (ctr=1,pbuf=memaddr;ctr <= numel && pbuf <= memlim;ctr++,pelem++)
    {

    // Assume value will come from val buffer, unless it's a character
    shvb.shvvalue.strptr = varval;

    switch (pelem->elemtype)
      {
      case STRUCT_ELEM_TYPE_LONG:
        // Signed Long integer
        sprintf(varval,"%ld",*((PLONG)(memaddr + pelem->elemoffset)));
        shvb.shvvalue.strlength = strlen(varval);
        break;
      case STRUCT_ELEM_TYPE_ULONG:
        // Unsigned Long integer
        sprintf(varval,"%lu",*((PULONG)(memaddr + pelem->elemoffset)));
        shvb.shvvalue.strlength = strlen(varval);
        break;
      case STRUCT_ELEM_TYPE_SHORT:
        // Signed Short integer
        sprintf(varval,"%d",*((PSHORT)(memaddr + pelem->elemoffset)));
        shvb.shvvalue.strlength = strlen(varval);
        break;
      case STRUCT_ELEM_TYPE_USHORT:
        // Unsigned Short integer
        sprintf(varval,"%u",*((PUSHORT)(memaddr + pelem->elemoffset)));
        shvb.shvvalue.strlength = strlen(varval);
        break;
      case STRUCT_ELEM_TYPE_DOUBLE:
        // Double
        sprintf(varval,"%f",*((double *)(memaddr + pelem->elemoffset)));
        shvb.shvvalue.strlength = strlen(varval);
        break;
      case STRUCT_ELEM_TYPE_CHAR:
        // Character array of one or more bytes
        // Get stem.n.L to find out length of this character array.
        shvb.shvvalue.strptr = memaddr + pelem->elemoffset;
        shvb.shvvalue.strlength = pelem->elemlen;
        break;
      default:
        return INVALID_ROUTINE;
        break;
      } // end-switch

    sprintf(varname,"%s%ld",args[0].strptr,ctr);
    shvb.shvname.strlength = strlen(varname);
    rc = RexxVariablePool(&shvb);

    } // end-for

  retstr->strlength = sprintf(retstr->strptr,"%ld",psuff->totlen);

  return VALID_ROUTINE;

}

// Rexx RxStem2Struct external function
EXPORTSPEC ULONG APIENTRY  RxStem2Struct(PUCHAR name, ULONG numargs, RXSTRING args[],
                     PSZ queuename, PRXSTRING retstr)
{

  ULONG            ulRegionSize, ulAllocFlags;
  PCH              memaddr, pbuf, npbuf;
  APIRET           rc;
  BOOL             goodmem, gotpointer=FALSE;
  UCHAR            varname[251];
  ULONG            numel;
  ULONG            ctr;
  ULONG            pack = 1;
  SHVBLOCK         shvb;
  UCHAR            vartype[5];
  UCHAR            varval[50];
  UCHAR            varlen[20];
  PCH              memlim;
  ULONG            cumlen = 0;
  UCHAR            numbuf[100];
  long             dumlong;
  ULONG            dumulong;
  short            dumshort;
  USHORT           dumushort;
  double           dumdouble;
  PSTRUCTMAPELEM   pelem;
  PSTRUCTMAPPREFIX ppref;
  PSTRUCTMAPSUFFIX psuff;
  ULONG            mapsize;

  // Arg 1 = Stem Name
  // Arg 2 = Address or name of rexx variable
  // Arg 3 = StructMap
  // Arg 4 = 'V' to indicate that arg#2 is the name of a rexx variable, rather
  //         than a pointer to memory

  if (numargs < 3)
    return INVALID_ROUTINE;

  if (args[0].strlength == 0)
    return INVALID_ROUTINE;
  strupr(args[0].strptr);

  if (numargs >= 4 && args[3].strlength && toupper(*args[3].strptr)=='V')
    {
    memaddr = 0;
    gotpointer = FALSE;
    }
  else
    {
    if (args[1].strlength != sizeof(memaddr))
      return INVALID_ROUTINE;
    memcpy(&memaddr,args[1].strptr,args[1].strlength);
    gotpointer = TRUE;
    }

  // Check structure map to make sure it's "intact".
  // It must be at least as long as prefix and suffix
  if ( args[2].strlength < sizeof(STRUCTMAPPREFIX) + sizeof(STRUCTMAPSUFFIX) )
    {
    printf("Map is shorter than minimum length\n");
    return INVALID_ROUTINE;
    }
  ppref = (PSTRUCTMAPPREFIX)args[2].strptr;
  numel = ppref->numel;
  // The map size must be consistent with the number of elements described
  mapsize =  sizeof(STRUCTMAPPREFIX)
           + (numel * sizeof(STRUCTMAPELEM))
           + sizeof(STRUCTMAPSUFFIX);
  if ( args[2].strlength != mapsize )
    {
    printf("Supplied map length (%ld) doesn't match calculated map length (%ld)\n",
           args[2].strlength,mapsize);
    return INVALID_ROUTINE;
    }
  // The beginning and ending brakets must be consistent
  pbuf = args[2].strptr + sizeof(STRUCTMAPPREFIX) + (numel * sizeof(STRUCTMAPELEM));
  psuff = (PSTRUCTMAPSUFFIX)pbuf;
  if ( ppref->beginbraket + psuff->endbraket != 0xffffffff )
    {
    printf("Map signature is corrupted\n");
    return INVALID_ROUTINE;
    }

  // Point to first element
  pelem = (PSTRUCTMAPELEM)(args[2].strptr + sizeof(STRUCTMAPPREFIX));

  // Check to see if memory is addressable
  ulRegionSize = 0xffffffff;      // 2**32 = 4 gigabytes
  ulAllocFlags = 0;
  goodmem = TRUE;
  if (gotpointer)
    {
    rc = DosQueryMem(memaddr,&ulRegionSize,&ulAllocFlags);
    if (rc)
      goodmem = FALSE;
    else
      {
      if (ulAllocFlags & PAG_FREE)
        goodmem = FALSE;
      if (! (ulAllocFlags & PAG_COMMIT))
        goodmem = FALSE;
      }
    if ( !goodmem )
      return INVALID_ROUTINE;
    }
  else
    {
    memaddr = (char*) malloc(psuff->totlen);
    ulRegionSize = psuff->totlen;
    }
  memlim = memaddr + ulRegionSize;
  if ( psuff->totlen > ulRegionSize )
    return INVALID_ROUTINE;

  // Set up SHVBLOCK for value variable names
  memset(&shvb,0x00,sizeof(shvb));
  shvb.shvnext = (PSHVBLOCK)NULL;
  shvb.shvcode = RXSHV_FETCH;
  shvb.shvname.strptr = varname;

  // Process each element of structure
  for (ctr=1,pbuf=memaddr;ctr <= numel && pbuf <= memlim;ctr++,pelem++)
    {

    // Get this element from stem
    sprintf(varname,"%s%ld",args[0].strptr,ctr);
    shvb.shvname.strlength = strlen(varname);
    MAKERXSTRING(shvb.shvvalue,NULL,0);
    RexxVariablePool(&shvb);

    memset(numbuf,0x00,sizeof(numbuf));

    switch (pelem->elemtype)
      {
      case STRUCT_ELEM_TYPE_LONG:
        // Signed Long integer
        memcpy(numbuf,shvb.shvvalue.strptr,min(shvb.shvvaluelen,sizeof(numbuf)-1));
        dumlong = atoi(numbuf);
        memcpy(memaddr + pelem->elemoffset,&dumlong,sizeof(dumlong));
        break;
      case STRUCT_ELEM_TYPE_ULONG:
        // Unsigned Long integer
        memcpy(numbuf,shvb.shvvalue.strptr,min(shvb.shvvaluelen,sizeof(numbuf)-1));
        dumulong = atoi(numbuf);
        memcpy(memaddr + pelem->elemoffset,&dumulong,sizeof(dumulong));
        break;
      case STRUCT_ELEM_TYPE_SHORT:
        // Signed Short integer
        memcpy(numbuf,shvb.shvvalue.strptr,min(shvb.shvvaluelen,sizeof(numbuf)-1));
        dumshort = atoi(numbuf);
        memcpy(memaddr + pelem->elemoffset,&dumshort,sizeof(dumshort));
        break;
      case STRUCT_ELEM_TYPE_USHORT:
        // Unsigned Short integer
        memcpy(numbuf,shvb.shvvalue.strptr,min(shvb.shvvaluelen,sizeof(numbuf)-1));
        dumushort = atoi(numbuf);
        memcpy(memaddr + pelem->elemoffset,&dumushort,sizeof(dumushort));
        break;
      case STRUCT_ELEM_TYPE_DOUBLE:
        // Double
        memcpy(numbuf,shvb.shvvalue.strptr,min(shvb.shvvaluelen,sizeof(numbuf)-1));
        dumdouble = atof(numbuf);
        memcpy(memaddr + pelem->elemoffset,&dumdouble,sizeof(dumdouble));
        break;
      case STRUCT_ELEM_TYPE_CHAR:
        // Character array of one or more bytes
        // Get stem.n.L to find out length of this character array.
        memcpy(memaddr + pelem->elemoffset,shvb.shvvalue.strptr,min(shvb.shvvaluelen,pelem->elemlen));
        break;
      default:
        return INVALID_ROUTINE;
        break;
      } // end-switch

    DosFreeMem(shvb.shvvalue.strptr);

    } // end-for

  // If name of rexx variable was passed, stuff structure in there
  if (!gotpointer)
    {
    strupr(args[1].strptr);
    memset(&shvb,0x00,sizeof(shvb));
    shvb.shvcode = RXSHV_SET;
    shvb.shvname.strptr = args[1].strptr;
    shvb.shvname.strlength = args[1].strlength;
    shvb.shvvalue.strptr = memaddr;
    shvb.shvvalue.strlength = psuff->totlen;
    RexxVariablePool(&shvb);
    free(memaddr);
    }

  retstr->strlength = sprintf(retstr->strptr,"%ld",psuff->totlen);

  return VALID_ROUTINE;

}

// Rexx RxGetInfoBlocks external function
EXPORTSPEC ULONG APIENTRY  RxGetInfoBlocks(PUCHAR name, ULONG numargs, RXSTRING args[],
                       PSZ queuename, PRXSTRING retstr)
{

  PTIB     ptib;  // Address of pointer to TIB
  PPIB     ppib;  // Address of pointer to PIB
  APIRET   rc;    // Return code
  SHVBLOCK shvb;
  UCHAR    varname[251];
  UCHAR    varval[251];

  if (numargs < 1)
    return INVALID_ROUTINE;
  if (args[0].strlength < 2)
    return INVALID_ROUTINE;
  strupr(args[0].strptr);

  rc = DosGetInfoBlocks(&ptib, &ppib);
  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);
  if (rc)
    return VALID_ROUTINE;

  memset(&shvb,0x00,sizeof(shvb));
  shvb.shvcode = RXSHV_SET;
  shvb.shvname.strptr = varname;

  // Set our pid
  sprintf(varname,"%sP.1",args[0].strptr);
  shvb.shvname.strlength = strlen(varname);
  sprintf(varval,"%ld",ppib->pib_ulpid);
  shvb.shvvalue.strptr = varval;
  shvb.shvvalue.strlength = strlen(varval);
  RexxVariablePool(&shvb);

  // Set our parent's pid
  sprintf(varname,"%sP.2",args[0].strptr);
  shvb.shvname.strlength = strlen(varname);
  sprintf(varval,"%ld",ppib->pib_ulppid);
  shvb.shvvalue.strptr = varval;
  shvb.shvvalue.strlength = strlen(varval);
  RexxVariablePool(&shvb);

  // Set our .EXE's module handle
  sprintf(varname,"%sP.3",args[0].strptr);
  shvb.shvname.strlength = strlen(varname);
  shvb.shvvalue.strptr = (PCH)&ppib->pib_hmte;
  shvb.shvvalue.strlength = sizeof(ppib->pib_hmte);
  RexxVariablePool(&shvb);

  // Set pointer to command line string
  sprintf(varname,"%sP.4",args[0].strptr);
  shvb.shvname.strlength = strlen(varname);
  shvb.shvvalue.strptr = (PCH)&ppib->pib_pchcmd;
  shvb.shvvalue.strlength = sizeof(ppib->pib_pchcmd);
  RexxVariablePool(&shvb);

  // Set environment string
  sprintf(varname,"%sP.5",args[0].strptr);
  shvb.shvname.strlength = strlen(varname);
  shvb.shvvalue.strptr = (PCH)&ppib->pib_pchenv;
  shvb.shvvalue.strlength = sizeof(ppib->pib_pchenv);
  RexxVariablePool(&shvb);

  // Set process-status-bits
  sprintf(varname,"%sP.6",args[0].strptr);
  shvb.shvname.strlength = strlen(varname);
  shvb.shvvalue.strptr = (PCH)&ppib->pib_flstatus;
  shvb.shvvalue.strlength = sizeof(ppib->pib_flstatus);
  RexxVariablePool(&shvb);

  // Set process-type-code
  sprintf(varname,"%sP.7",args[0].strptr);
  shvb.shvname.strlength = strlen(varname);
  shvb.shvvalue.strptr = varval;
  sprintf(varval,"%ld",ppib->pib_ultype);
  shvb.shvvalue.strlength = strlen(varval);
  RexxVariablePool(&shvb);

  // Set thread-id
  sprintf(varname,"%sT.1",args[0].strptr);
  shvb.shvname.strlength = strlen(varname);
  shvb.shvvalue.strptr = varval;
  sprintf(varval,"%ld",ptib->tib_ptib2->tib2_ultid);
  shvb.shvvalue.strlength = strlen(varval);
  RexxVariablePool(&shvb);

  // Set thread-priority
  sprintf(varname,"%sT.2",args[0].strptr);
  shvb.shvname.strlength = strlen(varname);
  shvb.shvvalue.strptr = varval;
  sprintf(varval,"0x%08x",ptib->tib_ptib2->tib2_ulpri);
  shvb.shvvalue.strlength = strlen(varval);
  RexxVariablePool(&shvb);

  // Set tib2-version
  sprintf(varname,"%sT.3",args[0].strptr);
  shvb.shvname.strlength = strlen(varname);
  shvb.shvvalue.strptr = varval;
  sprintf(varval,"%ld",ptib->tib_ptib2->tib2_version);
  shvb.shvvalue.strlength = strlen(varval);
  RexxVariablePool(&shvb);

  // Set tib-MCCount
  sprintf(varname,"%sT.4",args[0].strptr);
  shvb.shvname.strlength = strlen(varname);
  shvb.shvvalue.strptr = varval;
  sprintf(varval,"%d",ptib->tib_ptib2->tib2_usMCCount);
  shvb.shvvalue.strlength = strlen(varval);
  RexxVariablePool(&shvb);

  // Set tib-MCForceFlag
  sprintf(varname,"%sT.5",args[0].strptr);
  shvb.shvname.strlength = strlen(varname);
  shvb.shvvalue.strptr = varval;
  sprintf(varval,"%d",ptib->tib_ptib2->tib2_fMCForceFlag);
  shvb.shvvalue.strlength = strlen(varval);
  RexxVariablePool(&shvb);

  // Set tib-version
  sprintf(varname,"%sT.6",args[0].strptr);
  shvb.shvname.strlength = strlen(varname);
  shvb.shvvalue.strptr = varval;
  sprintf(varval,"%ld",ptib->tib_version);
  shvb.shvvalue.strlength = strlen(varval);
  RexxVariablePool(&shvb);

  // Set tib-ordinal
  sprintf(varname,"%sT.7",args[0].strptr);
  shvb.shvname.strlength = strlen(varname);
  shvb.shvvalue.strptr = varval;
  sprintf(varval,"%ld",ptib->tib_ordinal);
  shvb.shvvalue.strlength = strlen(varval);
  RexxVariablePool(&shvb);

  // Set pointer to head of exception handler chain
  sprintf(varname,"%sT.8",args[0].strptr);
  shvb.shvname.strlength = strlen(varname);
  shvb.shvvalue.strptr = (PCH)&ptib->tib_pexchain;
  shvb.shvvalue.strlength = sizeof(ptib->tib_pexchain);
  RexxVariablePool(&shvb);

  // Set pointer to base of stack
  sprintf(varname,"%sT.9",args[0].strptr);
  shvb.shvname.strlength = strlen(varname);
  shvb.shvvalue.strptr = (PCH)&ptib->tib_pstack;
  shvb.shvvalue.strlength = sizeof(ptib->tib_pstack);
  RexxVariablePool(&shvb);

  // Set pointer to end of stack
  sprintf(varname,"%sT.10",args[0].strptr);
  shvb.shvname.strlength = strlen(varname);
  shvb.shvvalue.strptr = (PCH)&ptib->tib_pstacklimit;
  shvb.shvvalue.strlength = sizeof(ptib->tib_pstacklimit);
  RexxVariablePool(&shvb);

  return VALID_ROUTINE;

}

// Rexx RxQueryAppType external function
EXPORTSPEC ULONG APIENTRY  RxQueryAppType(PUCHAR name, ULONG numargs, RXSTRING args[],
                      PSZ queuename, PRXSTRING retstr)
{

  APIRET   rc;  // Return code
  ULONG    apptype = 0;
  ULONG    ctr;

  if (numargs < 1)
    return INVALID_ROUTINE;
  if (args[0].strlength == 0)
    return INVALID_ROUTINE;

  rc = DosQueryAppType(args[0].strptr,&apptype);
  sprintf(retstr->strptr,"%ld",rc);
  if (!rc)
    {
    strcat(retstr->strptr," ");
    if (apptype & FAPPTYP_NOTSPEC)
      strcat(retstr->strptr,"N");
    if (apptype & FAPPTYP_NOTWINDOWCOMPAT)
      strcat(retstr->strptr,"w");
    if (apptype & FAPPTYP_WINDOWCOMPAT)
      strcat(retstr->strptr,"W");
    if (apptype & FAPPTYP_WINDOWAPI)
      strcat(retstr->strptr,"A");
    if (apptype & FAPPTYP_BOUND)
      strcat(retstr->strptr,"B");
    if (apptype & FAPPTYP_DLL)
      strcat(retstr->strptr,"L");
    if (apptype & FAPPTYP_DOS)
      strcat(retstr->strptr,"D");
    if (apptype & FAPPTYP_PHYSDRV)
      strcat(retstr->strptr,"Y");
    if (apptype & FAPPTYP_VIRTDRV)
      strcat(retstr->strptr,"V");
    if (apptype & FAPPTYP_PROTDLL)
      strcat(retstr->strptr,"P");
    if (apptype & FAPPTYP_32BIT)
      strcat(retstr->strptr,"3");
    }
  retstr->strlength = strlen(retstr->strptr);

  return VALID_ROUTINE;

}

// Rexx RxVioPopUp external function
EXPORTSPEC ULONG APIENTRY  RxVioPopUp(PUCHAR name, ULONG numargs, RXSTRING args[],
                  PSZ queuename, PRXSTRING retstr)
{

  APIRET16 rc;
  USHORT   waitFlags=0;
  HVIO     handle=0;

  if (numargs >= 1)
    {
    if (args[0].strlength > 0)
      {
      strupr(args[0].strptr);
      switch (*args[0].strptr)
        {
        case 'W':
          waitFlags |= VP_WAIT;
          break;
        case 'N':
          waitFlags |= VP_NOWAIT;
          break;
        default:
          return INVALID_ROUTINE;
          break;
        }
      }
    }

  if (numargs >= 2)
    {
    if (args[1].strlength > 0)
      {
      strupr(args[1].strptr);
      switch (*args[1].strptr)
        {
        case 'O':
          waitFlags |= VP_OPAQUE;
          break;
        case 'T':
          waitFlags |= VP_TRANSPARENT;
          break;
        default:
          return INVALID_ROUTINE;
          break;
        }
      }
    }

  rc = VioPopUp(&waitFlags,handle);
  retstr->strlength = sprintf(retstr->strptr,"%d",rc);

  return VALID_ROUTINE;

}

// Rexx RxVioEndPopUp external function
EXPORTSPEC ULONG APIENTRY  RxVioEndPopUp(PUCHAR name, ULONG numargs, RXSTRING args[],
                     PSZ queuename, PRXSTRING retstr)
{

  APIRET16 rc;
  HVIO     handle=0;

  rc = VioEndPopUp(handle);
  retstr->strlength = sprintf(retstr->strptr,"%d",rc);

  return VALID_ROUTINE;

}

// Rexx RxVioWrtCharStrAtt external function
EXPORTSPEC ULONG APIENTRY  RxVioWrtCharStrAtt(PUCHAR name, ULONG numargs, RXSTRING args[],
                          PSZ queuename, PRXSTRING retstr)
{

  APIRET16 rc;
  USHORT   waitFlags=0;
  HVIO     handle=0;
  USHORT   rowx=0,coly=0;
  char     attr=0x00;

  if (numargs < 1)
    return INVALID_ROUTINE;

  // Get "X" coordinate
  if (numargs >= 2)
    {
    if (args[1].strlength)
      rowx = atoi(args[1].strptr);
    }

  // Get "Y" coordinate
  if (numargs >= 3)
    {
    if (args[2].strlength)
      coly = atoi(args[2].strptr);
    }

  // Get display attributes
  if (numargs >= 4)
    {
    if (args[3].strlength)
      memcpy(&attr,args[3].strptr,sizeof(attr));
    }

  rc = VioWrtCharStrAtt(args[0].strptr,args[0].strlength,rowx,coly,&attr,handle);
  retstr->strlength = sprintf(retstr->strptr,"%d",rc);

  return VALID_ROUTINE;

}

// Rexx RxKbdCharIn external function
EXPORTSPEC ULONG APIENTRY  RxKbdCharIn(PUCHAR name, ULONG numargs, RXSTRING args[],
                   PSZ queuename, PRXSTRING retstr)
{

  APIRET16   rc;
  KBDKEYINFO ki;
  HKBD       hkbd=0;
  USHORT     waitFlag=0;
  PCH        cp;

  memset(&ki,0x00,sizeof(ki));
  if (numargs >= 1)
    {
    if (args[0].strlength)
      {
      strupr(args[0].strptr);
      switch (*args[0].strptr)
        {
        case 'W':
          waitFlag = IO_WAIT;
          break;
        case 'N':
          waitFlag = IO_NOWAIT;
          break;
        default:
          return INVALID_ROUTINE;
          break;
        }
      }
    }

  rc = KbdCharIn(&ki,waitFlag,hkbd);
  sprintf(retstr->strptr,"%d",rc);
  if (!rc)
    {
    strcat(retstr->strptr," ");
    cp = retstr->strptr + strlen(retstr->strptr);
    sprintf(cp,"%c",ki.chChar);
    strcat(cp," ");
    cp = retstr->strptr + strlen(retstr->strptr);
    sprintf(cp,"%02x",ki.chScan);
    strcat(cp," ");
    cp = retstr->strptr + strlen(retstr->strptr);
    sprintf(cp,"%02x",ki.fbStatus);
    strcat(cp," ");
    cp = retstr->strptr + strlen(retstr->strptr);
    sprintf(cp,"%02x",ki.bNlsShift);
    strcat(cp," ");
    cp = retstr->strptr + strlen(retstr->strptr);
    sprintf(cp,"%02x",ki.fsState);
    strcat(cp," ");
    cp = retstr->strptr + strlen(retstr->strptr);
    sprintf(cp,"%ld",ki.time);
    }
  retstr->strlength = strlen(retstr->strptr);

  return VALID_ROUTINE;

}

// RxStartRexxSession
EXPORTSPEC ULONG APIENTRY RxStartRexxSession(PUCHAR name, ULONG numargs, RXSTRING args[],
                         PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  STARTDATA   sd;
  UCHAR       szErrBuf[251];
  ULONG       SessID = 0, ctr;
  PID         Pid = 0;
  UCHAR       CreatedName[251];
  BOOL        bDupName = FALSE;

  memset(&sd,0x00,sizeof(sd));
  sd.Length = sizeof(sd);
  sd.PgmName = "RXSRS.EXE";

  if (numargs < 1)
    return INVALID_ROUTINE;

  if (args[0].strlength == 0)
    return INVALID_ROUTINE;

  // Create a Rexx queue (name is system generated)
  memset(CreatedName,0x00,sizeof(CreatedName));
  bDupName = FALSE;
  rc = RexxCreateQueue(CreatedName,sizeof(CreatedName),NULL,(BOOL*)&bDupName);
  if (rc)
    return INVALID_ROUTINE;

  // Pass name of Rexx program, and transport-Rexx-Queue name
  // as arguments.
  sd.PgmInputs = CreatedName;

  // Queue Rexx program name/string and args for "RXSRS.EXE"
  // to receive on queue
  for (ctr=0;ctr < numargs;ctr++)
    rc = RexxAddQueue(CreatedName,(PRXSTRING) &args[ctr], RXQUEUE_FIFO);

  sd.Related = SSF_RELATED_INDEPENDENT;
  sd.FgBg = SSF_FGBG_BACK;
  sd.TraceOpt = SSF_TRACEOPT_NONE;
  sd.TermQ = 0;
  sd.Environment = NULL;
  sd.InheritOpt = SSF_INHERTOPT_PARENT;
  sd.SessionType = SSF_TYPE_WINDOWABLEVIO;
  sd.IconFile = 0;
  sd.PgmHandle = 0;
  sd.PgmControl |= SSF_CONTROL_VISIBLE | SSF_CONTROL_MINIMIZE;
  sd.InitXPos = 0;
  sd.InitYPos = 0;
  sd.InitXSize = 0;
  sd.InitYSize = 0;
  sd.Reserved = 0;
  sd.ObjectBuffer = szErrBuf;
  sd.ObjectBuffLen = sizeof(szErrBuf);
  SessID = 0;
  Pid = 0;
  memset(szErrBuf,0x00,sizeof(szErrBuf));

  rc=DosStartSession(&sd,&SessID,&Pid);
  switch (rc)
    {
    case 0:
    case 457:    // It's ok if session was started in background
      retstr->strlength = sprintf(retstr->strptr,"%ld %ld %ld", rc, SessID, Pid);
      break;
    default:
      retstr->strlength = sprintf(retstr->strptr,"%ld %.*s",rc,sizeof(szErrBuf),szErrBuf);
      rc = RexxDeleteQueue((PSZ)CreatedName);
      break;
    }

  return VALID_ROUTINE;

}

#endif  // BLD4OS2

// Rexx RxScount external function
EXPORTSPEC ULONG APIENTRY  RxScount(PUCHAR name, ULONG numargs, RXSTRING args[],
                PSZ queuename, PRXSTRING retstr)
{

  PCH   nptr, hptr;
  ULONG nctr, hctr, fctr;
  BOOL  ic;

  // Gotta have at least two arguments
  if (numargs < 2)
    return INVALID_ROUTINE;

  // First argument can't be longer than second
  if (args[0].strlength > args[1].strlength)
    return INVALID_ROUTINE;

  // If third argument is supplied, then fold both to upper case
  ic = FALSE;
  if (numargs >= 3)
    {
    if (args[2].strlength > 0)
      ic = TRUE;
    }

  fctr = 0;
  nptr = args[0].strptr;
  hptr = args[1].strptr;
  for (hctr=1;hctr<=args[1].strlength;hctr++)
    {
    if (ic)
      {
      *nptr = toupper(*nptr);
      *hptr = toupper(*hptr);
      }
    if (*hptr == *nptr)
      {
      nptr++;
      hptr++;
      for (nctr=1;nctr<args[0].strlength;nctr++)
        {
        if (ic)
          {
          *nptr = toupper(*nptr);
          *hptr = toupper(*hptr);
          }
        if (*hptr != *nptr)
          break;
        hctr++;
        nptr++;
        hptr++;
        }
      if (nctr == args[0].strlength)
        fctr++;
      }
    else
      {
      hptr++;
      }
    nptr = args[0].strptr;
    }

  retstr->strlength = sprintf(retstr->strptr,"%ld",fctr);

  return VALID_ROUTINE;
}

#if defined(__BLD4OS2__)

// Rexx RxRsoe2f external function
EXPORTSPEC ULONG APIENTRY  RxRsoe2f(PUCHAR name, ULONG numargs, RXSTRING args[],
                PSZ queuename, PRXSTRING retstr)
{

  PSZ     pszFileName;
  HFILE   hfFileHandle, hfStdoe;
  ULONG   ActionTaken;
  ULONG   ulFileSize = 0;
  ULONG   ulFileAttribute = FILE_NORMAL;
  ULONG   ulOpenFlag;
  ULONG   ulOpenMode;
  PEAOP2  pEABuf = NULL;
  APIRET  rc;            // Return code
  UCHAR   arswc;

  PCH nptr, hptr;
  ULONG nctr, hctr, fctr, npl;
  BOOL ic;

  // Gotta have at least one argument
  if (numargs < 1)
    return INVALID_ROUTINE;

  if (args[0].strlength == 0)
    return INVALID_ROUTINE;

  pszFileName = args[0].strptr;

  // Default is to append
  ulOpenFlag = OPEN_ACTION_CREATE_IF_NEW | OPEN_ACTION_OPEN_IF_EXISTS;
  if (numargs >= 2)
    {
    if (args[1].strlength > 0)
      switch (*(args[1].strptr))
        {
        case 'a':
        case 'A':
          ulOpenFlag = OPEN_ACTION_CREATE_IF_NEW |
                       OPEN_ACTION_OPEN_IF_EXISTS;
          break;
        case 'r':
        case 'R':
          ulOpenFlag = OPEN_ACTION_CREATE_IF_NEW |
                       OPEN_ACTION_REPLACE_IF_EXISTS;
          break;
        default:
          return INVALID_ROUTINE;
          break;
        }
    }

  ulOpenMode = OPEN_FLAGS_SEQUENTIAL |
               OPEN_FLAGS_NOINHERIT  |
               OPEN_SHARE_DENYNONE   |
               OPEN_ACCESS_WRITEONLY;

  rc = DosOpen(pszFileName, &hfFileHandle,
               &ActionTaken, ulFileSize,
               ulFileAttribute, ulOpenFlag,
               ulOpenMode, pEABuf);
  if (rc)
    {
    printf("Trapfile DosOpen failed with rc = %ld\n",rc);
    return INVALID_ROUTINE;
    }

  rc = DosSetFilePtr(hfFileHandle,(ULONG)0,FILE_END,(ULONG *)&npl);
  if (rc)
    {
    if (rc != 132)
      {
      printf("Trapfile DosSetFilePtr failed with rc = %ld\n",rc);
      return INVALID_ROUTINE;
      }
    }

  hfStdoe = 0x01; // For Standard Out
  rc = DosDupHandle(hfFileHandle,&hfStdoe);
  if (rc)
    {
    printf("Trapfile DosDupHandle for Standard Out failed with rc = %ld\n",rc);
    return INVALID_ROUTINE;
    }

  hfStdoe = 0x02; // For Standard Error
  rc = DosDupHandle(hfFileHandle,&hfStdoe);
  if (rc)
    {
    printf("Trapfile DosDupHandle for Standard Error failed with rc = %ld\n",rc);
    return INVALID_ROUTINE;
    }

  rc = DosClose(hfFileHandle);

  retstr->strlength = sprintf(retstr->strptr,"0");

  return VALID_ROUTINE;
}

// Rexx RxRsoe2q external function
EXPORTSPEC ULONG APIENTRY  RxRsoe2q(PUCHAR name, ULONG numargs, RXSTRING args[],
                PSZ queuename, PRXSTRING retstr)
{

  APIRET        rc;
  HFILE         hread, hwrite, hfStdoe;
  ULONG         ulPipeSize=4096, byteswritten=0;
  PRXRSOE2QARGS pargs=(PRXRSOE2QARGS)NULL;

  // Arg #1 - action:
  //            E - enable
  //            D - disable
  // Arg #2 - queue name
  //            or
  //          token

  // Gotta have at least two arguments
  if (numargs < 2)
    return INVALID_ROUTINE;

  if (!args[0].strlength )
    return INVALID_ROUTINE;
  if (!args[1].strlength )
    return INVALID_ROUTINE;

  switch(toupper(*args[0].strptr))
    {
    case 'E':
      {
      ULONG         qcount=0;
      TID           tid=0;
      strupr(args[1].strptr);
      // Create pipe for redirection
      rc=DosCreatePipe(&hread,&hwrite,ulPipeSize);
      if (rc)
        {
        printf("DosCreatePipe rc=%ld\n",rc);
        return INVALID_ROUTINE;
        }
      // Redirect stdout/stderr to pipe
      hfStdoe = 0x01; // For Standard Out
      rc=DosDupHandle(hwrite,&hfStdoe);
      if (rc)
        {
        printf("DosDupHandle(stdout) rc=%ld\n",rc);
        return INVALID_ROUTINE;
        }
      hfStdoe = 0x02; // For Standard Error
      rc = DosDupHandle(hwrite,&hfStdoe);
      if (rc)
        {
        printf("DosDupHandle(stderr) rc=%ld\n",rc);
        return INVALID_ROUTINE;
        }
      // Start 2ndary thread to read pipe and write to queue
      pargs = (PRXRSOE2QARGS) malloc(sizeof(RXRSOE2QARGS));
      pargs->hread = hread;
      pargs->hwrite = hwrite;
      pargs->pqname = (PSZ)malloc(args[1].strlength + 1);
      rc=DosCreateEventSem((PSZ)NULL,&pargs->hevDoneWithPipe,0,FALSE);
      if (rc)
        {
        printf("DosCreateEventSem rc=%ld\n",rc);
        return INVALID_ROUTINE;
        }
      strcpy(pargs->pqname,args[1].strptr);
      tid = _beginthread(pipeToQueue
#if !defined(_MSVC_)
		  ,(void *)NULL
#endif
		  ,(unsigned)8096
		  ,(void *)pargs
		  );
      if (tid == -1)
        {
        free(pargs->pqname);
        free(pargs);
        printf("_beginthread failed\n");
        return INVALID_ROUTINE;
        }
      memcpy(retstr->strptr,&pargs,sizeof(PRXRSOE2QARGS));
      retstr->strlength = sizeof(PRXRSOE2QARGS);
      } // end-case ('E')
      break;
    case 'D':
      {
      RXSTRING rargs;
      UCHAR    conlit[] = "con";
      if ( args[1].strlength != sizeof(PRXRSOE2QARGS) )
        return INVALID_ROUTINE;
      memcpy(&pargs,args[1].strptr,sizeof(PRXRSOE2QARGS));
      rc=DosWrite((HFILE) pargs->hwrite,
                  (PVOID) _RXRSOE2Q_END_OF_FILE_MARKER_,
                  (ULONG) strlen(_RXRSOE2Q_END_OF_FILE_MARKER_),
                  (PULONG) &byteswritten);
      if (rc)
        {
        printf("DosWrite rc=%ld\n",rc);
        return INVALID_ROUTINE;
        }
      rc=DosWaitEventSem(pargs->hevDoneWithPipe,SEM_INDEFINITE_WAIT);
      rc=DosCloseEventSem(pargs->hevDoneWithPipe);
      free(pargs->pqname);
      free(pargs);
      rargs.strptr = conlit;
      rargs.strlength = strlen(rargs.strptr);
      rc=RxRsoe2f(name,1,&rargs,queuename,retstr);
      *retstr->strptr = '0';
      retstr->strlength = 1;
      }
      break;
    default:
      return INVALID_ROUTINE;
      break;
    } // end-switch (on action)

  return VALID_ROUTINE;
}

// Function which reads a pipe and writes to a rexx queue
void _Optlink pipeToQueue(void * voidp)

{

  APIRET        rc=0;
  PRXRSOE2QARGS pargs=(PRXRSOE2QARGS)voidp;
  ULONG         sizeoflit = strlen(_RXRSOE2Q_END_OF_FILE_MARKER_);
  ULONG         ulbuflen=0xffff, linelen;
  PCH           pbuf, cp;
  BOOL          type=RXQUEUE_FIFO;
  RXSTRING      qdata;
  FILE         *stream;

  stream = (FILE*)fdopen(pargs->hread,"r");
  pbuf = (PCH) malloc(ulbuflen);
  qdata.strptr = pbuf;
  while ( fgets(pbuf,ulbuflen,stream) && ( !rc ) )
    {
    // Determine exact line length (strip trailing CRLF and EOF marks)
    linelen = min(strlen(pbuf),ulbuflen);
    if ( linelen == sizeoflit-1 && !strncmp(pbuf,_RXRSOE2Q_END_OF_FILE_MARKER_,sizeoflit-1) )
      {
      DosClose(pargs->hwrite);
      break;
      }
    cp = pbuf + linelen;
    while (   ( *cp == 0x0d || *cp == 0x0a || *cp == 0x1a )
           && (cp >= (PCH) pbuf) )
      {
      cp--;
      linelen--;
      }
    linelen--;
    qdata.strlength = linelen;
    rc=RexxQueueAddElement(pargs->pqname,(PRXSTRING)&qdata,(BOOL)type);
    } // end-while (everything is ok)

  rc=DosPostEventSem(pargs->hevDoneWithPipe);
  fclose(stream);
  free(pbuf);

  _endthread();

  return;
}

// Rexx RxKillProcess external function
EXPORTSPEC ULONG APIENTRY  RxKillProcess(PUCHAR name, ULONG numargs, RXSTRING args[],
                     PSZ queuename, PRXSTRING retstr)
{

  // DosKillProcess flags a process to
  // terminate, and returns the termination
  // code to its parent (if any).

  ULONG   ulActionCode;
  PID     idProcessID;
  APIRET  rc;           // Return code

  // Gotta have at least one argument (first one is process-id (decimal))
  if (numargs < 1)
    return INVALID_ROUTINE;

  if (args[0].strlength == 0)
    return INVALID_ROUTINE;

  if (string2long(args[0].strlength,
      args[0].strptr,
      (LONG *) &idProcessID) != TRUE)
    {
    printf ("Process-Id is not a valid integer.\n");
    return (INVALID_ROUTINE);
    }

  ulActionCode = 1;    // Process Only
  if (numargs >= 2)
    {
    if (args[1].strlength > 0)
      {
      switch ( *(args[1].strptr) )
        {
        case 'p':
        case 'P':
          ulActionCode = 1;    // Process Only
          break;
        case 't':
        case 'T':
          ulActionCode = 0;    // Process Tree
          break;
        default:
          printf("Invalid Option '%s' supplied\n",args[1].strptr);
          printf("Valid options are 'Process' or 'Tree'\n");
          return INVALID_ROUTINE;
          break;
        }
      }
    }

  rc = DosKillProcess(ulActionCode,idProcessID);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;
}

// Rexx RxCreatePipe external function (Create an un-named Pipe)
EXPORTSPEC ULONG APIENTRY  RxCreatePipe(PUCHAR name, ULONG numargs, RXSTRING args[],
                    PSZ queuename, PRXSTRING retstr)
{

  HFILE   hread, hwrite;
  ULONG   ulPipeSize;
  APIRET  rc;            // Return code
  SHVBLOCK shvb;
  UCHAR   varname[251];
  UCHAR   varval[200];

  // Gotta have at least one argument (Rexx stem name)
  if (numargs < 1)
    return INVALID_ROUTINE;

  if (args[0].strlength == 0)
    return INVALID_ROUTINE;

  ulPipeSize = 4096;   // Default pipe size is 4k
  if (numargs >= 2)
    {
    if (args[1].strlength > 0)
      {
      if ( !string2long(args[1].strlength,args[1].strptr,(PLONG) &ulPipeSize) )
        {
        printf("Pipe Size '%.*s' is an invalid integer\n",
               args[1].strlength,args[1].strptr);
        return INVALID_ROUTINE;
        }
      }
    }

  rc = DosCreatePipe(&hread, &hwrite, ulPipeSize);
  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  if (args[0].strlength > 0)
    strupr(args[0].strptr);
  memset(&shvb,0x00,sizeof(shvb));
  shvb.shvcode = RXSHV_SET;
  shvb.shvname.strptr = varname;

  // Set stem.1 to pipe's read handle
  sprintf(varname,"%.*s1",args[0].strlength,args[0].strptr);
  shvb.shvname.strlength = strlen(varname);
  shvb.shvvalue.strptr = (PCH) &hread;
  shvb.shvvalue.strlength = sizeof(hread);
  RexxVariablePool(&shvb);

  // Set stem.2 to pipe's write handle
  sprintf(varname,"%.*s2",args[0].strlength,args[0].strptr);
  shvb.shvname.strlength = strlen(varname);
  shvb.shvvalue.strptr = (PCH) &hwrite;
  shvb.shvvalue.strlength = sizeof(hwrite);
  RexxVariablePool(&shvb);

  // Set stem.3 to pipe's size
  sprintf(varname,"%.*s3",args[0].strlength,args[0].strptr);
  shvb.shvname.strlength = strlen(varname);
  sprintf(varval,"%ld",ulPipeSize);
  shvb.shvvalue.strptr = varval;
  shvb.shvvalue.strlength = strlen(varval);
  RexxVariablePool(&shvb);

  return VALID_ROUTINE;
}

// Rexx RxDestroyPipe external function (Destroy an un-named Pipe)
EXPORTSPEC ULONG APIENTRY  RxDestroyPipe(PUCHAR name, ULONG numargs, RXSTRING args[],
                     PSZ queuename, PRXSTRING retstr)
{

  HFILE   hread, hwrite;
  APIRET  rc;            // Return code

  // Gotta have at least two arguments
  if (numargs < 2)
    return INVALID_ROUTINE;

  if (args[0].strlength != sizeof(HFILE))
    return INVALID_ROUTINE;

  if (args[1].strlength != sizeof(HFILE))
    return INVALID_ROUTINE;

  memcpy(&hread,args[0].strptr,args[0].strlength);
  memcpy(&hwrite,args[1].strptr,args[1].strlength);

  retstr->strlength = sprintf(retstr->strptr,"%ld %ld",DosClose(hread),DosClose(hwrite));

  return VALID_ROUTINE;
}

// Rexx RxCreateQueue (do a DosCreateQueue)
EXPORTSPEC ULONG APIENTRY  RxCreateQueue(PUCHAR name, ULONG numargs, RXSTRING args[],
                     PSZ queuename, PRXSTRING retstr)
{

  HQUEUE   QueueHandle;
  ULONG    ulQueueFlags = 0;
  APIRET   rc;
  SHVBLOCK shvb;
  PCH      cptr;
  ULONG    cctr;

  // Arg 1 = rexx variable name
  // Arg 2 = queue name
  // Arg 3 = Queue Flags

  if (numargs < 2)
    return INVALID_ROUTINE;

  if (args[0].strlength == 0 || args[1].strlength == 0)
    return INVALID_ROUTINE;

  if (numargs >= 3)
    {
    if (args[2].strlength > 0)
      {
      cptr = args[2].strptr;
      for (cctr=1;cctr <= args[2].strlength;cctr++)
        {
        switch (*cptr)
          {
          case 'f':
          case 'F':
            ulQueueFlags |= QUE_FIFO;
            break;
          case 'l':
          case 'L':
            ulQueueFlags |= QUE_LIFO;
            break;
          case 'p':
          case 'P':
            ulQueueFlags |= QUE_PRIORITY;
            break;
          case 'c':
          case 'C':
            ulQueueFlags |= QUE_CONVERT_ADDRESS;
            break;
          case 'n':
          case 'N':
            ulQueueFlags |= QUE_NOCONVERT_ADDRESS;
            break;
          default:
            return INVALID_ROUTINE;
            break;
          } // end-switch
        cptr++;
        } // end-for
      } // end-if
    } // end-if

  rc = DosCreateQueue(&QueueHandle,ulQueueFlags,args[1].strptr);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  strupr(args[0].strptr);
  memset(&shvb,0x00,sizeof(shvb));
  shvb.shvcode = RXSHV_SET;
  shvb.shvname.strptr = args[0].strptr;
  shvb.shvname.strlength = args[0].strlength;
  shvb.shvvalue.strptr = (PCH) &QueueHandle;
  shvb.shvvalue.strlength = sizeof(QueueHandle);
  RexxVariablePool(&shvb);

  return VALID_ROUTINE;
}

// Rexx RxOpenQueue (do a DosOpenQueue)
EXPORTSPEC ULONG APIENTRY  RxOpenQueue(PUCHAR name, ULONG numargs, RXSTRING args[],
                   PSZ queuename, PRXSTRING retstr)
{

  PID     pidQueueOwnerPID;
  HQUEUE  hqQueueHandle;
  APIRET  rc;            // Return code
  SHVBLOCK shvb;
  UCHAR   varname[251];
  UCHAR   varval[200];

  // Arg 1 = rexx stem variable name
  //         stem.1 = queue-owner-pid
  //         stem.2 = queue-handle
  // Arg 2 = queue name

  if (numargs < 2)
    return INVALID_ROUTINE;

  if (args[0].strlength == 0 || args[1].strlength == 0)
    return INVALID_ROUTINE;

  rc = DosOpenQueue(&pidQueueOwnerPID,&hqQueueHandle,args[1].strptr);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  strupr(args[0].strptr);
  memset(&shvb,0x00,sizeof(shvb));
  shvb.shvcode = RXSHV_SET;
  shvb.shvname.strptr = varname;

  // Set stem.1 to Queue's owner's PID
  sprintf(varname,"%.*s1",args[0].strlength,args[0].strptr);
  shvb.shvname.strlength = strlen(varname);
  shvb.shvvalue.strptr = varval;
  sprintf(varval,"%ld",pidQueueOwnerPID);
  shvb.shvvalue.strlength = strlen(varval);
  RexxVariablePool(&shvb);

  // Set stem.2 to Queue's handle
  sprintf(varname,"%.*s2",args[0].strlength,args[0].strptr);
  shvb.shvname.strlength = strlen(varname);
  shvb.shvvalue.strptr = (PCH) &hqQueueHandle;
  shvb.shvvalue.strlength = sizeof(hqQueueHandle);
  RexxVariablePool(&shvb);

  return VALID_ROUTINE;
}

// Rexx RxPeekQueue (do a DosPeekQueue)
EXPORTSPEC ULONG APIENTRY  RxPeekQueue(PUCHAR name, ULONG numargs, RXSTRING args[],
                   PSZ queuename, PRXSTRING retstr)
{

  APIRET        rc;
  SHVBLOCK      shvb;
  UCHAR         varname[251];
  UCHAR         varval[200];
  HQUEUE        QueueHandle;
  REQUESTDATA   RequestData;
  ULONG         DataLength;
  PVOID         DataAddress;
  ULONG         ElementCode = 0;
  BOOL32        f32NoWait = DCWW_WAIT;
  UCHAR         uchElemPriority;
  HEV           SemHandle = 0;

  // Arg 1 = rexx variable name
  //         stem.1 = data pointer
  //         stem.2 = data length
  //         stem.3 = pid of process which put element on queue
  //         stem.4 = event code
  //         stem.5 = element code
  //         stem.6 = element priority
  // Arg 2 = queue handle
  // Arg 3 = element code
  // Arg 4 = wait/nowait
  // Arg 5 = semhandle

  if (numargs < 2)
    return INVALID_ROUTINE;

  if (args[0].strlength == 0)
    return INVALID_ROUTINE;

  if (args[1].strlength != sizeof(HQUEUE))
    return INVALID_ROUTINE;
  memcpy(&QueueHandle,args[1].strptr,args[1].strlength);

  if (numargs >= 3)
    {
    if ( !string2long(args[2].strlength,args[2].strptr,(PLONG) &ElementCode) )
      return INVALID_ROUTINE;
    }

  if (numargs >= 4)
    {
    if (args[3].strlength > 0)
      {
      switch (*args[3].strptr)
        {
        case 'w':
        case 'W':
          f32NoWait = DCWW_WAIT;
          break;
        case 'n':
        case 'N':
          f32NoWait = DCWW_NOWAIT;
          break;
        default:
          return INVALID_ROUTINE;
          break;
        }
      }
    }

  if (numargs >= 5)
    {
    if (args[4].strlength != sizeof(HEV) )
      return INVALID_ROUTINE;
    memcpy(&SemHandle,args[4].strptr,args[4].strlength);
    }

  rc = DosPeekQueue(QueueHandle, &RequestData,
                    &DataLength, &DataAddress,
                    &ElementCode, f32NoWait,
                    (PBYTE) &uchElemPriority, SemHandle);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  if ( !rc )
    {
    strupr(args[0].strptr);
    memset(&shvb,0x00,sizeof(shvb));
    shvb.shvcode = RXSHV_SET;
    shvb.shvname.strptr = varname;

    // Put pointer in stem.1
    sprintf(varname,"%s1",args[0].strptr);
    shvb.shvname.strlength = strlen(varname);
    shvb.shvvalue.strptr = (PCH) &DataAddress;
    shvb.shvvalue.strlength = sizeof(DataAddress);
    RexxVariablePool(&shvb);

    // Put length in stem.2
    sprintf(varname,"%s2",args[0].strptr);
    shvb.shvname.strlength = strlen(varname);
    shvb.shvvalue.strptr = varval;
    sprintf(varval,"%ld",DataLength);
    shvb.shvvalue.strlength = strlen(varval);
    RexxVariablePool(&shvb);

    // Put enqueueing pid in stem.3
    sprintf(varname,"%s3",args[0].strptr);
    shvb.shvname.strlength = strlen(varname);
    shvb.shvvalue.strptr = varval;
    sprintf(varval,"%ld",RequestData.pid);
    shvb.shvvalue.strlength = strlen(varval);
    RexxVariablePool(&shvb);

    // Put event code in stem.4
    sprintf(varname,"%s4",args[0].strptr);
    shvb.shvname.strlength = strlen(varname);
    shvb.shvvalue.strptr = varval;
    sprintf(varval,"%ld",RequestData.ulData);
    shvb.shvvalue.strlength = strlen(varval);
    RexxVariablePool(&shvb);

    // Put element code in stem.5
    sprintf(varname,"%s5",args[0].strptr);
    shvb.shvname.strlength = strlen(varname);
    shvb.shvvalue.strptr = varval;
    sprintf(varval,"%ld",ElementCode);
    shvb.shvvalue.strlength = strlen(varval);
    RexxVariablePool(&shvb);

    // Put element priority in stem.6
    sprintf(varname,"%s6",args[0].strptr);
    shvb.shvname.strlength = strlen(varname);
    shvb.shvvalue.strptr = varval;
    sprintf(varval,"%d",uchElemPriority);
    shvb.shvvalue.strlength = strlen(varval);
    RexxVariablePool(&shvb);
    }

  return VALID_ROUTINE;
}

// Rexx RxReadQueue (do a DosReadQueue)
EXPORTSPEC ULONG APIENTRY  RxReadQueue(PUCHAR name, ULONG numargs, RXSTRING args[],
                   PSZ queuename, PRXSTRING retstr)
{

  APIRET        rc;
  SHVBLOCK      shvb;
  UCHAR         varname[251];
  UCHAR         varval[200];
  HQUEUE        QueueHandle;
  REQUESTDATA   RequestData;
  ULONG         DataLength;
  PVOID         DataAddress;
  ULONG         ElementCode = 0;
  BOOL32        f32NoWait = DCWW_WAIT;
  UCHAR         uchElemPriority;
  HEV           SemHandle = 0;

  // Arg 1 = rexx variable name
  //         stem.1 = data pointer
  //         stem.2 = data length
  //         stem.3 = pid of process which put element on queue
  //         stem.4 = event code
  //         stem.5 = element priority
  // Arg 2 = queue handle
  // Arg 3 = element code
  // Arg 4 = wait/nowait
  // Arg 5 = semhandle

  if (numargs < 2)
    return INVALID_ROUTINE;

  if (args[0].strlength == 0)
    return INVALID_ROUTINE;

  if (args[1].strlength != sizeof(HQUEUE))
    return INVALID_ROUTINE;
  memcpy(&QueueHandle,args[1].strptr,args[1].strlength);

  if (numargs >= 3)
    {
    if ( !string2long(args[2].strlength,args[2].strptr,(PLONG) &ElementCode) )
      return INVALID_ROUTINE;
    }

  if (numargs >= 4)
    {
    if (args[3].strlength > 0)
      {
      switch (*args[3].strptr)
        {
        case 'w':
        case 'W':
          f32NoWait = DCWW_WAIT;
          break;
        case 'n':
        case 'N':
          f32NoWait = DCWW_NOWAIT;
          break;
        default:
          return INVALID_ROUTINE;
          break;
        }
      }
    }

  if (numargs >= 5)
    {
    if (args[4].strlength != sizeof(HEV) )
      return INVALID_ROUTINE;
    memcpy(&SemHandle,args[4].strptr,args[4].strlength);
    }

  rc = DosReadQueue(QueueHandle, &RequestData,
                    &DataLength, &DataAddress,
                    ElementCode, f32NoWait,
                    (PBYTE) &uchElemPriority, SemHandle);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  if ( !rc )
    {
    strupr(args[0].strptr);
    memset(&shvb,0x00,sizeof(shvb));
    shvb.shvcode = RXSHV_SET;
    shvb.shvname.strptr = varname;

    // Put pointer in stem.1
    sprintf(varname,"%s1",args[0].strptr);
    shvb.shvname.strlength = strlen(varname);
    shvb.shvvalue.strptr = (PCH) &DataAddress;
    shvb.shvvalue.strlength = sizeof(DataAddress);
    RexxVariablePool(&shvb);

    // Put length in stem.2
    sprintf(varname,"%s2",args[0].strptr);
    shvb.shvname.strlength = strlen(varname);
    shvb.shvvalue.strptr = varval;
    sprintf(varval,"%ld",DataLength);
    shvb.shvvalue.strlength = strlen(varval);
    RexxVariablePool(&shvb);

    // Put enqueueing pid in stem.3
    sprintf(varname,"%s3",args[0].strptr);
    shvb.shvname.strlength = strlen(varname);
    shvb.shvvalue.strptr = varval;
    sprintf(varval,"%ld",RequestData.pid);
    shvb.shvvalue.strlength = strlen(varval);
    RexxVariablePool(&shvb);

    // Put event code in stem.4
    sprintf(varname,"%s4",args[0].strptr);
    shvb.shvname.strlength = strlen(varname);
    shvb.shvvalue.strptr = varval;
    sprintf(varval,"%ld",RequestData.ulData);
    shvb.shvvalue.strlength = strlen(varval);
    RexxVariablePool(&shvb);

    // Put element priority in stem.5
    sprintf(varname,"%s5",args[0].strptr);
    shvb.shvname.strlength = strlen(varname);
    shvb.shvvalue.strptr = varval;
    sprintf(varval,"%d",uchElemPriority);
    shvb.shvvalue.strlength = strlen(varval);
    RexxVariablePool(&shvb);
    }

  return VALID_ROUTINE;
}

// Rexx RxReadQueueStr (do a DosReadQueue and return value as result)
EXPORTSPEC ULONG APIENTRY  RxReadQueueStr(PUCHAR name, ULONG numargs, RXSTRING args[],
                      PSZ queuename, PRXSTRING retstr)
{

  APIRET        rc;
  UCHAR         qname[251];
  HQUEUE        hqQueueHandle;
  REQUESTDATA   RequestData;
  ULONG         DataLength;
  PVOID         DataAddress;
  ULONG         ElementCode = 0;
  BOOL32        f32NoWait = DCWW_WAIT;
  UCHAR         uchElemPriority;
  HEV           SemHandle = 0;
  ULONG         ulFlags;
  ULONG         ulRegionSize;
  BOOL          goodmem;
  BOOL          bRegionTooSmall;
  PID           pidQueueOwnerPid;

  // Arg 1 = queue name or queue handle

  retstr->strlength = sprintf(retstr->strptr,"");

  if (numargs < 1)
    return VALID_ROUTINE;

  if (args[0].strlength == 0)
    return VALID_ROUTINE;

  if (args[0].strlength == sizeof(HQUEUE) )
    memcpy(&hqQueueHandle,args[0].strptr,sizeof(HQUEUE));
  else
    {
    strupr(args[0].strptr);
    strcpy(qname,"\\QUEUES\\");
    strcat(qname,args[0].strptr);
    rc = DosOpenQueue(&pidQueueOwnerPid,&hqQueueHandle,qname);
    if (rc)
      return VALID_ROUTINE;
    }

  rc = DosReadQueue(hqQueueHandle, &RequestData,
                    &DataLength, &DataAddress,
                    ElementCode, f32NoWait,
                    (PBYTE) &uchElemPriority, SemHandle);

  if ( !rc )
    {
    // Check pointer we got off queue to see if we can use it
    ulFlags = 0;
    goodmem = TRUE;
    ulRegionSize = 0xffffffff;
    bRegionTooSmall = FALSE;
    rc = DosQueryMem(DataAddress, &ulRegionSize, &ulFlags);
    if (rc)
      goodmem = FALSE;
    else
      {
      if (ulFlags & PAG_FREE)
        goodmem = FALSE;
      if (! (ulFlags & PAG_COMMIT))
        goodmem = FALSE;
      if (ulRegionSize < DataLength)
        {
        bRegionTooSmall = FALSE;
        DataLength = ulRegionSize;
        }
      }
    if (goodmem)
      {
      DosFreeMem(retstr->strptr);
      retstr->strptr = DataAddress;
      retstr->strlength = DataLength;
      }
    else
      DosFreeMem(DataAddress);
    }

  return VALID_ROUTINE;
}

// Rexx RxWriteQueue (do a DosWriteQueue)
EXPORTSPEC ULONG APIENTRY  RxWriteQueue(PUCHAR name, ULONG numargs, RXSTRING args[],
                    PSZ queuename, PRXSTRING retstr)
{

  APIRET        rc;
  HQUEUE        QueueHandle;
  ULONG         ulRequest = 0;
  ULONG         ulElementPriority = 0;
  PVOID         pData;
  ULONG         ulDlen;

  // Arg 1 = queue handle
  // Arg 2 = pointer
  // Arg 3 = length
  // Arg 4 = ulRequestData
  // Arg 5 = element priority

  if (numargs < 3)
    return INVALID_ROUTINE;

  if (args[0].strlength != sizeof(HQUEUE))
    return INVALID_ROUTINE;
  memcpy(&QueueHandle,args[0].strptr,args[0].strlength);

  if (args[1].strlength != sizeof(PVOID))
    return INVALID_ROUTINE;
  memcpy(&pData,args[1].strptr,args[1].strlength);

  if ( !string2long(args[2].strlength,args[2].strptr,(PLONG) &ulDlen) )
    return INVALID_ROUTINE;

  if (numargs >= 4)
    {
    if ( !string2long(args[3].strlength,args[3].strptr,(PLONG) &ulRequest) )
      return INVALID_ROUTINE;
    }

  if (numargs >= 5)
    {
    if ( !string2long(args[4].strlength,args[4].strptr,(PLONG) &ulElementPriority) )
      return INVALID_ROUTINE;
    }

  rc = DosWriteQueue(QueueHandle, ulRequest,
                     ulDlen, pData,
                     ulElementPriority);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;
}

// Rexx RxPurgeQueue (do a DosPurgeQueue)
EXPORTSPEC ULONG APIENTRY  RxPurgeQueue(PUCHAR name, ULONG numargs, RXSTRING args[],
                    PSZ queuename, PRXSTRING retstr)
{

  APIRET        rc;
  HQUEUE        QueueHandle;

  // Arg 1 = queue handle

  if (numargs < 1)
    return INVALID_ROUTINE;

  if (args[0].strlength != sizeof(HQUEUE))
    return INVALID_ROUTINE;
  memcpy(&QueueHandle,args[0].strptr,args[0].strlength);

  rc = DosPurgeQueue(QueueHandle);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;
}

// Rexx RxQueryQueue (do a DosQueryQueue)
EXPORTSPEC ULONG APIENTRY  RxQueryQueue(PUCHAR name, ULONG numargs, RXSTRING args[],
                    PSZ queuename, PRXSTRING retstr)
{

  APIRET        rc;
  HQUEUE        QueueHandle;
  ULONG         ulNumElem = 0;

  // Arg 1 = queue handle

  if (numargs < 1)
    return INVALID_ROUTINE;

  if (args[0].strlength != sizeof(HQUEUE))
    return INVALID_ROUTINE;
  memcpy(&QueueHandle,args[0].strptr,args[0].strlength);

  rc = DosQueryQueue(QueueHandle,&ulNumElem);

  retstr->strlength = sprintf(retstr->strptr,"%ld %ld",rc,ulNumElem);

  return VALID_ROUTINE;
}

// Rexx RxCloseQueue (do a DosCloseQueue)
EXPORTSPEC ULONG APIENTRY  RxCloseQueue(PUCHAR name, ULONG numargs, RXSTRING args[],
                    PSZ queuename, PRXSTRING retstr)
{

  APIRET        rc;
  HQUEUE        QueueHandle;

  if (numargs < 1)
    return INVALID_ROUTINE;

  if (args[0].strlength != sizeof(HQUEUE))
    return INVALID_ROUTINE;
  memcpy(&QueueHandle,args[0].strptr,args[0].strlength);

  rc = DosCloseQueue(QueueHandle);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;
}

// Rexx RxSoSe2H external function (Standard-Out/Standard-Error To Handle)
EXPORTSPEC ULONG APIENTRY  RxSoSe2H(PUCHAR name, ULONG numargs, RXSTRING args[],
                PSZ queuename, PRXSTRING retstr)
{

  HFILE   hwrite, hfStdoe;
  APIRET  rc;            // Return code

  // Gotta have at least one argument
  if (numargs < 1)
    return INVALID_ROUTINE;

  if (args[0].strlength != sizeof(HFILE))
    return INVALID_ROUTINE;

  memcpy(&hwrite,args[0].strptr,args[0].strlength);

  hfStdoe = 0x01; // For Standard Out
  rc = DosDupHandle(hwrite,&hfStdoe);
  if (!rc)
    {
    hfStdoe = 0x02; // For Standard Error
    rc = DosDupHandle(hwrite,&hfStdoe);
    }

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;
}

// Rexx RxSi2H external function (Standard-In To Handle)
EXPORTSPEC ULONG APIENTRY  RxSi2H(PUCHAR name, ULONG numargs, RXSTRING args[],
              PSZ queuename, PRXSTRING retstr)
{

  HFILE   hread, hfStdin;
  APIRET  rc;            // Return code

  // Gotta have at least one argument
  if (numargs < 1)
    return INVALID_ROUTINE;

  if (args[0].strlength != sizeof(HFILE))
    return INVALID_ROUTINE;

  memcpy(&hread,args[0].strptr,args[0].strlength);

  hfStdin = 0x00; // For Standard In
  rc = DosDupHandle(hread,&hfStdin);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;
}

// Rexx RxLineInH external function (LineInput From File Handle)
EXPORTSPEC ULONG APIENTRY  RxLineInH(PUCHAR name, ULONG numargs, RXSTRING args[],
                 PSZ queuename, PRXSTRING retstr)
{

  FILE    *stream;
  HFILE   hfile;
  APIRET  rc;            // Return code
  PCH     pstr;

//  stream = stdin;
//  if (numargs >= 1)
//    {
//    if (args[0].strlength != sizeof(hfile))
//      return INVALID_ROUTINE;
//    memcpy(&hfile,args[0].strptr,args[0].strlength);
//    stream = fdopen(hfile,"r");
//    if ( !stream )
//      return INVALID_ROUTINE;
//    }

//  pstr = NULL;
//  pstr = fgets(retstr->strptr,250,stream);
//  if (pstr)
//    retstr->strlength = strlen(retstr->strptr);
//  else
//    retstr->strlength = 0;

  return VALID_ROUTINE;
}

// Rexx RxRSi2F external function
EXPORTSPEC ULONG APIENTRY  RxRSi2F(PUCHAR name, ULONG numargs, RXSTRING args[],
               PSZ queuename, PRXSTRING retstr)
{

  PSZ     pszFileName;
  HFILE   hfFileHandle, hfStdin;
  ULONG   ActionTaken;
  ULONG   ulFileSize = 0;
  ULONG   ulFileAttribute = FILE_NORMAL;
  ULONG   ulOpenFlag;
  ULONG   ulOpenMode;
  PEAOP2  pEABuf = NULL;
  APIRET  rc;            // Return code
  UCHAR   arswc;

  PCH nptr, hptr;
  ULONG nctr, hctr, fctr, npl;
  BOOL ic;

  // Gotta have at least one argument
  if (numargs < 1)
    return INVALID_ROUTINE;

  if (args[0].strlength == 0)
    return INVALID_ROUTINE;

  pszFileName = args[0].strptr;

  // Default is to append
  ulOpenFlag = OPEN_ACTION_FAIL_IF_NEW | OPEN_ACTION_OPEN_IF_EXISTS;

  ulOpenMode = OPEN_FLAGS_SEQUENTIAL |
               OPEN_FLAGS_NOINHERIT  |
               OPEN_SHARE_DENYNONE   |
               OPEN_ACCESS_READONLY;

  rc = DosOpen(pszFileName, &hfFileHandle,
               &ActionTaken, ulFileSize,
               ulFileAttribute, ulOpenFlag,
               ulOpenMode, pEABuf);
  if (!rc)
    {
    hfStdin = 0x00; // For Standard In
    rc = DosDupHandle(hfFileHandle,&hfStdin);
    }

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;
}

// RxOpen external function
EXPORTSPEC ULONG APIENTRY  RxOpen(PUCHAR name, ULONG numargs, RXSTRING args[],
              PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  HFILE       hfile;
  ULONG       ctr;
  PCH         cp;
  ULONG       ulActionTaken;
  ULONG       ulFileSize=0;
  ULONG       ulFileAttribute=0;
  ULONG       ulOpenFlag=0;
  ULONG       ulOpenMode=0;
  PEAOP2      pEABuf=NULL;
  SHVBLOCK    shvb;
  UCHAR       varname[251];
  UCHAR       varval[50];

  // Arg 1 = Rexx stem variable name
  // Arg 2 = File Name
  // Arg 3 = Open Flag
  // Arg 4 = Open Mode
  // Arg 5 = File Attribute
  // Arg 6 = File Size

  if (numargs < 4)
    return INVALID_ROUTINE;

  if (args[0].strlength < 1 || args[0].strlength > sizeof(varname)-2)
    return INVALID_ROUTINE;
  strupr(args[0].strptr);
  if (args[1].strlength < 1)
    return INVALID_ROUTINE;

  // OpenFlag
  strupr(args[2].strptr);
  for (ctr=1,cp=args[2].strptr;ctr<=args[2].strlength;ctr++,cp++)
    {
    switch (*cp)
      {
      case 'N':
        ulOpenFlag |= OPEN_ACTION_FAIL_IF_NEW;
        break;
      case 'C':
        ulOpenFlag |= OPEN_ACTION_CREATE_IF_NEW;
        break;
      case 'E':
        ulOpenFlag |= OPEN_ACTION_FAIL_IF_EXISTS;
        break;
      case 'O':
        ulOpenFlag |= OPEN_ACTION_OPEN_IF_EXISTS;
        break;
      case 'R':
        ulOpenFlag |= OPEN_ACTION_REPLACE_IF_EXISTS;
        break;
      default:
        return INVALID_ROUTINE;
        break;
      } // end-switch
    } // end-for

  // OpenMode
  for (ctr=1,cp=args[3].strptr;ctr<=args[3].strlength;ctr++,cp++)
    {
    switch (*cp)
      {
      case 'D':
        ulOpenMode |= OPEN_FLAGS_DASD;
        break;
      case 'T':
        ulOpenMode |= OPEN_FLAGS_WRITE_THROUGH;
        break;
      case 'F':
        ulOpenMode |= OPEN_FLAGS_FAIL_ON_ERROR;
        break;
      case 'C':
        ulOpenMode |= OPEN_FLAGS_NO_CACHE;
        break;
      case 'S':
        ulOpenMode |= OPEN_FLAGS_SEQUENTIAL;
        break;
      case 'X':
        ulOpenMode |= OPEN_FLAGS_RANDOM;
        break;
      case 'M':
        ulOpenMode |= OPEN_FLAGS_RANDOMSEQUENTIAL;
        break;
      case 'H':
        ulOpenMode |= OPEN_FLAGS_NOINHERIT;
        break;
      case 'b':
        ulOpenMode |= OPEN_SHARE_DENYREADWRITE;
        break;
      case 'w':
        ulOpenMode |= OPEN_SHARE_DENYWRITE;
        break;
      case 'r':
        ulOpenMode |= OPEN_SHARE_DENYREAD;
        break;
      case 'n':
        ulOpenMode |= OPEN_SHARE_DENYNONE;
        break;
      case 'R':
        ulOpenMode |= OPEN_ACCESS_READONLY;
        break;
      case 'W':
        ulOpenMode |= OPEN_ACCESS_WRITEONLY;
        break;
      case 'B':
        ulOpenMode |= OPEN_ACCESS_READWRITE;
        break;
      default:
        return INVALID_ROUTINE;
        break;
      } // end-switch
    } // end-for

  // FileAttribute
  if (numargs >= 5)
    {
    strupr(args[4].strptr);
    for (ctr=1,cp=args[4].strptr;ctr<=args[4].strlength;ctr++,cp++)
      {
      switch (*cp)
        {
        case 'A':
          ulFileAttribute |= FILE_ARCHIVED;
          break;
        case 'D':
          ulFileAttribute |= FILE_DIRECTORY;
          break;
        case 'S':
          ulFileAttribute |= FILE_SYSTEM;
          break;
        case 'H':
          ulFileAttribute |= FILE_HIDDEN;
          break;
        case 'R':
          ulFileAttribute |= FILE_READONLY;
          break;
        case 'N':
          ulFileAttribute |= FILE_NORMAL;
          break;
        default:
          return INVALID_ROUTINE;
          break;
        } // end-switch
      } // end-for
    } // end-if

  // FileSize
  if (numargs >= 6)
    {
    if (!args[5].strlength)
      return INVALID_ROUTINE;
    if ( !string2long(args[5].strlength,args[5].strptr,(PLONG)&ulFileSize) )
      return INVALID_ROUTINE;
    }

  memset(&shvb,0x00,sizeof(shvb));

  rc = DosOpen(args[1].strptr,&hfile,&ulActionTaken,
               ulFileSize,ulFileAttribute,ulOpenFlag,
               ulOpenMode,pEABuf);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  shvb.shvcode = RXSHV_DROPV;
  shvb.shvname.strptr = args[0].strptr;
  shvb.shvname.strlength = args[0].strlength;
  RexxVariablePool(&shvb);

  // Set up
  shvb.shvcode = RXSHV_SET;
  shvb.shvname.strptr = varname;

  // Set FileHandle
  sprintf(varname,"%s1",args[0].strptr);
  shvb.shvname.strlength = strlen(varname);
  shvb.shvvalue.strptr = (PCH)&hfile;
  shvb.shvvalue.strlength = sizeof(hfile);
  RexxVariablePool(&shvb);

  // Set ActionTaken
  shvb.shvname.strptr = varname;
  sprintf(varname,"%s2",args[0].strptr);
  shvb.shvname.strlength = strlen(varname);
  shvb.shvvalue.strptr = varval;
  switch (ulActionTaken)
    {
    case FILE_EXISTED:
      strcpy(varval,"Existed");
      break;
    case FILE_CREATED:
      strcpy(varval,"Created");
      break;
    case FILE_TRUNCATED:
      strcpy(varval,"Truncated");
      break;
    default:
      sprintf(varval,"%08x",ulActionTaken);
      break;
    }
  shvb.shvvalue.strlength = strlen(varval);
  RexxVariablePool(&shvb);

  return VALID_ROUTINE;

}

// RxDupHandle external function
EXPORTSPEC ULONG APIENTRY  RxDupHandle(PUCHAR name, ULONG numargs, RXSTRING args[],
                   PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  HFILE       hfile1, hfile2;
  SHVBLOCK    shvb;

  // Arg 1 = Rexx variable name
  // Arg 2 = File Handle 1
  // Arg 3 = File Handle 2

  if (numargs < 3)
    return INVALID_ROUTINE;

  if (args[0].strlength == 0)
    return INVALID_ROUTINE;

  if (args[1].strlength != sizeof(HFILE) || args[2].strlength != sizeof(HFILE))
    return INVALID_ROUTINE;

  strupr(args[0].strptr);
  memset(&shvb,0x00,sizeof(shvb));

  memcpy(&hfile1,args[1].strptr,args[1].strlength);
  memcpy(&hfile2,args[2].strptr,args[2].strlength);

  shvb.shvvalue.strptr = (PCH)&hfile2;
  shvb.shvvalue.strlength = sizeof(hfile2);

  rc = DosDupHandle(hfile1,&hfile2);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  shvb.shvcode = RXSHV_SET;
  shvb.shvname.strptr = args[0].strptr;
  shvb.shvname.strlength = args[0].strlength;
  RexxVariablePool(&shvb);

  return VALID_ROUTINE;

}

// RxSetMaxFH external function
EXPORTSPEC ULONG APIENTRY  RxSetMaxFH(PUCHAR name, ULONG numargs, RXSTRING args[],
                  PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  ULONG       ulNumFH=0;

  // Arg 1 = Number of file handles

  if (numargs < 1)
    return INVALID_ROUTINE;

  if (!args[0].strlength)
    return INVALID_ROUTINE;

  ulNumFH = atoi(args[0].strptr);

  rc = DosSetMaxFH(ulNumFH);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;

}

// RxSetRelMaxFH external function
EXPORTSPEC ULONG APIENTRY  RxSetRelMaxFH(PUCHAR name, ULONG numargs, RXSTRING args[],
                     PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  long        lNumIncrementalFH=0;
  ULONG       ulNumCurrentFH=0;
  char        tempbuf[40];

  // Arg 1 = Incremental number of file handles

  if (numargs < 1)
    return INVALID_ROUTINE;

  if (!args[0].strlength)
    return INVALID_ROUTINE;

  lNumIncrementalFH = atoi(args[0].strptr);

  rc = DosSetRelMaxFH(&lNumIncrementalFH,&ulNumCurrentFH);

  retstr->strlength = sprintf(retstr->strptr,"%ld %ld",rc,ulNumCurrentFH);

  return VALID_ROUTINE;

}

// RxQueryFHState external function
EXPORTSPEC ULONG APIENTRY  RxQueryFHState(PUCHAR name, ULONG numargs, RXSTRING args[],
                      PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  HFILE       hfile;
  ULONG       ulFlags=0;

  // Arg 1 = File Handle

  if (numargs < 1)
    return INVALID_ROUTINE;

  if (args[0].strlength != sizeof(HFILE))
    return INVALID_ROUTINE;

  memcpy(&hfile,args[0].strptr,args[0].strlength);

  rc = DosQueryFHState(hfile,&ulFlags);

  sprintf(retstr->strptr,"%ld",rc);
  if (!rc)
    {
    strcat(retstr->strptr," ");
    if (ulFlags & OPEN_FLAGS_DASD)
      strcat(retstr->strptr,"D");
    if (ulFlags & OPEN_FLAGS_WRITE_THROUGH)
      strcat(retstr->strptr,"T");
    if (ulFlags & OPEN_FLAGS_FAIL_ON_ERROR)
      strcat(retstr->strptr,"F");
    if (ulFlags & OPEN_FLAGS_NO_CACHE)
      strcat(retstr->strptr,"C");
    if (ulFlags & OPEN_FLAGS_NOINHERIT)
      strcat(retstr->strptr,"H");
    if (ulFlags & OPEN_SHARE_DENYREADWRITE)
      strcat(retstr->strptr,"b");
    if (ulFlags & OPEN_SHARE_DENYWRITE)
      strcat(retstr->strptr,"w");
    if (ulFlags & OPEN_SHARE_DENYREAD)
      strcat(retstr->strptr,"r");
    if (ulFlags & OPEN_SHARE_DENYNONE)
      strcat(retstr->strptr,"n");
    if (ulFlags & OPEN_ACCESS_READONLY)
      strcat(retstr->strptr,"R");
    if (ulFlags & OPEN_ACCESS_WRITEONLY)
      strcat(retstr->strptr,"W");
    if (ulFlags & OPEN_ACCESS_READWRITE)
      strcat(retstr->strptr,"B");
    }
  retstr->strlength = strlen(retstr->strptr);

  return VALID_ROUTINE;

}

// RxSetFHState external function
EXPORTSPEC ULONG APIENTRY  RxSetFHState(PUCHAR name, ULONG numargs, RXSTRING args[],
                    PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  HFILE       hfile;
  ULONG       ulOpenMode=0;
  PCH         cp;
  ULONG       ctr;

  // Arg 1 = File Handle 1
  // Arg 2 = State Flags

  if (numargs < 2)
    return INVALID_ROUTINE;

  if (args[0].strlength != sizeof(HFILE))
    return INVALID_ROUTINE;

  if (args[1].strlength == 0)
    return INVALID_ROUTINE;

  memcpy(&hfile,args[0].strptr,args[0].strlength);

  for (ctr=1,cp=args[1].strptr;ctr<=args[1].strlength;ctr++,cp++)
    {
    switch (*cp)
      {
      case 'D':
        ulOpenMode |= OPEN_FLAGS_DASD;
        break;
      case 'T':
        ulOpenMode |= OPEN_FLAGS_WRITE_THROUGH;
        break;
      case 'F':
        ulOpenMode |= OPEN_FLAGS_FAIL_ON_ERROR;
        break;
      case 'C':
        ulOpenMode |= OPEN_FLAGS_NO_CACHE;
        break;
      case 'S':
        ulOpenMode |= OPEN_FLAGS_SEQUENTIAL;
        break;
      case 'X':
        ulOpenMode |= OPEN_FLAGS_RANDOM;
        break;
      case 'M':
        ulOpenMode |= OPEN_FLAGS_RANDOMSEQUENTIAL;
        break;
      case 'H':
        ulOpenMode |= OPEN_FLAGS_NOINHERIT;
        break;
      case 'b':
        ulOpenMode |= OPEN_SHARE_DENYREADWRITE;
        break;
      case 'w':
        ulOpenMode |= OPEN_SHARE_DENYWRITE;
        break;
      case 'r':
        ulOpenMode |= OPEN_SHARE_DENYREAD;
        break;
      case 'n':
        ulOpenMode |= OPEN_SHARE_DENYNONE;
        break;
      case 'R':
        ulOpenMode |= OPEN_ACCESS_READONLY;
        break;
      case 'W':
        ulOpenMode |= OPEN_ACCESS_WRITEONLY;
        break;
      case 'B':
        ulOpenMode |= OPEN_ACCESS_READWRITE;
        break;
      default:
        return INVALID_ROUTINE;
        break;
      } // end-switch
    }

  rc = DosSetFHState(hfile,ulOpenMode);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;

}

// RxDosRead external function
EXPORTSPEC ULONG APIENTRY  RxDosRead(PUCHAR name, ULONG numargs, PRXSTRING pargs,
                 PSZ queuename, PRXSTRING retstr)
{
  return RxRead(name,numargs,pargs,queuename,retstr);
}

// RxRead external function
EXPORTSPEC ULONG APIENTRY  RxRead(PUCHAR name, ULONG numargs, RXSTRING args[],
              PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  HFILE       hfile;
  SHVBLOCK    shvb;
  ULONG       ulBufLen = 0, ulBytesRead = 0;

  // Arg 1 = Rexx variable name
  // Arg 2 = File Handle
  // Arg 3 = Length to read

  if (numargs < 2)
    return INVALID_ROUTINE;

  if (args[0].strlength == 0 || args[1].strlength != sizeof(HFILE))
    return INVALID_ROUTINE;

  strupr(args[0].strptr);
  memset(&shvb,0x00,sizeof(shvb));

  memcpy(&hfile,args[1].strptr,args[1].strlength);

  if (numargs >=3 )
    {
    if (! string2long(args[2].strlength,args[2].strptr,(PLONG) &ulBufLen) )
      return INVALID_ROUTINE;
    shvb.shvvalue.strptr = (PCH) malloc(ulBufLen);
    if ( !shvb.shvvalue.strptr )
      return INVALID_ROUTINE;
    }

  rc = DosRead(hfile, shvb.shvvalue.strptr, ulBufLen, &ulBytesRead);

  retstr->strlength = sprintf(retstr->strptr,"%ld %ld",rc,ulBytesRead);

  shvb.shvcode = RXSHV_SET;
  shvb.shvname.strptr = args[0].strptr;
  shvb.shvname.strlength = args[0].strlength;
  shvb.shvvalue.strlength = ulBytesRead;
  RexxVariablePool(&shvb);
  free(shvb.shvvalue.strptr);

  return VALID_ROUTINE;

}

// RxDosWrite external function
EXPORTSPEC ULONG APIENTRY  RxDosWrite(PUCHAR name, ULONG numargs, PRXSTRING pargs,
                  PSZ queuename, PRXSTRING retstr)
{
  return RxWrite(name,numargs,pargs,queuename,retstr);
}

// RxWrite external function
EXPORTSPEC ULONG APIENTRY  RxWrite(PUCHAR name, ULONG numargs, RXSTRING args[],
               PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  HFILE       hfile;
  PVOID       pBufPtr;
  UCHAR       dbuf[1];
  ULONG       ulBufLen = 0, ulBytesWritten = 0;

  // Arg 1 = File Handle
  // Arg 2 = Data to write

  if (numargs < 1)
    return INVALID_ROUTINE;

  if (args[0].strlength != sizeof(HFILE))
    return INVALID_ROUTINE;
  memcpy(&hfile,args[0].strptr,args[0].strlength);

  if (numargs >=2 )
    {
    pBufPtr = args[1].strptr;
    ulBufLen = args[1].strlength;
    }
  else
    {
    pBufPtr = dbuf;
    ulBufLen = 0;
    }

  rc = DosWrite(hfile, pBufPtr, ulBufLen, &ulBytesWritten);
  retstr->strlength = sprintf(retstr->strptr,"%ld %ld",rc,ulBytesWritten);

  return VALID_ROUTINE;

}

// RxCloseH external function
EXPORTSPEC ULONG APIENTRY  RxCloseH(PUCHAR name, ULONG numargs, RXSTRING args[],
                PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  HFILE       hfile;

  if (numargs < 1)
    {
    printf("Arg 1 must be a valid file handle\n");
    return INVALID_ROUTINE;
    }

  if (args[0].strlength != sizeof(HFILE))
    {
    printf("Arg 1 must a valid file handle (%ld bytes)\n",sizeof(HFILE));
    return INVALID_ROUTINE;
    }
  memcpy(&hfile,args[0].strptr,args[0].strlength);

  rc = DosClose(hfile);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);
  return VALID_ROUTINE;

}

// RxQuerySysInfo external function
EXPORTSPEC ULONG APIENTRY  RxQuerySysInfo(PUCHAR name, ULONG numargs, RXSTRING args[],
                      PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  ULONG       ulStartIdx = 1, ulEndIdx = 26, ctr, *pint;
  PCH         hptr;
  SHVBLOCK    shvb;
  UCHAR       varname[250], varval[250];

  if (numargs < 1)
    return INVALID_ROUTINE;
  if (args[0].strlength == 0)
    return INVALID_ROUTINE;
  strupr(args[0].strptr);

  if (numargs >=2 )
    {
    if (! string2long(args[1].strlength,args[1].strptr,(PLONG) &ulStartIdx) )
      {
      printf("Arg 2 is invalid integer\n");
      return INVALID_ROUTINE;
      }
    }

  if (numargs >=3 )
    {
    if (! string2long(args[2].strlength,args[2].strptr,(PLONG) &ulEndIdx) )
      {
      printf("Arg 3 is invalid integer\n");
      return INVALID_ROUTINE;
      }
    }

  hptr = (PCH) malloc(300);

  rc = DosQuerySysInfo(ulStartIdx, ulEndIdx, hptr, 300);

  if (rc == 0)
    {
    shvb.shvnext = (PSHVBLOCK) NULL;
    shvb.shvcode = RXSHV_SET;
    shvb.shvname.strptr = varname;
    shvb.shvvalue.strptr = varval;
    pint = (PULONG) hptr;
    for (ctr = ulStartIdx;ctr <= ulEndIdx;ctr++)
      {
      sprintf(varname,"%s%ld",args[0].strptr,ctr);
      shvb.shvname.strlength = strlen(varname);
      sprintf(varval,"%ld",*pint);
      shvb.shvvalue.strlength = strlen(varval);
      RexxVariablePool(&shvb);
      pint++;
      }
    }
  free(hptr);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);
  return VALID_ROUTINE;

}

// RxTmrQueryFreq external function
EXPORTSPEC ULONG APIENTRY  RxTmrQueryFreq(PUCHAR name, ULONG numargs, RXSTRING args[],
                      PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  ULONG       tmrfreq=0;

  rc=DosTmrQueryFreq(&tmrfreq);

  if (rc)
    retstr->strlength = sprintf(retstr->strptr,"%ld",rc);
  else
    retstr->strlength = sprintf(retstr->strptr,"%ld %ld",rc,tmrfreq);

  return VALID_ROUTINE;

}

// RxTmrQueryTime external function
EXPORTSPEC ULONG APIENTRY  RxTmrQueryTime(PUCHAR name, ULONG numargs, RXSTRING args[],
                      PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  QWORD       tmrtime;

  rc=DosTmrQueryTime(&tmrtime);

  if (rc)
    retstr->strlength = sprintf(retstr->strptr,"%lu",rc);
  else
    retstr->strlength = sprintf(retstr->strptr,"%ld %lu.%lu",rc,tmrtime.ulHi,tmrtime.ulLo);

  return VALID_ROUTINE;

}

// RxDevConfig external function
EXPORTSPEC ULONG APIENTRY  RxDevConfig(PUCHAR name, ULONG numargs, RXSTRING args[],
                   PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  PCH         pdevinfo=(PCH)NULL;
  ULONG       devinfolen=0;
  ULONG       ulDevType=0;

  // Arg #1 = name of rexx variable in which config info is returned
  // Arg #2 = integer identifying what type of device info to return
  // Arg #3 = (optional) length of buffer into which info for device
  //          type > 6 is returned

  if (numargs < 2)
    return INVALID_ROUTINE;
  if (args[0].strlength == 0 || args[1].strlength == 0)
    return INVALID_ROUTINE;
  strupr(args[0].strptr);
  ulDevType = atoi(args[1].strptr);
  if (ulDevType <= 6)
    devinfolen = 1;
  else
    {
    if (numargs < 3)
      return INVALID_ROUTINE;
    devinfolen = atoi(args[2].strptr);
    }
  if (!devinfolen)
    return INVALID_ROUTINE;
  pdevinfo = (PCH) malloc(devinfolen);

  rc = DosDevConfig((PVOID)pdevinfo,ulDevType);

  if (!rc)
    {
    SHVBLOCK    shvb;
    shvb.shvnext = (PSHVBLOCK) NULL;
    shvb.shvcode = RXSHV_SET;
    shvb.shvname.strptr = args[0].strptr;
    shvb.shvvalue.strptr = pdevinfo;
    shvb.shvname.strlength = args[0].strlength;
    shvb.shvvalue.strlength = devinfolen;
    RexxVariablePool(&shvb);
    }

  free(pdevinfo);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);
  return VALID_ROUTINE;

}

// RxPhysicalDisk external function
EXPORTSPEC ULONG APIENTRY  RxPhysicalDisk(PUCHAR name, ULONG numargs, RXSTRING args[],
                      PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  PCH         pretbuf=NULL, pparm=NULL;
  ULONG       function=0, retbuflen=0, parmbuflen=0;

  // Arg #1 = function code (1,2,3)
  // Arg #2 = name of rexx variable in which physical disk info is returned
  // Arg #3 = required length of return buffer
  // Arg #4 = (optional) parameter to be passed to API

  if (numargs < 3)
    return INVALID_ROUTINE;
  function = atoi(args[0].strptr);
  strupr(args[1].strptr);
  retbuflen = atoi(args[2].strptr);
  if (retbuflen)
    pretbuf = (PCH) malloc(retbuflen);
  if (numargs >= 4 && args[3].strlength)
    {
    pparm = args[3].strptr;
    parmbuflen = args[3].strlength;
    }

  rc = DosPhysicalDisk(function,(PVOID)pretbuf,retbuflen,(PVOID)pparm,parmbuflen);

  if (!rc)
    {
    SHVBLOCK    shvb;
    shvb.shvnext = (PSHVBLOCK) NULL;
    shvb.shvcode = RXSHV_SET;
    shvb.shvname.strptr = args[1].strptr;
    shvb.shvvalue.strptr = pretbuf;
    shvb.shvname.strlength = args[1].strlength;
    shvb.shvvalue.strlength = retbuflen;
    RexxVariablePool(&shvb);
    }

  if (pretbuf)
    free(pretbuf);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);
  return VALID_ROUTINE;

}

// RxDevIOCtl external function
EXPORTSPEC ULONG APIENTRY  RxDevIOCtl(PUCHAR name, ULONG numargs, RXSTRING args[],
                  PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  HFILE       devhandle=0;
  ULONG       category=0;
  ULONG       function=0;
  PVOID       pparmlist=(PVOID)NULL;
  ULONG       parmlenmax=0;
  ULONG       parmleninout=0;
  PVOID       pdataarea=(PVOID)NULL;
  ULONG       datalenmax=0;
  ULONG       dataleninout=0;
  ULONG       parmretsize=0;
  ULONG       dataretsize=0;
  ULONG       ctr;
  SHVBLOCK    shvb;

  // Arg #1 = name of rexx stem variable under which data is returned as such:
  //          stem.1 = parmlist returned
  //          stem.2 = dataarea returned
  // Arg #2 = device handle (obtained from prior DosOpen)
  // Arg #3 = integer indicating category
  // Arg #4 = integer indicating function
  // Arg #5 = parmlist
  // Arg #6 = integer indicating length of returned parmlist
  // Arg #7 = dataarea
  // Arg #8 = integer indicating length of returned dataarea

  if (numargs < 4)
    return INVALID_ROUTINE;
  for (ctr=0;ctr<4;ctr++)
    {
    if (args[ctr].strlength == 0)
      return INVALID_ROUTINE;
    }
  strupr(args[0].strptr);

  if (args[1].strlength != sizeof(HFILE))
    return INVALID_ROUTINE;
  memcpy(&devhandle,args[1].strptr,sizeof(devhandle));
  category = atoi(args[2].strptr);
  function = atoi(args[3].strptr);

  // Set up for parmlist
  if (numargs >= 5 && args[4].strlength)
    {
    if (numargs >= 6 && args[5].strlength)
      parmretsize = atoi(args[5].strptr);
    parmleninout = args[4].strlength;
    parmlenmax = max(args[4].strlength,parmretsize);
    pparmlist = (PVOID) malloc(parmlenmax);
    memcpy(pparmlist,args[4].strptr,args[4].strlength);
    } // end-if parmlist implicitly supplied

  // Set up for dataarea
  if (numargs >= 7 && args[6].strlength)
    {
    if (numargs >= 8 && args[7].strlength)
      dataretsize = atoi(args[7].strptr);
    dataleninout = args[6].strlength;
    datalenmax = max(args[6].strlength,dataretsize);
    pdataarea = (PVOID) malloc(datalenmax);
    memcpy(pdataarea,args[6].strptr,args[6].strlength);
    } // end-if dataarea explicitly supplied

  rc = DosDevIOCtl(devhandle,category,function,
                   pparmlist,parmlenmax,&parmleninout,
                   pdataarea,datalenmax,&dataleninout
                  );

  // Set up for setting rexx variable value
  memset(&shvb,0x00,sizeof(shvb));
  shvb.shvcode = RXSHV_SET;
  shvb.shvname.strptr = (PCH) malloc(args[0].strlength + 10);

  switch (rc)
    {
    case NO_ERROR:
      {
      // Set parmlist returned
      if (pparmlist)
        {
        sprintf(shvb.shvname.strptr,"%s1",args[0].strptr);
        shvb.shvvalue.strptr = (PCH)pparmlist;
        shvb.shvname.strlength = strlen(shvb.shvname.strptr);
        shvb.shvvalue.strlength = parmleninout;
        RexxVariablePool(&shvb);
        } // end-if (parmlist requested)
      // Set dataarea returned
      if (pdataarea)
        {
        sprintf(shvb.shvname.strptr,"%s2",args[0].strptr);
        shvb.shvvalue.strptr = (PCH)pdataarea;
        shvb.shvname.strlength = strlen(shvb.shvname.strptr);
        shvb.shvvalue.strlength = dataleninout;
        RexxVariablePool(&shvb);
        } // end-if (dataarea requested)
      free(shvb.shvname.strptr);
      }
      break;
    case ERROR_BUFFER_OVERFLOW:
      {
      UCHAR       valbuf[50];
      shvb.shvvalue.strptr = valbuf;
      // Set parmlist returned
      if (pparmlist)
        {
        sprintf(shvb.shvname.strptr,"%s1",args[0].strptr);
        sprintf(valbuf,"%ld %ld",parmlenmax,parmleninout);
        shvb.shvname.strlength = strlen(shvb.shvname.strptr);
        shvb.shvvalue.strlength = strlen(valbuf);
        RexxVariablePool(&shvb);
        } // end-if (parmlist data requested)
      // Set dataarea returned
      if (pdataarea)
        {
        sprintf(shvb.shvname.strptr,"%s2",args[0].strptr);
        sprintf(valbuf,"%ld %ld",datalenmax,dataleninout);
        shvb.shvname.strlength = strlen(shvb.shvname.strptr);
        shvb.shvvalue.strlength = strlen(valbuf);
        RexxVariablePool(&shvb);
        } // end-if (dataarea requested)
      free(shvb.shvname.strptr);
      }
      break;
    default:
      break;
    } // end-switch (on return code)

  free(pparmlist);
  free(pdataarea);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);
  return VALID_ROUTINE;

}

// RxExecPgm external function
EXPORTSPEC ULONG APIENTRY  RxExecPgm(PUCHAR name, ULONG numargs, RXSTRING args[],
                 PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  UCHAR       szErrBuf[100];
  ULONG       cctr, ulExecFlags = 0;
  PCH         cptr;
  PSZ         pArgs = NULL, pEnvStr = NULL;
  RESULTCODES ReturnCodes;

  if (numargs < 1)
    {
    printf("Arg 1 must be the name of a program to execute\n");
    return INVALID_ROUTINE;
    }

  if (args[0].strlength == 0)
    {
    printf("Arg 1 must be the name of a program to execute\n");
    return INVALID_ROUTINE;
    }

  if (numargs >= 2)
    {
    if (args[1].strlength > 0)
      {
      cptr = args[1].strptr;
      for (cctr=1;cctr <= args[1].strlength;cctr++)
        {
        switch (*cptr)
          {
          case 's':
          case 'S':
            ulExecFlags |= EXEC_SYNC;
            break;
          case 'a':
          case 'A':
            ulExecFlags |= EXEC_ASYNC;
            break;
          case 'r':
          case 'R':
            ulExecFlags |= EXEC_ASYNCRESULT;
            break;
          case 't':
          case 'T':
            ulExecFlags |= EXEC_TRACE;
            break;
          case 'b':
          case 'B':
            ulExecFlags |= EXEC_BACKGROUND;
            break;
          case 'l':
          case 'L':
            ulExecFlags |= EXEC_LOAD;
            break;
          case 'd':
          case 'D':
            ulExecFlags |= EXEC_ASYNCRESULTDB;
            break;
          default:
            printf("Arg 2, if supplied, must be:\n");
            printf("'S'ync, 'A'sync, async'R'esult, 'T'race, 'B'ackground\n");
            printf("'L'oad, asyncresult'D'b\n");
            return INVALID_ROUTINE;
            break;
          } // end-switch
        cptr++;
        } // end-for
      } // end-if
    } // end-if

  if (numargs >= 3)
    pArgs = args[2].strptr;

  ReturnCodes.codeTerminate = 0;
  ReturnCodes.codeResult = 0;
  memset(szErrBuf,0x00,sizeof(szErrBuf));

  rc = DosExecPgm(szErrBuf,             // Error string buffer
                  sizeof(szErrBuf),     // Length of error string buffer
                  ulExecFlags,          // Execution Flags
                  pArgs,                // Argument string
                  pEnvStr,              // Environment string
                  &ReturnCodes,         // Termination codes
                  args[0].strptr);      // Program name

  if (rc)
    retstr->strlength = sprintf(retstr->strptr,"%ld %.*s",rc,sizeof(szErrBuf),szErrBuf);
  else
    retstr->strlength = sprintf(retstr->strptr,"%ld %ld %ld",rc,ReturnCodes.codeTerminate,
                                ReturnCodes.codeResult);

  return VALID_ROUTINE;

}

// RxWaitChild external function
EXPORTSPEC ULONG APIENTRY  RxWaitChild(PUCHAR name, ULONG numargs, RXSTRING args[],
                   PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  ULONG       actioncode, waitoption;
  PID         childpid,retpid=0;
  RESULTCODES ResultCodes;

  // Arg #1 = child PID
  // Arg #2 = action code
  // Arg #3 = wait option

  if (numargs < 3)
    return INVALID_ROUTINE;

  if (args[0].strlength == 0 ||
      args[1].strlength == 0 ||
      args[2].strlength == 0 )
    return INVALID_ROUTINE;

  childpid = atoi(args[0].strptr);

  switch (toupper(*args[1].strptr))
    {
    case 'P':
      actioncode = DCWA_PROCESS;
      break;
    case 'T':
      actioncode = DCWA_PROCESSTREE;
      break;
    default:
      return INVALID_ROUTINE;
      break;
    }

  switch (toupper(*args[2].strptr))
    {
    case 'W':
      waitoption = DCWW_WAIT;
      break;
    case 'N':
      waitoption = DCWW_NOWAIT;
      break;
    default:
      return INVALID_ROUTINE;
      break;
    }

  ResultCodes.codeTerminate = 0;
  ResultCodes.codeResult = 0;

  rc=DosWaitChild(actioncode,waitoption,&ResultCodes,&retpid,childpid);

  sprintf(retstr->strptr,"%ld %ld",rc,retpid);
  if (!rc)
    {
    UCHAR tempbuf[50];
    sprintf(tempbuf," %ld",ResultCodes.codeResult);
    strcat(retstr->strptr,tempbuf);
    switch (ResultCodes.codeTerminate)
      {
      case TC_EXIT:
        strcat(retstr->strptr," NormalExit");
        break;
      case TC_HARDERROR:
        strcat(retstr->strptr," HardError");
        break;
      case TC_TRAP:
        strcat(retstr->strptr," Trap");
        break;
      case TC_KILLPROCESS:
        strcat(retstr->strptr," KillProcess");
        break;
      case TC_EXCEPTION:
        strcat(retstr->strptr," Exception");
        break;
      default:
        sprintf(tempbuf," %ld",ResultCodes.codeTerminate);
        strcat(retstr->strptr,tempbuf);
        break;
      } // end-switch on termination code
    } // end-if dosrc = 0

  retstr->strlength = strlen(retstr->strptr);
  return VALID_ROUTINE;

}

// RxDetachRexxPgm external function
EXPORTSPEC ULONG APIENTRY  RxDetachRexxPgm(PUCHAR name, ULONG numargs, RXSTRING args[],
                       PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  UCHAR       szErrBuf[251];
  ULONG       ctr, ulExecFlags = 0;
  PCH         cptr;
  RESULTCODES ReturnCodes;
  UCHAR       CreatedName[251];
  UCHAR       uchArgBuf[300];
  BOOL        bDupName;

  if (numargs < 1)
    {
    printf("Arg 1 must be the name of a Rexx program\n");
    return INVALID_ROUTINE;
    }

  if (args[0].strlength == 0)
    {
    printf("Arg 1 must be the name of a Rexx program\n");
    return INVALID_ROUTINE;
    }

  // Create a Rexx queue (name is system generated)
  memset(CreatedName,0x00,sizeof(CreatedName));
  bDupName = FALSE;
  rc = RexxCreateQueue(CreatedName, sizeof(CreatedName),
                       NULL, (BOOL *) &bDupName);
  if (rc)
    {
    printf("RexxCreateQueue failed with rc = %ld\n",rc);
    return INVALID_ROUTINE;
    }

  memset(uchArgBuf,0x00,sizeof(uchArgBuf));
  cptr = (PCH) uchArgBuf;
  memset(cptr,0x20,1);
  cptr++;
  cptr++;
  memcpy(cptr,CreatedName,strlen(CreatedName));

  // Queue Rexx program name/string and args for "RXSRS.EXE"
  // to receive on queue
  for (ctr=0;ctr < numargs;ctr++)
    rc = RexxAddQueue(CreatedName,(PRXSTRING) &args[ctr], RXQUEUE_FIFO);

  ulExecFlags |= EXEC_BACKGROUND;

  ReturnCodes.codeTerminate = 0;
  ReturnCodes.codeResult = 0;
  memset(szErrBuf,0x00,sizeof(szErrBuf));

  rc = DosExecPgm(szErrBuf,             // Error string buffer
                  sizeof(szErrBuf)-1,   // Length of error string buffer
                  ulExecFlags,          // Execution Flags
                  uchArgBuf,            // Argument string
                  NULL,                 // Environment string
                  &ReturnCodes,         // Termination codes
                  "RXSRS.EXE");         // Program name

  if (rc)
    {
    retstr->strlength = sprintf(retstr->strptr,"%ld %s",rc,szErrBuf);
    rc = RexxDeleteQueue((PSZ)CreatedName);
    }
  else
    retstr->strlength = sprintf(retstr->strptr,"%ld %ld %ld",rc,
                                ReturnCodes.codeTerminate,
                                ReturnCodes.codeResult);

  return VALID_ROUTINE;

}

// RxStartSession external function
EXPORTSPEC ULONG APIENTRY  RxStartSession(PUCHAR name, ULONG numargs, RXSTRING args[],
                      PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  STARTDATA   sd, *psd;
  UCHAR       szErrBuf[100];
  ULONG       cctr, ulExecFlags = 0;
  ULONG       SessID = 0;
  PID         Pid = 0;
  PCH         cptr;

  if (numargs < 1)
    return INVALID_ROUTINE;

  // If they passed a pointer to their own STARTDATA struct, then use that instead
  if (numargs==1 && args[0].strlength==sizeof(PVOID))
    memcpy(&psd,args[0].strptr,args[0].strlength);
  else
    {
    psd = &sd;
    memset(&sd,0x00,sizeof(sd));
    sd.Length = sizeof(sd);
    sd.PgmName = args[0].strptr;
    if (numargs >= 2)
      sd.PgmInputs = args[1].strptr;
    if (numargs >= 3)
      {
      if (args[2].strlength > 0)
        {
        switch (*(args[2].strptr))
          {
          case 'i':
          case 'I':
            sd.Related = SSF_RELATED_INDEPENDENT;
            break;
          case 'c':
          case 'C':
            sd.Related = SSF_RELATED_CHILD;
            break;
          default:
            return INVALID_ROUTINE;
            break;
          }
        }
      }

    // Foreground/Background option
    if (numargs >= 4)
      {
      if (args[3].strlength > 0)
        {
        switch (*(args[3].strptr))
          {
          case 'f':
          case 'F':
            sd.FgBg = SSF_FGBG_FORE;
            break;
          case 'b':
          case 'B':
            sd.FgBg = SSF_FGBG_BACK;
            break;
          default:
            return INVALID_ROUTINE;
            break;
          }
        }
      }

    // Program title option
    if (numargs >= 5)
      {
      if (args[4].strlength > 0)
        sd.PgmTitle = args[4].strptr;
      }

    sd.TraceOpt = SSF_TRACEOPT_NONE;
    sd.TermQ = 0;          // Assume no termination queue
    sd.Environment = NULL; // Assume no environment string
    sd.InheritOpt = SSF_INHERTOPT_PARENT;

    // Program type option
    if (numargs >= 6)
      {
      if (args[5].strlength > 0)
        {
        switch (*(args[5].strptr))
          {
          case 'd':
          case 'D':
            sd.SessionType = SSF_TYPE_DEFAULT;
            break;
          case 'f':
          case 'F':
            sd.SessionType = SSF_TYPE_FULLSCREEN;
            break;
          case 'p':
          case 'P':
            sd.SessionType = SSF_TYPE_PM;
            break;
          case 'v':
          case 'V':
            sd.SessionType = SSF_TYPE_VDM;
            break;
          case 'w':
          case 'W':
            sd.SessionType = SSF_TYPE_WINDOWEDVDM;
            break;
          case 'o':
          case 'O':
            sd.SessionType = SSF_TYPE_WINDOWABLEVIO;
            break;
          default:
            return INVALID_ROUTINE;
            break;
          }
        }
      }

    sd.IconFile = 0;
    sd.PgmHandle = 0;

    if (numargs >= 7)
      {
      if (args[6].strlength > 0)
        {
        cptr = args[6].strptr;
        for (cctr=1;cctr <= args[6].strlength;cctr++)
          {
          switch (*cptr)
            {
            case 'v':
            case 'V':
              sd.PgmControl |= SSF_CONTROL_VISIBLE;
              break;
            case 'i':
            case 'I':
              sd.PgmControl |= SSF_CONTROL_INVISIBLE;
              break;
            case 'x':
            case 'X':
              sd.PgmControl |= SSF_CONTROL_MAXIMIZE;
              break;
            case 'n':
            case 'N':
              sd.PgmControl |= SSF_CONTROL_MINIMIZE;
              break;
            case 'a':
            case 'A':
              sd.PgmControl |= SSF_CONTROL_NOAUTOCLOSE;
              break;
            default:
              return INVALID_ROUTINE;
              break;
            } // end-switch
          cptr++;
          } // end-for
        } // end-if
      } // end-if

    sd.InitXPos = 0;
    sd.InitYPos = 0;
    sd.InitXSize = 0;    // Initial window coordinates
    sd.InitYSize = 0;    // and size
    sd.Reserved = 0;
    sd.ObjectBuffer = szErrBuf;
    sd.ObjectBuffLen = sizeof(szErrBuf);
    memset(szErrBuf,0x00,sizeof(szErrBuf));
    } // end-else

  rc = DosStartSession(psd,&SessID,&Pid);

  if (rc)
    retstr->strlength = sprintf(retstr->strptr,"%ld %.*s",rc,sizeof(szErrBuf),szErrBuf);
  else
    retstr->strlength = sprintf(retstr->strptr,"%ld %ld %ld",rc,SessID,Pid);

  return VALID_ROUTINE;

}

// RxAllocMem external function
EXPORTSPEC ULONG APIENTRY  RxAllocMem(PUCHAR name, ULONG numargs, RXSTRING args[],
                  PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  LONG        memsize, cctr;
  PVOID       memptr;
  ULONG       ulFlags = 0;
  PCH         cptr;
  SHVBLOCK    shvb;

  if (numargs < 3)
    return INVALID_ROUTINE;

  // Get name of Rexx variable into which pointer will be put
  if (args[0].strlength > 0)
    {
    memset(&shvb,0x00,sizeof(shvb));
    shvb.shvcode = RXSHV_SET;
    strupr(args[0].strptr);
    shvb.shvname.strptr = args[0].strptr;
    shvb.shvname.strlength = args[0].strlength;
    shvb.shvvalue.strptr = (PVOID) &memptr;
    shvb.shvvalue.strlength = sizeof(memptr);
    }
  else
    return INVALID_ROUTINE;

  if (! string2long(args[1].strlength,args[1].strptr, &memsize) )
    return INVALID_ROUTINE;

  // Get allocation flags
  if (args[2].strlength > 0)
    {
    cptr = args[2].strptr;
    for (cctr=1;cctr <= args[2].strlength;cctr++)
      {
      switch (*cptr)
        {
        case 'c':
        case 'C':
          ulFlags |= PAG_COMMIT;
          break;
        case 't':
        case 'T':
          ulFlags |= OBJ_TILE;
          break;
        case 'x':
        case 'X':
          ulFlags |= PAG_EXECUTE;
          break;
        case 'r':
        case 'R':
          ulFlags |= PAG_READ;
          break;
        case 'w':
        case 'W':
          ulFlags |= PAG_WRITE;
          break;
        case 'g':
        case 'G':
          ulFlags |= PAG_GUARD;
          break;
        default:
          return INVALID_ROUTINE;
          break;
        } // end-switch
      cptr++;
      } // end-for
    } // end-if

  rc = DosAllocMem((PPVOID) &memptr,memsize,ulFlags);

  RexxVariablePool(&shvb);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;

}

// RxAllocSharedMem external function
EXPORTSPEC ULONG APIENTRY  RxAllocSharedMem(PUCHAR name, ULONG numargs, RXSTRING args[],
                        PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  LONG        memsize, cctr;
  ULONG       ulFlags = 0;
  PVOID       memptr;
  PCH         pmemname, cptr;
  SHVBLOCK    shvb;

  // Gotta have at least 3 arguments
  if (numargs < 3)
    return INVALID_ROUTINE;

  // Get name of Rexx variable into which pointer will be put
  if (args[0].strlength > 0)
    {
    memset(&shvb,0x00,sizeof(shvb));
    shvb.shvcode = RXSHV_SET;
    strupr(args[0].strptr);
    shvb.shvname.strptr = args[0].strptr;
    shvb.shvname.strlength = args[0].strlength;
    shvb.shvvalue.strptr = (PVOID) &memptr;
    shvb.shvvalue.strlength = sizeof(memptr);
    }
  else
    return INVALID_ROUTINE;

  // How much memory to allocate
  if (! string2long(args[1].strlength,args[1].strptr, &memsize) )
    return INVALID_ROUTINE;

  // Get allocation flags
  if (args[2].strlength > 0)
    {
    cptr = args[2].strptr;
    for (cctr=1;cctr <= args[2].strlength;cctr++)
      {
      switch (*cptr)
        {
        case 'c':
        case 'C':
          ulFlags |= PAG_COMMIT;
          break;
        case 't':
        case 'T':
          ulFlags |= OBJ_TILE;
          break;
        case 'i':
        case 'I':
          ulFlags |= OBJ_GIVEABLE;
          break;
        case 'e':
        case 'E':
          ulFlags |= OBJ_GETTABLE;
          break;
        case 'x':
        case 'X':
          ulFlags |= PAG_EXECUTE;
          break;
        case 'r':
        case 'R':
          ulFlags |= PAG_READ;
          break;
        case 'w':
        case 'W':
          ulFlags |= PAG_WRITE;
          break;
        case 'g':
        case 'G':
          ulFlags |= PAG_GUARD;
          break;
        default:
          return INVALID_ROUTINE;
          break;
        } // end-switch
      cptr++;
      } // end-for
    } // end-if

  // Named shared memory, or not?
  if (numargs >= 4)
    {
    if (args[3].strlength > 0)
      pmemname = args[3].strptr;
    else
      pmemname = NULL;
    }
  else
    pmemname = NULL;

  rc = DosAllocSharedMem((PPVOID) &memptr,
                         (PSZ)    pmemname,
                         (ULONG)  memsize,
                         (ULONG)  ulFlags);

  RexxVariablePool(&shvb);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;

}

// RxGetSharedMem external function
EXPORTSPEC ULONG APIENTRY  RxGetSharedMem(PUCHAR name, ULONG numargs, RXSTRING args[],
                      PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  PCH         memptr, cptr;
  ULONG       ulFlags = 0, cctr;

  if (numargs < 1)
    return INVALID_ROUTINE;

  if ( args[0].strlength != sizeof(memptr))
    return INVALID_ROUTINE;
  memcpy(&memptr,args[0].strptr,args[0].strlength);

  // Determine flags
  if (numargs >= 2)
    {
    if (args[1].strlength > 0)
      {
      cptr = args[1].strptr;
      for (cctr=1;cctr <= args[1].strlength;cctr++)
        {
        switch (*cptr)
          {
          case 'x':
          case 'X':
            ulFlags |= PAG_EXECUTE;
            break;
          case 'r':
          case 'R':
            ulFlags |= PAG_READ;
            break;
          case 'w':
          case 'W':
            ulFlags |= PAG_WRITE;
            break;
          case 'g':
          case 'G':
            ulFlags |= PAG_GUARD;
            break;
          default:
            return INVALID_ROUTINE;
            break;
          } // end-switch
        cptr++;
        } // end-for
      } // end-if
    } // end-if

  rc = DosGetSharedMem((PVOID) memptr, ulFlags);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;

}

// RxGiveSharedMem external function
EXPORTSPEC ULONG APIENTRY  RxGiveSharedMem(PUCHAR name, ULONG numargs, RXSTRING args[],
                       PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  PCH         memptr, cptr;
  PID         pid;
  ULONG       ulFlags = 0, cctr;

  if (numargs < 2)
    return INVALID_ROUTINE;

  if ( args[0].strlength != sizeof(memptr))
    return INVALID_ROUTINE;
  memcpy(&memptr,args[0].strptr,args[0].strlength);

  if ( args[1].strlength == 0)
    return INVALID_ROUTINE;
  if ( !string2long(args[1].strlength,args[1].strptr,(PLONG) &pid) )
    return INVALID_ROUTINE;

  // Determine flags
  if (numargs >= 3)
    {
    if (args[2].strlength > 0)
      {
      cptr = args[2].strptr;
      for (cctr=1;cctr <= args[2].strlength;cctr++)
        {
        switch (*cptr)
          {
          case 'x':
          case 'X':
            ulFlags |= PAG_EXECUTE;
            break;
          case 'r':
          case 'R':
            ulFlags |= PAG_READ;
            break;
          case 'w':
          case 'W':
            ulFlags |= PAG_WRITE;
            break;
          case 'g':
          case 'G':
            ulFlags |= PAG_GUARD;
            break;
          default:
            return INVALID_ROUTINE;
            break;
          } // end-switch
        cptr++;
        } // end-for
      } // end-if
    } // end-if

  rc = DosGiveSharedMem((PVOID) memptr, pid, ulFlags);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;

}

// RxGetNamedSharedMem external function
EXPORTSPEC ULONG APIENTRY  RxGetNamedSharedMem(PUCHAR name, ULONG numargs, RXSTRING args[],
                           PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  PCH         memptr, cptr;
  ULONG       ulFlags = 0, cctr;
  SHVBLOCK    shvb;

  if (numargs < 3)
    return INVALID_ROUTINE;

  // Get name of Rexx variable into which pointer will be put
  if (args[0].strlength > 0)
    {
    memset(&shvb,0x00,sizeof(shvb));
    shvb.shvcode = RXSHV_SET;
    strupr(args[0].strptr);
    shvb.shvname.strptr = args[0].strptr;
    shvb.shvname.strlength = args[0].strlength;
    shvb.shvvalue.strptr = (PVOID) &memptr;
    shvb.shvvalue.strlength = sizeof(memptr);
    }
  else
    return INVALID_ROUTINE;

  // Get shared-mem name
  if ( args[1].strlength == 0)
    return INVALID_ROUTINE;

  // Determine flags
  if (args[2].strlength > 0)
    {
    cptr = args[2].strptr;
    for (cctr=1;cctr <= args[2].strlength;cctr++)
      {
      switch (*cptr)
        {
        case 'x':
        case 'X':
          ulFlags |= PAG_EXECUTE;
          break;
        case 'r':
        case 'R':
          ulFlags |= PAG_READ;
          break;
        case 'w':
        case 'W':
          ulFlags |= PAG_WRITE;
          break;
        case 'g':
        case 'G':
          ulFlags |= PAG_GUARD;
          break;
        default:
          return INVALID_ROUTINE;
          break;
        } // end-switch
      cptr++;
      } // end-for
    } // end-if

  rc = DosGetNamedSharedMem((PPVOID) &memptr, args[1].strptr,
                            ulFlags);

  RexxVariablePool(&shvb);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;

}

// RxFreeMem external function
EXPORTSPEC ULONG APIENTRY  RxFreeMem(PUCHAR name, ULONG numargs, RXSTRING args[],
                 PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  PCH         memptr;

  if (numargs != 1)
    return INVALID_ROUTINE;

  if ( args[0].strlength != sizeof(memptr))
    return INVALID_ROUTINE;
  memcpy(&memptr,args[0].strptr,args[0].strlength);

  rc = DosFreeMem(memptr);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;

}

// RxSetMem external function
EXPORTSPEC ULONG APIENTRY  RxSetMem(PUCHAR name, ULONG numargs, RXSTRING args[],
                PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  LONG        memsize = 0, cctr;
  ULONG       ulFlags = 0;
  PVOID       memptr;
  PCH         cptr;

  if (numargs < 1)
    return INVALID_ROUTINE;

  if (args[0].strlength != sizeof(PVOID))
    return INVALID_ROUTINE;
  memcpy(&memptr,args[0].strptr,args[0].strlength);

  if (numargs >= 2)
    {
    if (args[1].strlength > 0)
      {
      if (! string2long(args[1].strlength,args[1].strptr, &memsize) )
        return INVALID_ROUTINE;
      }
    }

  // Determine flags
  if (numargs >= 3)
    {
    if (args[2].strlength > 0)
      {
      cptr = args[2].strptr;
      for (cctr=1;cctr <= args[2].strlength;cctr++)
        {
        switch (*cptr)
          {
          case 'c':
          case 'C':
            ulFlags |= PAG_COMMIT;
            break;
          case 'd':
          case 'D':
            ulFlags |= PAG_DECOMMIT;
            break;
          case 'f':
          case 'F':
            ulFlags |= PAG_DEFAULT;
            break;
          case 'x':
          case 'X':
            ulFlags |= PAG_EXECUTE;
            break;
          case 'r':
          case 'R':
            ulFlags |= PAG_READ;
            break;
          case 'w':
          case 'W':
            ulFlags |= PAG_WRITE;
            break;
          case 'g':
          case 'G':
            ulFlags |= PAG_GUARD;
            break;
          default:
            return INVALID_ROUTINE;
            break;
          } // end-switch
        cptr++;
        } // end-for
      } // end-if
    } // end-if

  rc = DosSetMem((PVOID) memptr,
                 (ULONG) memsize,
                 (ULONG) ulFlags);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;

}

// RxQueryMem external function
EXPORTSPEC ULONG APIENTRY  RxQueryMem(PUCHAR name, ULONG numargs, RXSTRING args[],
                  PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  LONG        memsize = 0;
  ULONG       ulFlags = 0;
  PVOID       memptr;

  if (numargs < 1)
    return INVALID_ROUTINE;

  if (args[0].strlength != sizeof(PVOID))
    return INVALID_ROUTINE;
  memcpy(&memptr,args[0].strptr,args[0].strlength);

  if (numargs >= 2)
    {
    if (args[1].strlength > 0)
      {
      if (! string2long(args[1].strlength,args[1].strptr, &memsize) )
        return INVALID_ROUTINE;
      }
    }

  rc = DosQueryMem((PVOID)  memptr,
                   (PULONG) &memsize,
                   (PULONG) &ulFlags);

  if (rc == 0)
    {
    sprintf(retstr->strptr,"%ld %ld ",rc,memsize);
    if (ulFlags & PAG_COMMIT)
      strcat(retstr->strptr,"C");
    if (ulFlags & PAG_FREE)
      strcat(retstr->strptr,"F");
    if (ulFlags & PAG_SHARED)
      strcat(retstr->strptr,"S");
    if (ulFlags & PAG_BASE)
      strcat(retstr->strptr,"B");
    if (ulFlags & PAG_EXECUTE)
      strcat(retstr->strptr,"X");
    if (ulFlags & PAG_READ)
      strcat(retstr->strptr,"R");
    if (ulFlags & PAG_WRITE)
      strcat(retstr->strptr,"W");
    if (ulFlags & PAG_GUARD)
      strcat(retstr->strptr,"G");
    }
  else
    sprintf(retstr->strptr,"%ld",rc);

  retstr->strlength = strlen(retstr->strptr);

  return VALID_ROUTINE;

}

// RxSubSetMem external function
EXPORTSPEC ULONG APIENTRY  RxSubSetMem(PUCHAR name, ULONG numargs, RXSTRING args[],
                   PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  LONG        memsize = 0, cctr;
  ULONG       ulFlags = 0;
  PVOID       memptr;
  PCH         cptr;

  if (numargs < 1)
    return INVALID_ROUTINE;

  if (args[0].strlength != sizeof(PVOID))
    return INVALID_ROUTINE;
  memcpy(&memptr,args[0].strptr,args[0].strlength);

  if (numargs >= 2)
    {
    if (args[1].strlength > 0)
      {
      if (! string2long(args[1].strlength,args[1].strptr, &memsize) )
        return INVALID_ROUTINE;
      }
    }

  // Determine flags
  if (numargs >= 3)
    {
    if (args[2].strlength > 0)
      {
      cptr = args[2].strptr;
      for (cctr=1;cctr <= args[2].strlength;cctr++)
        {
        switch (*cptr)
          {
          case 'i':
          case 'I':
            ulFlags |= DOSSUB_INIT;
            break;
          case 'g':
          case 'G':
            ulFlags |= DOSSUB_GROW;
            break;
          case 's':
          case 'S':
            ulFlags |= DOSSUB_SPARSE_OBJ;
            break;
          case 'r':
          case 'R':
            ulFlags |= DOSSUB_SERIALIZE;
            break;
          default:
            return INVALID_ROUTINE;
            break;
          } // end-switch
        cptr++;
        } // end-for
      } // end-if
    } // end-if

  rc = DosSubSetMem((PVOID) memptr,
                    (ULONG) ulFlags,
                    (ULONG) memsize);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;

}

// RxSubUnsetMem external function
EXPORTSPEC ULONG APIENTRY  RxSubUnsetMem(PUCHAR name, ULONG numargs, RXSTRING args[],
                     PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  PVOID       memptr;

  if (numargs < 1)
    return INVALID_ROUTINE;

  if (args[0].strlength != sizeof(PVOID))
    return INVALID_ROUTINE;
  memcpy(&memptr,args[0].strptr,args[0].strlength);

  rc = DosSubUnsetMem((PVOID) memptr);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;

}

// RxSubAllocMem external function
EXPORTSPEC ULONG APIENTRY  RxSubAllocMem(PUCHAR name, ULONG numargs, RXSTRING args[],
                     PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  LONG        memsize = 0;
  PVOID       memptr, pBlockOffset;
  SHVBLOCK    shvb;

  if (numargs < 3)
    return INVALID_ROUTINE;

  // Get name of Rexx variable into which pointer will be put
  if (args[0].strlength > 0)
    {
    memset(&shvb,0x00,sizeof(shvb));
    shvb.shvcode = RXSHV_SET;
    strupr(args[0].strptr);
    shvb.shvname.strptr = args[0].strptr;
    shvb.shvname.strlength = args[0].strlength;
    shvb.shvvalue.strptr = (PVOID) &pBlockOffset;
    shvb.shvvalue.strlength = sizeof(pBlockOffset);
    }
  else
    return INVALID_ROUTINE;

  if (args[1].strlength != sizeof(PVOID))
    return INVALID_ROUTINE;
  memcpy(&memptr,args[1].strptr,args[1].strlength);

  if (args[2].strlength > 0)
    {
    if (! string2long(args[2].strlength,args[2].strptr, &memsize) )
      return INVALID_ROUTINE;
    }

  rc = DosSubAllocMem((PVOID)  memptr,
                      (PPVOID) &pBlockOffset,
                      (ULONG)  memsize);

  RexxVariablePool(&shvb);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;

}

// RxSubFreeMem external function
EXPORTSPEC ULONG APIENTRY  RxSubFreeMem(PUCHAR name, ULONG numargs, RXSTRING args[],
                    PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  LONG        memsize = 0;
  PVOID       memptr, pBlockOffset;

  if (numargs < 2)
    return INVALID_ROUTINE;

  if (args[0].strlength != sizeof(PVOID))
    return INVALID_ROUTINE;
  memcpy(&memptr,args[0].strptr,args[0].strlength);

  if (args[2].strlength != sizeof(PVOID))
    return INVALID_ROUTINE;
  memcpy(&pBlockOffset,args[2].strptr,args[2].strlength);

  if (numargs >= 3)
    {
    if (args[1].strlength > 0)
      {
      if (! string2long(args[1].strlength,args[1].strptr, &memsize) )
        return INVALID_ROUTINE;
      }
    }

  rc = DosSubFreeMem((PVOID) memptr,
                     (PVOID) pBlockOffset,
                     (ULONG) memsize);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;

}

// RxCreateEventSem external function
EXPORTSPEC ULONG APIENTRY  RxCreateEventSem(PUCHAR name, ULONG numargs, RXSTRING args[],
                        PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  HEV         hevsem;
  ULONG       ulFlattr = 0;       // "Private", by default
  BOOL32      f32fState = FALSE;  // "Reset", by default
  PCH         psemname = NULL;    // Unnamed, by default
  SHVBLOCK    shvb;

  if (numargs < 1)
    return INVALID_ROUTINE;

  // Get name of Rexx variable into which sem handle will be put
  if (args[0].strlength > 0)
    {
    memset(&shvb,0x00,sizeof(shvb));
    shvb.shvcode = RXSHV_SET;
    strupr(args[0].strptr);
    shvb.shvname.strptr = args[0].strptr;
    shvb.shvname.strlength = args[0].strlength;
    shvb.shvvalue.strptr = (PVOID) &hevsem;
    shvb.shvvalue.strlength = sizeof(hevsem);
    }
  else
    return INVALID_ROUTINE;

  if (numargs >= 2)
    {
    if (args[1].strlength > 0)
      {
      switch (*(args[1].strptr))
        {
        case 's':    // Shared sem
        case 'S':
          ulFlattr = DC_SEM_SHARED;
          break;
        case 'p':    // Private Sem
        case 'P':
          ulFlattr = 0;
          break;
        default:
          return INVALID_ROUTINE;
          break;
        }
      }
    }

  if (numargs >= 3)
    {
    if (args[2].strlength > 0)
      psemname = args[2].strptr;
    }

  if (numargs >= 4)
    {
    if (args[3].strlength > 0)
      {
      switch (*(args[3].strptr))
        {
        case 'r':       // Reset State
        case 'R':
          f32fState = FALSE;
          break;
        case 'p':       // Posted State
        case 'P':
          f32fState = TRUE;
          break;
        default:
          return INVALID_ROUTINE;
          break;
        }
      }
    }

  rc = DosCreateEventSem((PSZ)  psemname,
                         (PHEV) &hevsem,
                         (ULONG) ulFlattr,
                         (ULONG) f32fState);

  RexxVariablePool(&shvb);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;

}

// RxOpenEventSem external function
EXPORTSPEC ULONG APIENTRY  RxOpenEventSem(PUCHAR name, ULONG numargs, RXSTRING args[],
                      PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  HEV         hevsem = 0;
  PCH         psemname = NULL;
  SHVBLOCK    shvb;

  if (numargs < 2)
    return INVALID_ROUTINE;

  // Get name of Rexx variable into which sem handle will be put
  if (args[0].strlength > 0)
    {
    memset(&shvb,0x00,sizeof(shvb));
    shvb.shvcode = RXSHV_SET;
    strupr(args[0].strptr);
    shvb.shvname.strptr = args[0].strptr;
    shvb.shvname.strlength = args[0].strlength;
    shvb.shvvalue.strptr = (PVOID) &hevsem;
    shvb.shvvalue.strlength = sizeof(hevsem);
    }
  else
    return INVALID_ROUTINE;

  // Get either Event-Sem handle or Event-Sem name
  if (args[1].strlength == sizeof(HEV))
    memcpy(&hevsem,args[1].strptr,args[1].strlength);
  else
    psemname = args[1].strptr;

  rc = DosOpenEventSem((PSZ)  psemname,
                       (PHEV) &hevsem);

  RexxVariablePool(&shvb);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;

}

// RxCloseEventSem external function
EXPORTSPEC ULONG APIENTRY  RxCloseEventSem(PUCHAR name, ULONG numargs, RXSTRING args[],
                       PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  HEV         hevsem = 0;

  if (numargs < 1)
    return INVALID_ROUTINE;

  if (args[0].strlength != sizeof(HEV))
    return INVALID_ROUTINE;
  memcpy(&hevsem,args[0].strptr,args[0].strlength);

  rc = DosCloseEventSem((HEV) hevsem);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;

}

// RxPostEventSem external function
EXPORTSPEC ULONG APIENTRY  RxPostEventSem(PUCHAR name, ULONG numargs, RXSTRING args[],
                      PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  HEV         hevsem = 0;

  if (numargs < 1)
    return INVALID_ROUTINE;

  if (args[0].strlength != sizeof(HEV))
    return INVALID_ROUTINE;
  memcpy(&hevsem,args[0].strptr,args[0].strlength);

  rc = DosPostEventSem((HEV) hevsem);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;

}

// RxQueryEventSem external function
EXPORTSPEC ULONG APIENTRY  RxQueryEventSem(PUCHAR name, ULONG numargs, RXSTRING args[],
                       PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  HEV         hevsem = 0;
  ULONG       ulPostCount = 0;

  if (numargs < 1)
    return INVALID_ROUTINE;

  if (args[0].strlength != sizeof(HEV))
    return INVALID_ROUTINE;
  memcpy(&hevsem,args[0].strptr,args[0].strlength);

  rc = DosQueryEventSem((HEV) hevsem, (PULONG) &ulPostCount);

  retstr->strlength = sprintf(retstr->strptr,"%ld %ld",rc,ulPostCount);

  return VALID_ROUTINE;

}

// RxResetEventSem external function
EXPORTSPEC ULONG APIENTRY  RxResetEventSem(PUCHAR name, ULONG numargs, RXSTRING args[],
                       PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  HEV         hevsem = 0;
  ULONG       ulPostCount = 0;

  if (numargs < 1)
    return INVALID_ROUTINE;

  if (args[0].strlength != sizeof(HEV))
    return INVALID_ROUTINE;
  memcpy(&hevsem,args[0].strptr,args[0].strlength);

  rc = DosResetEventSem((HEV) hevsem, (PULONG) &ulPostCount);

  retstr->strlength = sprintf(retstr->strptr,"%ld %ld",rc,ulPostCount);

  return VALID_ROUTINE;

}

// RxWaitEventSem external function
EXPORTSPEC ULONG APIENTRY  RxWaitEventSem(PUCHAR name, ULONG numargs, RXSTRING args[],
                      PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  HEV         hevsem = 0;
  ULONG       ulTimeOut = SEM_INDEFINITE_WAIT;  // Wait forever

  if (numargs < 1)
    return INVALID_ROUTINE;

  if (args[0].strlength != sizeof(HEV))
    return INVALID_ROUTINE;
  memcpy(&hevsem,args[0].strptr,args[0].strlength);

  if (numargs >= 2)
    {
    if (args[1].strlength > 0)
      {
      if (! string2long(args[1].strlength,args[1].strptr, (PLONG) &ulTimeOut) )
        {
        switch (*(args[1].strptr))
          {
          case 'i':
          case 'I':
            ulTimeOut = SEM_INDEFINITE_WAIT;
            break;
          default:
            printf("Arg 2, if supplied, must be a decimal integer (number of");
            printf(" miliseconds).\nOr the word 'Indefinite'\n");
            return INVALID_ROUTINE;
            break;
          }
        }
      }
    }

  rc = DosWaitEventSem((HEV) hevsem, (ULONG) ulTimeOut);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;

}

// RxCreateMutexSem external function
EXPORTSPEC ULONG APIENTRY  RxCreateMutexSem(PUCHAR name, ULONG numargs, RXSTRING args[],
                        PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  HMTX        hmtx;
  ULONG       ulFlattr = 0;       // "Private", by default
  BOOL32      f32fState = FALSE;  // "Unowned", by default
  PCH         psemname = NULL;    // Unnamed, by default
  SHVBLOCK    shvb;

  if (numargs < 1)
    return INVALID_ROUTINE;

  // Get name of Rexx variable into which sem handle will be put
  if (args[0].strlength > 0)
    {
    memset(&shvb,0x00,sizeof(shvb));
    shvb.shvcode = RXSHV_SET;
    strupr(args[0].strptr);
    shvb.shvname.strptr = args[0].strptr;
    shvb.shvname.strlength = args[0].strlength;
    shvb.shvvalue.strptr = (PVOID) &hmtx;
    shvb.shvvalue.strlength = sizeof(hmtx);
    }
  else
    return INVALID_ROUTINE;

  if (numargs >= 2)
    {
    if (args[1].strlength > 0)
      {
      switch (*(args[1].strptr))
        {
        case 's':    // Shared Sem
        case 'S':
          ulFlattr = DC_SEM_SHARED;
          break;
        case 'p':    // Private Sem
        case 'P':
          ulFlattr = 0;
          break;
        default:
          return INVALID_ROUTINE;
          break;
        }
      }
    }

  if (numargs >= 3)
    {
    if (args[2].strlength > 0)
      psemname = args[2].strptr;
    }

  if (numargs >= 4)
    {
    if (args[3].strlength > 0)
      {
      switch (*(args[3].strptr))
        {
        case 'u':     // Unowned
        case 'U':
          f32fState = FALSE;
          break;
        case 'o':     // Owned
        case 'O':
          f32fState = TRUE;
          break;
        default:
          return INVALID_ROUTINE;
          break;
        }
      }
    }

  rc = DosCreateMutexSem((PSZ)   psemname,
                         (PHMTX) &hmtx,
                         (ULONG) ulFlattr,
                         (ULONG) f32fState);

  RexxVariablePool(&shvb);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;

}

// RxOpenMutexSem external function
EXPORTSPEC ULONG APIENTRY  RxOpenMutexSem(PUCHAR name, ULONG numargs, RXSTRING args[],
                      PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  HMTX        hmtx = 0;
  PCH         psemname = NULL;
  SHVBLOCK    shvb;

  if (numargs < 2)
    return INVALID_ROUTINE;

  // Get name of Rexx variable into which sem handle will be put
  if (args[0].strlength > 0)
    {
    memset(&shvb,0x00,sizeof(shvb));
    shvb.shvcode = RXSHV_SET;
    strupr(args[0].strptr);
    shvb.shvname.strptr = args[0].strptr;
    shvb.shvname.strlength = args[0].strlength;
    shvb.shvvalue.strptr = (PVOID) &hmtx;
    shvb.shvvalue.strlength = sizeof(hmtx);
    }
  else
    return INVALID_ROUTINE;

  // Get Mutex Sem handle or Mutex Sem name
  if (args[1].strlength == sizeof(HMTX))
    memcpy(&hmtx,args[1].strptr,args[1].strlength);
  else
    psemname = args[1].strptr;

  rc = DosOpenMutexSem((PSZ)   psemname,
                       (PHMTX) &hmtx);

  RexxVariablePool(&shvb);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);
  return VALID_ROUTINE;

}

// RxCloseMutexSem external function
EXPORTSPEC ULONG APIENTRY  RxCloseMutexSem(PUCHAR name, ULONG numargs, RXSTRING args[],
                       PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  HMTX        hmtx = 0;

  if (numargs < 1)
    return INVALID_ROUTINE;

  if (args[0].strlength != sizeof(HMTX))
    return INVALID_ROUTINE;
  memcpy(&hmtx,args[0].strptr,args[0].strlength);

  rc = DosCloseMutexSem((HMTX) hmtx);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;

}

// RxQueryMutexSem external function
EXPORTSPEC ULONG APIENTRY  RxQueryMutexSem(PUCHAR name, ULONG numargs, RXSTRING args[],
                       PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  HMTX        hmtx = 0;
  ULONG       ulCount = 0;
  PID         pidOwner = 0;
  TID         tidOwner = 0;

  if (numargs < 1)
    return INVALID_ROUTINE;

  if (args[0].strlength != sizeof(HMTX))
    return INVALID_ROUTINE;
  memcpy(&hmtx,args[0].strptr,args[0].strlength);

  rc = DosQueryMutexSem((HMTX) hmtx, (PPID) &pidOwner,
                        (PTID) &tidOwner, (PULONG) &ulCount);

  retstr->strlength = sprintf(retstr->strptr,"%ld %ld %ld %ld",rc,pidOwner,tidOwner,ulCount);

  return VALID_ROUTINE;

}

// RxReleaseMutexSem external function
EXPORTSPEC ULONG APIENTRY  RxReleaseMutexSem(PUCHAR name, ULONG numargs, RXSTRING args[],
                         PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  HMTX        hmtx = 0;

  if (numargs < 1)
    return INVALID_ROUTINE;

  if (args[0].strlength != sizeof(HMTX))
    return INVALID_ROUTINE;
  memcpy(&hmtx,args[0].strptr,args[0].strlength);

  rc = DosReleaseMutexSem((HMTX) hmtx);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;

}

// RxRequestMutexSem external function
EXPORTSPEC ULONG APIENTRY  RxRequestMutexSem(PUCHAR name, ULONG numargs, RXSTRING args[],
                         PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  HMTX        hmtx = 0;
  ULONG       ulTimeOut = SEM_INDEFINITE_WAIT;  // Wait forever, by default

  if (numargs < 1)
    return INVALID_ROUTINE;

  if (args[0].strlength != sizeof(HMTX))
    return INVALID_ROUTINE;
  memcpy(&hmtx,args[0].strptr,args[0].strlength);

  if (numargs >= 2)
    {
    if (args[1].strlength > 0)
      {
      if (! string2long(args[1].strlength,args[1].strptr, (PLONG) &ulTimeOut) )
        {
        switch (*(args[1].strptr))
          {
          case 'i':
          case 'I':
            ulTimeOut = SEM_INDEFINITE_WAIT;
            break;
          default:
            printf("Arg 2, if supplied, must be a decimal integer (number of");
            printf(" miliseconds).\nOr the word 'Indefinite'\n");
            return INVALID_ROUTINE;
            break;
          }
        }
      }
    }

  rc = DosRequestMutexSem((HMTX) hmtx, (ULONG) ulTimeOut);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;

}

// RxCreateMuxWaitSem external function
EXPORTSPEC ULONG APIENTRY  RxCreateMuxWaitSem(PUCHAR name, ULONG numargs, RXSTRING args[],
                          PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  HMUX        hmux;
  ULONG       ulFlattr = 0;         // Default is private
  ULONG       ulcSemRec = 0;        // Number of semrecs
  PSEMRECORD  pSemRec, pholdSemRec; // Pointers to SEMRECORD array
  PCH         psemname = NULL;      // Unnamed, by default
  SHVBLOCK    shvb;
  ULONG       ctr;
  UCHAR       varname[250], varval[250];

  if (numargs < 2)
    return INVALID_ROUTINE;

  // Get name of Rexx variable into which sem handle will be put
  if (args[0].strlength == 0)
    return INVALID_ROUTINE;
  strupr(args[0].strptr);

  // Get value of "Stem.0" to know how many semaphores are supplied
  if (args[1].strlength == 0)
    return INVALID_ROUTINE;
  strupr(args[1].strptr);

  shvb.shvnext = (PSHVBLOCK)NULL;
  shvb.shvcode = RXSHV_FETCH;
  shvb.shvname.strptr = varname;
  sprintf(varname,"%s0",args[1].strptr);
  shvb.shvname.strlength = strlen(varname);
  shvb.shvvalue.strptr = varval;
  shvb.shvvalue.strlength = sizeof(varval);
  rc = RexxVariablePool(&shvb);
  if (rc != 0 || shvb.shvret != 0x00)
    {
    printf("RXSHV_FETCH failed for Rexx variable '%s' with rc = %ld",
           varname,rc);
    printf(" (shvb.shvret = %x)\n",shvb.shvret);
    return INVALID_ROUTINE;
    }

  if ( !string2long(shvb.shvvaluelen,shvb.shvvalue.strptr,(PLONG) &ulcSemRec) )
    {
    printf("The value of the Rexx variable '%s' contains invalid characters\n",
           varname);
    printf("The variable's value is '%.*s'\n",shvb.shvvaluelen,
           shvb.shvvalue.strptr);
    printf("The variable's value's length is %ld bytes\n",shvb.shvvaluelen);
    printf("This value must be a positive decimal integer\n");
    return INVALID_ROUTINE;
    }
  if (ulcSemRec == 0)
    {
    printf("The value of the Rexx variable '%s' is a negative integer\n",
           varname);
    printf("The variable's value is '%.*s'\n",shvb.shvvaluelen,
           shvb.shvvalue.strptr);
    printf("This value must be a positive decimal integer\n");
    return INVALID_ROUTINE;
    }

  pSemRec = (PSEMRECORD) malloc(sizeof(SEMRECORD) * ulcSemRec);
  pholdSemRec = pSemRec;

  // Spin through stem values, getting semaphore handles and sem-ids
  for (ctr=1;ctr <= ulcSemRec;ctr++)
    {
    // Get semaphore handle from stem.n.1
    sprintf(varname,"%s%ld.1",args[1].strptr,ctr);
    shvb.shvname.strlength = strlen(varname);
    shvb.shvvalue.strlength = sizeof(varval);
    rc = RexxVariablePool(&shvb);
    if (shvb.shvvaluelen != sizeof(pSemRec->hsemCur))
      {
      printf("The value of the Rexx variable '%s' must be a valid semaphore handle\n",
             varname);
      free(pholdSemRec);
      return INVALID_ROUTINE;
      }
    memcpy(&(pSemRec->hsemCur),shvb.shvvalue.strptr,shvb.shvvaluelen);

    // Get semaphore-id from stem.n.2
    sprintf(varname,"%s%ld.2",args[1].strptr,ctr);
    shvb.shvname.strlength = strlen(varname);
    shvb.shvvalue.strlength = sizeof(varval);
    rc = RexxVariablePool(&shvb);
    if (string2long(shvb.shvvaluelen,shvb.shvvalue.strptr,
        (PLONG) &(pSemRec->ulUser)) != TRUE)
      {
      printf("The value of the Rexx variable '%s' contains invalid characters\n",
             varname);
      printf("The variable's value is '%.*s'\n",shvb.shvvaluelen,
             shvb.shvvalue.strptr);
      printf("This value must be a decimal integer\n");
      free(pholdSemRec);
      return INVALID_ROUTINE;
      }
    pSemRec++;
    }

  pSemRec = pholdSemRec;

  // Get private-shared switch, if supplied
  if (numargs >= 3)
    {
    if (args[2].strlength > 0)
      {
      switch (*(args[2].strptr))
        {
        case 's':    // SharedSem
        case 'S':
          ulFlattr |= DC_SEM_SHARED;
          break;
        case 'p':    // PrivateSem
        case 'P':
          break;
        default:
          return INVALID_ROUTINE;
          break;
        }
      }
    }

  // Get any/all switch, if supplied
  if (numargs >= 4)
    {
    if (args[3].strlength > 0)
      {
      switch (*(args[3].strptr))
        {
        case 'y':
        case 'Y':
          ulFlattr |= DCMW_WAIT_ANY;
          break;
        case 'l':
        case 'L':
          ulFlattr |= DCMW_WAIT_ALL;
          break;
        default:
          return INVALID_ROUTINE;
          break;
        }
      }
    else
      ulFlattr |= DCMW_WAIT_ALL;
    }
  else
    ulFlattr |= DCMW_WAIT_ALL;

  // Get sem name, if supplied
  if (numargs >= 5)
    {
    if (args[4].strlength > 0)
      psemname = args[4].strptr;
    }

  rc = DosCreateMuxWaitSem((PSZ)        psemname,
                           (PHMUX)      &hmux,
                           (ULONG)      ulcSemRec,
                           (PSEMRECORD) pSemRec,
                           (ULONG)      ulFlattr);

  free(pSemRec);

  // Put sem handle into Rexx Variable
  memset(&shvb,0x00,sizeof(shvb));
  shvb.shvcode = RXSHV_SET;
  shvb.shvname.strptr = args[0].strptr;
  shvb.shvname.strlength = args[0].strlength;
  shvb.shvvalue.strptr = (PVOID) &hmux;
  shvb.shvvalue.strlength = sizeof(hmux);
  RexxVariablePool(&shvb);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;

}

// RxOpenMuxWaitSem external function
EXPORTSPEC ULONG APIENTRY  RxOpenMuxWaitSem(PUCHAR name, ULONG numargs, RXSTRING args[],
                        PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  HMUX        hmux = 0;
  PCH         psemname = NULL;
  SHVBLOCK    shvb;

  if (numargs < 2)
    return INVALID_ROUTINE;

  // Get name of Rexx variable into which sem handle will be put
  if (args[0].strlength > 0)
    {
    memset(&shvb,0x00,sizeof(shvb));
    shvb.shvcode = RXSHV_SET;
    strupr(args[0].strptr);
    shvb.shvname.strptr = args[0].strptr;
    shvb.shvname.strlength = args[0].strlength;
    shvb.shvvalue.strptr = (PVOID) &hmux;
    shvb.shvvalue.strlength = sizeof(hmux);
    }
  else
    return INVALID_ROUTINE;

  // Get sem name or sem handle
  if (args[1].strlength == sizeof(HMUX))
    memcpy(&hmux,args[1].strptr,args[1].strlength);
  else
    psemname = args[1].strptr;

  rc = DosOpenMuxWaitSem((PSZ)   psemname,
                         (PHMUX) &hmux);

  RexxVariablePool(&shvb);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);
  return VALID_ROUTINE;

}

// RxCloseMuxWaitSem external function
EXPORTSPEC ULONG APIENTRY  RxCloseMuxWaitSem(PUCHAR name, ULONG numargs, RXSTRING args[],
                         PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  HMUX        hmux = 0;

  if (numargs < 1)
    return INVALID_ROUTINE;

  if (args[0].strlength != sizeof(HMUX))
    return INVALID_ROUTINE;
  memcpy(&hmux,args[0].strptr,args[0].strlength);

  rc = DosCloseMuxWaitSem((HMUX) hmux);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;

}

// RxDeleteMuxWaitSem external function
EXPORTSPEC ULONG APIENTRY  RxDeleteMuxWaitSem(PUCHAR name, ULONG numargs, RXSTRING args[],
                          PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  HMUX        hmux = 0;
  HSEM        hsem;

  if (numargs < 2)
    return INVALID_ROUTINE;

  if (args[0].strlength != sizeof(HMUX))
    return INVALID_ROUTINE;
  memcpy(&hmux,args[0].strptr,args[0].strlength);

  if (args[1].strlength != sizeof(HSEM))
    return INVALID_ROUTINE;
  memcpy(&hsem,args[1].strptr,args[1].strlength);

  rc = DosDeleteMuxWaitSem((HMUX) hmux, (HSEM) hsem);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;

}

// RxAddMuxWaitSem external function
EXPORTSPEC ULONG APIENTRY  RxAddMuxWaitSem(PUCHAR name, ULONG numargs, RXSTRING args[],
                       PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  HMUX        hmux = 0;
  SEMRECORD   SemRec;

  if (numargs < 3)
    return INVALID_ROUTINE;

  if (args[0].strlength != sizeof(HMUX))
    return INVALID_ROUTINE;
  memcpy(&hmux,args[0].strptr,args[0].strlength);

  if (args[1].strlength != sizeof(HSEM))
    return INVALID_ROUTINE;
  memcpy(&(SemRec.hsemCur),args[1].strptr,args[1].strlength);

  if (string2long(args[2].strlength,args[2].strptr,(PLONG) &(SemRec.ulUser))
      != TRUE)
    return INVALID_ROUTINE;

  rc = DosAddMuxWaitSem((HMUX) hmux, (PSEMRECORD) &SemRec);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;

}

// RxQueryMuxWaitSem external function
EXPORTSPEC ULONG APIENTRY  RxQueryMuxWaitSem(PUCHAR name, ULONG numargs, RXSTRING args[],
                         PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  HMUX        hmux;
  ULONG       ulFlattr = 0;         // "Private", by default
  ULONG       ulcSemRec = 0;        // Number of semrecs
  PSEMRECORD  pSemRec, pholdSemRec; // Pointers to SEMRECORD array
  SHVBLOCK    shvb;
  ULONG       ctr;
  UCHAR       varname[250], varval[250];

  if (numargs < 2)
    return INVALID_ROUTINE;

  if (args[0].strlength != sizeof(HMUX))
    return INVALID_ROUTINE;
  memcpy(&hmux,args[0].strptr,args[0].strlength);

  if (args[1].strlength == 0)
    return INVALID_ROUTINE;

  strupr(args[1].strptr);

  rc = DosQueryMuxWaitSem((HMUX) hmux, (PULONG) &ulcSemRec,
                          (PSEMRECORD) NULL,
                          (PULONG) &ulFlattr);

  switch (rc)
    {
    case NO_ERROR:
    case ERROR_PARAM_TOO_SMALL:
      pSemRec = (PSEMRECORD) malloc(sizeof(SEMRECORD) * ulcSemRec);
      break;
    default:
      retstr->strlength = sprintf(retstr->strptr,"%ld",rc);
      return VALID_ROUTINE;
      break;
    }

  shvb.shvnext = (PSHVBLOCK)NULL;
  shvb.shvcode = RXSHV_SET;
  shvb.shvname.strptr = varname;
  sprintf(varname,"%s0",args[1].strptr);
  shvb.shvname.strlength = strlen(varname);
  shvb.shvvalue.strptr = varval;
  sprintf(varval,"%ld",ulcSemRec);
  shvb.shvvalue.strlength = strlen(varval);
  rc = RexxVariablePool(&shvb);
  if (rc != 0 && shvb.shvret != RXSHV_NEWV)
    {
    free(pSemRec);
    printf("RXSHV_SET failed for Rexx variable '%s' with rc = %ld",
           varname,rc);
    printf(" (shvb.shvret = %x)\n",shvb.shvret);
    return INVALID_ROUTINE;
    }

  rc = DosQueryMuxWaitSem((HMUX) hmux, (PULONG) &ulcSemRec,
                          (PSEMRECORD) pSemRec, (PULONG) &ulFlattr);
  if (rc != NO_ERROR)
    {
    free(pSemRec);
    retstr->strlength = sprintf(retstr->strptr,"%ld",rc);
    return VALID_ROUTINE;
    }

  pholdSemRec = pSemRec;

  for (ctr=1;ctr <= ulcSemRec;ctr++)
    {
    // Put semaphore handle in stem.n.1
    sprintf(varname,"%s%ld.1",args[1].strptr,ctr);
    shvb.shvname.strlength = strlen(varname);
    memcpy(varval,&(pSemRec->hsemCur),sizeof(pSemRec->hsemCur));
    shvb.shvvalue.strlength = sizeof(pSemRec->hsemCur);
    rc = RexxVariablePool(&shvb);

    // Put semaphore-id in stem.n.2
    sprintf(varname,"%s%ld.2",args[1].strptr,ctr);
    shvb.shvname.strlength = strlen(varname);
    sprintf(varval,"%ld",pSemRec->ulUser);
    shvb.shvvalue.strlength = strlen(varval);
    rc = RexxVariablePool(&shvb);

    pSemRec++;
    }

  free(pholdSemRec);

  sprintf(retstr->strptr,"0 ");
  if (ulFlattr & DC_SEM_SHARED)
    strcat(retstr->strptr,"S");
  if (ulFlattr & DCMW_WAIT_ANY)
    strcat(retstr->strptr,"Y");
  if (ulFlattr & DCMW_WAIT_ALL)
    strcat(retstr->strptr,"L");

  retstr->strlength = strlen(retstr->strptr);

  return VALID_ROUTINE;

}

// RxWaitMuxWaitSem external function
EXPORTSPEC ULONG APIENTRY  RxWaitMuxWaitSem(PUCHAR name, ULONG numargs, RXSTRING args[],
                        PSZ queuename, PRXSTRING retstr)
{

  APIRET      rc;
  HMUX        hmux = 0;
  ULONG       ulTimeOut = SEM_INDEFINITE_WAIT;  // Wait forever, by default
  ULONG       ulUser;

  if (numargs < 1)
    return INVALID_ROUTINE;

  if (args[0].strlength != sizeof(HMUX))
    return INVALID_ROUTINE;
  memcpy(&hmux,args[0].strptr,args[0].strlength);

  if (numargs >= 2)
    {
    if (args[1].strlength > 0)
      {
      if (! string2long(args[1].strlength,args[1].strptr, (PLONG) &ulTimeOut) )
        {
        switch (*(args[1].strptr))
          {
          case 'i':
          case 'I':
            ulTimeOut = SEM_INDEFINITE_WAIT;
            break;
          default:
            printf("Arg 2, if supplied, must be a decimal integer (number of");
            printf(" miliseconds).\nOr the word 'Indefinite'\n");
            return INVALID_ROUTINE;
            break;
          }
        }
      }
    }

  rc = DosWaitMuxWaitSem((HMUX) hmux, (ULONG) ulTimeOut, (PULONG) &ulUser);

  if (rc == 0)
    retstr->strlength = sprintf(retstr->strptr,"%ld %ld",rc,ulUser);
  else
    retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;

}

#endif  // BLD4OS2

// RxuInit (register related Rexx external functions)
EXPORTSPEC ULONG APIENTRY  RxuInit(PUCHAR name, ULONG numargs, RXSTRING args[],
               PSZ queuename, PRXSTRING retstr)
{

  INT    entries;                      // Num of entries
  INT    j;                            // Counter
  APIRET rc;
  ULONG  ctr=0;
#if defined(_MSVC_)
  char	 wb[500];
#endif

  entries = sizeof(RxFncTable)/sizeof(PSZ);

  for (j = 0; j < entries; j++)
    {
    rc = RexxQueryFunction(RxFncTable[j]);
    if ( rc == RXFUNC_NOTREG )
      {
#if defined(_MSVC_)
	  sprintf(wb,"_%s@20",RxFncTable[j]);
#endif
      rc = RexxRegisterFunctionDll(RxFncTable[j],_THIS_DLL_NAME_,
#if defined(_MSVC_)
		  wb
#else
		  RxFncTable[j]
#endif
		  );
      if (numargs > 0)
        printf("Function '%s' registered with rc = %ld\n",RxFncTable[j],rc);
      } // end-if
    else
      {
      if (numargs > 0)
        printf("Function '%s' NOT registered because rc = %ld\n",RxFncTable[j],rc);
      ctr++;
      } // end-else
    } // end-for all entries

  retstr->strlength = sprintf(retstr->strptr,"%ld %ld",ctr,entries);
  return VALID_ROUTINE;

}

// RxuQuery (query entry point names of Rexx external functions)
EXPORTSPEC ULONG APIENTRY  RxuQuery(PUCHAR name, ULONG numargs, RXSTRING args[],
                PSZ queuename, PRXSTRING retstr)
{

  INT    entries;                      // Num of entries
  INT    j;                            // Counter
  ULONG  strsize=0;
#if defined(__BLD4OS2__)
  ULONG  rc;
#endif  // BLD4OS2
#if defined(__BLD4NT__)
  HGLOBAL hglb=NULL;
#endif  // BLD4NT

  entries = sizeof(RxFncTable)/sizeof(PSZ);

  for (j = 0; j < entries; j++)
    strsize += strlen(RxFncTable[j]);

  strsize += entries + 50;

#if defined(__BLD4OS2__)
  DosFreeMem(retstr->strptr);
#endif  // BLD4OS2

#if defined(__BLD4OS2__)
  rc = DosAllocMem((PPVOID) &(retstr->strptr),strsize,PAG_COMMIT | PAG_WRITE);
  if (rc)
    return INVALID_ROUTINE;
#endif  // BLD4OS2
#if defined(__BLD4NT__)
  retstr->strptr = (char*) GlobalLock(GlobalAlloc(GHND,strsize));
#endif  // BLD4NT

  strcpy(retstr->strptr,_VERSION_INFO_LIT_);
  for (j=0;j < entries;j++)
    {
    strcat(retstr->strptr," ");
    strcat(retstr->strptr,RxFncTable[j]);
    }

  retstr->strlength = strlen(retstr->strptr);
  return VALID_ROUTINE;

}

// RxuTerm (de-register related Rexx external functions)
EXPORTSPEC ULONG APIENTRY  RxuTerm(PUCHAR name, ULONG numargs, RXSTRING args[],
               PSZ queuename, PRXSTRING retstr)
{

  INT    entries;          // Num of entries
  INT    j;                // Counter

  entries = sizeof(RxFncTable)/sizeof(PSZ);

  for (j = 0; j < entries; j++)
    RexxDeregisterFunction(RxFncTable[j]);

  retstr->strlength = sprintf(retstr->strptr,"0 %ld",entries);
  return VALID_ROUTINE;

}

#if defined(__BLD4OS2__)

// Rexx RxCreateNPipe external function (Create a Named Pipe)
EXPORTSPEC ULONG APIENTRY  RxCreateNPipe(PUCHAR name, ULONG numargs, RXSTRING args[],
                     PSZ queuename, PRXSTRING retstr)
{

  PSZ      pszFileName;
  HPIPE    hpipePipeHandle;
  ULONG    ulOpenMode = NP_WRITEBEHIND | NP_INHERIT
                      | NP_ACCESS_DUPLEX;
  ULONG    ulPipeMode = NP_WAIT | NP_TYPE_BYTE | NP_READMODE_BYTE
                      | NP_UNLIMITED_INSTANCES;
  ULONG    ulInstances=0;
  ULONG    ulOutBufSize = 65535;
  ULONG    ulInBufSize = 65535;
  ULONG    ulTimeOut = 0;
  APIRET   rc;            // Return code
  SHVBLOCK shvb;
  PCH      pch;
  ULONG    ulCtr;

  /*****************************************************************
   Arguments:

   Arg 1 = rexx variable name
   Arg 2 = pipename
   Arg 3 = open mode flags
   Arg 4 = pipe mode flags
   Arg 5 = instance count
   Arg 6 = output buffer size
   Arg 7 = input buffer size
   Arg 8 = timeout

   OpenMode Values
     W    = WriteBehind
     w    = NoWriteBehind
     I    = Inherit
     i    = NoInherit
     N(n) = Access Inbound
     O(o) = Access Outbound
     D(d) = Access Duplex

   PipeMode Values
     W    = Wait
     w    = NoWait
     T    = Pipe Type "Byte"
     t    = Pipe Type "Message"
     R    = Readmode "Byte"
     r    = Readmode "Message"

   Instance Count
     Integer

   Outbuf Size
     Integer

   Inbuf Size
     Integer

   Timeout
     Integer
  *****************************************************************/

  // Gotta have at least two arguments (rexx var name, pipename)
  if (numargs < 2)
    return INVALID_ROUTINE;

  if (args[0].strlength == 0 || args[1].strlength == 0)
    return INVALID_ROUTINE;

  strupr(args[0].strptr);
  memset(&shvb,0x00,sizeof(shvb));
  shvb.shvcode = RXSHV_SET;
  shvb.shvname.strptr = args[0].strptr;
  shvb.shvname.strlength = args[0].strlength;
  shvb.shvvalue.strptr = (PVOID) &hpipePipeHandle;
  shvb.shvvalue.strlength = sizeof(hpipePipeHandle);
  pszFileName = args[1].strptr;

  // Handle OpenModeFlags
  if (numargs >= 3)
    {
    for (ulCtr=0;ulCtr < args[2].strlength;ulCtr++)
      {
      switch ( args[2].strptr[ulCtr] )
        {
        case 'W':
          ulOpenMode |= NP_WRITEBEHIND;
          break;
        case 'w':
          ulOpenMode |= NP_NOWRITEBEHIND;
          break;
        case 'I':
          ulOpenMode |= NP_INHERIT;
          break;
        case 'i':
          ulOpenMode |= NP_NOINHERIT;
          break;
        case 'N':
        case 'n':
          ulOpenMode |= NP_ACCESS_INBOUND;
          break;
        case 'O':
        case 'o':
          ulOpenMode |= NP_ACCESS_OUTBOUND;
          break;
        case 'D':
        case 'd':
          ulOpenMode |= NP_ACCESS_DUPLEX;
          break;
        default:
          return INVALID_ROUTINE;
          break;
        }
      }
    }

  // Handle PipeModeFlags
  if (numargs >= 4)
    {
    for (ulCtr=0;ulCtr < args[3].strlength;ulCtr++)
      {
      switch ( args[3].strptr[ulCtr] )
        {
        case 'W':
          ulPipeMode |= NP_WAIT;
          break;
        case 'w':
          ulPipeMode |= NP_NOWAIT;
          break;
        case 'T':
          ulPipeMode |= NP_TYPE_BYTE;
          break;
        case 't':
          ulPipeMode |= NP_TYPE_MESSAGE;
          break;
        case 'R':
          ulPipeMode |= NP_READMODE_BYTE;
          break;
        case 'r':
          ulPipeMode |= NP_READMODE_MESSAGE;
          break;
        default:
          return INVALID_ROUTINE;
          break;
        }
      }
    }

  // Handle instance count
  if (numargs >= 5)
    {
    ulInstances = atol(args[4].strptr);
    ulPipeMode &= 0xff00;  // zero out rightmost 8 bits because now we're going to specify a number
    ulPipeMode |= ulInstances;
    }

  // Handle OutBufSize
  if (numargs >= 6)
    {
    if ( !(ulOutBufSize = atol(args[5].strptr)) )
      return INVALID_ROUTINE;
    }

  // Handle InBufSize
  if (numargs >= 7)
    {
    if ( !(ulInBufSize = atol(args[6].strptr)) )
      return INVALID_ROUTINE;
    }

  // Handle Timeout
  if (numargs >= 8)
    {
    if ( !(ulTimeOut = atol(args[7].strptr)) )
      return INVALID_ROUTINE;
    }

  rc = DosCreateNPipe(pszFileName,
                      &hpipePipeHandle, ulOpenMode,
                      ulPipeMode, ulOutBufSize,
                      ulInBufSize, ulTimeOut);

  if ( !rc )
    RexxVariablePool(&shvb);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;
}

// Rexx RxSetNPHState external function
EXPORTSPEC ULONG APIENTRY  RxSetNPHState(PUCHAR name, ULONG numargs, RXSTRING args[],
                     PSZ queuename, PRXSTRING retstr)
{

  HPIPE   hpipePipeHandle;
  CHAR    *cp;
  ULONG   state=0;
  APIRET  rc;

  // Gotta have at least two arguments
  if (numargs < 2)
    return INVALID_ROUTINE;

  if (args[0].strlength != sizeof(HPIPE))
    return INVALID_ROUTINE;
  memcpy(&hpipePipeHandle,args[0].strptr,sizeof(HPIPE));

  if (!args[1].strlength)
    return INVALID_ROUTINE;

  for (cp=args[1].strptr;cp;cp++)
    {
    switch (*cp)
      {
      case 'W':
        state |= NP_WAIT;
        break;
      case 'w':
        state |= NP_NOWAIT;
        break;
      case 'B':
        state |= NP_READMODE_BYTE;
        break;
      case 'M':
        state |= NP_READMODE_MESSAGE;
        break;
      default:
        break;
      } // end-switch
    } // end-for

  rc = DosSetNPHState(hpipePipeHandle,state);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;
}

// Rexx RxConnectNPipe external function
EXPORTSPEC ULONG APIENTRY  RxConnectNPipe(PUCHAR name, ULONG numargs, RXSTRING args[],
                      PSZ queuename, PRXSTRING retstr)
{

  HPIPE   hpipePipeHandle;
  APIRET  rc;

  // Gotta have at least one argument
  if (numargs < 1)
    return INVALID_ROUTINE;

  if (args[0].strlength != sizeof(HPIPE))
    return INVALID_ROUTINE;
  memcpy(&hpipePipeHandle,args[0].strptr,sizeof(HPIPE));

  rc = DosConnectNPipe(hpipePipeHandle);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;
}

// Rexx RxDisConnectNPipe external function
EXPORTSPEC ULONG APIENTRY  RxDisConnectNPipe(PUCHAR name, ULONG numargs, RXSTRING args[],
                         PSZ queuename, PRXSTRING retstr)
{

  HPIPE   hpipePipeHandle;
  APIRET  rc;

  // Gotta have at least one argument
  if (numargs < 1)
    return INVALID_ROUTINE;

  if (args[0].strlength != sizeof(HPIPE))
    return INVALID_ROUTINE;
  memcpy(&hpipePipeHandle,args[0].strptr,sizeof(HPIPE));

  rc = DosDisConnectNPipe(hpipePipeHandle);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;
}

// Rexx RxLoadModule external function
EXPORTSPEC ULONG APIENTRY  RxLoadModule(PUCHAR name, ULONG numargs, RXSTRING args[],
                    PSZ queuename, PRXSTRING retstr)
{

  CHAR      errtxt[251];
  HMODULE   hmod;
  APIRET    rc;
  SHVBLOCK  shvb;

  // Arg 1 = rexx variable into which module handle is put
  // Arg 2 = name of DLL

  if (numargs < 2)
    return INVALID_ROUTINE;

  if (args[0].strlength > 0)
    {
    memset(&shvb,0x00,sizeof(shvb));
    strupr(args[0].strptr);
    shvb.shvcode = RXSHV_SET;
    shvb.shvname.strptr = args[0].strptr;
    shvb.shvname.strlength = args[0].strlength;
    shvb.shvvalue.strptr = (PVOID) &hmod;
    shvb.shvvalue.strlength = sizeof(hmod);
    }
  else
    return INVALID_ROUTINE;

  if (args[1].strlength == 0)
    return INVALID_ROUTINE;

  memset(errtxt,0x00,sizeof(errtxt));
  rc = DosLoadModule(errtxt, (ULONG) sizeof(errtxt) - 1,
                     args[1].strptr, &hmod);

  RexxVariablePool(&shvb);

  retstr->strlength = sprintf(retstr->strptr,"%ld %s",rc,errtxt);

  return VALID_ROUTINE;
}

// Rexx RxFreeModule external function
EXPORTSPEC ULONG APIENTRY  RxFreeModule(PUCHAR name, ULONG numargs, RXSTRING args[],
                    PSZ queuename, PRXSTRING retstr)
{

  HMODULE   hmod;
  APIRET    rc;

  // Gotta have at least one argument
  if (numargs < 1)
    return INVALID_ROUTINE;

  if (args[0].strlength != sizeof(hmod))
    return INVALID_ROUTINE;

  memcpy(&hmod,args[0].strptr,args[0].strlength);

  rc = DosFreeModule(hmod);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;
}

// Rexx RxQueryModuleName external function
EXPORTSPEC ULONG APIENTRY  RxQueryModuleName(PUCHAR name, ULONG numargs, RXSTRING args[],
                         PSZ queuename, PRXSTRING retstr)
{

  HMODULE   hmod;
  APIRET    rc;
  SHVBLOCK  shvb;
  UCHAR     modname[251];

  // Arg 1 = rexx variable into which module name is put
  // Arg 2 = module handle

  if (numargs < 2)
    return INVALID_ROUTINE;

  if (args[0].strlength > 0)
    {
    memset(&shvb,0x00,sizeof(shvb));
    strupr(args[0].strptr);
    shvb.shvcode = RXSHV_SET;
    shvb.shvname.strptr = args[0].strptr;
    shvb.shvname.strlength = args[0].strlength;
    shvb.shvvalue.strptr = modname;
    }
  else
    return INVALID_ROUTINE;

  if (args[1].strlength != sizeof(hmod))
    return INVALID_ROUTINE;
  memcpy(&hmod,args[1].strptr,args[1].strlength);

  memset(modname,0x00,sizeof(modname));
  rc = DosQueryModuleName(hmod, sizeof(modname), modname);

  shvb.shvvalue.strlength = strlen(modname);
  RexxVariablePool(&shvb);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;
}

// Rexx RxQueryModuleHandle external function
EXPORTSPEC ULONG APIENTRY  RxQueryModuleHandle(PUCHAR name, ULONG numargs, RXSTRING args[],
                           PSZ queuename, PRXSTRING retstr)
{

  HMODULE   hmod;
  APIRET    rc;
  SHVBLOCK  shvb;

  // Arg 1 = name of rexx variable into which module handle is put
  // Arg 2 = module name

  if (numargs < 2)
    return INVALID_ROUTINE;

  if (args[0].strlength > 0)
    {
    memset(&shvb,0x00,sizeof(shvb));
    strupr(args[0].strptr);
    shvb.shvcode = RXSHV_SET;
    shvb.shvname.strptr = args[0].strptr;
    shvb.shvname.strlength = args[0].strlength;
    shvb.shvvalue.strptr = (PVOID) &hmod;
    shvb.shvvalue.strlength = sizeof(hmod);
    }
  else
    return INVALID_ROUTINE;

  if (args[1].strlength == 0)
    return INVALID_ROUTINE;

  rc = DosQueryModuleHandle(args[1].strptr, &hmod);

  RexxVariablePool(&shvb);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;
}

// Rexx RxQueryProcType external function
EXPORTSPEC ULONG APIENTRY  RxQueryProcType(PUCHAR name, ULONG numargs, RXSTRING args[],
                       PSZ queuename, PRXSTRING retstr)
{

  HMODULE   hmod;
  PSZ       pszProcName;
  ULONG     ulOrdinal, ulProcType = 0;
  APIRET    rc;

  // Arg 1 = module handle
  // Arg 2 = procedure name or ordinal

  // Gotta have at least two arguments (modhandle, ordinal/name)
  if (numargs < 2)
    return INVALID_ROUTINE;

  if (args[0].strlength != sizeof(hmod))
    return INVALID_ROUTINE;

  memcpy(&hmod,args[0].strptr,args[0].strlength);

  if (args[1].strlength == 0 )
    return INVALID_ROUTINE;

  if (string2long(args[1].strlength,args[1].strptr,(PLONG) &ulOrdinal) != TRUE)
    {
    ulOrdinal = 0;
    pszProcName = args[1].strptr;
    }
  else
    pszProcName = (PSZ) NULL;

  rc = DosQueryProcType(hmod, ulOrdinal, pszProcName, &ulProcType);

  sprintf(retstr->strptr,"%ld",rc);
  switch (ulProcType)
    {
    case PT_16BIT:
      strcat(retstr->strptr," 16");
      break;
    case PT_32BIT:
      strcat(retstr->strptr," 32");
      break;
    default:
      strcat(retstr->strptr," ?");
      break;
    }

  retstr->strlength = strlen(retstr->strptr);
  return VALID_ROUTINE;
}

// Rexx RxQueryProcAddr external function
EXPORTSPEC ULONG APIENTRY  RxQueryProcAddr(PUCHAR name, ULONG numargs, RXSTRING args[],
                       PSZ queuename, PRXSTRING retstr)
{

  HMODULE   hmod;
  ULONG     ulOrdinal;
  PSZ       pszProcName;
  PFN       pfnProcAddr;
  APIRET    rc;
  SHVBLOCK  shvb;

  // Arg 1 = rexx variable name into which procaddress is put
  // Arg 2 = module handle
  // Arg 3 = procedure name or ordinal

  if (numargs < 3)
    return INVALID_ROUTINE;

  if (args[0].strlength > 0)
    {
    memset(&shvb,0x00,sizeof(shvb));
    strupr(args[0].strptr);
    shvb.shvcode = RXSHV_SET;
    shvb.shvname.strptr = args[0].strptr;
    shvb.shvname.strlength = args[0].strlength;
    shvb.shvvalue.strptr = (PVOID) &pfnProcAddr;
    shvb.shvvalue.strlength = sizeof(pfnProcAddr);
    }
  else
    return INVALID_ROUTINE;

  if (args[1].strlength != sizeof(hmod))
    return INVALID_ROUTINE;
  memcpy(&hmod,args[1].strptr,args[1].strlength);

  if (args[2].strlength == 0 )
    return INVALID_ROUTINE;

  if ( !string2long(args[2].strlength,args[2].strptr,(PLONG) &ulOrdinal) )
    {
    ulOrdinal = 0;
    pszProcName = args[2].strptr;
    }
  else
    pszProcName = (PSZ) NULL;

  rc = DosQueryProcAddr(hmod, ulOrdinal, pszProcName, &pfnProcAddr);

  RexxVariablePool(&shvb);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  return VALID_ROUTINE;
}

// Rexx RxRegisterFuncAddress external function
EXPORTSPEC ULONG APIENTRY  RxRegisterFuncAddress(PUCHAR name, ULONG numargs, RXSTRING args[],
                             PSZ queuename, PRXSTRING retstr)
{
  APIRET         rc;
  PFUNCCALLINFO  pfci;
  SHVBLOCK       shvb;
  UCHAR          varname[300],varval[300];
  ULONG          numfuncargs=0,bufsize=0,ctr;

  // Arg#1 = function address (pointer)
  // Arg#2 = rexx stem in which function signature specification is supplied,
  //         and in which function handle is returned, as such:
  //           stem.0H = function handle
  //           stem.0R = return type:
  //                     char, short, long, int, etc.
  //           stem.0L = linkage type:
  //                     _System, _Optlink, cdecl, etc.
  //           stem.0 = number of arguments
  //           stem.n = type for argument "n" (n=1,2,3,...):
  //                    CHAR, UCHAR, SHORT, USHORT, LONG, ULONG, FLOAT, DOUBLE, STRUCT
  //           stem.n.0L = length (if argument type = STRUCT)

  // Must have at least 2 arguments
  if (numargs < 2)
    return INVALID_ROUTINE;
  // First argument must be a 32-bit pointer
  if (args[0].strlength != sizeof(PVOID))
    return INVALID_ROUTINE;
  // 2nd argument must be non-zero length
  if (!args[1].strlength)
    return INVALID_ROUTINE;
  strupr(args[1].strptr);

  // Initialize SHVBLOCK
  memset(&shvb,0x00,sizeof(shvb));
  shvb.shvcode = RXSHV_FETCH;
  shvb.shvname.strptr = varname;
  shvb.shvvalue.strptr = varval;

  // Get number of function arguments
  sprintf(varname,"%s0",args[1].strptr);
  memset(varval,0x00,sizeof(varval));
  shvb.shvname.strlength = strlen(varname);
  shvb.shvvalue.strlength = sizeof(varval)-1;
  RexxVariablePool(&shvb);
  if (shvb.shvret & RXSHV_NEWV || shvb.shvret & RXSHV_TRUNC)
    return INVALID_ROUTINE;
  numfuncargs = atoi(varval);

  // Allocate function info block
  bufsize = sizeof(FUNCCALLINFO) + (numfuncargs?(numfuncargs-1)*(sizeof(FUNCCALLTYPEINFO)):0);
  pfci = (PFUNCCALLINFO) malloc(bufsize);
  memset(pfci,0x00,bufsize);

  // Set function pointer and numfuncargs elements of function info buffer
  memcpy(&pfci->pfunc,args[0].strptr,sizeof(PVOID));
  pfci->numfuncargs = numfuncargs;

  // Set function return type element of function info buffer
  sprintf(varname,"%s0R",args[1].strptr);
  memset(varval,0x00,sizeof(varval));
  shvb.shvname.strlength = strlen(varname);
  shvb.shvvalue.strlength = sizeof(varval)-1;
  RexxVariablePool(&shvb);
  if (shvb.shvret & RXSHV_NEWV || shvb.shvret & RXSHV_TRUNC)
    {
    free(pfci);
    return INVALID_ROUTINE;
    }
  if (!getDataType(&pfci->ret,varval))
    {
    free(pfci);
    return INVALID_ROUTINE;
    }

  // Set function linkage type element of function info buffer
  sprintf(varname,"%s0L",args[1].strptr);
  memset(varval,0x00,sizeof(varval));
  shvb.shvname.strlength = strlen(varname);
  shvb.shvvalue.strlength = sizeof(varval)-1;
  RexxVariablePool(&shvb);
  if (shvb.shvret & RXSHV_NEWV || shvb.shvret & RXSHV_TRUNC)
    {
    free(pfci);
    return INVALID_ROUTINE;
    }
  if (!getLinkageType(&pfci->linkage,varval))
    {
    free(pfci);
    return INVALID_ROUTINE;
    }

  // Spin through each argument and record it in function info block
  for (ctr=1;ctr<=numfuncargs;ctr++)
    {
    sprintf(varname,"%s%ld",args[1].strptr,ctr);
    shvb.shvname.strlength = strlen(varname);
    memset(varval,0x00,sizeof(varval));
    shvb.shvvalue.strlength = sizeof(varval)-1;
    RexxVariablePool(&shvb);
    if (shvb.shvret & RXSHV_NEWV || shvb.shvret & RXSHV_TRUNC)
      {
      free(pfci);
      return INVALID_ROUTINE;
      }
    if (!getDataType(&pfci->arg[ctr-1],varval))
      {
      free(pfci);
      return INVALID_ROUTINE;
      }
    // Handle arg-type specific processing (getting length of structs, adjusting required stack size, etc.)
    if (pfci->arg[ctr-1].argbyref)
      {
      // Argument is by reference, so argument's stack footprint is that of a pointer
      pfci->arg[ctr-1].arglen = sizeof(PVOID);
      }
    else
      {
      switch (pfci->arg[ctr-1].argtype)
        {
        case ARG_TYPE_STRUCT:
          sprintf(varname,"%s%ld.0L",args[1].strptr,ctr);
          shvb.shvname.strlength = strlen(varname);
          memset(varval,0x00,sizeof(varval));
          shvb.shvvalue.strlength = sizeof(varval)-1;
          RexxVariablePool(&shvb);
          if (shvb.shvret & RXSHV_NEWV || shvb.shvret & RXSHV_TRUNC)
            {
            free(pfci);
            return INVALID_ROUTINE;
            }
          pfci->arg[ctr-1].arglen = atoi(varval);
          // If struct length isn't integral multiple of dwords, then pad to dword
          if (pfci->arg[ctr-1].arglen%sizeof(ULONG))
            pfci->arg[ctr-1].arglen = sizeof(ULONG)*(1+(pfci->arg[ctr-1].arglen/sizeof(ULONG)));
          if (!pfci->arg[ctr-1].arglen)
            {
            free(pfci);
            return INVALID_ROUTINE;
            }
          break;
        default:
          pfci->arg[ctr-1].arglen = sizeof(ULONG);    // even for "char" because we align on dword boundary
          break;
        } // end-switch on arg type
      } // end-if argument is by reference
    pfci->stackforargs += pfci->arg[ctr-1].arglen;
    } // end-for each function argument

  // Set function "handle"
  shvb.shvcode = RXSHV_SET;
  sprintf(varname,"%s0H",args[1].strptr);
  shvb.shvname.strlength = strlen(varname);
  shvb.shvvalue.strptr = (char*)pfci;
  shvb.shvvalue.strlength = bufsize;
  RexxVariablePool(&shvb);
  free(pfci);

  retstr->strlength = 1;
  *retstr->strptr = '1';
  return VALID_ROUTINE;

}

BOOL getDataType(PFUNCCALLTYPEINFO parg, char * str)
{
  ULONG  temp;
  PCH    cp=str;

  // Check to see if this "by reference" or "by value"
  parg->argbyref = FALSE;
  if (*str == '&')
    {
    parg->argbyref = TRUE;
    cp++;
    }
  strupr(cp);
  if (!strcmp(cp,"CHAR"))
    temp = ARG_TYPE_CHAR;
  else if (!strcmp(cp,"UCHAR"))
    temp = ARG_TYPE_UCHAR;
  else if (!strcmp(cp,"LONG"))
    temp = ARG_TYPE_LONG;
  else if (!strcmp(cp,"ULONG"))
    temp = ARG_TYPE_ULONG;
  else if (!strcmp(cp,"SHORT"))
    temp = ARG_TYPE_SHORT;
  else if (!strcmp(cp,"USHORT"))
    temp = ARG_TYPE_USHORT;
  else if (!strcmp(cp,"PVOID"))
    temp = ARG_TYPE_PVOID;
  //else if (!strcmp(cp,"FLOAT"))
  //  temp = ARG_TYPE_FLOAT;
  //else if (!strcmp(cp,"DOUBLE"))
  //  temp = ARG_TYPE_DOUBLE;
  else if (!strcmp(cp,"STRUCT"))
    temp = ARG_TYPE_STRUCT;
  else
    return FALSE;

  parg->argtype = temp;
  return TRUE;
}

BOOL getLinkageType(ULONG * ptype, char * str)
{
  ULONG  temp;

  strupr(str);
  if (!strcmp(str,"_SYSTEM"))
    temp = LINK_TYPE_SYSTEM;
  //else if (!strcmp(str,"_OPTLINK"))
  //  temp = LINK_TYPE_OPTLINK;
  //else if (!strcmp(str,"CDECL"))
  //  temp = LINK_TYPE_CDECL;
  else
    return FALSE;

  *ptype = temp;
  return TRUE;
}

// Rexx RxCallFuncAddress external function
EXPORTSPEC ULONG APIENTRY  RxCallFuncAddress(PUCHAR name, ULONG numargs, RXSTRING args[],
                         PSZ queuename, PRXSTRING retstr)
{
  APIRET                      rc;
  EXCEPTIONREGISTRATIONRECORD regrec;
  EXCEPTIONREPORTRECORD       rptrecb, rptreca;
  PCALLFUNCADDRESS            pfunc;
  PFUNCCALLINFO               pfci;
  PCH                         pstackargbuf=0, cp;
  ULONG                       ctr;

  // Arg#1 = function info buffer
  // Arg#2...n = arguments to function

  // Get function call info block
  if (numargs < 1)
    return INVALID_ROUTINE;
  if (args[0].strlength < sizeof(FUNCCALLINFO))
    return INVALID_ROUTINE;
  pfci = (PFUNCCALLINFO)args[0].strptr;
  if (numargs < pfci->numfuncargs+1)
    return INVALID_ROUTINE;

  // Length of supplied info buffer must be consistent with number of arguments specified
  if (args[0].strlength !=
      sizeof(FUNCCALLINFO) + (pfci->numfuncargs?(pfci->numfuncargs-1)*(sizeof(FUNCCALLTYPEINFO)):0))
    return INVALID_ROUTINE;
  pfunc = (PCALLFUNCADDRESS)pfci->pfunc;

  pstackargbuf = (char*)malloc(pfci->stackforargs);
  memset(pstackargbuf,0x00,pfci->stackforargs);
  for (cp=pstackargbuf,ctr=1;ctr<=pfci->numfuncargs;ctr++)
    {
    memcpy(cp,args[ctr].strptr,min(pfci->arg[ctr-1].arglen,args[ctr].strlength));
    cp += pfci->arg[ctr-1].arglen;
    }

  // Register our own exception handler (which will set up stack and registers for call)
  regrec.ExceptionHandler = callFunctionExcpHandler;
  rc=DosSetExceptionHandler(&regrec);
  if (rc)
    return INVALID_ROUTINE;

  // Set up exception report record for use by our exception handler
  memset(&rptrecb,0x00,sizeof(rptrecb));
  rptrecb.ExceptionNum = XCPT_CALL_FUNCTION_EXCPNUMB;
  rptrecb.cParameters = 2;
  rptrecb.ExceptionInfo[0] = (ULONG)pfci;
  rptrecb.ExceptionInfo[1] = (ULONG)pstackargbuf;

  // Copy "before" exceptionReportRecord to "after" exceptionReportRecord
  memcpy(&rptreca,&rptrecb,sizeof(rptrecb));
  rptreca.ExceptionNum = XCPT_CALL_FUNCTION_EXCPNUMA;

  // Raise artificial exception to set up for call (set up stack and registers)
  // (this will effectively invoke function "callFunctionExcpHandler")
  DosRaiseException(&rptrecb);

  // Issue call to function (stack & registers have been set up by exception handler)
  (*pfunc)();

  // Raise artificial exception to handle return from call (get rc from EAX, clean up stack)
  // (this will effectively invoke function "callFunctionExcpHandler")
  DosRaiseException(&rptreca);

  // De-register our own exception handler
  rc=DosUnsetExceptionHandler(&regrec);
  if (rc)
    return INVALID_ROUTINE;

  switch(pfci->linkage)
    {
    case LINK_TYPE_SYSTEM:
      retstr->strlength = sprintf(retstr->strptr,"%ld",pfci->retcode);
      break;
    default:
      retstr->strlength = 0;
      break;
    }
  return VALID_ROUTINE;

}

ULONG _System callFunctionExcpHandler(PEXCEPTIONREPORTRECORD       prpt
                                     ,PEXCEPTIONREGISTRATIONRECORD preg
                                     ,PCONTEXTRECORD               pctxt
                                     ,PVOID                        pv
                                     )
{
  PFUNCCALLINFO  pfci;
  ULONG          ctr;
  PCH            cp;

  pfci = (PFUNCCALLINFO)prpt->ExceptionInfo[0];
  switch(prpt->ExceptionNum)
    {
    // Handle before-call case
    case XCPT_CALL_FUNCTION_EXCPNUMB:
      switch(pfci->linkage)
        {
        case LINK_TYPE_SYSTEM:
          // Place arguments on "stack" from lexical right to left
          // Decrement ESP by size of argument list, copy argument list into stack location
          pfci->origesp = pctxt->ctx_RegEsp + 4; // gotta add 4 for some reason
          cp = (char*) (pfci->origesp - pfci->stackforargs);
          memcpy(cp,(char*)prpt->ExceptionInfo[1],pfci->stackforargs);
          pctxt->ctx_RegEsp = (ULONG)cp;
          break;
        default:
          break;
        }
      return XCPT_CONTINUE_EXECUTION;
      break;
    // Handle after-call case
    case XCPT_CALL_FUNCTION_EXCPNUMA:
      switch(pfci->linkage)
        {
        case LINK_TYPE_SYSTEM:
          // Reclaim stack space used for arguments, put EAX into return code
          pctxt->ctx_RegEsp = pfci->origesp;
          pfci->uretcode = pctxt->ctx_RegEax;
          break;
        default:
          break;
        }
      return XCPT_CONTINUE_EXECUTION;
      break;
    default:
      return XCPT_CONTINUE_SEARCH;
      break;
    }

  return XCPT_CONTINUE_SEARCH;

}

// Rexx RxCallEntryPoint external function
EXPORTSPEC ULONG APIENTRY  RxCallEntryPoint(PUCHAR name, ULONG numargs, RXSTRING args[],
                        PSZ queuename, PRXSTRING retstr)
{

  APIRET    rc;            // Return code
  PVOID     pData[19];
  PFN       pfnProcAddr;
  ULONG     RegionSize, AllocFlags, ctr;
  BOOL      malloced[19];

  // Gotta have at least one argument (procaddr [,data pointer, ...] )
  if (numargs < 1)
    return INVALID_ROUTINE;

  if (args[0].strlength != sizeof(pfnProcAddr))
    return INVALID_ROUTINE;

  memcpy(&pfnProcAddr,args[0].strptr,args[0].strlength);

  for (ctr=0;ctr < numargs - 1;ctr++)
    {
    malloced[ctr] = FALSE;
    if (args[ctr+1].strlength == sizeof(pData[ctr]))
      {
      RegionSize = 0xffffffff;      // 2**32 = 4 gigabytes
      memcpy(&(pData[ctr]),args[ctr+1].strptr,args[ctr+1].strlength);
      if (DosQueryMem(pData[ctr], &RegionSize, &AllocFlags) != 0)
        {
        rc = DosAllocMem(&(pData[ctr]),args[ctr+1].strlength,
                         PAG_COMMIT | PAG_WRITE);
        if (rc != 0)
          {
          printf("DosAllocMem failed with rc = %ld\n",rc);
          return INVALID_ROUTINE;
          }
        memcpy(pData[ctr],args[ctr+1].strptr,args[ctr+1].strlength);
        malloced[ctr] = TRUE;
        } // end-if
      } // end-if
    else
      {
      rc = DosAllocMem(&(pData[ctr]),args[ctr+1].strlength,
                       PAG_COMMIT | PAG_WRITE);
      if (rc != 0)
        {
        printf("DosAllocMem failed with rc = %ld\n",rc);
        return INVALID_ROUTINE;
        }
      memcpy(pData[ctr],args[ctr+1].strptr,args[ctr+1].strlength);
      malloced[ctr] = TRUE;
      } // end-else
    } // end-for

  switch (numargs)
    {
    case 1:
      rc = pfnProcAddr();
      break;
    case 2:
      rc = pfnProcAddr(pData[0]);
      break;
    case 3:
      rc = pfnProcAddr(pData[0],pData[1]);
      break;
    case 4:
      rc = pfnProcAddr(pData[0],pData[1],pData[2]);
      break;
    case 5:
      rc = pfnProcAddr(pData[0],pData[1],pData[2],pData[3]);
      break;
    case 6:
      rc = pfnProcAddr(pData[0],pData[1],pData[2],pData[3],pData[4]);
      break;
    case 7:
      rc = pfnProcAddr(pData[0],pData[1],pData[2],pData[3],pData[4],
                       pData[5]);
      break;
    case 8:
      rc = pfnProcAddr(pData[0],pData[1],pData[2],pData[3],pData[4],
                       pData[5],pData[6]);
      break;
    case 9:
      rc = pfnProcAddr(pData[0],pData[1],pData[2],pData[3],pData[4],
                       pData[5],pData[6],pData[7]);
      break;
    case 10:
      rc = pfnProcAddr(pData[0],pData[1],pData[2],pData[3],pData[4],
                       pData[5],pData[6],pData[7],pData[8]);
      break;
    case 11:
      rc = pfnProcAddr(pData[0],pData[1],pData[2],pData[3],pData[4],
                       pData[5],pData[6],pData[7],pData[8],pData[9]);
      break;
    case 12:
      rc = pfnProcAddr(pData[0],pData[1],pData[2],pData[3],pData[4],
                       pData[5],pData[6],pData[7],pData[8],pData[9],
                       pData[10]);
      break;
    case 13:
      rc = pfnProcAddr(pData[0],pData[1],pData[2],pData[3],pData[4],
                       pData[5],pData[6],pData[7],pData[8],pData[9],
                       pData[10],pData[11]);
      break;
    case 14:
      rc = pfnProcAddr(pData[0],pData[1],pData[2],pData[3],pData[4],
                       pData[5],pData[6],pData[7],pData[8],pData[9],
                       pData[10],pData[11],pData[12]);
      break;
    case 15:
      rc = pfnProcAddr(pData[0],pData[1],pData[2],pData[3],pData[4],
                       pData[5],pData[6],pData[7],pData[8],pData[9],
                       pData[10],pData[11],pData[12],pData[13]);
      break;
    case 16:
      rc = pfnProcAddr(pData[0],pData[1],pData[2],pData[3],pData[4],
                       pData[5],pData[6],pData[7],pData[8],pData[9],
                       pData[10],pData[11],pData[12],pData[13],
                       pData[14]);
      break;
    case 17:
      rc = pfnProcAddr(pData[0],pData[1],pData[2],pData[3],pData[4],
                       pData[5],pData[6],pData[7],pData[8],pData[9],
                       pData[10],pData[11],pData[12],pData[13],
                       pData[14],pData[15]);
      break;
    case 18:
      rc = pfnProcAddr(pData[0],pData[1],pData[2],pData[3],pData[4],
                       pData[5],pData[6],pData[7],pData[8],pData[9],
                       pData[10],pData[11],pData[12],pData[13],
                       pData[14],pData[15],pData[16]);
      break;
    case 19:
      rc = pfnProcAddr(pData[0],pData[1],pData[2],pData[3],pData[4],
                       pData[5],pData[6],pData[7],pData[8],pData[9],
                       pData[10],pData[11],pData[12],pData[13],
                       pData[14],pData[15],pData[16],pData[17]);
      break;
    case 20:
      rc = pfnProcAddr(pData[0],pData[1],pData[2],pData[3],pData[4],
                       pData[5],pData[6],pData[7],pData[8],pData[9],
                       pData[10],pData[11],pData[12],pData[13],
                       pData[14],pData[15],pData[16],pData[17],pData[18]);
      break;
    default:
      printf("Too many arguments supplied to RxCallEntryPoint\n");
      printf("Limit is 19\n");
      break;
    }

  for (ctr=0;ctr < numargs - 1;ctr++)
    {
    if (malloced[ctr])
      DosFreeMem(pData);
    }

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);
  return VALID_ROUTINE;
}

// Rexx RxCallProcAddr external function
EXPORTSPEC ULONG APIENTRY  RxCallProcAddr(PUCHAR name, ULONG numargs, RXSTRING args[],
                      PSZ queuename, PRXSTRING retstr)
{

  typedef union _UFuncPointers
    {
    PVOID          pfn;
    PGENERIC16ARG0 pfn16a0;
    PGENERIC16ARG1 pfn16a1;
    PGENERIC16ARG2 pfn16a2;
    PGENERIC16ARG3 pfn16a3;
    PGENERIC16ARG4 pfn16a4;
    PGENERIC16ARG5 pfn16a5;

    PGENERIC32SYSARG0 pfn32a0;
    PGENERIC32SYSARG1 pfn32a1;
    PGENERIC32SYSARG2 pfn32a2;
    PGENERIC32SYSARG3 pfn32a3;
    PGENERIC32SYSARG4 pfn32a4;
    PGENERIC32SYSARG5 pfn32a5;
    } UFUNCPTR, *PUFUNCPTR;

  APIRET    frc;
  UFUNCPTR  ufp;
  ULONG     ctr;
  PVOID     parg[5];

  // Arg#1       = addressing mode (16 or 32)
  // Arg#2       = procedure address (always a 32-bit address)
  // Arg#3-Arg#7 = arguments 1-5 to procedure (must be pointers)

  if (numargs < 2 || numargs > 7)
    return INVALID_ROUTINE;

  if (args[0].strlength == 0)
    return INVALID_ROUTINE;

  if (args[1].strlength != sizeof(ufp.pfn))
    return INVALID_ROUTINE;
  memcpy(&ufp.pfn,args[1].strptr,sizeof(ufp.pfn));

  for (ctr=2;ctr < numargs;ctr++)
    {
    if (args[ctr].strlength != sizeof(PVOID))
      return INVALID_ROUTINE;
    memcpy(&parg[ctr-2],args[ctr].strptr,sizeof(PVOID));
    }

  strupr(args[0].strptr);
  switch (*args[0].strptr)
    {
    case '1':
      {
      APIRET16 rc;
      switch (numargs)
        {
        case 2:
          rc=(*ufp.pfn16a0)();
          break;
        case 3:
          rc=(*ufp.pfn16a1)((PVOID _Seg16)parg[0]);
          break;
        default:
          return INVALID_ROUTINE;
          break;
        }
      frc = rc;
      }
      break;
    case '3':
      {
      APIRET rc;
      switch (numargs)
        {
        case 2:
          rc=(*ufp.pfn32a0)();
          break;
        case 3:
          rc=(*ufp.pfn32a1)(parg[0]);
          break;
        default:
          return INVALID_ROUTINE;
          break;
        }
      frc = rc;
      }
      break;
    default:
      return INVALID_ROUTINE;
      break;
    }

  retstr->strlength = sprintf(retstr->strptr,"%ld",frc);

  return VALID_ROUTINE;

}

// Rexx RxAddMacro external function
EXPORTSPEC ULONG APIENTRY  RxAddMacro(PUCHAR name, ULONG numargs, RXSTRING args[],
                  PSZ queuename, PRXSTRING retstr)
{

  ULONG     ulSrchOrder;
  APIRET    rc;            // Return code

  // Gotta have at least two arguments (funcname, sourcefile [,srchorder])
  if (numargs < 2)
    return INVALID_ROUTINE;

  if (args[0].strlength == 0 || args[1].strlength == 0)
    return INVALID_ROUTINE;

  if (numargs >= 3)
    {
    if (args[2].strlength == 0)
      ulSrchOrder = RXMACRO_SEARCH_BEFORE;
    else
      {
      switch (*(args[2].strptr))
        {
        case 'b':
        case 'B':
          ulSrchOrder = RXMACRO_SEARCH_BEFORE;
          break;
        case 'a':
        case 'A':
          ulSrchOrder = RXMACRO_SEARCH_AFTER;
          break;
        default:
          return INVALID_ROUTINE;
          break;
        } // end-switch
      }
    }
  else
    ulSrchOrder = RXMACRO_SEARCH_BEFORE;

  rc = RexxAddMacro(args[0].strptr, args[1].strptr, ulSrchOrder);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);
  return VALID_ROUTINE;
}

// Rexx RxDropMacro external function
EXPORTSPEC ULONG APIENTRY  RxDropMacro(PUCHAR name, ULONG numargs, RXSTRING args[],
                   PSZ queuename, PRXSTRING retstr)
{

  APIRET    rc;            // Return code

  // Gotta have at least one argument (funcname)
  if (numargs < 1)
    return INVALID_ROUTINE;

  if (args[0].strlength == 0)
    return INVALID_ROUTINE;

  rc = RexxDropMacro(args[0].strptr);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);
  return VALID_ROUTINE;
}

// Rexx RxClearMacroSpace external function
EXPORTSPEC ULONG APIENTRY  RxClearMacroSpace(PUCHAR name, ULONG numargs, RXSTRING args[],
                         PSZ queuename, PRXSTRING retstr)
{

  APIRET    rc;            // Return code

  rc = RexxClearMacroSpace();

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);
  return VALID_ROUTINE;
}

// Rexx RxSaveMacroSpace external function
EXPORTSPEC ULONG APIENTRY  RxSaveMacroSpace(PUCHAR name, ULONG numargs, RXSTRING args[],
                        PSZ queuename, PRXSTRING retstr)
{

  APIRET    rc;            // Return code
  ULONG     ulFuncCount=0;
  char      *cp, **fn=NULL;

  // Gotta have at least two arguments (funcnamelist, macrolibfile)
  if (numargs < 2)
    return INVALID_ROUTINE;

  if (!args[1].strlength)
    return INVALID_ROUTINE;

  if (args[0].strlength)
    {
    fn = (char**) malloc(args[0].strlength * sizeof(char*));
    for (cp=strtok(args[0].strptr," ");cp;cp=strtok(NULL," "))
      {
      fn[ulFuncCount] = cp;
      ulFuncCount++;
      } // end-for each token
    }

  rc = RexxSaveMacroSpace(ulFuncCount,fn,args[1].strptr);

  free(fn);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);
  return VALID_ROUTINE;
}

// Rexx RxLoadMacroSpace external function
EXPORTSPEC ULONG APIENTRY  RxLoadMacroSpace(PUCHAR name, ULONG numargs, RXSTRING args[],
                        PSZ queuename, PRXSTRING retstr)
{

  APIRET    rc;            // Return code
  ULONG     ulFuncCount=0;
  char      *cp, **fn=NULL;

  // Gotta have at least two arguments (funcnamelist, macrolibfile)
  if (numargs < 2)
    return INVALID_ROUTINE;

  if (!args[1].strlength)
    return INVALID_ROUTINE;

  if (args[0].strlength)
    {
    fn = (char**) malloc(args[0].strlength * sizeof(char*));
    for (cp=strtok(args[0].strptr," ");cp;cp=strtok(NULL," "))
      {
      fn[ulFuncCount] = cp;
      ulFuncCount++;
      } // end-for each token
    }

  rc = RexxLoadMacroSpace(ulFuncCount,fn,args[1].strptr);

  free(fn);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);
  return VALID_ROUTINE;
}

// Rexx RxQueryMacro external function
EXPORTSPEC ULONG APIENTRY  RxQueryMacro(PUCHAR name, ULONG numargs, RXSTRING args[],
                    PSZ queuename, PRXSTRING retstr)
{

  APIRET    rc;            // Return code
  USHORT    usPosition;

  // Gotta have at least one argument (funcname)
  if (numargs < 1)
    return INVALID_ROUTINE;

  if (args[0].strlength == 0)
    return INVALID_ROUTINE;

  rc = RexxQueryMacro(args[0].strptr, &usPosition);
  if (rc == 0)
    {
    sprintf(retstr->strptr,"%ld ",rc);
    switch (usPosition)
      {
      case RXMACRO_SEARCH_BEFORE:
        strcat(retstr->strptr,"Before");
        break;
      case RXMACRO_SEARCH_AFTER:
        strcat(retstr->strptr,"After");
        break;
      default:
        strcat(retstr->strptr,"?");
        break;
      }
    }
  else
    sprintf(retstr->strptr,"%ld",rc);

  retstr->strlength = strlen(retstr->strptr);
  return VALID_ROUTINE;
}

// Rexx RxReorderMacro external function
EXPORTSPEC ULONG APIENTRY  RxReorderMacro(PUCHAR name, ULONG numargs, RXSTRING args[],
                      PSZ queuename, PRXSTRING retstr)
{

  APIRET    rc;            // Return code
  ULONG     ulPosition;

  // Gotta have at least two arguments (funcname, position)
  if (numargs < 2)
    return INVALID_ROUTINE;

  if (args[0].strlength == 0 || args[1].strlength == 0)
    return INVALID_ROUTINE;

  switch (*(args[1].strptr))
    {
    case 'b':
    case 'B':
      ulPosition = RXMACRO_SEARCH_BEFORE;
      break;
    case 'a':
    case 'A':
      ulPosition = RXMACRO_SEARCH_AFTER;
      break;
    default:
      return INVALID_ROUTINE;
      break;
    }

  rc = RexxReorderMacro(args[0].strptr, ulPosition);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);
  return VALID_ROUTINE;
}

// Rexx RxNbSessionStatus external function
EXPORTSPEC ULONG APIENTRY  RxNbSessionStatus(PUCHAR name, ULONG numargs, RXSTRING args[],
                         PSZ queuename, PRXSTRING retstr)
{
  struct   _NcbSessStatInfo
    {
    byte     local_session_number;       // Local session number
    byte     session_state;              // State of session
    byte     local_name[16];             // Local name
    byte     remote_name[16];            // Remote name
    byte     active_receives;            // # of RECEIVE cmnds out
    byte     active_sends;               // # of SEND, CHAIN.SEND out
    } *pnssi;

  CHAR       varname[250], varval[50], ncbrc1, ncbrc2, ncbrc3;
  LONG       longnum, ctr;
  USHORT     rc;
  struct     network_control_block ncb;
  struct     ncb_session_status    ncbss;
  struct     ncb_reset             ncbrst;
  PCH        cptr;
  SHVBLOCK   shvb;
  PNETBIOSFN pNetBiosFn;

  // Gotta have at least two arguments (nbname, rexx-stem)
  if (numargs < 2)
    {
    printf("First argument must be a 1-16 character NetBios name\n");
    return INVALID_ROUTINE;
    }

  if (args[0].strlength > sizeof(ncb.ncb_name) || args[0].strlength == 0)
    {
    printf("First argument must be a 1-16 character NetBios name\n");
    return INVALID_ROUTINE;
    }

  if (args[1].strlength == 0)
    {
    printf("Arg 2 must be a valid Rexx stem\n");
    return INVALID_ROUTINE;
    }

  if ( !loadAPI("ACSNETB","NETBIOS",0,(PFN *)&pNetBiosFn) )
    return INVALID_ROUTINE;

  // Fold stem name to upper case
  strupr(args[1].strptr);

  shvb.shvnext = NULL;
  shvb.shvname.strptr = varname;
  shvb.shvvalue.strptr = varval;

  shvb.shvcode = RXSHV_DROPV;
  sprintf(varname,"%.*s",args[1].strlength,args[1].strptr);
  shvb.shvname.strlength = strlen(varname);
  RexxVariablePool(&shvb);

  // Do a reset, if requested
  if (numargs >= 3)
    {
    memset(&ncbrst,0x00,sizeof(ncbrst));
    ncbrst.ncb_command  = NB_RESET_WAIT;
    ncbrst.ncb_lsn      = 0x00;
    ncbrst.req_sessions = 0x01;
    ncbrst.req_commands = 0x01;
    ncbrst.req_names    = 0x01;
    rc = (*pNetBiosFn)((char * _Seg16) &ncbrst);
    //rc = NETBIOS((char * _Seg16) &ncbrst);
    memcpy(&ncbrc1,&(ncbrst.ncb_retcode),sizeof(ncbrc1));
    }

  // Get session status
  memset(&ncb,0x00,sizeof(ncb));
  ncb.ncb_command = NB_SESSION_STATUS_WAIT;
  memcpy(ncb.ncb_name,args[0].strptr,args[0].strlength);
  ncb.ncb_lana_num = 0x00;
  ncb.ncb_length = sizeof(ncbss);
  //ncb.ncb_buffer_address = (char * _Seg16) malloc(ncb.ncb_length);
  ncb.ncb_buffer_address = (char *) malloc(ncb.ncb_length);

  // First, get required buffer length and malloc accordingly
  rc = (*pNetBiosFn)((char * _Seg16) &ncb);
  //rc = NETBIOS((char * _Seg16) &ncb);
  memset(&ncbss,0x00,sizeof(ncbss));
  memcpy(&ncbss,ncb.ncb_buffer_address,sizeof(ncbss));
  free(ncb.ncb_buffer_address);
  ncb.ncb_length = 4 + ((short) ncbss.sessions_using_name * 36);
  ncb.ncb_buffer_address = (char * _Seg16) malloc(ncb.ncb_length);

  // Then, actually get the information
  rc = (*pNetBiosFn)((char * _Seg16) &ncb);
  //rc = NETBIOS((char * _Seg16) &ncb);
  memcpy(&ncbrc2,&(ncb.ncb_retcode),sizeof(ncbrc2));

  memset(&ncbss,0x00,sizeof(ncbss));
  if (ncb.ncb_length < sizeof(ncbss))
    memcpy(&ncbss,ncb.ncb_buffer_address,ncb.ncb_length);
  else
    memcpy(&ncbss,ncb.ncb_buffer_address,sizeof(ncbss));

  shvb.shvcode = RXSHV_SET;
  sprintf(varname,"%.*s0",args[1].strlength,args[1].strptr);
  shvb.shvname.strlength = strlen(varname);
  sprintf(varval,"%d",ncbss.sessions_using_name);
  shvb.shvvalue.strlength = strlen(varval);
  RexxVariablePool(&shvb);

  // Point to beginning of ncbss struct
  cptr = (PCH) ncb.ncb_buffer_address;
  // Point four bytes beyond, to beginning of repeated info blocks
  pnssi = (struct _NcbSessStatInfo *) (cptr + 4);

  for (ctr=1;ctr <= ncbss.sessions_using_name;ctr++)
    {
    byte     active_receives;            // # of RECEIVE cmnds out
    byte     active_sends;               // # of SEND, CHAIN.SEND out

    sprintf(varname,"%.*s%ld.1",args[1].strlength,args[1].strptr,ctr);
    shvb.shvname.strlength = strlen(varname);
    shvb.shvvalue.strptr = varval;
    sprintf(varval,"%d",pnssi->local_session_number);
    shvb.shvvalue.strlength = strlen(varval);
    RexxVariablePool(&shvb);

    sprintf(varname,"%.*s%ld.2",args[1].strlength,args[1].strptr,ctr);
    shvb.shvname.strlength = strlen(varname);
    shvb.shvvalue.strptr = varval;
    sprintf(varval,"%x",pnssi->session_state);
    shvb.shvvalue.strlength = strlen(varval);
    RexxVariablePool(&shvb);

    sprintf(varname,"%.*s%ld.3",args[1].strlength,args[1].strptr,ctr);
    shvb.shvname.strlength = strlen(varname);
    shvb.shvvalue.strptr = pnssi->local_name;
    shvb.shvvalue.strlength = sizeof(pnssi->local_name);
    RexxVariablePool(&shvb);

    sprintf(varname,"%.*s%ld.4",args[1].strlength,args[1].strptr,ctr);
    shvb.shvname.strlength = strlen(varname);
    shvb.shvvalue.strptr = pnssi->remote_name;
    shvb.shvvalue.strlength = sizeof(pnssi->remote_name);
    RexxVariablePool(&shvb);

    sprintf(varname,"%.*s%ld.5",args[1].strlength,args[1].strptr,ctr);
    shvb.shvname.strlength = strlen(varname);
    shvb.shvvalue.strptr = varval;
    sprintf(varval,"%d",pnssi->active_receives);
    shvb.shvvalue.strlength = strlen(varval);
    RexxVariablePool(&shvb);

    sprintf(varname,"%.*s%ld.6",args[1].strlength,args[1].strptr,ctr);
    shvb.shvname.strlength = strlen(varname);
    shvb.shvvalue.strptr = varval;
    sprintf(varval,"%d",pnssi->active_sends);
    shvb.shvvalue.strlength = strlen(varval);
    RexxVariablePool(&shvb);

    pnssi++;

    }

  free(ncb.ncb_buffer_address);

  // Reset again, if first Reset was done
  if (numargs >= 3)
    {
    memset(&ncbrst,0x00,sizeof(ncbrst));
    ncbrst.ncb_command  = NB_RESET_WAIT;
    ncbrst.ncb_lsn      = 0xff;
    rc = (*pNetBiosFn)((char * _Seg16) &ncbrst);
    //rc = NETBIOS((char * _Seg16) &ncbrst);
    memcpy(&ncbrc3,&(ncbrst.ncb_retcode),sizeof(ncbrc3));
    }

  if (numargs >= 3)
    retstr->strlength = sprintf(retstr->strptr,"%x %x %x",ncbrc1,ncbrc2,ncbrc3);
  else
    retstr->strlength = sprintf(retstr->strptr,"%x",ncbrc2);
  return VALID_ROUTINE;
}

// Rexx RxCreateThread external function
// (to execute a procedure (by entrypoint) on another thread
EXPORTSPEC ULONG APIENTRY  RxCreateThread(PUCHAR name, ULONG numargs, RXSTRING args[],
                      PSZ queuename, PRXSTRING retstr)
{

  TID         ThreadID = 0;       // New thread ID (returned)
  PFNTHREAD   ThreadAddr = NULL;  // Program address
  ULONG       ThreadArg = 0;      // Parameter to thread routine
  ULONG       ThreadFlags = 0;    // Default is "immediate"
  ULONG       StackSize = 0x2000; // Size in bytes of new thread's stack
  APIRET      rc;                 // Return code
  CHAR        linktype = 'S';     // Default is "System" linkage

  // Arg#1 = procedure address
  // Arg#2 = optional, arg address (default = NULL)
  // Arg#3 = optional, linkage type: S = _System (default)
  //                                 O = _Optlink
  // Arg#4 = optional, stacksize (default = 8k)
  // Arg#5 = optional, threadflags: I = Immediate (default = Immediate)
  //                                S = Suspend

  // Gotta have at least one argument (procedure address)
  if (numargs < 1)
    return INVALID_ROUTINE;

  if (args[0].strlength != sizeof(PFNTHREAD))
    return INVALID_ROUTINE;
  memcpy(&ThreadAddr,args[0].strptr,args[0].strlength);

  if (numargs >= 2 )
    {
    if (args[1].strlength != sizeof(ThreadArg))
      return INVALID_ROUTINE;
    memcpy(&ThreadArg,args[1].strptr,args[1].strlength);
    }

  if (numargs >= 3 && args[2].strlength)
    {
    switch (*(args[2].strptr))
      {
      case 's':
      case 'S':
        linktype = 'S';
        break;
      case 'o':
      case 'O':
        linktype = 'O';
        break;
      default:
        return INVALID_ROUTINE;
        break;
      }
    }

  // Get StackSize, if supplied
  if (numargs >= 4 && args[3].strlength)
    {
    StackSize = atoi(args[3].strptr);
    if (!StackSize)
      return INVALID_ROUTINE;
    }

  // Get ThreadFlags, if supplied
  if (numargs >= 5 && args[4].strlength)
    {
    switch (*(args[4].strptr))
      {
      case 'i':
      case 'I':
        ThreadFlags = 0;
        break;
      case 's':
      case 'S':
        ThreadFlags = 1;
        break;
      default:
        return INVALID_ROUTINE;
        break;
      }
    }

  if (linktype == 'S')
    {
    rc = DosCreateThread(&ThreadID,ThreadAddr,ThreadArg,ThreadFlags,StackSize);
    retstr->strlength = sprintf(retstr->strptr,rc?"%ld":"0 %ld",rc);
    }
  else
    {
    ThreadID = _beginthread((void (*_Optlink)(void*))ThreadAddr
#if !defined(_MSVC_)
		,NULL
#endif
		,(unsigned)StackSize
		,(void*)ThreadArg
		);
    retstr->strlength = sprintf(retstr->strptr,ThreadID==-1?"%ld":"0 %ld",ThreadID);
    }

  return VALID_ROUTINE;

}

// Rexx RxSetPriority external function
EXPORTSPEC ULONG APIENTRY  RxSetPriority(PUCHAR name, ULONG numargs, RXSTRING args[],
                     PSZ queuename, PRXSTRING retstr)
{

  ULONG       ulScope = PRTYS_PROCESS;
  ULONG       ulPriorityClass = PRTYC_NOCHANGE;
  LONG        lPriorityDelta = 0;
  ULONG       ulId = 0;
  APIRET      rc;                 // Return code

  // Get Scope, if supplied
  if (numargs >= 1)
    {
    if (args[0].strlength > 0)
      {
      switch (*(args[0].strptr))
        {
        case 'p':
        case 'P':
          ulScope = PRTYS_PROCESS;
          break;
        case 'r':
        case 'R':
          ulScope = PRTYS_PROCESSTREE;
          break;
        case 't':
        case 'T':
          ulScope = PRTYS_THREAD;
          break;
        default:
          printf("Arg 1, if supplied, must be 'P'rocess, t'R'ee, 'T'hread\n");
          return INVALID_ROUTINE;
          break;
        }
      }
    }

  // Get Class, if supplied
  if (numargs >= 2)
    {
    if (args[1].strlength > 0)
      {
      switch (*(args[1].strptr))
        {
        case 'n':
        case 'N':
          ulPriorityClass = PRTYC_NOCHANGE;
          break;
        case 'i':
        case 'I':
          ulPriorityClass = PRTYC_IDLETIME;
          break;
        case 'r':
        case 'R':
          ulPriorityClass = PRTYC_REGULAR;
          break;
        case 't':
        case 'T':
          ulPriorityClass = PRTYC_TIMECRITICAL;
          break;
        case 's':
        case 'S':
          ulPriorityClass = PRTYC_FOREGROUNDSERVER;
          break;
        default:
          printf("Arg 2, if supplied, must be 'N'ochange, 'I'dletime\n");
          printf("       'R'egular, 'T'imecritical, or foreground'S'erver\n");
          return INVALID_ROUTINE;
          break;
        }
      }
    }

  // Get Priority-Delta, if supplied
  if (numargs >= 3)
    {
    if (args[2].strlength > 0)
      {
      if ( ! string2long(args[2].strlength,
                         args[2].strptr,
                        (LONG *) &(lPriorityDelta)))
        {
        switch ( *(args[2].strptr) )
          {
          case 'x':
          case 'X':
            lPriorityDelta = PRTYD_MAXIMUM;
            break;
          case 'n':
          case 'N':
            lPriorityDelta = PRTYD_MINIMUM;
            break;
          default:
            return INVALID_ROUTINE;
            break;
          } // end-switch
        } // end-if
      } // end-if
    } // end-if

  // Get Process-Id or Thread-Id, if supplied
  if (numargs >= 4)
    {
    if (args[3].strlength > 0)
      {
      if ( !string2long(args[3].strlength,args[3].strptr,(PLONG)&ulId) )
        return INVALID_ROUTINE;
      }
    }

  rc = DosSetPriority((ULONG) ulScope,
                      (ULONG) ulPriorityClass,
                      (LONG)  lPriorityDelta,
                      (ULONG) ulId);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);
  return VALID_ROUTINE;

}

// Rexx RxKillThread external function
EXPORTSPEC ULONG APIENTRY  RxKillThread(PUCHAR name, ULONG numargs, RXSTRING args[],
                    PSZ queuename, PRXSTRING retstr)
{

  TID         ThreadID = 0;       // New thread ID
  APIRET      rc;                 // Return code

  // Gotta have at least one argument (thread-id)
  if (numargs < 1)
    return INVALID_ROUTINE;
  if (args[0].strlength != sizeof(TID))
    return INVALID_ROUTINE;
  memcpy(&ThreadID,args[0].strptr,args[0].strlength);

  rc = DosKillThread(ThreadID);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);
  return VALID_ROUTINE;

}

// Rexx RxResumeThread external function
EXPORTSPEC ULONG APIENTRY  RxResumeThread(PUCHAR name, ULONG numargs, RXSTRING args[],
                      PSZ queuename, PRXSTRING retstr)
{

  TID         ThreadID = 0;       // New thread ID
  APIRET      rc;                 // Return code

  // Gotta have at least one argument (thread-id)
  if (numargs < 1)
    {
    printf("Arg 1 must be a valid Thread-Id\n");
    return INVALID_ROUTINE;
    }

  if (args[0].strlength != sizeof(TID))
    {
    printf("Arg 1 must be a valid Thread-Id\n");
    return INVALID_ROUTINE;
    }
  memcpy(&ThreadID,args[0].strptr,args[0].strlength);

  rc = DosResumeThread(ThreadID);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);
  return VALID_ROUTINE;

}

// Rexx RxSuspendThread external function
EXPORTSPEC ULONG APIENTRY  RxSuspendThread(PUCHAR name, ULONG numargs, RXSTRING args[],
                       PSZ queuename, PRXSTRING retstr)
{

  TID         ThreadID = 0;       // New thread ID
  APIRET      rc;                 // Return code

  // Gotta have at least one argument (thread-id)
  if (numargs < 1)
    {
    printf("Arg 1 must be a valid Thread-Id\n");
    return INVALID_ROUTINE;
    }

  if (args[0].strlength != sizeof(TID))
    {
    printf("Arg 1 must be a valid Thread-Id\n");
    return INVALID_ROUTINE;
    }
  memcpy(&ThreadID,args[0].strptr,args[0].strlength);

  rc = DosSuspendThread(ThreadID);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);
  return VALID_ROUTINE;

}

#endif  // BLD4OS2

// Rexx RxCreateRexxThread external function
// (to execute a rexx exec in another thread
EXPORTSPEC ULONG APIENTRY  RxCreateRexxThread(PUCHAR name, ULONG numargs, RXSTRING args[],
                          PSZ queuename, PRXSTRING retstr)
{

  ULONG       actr, argstructsize=0;
  TID         tid=0;
  ULONG       tflags=0;
  ULONG       stacksize=0xffff;
  PRTCARGS    pargs=(PRTCARGS)NULL;

  // Gotta have at least one argument (name of Rexx program)
  if (numargs < 1)
    return INVALID_ROUTINE;
  if (args[0].strlength == 0)
    return INVALID_ROUTINE;

  // Store copy of arguments on heap so we can pass them to 2ndary thread
  argstructsize = sizeof(RTCARGS) + ((numargs-1)*(sizeof(RXSTRING)));
  pargs = (PRTCARGS) malloc(argstructsize);
  memset(pargs,0x00,argstructsize);
  pargs->numargs = numargs;
  for (actr=0;actr < numargs;actr++)
    {
    pargs->args[actr].strptr = (PCH) malloc(args[actr].strlength+1);
    pargs->args[actr].strlength = args[actr].strlength;
    memset(pargs->args[actr].strptr,0x00,args[actr].strlength+1);
    memcpy(pargs->args[actr].strptr,args[actr].strptr,args[actr].strlength);
    }

  // Start 2ndary thread
  tid = _beginthread(RexxFunctionCall
#if !defined(_MSVC_)
	  ,(void *)   NULL
#endif  // BLD4OS2
	  ,(unsigned) stacksize
	  ,(void *)   pargs
	  );

  if (tid == -1)
    return INVALID_ROUTINE;

  retstr->strlength = sprintf(retstr->strptr,"%ld",tid);

  return VALID_ROUTINE;

}

#if defined(__BLD4OS2__)

// Rexx RxSetExceptionExit external function
EXPORTSPEC ULONG APIENTRY  RxSetExceptionExit(PUCHAR name, ULONG numargs, RXSTRING args[],
                          PSZ queuename, PRXSTRING retstr)
{

  // Arg#1 = address (32-bit binary value) of exception-handler exit (per process)

  if (numargs < 1)
    return INVALID_ROUTINE;
  if (args[0].strlength != sizeof(pExceptionExit))
    return INVALID_ROUTINE;
  memcpy(&pExceptionExit,args[0].strptr,sizeof(pExceptionExit));

  retstr->strlength = 0;

  return VALID_ROUTINE;

}

// 'C' function to set exception exit address
void CdeclSetExceptionExit(PPVOID pexit)
{

  pExceptionExit = *((ERR*)pexit);

  return;

}

#endif  // BLD4OS2

// Function which executes rexx exec in other thread
void _Optlink RexxFunctionCall(void * voidptr)
{

  USHORT    actr;
  long      apirc;
  RXSTRING  rexxretval;                // return value from REXX
  SHORT     rexxrc = 0;                // return code from function
  RXSTRING  instore[2];
  PRXSTRING pinstore=(PRXSTRING)NULL;
  PRXSTRING prxargs=(PRXSTRING)NULL;
  char     *execname;
  BOOL      bTokenized = FALSE;
  PRTCARGS  pargs=(PRTCARGS)voidptr;

  MAKERXSTRING(rexxretval,NULL,0);
  memset(&instore[0],0x00,sizeof(instore));

  switch (*pargs->args[0].strptr)
    {
    case '$':    // Explicit In-Storage source string
      execname = "StringInStore";
      MAKERXSTRING(instore[0],pargs->args[0].strptr+1,pargs->args[0].strlength-1);
      pinstore = &instore[0];
      break;
    case '&':    // Explicit In-Storage tokenized string
      execname = "TokenizedInStore";
      MAKERXSTRING(instore[1],pargs->args[0].strptr+1,pargs->args[0].strlength-1);
      pinstore = &instore[0];
      bTokenized = TRUE;
      break;
    case '+':      // Explicitly a macrospace rexx program
      MAKERXSTRING(instore[0],NULL,0);
      MAKERXSTRING(instore[1],NULL,0);
      pinstore = &instore[0];
      execname = pargs->args[0].strptr+1;
      break;
    default:     // Explicit on-disk (or macrospace) program name
      execname = pargs->args[0].strptr;
      break;
    }

  if (pargs->numargs >= 2)
    prxargs = &pargs->args[1];

  apirc=RexxStart((LONG)       pargs->numargs-1,
                  (PRXSTRING)  prxargs,              // arguments
                  (PSZ)        execname,             // name of REXX file
                  (PRXSTRING)  pinstore,             // No INSTORE used
                  (PSZ)        NULL,                 // Command env. name
                  (LONG)       RXCOMMAND,            // Code for how invoked
                  (PRXSYSEXIT) 0,                    // No EXITs on this call
                  (PSHORT)     &rexxrc,              // Rexx program output
                  (PRXSTRING)  &rexxretval );        // Rexx program output

#if defined(__BLD4OS2__)
  DosFreeMem(rexxretval.strptr);
#endif  // BLD4OS2
#if defined(__BLD4NT__)
  GlobalFree(GlobalHandle(rexxretval.strptr));
#endif  // BLD4NT

  if (!bTokenized && instore[1].strptr)
#if defined(__BLD4OS2__)
    DosFreeMem(instore[1].strptr);
#endif  // BLD4OS2
#if defined(__BLD4NT__)
    GlobalFree(GlobalHandle(instore[1].strptr));
#endif  // BLD4NT

  for (actr=0;actr < pargs->numargs;actr++)
    free(pargs->args[actr].strptr);
  free(pargs);

  _endthread();

  return;

}

// Function to pass variables by name to rexx program on 2ndary thread
void passByNameToThread(void *pdata)
{
  PPBNTTARGS pbnargs=(PPBNTTARGS)pdata;
  long       apirc=0;
  ULONG      ctr;

  apirc = passByNameInvokeInterpreter((ULONG)pbnargs->ptargs->numargs
                                     ,(PRXSTRING)pbnargs->ptargs->args
                                     ,(PRXSTRING)NULL    // No return string provided
                                     ,(PINITEXITHANDLERARGS)pbnargs->pehargs
                                     ,TRUE);  // Running on 2ndary thread

  // Free memory used to pass argument copies to this thread
  for (ctr=0;ctr < pbnargs->ptargs->numargs;ctr++)
    free(pbnargs->ptargs->args[ctr].strptr);
  free(pbnargs->ptargs);
  free(pbnargs);

  return;
}

// Call a function/subroutine, passing variables by name (by reference or value)
long  passByNameInvokeInterpreter(ULONG numargs, RXSTRING args[], PRXSTRING retstr,
                                  PINITEXITHANDLERARGS pehargs, BOOL on2ndThread)
{
  RXSTRING             rexxretval;
  LONG                 apirc;
  SHORT                rexxrc=0;
  PRXSYSEXIT           psysexit=NULL;
  RXSTRING             instore[2], *pinstore=instore;
  PCH                  pexecname="InStore";
  PRXSTRING            pargs=NULL;
  PVARNAMELIST         ptmpn, ptmpp, phold;
  PSHVBLOCK            prevshvb=NULL, nshvb;
  ULONG                passedargs=0;
  PINITEXITHANDLERARGS pehatail=NULL;
  PCH                  prtn;
  ULONG                lrtn;

  // ***************  FOR LINE ECHO DEBUGGING  ***************
  //int qprintf(PCH qname, const unsigned char *f,...);
  // qprintf(NULL,"sprintf-like-string",argn,...);
  // ***************  FOR LINE ECHO DEBUGGING  ***************

  //_dump_allocated(0);

  prtn = args[0].strptr;
  lrtn = args[0].strlength;
  if (on2ndThread)
    {
    prtn++;
    lrtn--;
    }

  // Locate or create process/thread specific exit anchor
  if (!getEhargsTail(&pehatail,&psysexit,NULL))
    return INVALID_ROUTINE;

  // Add eharg element to chain
  pehatail->pnextarg = pehargs;

  if (numargs >= 2 && args[1].strlength)
    {
    if (numargs >= 3)
      pargs = &args[2];
    else
      pargs = NULL;
    passedargs = numargs - 2;
    } // end-if variable list was supplied

  // Determine whether we're calling another rexx program by file-name, or as an
  // in-storage program
  switch (*prtn)
    {
    case '$':      // Explicitly an in-storage source string
      MAKERXSTRING(instore[0],prtn+1,lrtn-1);
      MAKERXSTRING(instore[1],NULL,0);
      break;
    case '&':      // Explicitly an in-storage tokenized string
      MAKERXSTRING(instore[0],NULL,0);
      MAKERXSTRING(instore[1],prtn+1,lrtn-1);
      break;
    case '+':      // Explicitly a macrospace rexx program
      MAKERXSTRING(instore[0],NULL,0);
      MAKERXSTRING(instore[1],NULL,0);
      pexecname = prtn+1;
      break;
    default:       // Implicitly an on-disk rexx program
      pexecname = prtn;
      pinstore = NULL;
      break;
    }

  MAKERXSTRING(rexxretval,NULL,0);

  apirc=RexxStart((LONG)       passedargs            // number of arguments
                 ,(PRXSTRING)  pargs                 // arguments
                 ,(PSZ)        pexecname             // name of "program" (if not in-store)
                 ,(PRXSTRING)  pinstore              // Pointer to in-storage code (if not on-disk)
                 ,(PSZ)        NULL                  // Command env. name
                 ,(LONG)       RXSUBROUTINE          // Code for how invoked
                 ,(PRXSYSEXIT) psysexit              // System exit list
                 ,(PSHORT)     &rexxrc               // Rexx program output
                 ,(PRXSTRING)  &rexxretval           // Rexx program output
                 );

  // If any "by reference" values were passed BACK from called rexx program, then put them in variable pool
  // (but only if we're NOT running on 2ndary thread, because then we'd have no use for "returned" values)
  if (!on2ndThread)
    {
    if (pehargs->pbase && apirc==0)
      RexxVariablePool(pehargs->pbase);
    } // end-if NOT on 2ndary thread

  // Free storage used to hold returned variables
  for (nshvb=pehargs->pbase;nshvb;prevshvb=nshvb,nshvb=nshvb->shvnext,free(prevshvb))
    {
    free(nshvb->shvvalue.strptr);
    free(nshvb->shvname.strptr);
    }
  // Free memory used for variable and stem name lists
  for (ptmpp=pehargs->pprefs;ptmpp;phold=ptmpp,ptmpp=ptmpp->pnextvar,free(phold))
    {
    free(ptmpp->pvar);
    }
  for (ptmpn=pehargs->pvars;ptmpn;phold=ptmpn,ptmpn=ptmpn->pnextvar,free(phold))
    {
    free(ptmpn->pvar);
    }

  // Free exit handler argument area
  free(pehargs);

  // Remove our ehargs element from tail of chain
  if (pehatail)
    pehatail->pnextarg = NULL;

  //_dump_allocated(0);

  if (on2ndThread)
    {
#if defined(__BLD4OS2__)
    DosFreeMem(rexxretval.strptr);
#endif  // BLD4OS2
#if defined(__BLD4NT__)
    GlobalFree(GlobalHandle(rexxretval.strptr));
#endif  // BLD4NT
    if (apirc < 0)
      return INVALID_ROUTINE;
    else
      return VALID_ROUTINE;
    } // end-if we're running on a 2ndary thread
  else
    {
    if (apirc < 0)
      return INVALID_ROUTINE;
    else
      {
      // Return result that was returned to us
      if (retstr)
        {
#if defined(__BLD4OS2__)
        DosFreeMem(retstr->strptr);
#endif  // BLD4OS2
#if defined(__BLD4NT__)
        GlobalFree(GlobalHandle(retstr->strptr));
#endif  // BLD4NT
        retstr->strptr = rexxretval.strptr;
        retstr->strlength = rexxretval.strlength;
        }
      return VALID_ROUTINE;
      } // end-else apirc was OK
    } // end-else we're NOT running on a 2ndary thread

}

// Call a function/subroutine, passing variables by name (by reference or value)
EXPORTSPEC ULONG APIENTRY  RxPassByName(PUCHAR name, ULONG numargs, RXSTRING args[],
                    PSZ queuename, PRXSTRING retstr)
{
  LONG                 apirc;
  PINITEXITHANDLERARGS pehargs=NULL;
  ULONG                ctr;
  ULONG                argstructsize;
  PPBNTTARGS           pbnargs=NULL;

  if (numargs < 1 || !args[0].strlength)
    return INVALID_ROUTINE;

  // Create a new eharg block
  pehargs = (PINITEXITHANDLERARGS) malloc(sizeof(INITEXITHANDLERARGS));
  memset(pehargs,0x00,sizeof(INITEXITHANDLERARGS));
  // Get variables and their values (if any) from calling rexx program
  if (numargs >= 2 && args[1].strlength)
    {
    // We got routine name, variable list (and possibly other arguments)
    addToListOfPassedVariables(pehargs,args[1].strptr,FALSE);
    extractVariableSubPool(pehargs,FALSE);
    } // end-if variable list was supplied

  // Either call rexx program on 2ndary thread, or on this thread
  if (*args[0].strptr == '~') // The tilda '~' indicates call on 2ndary thread
    {
    TID  tid=0;
    pbnargs = (PPBNTTARGS) malloc(sizeof(PBNTTARGS));
    // Store copy of arguments on heap so we can pass them to 2ndary thread
    argstructsize = sizeof(RTCARGS) + ((numargs-1)*(sizeof(RXSTRING)));
    pbnargs->ptargs = (PRTCARGS) malloc(argstructsize);
    memset(pbnargs->ptargs,0x00,argstructsize);
    pbnargs->ptargs->numargs = numargs;
    for (ctr=0;ctr<numargs;ctr++)
      {
      pbnargs->ptargs->args[ctr].strptr = (char*) malloc(args[ctr].strlength+1);
      pbnargs->ptargs->args[ctr].strlength = args[ctr].strlength;
      *(pbnargs->ptargs->args[ctr].strptr + args[ctr].strlength) = 0x00;
      memcpy(pbnargs->ptargs->args[ctr].strptr,args[ctr].strptr,args[ctr].strlength);
      } // end-for each argument
    pbnargs->pehargs = pehargs;
    tid = _beginthread(passByNameToThread
#if !defined(_MSVC_)
		,NULL
#endif
		,0xffff
		,(void*)pbnargs
		);
    if (tid == -1)
      return INVALID_ROUTINE;
    retstr->strlength = sprintf(retstr->strptr,"%ld",tid);
    } // end-if it's a call to rexx program on 2ndary thread
  else
    {
    apirc = passByNameInvokeInterpreter(numargs,args,retstr,pehargs,FALSE);
    if (apirc < 0)
      return INVALID_ROUTINE;
    } // end-if
  return VALID_ROUTINE;

}

// Return variables by name (by reference or value) to calling routine (if applicable)
EXPORTSPEC ULONG APIENTRY  RxReturnByName(PUCHAR name, ULONG numargs, RXSTRING args[],
                      PSZ queuename, PRXSTRING retstr)

{

  ULONG                numehargs;
  PINITEXITHANDLERARGS pehargs=NULL;

  if (!numargs)
    return INVALID_ROUTINE;

  *retstr->strptr = '1';
  retstr->strlength = 1;

  // Locate or create process/thread specific exit anchor
  if (!getEhargsTail(&pehargs,NULL,&numehargs))
    return INVALID_ROUTINE;

  // All that we can determine is that we were called by a rexx routine that was invoked at
  // least SOMEPLACE further back in the call chain via "RxPassByName()".  It is up to the
  // user to know if this "RxReturnByName()" will skip over called routines (i.e. ones that
  // themselves were not DIRECTLY invoked via "RxPassByName()").
  if (!numehargs)
    {
    *retstr->strptr = '0';
    return VALID_ROUTINE;
    }

  if (args[0].strlength)
    addToListOfPassedVariables(pehargs,args[0].strptr,TRUE);

  return VALID_ROUTINE;

}

// Function/Subroutine Initialization/Termination Exit Handler
LONG APIENTRY passByNameInitTermExitHandler(LONG exitnumber, LONG subfunction, PEXIT pParmBlock)
{

  PSHVBLOCK            pnext, pprev;
  PINITEXITHANDLERARGS pehargs=NULL;

  // Locate or create process/thread specific exit anchor
  if (!getEhargsTail(&pehargs,NULL,NULL))
    return RXEXIT_RAISE_ERROR;

  // Determine whether we're being called for RXINI or RXTER
  switch (exitnumber)
    {
    case RXINI:
      // If any values were passed INTO this rexx program, then put them in variable pool
      if (pehargs->pbase)
        RexxVariablePool(pehargs->pbase);
      // Free storage used to hold inbound variables
      for (pnext=pehargs->pbase;pnext;pprev=pnext,pnext=pnext->shvnext,free(pprev))
        {
        free(pnext->shvvalue.strptr);
        free(pnext->shvname.strptr);
        }
      // Set variable value list to NULL
      pehargs->pbase = NULL;
      break;
    case RXTER:
      extractVariableSubPool(pehargs,TRUE);
      break;
    default:
      return RXEXIT_RAISE_ERROR;
      break;
    } // end-switch on exitnumber

  // Tell Rexx interpreter to continue on with function/subroutine call
  return RXEXIT_HANDLED;

}

// Function to find tail of ehargs chain
BOOL getEhargsTail(PINITEXITHANDLERARGS * ppehatail,PRXSYSEXIT * ppsysexit, ULONG * pnumehargs)
{
  APIRET               rc;
  char                 exitname[60];
  PVOID                pnada[2];
#if defined(__BLD4OS2__)
  PPIB                 ppib;
  PTIB                 ptib;
#endif  // BLD4OS2
  USHORT               queryflag=0;
  ULONG                ctr;
  PINITEXITHANDLERARGS pehargs=NULL, pehatail=NULL;
  PRXSYSEXIT           psysexit=NULL;

  if (pnumehargs)
    *pnumehargs = 0;

  // Build exit-name based on process-id and thread-id in which we're running
#if defined(__BLD4OS2__)
  DosGetInfoBlocks(&ptib,&ppib);
  sprintf(exitname,"passByNameExit_%ld_%ld",ppib->pib_ulpid,ptib->tib_ptib2->tib2_ultid);
#endif  // BLD4OS2
#if defined(__BLD4NT__)
  sprintf(exitname,"passByNameExit_%ld_%ld",GetCurrentProcessId(),GetCurrentThreadId());
#endif  // BLD4NT

  rc=RexxQueryExit(exitname,NULL,&queryflag,(PUCHAR)pnada);
  switch (rc)
    {
    case RXEXIT_OK:
      // Find end of ehargs chain
      for (pehatail=(PINITEXITHANDLERARGS)pnada[0],ctr=0;
           pehatail->pnextarg;
           pehatail=pehatail->pnextarg,ctr++);
      if (pnumehargs)
        *pnumehargs = ctr;
      if (ppsysexit)
        *ppsysexit = (PRXSYSEXIT) pnada[1];
      *ppehatail = pehatail;
      break;
    case RXEXIT_NOTREG:
      // Exit hadn't been registered, so create anchor of "pehargs" chain and exitlist array,
      // then register exit
      pehargs = (PINITEXITHANDLERARGS) malloc(sizeof(INITEXITHANDLERARGS));
      memset(pehargs,0x00,sizeof(INITEXITHANDLERARGS));
      pnada[0] = pehargs;
      psysexit = (PRXSYSEXIT) malloc(3*sizeof(RXSYSEXIT));
      psysexit[0].sysexit_code = RXINI;
      psysexit[0].sysexit_name = (char*) malloc(strlen(exitname)+1);
      strcpy(psysexit[0].sysexit_name,exitname);
      psysexit[1].sysexit_code = RXTER;
      psysexit[1].sysexit_name = psysexit[0].sysexit_name;
      psysexit[2].sysexit_code = RXENDLST;
      psysexit[2].sysexit_name = NULL;
      pnada[1] = psysexit;
      rc=RexxRegisterExitExe(exitname,(PFN)passByNameInitTermExitHandler,(PUCHAR)pnada);
      if (ppsysexit)
        *ppsysexit = psysexit;
      *ppehatail = pehargs;
      break;
    default:
      printf("\n\tRexxQueryExit() rc = %ld\n",rc);
      return FALSE;
      break;
    }

  return TRUE;

}

// Function to accept a list of variables and add them to our internal list
void addToListOfPassedVariables(PINITEXITHANDLERARGS pehargs,PCH pvarlist,BOOL alwaysbyref)
{
  PCH           cp;
  char          tc;
  PVARNAMELIST  ptmpn=pehargs->pvars, ptmpp=pehargs->pprefs, phold;
  BOOL          refval;

  // Find end of variable and prefix chains
  for (phold=pehargs->pvars;phold;ptmpn=phold,phold=phold->pnextvar);
  for (phold=pehargs->pprefs;phold;ptmpp=phold,phold=phold->pnextvar);

  strupr(pvarlist);
  // Convert commas to spaces (just in case they were supplied)
  for (cp=pvarlist;*cp;cp++)
    {
    if (*cp == ',')
      *cp = ' ';
    } // end-for each character in variable list
  // Split variable names into whole and "prefix" (i.e. stem) variables
  for (cp=strtok(pvarlist," ");cp;cp=strtok(NULL," "))
    {
    // Make note of (and adjust for) "by reference" variables
    if (*cp == '&' || alwaysbyref)
      {
      refval = TRUE;
      if (*cp == '&')
        cp++;
      }
    else
      refval = FALSE;
    tc = *(cp+strlen(cp)-1);
    if (tc == '.')
      {
      if (!pehargs->pprefs)
        {
        ptmpp = pehargs->pprefs = (PVARNAMELIST) malloc(sizeof(VARNAMELIST));
        }
      else
        {
        ptmpp->pnextvar = (PVARNAMELIST) malloc(sizeof(VARNAMELIST));
        ptmpp = ptmpp->pnextvar;
        }
      ptmpp->pvar = (char*) malloc(strlen(cp)+1);
      strcpy(ptmpp->pvar,cp);
      ptmpp->pnextvar = NULL;
      ptmpp->byref = refval;
      pehargs->someprefbyref |= refval;
      }
    else
      {
      if (!pehargs->pvars)
        {
        ptmpn = pehargs->pvars = (PVARNAMELIST) malloc(sizeof(VARNAMELIST));
        }
      else
        {
        ptmpn->pnextvar = (PVARNAMELIST) malloc(sizeof(VARNAMELIST));
        ptmpn = ptmpn->pnextvar;
        }
      ptmpn->pvar = (char*) malloc(strlen(cp)+1);
      strcpy(ptmpn->pvar,cp);
      ptmpn->pnextvar = NULL;
      ptmpn->byref = refval;
      pehargs->somevarbyref |= refval;
      }
    } // end-for each token in list

  return;

}

void extractVariableSubPool(PINITEXITHANDLERARGS pehargs,BOOL breturning)
{
  SHVBLOCK      shvb;
  PVARNAMELIST  ptmpn, ptmpp, phold, ptmpt;
  PSHVBLOCK     prevshvb=pehargs->pbase, nshvb, ptshvb;
  BOOL          matchedVar;
  ULONG         stemlen;
  ULONG         workbufsize=ulPassByNameBufSize;  // Get value as "quickly" as possible, to avoid
                                                  // multithreading collisions.  This allows us
                                                  // to use this value at different places in this
                                                  // function without possibility of this global
                                                  // variable having different values.
  PVARNAMELIST  ptryagain=NULL;
  char         *pbuf=NULL;

  // Find end of variable values chain
  for (ptshvb=pehargs->pbase;ptshvb;prevshvb=ptshvb,ptshvb=ptshvb->shvnext);

  // Generate SHVBLOCK to drop each stem first
  if (breturning && pehargs->someprefbyref)
    {
    for (ptmpp=pehargs->pprefs;ptmpp;ptmpp=ptmpp->pnextvar)
      {
      // Only do this for "by reference" variables
      if (!ptmpp->byref)
        continue;
      stemlen = strlen(ptmpp->pvar);
      nshvb = (PSHVBLOCK) malloc(sizeof(SHVBLOCK));
      // If this is first item in linked-list, then begin list
      if (!pehargs->pbase)
        pehargs->pbase = nshvb;
      else
        prevshvb->shvnext = nshvb;
      memset(nshvb,0x00,sizeof(SHVBLOCK));
      nshvb->shvcode = RXSHV_DROPV;
      nshvb->shvname.strptr = (char*) malloc(stemlen);
      nshvb->shvname.strlength = stemlen;
      memcpy(nshvb->shvname.strptr,ptmpp->pvar,stemlen);
      prevshvb = nshvb;
      } // end-for each stem variable
    } // end-if we are on return trip and at least some variables are "by reference"

  // Spin through variable pool, picking out variables that match stem(s) and
  // adding their values to linked-list of variables' values
  if ( (breturning && pehargs->someprefbyref) || !breturning )
    {
    if (!pbuf)
      pbuf = (char*) malloc(workbufsize);  // allocate work area to hold *most* fetched values
    memset(&shvb,0x00,sizeof(shvb));
    shvb.shvcode = RXSHV_NEXTV;
    shvb.shvname.strptr = (char*) malloc(_REXX_VARIABLE_NAME_MAX_LEN_+1);
    shvb.shvvalue.strptr = pbuf;
    while (!(shvb.shvret & RXSHV_LVAR))
      {
      memset(shvb.shvname.strptr,0x00,_REXX_VARIABLE_NAME_MAX_LEN_+1);
      shvb.shvname.strlength = _REXX_VARIABLE_NAME_MAX_LEN_;
      shvb.shvvalue.strlength = workbufsize;
      shvb.shvnamelen = 0;
      shvb.shvvaluelen = 0;
      RexxVariablePool(&shvb);
      matchedVar = FALSE;
      for (ptmpp=pehargs->pprefs;ptmpp;ptmpp=ptmpp->pnextvar)
        {
        // Only do this for "by reference" variables (only care on return trip)
        if (breturning && !ptmpp->byref)
          continue;
        stemlen = strlen(ptmpp->pvar);
        if (shvb.shvnamelen >= stemlen && !strncmp(shvb.shvname.strptr,ptmpp->pvar,stemlen))
          {
          // If our buffer wasn't big enough, add this variable's name and size to a list of variables
          // we have to come back and get later (i.e. after the RXSHV_NEXTV loop)
          if (shvb.shvret & RXSHV_TRUNC)
            {
            if (!ptryagain)
              {
              ptmpt = ptryagain = (PVARNAMELIST) malloc(sizeof(VARNAMELIST));
              }
            else
              {
              ptmpt->pnextvar = (PVARNAMELIST) malloc(sizeof(VARNAMELIST));
              ptmpt = ptmpt->pnextvar;
              }
            ptmpt->pvar = (char*) malloc(shvb.shvnamelen+1);
            memcpy(ptmpt->pvar,shvb.shvname.strptr,shvb.shvnamelen);
            *(ptmpt->pvar+shvb.shvnamelen) = 0x00;
            ptmpt->pnextvar = NULL;
            ptmpt->byref = shvb.shvvaluelen; // save actual length of value
            } // end-if buffer was too small
          else
            {
            nshvb = (PSHVBLOCK) malloc(sizeof(SHVBLOCK));
            // If this is first item in linked-list, then begin list
            if (!pehargs->pbase)
              pehargs->pbase = nshvb;
            else
              prevshvb->shvnext = nshvb;
            memset(nshvb,0x00,sizeof(SHVBLOCK));
            nshvb->shvname.strptr = (char*) malloc(shvb.shvnamelen);
            nshvb->shvname.strlength = shvb.shvnamelen;
            memcpy(nshvb->shvname.strptr,shvb.shvname.strptr,shvb.shvnamelen);
            nshvb->shvvalue.strptr = (char*) malloc(max(shvb.shvvaluelen,1));
            memcpy(nshvb->shvvalue.strptr,shvb.shvvalue.strptr,shvb.shvvaluelen);
            nshvb->shvvalue.strlength = shvb.shvvaluelen;
            nshvb->shvcode = RXSHV_SET;  // Change opcode to "SET" for use by exit handler
            matchedVar = TRUE;
            prevshvb = nshvb;
            break; // break out of this inner "for" loop
            } // end-else buffer wasn't too small
          } // end-if variable matches prefix
        } // end-for each prefix
      if (matchedVar)
        continue;
      } // end-while not end of RXSHV_NEXTV list
    free(shvb.shvname.strptr);
    // Spin through and add "try again" variables to list (if any)
    for (ptmpt=ptryagain;ptmpt;ptmpt=ptmpt->pnextvar)
      {
      nshvb = (PSHVBLOCK) malloc(sizeof(SHVBLOCK));
      // If this is first item in linked-list, then begin list
      if (!pehargs->pbase)
        pehargs->pbase = nshvb;
      else
        prevshvb->shvnext = nshvb;
      memset(nshvb,0x00,sizeof(SHVBLOCK));
      nshvb->shvcode = RXSHV_FETCH;
      nshvb->shvname.strlength = strlen(ptmpt->pvar);
      nshvb->shvname.strptr = ptmpt->pvar;
      // Stuff fetched value (as much as will fit) in "pbuf"
      nshvb->shvvalue.strptr = (char*) malloc(ptmpt->byref);
      nshvb->shvvalue.strlength = ptmpt->byref;
      RexxVariablePool(nshvb);
      nshvb->shvcode = RXSHV_SET;  // Change opcode to "SET" for use by exit handler
      prevshvb = nshvb;
      }
    for (ptmpt=ptryagain;ptmpt;phold=ptmpt,ptmpt=ptmpt->pnextvar,free(phold));
    } // end-if at least some stems are "by reference"

  // Add value of each whole variable to linked-list of variables' values
  if ( (breturning && pehargs->somevarbyref) || !breturning )
    {
    if (!pbuf)
      pbuf = (char*) malloc(workbufsize);  // allocate work area to hold *most* fetched values
    for (ptmpn=pehargs->pvars;ptmpn;ptmpn=ptmpn->pnextvar)
      {
      // Only do this for "by reference" variables (only care on return trip)
      if (breturning && !ptmpn->byref)
        continue;
      nshvb = (PSHVBLOCK) malloc(sizeof(SHVBLOCK));
      // If this is first item in linked-list, then begin list
      if (!pehargs->pbase)
        pehargs->pbase = nshvb;
      else
        prevshvb->shvnext = nshvb;
      memset(nshvb,0x00,sizeof(SHVBLOCK));
      nshvb->shvcode = RXSHV_FETCH;
      nshvb->shvname.strlength = strlen(ptmpn->pvar);
      nshvb->shvname.strptr = (char*) malloc(nshvb->shvname.strlength);
      memcpy(nshvb->shvname.strptr,ptmpn->pvar,nshvb->shvname.strlength);
      // Stuff fetched value (as much as will fit) in "pbuf"
      nshvb->shvvalue.strptr = pbuf;
      nshvb->shvvalue.strlength = workbufsize;
      RexxVariablePool(nshvb);
      if (nshvb->shvret & RXSHV_NEWV)
        {
        // Variable didn't exist "here," so drop it "there"
        nshvb->shvcode = RXSHV_DROPV;
        nshvb->shvvalue.strptr = NULL;
        }
      else
        {
        // We got a value, so allocate storage for transfer of value
        nshvb->shvvalue.strptr = (char*) malloc(nshvb->shvvaluelen);
        nshvb->shvvalue.strlength = nshvb->shvvaluelen;
        // If our buffer wasn't big enough, allocate new buffer of correct size
        if (nshvb->shvret & RXSHV_TRUNC)
          RexxVariablePool(nshvb);
        else
          memcpy(nshvb->shvvalue.strptr,pbuf,nshvb->shvvalue.strlength);
        nshvb->shvcode = RXSHV_SET;  // Change opcode to "SET" for use by exit handler
        }
      prevshvb = nshvb;
      } // end-for each variable in list
    } // end-if at least some variables are by reference

  // Free temp work buffer (if it was allocated)
  if (pbuf)
    free(pbuf);

  return;

}

// Set/query buffer size used by "RxPassByName()" for fetching
EXPORTSPEC ULONG APIENTRY  RxPBNBufSize(PUCHAR name, ULONG numargs, RXSTRING args[],
                    PSZ queuename, PRXSTRING retstr)

{

  ULONG  temp=ulPassByNameBufSize;  // Get value as "quickly" as possible, to avoid
                                    // multithreading collisions

  // Get current value for return
  retstr->strlength = sprintf(retstr->strptr,"%ld",temp);

  if (numargs && args[0].strlength)
    {
    // Set global buffer size to supplied value
    temp = atoi(args[0].strptr);
    if (!temp)
      return INVALID_ROUTINE;
    ulPassByNameBufSize = temp;
    }

  return VALID_ROUTINE;

}

// Function which calls the contents of a passed string as though
// it were an actual Rexx program on disk
EXPORTSPEC ULONG APIENTRY  RxCallInStore(PUCHAR name, ULONG numargs, RXSTRING args[],
                     PSZ queuename, PRXSTRING retstr)

{

  RXSTRING  rexxretval;                // return value from REXX
  long      apirc;
  SHORT     rexxrc = 0;                // return code from function
  RXSTRING  instore[2];
  PRXSTRING pargs;
  UCHAR     tempname[10] = "InStore";
  BOOL      bTokenized = FALSE;

  if (numargs < 1)
    return INVALID_ROUTINE;

  if ( args[0].strlength > 1 )
    {
    switch ( *(args[0].strptr) )
      {
      case '$':      // Explicitly an in-storage source string
        MAKERXSTRING(instore[0],args[0].strptr+1,args[0].strlength-1);
        MAKERXSTRING(instore[1],NULL,0);
        break;
      case '&':      // Explicitly an in-storage tokenized string
        MAKERXSTRING(instore[0],NULL,0);
        MAKERXSTRING(instore[1],(args[0].strptr)+1,args[0].strlength-1);
        break;
      default:       // Implicitly an in-storage source string
        MAKERXSTRING(instore[0],args[0].strptr,args[0].strlength);
        MAKERXSTRING(instore[1],NULL,0);
        break;
      }
    }
  else
    {
    MAKERXSTRING(instore[0],args[0].strptr,args[0].strlength);
    MAKERXSTRING(instore[1],NULL,0);
    }

  MAKERXSTRING(rexxretval,NULL,0);

  if (numargs >= 2)
    pargs = &(args[1]);
  else
    pargs = NULL;

  apirc=RexxStart((LONG)       numargs - 1,          // number of arguments
                  (PRXSTRING)  pargs,                // arguments
                  (PSZ)        tempname,             // name of "program"
                  (PRXSTRING)  &(instore[0]),        // Pointer to Code
                  (PSZ)        NULL,                 // Command env. name
                  (LONG)       RXSUBROUTINE,         // Code for how invoked
                  (PRXSYSEXIT) 0,                    // No EXITs on this call
                  (PSHORT)     &rexxrc,              // Rexx program output
                  (PRXSTRING)  &rexxretval );        // Rexx program output

  if (apirc < 0)
    return INVALID_ROUTINE;

  // If in-store string was not a tokenized string, then free it
  if ( !bTokenized )
#if defined(__BLD4OS2__)
    DosFreeMem(instore[1].strptr);
#endif  // BLD4OS2
#if defined(__BLD4NT__)
    GlobalFree(GlobalHandle(instore[1].strptr));
#endif  // BLD4NT

#if defined(__BLD4OS2__)
  DosFreeMem(retstr->strptr);
#endif  // BLD4OS2
#if defined(__BLD4NT__)
    GlobalFree(GlobalHandle(retstr->strptr));
#endif  // BLD4NT
  retstr->strptr = rexxretval.strptr;
  retstr->strlength = rexxretval.strlength;

  return VALID_ROUTINE;;
}

#if defined(__BLD4OS2__)

// Function which tokenizes a passed string (a complete Rexx program)
// and returns the actual tokenized image
EXPORTSPEC ULONG APIENTRY  RxTokenize(PUCHAR name, ULONG numargs, RXSTRING args[],
                  PSZ queuename, PRXSTRING retstr)

{

  RXSTRING  rexxretval;                // return value from REXX
  long      apirc;
  APIRET    rc;                        // return code from REXX
  SHORT     rexxrc = 0;                // return code from function
  RXSTRING  instore[2];
  RXSTRING  targs;
  UCHAR     tempname[10] = "InStore";
  UCHAR     tokenize[] = "//T";
  SHVBLOCK  shvb;

  // Arg#1 - Rexx source to be tokenized
  // Arg#2 - name of rexx variable in which tokenized result is returned

  if (numargs < 1)
    return INVALID_ROUTINE;

  MAKERXSTRING(instore[0],args[0].strptr,args[0].strlength);
  MAKERXSTRING(instore[1],NULL,0);
  MAKERXSTRING(targs,tokenize,sizeof(tokenize));
  MAKERXSTRING(rexxretval, NULL, 0);  // null out RXSTRING

  apirc=RexxStart((LONG)       1,                    // just 1 argument
                  (PRXSTRING)  &targs,               // "tokenize" arg
                  (PSZ)        tempname,             // name of program
                  (PRXSTRING)  &(instore[0]),        // Pointer to Code
                  (PSZ)        NULL,                 // Command env. name
                  (LONG)       RXCOMMAND,            // Code for how invoked
                  (PRXSYSEXIT) 0,                    // No EXITs on this call
                  (PSHORT)     &rexxrc,              // Rexx program output
                  (PRXSTRING)  &rexxretval );        // Rexx program output

  if (numargs >= 2)
    {
    if (!args[1].strlength)
      return INVALID_ROUTINE;
    strupr(args[1].strptr);
    memset(&shvb,0x00,sizeof(shvb));
    shvb.shvcode = RXSHV_SET;
    shvb.shvname.strptr = args[1].strptr;
    shvb.shvname.strlength = args[1].strlength;
    shvb.shvvalue.strptr = instore[1].strptr;
    shvb.shvvalue.strlength = instore[1].strlength;
    RexxVariablePool(&shvb);
    retstr->strlength = sprintf(retstr->strptr,"%ld %d",rc,rexxrc);
    }
  else
    {
    // This option provided for compatibility with older version of "rxtokenize()"
    if (apirc < 0)
      return INVALID_ROUTINE;
    DosFreeMem(retstr->strptr);
    retstr->strptr = instore[1].strptr;
    retstr->strlength = instore[1].strlength;
    }

  return VALID_ROUTINE;;

}

// Function which sets extended LIBPATH
EXPORTSPEC ULONG APIENTRY  RxSetExtLibPath(PUCHAR name, ULONG numargs, RXSTRING args[],
                       PSZ queuename, PRXSTRING retstr)
{
  // Arg#1 = extended LIBPATH string
  // Arg#2 = (optional) flag indicating when to search extended LIBPATH
  //                    (default = BEFORE)

  ULONG  flags=BEGIN_LIBPATH;
  APIRET rc;

  if (!numargs)
    return INVALID_ROUTINE;
  if (numargs >= 2 && args[1].strlength)
    {
    flags = toupper(*args[1].strptr);
    switch (flags)
      {
      case 'B': // before
        flags = BEGIN_LIBPATH;
        break;
      case 'A': // after
        flags = END_LIBPATH;
        break;
      default:
        return INVALID_ROUTINE;
        break;
      }
    }

  rc=DosSetExtLIBPATH(args[0].strptr,flags);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);
  return VALID_ROUTINE;

}

// Function which queries extended LIBPATH
EXPORTSPEC ULONG APIENTRY  RxQueryExtLibPath(PUCHAR name, ULONG numargs, RXSTRING args[],
                         PSZ queuename, PRXSTRING retstr)
{
  // Arg#1 = name of rexx variable in which extension is returned
  // Arg#2 = flag indicating which extended LIBPATH string to query

  ULONG  flags=BEGIN_LIBPATH, bufsize=1500;
  PCH    pbuf=NULL;
  APIRET rc;

  if (numargs < 2)
    return INVALID_ROUTINE;
  if (!args[0].strlength || !args[1].strlength)
    return INVALID_ROUTINE;
  strupr(args[0].strptr);
  flags = toupper(*args[1].strptr);
  switch (flags)
    {
    case 'B': // before
      flags = BEGIN_LIBPATH;
      break;
    case 'A': // after
      flags = END_LIBPATH;
      break;
    default:
      return INVALID_ROUTINE;
      break;
    }

  pbuf = (char*) malloc(bufsize);
  memset(pbuf,' ',bufsize);
  *(pbuf+(bufsize-1)) = 0x00;
  rc=DosQueryExtLIBPATH(pbuf,flags);
  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);
  if (!rc)
    {
    SHVBLOCK shvb;
    memset(&shvb,0x00,sizeof(shvb));
    shvb.shvname.strptr = args[0].strptr;
    shvb.shvname.strlength = args[0].strlength;
    shvb.shvvalue.strptr = pbuf;
    shvb.shvvalue.strlength = strlen(pbuf);
    RexxVariablePool(&shvb);
    }

  free(pbuf);
  return VALID_ROUTINE;

}

// Function which returns Process Status information
EXPORTSPEC ULONG APIENTRY  RxQProcStatus(PUCHAR name, ULONG numargs, RXSTRING args[],
                     PSZ queuename, PRXSTRING retstr)

{

  SHVBLOCK        shvb;
  SHVBLOCK        shvbsym;
  UCHAR           syvar[] = "RXURXQPROCSTATUSRXU";
  PCH             cp;
  PqsPtrRec_t     pPtrRec;
  PqsPrec_t       pProcRec;
  PqsTrec_t       pThreadRec;
  PqsS16Headrec_t pS16Header;
  PqsS16rec_t     pS16Rec;
  PqsLrec_t       pLibRec;
  PqsMrec_t       pMemRec;
  PCH             varnamebuf=(PCH)NULL;
  PCH             varvalbuf=(PCH)NULL;
  ULONG           ulNumProc, ulNumThread, idxctr, ulNumMem, ulNumSem, ulNumLib;
  PUSHORT         pidx;
  APIRET          rc;
  PSZ             pszModName;
  BOOL            bGetSemInfo=FALSE;
  BOOL            bGetMemInfo=FALSE;
  BOOL            bGetLibInfo=FALSE;

  // Arg #1 - Rexx stem name
  // Arg #2 - switches:
  //          S - Get 16-bit semaphore information
  //          M - Get shared memory information
  //          L - Get library/dll information

  if (numargs < 1)
    return INVALID_ROUTINE;

  if (args[0].strlength == 0)
    return INVALID_ROUTINE;

  strupr(args[0].strptr);

  // If second argument supplied, process it
  if (numargs >= 2)
    {
    if (args[1].strlength > 0)
      {
      strupr(args[1].strptr);
      for (idxctr=1,cp=args[1].strptr;idxctr<=args[1].strlength;idxctr++,cp++)
        {
        switch (*cp)
          {
          case 'S':
            bGetSemInfo=TRUE;
            break;
          case 'M':
            bGetMemInfo=TRUE;
            break;
          case 'L':
            bGetLibInfo=TRUE;
            break;
          default:
            return INVALID_ROUTINE;
            break;
          } // end-switch on character
        } // end-for each character in second argument
      } // end-if second arg isn't empty
    } // end-if second argument supplied

  // Drop variable name first
  memset(&shvb,0x00,sizeof(shvb));
  shvb.shvcode = RXSHV_DROPV;
  shvb.shvname.strptr = args[0].strptr;
  shvb.shvname.strlength = args[0].strlength;
  RexxVariablePool(&shvb);

  rc=DosAllocMem((PVOID) &(pPtrRec),(ULONG)QPROCSTATBUFFSIZE,
                 (ULONG) PAG_COMMIT | PAG_WRITE | OBJ_TILE);
  if (rc)
    return INVALID_ROUTINE;
  memset(pPtrRec,0x00,QPROCSTATBUFFSIZE);

  rc=DosQProcStatus((PULONG)pPtrRec,(USHORT)QPROCSTATBUFFSIZE);

  retstr->strlength = sprintf(retstr->strptr,"%ld",rc);

  if (rc)
    {
    DosFreeMem(pPtrRec);
    return VALID_ROUTINE;
    } // end-if

  varnamebuf = (PCH) malloc(args[0].strlength + 100);
  if (!varnamebuf)
    {
    DosFreeMem(pPtrRec);
    return INVALID_ROUTINE;
    }
  varvalbuf  = (PCH) malloc(500);
  if (!varvalbuf)
    {
    DosFreeMem(pPtrRec);
    return INVALID_ROUTINE;
    }

  // Set up for RexxVariablePool calls
  shvb.shvcode = RXSHV_SET;
  shvb.shvname.strptr = varnamebuf;
  shvb.shvvalue.strptr = varvalbuf;

  memset(&shvbsym,0x00,sizeof(shvbsym));
  shvbsym.shvcode = RXSHV_SYSET;
  shvbsym.shvname.strptr = varnamebuf;
  shvbsym.shvvalue.strptr = varvalbuf;

  // Set Global Record info
  sprintf(varnamebuf,"%s0G.1",args[0].strptr);
  shvb.shvname.strlength = strlen(varnamebuf);
  sprintf(varvalbuf,"%ld",pPtrRec->pGlobalRec->cThrds);
  shvb.shvvalue.strlength = strlen(varvalbuf);
  RexxVariablePool(&shvb);
  sprintf(varnamebuf,"%s0G.2",args[0].strptr);
  shvb.shvname.strlength = strlen(varnamebuf);
  sprintf(varvalbuf,"%ld",pPtrRec->pGlobalRec->Reserved1);
  shvb.shvvalue.strlength = strlen(varvalbuf);
  RexxVariablePool(&shvb);
  sprintf(varnamebuf,"%s0G.3",args[0].strptr);
  shvb.shvname.strlength = strlen(varnamebuf);
  sprintf(varvalbuf,"%ld",pPtrRec->pGlobalRec->Reserved2);
  shvb.shvvalue.strlength = strlen(varvalbuf);
  RexxVariablePool(&shvb);

  // Loop through 16-bit Semaphore Section (if requested)
  if (bGetSemInfo)
    {
    pS16Header=pPtrRec->p16SemRec;

    // Set stem.0S.0.1 (index of first sem)
    sprintf(varnamebuf,"%s0S.0.1",args[0].strptr);
    shvb.shvname.strlength = strlen(varnamebuf);
    sprintf(varvalbuf,"%08x",pS16Header->S16TblOff);
    shvb.shvvalue.strlength = strlen(varvalbuf);
    RexxVariablePool(&shvb);

    for (ulNumSem=1,pS16Rec=(PqsS16rec_t)(((PCH)pS16Header) + sizeof(qsS16Headrec_t));
         pS16Rec->pNextRec;
         pS16Rec=pS16Rec->pNextRec,ulNumSem++)
      {

      // Set stem.0S.n.1 (semindex)
      sprintf(varnamebuf,"%s0S.%ld.1",args[0].strptr,ulNumSem);
      shvb.shvname.strlength = strlen(varnamebuf);
      sprintf(varvalbuf,"%04x",pS16Rec->semindex);
      shvb.shvvalue.strlength = strlen(varvalbuf);
      RexxVariablePool(&shvb);

      // Set stem.0S.n.2 (owning slot)
      //sprintf(varnamebuf,"%s0S.%ld.2",args[0].strptr,ulNumSem);
      //shvb.shvname.strlength = strlen(varnamebuf);
      //sprintf(varvalbuf,"%08x",pS16Rec->s_SysSemOwner);
      //shvb.shvvalue.strlength = strlen(varvalbuf);
      //RexxVariablePool(&shvb);

      // Set stem.0S.n.3 (semaphore flag)
      sprintf(varnamebuf,"%s0S.%ld.3",args[0].strptr,ulNumSem);
      shvb.shvname.strlength = strlen(varnamebuf);
      memcpy(varvalbuf,&pS16Rec->s_SysSemFlag[1],1);
      pS16Rec->s_SysSemFlag[1] = pS16Rec->s_SysSemFlag[0];
      memcpy(&pS16Rec->s_SysSemFlag[0],varvalbuf,1);
      sprintf(varvalbuf,"%04x",*((PUSHORT)(&pS16Rec->s_SysSemFlag[0])));
      shvb.shvvalue.strlength = strlen(varvalbuf);
      RexxVariablePool(&shvb);

      // Set stem.0S.n.4 (reference count)
      sprintf(varnamebuf,"%s0S.%ld.4",args[0].strptr,ulNumSem);
      shvb.shvname.strlength = strlen(varnamebuf);
      sprintf(varvalbuf,"%02x",pS16Rec->s_SysSemRefCnt);
      shvb.shvvalue.strlength = strlen(varvalbuf);
      RexxVariablePool(&shvb);

      // Set stem.0S.n.5 (request count)
      sprintf(varnamebuf,"%s0S.%ld.5",args[0].strptr,ulNumSem);
      shvb.shvname.strlength = strlen(varnamebuf);
      sprintf(varvalbuf,"%02x",pS16Rec->s_SysSemProcCnt);
      shvb.shvvalue.strlength = strlen(varvalbuf);
      RexxVariablePool(&shvb);

      // Set stem.0S.n.6 (semaphore name)
      sprintf(varnamebuf,"%s0S.%ld.6",args[0].strptr,ulNumSem);
      shvb.shvname.strlength = strlen(varnamebuf);
      shvb.shvvalue.strptr = pS16Rec->SemName;
      shvb.shvvalue.strlength = strlen(pS16Rec->SemName);
      RexxVariablePool(&shvb);
      shvb.shvvalue.strptr = varvalbuf;

      } // end-for all 16-bit semaphores

    ulNumSem--;

    // Set stem.0S.0 (Number of 16-bit semaphores in system)
    sprintf(varnamebuf,"%s0S.0",args[0].strptr);
    shvb.shvname.strlength = strlen(varnamebuf);
    sprintf(varvalbuf,"%ld",ulNumSem);
    shvb.shvvalue.strlength = strlen(varvalbuf);
    RexxVariablePool(&shvb);

    } // end-if 16-bit sem information requested

  // Loop through Shared Memory Section (if requested)
  if (bGetMemInfo)
    {
    for (ulNumMem=1,pMemRec=pPtrRec->pShrMemRec;
         pMemRec->pNextRec;
         pMemRec=pMemRec->pNextRec,ulNumMem++)
      {

      // Set symbolic index to this shared memory chunk's memory handle
      sprintf(varvalbuf,"%04x",pMemRec->hmem);
      shvb.shvname.strptr = syvar;
      shvb.shvname.strlength = strlen(syvar);
      shvb.shvvalue.strlength = strlen(varvalbuf);
      RexxVariablePool(&shvb);
      shvb.shvname.strptr = varnamebuf;

      // Set stem.0M.n (memory handle)
      sprintf(varnamebuf,"%s0M.%ld",args[0].strptr,ulNumMem);
      shvb.shvname.strlength = strlen(varnamebuf);
      sprintf(varvalbuf,"%04x",pMemRec->hmem);
      shvb.shvvalue.strlength = strlen(varvalbuf);
      RexxVariablePool(&shvb);

      // Set stem.0M.index.1 (memory selector)
      sprintf(varnamebuf,"%s0M.%s.1",args[0].strptr,syvar);
      shvbsym.shvname.strlength = strlen(varnamebuf);
      sprintf(varvalbuf,"%04x",pMemRec->sel);
      shvbsym.shvvalue.strlength = strlen(varvalbuf);
      RexxVariablePool(&shvbsym);

      // Set stem.0M.index.2 (reference count)
      sprintf(varnamebuf,"%s0M.%s.2",args[0].strptr,syvar);
      shvbsym.shvname.strlength = strlen(varnamebuf);
      sprintf(varvalbuf,"%d",pMemRec->refcnt);
      shvbsym.shvvalue.strlength = strlen(varvalbuf);
      RexxVariablePool(&shvbsym);

      // Set stem.0M.index.3 (memory name)
      sprintf(varnamebuf,"%s0M.%s.3",args[0].strptr,syvar);
      shvbsym.shvname.strlength = strlen(varnamebuf);
      shvbsym.shvvalue.strptr = pMemRec->Memname;
      shvbsym.shvvalue.strlength = strlen(pMemRec->Memname);
      RexxVariablePool(&shvbsym);
      shvbsym.shvvalue.strptr = varvalbuf;

      } // end-for all memory objects

    ulNumMem--;

    // Set stem.0M.0 (Number of shared-memory objects in system)
    sprintf(varnamebuf,"%s0M.0",args[0].strptr);
    shvb.shvname.strlength = strlen(varnamebuf);
    sprintf(varvalbuf,"%ld",ulNumMem);
    shvb.shvvalue.strlength = strlen(varvalbuf);
    RexxVariablePool(&shvb);

    } // end-if shared memory information requested

  // Loop through Linked-Library Section (if requested)
  if (bGetLibInfo)
    {
    for (ulNumLib=1,pLibRec=pPtrRec->pLibRec;
         pLibRec->pNextRec;
         pLibRec=pLibRec->pNextRec,ulNumLib++)
      {

      // Set symbolic index to this library's index value (module handle)
      sprintf(varvalbuf,"%04x",pLibRec->hmte);
      shvb.shvname.strptr = syvar;
      shvb.shvname.strlength = strlen(syvar);
      shvb.shvvalue.strlength = strlen(varvalbuf);
      RexxVariablePool(&shvb);
      shvb.shvname.strptr = varnamebuf;

      // Set stem.0L.n (module handle)
      sprintf(varnamebuf,"%s0L.%ld",args[0].strptr,ulNumLib);
      shvb.shvname.strlength = strlen(varnamebuf);
      sprintf(varvalbuf,"%04x",pLibRec->hmte);
      shvb.shvvalue.strlength = strlen(varvalbuf);
      RexxVariablePool(&shvb);

      // Set stem.0L.index.0I.0 (number of imported modules)
      sprintf(varnamebuf,"%s0L.%s.0I.0",args[0].strptr,syvar);
      shvbsym.shvname.strlength = strlen(varnamebuf);
      sprintf(varvalbuf,"%ld",pLibRec->ctImpMod);
      shvbsym.shvvalue.strlength = strlen(varvalbuf);
      RexxVariablePool(&shvbsym);

      for (idxctr=0;idxctr<pLibRec->ctImpMod;idxctr++)
        {
        // Set stem.0L.index.0I.n (module imported module reference)
        sprintf(varnamebuf,"%s0L.%s.0I.%ld",args[0].strptr,syvar,idxctr+1);
        shvbsym.shvname.strlength = strlen(varnamebuf);
        sprintf(varvalbuf,"%04x",pLibRec->imphmte[idxctr]);
        shvbsym.shvvalue.strlength = strlen(varvalbuf);
        RexxVariablePool(&shvbsym);
        } // end-for each lib entry

      // Set stem.0L.index.1 (library name)
      sprintf(varnamebuf,"%s0L.%s.1",args[0].strptr,syvar);
      shvbsym.shvname.strlength = strlen(varnamebuf);
      shvbsym.shvvalue.strptr = pLibRec->pName;
      shvbsym.shvvalue.strlength = strlen(pLibRec->pName);
      RexxVariablePool(&shvbsym);
      shvbsym.shvvalue.strptr = varvalbuf;

      } // end-for all linked libraries

    ulNumLib--;

    // Set stem.0L.0 (Number of statically linked-libraries in system)
    sprintf(varnamebuf,"%s0L.0",args[0].strptr);
    shvb.shvname.strlength = strlen(varnamebuf);
    sprintf(varvalbuf,"%ld",ulNumLib);
    shvb.shvvalue.strlength = strlen(varvalbuf);
    RexxVariablePool(&shvb);

    } // end-if linked library information requested

  // Clean up working symbolic variable (i.e. DROP it by name)
  shvb.shvcode = RXSHV_DROPV;
  strcpy(varnamebuf,syvar);
  shvb.shvname.strlength = strlen(syvar);
  RexxVariablePool(&shvb);
  shvb.shvcode = RXSHV_SET;

  // Loop through Process/Thread Sections
  pProcRec=pPtrRec->pProcRec;
  ulNumProc=0;
  while (pProcRec < (PqsPrec_t)pPtrRec->p16SemRec && pProcRec->RecType == 0x01)
    {
    ulNumProc++;

    // Set stem.0P.n.1 (Process-Id)
    sprintf(varnamebuf,"%s0P.%ld.1",args[0].strptr,ulNumProc);
    shvb.shvname.strlength = strlen(varnamebuf);
    sprintf(varvalbuf,"%04x",pProcRec->pid);
    shvb.shvvalue.strlength = strlen(varvalbuf);
    RexxVariablePool(&shvb);

    // Set stem.0P.n.2 (Parent Process-Id)
    sprintf(varnamebuf,"%s0P.%ld.2",args[0].strptr,ulNumProc);
    shvb.shvname.strlength = strlen(varnamebuf);
    sprintf(varvalbuf,"%04x",pProcRec->ppid);
    shvb.shvvalue.strlength = strlen(varvalbuf);
    RexxVariablePool(&shvb);

    // Set stem.0P.n.3 (Process Type)
    sprintf(varnamebuf,"%s0P.%ld.3",args[0].strptr,ulNumProc);
    shvb.shvname.strlength = strlen(varnamebuf);
    sprintf(varvalbuf,"%08x",pProcRec->type);
    shvb.shvvalue.strlength = strlen(varvalbuf);
    RexxVariablePool(&shvb);

    // Set stem.0P.n.4 (Process Status)
    sprintf(varnamebuf,"%s0P.%ld.4",args[0].strptr,ulNumProc);
    shvb.shvname.strlength = strlen(varnamebuf);
    sprintf(varvalbuf,"%08x",pProcRec->stat);
    if ( pProcRec->stat & 0x01 )
      strcat(varvalbuf,"/InExitList");
    if ( pProcRec->stat & 0x02 )
      strcat(varvalbuf,"/ExitingThread#1");
    if ( pProcRec->stat & 0x04 )
      strcat(varvalbuf,"/ProcessTerminate");
    if ( pProcRec->stat & 0x10 )
      strcat(varvalbuf,"/ParentWaitingForTermination");
    if ( pProcRec->stat & 0x20 )
      strcat(varvalbuf,"/ParentDidExecAndWait");
    if ( pProcRec->stat & 0x40 )
      strcat(varvalbuf,"/ProcessIsDying");
    if ( pProcRec->stat & 0x80 )
      strcat(varvalbuf,"/ProcessIsEmbryonic");
    shvb.shvvalue.strlength = strlen(varvalbuf);
    RexxVariablePool(&shvb);

    // Set stem.0P.n.5 (Process ScreenGroup/Session Id)
    sprintf(varnamebuf,"%s0P.%ld.5",args[0].strptr,ulNumProc);
    shvb.shvname.strlength = strlen(varnamebuf);
    sprintf(varvalbuf,"%08x",pProcRec->sgid);
    shvb.shvvalue.strlength = strlen(varvalbuf);
    RexxVariablePool(&shvb);

    // Set stem.0P.n.6 (Process Module Name)
    sprintf(varnamebuf,"%s0P.%ld.6",args[0].strptr,ulNumProc);
    shvb.shvname.strlength = strlen(varnamebuf);
    pszModName = GetModName(pProcRec->hMte,pPtrRec->pLibRec,pProcRec->pid);
    if (pszModName)
      {
      shvb.shvvalue.strptr = pszModName;
      shvb.shvvalue.strlength = strlen(pszModName);
      }
    else
      {
      strcpy(varvalbuf,"");
      shvb.shvvalue.strlength = strlen(varvalbuf);
      }
    RexxVariablePool(&shvb);
    shvb.shvvalue.strptr = varvalbuf;

    // Set stem.0P.n.0S.0 (Process Number of 16-bit semaphores)
    sprintf(varnamebuf,"%s0P.%ld.0S.0",args[0].strptr,ulNumProc);
    shvb.shvname.strlength = strlen(varnamebuf);
    sprintf(varvalbuf,"%d",pProcRec->c16Sem);
    shvb.shvvalue.strlength = strlen(varvalbuf);
    RexxVariablePool(&shvb);
    if (bGetSemInfo)
      {
      for (pidx=pProcRec->p16SemRec,idxctr=1;idxctr<=pProcRec->c16Sem;idxctr++,pidx++)
        {
        // Set stem.0P.n.0S.n (Process 16-bit semaphore list index)
        sprintf(varnamebuf,"%s0P.%ld.0S.%ld",args[0].strptr,ulNumProc,idxctr);
        shvb.shvname.strlength = strlen(varnamebuf);
        sprintf(varvalbuf,"%d",(*(pidx))+1);
        shvb.shvvalue.strlength = strlen(varvalbuf);
        RexxVariablePool(&shvb);
        } // end-for each 16-bit sem entry
      } // end-if 16-bit semaphore information requested

    // Set stem.0P.n.0L.0 (Process Number of runtime linked libraries)
    sprintf(varnamebuf,"%s0P.%ld.0L.0",args[0].strptr,ulNumProc);
    shvb.shvname.strlength = strlen(varnamebuf);
    sprintf(varvalbuf,"%d",pProcRec->cLib);
    shvb.shvvalue.strlength = strlen(varvalbuf);
    RexxVariablePool(&shvb);
    if (bGetLibInfo)
      {
      for (pidx=pProcRec->pLibRec,idxctr=1;idxctr<=pProcRec->cLib;idxctr++,pidx++)
        {
        // Set stem.0P.n.0L.n (Process runtime linked library list index)
        sprintf(varnamebuf,"%s0P.%ld.0L.%ld",args[0].strptr,ulNumProc,idxctr);
        shvb.shvname.strlength = strlen(varnamebuf);
        sprintf(varvalbuf,"%04x",*(pidx));
        shvb.shvvalue.strlength = strlen(varvalbuf);
        RexxVariablePool(&shvb);
        } // end-for each lib entry
      } // end-if linked library information requested

    // Set stem.0P.n.0M.0 (Process Number of shared memory handles)
    sprintf(varnamebuf,"%s0P.%ld.0M.0",args[0].strptr,ulNumProc);
    shvb.shvname.strlength = strlen(varnamebuf);
    sprintf(varvalbuf,"%d",pProcRec->cShrMem);
    shvb.shvvalue.strlength = strlen(varvalbuf);
    RexxVariablePool(&shvb);
    if (bGetMemInfo)
      {
      for (pidx=pProcRec->pShrMemRec,idxctr=1;idxctr<=pProcRec->cShrMem;idxctr++,pidx++)
        {
        // Set stem.0P.n.0M.n (Process shared memory list index)
        sprintf(varnamebuf,"%s0P.%ld.0M.%ld",args[0].strptr,ulNumProc,idxctr);
        shvb.shvname.strlength = strlen(varnamebuf);
        sprintf(varvalbuf,"%04x",*(pidx));
        shvb.shvvalue.strlength = strlen(varvalbuf);
        RexxVariablePool(&shvb);
        } // end-for each shared memory object
      } // end-if shared memory information requested

    // Set stem.0P.n.0T.0 (Process Thread-Count)
    sprintf(varnamebuf,"%s0P.%ld.0T.0",args[0].strptr,ulNumProc);
    shvb.shvname.strlength = strlen(varnamebuf);
    sprintf(varvalbuf,"%d",pProcRec->cTCB);
    shvb.shvvalue.strlength = strlen(varvalbuf);
    RexxVariablePool(&shvb);

    pThreadRec = pProcRec->pThrdRec;
    for (ulNumThread=1;ulNumThread<=pProcRec->cTCB;ulNumThread++,pThreadRec++)
      {
      // Set stem.0P.n.0T.m.1 (Thread-Id Within Process)
      sprintf(varnamebuf,"%s0P.%ld.0T.%ld.1",args[0].strptr,ulNumProc,ulNumThread);
      shvb.shvname.strlength = strlen(varnamebuf);
      sprintf(varvalbuf,"%04x",pThreadRec->tid);
      shvb.shvvalue.strlength = strlen(varvalbuf);
      RexxVariablePool(&shvb);

      // Set stem.0P.n.0T.m.2 (Unique thread slot number)
      sprintf(varnamebuf,"%s0P.%ld.0T.%ld.2",args[0].strptr,ulNumProc,ulNumThread);
      shvb.shvname.strlength = strlen(varnamebuf);
      sprintf(varvalbuf,"%04x",pThreadRec->slot);
      shvb.shvvalue.strlength = strlen(varvalbuf);
      RexxVariablePool(&shvb);

      // Set stem.0P.n.0T.m.3 (Sleep Id thread is sleeping on)
      sprintf(varnamebuf,"%s0P.%ld.0T.%ld.3",args[0].strptr,ulNumProc,ulNumThread);
      shvb.shvname.strlength = strlen(varnamebuf);
      sprintf(varvalbuf,"%08x",pThreadRec->sleepid);
      shvb.shvvalue.strlength = strlen(varvalbuf);
      RexxVariablePool(&shvb);

      // Set stem.0P.n.0T.m.4 (Thread Priority)
      sprintf(varnamebuf,"%s0P.%ld.0T.%ld.4",args[0].strptr,ulNumProc,ulNumThread);
      shvb.shvname.strlength = strlen(varnamebuf);
      sprintf(varvalbuf,"%08x",pThreadRec->priority);
      shvb.shvvalue.strlength = strlen(varvalbuf);
      RexxVariablePool(&shvb);

      // Set stem.0P.n.0T.m.5 (Thread System Time)
      sprintf(varnamebuf,"%s0P.%ld.0T.%ld.5",args[0].strptr,ulNumProc,ulNumThread);
      shvb.shvname.strlength = strlen(varnamebuf);
      sprintf(varvalbuf,"%ld",pThreadRec->systime);
      shvb.shvvalue.strlength = strlen(varvalbuf);
      RexxVariablePool(&shvb);

      // Set stem.0P.n.0T.m.6 (Thread User Time)
      sprintf(varnamebuf,"%s0P.%ld.0T.%ld.6",args[0].strptr,ulNumProc,ulNumThread);
      shvb.shvname.strlength = strlen(varnamebuf);
      sprintf(varvalbuf,"%ld",pThreadRec->usertime);
      shvb.shvvalue.strlength = strlen(varvalbuf);
      RexxVariablePool(&shvb);

      // Set stem.0P.n.0T.m.7 (Thread state)
      sprintf(varnamebuf,"%s0P.%ld.0T.%ld.7",args[0].strptr,ulNumProc,ulNumThread);
      shvb.shvname.strlength = strlen(varnamebuf);
      sprintf(varvalbuf,"%08x ",pThreadRec->state);
      switch (pThreadRec->state)
        {
        case 1:
          strcat(varvalbuf,"Ready");
          break;
        case 2:
          strcat(varvalbuf,"Blocked");
          break;
        case 4:
          strcat(varvalbuf,"WaitOnCritSec");
          break;
        case 5:
          strcat(varvalbuf,"Running");
          break;
        default:
          strcat(varvalbuf,"?");
          break;
        }
      shvb.shvvalue.strlength = strlen(varvalbuf);
      RexxVariablePool(&shvb);

      } // end-for (each thread in process)

    // Increment pointer to next Process-Info Block
    pProcRec = (PqsPrec_t) (pProcRec->pThrdRec + pProcRec->cTCB);

    } // end-while (each process)

  // Set stem.0P.0 (Number of Processes in system)
  sprintf(varnamebuf,"%s0P.0",args[0].strptr);
  shvb.shvname.strlength = strlen(varnamebuf);
  sprintf(varvalbuf,"%ld",ulNumProc);
  shvb.shvvalue.strlength = strlen(varvalbuf);
  RexxVariablePool(&shvb);

  DosFreeMem(pPtrRec);
  free(varnamebuf);
  free(varvalbuf);

  return VALID_ROUTINE;;

}

// Get Module Name corresponding to passed module handle
PSZ GetModName(USHORT hmte, PqsLrec_t pli, USHORT pid)

{

  while (pli->pNextRec)
    {
    if (pli->hmte == hmte)
      {
      if ( !strcmp(pli->pName,"SYSINIT") )
        {
        HSWITCH hs;
        SWCNTRL swctl;
        hs = WinQuerySwitchHandle(0,pid);
        if (hs)
          {
          WinQuerySwitchEntry(hs,&swctl);
          return (PSZ) swctl.szSwtitle;
          }
        else
          return (PSZ) pli->pName;
        }
      else
        return (PSZ) pli->pName;
      }
    pli = pli->pNextRec;
    }

  return (PSZ) NULL;

}

// Rexx RxWinSetSelf external function
EXPORTSPEC ULONG APIENTRY  RxWinSetSelf(PUCHAR name, ULONG numargs, RXSTRING args[],
                    PSZ queuename, PRXSTRING retstr)
{

  HAB             hab=0;
  SWCNTRL         swctl;
  PTIB            ptib;
  PPIB            ppib;
  APIRET          rc;
  MRESULT         rslt;
  PqsPtrRec_t     pPtrRec;
  PqsPrec_t       pProcRec;
  PqsTrec_t       pThreadRec;
  ULONG           sessid=0, ulNumThread;

  hab = WinInitialize(0);

  DosGetInfoBlocks(&ptib, &ppib);

  // Get our session-id
  rc=DosAllocMem((PVOID) &(pPtrRec),(ULONG)QPROCSTATBUFFSIZE,
                 (ULONG) PAG_COMMIT | PAG_WRITE | OBJ_TILE);
  if (rc)
    return INVALID_ROUTINE;
  memset(pPtrRec,0x00,QPROCSTATBUFFSIZE);
  rc=DosQProcStatus((PULONG)pPtrRec,(USHORT)QPROCSTATBUFFSIZE);
  pProcRec=pPtrRec->pProcRec;
  while (pProcRec < (PqsPrec_t)pPtrRec->p16SemRec && pProcRec->RecType == 0x01)
    {
    if (ppib->pib_ulpid == pProcRec->pid)
      {
      sessid = pProcRec->sgid;
      break;
      }
    pThreadRec = pProcRec->pThrdRec;
    for (ulNumThread=1;ulNumThread<=pProcRec->cTCB;ulNumThread++,pThreadRec++);
    // Increment pointer to next Process-Info Block
    pProcRec = (PqsPrec_t) (pProcRec->pThrdRec + pProcRec->cTCB);
    } // end-while (each process)
  DosFreeMem(pPtrRec);

  if (searchSwitchList4PID(&swctl,hab,sessid))
    {
    SWP swp;
    memset(&swp,0x00,sizeof(SWP));
    swp.fl = SWP_MAXIMIZE;
    swp.hwnd = swctl.hwnd;
    rslt = WinSendMsg(swctl.hwnd,WM_MINMAXFRAME,(MPARAM)&swp,(MPARAM)0);
    }

  WinTerminate(hab);

  *retstr->strptr = '0';
  retstr->strlength = 1;

  return VALID_ROUTINE;

}

// Search Switch List for entry with specified session-id
BOOL  searchSwitchList4PID(SWCNTRL * pswctl,HAB hab,ULONG sessid)

{

  ULONG    cbItems, cbBuf, ctr;
  PSWBLOCK pswblk;
  BOOL     rslt=FALSE;

  cbItems = WinQuerySwitchList(hab,NULL,0);
  cbBuf = (cbItems * sizeof(SWENTRY)) + sizeof(ULONG);
  pswblk = (PSWBLOCK) malloc(cbBuf);
  WinQuerySwitchList(hab,pswblk,cbBuf);

  for (ctr=0;ctr < cbItems;ctr++)
    {
    if (pswblk->aswentry[ctr].swctl.idSession == sessid)
      {
      memcpy(pswctl,&(pswblk->aswentry[ctr].swctl),sizeof(SWCNTRL));
      rslt = TRUE;
      break;
      }
    }
  free(pswblk);

  return rslt;

}

// Rexx RxPmPrintf external function
EXPORTSPEC ULONG APIENTRY  RxPmPrintf(PUCHAR name, ULONG numargs, RXSTRING args[],
                  PSZ queuename, PRXSTRING retstr)
{

  APIRET  rc = 0;
  ULONG   ctr, byteswritten, memlen;
  PVOID   memptr;
  PCH     qname;
  PCH     qworkarea;
  ULONG   ourpid = 0, servepid = 0;
  HQUEUE  qhandle=0;
  PTIB    ptib;
  PPIB    ppib;
  time_t  timenow;

  qname = NULL;
  if (numargs >= 2)
    {
    if (args[0].strlength > 0)
      {
      strupr(args[0].strptr);
      qname = args[0].strptr;
      }
    }
  else
    {
    *retstr->strptr = '0';
    retstr->strlength = 1;
    return VALID_ROUTINE;
    }

  // Check to see if OS/2 Queue exists
  if (qname == NULL)
    rc = DosOpenQueue(&servepid, &qhandle, PRINTFQNAME);
  else
    {
    qworkarea = (PCH) malloc(strlen(QUEUEPREFIX) + strlen(qname) + 1);
    memset(qworkarea,0x00,strlen(QUEUEPREFIX) + strlen(qname) + 1);
    strcpy(qworkarea,QUEUEPREFIX);
    strcat(qworkarea,qname);
    rc = DosOpenQueue(&servepid, &qhandle, qworkarea  );
    free(qworkarea);
    }

  // If queue does not exist or error opening queue, just quit
  if (rc)
    {
    *retstr->strptr = '0';
    retstr->strlength = 1;
    return VALID_ROUTINE;
    }

  // Find out what process-id we are
  DosGetInfoBlocks(&ptib, &ppib);
  ourpid = ppib->pib_ulpid;

  // Send each argument (starting at arg#2) across OS/2 queue
  byteswritten = 0;
  for (ctr=1;ctr < numargs;ctr++)
    {
    if (args[ctr].strlength == 0)
      continue;
    memlen = args[ctr].strlength + 1;
    rc = DosAllocSharedMem(&memptr,
                           NULL,
                           memlen,
                           OBJ_GIVEABLE | PAG_WRITE | PAG_COMMIT);
    // If error allocating shared memory, just quit
    if (rc)
      {
      *retstr->strptr = '0';
      retstr->strlength = 1;
      return VALID_ROUTINE;
      }
    if ( ourpid != servepid )
      {
      rc = DosGiveSharedMem(memptr, servepid, PAG_READ);
      if (rc)
        {
        DosFreeMem(memptr);
        *retstr->strptr = '0';
        retstr->strlength = 1;
        return VALID_ROUTINE;
        }
      }
    memset(memptr,0x00,memlen);
    memcpy(memptr,args[ctr].strptr,args[ctr].strlength);
    time(&timenow);
    rc = DosWriteQueue(qhandle,timenow,memlen,memptr,0);
    if ( ourpid != servepid || rc )
      DosFreeMem(memptr);
    if (rc)
      continue;
    byteswritten += args[ctr].strlength;
    } // end-for

  retstr->strlength = sprintf(retstr->strptr,"%ld",byteswritten);
  return VALID_ROUTINE;
}

/********************************************************************
* Function:  string2long(length, pointer, *number)                  *
*                                                                   *
* Purpose:   Validates and converts an ASCII-Z string from string   *
*            form to a signed long.  Returns FALSE if the number    *
*            is not valid, TRUE if the number was successfully      *
*            converted.                                             *
*                                                                   *
* RC:        TRUE - Good number converted                           *
*            FALSE - Invalid number supplied.                       *
*********************************************************************/

BOOL string2long(ULONG length, PCH string, LONG *number)
{
  ULONG    accumulator;                // converted number
  INT      sign;                       // sign of number

  sign = 1;                            // set default sign

  // Check for leading plus sign
  if (*string == '+')                  // positive?
    {
    sign = 1;                          // change sign
    string++;                          // step past sign
    length--;
    }

  // Check for leading minus sign
  if (*string == '-')                  // negative?
    {
    sign = -1;                         // change sign
    string++;                          // step past sign
    length--;
    }

  if (length == 0 ||                   // if null string
      length > 9)                      // or too long
    return FALSE;                      // not valid

  accumulator = 0;                     // start with zero

  while (length) {                     // while more digits
    if (!isdigit(*string))             // not a digit?
      return FALSE;                    // tell caller
                                       // add to accumulator
    accumulator = accumulator *10 + (*string - '0');
    length--;                          // reduce length
    string++;                          // step pointer
  }
  *number = accumulator * sign;        // return the value
  return TRUE;                         // good number
}

#endif  // BLD4OS2

int RexxQueueAddElement(PSZ qname, PRXSTRING pelem, BOOL type)
{

  APIRET rc;
  UCHAR  CreatedName[250];
  ULONG  bDupName = FALSE;

  rc = RexxAddQueue(qname,(PRXSTRING)pelem,type);
  switch (rc)
    {
    case 0:
      return 0;
      break;
    case 9:
      memset(CreatedName,0x00,sizeof(CreatedName));
      rc = RexxCreateQueue(CreatedName,sizeof(CreatedName),qname,&bDupName);
      if (rc)
        return rc;
      rc = RexxAddQueue(qname,(PRXSTRING) pelem, type);
      if (rc)
        return rc;
      return 0;
      break;
    default:
      return rc;
      break;
    }

  return 0;
}

#if defined(__BLD4OS2__)

// Load a function API
ULONG loadAPI(PSZ dllname, PSZ procname, ULONG ulOrdinal, PFN * pprocaddr)
{

  APIRET    rc;
  HMODULE   hmod;
  ULONG     ulProcType = 0;
  PFN       pfnProcAddr;

  rc = DosQueryModuleHandle(dllname, &hmod);
  switch (rc)
    {
    case 0:    // Ok
      break;
    case ERROR_MOD_NOT_FOUND:  // Module Not Found (126)
      rc = DosLoadModule((PSZ)NULL, (ULONG)0, dllname, &hmod);
      if (rc)
        return 0;
      break;
    default:
      return 0;
      break;
    }

  rc = DosQueryProcAddr(hmod, ulOrdinal, procname,
                        &pfnProcAddr);
  switch (rc)
    {
    case 0:
      break;
    case ERROR_INVALID_HANDLE:
      rc = DosLoadModule((PSZ)NULL, (ULONG)0, dllname, &hmod);
      if (rc)
        return 0;
      rc = DosQueryProcAddr(hmod, ulOrdinal, procname,
                            &pfnProcAddr);
      if (rc)
        return 0;
      break;
    default:
      return 0;
      break;
    }

  *pprocaddr = (PFN) pfnProcAddr;

  rc = DosQueryProcType(hmod, ulOrdinal, procname,
                        &ulProcType);
  switch (ulProcType)
    {
    case PT_16BIT:
      return 16;
      break;
    case PT_32BIT:
      return 32;
      break;
    default:
      return 0;
      break;
    }

  return 0;
}

// *****************************************************************
//     Global function which is an OS/2 Exception Handler
// *****************************************************************
ULONG APIENTRY __SecondaryThreadHandler(PEXCEPTIONREPORTRECORD prptRec,
                          PEXCEPTIONREGISTRATIONRECORD pRegisRecord,
                          PCONTEXTRECORD               pContextRecord,
                          PVOID                        pvSpare)
{
  APIRET  rc;

  if (pExceptionExit)
    return (*pExceptionExit)(prptRec,pRegisRecord,pContextRecord,pvSpare);

  return (ULONG)XCPT_CONTINUE_SEARCH;

}

#endif  // BLD4OS2
