rxsrs.exe:  \
  rxsrs.OBJ \
  rxsrs.MAK
   @REM @<<rxsrs.@0
     /ST:65535 /PM:VIO +
     rxsrs.OBJ
     rxsrs.exe
     
     cppom30.lib+rexx.lib
     rxsrs.def;
<<
   ILINK.EXE /NOFREE @rxsrs.@0

rxsrs.obj:
   ICC.EXE /Ss /I. /O /Ge+ /C .\rxsrs.c
