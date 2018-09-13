clang -c -m32 -nostdlib -o compress.o compress.c -O1
clang -c -m32 -o asm_tester.o asm_tester.c
clang -c -m32 -o stack.o stack.c
clang -c -m32 -o clipboard.o clipboard.c
clang -c -m32 -o copy_paste_clipboard.o copy_paste_clipboard.c
clang -m32 -o asm asm_tester.o compress.o stack.o clipboard.o copy_paste_clipboard.o


./asm

rm *.o
