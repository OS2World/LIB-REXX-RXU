:userdoc.
:prolog.
:title.
:tline.RXU v1.b Help
:etitle.
:eprolog.
:body.

:h1.Name Changed!
:p.Starting with version 1.9 of the RXU function package, all the pieces of
the package will have the name RXU (no longer any references to YDBAUTIL).
I'm doing this for two reasons:
:xmp.
  1) The package is known and distributed in many places as RXU
  2) The name YDBAUTIL isn't as easy to build on as RXU
:exmp.
:p.I don't think this will have any effect on anyone using the package,
other than that you must now have RXU.DLL in your LIBPATH to use the
functions, and the function registration function is now called "RxuInit()"
(rather than the old "RxYdbaUtilInit()").  My apologies for any inconvenience
this name change may cause (forseen and unforseen).

:h1.About RXU
:p.RXU is a package of diverse OS/2 Rexx External Functions.
Almost all the functions in this package are in use in many of my own programs
and are fairly well de-bugged.  Please let me know of any problems you
encounter in using the functions, or of any suggestions for enhancements
to existing functions, or ideas for new ones.
:p.I've written the functions in RXU both to more fully enable Rexx as a
programming language for the OS/2 environment, and as a personal technical
challenge.  Since I'm not making any money off them, I do from time to time
put support/enhancement on the back burner due to other distractions.
:p.I'm waiting to see what the Rexx landscape will look like in the Win95 world
before I explore porting RXU (at least, the portable parts) from OS/2 to Win95.

:xmp.

  With the functions in the RXU function package, you can make
  use of some of OS/2's most powerful facilities, such as multi-threading,
  processes, sessions, semaphores, memory access, OS/2 queues, etc.
  Some of these features are not only useful in pure Rexx applications,
  but they allow you to write Rexx programs which can talk to compiled
  programs on their level (i.e. reference memory by address, use
  semaphores, etc.).

  Another benefit is the ability to quickly prototype real OS/2 programs
  using real OS/2 APIs so that you can cheaply 'prove' techniques
  before committing them to 'C' code.

  The capabilities provided to Rexx programs by this package, in
  conjunction with those provided by other Rexx function packages
  (database access, APPC, etc.) and the base language itself allow
  you to prototype or build Rexx applications with much of the
  functional sophistication (and complexity, unfortunately) of "real"
  OS/2 applications.

  In addition to the many OS/2 API "passthrough" functions, there are
  many Rexx-specific enhancements and additions.


  Author:        Dave Boll
                 (CompuServe = 74170,2016)
                 Internet: 74170.2016@compuserve.com
  Release Date:  25 October, 1996
  Version:       1.b
:exmp.

:h1.Release Notes
:p.Release information follows:
:xmp.
Release 1.b features:
  RxF2C                 - Convert rexx number to floating point
  RxC2F                 - Convert floating point number to rexx number
  RxStruct2Stem         - Now allows struct to be supplied as argument, in addition
                          to being supplied as a pointer to memory
  RxStem2Struct         - Now allows struct to be supplied as argument, in addition
                          to being supplied as a pointer to memory

Release 1.a features:
  RxMalloc              - Allocate memory on the 'C' runtime-managed heap
  RxFree                - Free memory allocated with "RxMalloc()"
  RxuMth...             - Set of math library functions
  RxSetExtLibPath       - Set extended LIBPATH for current process
                          (allows you to dynamically alter LIBPATH per process)
  RxQueryExtLibPath     - Query extended LIBPATH for current process
  RxSetNPHState         - Set state of a named pipe

Release 1.9 features:
                          (The PassByName functions were motivated by seeing many posts in
                           various places inquiring whether variables could be shared twixt
                           separate command files, and by David Durgee, who specifically inquired
                           about this capability and provided feedback during development of
                           these functions (thanks David).  In fairness to him, he suggested a far
                           more rich and useful function scheme than I could figure out how to
                           implement (sorry for not doing justice to the concept).  But I do think
                           the way variable sharing is implemented here will be at least somewhat
                           useful.
                          )
  RxPassByName          - Allows rexx program to call another rexx program (external .CMD file),
                          passing rexx variables (including stems) by name (reference or value).
  RxReturnByName        - Allows a called rexx program to return rexx variables (including stems)
                          by name (reference or value) back to a calling rexx program.
  RxPBNBufSize          - Set/Query fetch buffer size (used by RxPassByName)
  RxPhysicalDisk        - Query information about partitionable disks
  RxTmrQueryFreq        - Query frequency of IRQ0 high-resolution timer
  RxTmrQueryTime        - Query snapshot of IRQ0 high-resolution timer value

Release 1.8 features:
  RxSetRelMaxFH         - Set max file handles for process (relative to current value)
  RxSetMaxFH            - Set maximum file handles for process
  RxDevConfig           - Obtain device configuration information
  RxDevIOCtl            - Do DosDevIOCtl calls (to talk to character devices directly)
  RxWaitChild           - Wait for child process(es) to end
  RxDosRead             - Alias for "RxRead()"
  RxDosWrite            - Alias for "RxWrite()"
  RxSetExceptionExit    - Specify the address of exception exit function

Release 1.7 features:
  RxRsoe2q              - Redirect StdOut/StdErr to a rexx queue (by queue name)
  RxGlobalVar           - Put,Get,Delete system-wide global variables

Release 1.6 features:
  RxRegisterFunctionExe - Use "RexxRegisterFunctionExe()"
  RxRegisterExitDll     - Use "RexxRegisterExitDll()"
  RxRegisterExitExe     - Use "RexxRegisterExitExe()"
  RxQueryExit           - Use "RexxQueryExit()"
  RxDeregisterExit      - Use "RexxDeregisterExit()"

Release 1.5 features:
  RxOpen                - open a file with full DosOpen() capabilities
  RxGetInfoBlocks       - get DosGetInfoBlocks() info
  RxStem2Struct         - map values of a stem into a structure (finally implemented)
  RxQueryAppType        - query application type of an executable file

  RxVioPopUp            - Start a VioPopUp display screen
  RxVioEndPopUp         - Close a VioPopUp display screen
  RxVioWrtCharStrAtt    - Write characters to a VioPopUp display screen
  RxKbCharIn            - Get a character from a VioPopUp display screen
  RxDupHandle           - Do a "DosDupHandle()"
  RxSetFHState          - Set file handle state
  RxQueryFHState        - Query file handle state
:exmp.
:p.Also, the "RxPstat()" function was reincarnated as "RxQProcStatus()" and
is documented herein (it now works properly).
:p.In this version (v1.5), "RxQProcStatus()" stores Library, Shared-Memory,
and 16-Bit Semaphore information differently.  See that section for more
information.

:p.This function package is 32-bit code and will only work on OS/2 2.x

:h1.How to Use It

:p.How to register the functions:
:xmp.
In order to use the functions, you have to register them with Rexx like this:

  call rxfuncadd 'rxuinit','rxu','rxuinit'
  call rxuinit

The .DLL "rxu.dll" must be in your libpath.
The program "RXSRS.EXE" must be in your path in order to use the functions
"RxStartRexxSession" and "RxDetachRexxPgm".
:exmp.

:h1.Available External Functions
:p.The available functions are:
:xmp.

  Function Package utility functions
  ----------------------------------
  RxuInit               - Register all RXU Rexx functions
  RxuTerm               - De-Register all RXU Rexx functions
  RxuQuery              - Query function package version and available
                          external function entry point names

  Math Library Functions
  ----------------------
  RxuMthceil
  RxuMthcosh
  RxuMthexp
  RxuMthfloor
  RxuMthfmod
  RxuMthfrexp
  RxuMthldexp
  RxuMthlog
  RxuMthlog10
  RxuMthmodf
  RxuMthpow
  RxuMthsinh
  RxuMthtanh
  RxuMthasin
  RxuMthacos
  RxuMthatan2
  RxuMthatan
  RxuMthsin
  RxuMthcos
  RxuMthtan
  RxuMthsqrt
  RxuMthfabs
  RxuMtherf
  RxuMtherfc
  RxuMthgamma
  RxuMthhypot

  UPM/Net Related
  -----------
  RxUpm                 - Rexx interface to UPM
  RxNet                 - Rexx interface to certain NET calls
                          (also some UPM-related calls)

  System-Info related
  -------------------
  RxSetExtLibPath       - Set extended LIBPATH for current process
                          (allows you to dynamically alter LIBPATH per process)
  RxQueryExtLibPath     - Query extended LIBPATH for current process
  RxTmrQueryFreq        - Query frequency of IRQ0 high-resolution timer
  RxTmrQueryTime        - Query snapshot of IRQ0 high-resolution timer value
  RxProcId              - Get process' own PID and TID information
  RxGetInfoBlocks       - Get information about current process/thread
  RxQueryAppType        - Get information about an executable file
  RxQuerySysInfo        - Invoke DosQuerySysInfo
  RxQProcStatus         - Obtain Process Status Information (like PSTAT)
  RxSetError            - Set DosError settings (enable/disable HardError
                          and Exception popups)
  RxReplaceModule       - Replace an active .DLL or .EXE file
  RxExitList            - Use DosExitList
  RxDevConfig           - Obtain device configuration information
  RxPhysicalDisk        - Query information about partitionable disks
  RxDevIOCtl            - Do DosDevIOCtl calls (to talk to character devices directly)

  Rexx programming and debugging functions
  ----------------------------------------
  RxF2C                 - Convert rexx number to floating point
  RxC2F                 - Convert floating point number to rexx number
  RxVlist               - List, manipulate Rexx variable pool
  RxGlobalVar           - Put,Get,Delete system-wide global variables
  RxScount              - Count strings (needle) in another string (haystack)
  RxPmPrintf            - Write lines to a PMPrintf Monitor
  RxCallInStore         - Execute a string as a program
  RxPassByName          - Allows rexx program to call another rexx program (external .CMD file),
                          passing rexx variables (including stems) by name (reference or value).
  RxReturnByName        - Allows a called rexx program to return rexx variables (including stems)
                          by name (reference or value) back to a calling rexx program.
  RxPBNBufSize          - Set/Query fetch buffer size (used by RxPassByName)
  RxTokenize            - Tokenize ("Compile") a program string
  RxPullQueue           - Pull items from any Rexx data queue
  RxAddQueue            - Add items to any Rexx data queue
  RxQueued              - Query number of items on any Rexx data queue
  RxQExists             - Query existence of a Rexx Queue
  RxSearchPath          - Find a file in a path
  RxRegisterFunctionExe - Use "RexxRegisterFunctionExe()"
  RxRegisterExitDll     - Use "RexxRegisterExitDll()"
  RxRegisterExitExe     - Use "RexxRegisterExitExe()"
  RxQueryExit           - Use "RexxQueryExit()"
  RxDeregisterExit      - Use "RexxDeregisterExit()"

  I/O related
  -----------
  RxRsoe2f              - Redirect StdOut/StdErr to a file (by file name)
  RxRsoe2q              - Redirect StdOut/StdErr to a rexx queue (by queue name)
  RxSoSe2H              - Redirect StdOut/StdErr to a file (by file handle)
  RxSi2H                - Redirect StdIn from a file (by file handle)
  RxRSi2F               - Redirect StdIn from a file (by file name)
  RxOpen                - Open a file (with full DosOpen capabilities)
  RxRead                - Read data from a file handle
  RxDosRead             - Alias for "RxRead()"
  RxWrite               - Write data to a file handle
  RxDosWrite            - Alias for "RxWrite()"
  RxCloseH              - Close a file handle
  RxExecI               - Read data into a Rexx queue or stem from a file
  RxExecO               - Write data from a Rexx queue or stem to a file
  RxVioPopUp            - Start a VioPopUp display screen
  RxVioEndPopUp         - Close a VioPopUp display screen
  RxVioWrtCharStrAtt    - Write characters to a VioPopUp display screen
  RxKbCharIn            - Get a character from a VioPopUp display screen
  RxDupHandle           - Do a "DosDupHandle()"
  RxSetFHState          - Set file handle state
  RxQueryFHState        - Query file handle state
  RxSetMaxFH            - Set maximum file handles for process
  RxSetRelMaxFH         - Set max file handles for process (relative to current value)

  OS/2 Pipes
  ----------
  RxCreateNPipe         - Create a named pipe
  RxSetNPHState         - Set state of a named pipe
  RxConnectNPipe        - Connect to a named pipe
  RxDisConnectNPipe     - Disconnect from a named pipe
  RxCreatePipe          - Create an un-named pipe
  RxDestroyPipe         - Destroy a pipe

  OS/2 Queues
  ----------
  RxCreateQueue         - DosCreateQueue
  RxOpenQueue           - DosOpenQueue
  RxPeekQueue           - DosPeekQueue
  RxReadQueue           - DosReadQueue
  RxWriteQueue          - DosWriteQueue
  RxPurgeQueue          - DosPurgeQueue
  RxQueryQueue          - DosQueryQueue
  RxCloseQueue          - DosCloseQueue
  RxReadQueueStr        - Returns data from de-referenced queue pointer

  NetBios information
  -------------------
  RxNbSessionStatus     - Obtain NETBIOS session status information

  Tasking, threads, etc.
  ----------------------
  RxSetPriority         - Set the priority of processes or threads
            Process - Related
  RxKillProcess         - Kill an OS/2 process by process-id
  RxExecPgm             - Execute a program using DosExecPgm
  RxWaitChild           - Wait for a child process to end
  RxStartSession        - Start a program in another session (DosStartSession)
  RxStartRexxSession    - Start a Rexx program in another session
  RxDetachRexxPgm       - Detach a Rexx program
            Thread - Related
  RxCreateRexxThread    - Execute a Rexx program on another thread
  RxSetExceptionExit    - Specify the address of a "_System" linkage function
                          which is called when an OS/2 exception occurs on a
                          thread started with "RxCreateRexxThread".
  RxCreateThread        - Call a procedure address on another thread
  RxKillThread          - Kill a thread by thread-id
  RxResumeThread        - Resume thread execution by thread-id
  RxSuspendThread       - Suspend thread execution by thread-id
  RxCallEntryPoint      - Call a (non-Rexx) routine by entry point address

  OS/2 Memory managment
  ---------------------
  RxMalloc              - Allocate memory using "malloc()"
  RxFree                - Free memory allocated by "rxmalloc()" using "free()"
  RxStructMap           - Generate a structure map for RxStruct2Stem()
  RxStruct2Stem         - Map structure elements into a stem
  RxStem2Struct         - Map a stem into structure elements
  RxStorage             - Query/Alter storage by address
  RxAdd2Ptr             - Pointer Arithmetic (Add/Subtract)
  RxThunkAddr           - Thunk an address Flat->Segmented, Segmented->Flat
  RxAllocMem            - Allocate Memory
  RxFreeMem             - Free Memory
  RxAllocSharedMem      - Allocated Shared memory (named or un-named)
  RxGetSharedMem        - Get (gettable) shared memory
  RxGiveSharedMem       - Give (giveable) shared memory
  RxGetNamedSharedMem   - Get named shared memory
  RxSetMem              - Set memory attributes
  RxQueryMem            - Query memory attributes
  RxSubAllocMem         - Suballocate memory
  RxSubFreeMem          - Free suballocated memory
  RxSubSetMem           - Set memory for suballocation
  RxSubUnsetMem         - Unset previously "SubSet" memory

  OS/2 Semaphores
  ---------------
             Event Semaphore
  RxCreateEventSem      - Create an event semaphore
  RxCloseEventSem       - Close an event semaphore
  RxOpenEventSem        - Open an event semaphore
  RxPostEventSem        - Post an event semaphore
  RxQueryEventSem       - Query an event semaphore
  RxResetEventSem       - Reset an event semaphore
  RxWaitEventSem        - Wait on an event semaphore
             Mutex Semaphore
  RxCreateMutexSem      - Create a Mutex semaphore
  RxOpenMutexSem        - Invoke DosOpenMutexSem
  RxCloseMutexSem       - Invoke DosCloseMutexSem    
  RxQueryMutexSem       - Invoke DosQueryMutexSem    
  RxReleaseMutexSem     - Invoke DosReleaseMutexSem  
  RxRequestMutexSem     - Invoke DosRequestMutexSem  
             MuxWait Semaphore
  RxCreateMuxWaitSem    - Invoke DosCreateMuxWaitSem 
  RxCloseMuxWaitSem     - Invoke DosCloseMuxWaitSem  
  RxOpenMuxWaitSem      - Invoke DosOpenMuxWaitSem   
  RxWaitMuxWaitSem      - Invoke DosWaitMuxWaitSem   
  RxAddMuxWaitSem       - Invoke DosAddMuxWaitSem    
  RxDeleteMuxWaitSem    - Invoke DosDeleteMuxWaitSem 
  RxQueryMuxWaitSem     - Invoke DosQueryMuxWaitSem  

  DLL Handling
  ------------
  RxLoadModule          - Load a DLL
  RxFreeModule          - Free a DLL
  RxQueryModuleName     - Query the fully qualified name of a DLL (by handle)
  RxQueryModuleHandle   - Query the module handle of a DLL (by name)
  RxQueryProcType       - Query the addressing mode of an entry point in a DLL
  RxQueryProcAddr       - Query the procedure address of an entry point in a DLL

  Rexx Macro Space Handling
  -------------------------
  RxAddMacro            - Add a particular Macro Space function
  RxDropMacro           - Drop a particular Macro Space function
  RxClearMacroSpace     - Clear the Rexx Macro Space
  RxSaveMacroSpace      - Save a particular Macro Space function to a file
  RxLoadMacroSpace      - Load a particular Macro Space function from a file
  RxQueryMacro          - Query the position of a particular Macro Space function
  RxReorderMacro        - Reorder a function's position in a Macro Space

  PM / Wp related functions
  -------------------------
  RxWinQueryObject      - Query object handle of a WP object
  RxWinDestroyObject    - Destroy a WP object
