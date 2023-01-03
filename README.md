# DeepHook
A driver based kernel-mode rootkit that hooks system calls by using ssdt hooking!
ֳ#The Idea behind the attack
is to hook system calls from the deepest levels possible so unlike um hooking, each and every process is effected by the hooking and we can do pretty cool stuff such as hiding our own process from tasklist and so on.

#How is the attck working
each time we call a function from a program such as CreateFileA() these are the steps that are taken by the os:
1.Calling CreateFileA() from kernel32.dll
2.Calling NtCreateFileA() from NtDll.dll
  the function moves the address of the service to be called to eax.
  then it is calling another function called -KiFastSystemCall!ntdll:
  1. MOV EAX, <SyscallNumber>
  2. MOV EDX, 7FFE0300h ; EDX = SystemCallStub #addr of kifastsystemcall
  3. CALL DWORD PTR [EDX]
  4. ret 8

3.KiFastSystemCall moves the parameters for the function to edx register. then it calls KiSystemService which uses the sysenter command/opcode.
  
4. sysenter gets us to the kernel's mode. it is doing so by changing a restricted bit called mode bit which can be found in the psw register or in a 2 bits field in the   cs register. it really depends on the actual CPU.
5.KiSystemService is in the kernel and has access to the ssdt. the ssdt or system service dispatch table contains for each entry a number and a refetnce to a function.
  KiSystemService uses this number to get the reference to the function from the ssdt. then it is running that function that is loacted in ntoskrnl.exe.
  Now the function is executed.
  
  
  
  
  
lets say that we want to hook CreateFileA and the entry in the ssdt is: 100h|0xAF12AF. we want to change the address 0xAF12AF that points to the original function ntsokrnl.exe to point to our function in the drivers memory space.
  
 we need to edit some kernelic data. and do to the fact that in protected mode we have 3 prirv levels. well have to get to ring0 at first.
 we can do so by craeting a driver that works from ring0. then we will need to tuen off a bit in cro that is suppose to disallow the editing of kernelic data.
 after that we can simply change the address at the ssdt.
  
 
  
  
  
  

