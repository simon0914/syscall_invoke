
#if defined(__linux__)

/* this code will be compiled on linux */

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <asm/unistd.h>

pid_t getpid_syscall(void)
{
	pid_t res;
	asm volatile (
		/* this test for 32 bit architecture is a bit brittle... */
#if defined(__i386__)
		// 32 bit version:
		  "int $0x80"
		: "=a"(res)
		: "0"(0x0000) /* TODO: replace 0x0000 with the syscall number */
#elif defined(__x86_64__)
		// 64 bit version:
		  "syscall"
		: "=a"(res)
		: "0"(0x0000) /* TODO: replace 0x0000 with the syscall number */
		: "rcx", "r11"
#else
#		error("unrecognized architecture")
#endif
		);

	if (res <= 0) {
		errno = -res;
	}

	return res;
}

#elif defined(_WIN32)

/* this code will be compiled on windows */

#define _CRT_NONSTDC_NO_DEPRECATE
#include <stdio.h>
#include <errno.h>
#include <windows.h>
#include <winternl.h>
#include <conio.h>
#include <process.h>

/* we define a pid integer to be of DWORD size */
typedef DWORD pid_t;

/* declare the function that is defined in assembly in syscall_.asm */
extern NTSTATUS CallNtQueryInformationProcess(
	IN HANDLE           ProcessHandle,
	IN PROCESSINFOCLASS ProcessInformationClass,
	OUT PVOID           ProcessInformation,
	IN ULONG            ProcessInformationLength,
	OUT PULONG          ReturnLength);

/* Wrapper function, so we can use a common interface down in the main function. */
pid_t getpid_syscall(void)
{
	PROCESS_BASIC_INFORMATION pbi;
	ULONG insize = sizeof(pbi);
	ULONG outsize = 0;

	int res = CallNtQueryInformationProcess(
		GetCurrentProcess(),
		ProcessBasicInformation,
		&pbi,
		insize,
		&outsize
	);

	return pbi.UniqueProcessId;
}

#else
/* if neither linux, nor windows, we can't proceed. */
#error neither windows nor linux?
#endif

int main(void)
{
	/* produce user mode API pid */
	pid_t pid_getpid = getpid();
	/* produce syscall API pid */
	pid_t pid_syscall = getpid_syscall();

	/* error handling is always useful */
	if (pid_syscall <= 0) {
		perror("getpid_syscall");
		return 1;
	}

	/* print both and exit */
	printf("%d\n%d\n", pid_getpid, pid_syscall);
	return 0;
}