:exmp.

:h1.Miscellaneous Programming Notes
:p.A few things to keep in mind when using the functions in this package.

:h2.Documentation of Parameter Values
:p.Many functions in this package are meant to provide a "pass-through" to
the underlying OS/2 API set.  For this reason, in most cases, I did not
"simplify" the parameter lists (one man's simplification is often another
man's complication anyway).  I also did not document the meaning of most of
the API parameter values (because of duplication effort, copyright, playing
"keep-up", etc.).

:h2.Binary Parameter Values
:p.Many of the OS/2 API pass-through functions that deal with file-handles, memory
addresses, semaphore handles, etc., treat these arguments/returned-values as binary
values (i.e. 16-/32-bit values) and do not convert these values to hexadecimal strings.
The reason for this is that these values are typically only meant to be used as tokens
that are passed back into some relevant function (e.g. a semaphore handle obtained by
a create-semaphore API is passed back into some other semaphore function) and not
scrutinized by the rexx code itself.  In some cases, some functions return an integer
value (as a normal rexx integer number), but this number would subsequently be used
by some other function as a binary value (e.g. rxcreaterexxthread returns thread-id as
a normal rexx integer number, but rxkillthread() takes a binary value for the thread-id
argument).  The binary values required in these cases are typically 32-bit integers,
and also must be in little-endian form (i.e. byte-reversed).  A simple way to convert
the integer value to the proper binary form is as such:
:xmp.
  binary_value = reverse(d2c(integer_value,4))
:exmp.

:h2.How Functions Return Values
:p.The functions in this package return essentially three categories of values:
:xmp.
  1) An unknown number of values (e.g. rxvlist())
  2) A known or fixed number of values that can be strung together as blank-delimited
     tokens
  3) A known or fixed number of values that may contain binary data (i.e. not what
     we'd consider character strings)
:exmp.
:p.For case #1, the values are usually returned either under a stem, or on a queue.  For
case #2, the values are often (but not always) returned as blank-delimited tokens following
the return code of the underlying OS/2 API (thus, the values are most easily obtained by
using the PARSE instruction, for instance).  For case #3, the function usually requires
you to supply a Rexx variable name (or names) in which the binary values will be returned.

:h1.UPM Related Functions
:p.Functions which allow certain UPM activities to be performed.

:h2.RxUpm - Rexx/UPM Interface
:p.This function allows you to make most UPM calls from a Rexx program.
:xmp.
Syntax:

  func  = 'Logon'     For Process level logon, only usable by that process */
          'Logonp'
        = 'Logonu'    For "User" logon, usable by all processes
        = 'Logoff'    For Process level logoff, undoes a "LOGONP"
          'Logoffp'
        = 'Logoffu'   For "User" logoff, undoes a "LOGONU"
        = 'LogonList' Gets list of current active logons
  uid   = 'Userid'    Userid to be logged on/off
        = 'stemname'  Name of Rexx stem variable under which results
                      of "LogonList" are mapped as such:
                      stem.0    - Number of logons reported on
                      stem.n.1  - Userid
                      stem.n.2  - Node/Domain name
                      stem.n.3  - Session-Id
                      stem.n.4  - Logon type (Local/Node/Domain)
  pw    = 'Password'  Password to be used with 'Userid' (for logon)
  type  = 'Local'     Logs you on/off UPM locally (default is LOCAL if
                      no value supplied)
        = 'Node'      Logs you on/off a node
        = 'Domain'    Logs you on/off a LAN domain
        = 'dataBase'  Logs you on/off the correct node for a database
  name  = 'Name'      The name of the node/domain/database, depending on
                      what "TYPE" was chosen
  check = 'Admin'     Checks to see if userid has Admin authority
        = 'User'      Checks to see if userid has User authority
        = 'Config'    Checks to see if logon was done from Config.Sys

  upmrc = RxUpm(func, uid, pw, type, name, check)
:exmp.

:h2.RxNet - Rexx/NET Interface
:p.This function allows you to make certain NetApi calls from Rexx.  This
enables you to do such things as changing passwords, adding and deleting
userids, validating a userid/pw, listing users and logons.
:xmp.
Syntax:

/* General syntax */
  netrc = RxNet(func, server, arg3, arg4, arg5, arg6)

where:

  func   = 'UserPwSet'    /* Change a user's password */
         = 'UserAdd'      /* Add a user to UPM */
         = 'UserDel'      /* Delete a user from UPM */
         = 'UserVal'      /* Validate a userid/password without logging on */
         = 'UserEnum'     /* List users in UPM */
         = 'UserGetInfo'  /* Get user information */
         = 'LogonEnum'    /* List active logons */
         = 'FileEnum'     /* File usage on the LAN */
         = 'UseAdd'       /* Do a "Net Use dev \\srv\alias" */
         = 'UseDel'       /* Do a "Net Use dev /d" */
         = 'UseEnum'      /* Do a "Net Use" */
  server = name of server, or '00'x for local
  netrc  = return code from NetApi call. If netrc < 2100, then it's probably
           a base OS/2 return code, otherwise, it's defined in NETCONS.H

/* Specific calls */
  netrc = RxNet('UserPwSet', server, userid, oldpw, newpw)

  netrc = RxNet('UserAdd', server, userid, password, type, comment)
          type = 'User', 'Guest', or 'Admin'

  netrc = RxNet('UserDel', server, userid)

  netrc = RxNet('UserVal', server, userid, password)

  netrc = RxNet('UserEnum',server, rexxstem [,uidfilter][,infotype])
          uidfilter = userid prefix filter (i.e. only return info on
                      those userids whose leading characters match
                      this prefix).
          infotype  = "FULL" or "USERNAME" which indicates what type
                      of information to return
          rexxstem  = Rexx stem name, under which results are mapped as:
                      stem.0 = if "uidfilter" not supplied:
                                 - number of entries returned in stem
                               if "uidfilter" was supplied, two blank
                               delimited integers:
                                 - number of entries returned in stem
                                 - total number of entries
                      stem.n.1 = userid

                      If infotype = "FULL", the following also:

                      stem.n.2 = privilege level (Guest, User, Admin, ?)
                      stem.n.3 = password age (in seconds)
                      stem.n.4 = comment
                      stem.n.5 = script path
                      stem.n.6 = full name
                      stem.n.7 = user comment
                      stem.n.8 = parms
                      stem.n.9 = logon server

  netrc = RxNet('UserGetInfo',server, rexxstem ,username)
          username  = username to get information for
          rexxstem  = Rexx stem name, under which results are mapped as:
                      stem.1 = userid
                      stem.2 = privilege level (Guest, User, Admin, ?)
                      stem.3 = password age (in seconds)
                      stem.4 = comment
                      stem.5 = script path
                      stem.6 = full name
                      stem.7 = user comment
                      stem.8 = parms
                      stem.9 = logon server

  netrc = RxNet('LogonEnum',server, rexxstem)
          rexxstem = Rexx stem name, under which results are mapped as:
                     stem.0 = number of users
                     stem.n.1 = userid

  netrc = RxNet('FileEnum',server, basepath, username, rexxstem)
          basepath = file path/name qualifier
          username = username qualifier
          rexxstem = Rexx stem name, under which results are mapped as:
                     stem.0 = number of entries
                     stem.n.1 = file-id
                     stem.n.2 = permissions
                     stem.n.3 = numlocks
                     stem.n.4 = pathname
                     stem.n.5 = username

  netrc = RxNet('UseAdd', server, device, name, password)
          device   = Local name for resource, such as a drive letter
          name     = server\alias (must be like "\\servname\alias")
          password = optional password to use resource

  netrc = RxNet('UseDel', server, device, forcemode)
          device    = Local name for resource, such as a drive letter
          forcemode = 'F'orce
                      'N'o force
                      'M'ax force

  netrc = RxNet('UseEnum',server, rexxstem)
          rexxstem = Rexx stem name, under which results are mapped as:
                     stem.0 = number of entries
                     stem.n.1 = device name
                     stem.n.2 = name
                     stem.n.3 = status, values are:
                                Ok
                                Paused
                                Disconnected/SessionLost
                                NetError
                                Connecting
                                Reconnecting
                                ?
                     stem.n.4 = asg_type, values are:
                                WildCard
                                DiskDevice
                                SpooledPrinter
                                SerialDevice
                                IPC (InterProcessCommunication)
                                ?
                     stem.n.5 = refcount
                     stem.n.5 = usecount
:exmp.

:h1.Math Library Functions
:p.Passthrough functions to standard 'C' math library functions.
:p.All functions take a single floating point value as an argument and 
return a double precision floating point value as a result (with a few
exceptions, where noted by @).
:xmp.
  RxuMthceil(x)    - smallest integer that is greater than or equal to "x" (from "ceiling")
  RxuMthexp(x)     - "e" to the "x" power
  RxuMthfloor(x)   - largest integer less than or equal to "x" (from "floor")
 @RxuMthfmod(x,y)  - floating point remainder of x/y
 @RxuMthfrexp(x)   - returns mantissa and exponent of "x" (two blank delimited values)
 @RxuMthldexp(x,y) - computes x*(2**y)
  RxuMthlog(x)     - natural logarithm of "x"
  RxuMthlog10(x)   - base-10 logarithm of "x"
 @RxuMthmodf(x)    - returns fractional and integral parts of "x" (two blank delimited values)
 @RxuMthpow(x,y)   - "x" to the "y" power (x**y)
  RxuMthsinh(x)    - hyperbolic sine of "x" ("x" in radians)
  RxuMthcosh(x)    - hyperbolic cosine of "x" ("x" in radians)
  RxuMthtanh(x)    - hyperbolic tangent of "x" ("x" in radians)
  RxuMthasin(x)    - arcsine of "x" (in radians)
  RxuMthacos(x)    - arccosine of "x" (in radians)
 @RxuMthatan2(y,x) - arctangent of "y/x" (in radians)
  RxuMthatan(x)    - arctangent of "x" (in radians)
  RxuMthsin(x)     - sine of "x" ("x" in radians)
  RxuMthcos(x)     - cosine of "x" ("x" in radians)
  RxuMthtan(x)     - tangent of "x" ("x" in radians)
  RxuMthsqrt(x)    - square root of "x"
  RxuMthfabs(x)    - absolute value of "x"
  RxuMtherf(x)     - error function of "x"
  RxuMtherfc(x)    - computes 1.0 - erf(x) (used for large values of "x")
  RxuMthgamma(x)   - gamma function of "x" ( ln(|G(x)|) )
 @RxuMthhypot(x,y) - hypoteneuse of right triangle of sides "x" and "y" ( sqrt(x**2 + y**2) )
:exmp.

:h1.Rexx Programming/Debugging
:p.Tools for developing Rexx programs.

:h2.RxSCount - Count instances of a string
:p.This function returns a count of the number of instances of a string
"needle" in a string "haystack", optionally ignoring case.

:xmp.
Syntax:

  numitem = rxscount(needle,haystack[,case])

where:

  needle   = string you want to count instances of
  haystack = string in which you want to count instances of "needle"
  case     = any character to indicate that case (upper/lower) is to
             be ignored
  numitem  = number of times "needle" is found in "haystack"
:exmp.

:h2.RxPmPrintf - Write lines to a PMPrintf Monitor
:p.This function writes data lines to a PMPrintf "Monitor" program.
:xmp.
Syntax:

  bytes = RxPmPrintf(qname, [data1 [,data2 [,data3 [...]]]] )

where:

  qname    = name of PMPrintf queue
             If blank, uses default PMPrintf Queue (PRINTF32).
  data1    = data to be written to PMPrintf
  ...        Each argument is written as a separate line
  datan

  bytes    = number of bytes written in this operation

Notes:

  The PMPrintf tool can be downloaded from the OS/2 Bulletin Board.
  This Rexx function does work with PMPrintf v2.5, but may not work
  with earlier versions.  You can start the PMPrintf program with
  a command line argument specifying an alternate queue name for it
  to use.  This queue name can be supplied as the first argument of
  RxPmPrintf to direct the data to a specific running instance of
  PMPrintf.

  If only the queue name is supplied (i.e. args 2-n are not supplied),
  nothing at all is written to queue.  If an argument has zero length,
  no data will be written to the queue for that argument (i.e. you
  must supply at least one byte to be written to queue).
:exmp.

:h2.RxPassByName - Pass variables (by name) to external rexx (.CMD file) program
:p.This function allows one Rexx program to call another Rexx program
(i.e. another .CMD file), passing it rexx variables by name.  In
addition, these variables can be passed "by reference" or "by value."
:p.The normal method of invoking a Rexx external function is by the "call"
instruction or by function reference.  In these cases, arguments can be
passed as expressions and are accessed in the called routine by the "parse arg"
instruction or the "arg()" built-in function.
:p.To pass a variable name "by value" means that the variable's name and
value will be placed in the called program's variable pool before that
called program begins executing.  To pass a variable "by reference"
means the same as "by value," but additionally, any changes to the
variable in the called program will be reflected in the variable pool
of the calling program upon return of control (to the caller).
:xmp.
Syntax:

  result = RxPassByName(yourfunc, varlist [,arg1,arg2,...,arg18])

where:

  yourfunc = name of rexx program file to be called (as function or subroutine)
                   or
             a '$' immediately preceding a rexx program source string
                   or
             a '&amp.' immediately preceding a tokenized program string
                   or
             a '+' immediately preceding the name of a program in the macrospace

             whatever form of rexx program is specified, it can be run on a 2ndary
             thread by preceding "yourfunc" with a tilda (~).  If the rexx program
             is run on a 2ndary thread, "by reference" will not have an effect on
             the calling program.
  varlist  = list of blank delimited variable names to be passed to "yourfunc"
             Individual items in the list are passed "by value" unless the item is
             preceded by an ampersand (&amp.), in which case it is passed by
             reference.  For instance, in the following example,

               call rxpassbyname 'yourfunc','abc  &amp.frank st1. &amp.st2.', date()

             the variable "abc", and the stem "st1." would be passed "by value"
             to "yourfunc", whereas the variable "frank" and the stem "st2." would
             be passed "by reference" to "yourfunc".  That is, "abc" and all values
             of the stem "st1." would be passed to "yourfunc" (but whatever "yourfunc"
             does to these variables has no effect on their values in the caller), and
             "frank" and all values of the stem "st2." would be passed to "yourfunc"
             (and whatever "yourfunc" does to these variables WILL have an effect on
             their values in the caller).

             Note that the called Rexx program ("yourfunc") will receive "arg1" as
             its first argument, "arg2" as its second argument, and so on.  If you want
             the called routine to know what variables were passed to it, you must
             supply this variable list as a separate argument.
  arg1     = subsequent argument(s) to "yourfunc", passed on as-is (i.e. in normal Rexx
  ...        fashion.  Note that given the Rexx restriction of 20 arguments, and the
  argn       fact that two arguments are "used up" by the called routine's name and
             the variable name list, only 18 remain for normal argument passing.
  result   = whatever value is returned by "yourfunc" (independent of what "flows"
             back from "yourfunc" as a result of "by reference" variables).
                   or
             the thread-id of the 2ndary thread, if called routine was run on a 2ndary
             thread

Notes:

  When variables are passed "by reference," whatever is done to a variable (or stem) in
  the called routine will show up back in the calling routine, including dropping the
  variable ("drop" instruction) with all that this implies for stems.  Thus, passing a
  variable "by reference" to an external Rexx routine should have the same effect that
  the "expose" option of the "procedure" instruction has on internal Rexx routines.
  The difference is that "expose" is specified by the called routine, and "by reference"
  is specified by the calling routine.

  RxPassByName() is multi-thread safe.  It may also be called by nested Rexx programs
  (i.e. "pgma.cmd" calls "pgmb.cmd" which calls "pgmc.cmd", etc.), as would be expected.
:exmp.

:h2.RxReturnByName - Return variables (by name) to external calling Rexx program
:p.This function allows a RxPassByName-called Rexx program to return rexx variables
(by name) to the calling rexx program (i.e. another .CMD file).  These variables
are always passed "by value."  Values are obtained for return at the time the rexx
routine actually terminates (NOT when this function is called).  So, this function
has the effect of specifying what variables are to be returned (actually, simple
variables are specified at this time (i.e. non-stems), but only the stem of compound
variables are specified -- the actual tails of a stem are resolved at return time).
:xmp.
Syntax:

  successful = RxReturnByName(varlist)

where:

  varlist    = list of blank delimited variable names to be returned to caller
               Individual variables are passed "by value".
  successful = boolean (1 or 0) indicating whether items to be returned were noted

Notes:

  When using this function, it is up to you to ensure that the rexx routine from which
  you call this function was itself invoked via the "RxPassByName()" function.  If this
  is not the case, but some prior rexx routine in the call chain (on current thread) was
  invoked via "RxPassByName()", that routine's caller will pick up the variable values
  being passed back, and furthermore that "RxPassByName()"-invoked rexx program's values
  will be returned, not those values from the current rexx program's variable pool.

  The upshot is that you must be careful when using this function that you call it only
  from within a rexx program that was itself invoked (directly) via "RxPassByName()".

  RxReturnByName() is multi-thread safe.
:exmp.

:h2.RxPBNBufSize - Set/Query fetch buffer size (used by RxPassByName)
:p.This function allows you to set the working buffer size used by
RxPassByName to fetch subsets of the variable pool when passing stem
variables between Rexx programs.  You do not have to use this function
in order to use RxPassByName (working buffer defaults to 32k), but you
could use this function if, for instance, many stem values are more than
the default size (32k).  In this case, you would specify a larger work
buffer size to prevent RxPassByName from having to access the variable
pool twice for each stem value.  The size you specify would ideally be
as large as the longest stem value, but doesn't have to be (i.e. RxPassByName
will always work, just not as efficiently as it could).
:p.As a rule, you should never need to use this function.  It is just
provided as a means to tune RxPassByName in cases where tuning would
matter (memory constrained environment where you can't afford the default
of 32k and stem value lengths are usually much less than 32k, stem values
longer than 32k, etc.).
:p.Also, note that the work buffer size applies to all threads in a process.
That is, Rexx programs running on different threads in the same process will
all use the current value, so that any changes made to the buffer size will
affect all threads.
:xmp.
Syntax:

  oldval = RxPBNBufSize([newsize])

where:

  newsize  = optional, new work buffer size
  oldval   = previous work buffer size
:exmp.

:h2.RxCallInStore - Execute a string as a program
:p.This function allows the Rexx program developer to execute a
string as though it were a Rexx program.  This allows you to build
the equivalent of a Rexx program's text in a variable, and then
execute it from the string.  Actually, the Rexx program text can come
from anywhere -- a file, over a communications link, from a database.
For repeated execution of a program, use "RxTokenize" to "compile" the
program, then call "RxCallInStore" with "&amp." to re-use the tokenized
program.
:xmp.
Syntax:

  result = RxCallInStore strtext, arg1, arg2, ..., arg19

where:

  strtext  = text of a Rexx program, complete with crlf and eof markers
                  or
             a '$' immediately preceding a source program string
                  or
             a '&amp.' immediately preceding a tokenized program string
  arg1     = arguments to the called program
  ...
  arg19

Note:

  Example of executing a program string:

    crlf = '0d0a'x
    str = '/* Rexx */'crlf
    str = str 'Do i=1 To Arg()'crlf
    str = str '  Say "Arg#"i "= {"arg(i)"}"'crlf
    str = str 'End'crlf
    str = str||'1a'x       /* EOF marker */
    call rxcallinstore str, date(), time()
               ( or )
    call rxcallinstore '$'str, date(), time()
               ( or )
    tstr = rxtokenize(str)
    call rxcallinstore '&amp.'tstr, date(), time()
:exmp.

:h2.RxTokenize - Tokenize a program source string
:p.This function in effect "compiles" a program source string for
re-use with subsequent "RxCallInstore" or "RxCreateRexxThread" calls.
This provides better performance when a program string is repeatedly
called.
:xmp.
Syntax:

  tokenstr = RxTokenize(progstr)
  rxrc = RxTokenize(progstr,rexxvar)

where:

  progstr  = actual Rexx program source string
  rexxvar  = name of rexx variable in which tokenized version is returned
  tokenstr = tokenized ("compiled") result of "progstr"
  rxrc     = two blank delimited integers:
             1) return code from invoking rexx interpreter
             2) rexx return code

