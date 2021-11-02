
#if defined(__linux__) || defined(__APPLE__)

/* this code will be compiled on linux and macos.
 * specific differences are highlighted below. */

#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#if defined(__linux__)
#	include <asm/unistd.h>
#	define SYSCALL_MASK 0x0
#elif defined(__APPLE__)
#	include <sys/syscall.h>
#	define SYSCALL_MASK 0x02000000
#endif

pid_t getpid_syscall(void)
{
	pid_t res;
	asm volatile (
		/* this test for 32 bit architecture is a bit brittle... */
#if defined(__i386__)
		// 32 bit version:
		  "int $0x80"
		: "=a"(res)
		/* TODO: replace 0x0000 below with the syscall number
		 *       or a symbolic constant. */
		: "0"(SYSCALL_MASK | 0x0000)
#elif defined(__x86_64__)
		// 64 bit version:
		  "syscall"
		: "=a"(res)
		/* TODO: replace 0x0000 below with the syscall number
		 *       or a symbolic constant. */
		: "0"(SYSCALL_MASK | 0x0000)
		: "rcx", "r11"
#else
#error Unsupported Architecture. Send a PR to add support!
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
#error Unsupported OS. Send a PR to add support!
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
