rxu.dll:  \
  rxu.OBJ \
  rxu.def \
  rxu.MAK
   @REM @<<rxu.@0
     rxu.obj /MAP:FULL
     rxu.dll
     
     cppom30.lib+rexx.lib
     rxu.def;
<<
  ILINK.EXE /NOFREE @rxu.@0
  IMPLIB rxu.LIB rxu.def
  mapsym rxu.map

rxu.obj: \
   rxu.c
   ICC.EXE /Ss /Sm /Q /I.;\dboll\include;d:\muglib /O /Gm+ /Ge- /Gd- /Gt /C .\rxu.c
