# IBM Developer's Workframe/2 Make File Creation run at 08:49:55 on 11/16/92

# Make File Creation run in directory:
#   D:\DBOLL\C;

.SUFFIXES:

.SUFFIXES: .c

ALL:      qprintf.OBJ

{.}.c.obj:
   ICC.EXE /Fd /Gm /O+ /Q /C /Kabgop .\$*.c