Note:

  This function, in conjunction with the "RxCallInstore" and/or
  "RxCreateRexxThread" functions, can be used to achieve the same
  effect as the Rexx Macrospace, except that this technique is local
  to the program using it (whereas the Macrospace is system global).
:exmp.

:h2.RxF2C - Convert rexx number to floating point
:p.This function is analogous to the Rexx built-in function "d2c()" except
that it operates on floating point numbers rather than decimal numbers.
:xmp.
Syntax:

  fpnumber = RxF2C( rexxnumber [,fplength] )

where:

  rexxnumber  = a valid rexx number
  fplength    = one of the following, which indicates what kind of floating point number
                to create:
                        32 - float
                        64 - double
                        80 - long double
  fpnumber    = binary floating point number
:exmp.

:h2.RxC2F - Convert floating point number to rexx number
:p.This function is analogous to the Rexx built-in function "c2d()" except
that it operates on floating point numbers rather than decimal numbers.
:xmp.
Syntax:

  rexxnumber = RxC2F( fpnumber [,numdecdigits] )

where:

  fpnumber     = binary floating point number
  numdecdigits = number of digits of precision to use in rendering "fpnumber" as a
                 rexx number
  rexxnumber   = a valid rexx number
:exmp.

:h2.RxVlist - Display/Process Rexx Variables
:p.This function allows the Rexx program developer to view portions of the
Rexx variable pool.  It also allows you to effectively "pass" variable
pools (or subsets thereof) between execs across Rexx queues.
:xmp.
Syntax:

  numvar = RxVlist( [vnamepat] [,func] [,qname] )

where:

  func     = 'V'  /* Puts variable name/value pairs on a queue */
           = 'N'  /* Puts variable names on a queue */
           = 'G'  /* Gets variable name/value pairs off a queue */
                  /* and incorporates them into the current variable pool */
           = 'D'  /* display variables and their values (default) */
  vnamepat = variable name prefix (every variable whose name begins with this
             value is processed according to the specified function)
             (default is all variables)
  qname    = name of a Rexx queue to use (default is the current queue)
:exmp.

:h2.RxPullQueue - Pull items off any Rexx queue
:p.This function does what the Rexx "Pull" instruction and "LineIn" functions
do, except this function allows you to specify the queue name as part of the
call, regardless of what the current queue may be.
:xmp.
Syntax:

  data = RxPullQueue( [qname] [,type] [,rexxvar] )

where:

  qname   = name of Rexx queue to pull from (default is current queue)
  type    = 'Wait' or 'Nowait' (default is 'Wait')
  rexxvar = name of a Rexx variable into which is put the queue-insertion-
            timestamp for the retreived element
  data    = data obtained from queue
:exmp.

:h2.RxGlobalVar - Put,Get,Delete system-wide global variables
:p.This function manages a global variable pool (among all processes).  It
implements the pool using named Rexx queues (lazy but effective).  Each
global variable is just a queue whose name is derived from the variable's
name and which contains just one element (the variable's value).  The
queue name is just the variable name bracketed by "RXUGV".  For example,
the variable name "FRANK" would be implemented with a queue named
"RXUGVFRANKRXUGV".  This obviously presents the possibility of stepping
on queues created for other purposes (that happen to have the same name),
but this isn't likely.
:xmp.
Syntax:

  value = RxGlobalVar(action,varname[,newvalue])

where:

  action   = 'Put'
             'Get'
             'Delete'
  varname  = name of variable to put, get, delete
  newvalue = value for "varname" (put only)
  value    = value of "varname" (get only)
:exmp.

:h2.RxPullQueue - Pull items off any Rexx queue
:p.This function does what the Rexx "Pull" instruction and "LineIn" functions
do, except this function allows you to specify the queue name as part of the
call, regardless of what the current queue may be.
:xmp.
Syntax:

  data = RxPullQueue( [qname] [,type] [,rexxvar] )

where:

  qname   = name of Rexx queue to pull from (default is current queue)
  type    = 'Wait' or 'Nowait' (default is 'Wait')
  rexxvar = name of a Rexx variable into which is put the queue-insertion-
            timestamp for the retreived element
  data    = data obtained from queue
:exmp.

:h2.RxAddQueue - Add items to any Rexx queue
:p.This function does what the Rexx "Queue" and "Push" instructions, and the
"LineOut" function do, except this function allows you to specify the queue
name as part of the call, regardless of what the current queue may be.
:xmp.
Syntax:

  qrc = RxAddQueue( data [,qname] [,type] )

where:

  data  = data to be placed on queue
  qname = name of Rexx queue to add to (default is current queue)
  type  = 'Queue' or 'Push' (default is 'Queue')
:exmp.

:h2.RxQueued - Query number of items on any Rexx queue
:p.This function does what the Rexx "Queued" function does, except this
function allows you to specify the queue name as part of the call, regardless
of what the current queue may be.
:xmp.
Syntax:

  numq = RxQueued( [qname] )

where:

  qname = name of Rexx queue to report on (default is current queue)
:exmp.

:h2.RxQExists - Query existence of a Rexx Queue
:p.This function returns "1" or "0", indicating the existence of the
Rexx Queue whose name you supply as an argument.
:xmp.
Syntax:

  bool = RxQExists( [qname] )

where:

  qname = name of Rexx queue to check for existence of
  bool  = truth value of queue's existence
:exmp.

:h2.RxSearchPath - Find a file in a path
:p.This function does what REXXUTIL's "SysSearchPath" does, but allows
full use of DosSearchPath capabilities.
:xmp.
Syntax:

  info = RxSearchPath(fname,path[,flags])

where:

  fname = name of file to search for (can contain pattern characters)
  path  = either an actual path string, or the name of an environment
          variable containing a path string
  flags = any combination of:
          V - indicates that "path" is an environment variable name,
              rather than an actual path string
          I - ignore network errors when searching through network
              drives
          C - search current directory first
          The default is "Not V, Not I, Not C".
  info  = two blank delimited tokens:
          1) return code from DosSearchPath
          2) If (1) is zero, the fully qualified name of the file found
:exmp.

:h2.RxRegisterFunctionExe - Use "RexxRegisterFunctionExe()"
:p.This function allows you to register an external function for the
current .EXE only.
:xmp.
Syntax:

  rxrc = RxRegisterFunctionExe(funcname,funcaddr)

where:

  funcname = name of external function
  funcaddr = address of function implementation
             (e.g. - could be obtained by RxQueryProcAddr)
  rxrc     = return code from RexxRegisterFunctionExe()
:exmp.

:h2.RxRegisterExitDll - Use "RexxRegisterExitDll()"
:p.This function allows you to register a Rexx exit handler that resides in
a .DLL
:xmp.
Syntax:

  rxrc = RxRegisterExitDll(hname,dllname,procname,udata,dropflag)

where:

  hname    = handler name
  dllname  = name of .DLL where handler code resides
  procname = name of procedure in .DLL that implements handler
  udata    = user data (1-8 bytes)
  dropflag = D - can be dropped
             N - cannot be dropped
  rxrc     = return code from RexxRegisterExitDll()
:exmp.

:h2.RxRegisterExitExe - Use "RexxRegisterExitExe()"
:p.This function allows you to register a Rexx exit handler that resides in
a .EXE
:xmp.
Syntax:

  rxrc = RxRegisterExitExe(hname,funcaddr,udata)

where:

  hname    = handler name
  funcaddr = address of handler function implementation
             (e.g. - could be obtained by RxQueryProcAddr)
  udata    = user data (1-8 bytes)
  rxrc     = return code from RexxRegisterExitExe()
:exmp.

:h2.RxQueryExit - Use "RexxQueryExit()"
:p.This function allows you to query exit information
:xmp.
Syntax:

  rxrc = RxQueryExit(hname,modname)

