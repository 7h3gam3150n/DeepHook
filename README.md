# DeepHook
:)
The described attack is a driver-based kernel-mode rootkit that uses SSDT hooking to intercept and manipulate system calls. The rootkit hooks system calls from the deepest levels possible, which allows it to affect every process on the system.

When a program calls a function such as CreateFileA(), the operating system follows a series of steps. First, it calls CreateFileA() from kernel32.dll, then it calls NtCreateFileA() from NtDll.dll. This function moves the address of the service to be called to the EAX register and then calls another function called KiFastSystemCall!ntdll.

KiFastSystemCall moves the function's parameters to the EDX register and then calls KiSystemService, which uses the sysenter command/opcode to enter kernel mode. Sysenter changes a restricted bit called the mode bit, which can be found in the PSW register or in a 2-bit field in the CS register, depending on the actual CPU.

KiSystemService is located in the kernel and has access to the SSDT. The SSDT, or System Service Dispatch Table, contains for each entry a number and a reference to a function. KiSystemService uses this number to get the reference to the function from the SSDT and then runs that function, which is located in ntoskrnl.exe.

To hook a system call such as CreateFileA(), the attacker needs to change the address in the SSDT that points to the original function in ntoskrnl.exe to point to their function in the driver's memory space. This requires editing kernel data, which is protected by privilege levels.

The attacker can achieve this by creating a driver that works from ring 0, the highest privilege level, and turning off a bit in the CR0 register that is supposed to disallow the editing of kernel data. Once the bit is turned off, the attacker can simply change the address in the SSDT.

In conclusion, SSDT hooking is a powerful technique that allows attackers to intercept and manipulate system calls from the deepest levels possible, affecting every process on the system.
