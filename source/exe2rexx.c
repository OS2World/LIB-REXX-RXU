/* Include files */

#define INCL_DOS
#define INCL_DOSPROCESS
#define INCL_ERRORS
#include <os2.h>
#define INCL_REXXSAA
#include <rexxsaa.h>                   /* needed for RexxStart()     */
#include <stdio.h>                     /* needed for printf()        */
#include <stdlib.h>
#include <string.h>                    /* needed for strlen()        */

int main(int argc, char *argv[])

{

  RXSTRING arg;                       /* argument string for REXX  */
  RXSTRING rexxretval;                /* return value from REXX    */
  APIRET   rc;                        /* return code from REXX     */
  SHORT    rexxrc = 0;                /* return code from function */
  UCHAR    execname[250];
  RXSTRING execargs[20];
  ULONG    ulCtr;
  UCHAR    uchdrive[_MAX_DRIVE];
  UCHAR    uchdir[_MAX_DIR];
  UCHAR    uchext[_MAX_EXT];
  UCHAR    uchfname[_MAX_FNAME];
  UCHAR    uchfqfile[CCHMAXPATH+1];
  PPIB     ppib;
  PTIB     ptib;

  DosGetInfoBlocks(&ptib,&ppib);
  memset(uchfqfile,0x00,sizeof(uchfqfile));
  DosQueryModuleName(ppib->pib_hmte,sizeof(uchfqfile)-1,uchfqfile);

  _splitpath(uchfqfile,uchdrive,uchdir,uchfname,uchext);
  sprintf(execname,"%s.cmd",uchfname);
  printf("Executing Rexx Program '%s'\n",execname);

  for (ulCtr=1;ulCtr < argc;ulCtr++)
    {
    execargs[ulCtr-1].strptr = argv[ulCtr];
    execargs[ulCtr-1].strlength = strlen(argv[ulCtr]);
    }

  MAKERXSTRING(rexxretval, NULL, 0);  /* null out RXSTRING           */

  rc=RexxStart((LONG)      argc-1,        /* number of arguments   */
              (PRXSTRING)  &execargs[0],  /* arguments             */
              (PSZ)        execname,      /* name of REXX file     */
              (PRXSTRING)  0,             /* No INSTORE used       */
              (PSZ)        NULL,          /* Command env. name     */
              (LONG)       RXCOMMAND,     /* Code for how invoked  */
              (PRXSYSEXIT) 0,             /* No EXITs on this call */
              (PSHORT)     &rexxrc,       /* Rexx program output   */
              (PRXSTRING)  &rexxretval ); /* Rexx program output   */

  DosFreeMem(rexxretval.strptr);

  return 0;
}