where:

  hname    = name of handler
  modname  = handler module name
  rxrc     = return code from RexxQueryExit()
:exmp.

:h2.RxDeregisterExit - Use "RexxDeregisterExit()"
:p.This function allows you to de-register a Rexx exit
:xmp.
Syntax:

  rxrc = RxDeregisterExit(hname,modname)

where:

  hname    = name of handler
  modname  = handler module name
  rxrc     = return code from RexxDeregisterExit()
:exmp.

:h1.Memory Management/Access
:p.These functions allow Rexx programs to create, access, and manage OS/2
memory objects by address.  This includes objects such as shared memory
(named and un-named).
:p.Note that for named shared memory, the memory name must begin with
"\sharemem\".  Thus, a valid shared memory name would be "\sharemem\frank"
whereas "\frank" would not be valid.

:h2.RxMalloc - Allocate memory using "malloc()"
:p.This function allocates memory using the 'C' "malloc()" function and
returns the address of this memory.  This will allocate memory faster
and more efficiently than the "RxAllocMem()" function (as a rule).
Whereas the granularity of "RxAllocMem()" is at best a 4k page, the
"rxmalloc()" granularity is much better as a rule.
:p.Memory allocated with "rxmalloc()" must only be freed with "rxfree()"
and NOT with "RxFreeMem()".
:xmp.
Syntax:

  address = RxMalloc(size)

where

  address = 32-bit address of allocated storage
  size    = bytes of storage to allocate
:exmp.

:h2.RxFree - Free memory allocated by "rxmalloc()" using "free()"
:p.This function frees memory using the 'C' "free()" function and
returns the address of this memory.
:p."rxfree()" must only be used to free memory allocated with the
"rxmalloc()" function and NOT to free memory allocated with "RxAllocMem()"
or "RxAllocSharedMem()", etc.
:xmp.
Syntax:

  done = RxFree(address)

where

  address = 32-bit address of storage to be freed
  done    = always '1'
:exmp.

:h2.RxStructMap - Generate a structure map for RxStruct2Stem()
:p.This function builds a "structure map" which can be used by the
function RxStruct2Stem() to map the elements of a structure into a
Rexx stem.  The elements of the structure must be described by the
Rexx program.  This can be used to access the elements of a structure
whose address is passed to the Rexx program by a compiled program.
:xmp.
Syntax:

  map = RxStructMap(stemname)

where

  stemname = name of Rexx stem under which structure is described as such:

             stem.0   = Number of elements in structure (1-n)
             stem.P   = Packing mode of structure (default = 1)
             stem.n.T = Type of element "n" in structure.  Valid
                        values are:
                        l = signed long integer    (4 bytes)
                        L = unsigned long integer  (4 bytes)
                        s = signed short integer   (2 bytes)
                        S = unsigned short integer (2 bytes)
                        d = double                 (8 bytes)
                        D = double                 (8 bytes)
                        c = character array        (stem.n.L bytes)
                        C = character array        (stem.n.L bytes)
             stem.n.L = Length of character array (if type = 'C'
                        or 'c')

  map      = binary structure map used by RxStruct2Stem()
:exmp.

:h2.RxStruct2Stem - Map structure elements into a stem
:p.This function allows a Rexx program to map the elements of a structure
into a Rexx stem.  The structure is referred to by its address.  The
structure is described by the map built with RxStructMap().  This can
be used to access the elements of a structure whose address is passed
to the Rexx program by a compiled program.
:xmp.
Syntax:

  bytes = RxStruct2Stem(stemname, struct, map [,ind])

where

  stemname = name of Rexx stem under which structure elements' values
             are returned, as such:

             stem.0   = Number of elements in structure
             stem.n   = Actual value of element "n" in structure

  struct   = either a 32-bit pointer (address) to base of structure,
             or the actual structure contents (i.e. length of arg 2 is sizeof(struct))
  map      = structure map generated by RxStructMap()
  ind      = 'S' to indicate that "struct" is structure contents, rather than a pointer
             to memory
  bytes    = total number of bytes in structure
:exmp.

:h2.RxStem2Struct - Map a stem into structure elements
:p.This function allows a Rexx program to map a stem variable into
the elements of a structure.  The structure is referred to by its
address.
:xmp.

Syntax:

  bytes = RxStem2Struct(stemname, struct, map [,ind])

where

  stemname = name of Rexx stem under which structure elements' values
             are provided, as such:

             stem.n   = Actual value of element "n" in structure

  struct   = 32-bit pointer (address) to base of structure,
             or the name of a rexx variable whose value will be set with the contents
             of the structure
  map      = structure map generated by RxStructMap()
  ind      = 'V' to indicate that "struct" is the name of a Rexx variable
  bytes    = total number of bytes in structure
:exmp.

:h2.RxStorage - Access/Alter memory by address
:p.This function allows a Rexx program to access and/or alter storage by
address.  This could be used to process data passed to it by a program.  This
could also be used to "Pass By Reference" among Rexx programs when using the
RxAllocMem and RxFreeMem functions.
:xmp.
Syntax:

  outdata = RxStorage( pointer [, length] [, indata] )

where

  pointer  = 32-bit pointer (address)
  length   = Length of storage to be queried (defaults to 1)
  indata   = Data to place at address "pointer" for the length of the data
             (regardless of "length" value)
  outdata  = Data at address "pointer" for length "length" (truncated if
             "length" takes you beyond allowed storage)

Note:

  A request to query memory out-of-bounds returns a string up to the bound-
  ary.  A request to set memory out-of-bounds, sets memory up to the bound-
  ary.  All memory is allocated in 4k chunks.  Therefore, some query or set
  operations will access memory up to the next 4k boundary.
:exmp.

:h2.RxAdd2Ptr - Pointer Arithmetic (Add/Subtract)
:p.This function allows a Rexx program to add or subtract from an OS/2
32-bit linear address.
:xmp.
Syntax:

  newptr = RxAdd2Ptr( pointer , [number] )

where:

  pointer  = 32-bit pointer (address)
  number   = amount by which "pointer" is to be incremented/decremented
             in bytes.  Default is zero.
  newptr   = result of addition/subtraction
:exmp.

:h2.RxThunkAddr - Thunk an address Flat->Segmented, Segmented->Flat
:p.This function allows a Rexx program to "thunk" an address from flat to
segmented, or from segmented to flat (i.e. 32->16 bit or 16->32 bit).
:xmp.
Syntax:

  newptr = RxThunkAddr( pointer , type )

where:

  pointer  = input address
  type     = indicates what to convert input address to:
             S - convert input address from flat to segmented (32->16)
             F - convert input address from segmented to flat (16->32)
  newptr   = thunked address
:exmp.

:h2.RxAllocMem - Allocate memory (using DosAllocMem)
:p.This function allows a Rexx program to allocate memory.  This memory can
then be accessed using RxStorage.
:xmp.
Syntax:

  allocrc = RxAllocMem(rexxvar, length, flags)

where

  rexxvar = name of a rexx variable in which pointer is returned
  length  = a decimal integer indicating amount of storage (in bytes) to
            allocate
  flags   = any combination of:
            C - Pag_Commit
            T - Obj_Tile
            X - Pag_Execute
            R - Pag_Read
            W - Pag_Write
            G - Pag_Guard
  allocrc = return code from DosAllocMem()

for example:
            /* Following would get 4k committed read/write */
  allocrc = RxAllocMem('pointer', 4096, 'crw')

Note:

  All memory is allocated in 4k chunks.  Therefore, even if you allocate 200
  bytes, you will get access to 4096 bytes.
:exmp.

:h2.RxFreeMem - Free memory allocated by RxAllocMem
:p.This function allows a Rexx program to free memory allocated by
RxAllocMem.
:xmp.
Syntax:

  freerc = RxFreeMem(pointer)

where

  pointer - A valid 32 bit pointer.

Note:

  Returned value is return code from DosFreeMem
:exmp.

:h2.RxAllocSharedMem - Allocate shared memory
:p.This function allocates shared (named or un-named) memory.  Other
processes can access this memory with either RxGetSharedMem (un-named), or
RxGetNamedSharedMem (named), or if it is "Given", followed by calls to
RxStorage.
:xmp.
Syntax:

  allocrc = RxAllocSharedMem(rexxvar, length, flags [,memname] )

where

  rexxvar = name of a rexx variable in which pointer is returned
  length  = a decimal integer indicating amount of storage (in bytes) to
            allocate
  flags   = any combination of:
            C - Pag_Commit
            T - Obj_Tile
            I - Obj_Giveable
            E - Obj_Gettable (sic)
            X - Pag_Execute
            R - Pag_Read
            W - Pag_Write
            G - Pag_Guard
  memname = name of shared memory object, if this is to be named shared
            memory
  allocrc = return code from DosAllocSharedMem()

for example:

  /* To allocate an un-named, gettable object */
  allocrc = RxAllocSharedMem('pointer', 4096, 'cerw')

  /* To allocate a named object */
  allocrc = RxAllocSharedMem('pointer', 4096, 'crw','\SHAREMEM\TEST.MEM')

Note:

  All memory is allocated in 4k chunks.  Therefore, even if you allocate 200
  bytes, you will get access to 4096 bytes.
:exmp.

:h2.RxGiveSharedMem - Give access to shared memory
:p.This function gives access to shared, un-named memory to a process-id
:xmp.
Syntax:

  giverc = RxGiveSharedMem(pointer, pid [,flags])

where

  pointer = valid 32-bit pointer
  pid     = valid process-id (as a decimal integer)
  flags   = any combination of:
            X - Pag_Execute
            R - Pag_Read
            W - Pag_Write
            G - Pag_Guard

for example:

  /* To give shared memory */
  giverc = RxGiveSharedMem(pointer, 23, 'rw')

Note:

  A valid pointer must be supplied.  This is obtained from the process which
  allocated the shared un-named memory via RxAllocSharedMem.  The process-id
  supplied must be valid (i.e. must currently exist).
:exmp.

:h2.RxGetSharedMem - Get access to shared memory
:p.This function gets access to shared, un-named, gettable memory.
:xmp.
Syntax:

  getrc = RxGetSharedMem(pointer, flags)

where

  pointer = valid 32-bit pointer
  flags   = any combination of:
            X - Pag_Execute
            R - Pag_Read
            W - Pag_Write
            G - Pag_Guard

for example:

  /* To get shared memory */
  getrc = RxGetSharedMem(pointer, 'rw')

Note:

  A valid pointer must be supplied.  This is obtained from the process which
  allocated the shared un-named memory via RxAllocSharedMem.
:exmp.

:h2.RxGetNamedSharedMem - Get access to named shared memory
:p.This function gets access to shared, named memory.
:xmp.
Syntax:

  getrc = RxGetNamedSharedMem(rexxvar, memname, flags)

where

  rexxvar = name of a rexx variable in which pointer is returned
  memname = valid name of a shared memory object
  flags   = any combination of:
            X - Pag_Execute
            R - Pag_Read
            W - Pag_Write
            G - Pag_Guard
  getrc   = return code from DosGetNamedSharedMem()

for example:

  /* To get named shared memory */
  getrc = RxGetNamedSharedMem('pointer', '\SHAREMEM\TEST.MEM', 'rw')
:exmp.

:h2.RxSetMem - Set memory attributes
:p.This function sets memory attributes using DosSetMem
:xmp.
Syntax:

  setrc = RxSetMem(pointer [,memsize [,flags]])

where

  pointer = valid 32-bit pointer
  memsize = size of memory block to set
  flags   = any combination of:
            C - Pag_Commit
            D - Pag_Decommit
            F - Pag_Default
            X - Pag_Execute
            R - Pag_Read
            W - Pag_Write
            G - Pag_Guard

for example:

  /* To set memory attributes */
  setrc = RxSetMem(pointer, 4096, 'crw')

Note:

  Returned value is return code from DosSetMem.
:exmp.

:h2.RxQueryMem - Query memory attributes
:p.This function queries memory attributes using DosQueryMem
:xmp.
Syntax:

  qinfo = RxQueryMem(pointer [,memsize])

where

  pointer = valid 32-bit pointer
  memsize = size of memory block to query
  qinfo   = either one or three blank-delimited tokens:
            1) return code from DosQueryMem
               If #1 is zero:
                 2) actual memory size
                 3) memory attribute flags, which can be any combination of:
                    C - Pag_Commit
                    F - Pag_Free
                    S - Pag_Shared
                    B - Pag_Base
                    X - Pag_Execute
                    R - Pag_Read
                    W - Pag_Write
                    G - Pag_Guard

for example:

  /* To query memory attributes */
  qinfo = RxQueryMem(pointer, 4096)
  /* "Say qinfo" might say -> "0 4096 CSRW" for instance */

Note:

  If first token is not zero, no other information was returned.
:exmp.

:h2.RxSubSetMem - Set memory for suballocation
:p.This function sets memory for suballocation using DosSubSetMem
:xmp.
Syntax:

  subsetrc = RxSubSetMem(pointer [,memsize [,flags]])

where

  pointer = valid 32-bit pointer to pool
  memsize = size of memory block to subset
  flags   = any combination of:
            I - DosSub_Init
            G - DosSub_Grow
            S - DosSub_Sparse
            R - DosSub_Serialize

for example:

  /* To subset some memory */
  subsetrc = RxSubSetMem(pointer, 4096, 'i')

Note:

  Returned value is return code from DosSubSetMem.
:exmp.

:h2.RxSubUnSetMem - UnSet memory from suballocation
:p.This function unsets memory from suballocation using DosSubUnSetMem
:xmp.
Syntax:

  subunsetrc = RxSubUnSetMem(pointer)

where

  pointer = valid 32-bit pointer to pool

for example:

  /* To subunset some memory */
  subunsetrc = RxSubUnSetMem(pointer)

Note:

  Returned value is return code from DosSubUnSetMem.
:exmp.

:h2.RxSubAllocMem - Suballocate memory
:p.This function suballocates memory.
:xmp.
Syntax:

  subarc = RxSubAllocMem(rexxvar, pointer [,memsize])

where

  rexxvar = name of a rexx variable in which block offset is returned
  pointer = valid 32-bit pointer to pool
  memsize = size of block to suballocate
  subarc  = return code from DosSubAllocMem()

for example:

  /* To suballocate some memory */
  subarc = RxSubAllocMem('block_offset', pointer, 4096)
:exmp.

:h2.RxSubFreeMem - Free suballocated memory
:p.This function frees suballocated memory.
:xmp.
Syntax:

  freerc = RxSubFreeMem(pointer, boffset [,memsize])

where

  pointer = valid 32-bit pointer to pool
  boffset = block offset pointer
  memsize = size of block to free

for example:

  /* To free suballocated memory */
  freerc = RxSubFreeMem(pointer, boffset, 4096)

Note:

  Returned value is return code from DosSubFreeMem.
:exmp.

:h1.Semaphore handling
:p.These functions allow Rexx programs to make use of OS/2 semaphores for
synchronization/serialization purposes.
:p.Note that for named semaphores (event, mutex, or mux) the semaphore
name must begin with "\sem32\".  Thus, a valid semaphore name would be
"\sem32\frank" whereas "\frank" would not be valid.

:h2.RxCreateEventSem - Create an event semaphore
:p.This function creates a (named or un-named) event semaphore.
:xmp.
Syntax:

  semrc = RxCreateEventSem(rexxvar [,type] [,name] [,state] )

