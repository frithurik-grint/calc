.assembly Test {}
.assembly extern mscorlib {}

.module test.exe

.method private hidebysig static void Main() cil managed
{
.entrypoint
.maxstack 3

ldc.i4.s 12
ldc.i4.s 3
ldc.i4.s 54
mul
sub
call void [mscorlib]System.Console::WriteLine(int32)
ret
}
