chcp 65001
cl.exe /utf-8 /std:c11 /Wall /wd4820 /wd4200 /wd4464 /wd4996 /wd4100 /DMSVC ^
    ..\chapter02\type.c ^
    ..\chapter03\buffer.c ^
    ..\chapter04\cons_buffer.c ^
    ..\chapter05\state.c ^
    ..\chapter05\environment.c ^
    ..\chapter08\reader.c ^
    ..\chapter08\parser.c ^
    ..\chapter09\dot.c ^
    helper.c ^
    ..\chapter13\printer.c ^
    built_in_func.c ^
    eval.c ^
    test.c ^
    /Fetest
del *.obj