where

  rexxvar = name of a rexx variable in which sem handle is returned
  type    = 'Shared' or 'Private' (default is Private)
  name    = semaphore name (default is un-named) (must begin with "\sem32\")
  state   = 'Reset' or 'Posted' (default is Reset)
  semrc   = return code from DosCreateEventSem()
:exmp.

:h2.RxOpenEventSem - Open an event semaphore
:p.This function opens an event semaphore for use by a process that did not
create the semaphore.
:xmp.
Syntax:

  semrc = RxOpenEventSem(rexxvar, semid)

where

  rexxvar = name of a rexx variable in which sem handle is returned
  semid   = either a semaphore handle, or a semaphore name
  semrc   = return code from DosOpenEventSem()

Note:

  A zero length string is returned if DosOpenEventSem fails.
:exmp.

:h2.RxPostEventSem - Post an event semaphore
:p.This function posts an event semaphore.
:xmp.
Syntax:

  postrc = RxPostEventSem(hev)

where

  hev    = semaphore handle
  postrc = return code from DosPostEventSem
:exmp.

:h2.RxQueryEventSem - Query an event semaphore
:p.This function returns the post count for an event semaphore.
:xmp.
Syntax:

  info = RxQueryEventSem(hev)

where

  hev    = semaphore handle
  info   = return code from DosQueryEventSem, followed by number of posts
Note:

  Word 1 of info is the return code from DosQueryEventSem
  Word 2 of info is the actual number of posts to the semaphore
:exmp.

:h2.RxResetEventSem - Reset an event semaphore
:p.This function resets an event semaphore, returning the number of postings
for that semaphore.
:xmp.
Syntax:

  info = RxResetEventSem(hev)

where

  hev    = semaphore handle
  info   = return code from DosResetEventSem, followed by number of posts

Note:

  Word 1 of info is the return code from DosResetEventSem
  Word 2 of info is the actual number of posts to the semaphore before
                    the Reset.
:exmp.

:h2.RxWaitEventSem - Wait on an event semaphore
:p.This function waits on the posting of an event semaphore.
:xmp.
Syntax:

  semhandle /* A valid semaphore handle */
  time = 1000   /* in milliseconds */

  waitrc = RxWaitEventSem(hev [,time])

where

  hev    = semaphore handle
  time   = number of miliseconds to wait before returning, or the word
           'Indefinite'.  Default is 'Indefinite'.
  waitrc = return code from DosWaitEventSem
:exmp.

:h2.RxCloseEventSem - Close an event semaphore
:p.This function closes an event semaphore
:xmp.
Syntax:

  closerc = RxCloseEventSem(hev)

where

  hev     = semaphore handle
  closerc = return code from DosCloseEventSem
:exmp.

:h2.RxCreateMutexSem - Create a Mutex Semaphore
:p.This function creates a mutex semaphore
:xmp.
Syntax:

  semrc = RxCreateMutexSem(rexxvar [,type] [,name] [,state] )

where

  rexxvar = name of a rexx variable in which sem handle is returned
  type    = 'Shared' or 'Private' (default is 'Private')
  name    = semaphore name (default is un-named) (must begin with "\sem32\")
  state   = 'Owned' or 'Unowned' (default is un-owned)
  semrc   = return code from DosCreateMutexSem()
:exmp.

:h2.RxOpenMutexSem - Open a Mutex Semaphore
:p.This function opens a mutex semaphore
:xmp.
Syntax:

  semrc = RxOpenMutexSem(rexxvar, semid )

where

  rexxvar = name of a rexx variable in which sem handle is returned
  semid   = either a semaphore name, or an existing mutex semaphore handle
  semrc   = return code from DosOpenMutexSem()
:exmp.

:h2.RxCloseMutexSem - Close a Mutex Semaphore
:p.This function closes a mutex semaphore
:xmp.
Syntax:

  mtxrc = RxCloseMutexSem( hmtx )

where

  hmtx  = existing mutex semaphore handle
  mtxrc = return code from DosCloseMutexSem
:exmp.

:h2.RxQueryMutexSem - Query Mutex Semaphore
:p.This function queries a mutex semaphore's attributes
:xmp.
Syntax:

  mtxinfo = RxQueryMutexSem( hmtx )

where

  hmtx    = existing mutex semaphore handle
  mtxinfo = four blank delimited tokens:
            1) return code from DosQueryMutexSem
            2) Process-Id of semaphore's current owner
            3) Thread-Id of semaphore's current owner
            4) A count of the number of calls to DosRequestMutexSem,
               minus the number of calls to DosReleaseMutexSem, that have
               been made for the semaphore by the owning thread.
               If the semaphore is unowned, this value will be zero.
               If the owning thread has ended, the value will be the
               request count for the ended owner.
:exmp.

:h2.RxReleaseMutexSem - Release Mutex Semaphore
:p.This function releases a mutex semaphore
:xmp.
Syntax:

  mtxrc = RxReleaseMutexSem( hmtx )

where

  hmtx  = existing mutex semaphore handle
  mtxrc = return code from DosReleaseMutexSem
:exmp.

:h2.RxRequestMutexSem - Request Mutex Semaphore
:p.This function requests a mutex semaphore
:xmp.
Syntax:

  mtxrc = RxRequestMutexSem( hmtx [, timeout] )

where

  hmtx    = existing mutex semaphore handle
  timeout = either a decimal integer (miliseconds to block), or the word
            'Indefinite'
  mtxrc   = return code from DosRequestMutexSem
:exmp.

:h2.RxCreateMuxWaitSem - Create a MuxWait Semaphore
:p.This function creates a muxwait semaphore
:xmp.
Syntax:

  semrc = RxCreateMuxWaitSem(rexxvar, stemname [,type] [,anyall]
                             [,name] )

where

  rexxvar  = name of a rexx variable in which sem handle is returned
  stemname = a Rexx stem name (ending in a period '.'), under which are
             supplied the semaphore handles which constitute this muxwait
             semaphore.  The expected structure of the stem is:
             1) stem.0   = a decimal integer indicating how many semaphores
                           are supplied
             2) stem.n.1 = handle of Nth semaphore
             3) stem.n.2 = decimal integer used as an Id for Nth semaphore
  type     = 'Shared' or 'Private' (default is 'Private')
  anyall   = 'anY' or 'alL' (default is 'All')
  name     = semaphore name (default is un-named) (must begin with "\sem32\")
  semrc    = return code from DosCreateMuxWaitSem()
:exmp.

:h2.RxOpenMuxWaitSem - Open a MuxWait Semaphore
:p.This function opens a muxwait semaphore
:xmp.
Syntax:

  semrc = RxOpenMuxWaitSem(rexxvar, semid )

where

  rexxvar = name of a rexx variable in which sem handle is returned
  semid   = either a semaphore name, or an existing muxwait
            semaphore handle
  semrc   = return code from DosOpenMuxWaitSem()
:exmp.

:h2.RxCloseMuxWaitSem - Close a MuxWait Semaphore
:p.This function closes a muxwait semaphore
:xmp.
Syntax:

  muxrc = RxCloseMuxWaitSem( hmux )

where

  hmux  = existing muxwait semaphore handle
  muxrc = return code from DosCloseMuxWaitSem
:exmp.

:h2.RxDeleteMuxWaitSem - Delete from a MuxWait Semaphore
:p.This function deletes a semaphore from a muxwait semaphore list
:xmp.
Syntax:

  muxrc = RxDeleteMuxWaitSem( hmux, hsem )

where

  hmux  = existing muxwait semaphore handle
  hsem  = handle of a semaphore in the muxwait semaphore list
  muxrc = return code from DosDeleteMuxWaitSem
:exmp.

:h2.RxAddMuxWaitSem - Add to a MuxWait Semaphore
:p.This function adds a semaphore to a muxwait semaphore list
:xmp.
Syntax:

  muxrc = RxAddMuxWaitSem( hmux, hsem, semid )

where

  hmux  = existing muxwait semaphore handle
  hsem  = handle of a semaphore to add to the muxwait semaphore list
  semid = decimal integer used as an Id for this semaphore
  muxrc = return code from DosAddMuxWaitSem
:exmp.

:h2.RxQueryMuxWaitSem - Query a MuxWait Semaphore
:p.This function queries information about a muxwait semaphore
:xmp.
Syntax:

  muxinfo = RxQueryMuxWaitSem( hmux, stemname )

where

  hmux     = existing muxwait semaphore handle
  stemname = a Rexx stem name (ending in a period '.'), under which is
             returned information about the muxwait semaphore.  The structure
             of the returned information is:
             1) stem.0   = a decimal integer indicating how many semaphores
                           are listed in this muxwait semaphore
             2) stem.n.1 = handle of Nth semaphore
             3) stem.n.2 = decimal integer Id of Nth semaphore
  muxinfo  = Two blank delimited tokens:
             1) return code from DosQueryMuxWaitSem
             2) a string of letters indicating this muxwait semaphore's
                properties:
                'S' if it's shared
                'Y' if it's a wait ANY
                'L' if it's a wait ALL
:exmp.

:h2.RxWaitMuxWaitSem - Wait on a MuxWait Semaphore
:p.This function waits on a muxwait semaphore
:xmp.
Syntax:

  muxinfo = RxWaitMuxWaitSem( hmux [,timeout] )

where

  hmux     = existing muxwait semaphore handle
  timeout  = either a decimal integer (miliseconds to block), or the word
            'Indefinite'
  muxinfo  = Two blank delimited tokens:
             1) return code from DosQueryMuxWaitSem
             2) semaphore-id of releaser of muxwait sem (for more info,
                see details of DosWaitMuxWaitSem)
:exmp.

:h1.Rexx Macro Space Functions
:p.These functions allow Rexx programs to query and alter the Rexx MacroSpace.

:h2.RxAddMacro - Add Entry to MacroSpace
:p.This function adds a Rexx program to the macrospace.
:xmp.
Syntax:

  addrc = RxAddMacro(funcname, sourcefile [,order])

where

  funcname   = name for function in macrospace
  sourcefile = name of source file where Rexx source code exists
  order      = order in macrospace (Before or After)

Note:

  Returned value is return code from RexxAddMacro.
:exmp.

:h2.RxDropMacro - Drop Entry from MacroSpace
:p.This function drops a function from the macrospace.
:xmp.
Syntax:

  droprc = RxDropMacro(funcname)

where

  funcname   = name of function in macrospace

Note:

  Returned value is return code from RexxDropMacro.
:exmp.

:h2.RxClearMacroSpace - Clear Entire MacroSpace
:p.This function clears all functions from the macro space.  Be careful when
using this as it will affect all processes in the system.
:xmp.
Syntax:

  clear_rc = RxClearMacroSpace()

Note:

  Returned value is return code from RexxClearMacroSpace.
:exmp.

:h2.RxSaveMacroSpace - Save Macro to File
:p.This function saves a macrospace function to a file.
:xmp.
Syntax:

  saverc = RxSaveMacroSpace(funclist, macfile)

where

  funclist = blank delimited list of function names to save from macrospace
  macfile  = name of file in which to save functions from macrospace

Note:

  Returned value is return code from RexxSaveMacroSpace.
:exmp.

:h2.RxLoadMacroSpace - Load a Macro from File
:p.This function loads a macrospace function from a file.
:xmp.
Syntax:

  loadrc = RxLoadMacroSpace(funcist, macfile)

where

  funclist = blank delimited list of function names to load into macrospace
  macfile  = name of macrospace file from which to load functions

Note:

  Returned value is return code from RexxLoadMacroSpace.
:exmp.

:h2.RxQueryMacro - Query MacroSpace
:p.This function queries the existence/position of a macro in the macrospace.
:xmp.
Syntax:

  qinfo = RxQueryMacro(funcname)

where

  funcname = name of function in macrospace
  qinfo    = blank-delimited tokens:
             1) return code from RexxQueryMacro
                If #1 is zero, then:
                2) Before/After/?

Note:

  If first token is not zero, no other information was returned.
:exmp.

:h2.RxReorderMacro - Reorder Macro Position
:p.This function reorders the position of a macro within the macrospace.
:xmp.
Syntax:

  reorderrc = RxReorderMacro(funcname, position)

where

  funcname = name of function in macrospace
  position = relative position in macrospace (Before/After)

Note:

  Returned value is return code from RexxReorderMacro
:exmp.

:h1.Threads, Tasking, etc.
:p.These functions allow a Rexx program to start and control new threads and
processes.

:h2.RxWaitChild - Wait for child process(es) to end
:p.This function will wait for child process(es) to end
:xmp.
Syntax:

  dosrc = RxWaitChild(childpid,actioncode,waitoption)

where

  childpid   = PID of child process (decimal number)
  actioncode = kill single process, or a process tree:
               P - single process
               T - process tree
  waitoption = whether or not to wait for child to end:
               W - wait
               N - no wait
  dosrc      = four blank-delimited tokens:
               1) return code from DosWaitChild
               if (1) is zero, then
                 2) returned PID
                 3) result code from terminating process
                 4) text indicating type of child termination:
                    - NormalExit
                    - HardError
                    - Trap
                    - KillProcess
                    - Exception
:exmp.

:h2.RxExecPgm - Execute a program
:p.This function starts another program in the same session.

:xmp.
Syntax:

  exrc = RxExecPgm(pgmname [, mode [,argstring]])

where

  pgmname   = name of .EXE file to execute
  mode      = 'S'ync, 'A'sync, async'R'esult, 'T'race, 'B'ackground, 'L'oad,
              asyncresult'D'b.
              Default is 'S'ync.
  argstring = arg string to pass to program
              typically, to recreate the argument string 'C' programs expect from
              the command line, the arguments should have the form:
                '00'x || argstring ||'0000'x
  exrc      = three blank-delimited tokens:
              1) return code from DosExecPgm
              2) termination code / process-id (for asynchronous)
              3) new program's return code
:exmp.

:h2.RxStartSession - Start Another Session
:p.This function starts a program in another OS/2 session.

:xmp.
Syntax:

  strc = RxStartSession(pgmname [,args [,related [,fgbg [,title [,type
                        [,ctrl] ] ] ] ] ])

where

  pgmname   = name of .EXE file to execute
                or
              32-bit pointer to populated STARTDATA structure used by DosStartSession
  args      = arg string to pass to program
  related   = Relation of started session to this session, can be:
              'I'ndependent or 'C'hild
  fgbg      = Foreground/Background option, can be:
              'F'oreground or 'B'ackground
  title     = Session title (defaults to program name)
  type      = Program type, can be:
              'D'efault, 'F'ullscreen, 'P'M, 'V'irtual-Dos,
              'W'indowed-Virtual-Dos, vi'O'-windowable
  ctrl      = Program control, can be:
              'V'isible, 'I'nvisible, ma'X'imized, mi'N'imized,
              no'A'utoclose
  strc      = if DosStartSession failed:
              - return code from DosStartSession followed by error info
              if DosStartSession got zero rc:
              - return code from DosStartSession Session-ID Process-ID
:exmp.

:h2.RxStartRexxSession - Start a Rexx program on another session
:p.This function starts a Rexx program in another OS/2 session.
(The program "RXSRS.EXE" must be in your path in order to use this function)

:xmp.
Syntax:

  strc = RxStartRexxSession(execname [,arg1] [,arg2] ... [,argn])

