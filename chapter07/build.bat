cl.exe /utf-8 /std:c11 /Wall /wd4820 /wd4464 /wd4996 /DMSVC ^
    ..\chapter02\type.c ^
    ..\chapter03\buffer.c ^
    ..\chapter04\cons_buffer.c ^
    ..\chapter05\state.c ^
    ..\chapter06\reader.c ^
    helper.c ^
    printer.c ^
    test.c ^
    /Fetest
del *.obj
