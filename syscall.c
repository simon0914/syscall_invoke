
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
		/* TODO: Issue syscall __NR_getpid

				 Hint: You can check rdmsr.h on how to set and read
				 registers with GCC's inline-assembly.
		*/

		);

	if (/* TODO: Set errno to correct error value if syscall fails. */) {

	}

	/* Placeholder (remove this): Not implemented syscalls return -1 and errno ENOSYS */
	errno = ENOSYS;
	return -1;
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