where

  execname = name of Rexx program to execute
                   or
             a '$' immediately preceding a source program string
                   or
             a '&amp.' immediately preceding a tokenized program string
             (for more info about these, see "RxCallInStore" and
              "RxTokenize")
             a '+' immediately preceding the name of a program in the macrospace
  arg1     = arguments 1-n for target Rexx program
  ...
  argn
  strc     = if DosStartSession failed:
             - return code from DosStartSession followed by error info
             if DosStartSession worked:
             - return code from DosStartSession Session-ID Process-ID
:exmp.

:h2.RxDetachRexxPgm - Detach a Rexx program
:p.This function starts a Rexx program in the detached OS/2 session.
(The program "RXSRS.EXE" must be in your path in order to use this function)

:xmp.
Syntax:

  dtrc = RxDetachRexxPgm(execname [,arg1] [,arg2] ... [,argn])

where

  execname = name of Rexx program to execute
                   or
             a '$' immediately preceding a source program string
                   or
             a '&amp.' immediately preceding a tokenized program string
             (for more info about these, see "RxCallInStore" and
              "RxTokenize")
             a '+' immediately preceding the name of a program in the macrospace
  arg1     = arguments 1-n for target Rexx program
  ...
  argn
  dtrc     = if DosExecPgm failed:
             - return code from DosExecPgm, followed by error info
             if DosExecPgm worked:
             - return code from DosExecPgm "codeTerminate" and "codeResult"
:exmp.

:h2.RxKillProcess - Kill an OS/2 Process
:p.This function kills an OS/2 process by process-id.

:xmp.
Syntax:

  killrc = RxKillProcess(pid [, action])

where

  pid    = process-id (decimal integer)
  action = 'Process' or 'Tree' (default is 'Process')
  killrc = return code from DosKillProcess
:exmp.

:h2.RxCreateThread - Start new thread
:p.This function starts execution at a supplied entry point on a new thread.
The entry point is any valid procedure address (e.g. obtained by a call to
RxQueryProcAddr, etc.).
:xmp.
Syntax:

  info = RxCreateThread(procaddr [,dataptr] [,linktype] [,stacksize] [,threadflag] )

where

  procaddr   = address of function to start on new thread
  dataptr    = optional, pointer to argument
  linktype   = optional, S = _System (default = _System)
                         O = _Optlink
  stacksize  = optional, stacksize for thread (default = 8k)
  threadflag = optional, I = Immediate (default = Immediate) (ignored for linktype = 'O')
                         S = Suspend
  info       = two blank delimited values:
               1) return code from DosCreateThread (linktype = 'S')
                                   _beginthread    (linktype = 'O')
               2) thread-id of newly created thread (if return code was zero)

Note:
  Pointer to argument is optional, but if supplied must be a valid pointer.
:exmp.

:h2.RxKillThread - Kill a thread
:p.This function kills a thread by thread-id.
:xmp.
Syntax:

  killrc = RxKillThread(tid)

where

  tid    = thread-id
  killrc = return code from DosKillThread
:exmp.

:h2.RxSetPriority - Set the priority of processes or threads
:p.This function sets the priority of one or more process and/or
threads.
:xmp.
Syntax:

  setrc = RxSetPriority(scope,class,delta,id)

where

  scope = 'P'rocess (default)
          t'R'ee
          'T'hread
  class = 'N'ochange (default)
          'I'dletime
          'R'egular
          'T'imecritical
          foreground'S'erver
  delta = any integer from -31 to +31, or ma'X'imum, or mi'N'imum
  id    = a Process-Id, Thread-Id, or nothing, depending on value
          of "scope"
  setrc = return code from DosSetPriority
:exmp.

:h2.RxResumeThread - Resume thread execution
:p.This function causes a thread to resume execution
:xmp.
Syntax:

  resrc = RxResumeThread(tid)

where

  tid   = thread-id
  resrc = return code from DosResumeThread
:exmp.

:h2.RxSuspendThread - Suspend thread execution
:p.This function causes a thread to suspend execution
:xmp.
Syntax:

  susrc = RxSuspendThread(tid)

where

  tid   = thread-id
  susrc = return code from DosSuspendThread
:exmp.

:h2.RxCreateRexxThread - Rexx on another thread
:p.This function executes a Rexx program on another thread of the same
process.  The started Rexx program has a separate variable pool, and maintains
its own "current Rexx queue", indpendently of the starting Rexx program.  But
all other features of multi-threaded applications apply (i.e. shared file
handles, etc.)
:xmp.
Syntax:

  tid = RxCreateRexxThread(execname [,arg1 [,arg2 [, ...]]])

where

  execname = name of Rexx program to execute
                   or
             a '$' immediately preceding a source program string
                   or
             a '&amp.' immediately preceding a tokenized program string
             (for more info about these, see "RxCallInStore" and
              "RxTokenize")
                   or
             a '+' immediately preceding the name of a program in the macrospace
  arg1     = arguments to Rexx program (up to 19 possible)
  arg2
  ...
  tid      = Thread-Id of new thread (Rexx integer)

Note:

  Example of executing a program string:

    crlf = '0d0a'x
    str = '/* Rexx */'crlf
    str = str 'Do i=1 To Arg()'crlf
    str = str '  Say "Arg#"i "= {"arg(i)"}"'crlf
    str = str 'End'crlf
    str = str||'1a'x       /* EOF marker */
    call rxcreaterexxthread '$'str, date(), time()
:exmp.

  
:h2.RxSetExceptionExit - Specify the address of exception exit
                          which is called when an OS/2 exception occurs on a
                          thread started with "RxCreateRexxThread".
:p.This function specifies the address of a "_System" linkage function which is
to be called when an OS/2 exception occurs on a thread which was started with
"RxCreateRexxThread".
:xmp.
Syntax:

  rslt = RxSetExceptionExit(pfn)

where

  pfn     = 32-bit pointer to a _System linkage exception (i.e. a four byte
            binary value)
  rslt    = empty string
:exmp.

:h1.NETBIOS information
:p.These functions allow a Rexx program to access certain NETBIOS information.

:h2.RxNbSessionStatus - NetBios Session Status
:p.This function gets NetBios session status information.
:xmp.
Syntax:

  nbname = 'S$SQLDBSERVR1#DB'

  nbrc =  RxNbSessionStatus(nbname,'s.','r')

Note:

  Return code is from NETBIOS api.  Second argument is name of a Rexx stem
  variable under which results are returned as such:

    stem.0 = number of sessions reported on
      stem.n.1 = Local session number
      stem.n.2 = Session state
      stem.n.3 = Local name
      stem.n.4 = Remote name
      stem.n.5 = Number of receive commands pending
      stem.n.6 = Number of send commands pending

  Third argument may be supplied to have a NETBIOS "Reset" done before and
  after obtaining session status.
:exmp.

:h1.DLL Handling
:p.These functions allow a Rexx program to load and execute procedures with-
in DLL's.  Also, certain information about DLL's can be obtained.

:h2.RxLoadModule - Load a DLL
:p.This function loads a DLL, returning a module handle (or an error code).
:xmp.
Syntax:

  dosrc = RxLoadModule(rexxvar, dllname)

where:
 
  rexxvar = name of rexx variable in which module handle is returned
  dllname = name of DLL to load
  dosrc   = return code from DosLoadModule
:exmp.

:h2.RxFreeModule - Free a DLL
:p.This function frees the DLL whose module handle is supplied.
:xmp.
Syntax:

  dosrc = RxFreeModule(hmod)

where:
 
  hmod    = module handle
  dosrc   = return code from DosFreeModule
:exmp.

:h2.RxQueryModuleName - Get Module Name
:p.This function returns the name of a module, given the module handle.
:xmp.
Syntax:

  dosrc = RxQueryModuleName(rexxvar,hmod)

where:
 
  rexxvar = name of rexx variable in which module name is returned
  hmod    = module handle of DLL
  dosrc   = return code from DosQueryModuleName
:exmp.

:h2.RxQueryModuleHandle - Get Module Handle
:p.This function returns the handle of a module, given the module's name.
:xmp.
Syntax:

  dosrc = RxQueryModuleHandle(rexxvar,modname)

where:
 
  rexxvar = name of rexx variable in which module handle is returned
  modname = name of DLL
  dosrc   = return code from DosQueryModuleHandle
:exmp.

:h2.RxQueryProcType - Query Procedure Type
:p.This function returns the addressing mode (16/32 bit) of a procedure of a
DLL.  The procedure can be identified either by name, or ordinal number.
:xmp.
Syntax:

  entryname = 'SQLEXEC'
  ordinal = 3

  procinfo = RxQueryProcType(hmod,procid)
  amode = RxQueryProcType(hmod,entryname)

where:
 
  hmod     = module handle
  procid   = procedure-id (either an ordinal, or a procedure name)
  procinfo = Two blank-delimited tokens:
             1) return code from DosQueryProcType
             2) either "16" or "32", indicating 16 or 32 bit
:exmp.

:h2.RxQueryProcAddr - Query Procedure Address
:p.This function returns the address of an entry point in a DLL.  This entry
point can then be used in RxCallEntryPoint to call an entry point in a Loaded
DLL.
:xmp.
Syntax:

  entryname = 'SQLEXEC' /* Entry name */
              3         /* Entry ordinal */

  dosrc = RxQueryProcAddr(rexxvar,hmod,procid)

where:
 
  rexxvar = name of rexx variable in which procedure address is returned
  hmod    = module handle of DLL
  procid  = procedure-id (either an ordinal, or a procedure name)
:exmp.

:h2.RxCallEntryPoint - Call an entry point
:p.This function "calls" the code which begins at the ProcAddress supplied.
This ProcAddress could be the address obtained by RxQueryProcAddr for the
entry point of a DLL.  But, it can be any "legal" entry point address of any
routine.
:xmp.
Syntax:

  procrc = RxCallEntryPoint(procaddr [,arg1 [, arg2 [,...]]] )

where

  procaddr = proc-address (could be obtained from RxQueryProcAddr, etc.)
  parg1
  ...
  parg19   = up to 19 arguments.  If an argument is a valid pointer, the
             pointer will be passed.  If not, a copy of the argument's value
             will be passed by reference.  Either way, the procedure must be
             expecting a pointer for each argument.
  procrc   = return code from the procedure

Note:
  Linkage to the proc-address is type _System.
:exmp.

:h1.OS/2 Pipes
:p.An assortment of OS/2 pipe-related functions (not complete).

:h2.RxCreateNPipe - Create a named pipe
:p.This function creates an OS/2 named pipe.

:xmp.
Syntax:

  dosrc = RxCreateNPipe( rexxvar, pipename, openmode, pipemode,
                         instance_count, outbufsize, inbufsize,
                         timeout)

where

   rexxvar  = name of Rexx variable into which read/write handle is placed
   pipename = name of pipe (must begin with "\pipe\")

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

   Instance_Count
     Integer

   Outbuf Size
     Integer

   Inbuf Size
     Integer

   Timeout
     Integer

  dosrc   = return code from DosCreateNPipe()
:exmp.
  
:h2.RxSetNPHState - Set state of a named pipe
:p.This function sets the state of an OS/2 named pipe

:xmp.
Syntax:

  dosrc = RxSetNPHState(pipehandle, stateflags)

where

  pipehandle = handle of pipe
  stateflags = string (case sensitive) consisting of a (valid) combination of:
               W - wait (blocking)
               w - no-wait (non-blocking)
               B - byte mode pipe
               M - message mode pipe
  dosrc   = return code from DosSetNPHState()
:exmp.

:h2.RxConnectNPipe - Connect to a named pipe
:p.This function connects to a named pipe

:xmp.
Syntax:

  dosrc = RxConnectNPipe(hpipe)

where

   hpipe  = pipe handle
   dosrc  = return code from DosConnectNPipe()
:exmp.

:h2.RxDisConnectNPipe - Disconnect from a named pipe
:p.This function disconnects from a named pipe

:xmp.
Syntax:

  dosrc = RxDisConnectNPipe(hpipe)

where

   hpipe  = pipe handle
   dosrc  = return code from DosDisConnectNPipe()
:exmp.

:h2.RxCreatePipe - Create an un-named pipe
:p.This function creates an OS/2 un-named pipe

:xmp.
Syntax:

  dosrc = RxCreatePipe(rexxstem [,pipesize])

where

   rexxstem = name of Rexx stem variable under which read handle,
              write handle, and pipe size are returned as such:
              stem.1 = read handle
              stem.2 = write handle
              stem.3 = pipe size
   dosrc  = return code from DosCreatePipe()
:exmp.

:h2.RxDestroyPipe - Destroy an un-named pipe
:p.This function destroys an OS/2 un-named pipe

:xmp.
Syntax:

  dosrc = RxDestroyPipe(hread,hwrite)

where

   hread  = read handle
   hwrite = write handle
   dosrc  = return codes from DosClose()
:exmp.

:h1.OS/2 Queues
:p.Most of the OS/2 queue APIs are implemented here.

:h2.RxCreateQueue - Create an OS/2 queue.
:p.This function creates an OS/2 queue.

:xmp.
Syntax:

  dosrc = RxCreateQueue(rexxvar, qname, qflags)

where

   rexxvar  = name of Rexx variable into which queue handle is placed
   qname    = name of OS/2 queue (must begin with "\queues\")
   qflags   = flags for queue creation:
              F - Fifo
              L - Lifo
              P - Priority
              C - Convert Address
              N - No-convert Address
  dosrc   = return code from DosCreateQueue()
:exmp.

:h2.RxOpenQueue - Open an OS/2 queue
:p.This function opens an OS/2 queue

:xmp.
Syntax:

  dosrc = RxOpenQueue(rexxstem,qname)

where

   rexxstem = name of Rexx stem, under which info is returned:
              stem.1 = queue-owner-pid (integer)
              stem.2 = queue handle
   qname    = name of queue to open
   dosrc    = return code from DosOpenQueue()
:exmp.

:h2.RxPeekQueue - Peek at an OS/2 queue
:p.This function peeks at an OS/2 queue without reading it

:xmp.
Syntax:

  dosrc = RxPeekQueue(rexxstem,qhandle,elemcode,waitcode,semhandle)

where

   rexxstem  = name of Rexx stem, under which info is returned:
               stem.1 = data pointer (e.g. for use with RxStorage)
               stem.2 = data length (of that pointed to by stem.1)
               stem.3 = PID of process which enqueued element
               stem.4 = event code (RequestData.ulData)
               stem.5 = element code
               stem.6 = element priority
   qhandle   = handle to queue
   elemcode  = element code (integer)
   waitcode  = whether to wait for data in queue
               W = Wait
               N = Nowait
   semhandle = handle of semaphore
   dosrc     = return code from DosPeekQueue()
:exmp.

:h2.RxReadQueue - Read an OS/2 queue
:p.This function reads an element off an OS/2

:xmp.
Syntax:

  dosrc = RxReadQueue(rexxstem,qhandle,elemcode,waitcode,semhandle)

where

   rexxstem  = name of Rexx stem, under which info is returned:
               stem.1 = data pointer (e.g. for use with RxStorage)
               stem.2 = data length (of that pointed to by stem.1)
               stem.3 = PID of process which enqueued element
               stem.4 = event code (RequestData.ulData)
               stem.5 = element priority
   qhandle   = handle to queue
   elemcode  = element code (integer)
   waitcode  = whether to wait for data in queue
               W = Wait
               N = Nowait
   semhandle = handle of semaphore
   dosrc     = return code from DosReadQueue()
:exmp.

:h2.RxWriteQueue - Write to an OS/2 queue
:p.This function writes an element to an OS/2

:xmp.
Syntax:

  dosrc = RxWriteQueue(qhandle,pointer,length,reqdata,elemprty)

where

   qhandle   = handle to queue
   pointer   = whatever 32 bits you want to put on queue
   length    = integer length associated with "pointer"
   reqdata   = integer request data
   elemprtry = element priority, if this is a priority queue
   dosrc     = return code from DosWriteQueue()
:exmp.

:h2.RxPurgeQueue - Purge elements from OS/2 queue
:p.This function purges elements from an OS/2 queue

:xmp.
Syntax:

  dosrc = RxPurgeQueue(qhandle)

where

   qhandle   = handle to queue
   dosrc     = return code from DosPurgeQueue()
:exmp.

:h2.RxQueryQueue - Query info about OS/2 queue
:p.This function returns information about an OS/2 queue

:xmp.
Syntax:

  info = RxQueryQueue(qhandle)

where

   qhandle   = handle to queue
   info      = two blank delimited tokens:
               1) return code from DosQueryQueue()
               2) number of elements on queue (if (1) is zero)
