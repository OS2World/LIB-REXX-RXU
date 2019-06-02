#if defined(__HOS_OS2__) && defined(__TOS_OS2__) && defined(__OS2__)
  #define __BLD4OS2__
#endif  // building for OS/2

#if defined(_MSC_VER) && defined(_WIN32)
  #define __BLD4NT__
#endif  // building for WindowsNT

// Include files
#if defined(__BLD4OS2__)
  #define INCL_DOS
  #define INCL_DOSPROCESS
  #include <os2.h>
  #define  INCL_REXXSAA
  #include <rexxsaa.h>
  #include <rexxque.h>
#endif  // BLD4OS2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define  INVALID_ROUTINE 40            /* Raise Rexx error           */
#define  VALID_ROUTINE    0            /* Successful completion      */
//int _Optlink qprintf(PCH, const char *, ... );

int main(int argc, char *argv[])
{

  RXSTRING    isarg[20];               // argument string for REXX
  RXSTRING    rexxretval;              // return value from REXX
  SHORT       rexxrc = 0;              // return code from function
  SHORT       actrout, iargc;
  APIRET      rc;
  PCH         pBuf;
  DATETIME    enqueuetime;
  ULONG       qcount;
  RXSTRING    instore[2];
  PRXSTRING   pinstore = NULL;
  RXSTRING    temprxs;
  PRXSTRING   pargs;
  UCHAR       tempname[10] = "InStore";
  BOOL        bInStore = FALSE;
  UCHAR       msgbuf[100];
  PCH         pExecInfo = NULL;
  PSZ         pszExecName = NULL;

  if (argc < 2)
    {
    printf("RxSrs:  At least one argument is required (RexxQueueName)\n");
    return INVALID_ROUTINE;
    }

  rc = RexxQueryQueue((PSZ)argv[1], (PULONG)&qcount);
  if (qcount < 1)
    {
    printf("RxSrs:  At least one element required on queue\n");
    rc = RexxDeleteQueue((PSZ)argv[1]);
    return INVALID_ROUTINE;
    }

  // Get Rexx program's name (or string content) off queue
  MAKERXSTRING(temprxs,NULL,0);
  rc = RexxPullQueue((PSZ) argv[1],
                     (PRXSTRING) &temprxs,
                     (PDATETIME) &enqueuetime,
                     (BOOL) RXQUEUE_NOWAIT);

  pExecInfo = (PCH) malloc(temprxs.strlength + 1);
  memset(pExecInfo,0x00,temprxs.strlength + 1);
  memcpy(pExecInfo,temprxs.strptr,temprxs.strlength);

  if ( temprxs.strlength > 1 )
    {
    switch ( *(pExecInfo) )
      {
      case '$':      // Explicitly an in-storage source string
        MAKERXSTRING(instore[0],pExecInfo+1,temprxs.strlength-1);
        MAKERXSTRING(instore[1],NULL,0);
        pinstore = &instore[0];
        pszExecName = tempname;
        bInStore = TRUE;
        break;
      case '&':      // Explicitly an in-storage tokenized string
        MAKERXSTRING(instore[0],NULL,0);
        MAKERXSTRING(instore[1],pExecInfo+1,temprxs.strlength-1);
        pinstore = &instore[0];
        pszExecName = tempname;
        bInStore = TRUE;
        break;
      case '+':      // Explicitly a macrospace rexx program
        MAKERXSTRING(instore[0],NULL,0);
        MAKERXSTRING(instore[1],NULL,0);
        pinstore = &instore[0];
        pszExecName = (PSZ) malloc(temprxs.strlength + 1);
        memset(pszExecName,0x00,temprxs.strlength + 1);
        memcpy(pszExecName,pExecInfo,temprxs.strlength);
        break;
      default:       // Implicitly an on-disk exec name
        pszExecName = (PSZ) malloc(temprxs.strlength + 1);
        memset(pszExecName,0x00,temprxs.strlength + 1);
        memcpy(pszExecName,pExecInfo,temprxs.strlength);
        break;
      }
    }
  else
    {
    pszExecName = (PSZ) malloc(temprxs.strlength + 1);
    memset(pszExecName,0x00,temprxs.strlength + 1);
    memcpy(pszExecName,pExecInfo,temprxs.strlength);
    }
  DosFreeMem(temprxs.strptr);

  // Get arguments off Rexx Queue (whose name was supplied as arg#1)
  for (actrout=0;actrout < qcount-1;actrout++)
    {
    MAKERXSTRING(temprxs,NULL,0);
    rc = RexxPullQueue((PSZ) argv[1],
                       (PRXSTRING) &temprxs,
                       (PDATETIME) &enqueuetime,
                       (BOOL) RXQUEUE_NOWAIT);
    isarg[actrout].strptr = (PCH) malloc(temprxs.strlength);
    isarg[actrout].strlength = temprxs.strlength;
    memcpy(isarg[actrout].strptr,temprxs.strptr,temprxs.strlength);
    DosFreeMem(temprxs.strptr);
    } /* end-for */

  rc = RexxDeleteQueue((PSZ)argv[1]);

  // Start Rexx program
  rc=RexxStart((LONG)       qcount - 1,    // number of arguments
               (PRXSTRING)  &isarg[0],     // arguments to exec
               (PSZ)        pszExecName,   // name of REXX file
               (PRXSTRING)  pinstore,      // Pointer to InStorage Exec
               (PSZ)        NULL,          // Command env. name
               (LONG)       RXCOMMAND,     // Invoke as a command
               (PRXSYSEXIT) 0,             // No EXITs on this call
               (PSHORT)     &rexxrc,       // Rexx program rc
               (PRXSTRING)  &rexxretval);  // Rexx program result string
  if (rc != 0)
    {
    pBuf = (PCH) malloc(500);
    if ( bInStore )
      printf("RexxStart of In-Storage exec failed with rc = %ld %ld\n",
             rc,rexxrc);
    else
      printf("RexxStart of '%s' failed with rc = %ld %ld\n",
             pszExecName,rc,rexxrc);
    free(pBuf);
    }

  for (actrout=0;actrout < qcount-1;actrout++)
    free(isarg[actrout].strptr);
  free(pExecInfo);
  if ( !bInStore )
    free(pszExecName);

  /* Clean up resources */
  DosFreeMem(rexxretval.strptr);
  return rexxrc;
}