:exmp.

:h2.RxCloseQueue - Close an OS/2 queue
:p.This function closes an OS/2 queue

:xmp.
Syntax:

  dosrc = RxCloseQueue(qhandle)

where

   qhandle   = handle to queue
   dosrc     = return code from DosCloseQueue()
:exmp.

:h2.RxReadQueueStr - Read data from OS/2 queue
:p.This function returns the de-referenced contents of an OS/2 queue
element.  That is, it dequeues an element and returns the data pointed
to by the dequeued element as the function result.  It first does a
DosOpenQueue, then a DosReadQueue(wait), but does NOT do a DosCloseQueue.

:xmp.
Syntax:

  datastr = RxReadQueueStr(qid)

where

   qid       = name of queue to read, or handle of already-opened queue
   datastr   = data pointed to by dequeued pointer
:exmp.

:h1.I/O Related
:p.Miscellaneous functions related to I/O operations.

:h2.RxOpen - Open a file
:p.This function opens a file.

:xmp.
Syntax:

  dosrc = RxOpen(rexxstem, filename, openflag, openmode [,attr] [,size])

where

  rexxstem  = name of Rexx stem variable under which info is returned:
              stem.1 = file handle
              stem.2 = action taken:
                       Existed
                       Created
                       Truncated
  filename  = name of file/pipe/stream to open
  openflag  = flag indicating what to do if file exists:
              (case does not matter)
              N - fail if New
              C - Create if new
              E - fail if Exists
              O - Open if exists
              R - Replace if exists
  openmode  = how to open file, valid combinations of:
              (case DOES matter)
              D - Dasd
              T - write Through
              F - Fail on error
              C - no Cache
              S - Sequential
              R - Random
              M - random/sequential (Mixed)
              H - no inHerit
              b - (sharemode) deny read/write (both)
              w - (sharemode) deny write
              r - (sharemode) deny read
              n - (sharemode) deny none
              R - (accessmode) Read only
              W - (accessmode) Write only
              B - (accessmode) readwrite (Both)
  attr      = file attribute, valid combinations of:
              (only if open creates file)
              A - Archived
              D - Directory
              S - System
              H - Hidden
              R - Readonly
              N - Normal (default)
  size      = file size (only if open creates file)
  dosrc     = return code from DosOpen()
:exmp.

:h2.RxDosRead - Read from File Handle (alias for RxRead)
:p.This alias for "RxRead()" is provided to allow RXU to be used along with
other Rexx function packages (such as RXUTILS, etc.) which already use the function
name "RxRead".

:h2.RxRead - Read from File Handle
:p.This function reads a character stream in from a file handle, rather than
from a file name.

:xmp.
Syntax:

  dosrc = RxRead( rexxvar, hfile [,length] )

where

  rexxvar = name of Rexx variable into which read data is placed
  hfile   = handle of file to be read from
  length  = number of bytes to read (defaults to zero)
  dosrc   = two blank-delimited words:
            1) return code from DosRead
            2) if (1) is zero, an integer indicating number of bytes
               read (should be same as length of rexx variable's
               value)
:exmp.

:h2.RxDosWrite - Write to File Handle (alias for RxWrite)
:p.This alias for "RxWrite()" is provided to allow RXU to be used along with
other Rexx function packages (such as RXUTILS, etc.) which already use the function
name "RxWrite".

:h2.RxWrite - Write to File Handle
:p.This function writes a character stream out to a file handle, rather than
from a file name.

:xmp.
Syntax:

  dosrc = RxWrite( hfile , data )

where

  hfile = handle of file to be written to
  data  = data to be written to file
  dosrc   = two blank-delimited words:
            1) return code from DosWrite
            2) if (1) is zero, an integer indicating number of bytes
               written (may not be same as length of data supplied)
:exmp.

:h2.RxCloseH - Close a File Handle
:p.This function closes a file handle.

:xmp.
Syntax:

  closerc = RxCloseH(hfile)

where

  hfile   = handle of file to be closed
  closerc = return code from DosClose()
:exmp.

:h2.RxSoSe2H - Redirect StdOut/StdErr to a file (by file handle)
:p.This function redirects StdOut/StdErr to a file whose handle you supply.

:xmp.
Syntax:

  dosrc = RxSoSe2H(hfile)

where

  hfile   = handle of file
  dosrc   = return code from DosDupHandle()
:exmp.

:h2.RxSi2H - Redirect StdIn from a file (by file handle)
:p.This function redirects StdIn from a file whose handle you supply.

:xmp.
Syntax:

  dosrc = RxSi2H(hfile)

where

  hfile   = handle of file
  dosrc   = return code from DosDupHandle()
:exmp.

:h2.RxRSi2F - Redirect StdIn from a file (by file name)
:p.This function redirects StdIn from a file whose name you supply.

:xmp.
Syntax:

  dosrc = RxRSi2F(filename)

where

  filename = name of file
  dosrc    = return code from DosOpen() or DosDupHandle()
:exmp.

:h2.RxExecI - Read data into a Rexx queue or stem from a file
:p.This function reads (text) data into a Rexx queue or stem variable
from a file.  It is meant to be like EXECIO in TSO and VM, and can
be used in a similar fashion for file input.

:xmp.
Syntax:

  info = RxExecI([fname], [qtype], [name], [type])

where:

  fname  = name of file to be read from (default reads from stdin)
  qtype  = 'Lifo' or 'Fifo' (default is 'Fifo')
           determines how lines are read onto queue
           (ignored for stems)
  name   = name of Rexx queue from into which data is read (default uses
           current queue)
               or
           name of a Rexx stem variable under which data is to be mapped as
             stem.0 = number of lines (stem.1 - stem.n)
             stem.1
             stem.2
             ...
             stem.n
  type   = 'Queue' or 'Stem' (default is 'Queue')
           this tells whether arg 3 is a Rexx Queue name or a stem
           variable name
  info   = two blank delimited numbers:
           1) number of lines read
           2) total number of bytes read

Example:

  /* Read a text file into the Rexx stem 's.' */
  info = rxexeci('c&colon.\config.sys',,'s.','s')
  parse var info lines bytes
  Say lines 'lines were read'
  Say bytes 'bytes were read'
:exmp.

:h2.RxExecO - Write data from a Rexx queue or stem to a file
:p.This function writes data from a Rexx queue or stem variable
to a file.  It is meant to be like EXECIO in TSO and VM, and can
be used in a similar fashion for file output.

:xmp.
Syntax:

  info = RxExecO([fname], [action], [name], [type])

where:

  fname  = name of file to be written to (default sends to stdout)
  action = Replace or Append (default is 'Replace')
           replaces or appends to 'fname'
  name   = name of Rexx queue from which to get data (default uses
           current queue)
               or
           name of a Rexx stem variable under which data is mapped as
             stem.1
             stem.2
             ...
             stem.n
           Once an uninitialized value for 'stem.n' is encountered,
           i/o stops.
  type   = 'Queue' or 'Stem' (default is 'Queue')
           this tells whether arg 3 is a Rexx Queue name or a stem
           variable name
  info   = two blank delimited numbers:
           1) number of lines written
           2) total number of bytes written

Example:

  /* Write 100 lines from current queue to c&colon.\junk.dat (replace file) */
  Do 100
    Queue i time('l')
  End
  info = rxexeco('c&colon.\junk.dat','r')
  parse var info lines bytes
  Say lines 'lines were written'
  Say bytes 'bytes were written'
:exmp.

:h2.RxVioPopUp - Start a VioPopUp display screen
:p.This function starts a VioPopUp display screen.
:xmp.
Syntax:

  viorc = RxVioPopUp(wait,opaque)

where:

  wait   = Wait   - wait if a pop-up screen already exists
         = Nowait - return with error if a pop-up screen already exists
  opaque = Opaque or Transparent
  viorc  = return code from VioPopUp()
:exmp.

:h2.RxVioEndPopUp - Close a VioPopUp display screen
:p.This function closes a VioPopUp display screen.
:xmp.
Syntax:

  viorc = RxVioEndPopUp()

where:

  viorc  = return code from VioEndPopUp()
:exmp.

:h2.RxVioWrtCharStrAtt - Write characters to a VioPopUp display screen
:p.This function writes characters to a VioPopUp display screen.
:xmp.
Syntax:

  viorc = RxWrtCharStrAtt(string,rowx,coly,attr)

where:

  string = data to write
  rowx   = row number
  coly   = column number
  attr   = attribute bit string
  viorc  = return code from VioWrtCharStrAtt()
:exmp.

:h2.RxKbCharIn - Get a character from a VioPopUp display screen
:p.This function gets a character from a VioPopUp display screen
:xmp.
Syntax:

  info = RxKbdCharIn(wait)

where:

  wait   = Wait   - wait for character
           Nowait - no wait for character
  info   = return code from KbdCharIn(), and, if zero, followed by these
           blank delimited values:
           1) Character
           2) Scan Code (hex)
           3) NLS state (hex)
           4) NLS shift (hex)
           5) Shift Key state (hex)
           6) Millisecond timestamp of keystroke
:exmp.

:h2.Rxrsoe2f - Redirect Std-Out/Err to File
:p.This function redirects Standard Out and Standard Error to a named file
(or device name, such as "CON").

:xmp.
Syntax:

  Call Rxrsoe2f stream,opt

where:

  stream = name of file or device (e.g. 'con', 'nul', 'kbd', etc.)
           to which standard-out and standard-error are re-directed.
  opt    = either 'Append' or 'Replace' file/device
:exmp.

:h2.Rxrsoe2q - Redirect Std-Out/Err to Rexx Queue
:p.This function redirects Standard Out and Standard Error to a named queue.

:xmp.
Syntax:

  token = rxrsoe2q(opt,value)

where:

  opt    = either 'Enable' or 'Disable'
  value  = queue name - (when opt=Enable)  (created if it doesn't exist)
           token      - (when opt=Disable)
  token  = binary token returned when opt=Enable, subsequently supplied as a parameter
           when opt=Disable
:exmp.

:h2.RxDupHandle - Do a "DosDupHandle()"
:p.This function allows you to do a "DosDupHandle()"

:xmp.
Syntax:

  dosrc = RxDupHandle(rexxvar,hfile1,hfile2)

where:

  rexxvar = name of Rexx variable in which is return duplicated handle
            (value returned in arg2 by DosDupHandle)
  hfile1  = arg1 to DosDupHandle
  hfile2  = arg2 to DosDupHandle
  dosrc   = return code from DosDupHandle()
:exmp.

:h2.RxSetFHState - Set file handle state
:p.This function allows you to set a file handle's state

:xmp.
Syntax:

  dosrc = RxSetFHState(hfile,flags)

where:

  hfile  = file handle whose state you want to set
  flags  = valid combinations of:
           (case DOES matter)
           D - Dasd
           T - write Through
           F - Fail on error
           C - no Cache
           S - Sequential
           R - Random
           M - random/sequential (Mixed)
           H - no inHerit
           b - (sharemode) deny read/write (both)
           w - (sharemode) deny write
           r - (sharemode) deny read
           n - (sharemode) deny none
           R - (accessmode) Read only
           W - (accessmode) Write only
           B - (accessmode) readwrite (Both)
  dosrc  = return code from DosSetFHState()
:exmp.

:h2.RxSetMaxFH - Set maximum file handles for process
:p.This function allows you to set the maximum number of file handles
for the calling process.

:xmp.
Syntax:

  dosrc = RxSetMaxFH(numhandles)

where:

  numhandles = maximum number of file handles for calling process
  dosrc      = return code from DosSetMaxFH
:exmp.

:h2.RxSetRelMaxFH - Set max file handles for process (relative to current value)
:p.This function allows you to set the maximum number of file handles
(relative to the current number) for the calling process.

:xmp.
Syntax:

  info = RxSetRelMaxFH(numhandles)

where:

  numhandles = maximum number of file handles for calling process
  info       = two blank delimited integers:
               1) return code from DosSetMaxFH
               2) new total number of allocated file handles
:exmp.

:h2.RxQueryFHState - Query file handle state
:p.This function allows you to query a file handle's state

:xmp.
Syntax:

  info = RxQueryFHState(hfile)

where:

  hfile  = file handle whose state you want to query
  info   = two blank delimited tokens:
           1) return code from DosQueryFHState(),
           2) if (1) is zero, then a string of flags indicating state
              of file, flag values are (case is important):
              D - OPEN_FLAGS_DASD
              T - OPEN_FLAGS_WRITE_THROUGH
              F - OPEN_FLAGS_FAIL_ON_ERROR
              C - OPEN_FLAGS_NO_CACHE
              H - OPEN_FLAGS_NOINHERIT
              b - OPEN_SHARE_DENYREADWRITE
              w - OPEN_SHARE_DENYWRITE
              r - OPEN_SHARE_DENYREAD
              n - OPEN_SHARE_DENYNONE
              R - OPEN_ACCESS_READONLY
              W - OPEN_ACCESS_WRITEONLY
              B - OPEN_ACCESS_READWRITE
:exmp.

:h1.System-Info related
:p.Functions returning OS/2 system information.

:h2.RxSetExtLibPath - Set extended LIBPATH for current process
:p.This function uses the DosSetExtLIBPATH API to set an extension to the LIBPATH
for the current process.  This allows you to dynamically alter LIBPATH per process.
:xmp.

Syntax:

  dosrc = RxSetExtLibPath( pathstring [,flag] )

Where:

  pathstring = semicolon delimited list of directories to be prepended/appended
               to existing system-wide LIBPATH for this process
  flag       = 'B' indicating that "pathstring" is to be prepended to LIBPATH
               'A' indicating that "pathstring" is to be appended to LIBPATH
  dosrc      = return code from DosSetExtLIBPATH
:exmp.

:h2.RxQueryExtLibPath - Query extended LIBPATH for current process
:p.This function uses the DosQueryExtLIBPATH API to query the extension to the LIBPATH
for the current process.
:xmp.

Syntax:

  dosrc = RxQueryExtLibPath( rexxvar, flag )

Where:

  flag       = 'B' to query LIBPATH extension that was prepended to system-wide LIBPATH
               'A' to query LIBPATH extension that was appended to system-wide LIBPATH
  rexxvar    = name of rexx variable in which LIBPATH extension string is returned
  dosrc      = return code from DosQueryExtLIBPATH
:exmp.

:h2.RxQProcstatus - Get Process Status Information
:p.This function returns process status information (like PSTAT).  In this
version of RXU (v1.5), Library, Shared-Memory and 16-Bit Semaphore
information is stored differently than in the previous release (v1.4).
In the previous version, if you wanted the name of a module used by a process,
for instance, you would have to search through the Library array to match the
module index value "stem.0p.n.0l.m", to get the module name "stem.0l.n.2".  In
this version, you simply get the module name by indexing into the Library array
using the index value in "stem.0p.n.0l.m".
:xmp.

Syntax:

  dosrc = RxQProcStatus(stemname [,flags])

where:
 
  stemname = a Rexx stem variable name under which results are 
             mapped as such:

               // System Global Information
               stem.0G.1 = total number of threads in system
               stem.0G.2 = undocumented value 1
               stem.0G.3 = undocumented value 2

               // Process Information
               stem.0P.0 = number of processes
               stem.0P.n.1 = process-id (hex)
               stem.0P.n.2 = parent process-id (hex)
               stem.0P.n.3 = process type (hex)
               stem.0P.n.4 = process status (hex, followed by blank, followed
                             by text description)
               stem.0P.n.5 = process screen-group/session-id (hex)
               stem.0P.n.6 = process module name (value of "SYSINIT" may
                             indicate a DOS "process")
               stem.0P.n.0S.0 = number of 16-bit semaphores used by process
               stem.0P.n.0S.n = 16-bit semaphore index value (use to "hash"
                                into list of "stem.0S.index" values)
               stem.0P.n.0L.0 = number of libraries statically linked by process
               stem.0P.n.0L.n = linked library index value (use to "hash"
                                into list of "stem.0L.index" values)
               stem.0P.n.0M.0 = number of shared memory objects used by process
               stem.0P.n.0M.n = shared memory index value (use to "hash"
                                into list of "stem.0M.index" values)
               stem.0P.n.0T.0 = number of threads in this process
               stem.0P.n.0T.m.1 = thread-id within process (hex)
               stem.0P.n.0T.m.2 = unique thread slot number (hex, system wide)
               stem.0P.n.0T.m.3 = sleep-id thread is sleeping on (hex)
               stem.0P.n.0T.m.4 = thread priority (hex)
               stem.0P.n.0T.m.5 = system time (# ring 0 time slices)
               stem.0P.n.0T.m.6 = user time (# ring 3 time slices)
               stem.0P.n.0T.m.7 = thread state (hex followed by text string)

               // 16-Bit Semaphore Information
               stem.0S.0 = number of 16-bit semaphore records
               stem.0S.0.1 = index of first semaphore (hex)
               stem.0S.n = semaphore index (hex, hash into using "stem.0P.n.0S.n")
               stem.0S.index.1 = owning slot-id (hex)
               stem.0S.index.3 = semaphore flag
               stem.0S.index.4 = reference count
               stem.0S.index.5 = request count
               stem.0S.index.6 = semaphore name

               // Shared Memory Object Information
               stem.0M.0 = number of shared memory objects
               stem.0M.n = memory handle (hex, hash into using "stem.0P.n.0M.n")
               stem.0M.index.1 = memory selector (hex)
               stem.0M.index.2 = reference count
               stem.0M.index.3 = memory name

               // Statically Linked Library Information
               stem.0L.0 = number of statically linked libraries/modules
               stem.0L.n = module handle (hex, hash into using "stem.0P.n.0L.n")
               stem.0L.index.1 = library name
               stem.0L.index.0I.0 = number of imported modules
               stem.0L.index.0I.n = imported module handle (hex, use to "hash"
                                    into list of "stem.0L.index" values)

  flags    = flags indicating what kind of information to return, may be
             any combination of:
               M - shared memory information
               S - 16-bit semaphore information
               L - linked library information

  dosrc    = return code from DosQProcStatus()

Example:

  /* rexx */
  /* Find out what modules each process has statically linked (1st level) */

  dosrc = rxqprocstatus('q.','l')

  Say 'There are' q.0p.0 'processes ('q.0g.1 'threads) running right now'
  do j=1 to q.0p.0
    say;say 'Process ('q.0p.j.1')'q.0p.j.6 'statically links the following libraries&colon.'
    do r=1 to q.0p.j.0l.0
      idx = q.0p.j.0l.r
      say '   ' q.0l.idx.1
    end
  end

  exit

Note:

  Process/Thread information is always returned, whether or not semaphore,
  library or shared-memory information is requested.

  The reason such odd compound variable index values were chosen (such as "0P",
  "0S", etc.) is because too long a constant index value (such as "PROC")
  tended to increase processing time for the function call (actual size of
  variable name pool is larger).  The alternative, to use brief constant index
  values (such as "M") turned out to cause problems when I happened to be using
  one of these for a variable.  The solution was to choose a constant index name
  which begins with a digit ("0" - i.e. zero), which is an invalid variable name,
  thus ensuring that no variable could have the same name as the constant index.
  This made relatively brief names (e.g. - "0M") possible and prevents "collision"
  with variable names your program may be using.
:exmp.

:h2.RxSetError - Set DosError settings
:p.This function allows you to turn HardError and Exception popup on
or off.  This change takes effect for the process in which it is issued,
no matter what the previous setting was, by whom, or on what thread.
Presently, there is no way (that I know of) to query or "reset" these
error settings.
:xmp.
Syntax:

  dosrc = RxSetError(flag)

where:
 
  flag  = any combination of:
          'h' to disable HardError popup
          'H' to enable  HardError popup
          'e' to disable Exception popup
          'E' to enable  Exception popup
  dosrc = return code from DosError()

Example:

  Call rxseterror 'eh'  /* would suppress popups from both harderror */
                        /* and exception conditions */
:exmp.

:h2.RxReplaceModule - Replace an active .DLL or .EXE
:p.This function uses the OS/2 "DosReplaceModule()" API to replace an
active .DLL or .EXE (i.e. such a module that is locked because it is
currently in use).
:xmp.
Syntax:

  dosrc = RxReplaceModule(oldmodname, newmodname, backupmodname)

where:
 
  oldmodname    = name of locked module
  newmodname    = name of new module to replace old one
  backupmodname = name of backup module for recoverability
  dosrc         = return code from DosReplaceModule()
:exmp.

:h2.RxExitList - Use DosExitList
:p.This function uses the OS/2 "DosExitList()" API to work with OS/2
Exit-Handlers (i.e. add/remove/exit).  For more information on this
function, see the OS/2 developer's reference for "DosExitList()".
:xmp.
Syntax:

  dosrc = RxExitList(funcaddr [,action] [,order] )

where:
 
  funcaddr  = function pointer (e.g. obtained by RxQueryProcAddr())
  action    = Add, Remove, Exit
  order     = 1 byte value (0x00-0xff) indicating order for "Add"
  dosrc     = return code from DosExitList()
:exmp.

:h2.RxProcId - Get process' own PID and TID information
:p.This function returns the process-id, parent process-id, and
thread-id of the current thread of the current process.
:xmp.
Syntax:

  procinfo = rxprocid()

where:

  procinfo = three blank delimited integers:
             1) process-id
             2) parent process-id
             3) thread-id
:exmp.

:h2.RxGetInfoBlocks - Get information about current process/thread
:p.This function returns the various pieces of information which
the DosGetInfoBlocks() function returns.
:xmp.
Syntax:

  dosrc = RxGetInfoBlocks(rexxstem)

where:

  rexxstem = name of rexx stem under which information is returned as
             such:

             stem.P.1  = process-id
             stem.P.2  = parent process-id
             stem.P.3  = our .EXE's module handle
             stem.P.4  = pointer to command line string
             stem.P.5  = pointer to environment string
             stem.P.6  = process status bits
             stem.P.7  = process type code

             stem.T.1  = thread-id
             stem.T.2  = thread priority
             stem.T.3  = tib2 version
             stem.T.4  = MustComplete count
             stem.T.5  = MustComplete Force Flag
             stem.T.6  = tib version
             stem.T.7  = thread ordinal
             stem.T.8  = pointer to head of exception handler chain
             stem.T.9  = pointer to base of stack
             stem.T.10 = pointer to end of stack

  dosrc    = return code from DosGetInfoBlocks()

Note:

  for stem.P.4, stem.P.5, stem.T.9, and stem.T.10, you could use
  RxStorage() to get to the values at these addresses.
:exmp.

:h2.RxQueryAppType - Get information about an executable file
:p.This function returns information about the type of an executable
file as indicated in that file's header.
:xmp.
Syntax:

  info = RxQueryAppType(filename)

where:

  filename = name of executable file whose type you want to know
             (either fully qualified, or just name and will search
              path)
  info     = return code from DosQueryAppType, and if zero, followed
             by any combination of:

             N - NotSpecified
             w - NotWindowCompat
             W - WindowCompat
             A - WindowApi
             B - Bound
             L - DLL
             D - DOS
             Y - Physical Device Driver
             V - Virtual Device Driver
             P - Protected Memory DLL
             3 - 32 Bit
:exmp.

:h2.RxTmrQueryFreq - Query frequency of IRQ0 high-resolution timer
:p.This function returns the frequency of the IRQ0 high-resolution (8254) timer
via the DosTmrQueryFreq() API.
:xmp.
Syntax:

  info = RxTmrQueryFreq()

where

  info     = two blank delimited values:
             1) return code from DosTmrQueryFreq()
                and if return code is zero:
             2) frequency of high-resolution timer in hertz (timer units/sec).
:exmp.

:h2.RxTmrQueryTime - Query snapshot of IRQ0 high-resolution timer value
:p.This function returns a snapshot of the value from the IRQ0 high-resolution
(8254) timer via the DosTmrQueryTime() API.
:xmp.
Syntax:

  info = RxTmrQueryTime()

where

  info     = two blank delimited values:
             1) return code from DosTmrQueryTime()
                and if return code is zero:
             2) two period-delimited integers indicating high-order and low-order
                portions of timer value, as such:
                  high-order.low-order
:exmp.

:h2.RxQuerySysInfo - Query OS/2 SysInfo
:p.This function returns live system information about OS/2 via the
DosQuerySysInfo API.
:xmp.
Syntax:

  dosrc = RxQuerySysInfo( stemname [,startidx [,endidx]])

where

  stemname = Rexx stem name, under which results are returned.  Each
             DosQuerySysInfo item is returned under the appropriate stem.n
             For instance, the call "call rxquerysysinfo('s.',3,5)" would
             set the rexx variables "s.3", "s.4", and "s.5".
  dosrc    = return code from DosQuerySysInfo()
  startidx = Default is 1
  endidx   = Default is 26

  Index values:

  1. Maximum length, in bytes, of a path name.
  2. Maximum number of text sessions.
  3. Maximum number of PM sessions.
  4. Maximum number of DOS sessions.
  5. Drive from which the system was started (1 means drive A,
     2 means drive B, and so on).
  6. Dynamic priority variation flag (0 means absolute priority,
     1 means dynamic priority).
  7. Maximum wait in seconds.
  8. Minimum time slice in milliseconds.
  9. Maximum time slice in milliseconds.
 10. Memory page size in bytes.  This value is 4096 for the 80386
     processor.
 11. Major version number.
 12. Minor version number.
 13. Revision letter.
 14. Value of a 32-bit, free-running millisecond counter.  This value
     is zero when the system is started.
 15. Low-order 32 bits of the time in seconds since January 1, 1970
     at 0&colon.00&colon.00.
 16. High-order 32 bits of the time in seconds since January 1, 1970
     at 0&colon.00&colon.00.
 17. Total number of pages of physical memory in the system.
     One page is 4KB.
 18. Total number of pages of resident memory in the system.
 19. Maximum number of pages of memory that can be allocated by all
     processes in the system.  This number is advisory and is not
     guaranteed, since system conditions change constantly.
 20. Maximum number of bytes of memory that this process can allocate
     in its private arena.  This number is advisory and is not
     guaranteed, since system conditions change constantly.
 21. Maximum number of bytes of memory that a process can allocate in
     the shared arena.  This number is advisory and is not guaranteed,
     since system conditions change constantly.
 22. Timer interval in tenths of a millisecond.
 23. Maximum length, in bytes, of one component in a path name.
 24. Session ID of the current foreground full-screen session (full screen
     sessions only; PM session has sessid=1)
 25. Process ID of current (i.e. one making this call) foreground process
 26. Number of processors on machine
:exmp.

:h2.RxPhysicalDisk - Query information about partitionable disks
:p.This function returns information about physical disks via the
DosPhysicalDisk() API.
:xmp.
Syntax:

  dosrc = RxDevConfig(function, rexxvar ,retlen [,parm])

where

  function = function code (1,2,3)
  rexxvar  = Rexx variable name, in which physical disk information is returned
  retlen   = return buffer length
  parm     = (optional) parameter to be passed to API
  dosrc    = return code from DosPhysicalDisk()
:exmp.

:h2.RxDevConfig - Obtain device configuration information
:p.This function returns device configuration information using the
DosDevConfig() API.
:xmp.
Syntax:

  dosrc = RxDevConfig(rexxvar ,devtype [,infolen])

where

  rexxvar  = Rexx variable name, in which device information is returned
  devtype  = integer (currently 0-6) indicating which device to get info on
  infolen  = if devtype > 6, length of buffer necessary to hold devinfo
  dosrc    = return code from DosDevConfig()
:exmp.

:h2.RxDevIOCtl - Do DosDevIOCtl calls (to talk to character devices directly)
:p.This function allows direct communication with character devices using
the DosDevIOCtl() API.
:xmp.
Syntax:

  dosrc = RxDevIOCtl(rexxstem, devhandle, category, function,
                     parmlist, retparmlen, data, retdatalen)

where

  rexxstem   = Rexx stem name, under which data is returned as such:
               stem.1 = parmlist returned
               stem.2 = dataarea returned
  devhandle  = device handle (binary) obtained from DosOpen()
  category   = integer indicating category
  function   = integer indicating function
  parmlist   = parmlist data (if zero length or "empty" string, no parmlist is
               passed to DosDevIOCtl())
  retparmlen = length of expected parmlist returned
  data       = dataarea data (if zero length or "empty" string, no dataarea is
               passed to DosDevIOCtl())
  retdatalen = length of expected dataarea returned
  dosrc      = return code from DosDevIOCtl()
:exmp.

:h1.PM / Wp related functions
:p.Functions related to PM or Workplace Shell Objects

:h2.RxWinQueryObject - Query WP-Shell Object Handle
:p.This function returns the object handle of a WorkPlace Shell object.
:xmp.
Syntax:

  hobj = RxWinQueryObject(objname)

where

  objname = name of WorkPlace Shell object
  hobj    = handle to object (or null string if not successful)
:exmp.

:h2.RxWinDestroyObject - Destroy a WP-Shell Object
:p.This function destroys a WorkPlace Shell object by object handle.
:xmp.
Syntax:

  yorn = RxWinDestroyObject(hobj)

where

  hobj    = handle to object
  yorn    = "1" if it worked, "0" if it didn't
:exmp.

:euserdoc.
