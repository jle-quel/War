#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

uid_t _getuid(void)
{
	uid_t ret;

	asm volatile
	(
		"mov rax, 0x66\n"
		"syscall\n"
	);
	asm volatile
	(
		"mov %0, eax\n"
		: "=r"(ret)
		:
	);

	return ret;
}

ssize_t _read(int fd, void *buf, size_t count)
{
	ssize_t ret;

	asm volatile
	(
		"mov edi, %0\n"
		"mov rsi, %1\n"
		"mov rdx, %2\n"

		"mov rax, 0x0\n"
		"syscall\n"
		:
		: "g"(fd), "g"(buf), "g"(count)
	);
	asm volatile
	(
		"mov %0, rax\n"
		: "=r"(ret)
		:
	);

	return ret;
}

int _open(const char *pathname, int flags, mode_t mode)
{
	int ret;

	asm volatile
	(
		"mov rdi, %0\n"
		"mov esi, %1\n"
		"mov edx, %2\n"

		"mov rax, 0x2\n"
		"syscall\n"
		:
		: "g"(pathname), "g"(flags), "g"(mode)
	);
	asm volatile
	(
		"mov %0, eax\n"
		: "=r"(ret)
		:
	);

	return ret;
}

int _close(int fd)
{
	int ret;

	asm volatile
	(
		"mov edi, %0\n"

		"mov rax, 0x3\n"
		"syscall\n"
		:
		: "g"(fd)
	);
	asm volatile
	(
		"mov %0, eax\n"
		: "=r"(ret)
		:
	);

	return ret;
}

int _getdents64(unsigned int fd, struct linux_dirent64 *dirp, unsigned int count)
{
	int ret;

	asm volatile
	(
		"mov edi, %0\n"
		"mov rsi, %1\n"
		"mov edx, %2\n"

		"mov rax, 0xd9\n"
		"syscall\n"
		:
		: "g"(fd), "g"(dirp), "g"(count)
	);
	asm volatile
	(
		"mov %0, eax\n"
		: "=r"(ret)
		:
	);

	return ret;
}

ssize_t _getrandom(void *buf, size_t buflen, unsigned int flags)
{
	ssize_t ret;

	asm volatile
	(
		"mov rdi, %0\n"
		"mov rsi, %1\n"
		"mov edx, %2\n"

		"mov rax, 0x13e\n"
		"syscall\n"
		:
		: "g"(buf), "g"(buflen), "g"(flags)
	);
	asm volatile
	(
		"mov %0, rax\n"
		: "=r"(ret)
		:
	);

	return ret;
}

int _stat(const char *filename, struct stat *statbuf)
{
	int ret;

	asm volatile
	(
		"mov rdi, %0\n"
		"mov rsi, %1\n"

		"mov rax, 0x4\n"
		"syscall\n"
		:
		: "g"(filename), "g"(statbuf)
	);
	asm volatile
	(
		"mov %0, eax\n"
		: "=r"(ret)
		:
	);

	return ret;
}


void *_mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset)
{
	void *ret;

	asm volatile
	(
		"mov rdi, %0\n"
		"mov rsi, %1\n"
		"mov edx, %2\n"
		"mov r10d, %3\n"
		"mov r8d, %4\n"
		"mov r9, %5\n"

		"mov rax, 0x9\n"
		"syscall\n"
		:
		: "g"(addr), "g"(length), "g"(prot), "g"(flags), "g"(fd), "g"(offset)
		);
	asm volatile
	(
		"mov %0, rax\n"
		: "=r"(ret)
		:
	);

	return ret;
}

ssize_t _write(int fd, const void *buf, size_t count)
{
	ssize_t ret;

	asm volatile
	(
		"mov edi, %0\n"
		"mov rsi, %1\n"
		"mov rdx, %2\n"

		"mov rax, 0x1\n"
		"syscall\n"
		:
		: "g"(fd), "g"(buf), "g"(count)
	);
	asm volatile
	(
		"mov %0, rax\n"
		: "=r"(ret)
		:
	);

	return ret;
}

int _munmap(void *addr, size_t length)
{
	int ret = 0;

	asm volatile
	(
		"mov rdi, %0\n"
		"mov rsi, %1\n"

		"mov rax, 0xb\n"
		"syscall\n"
		:
		: "g"(addr), "g"(length)
	);
	asm volatile
	(
		"mov %0, eax\n"
		: "=r"(ret)
		:
	);

	return ret;
}

void _fatal(int exit)
{
	asm volatile
	(
	 	"mov	edi, %0\n"

	 	"mov rax, 0x3c\n"
		"syscall\n"
		:
		: "g"(exit)
	);
}

pid_t _fork(void)
{
	pid_t ret = 0;

	asm volatile
	(
		"mov rax, 0x39\n"
		"syscall\n"
	);
	asm volatile
	(
		"mov %0, eax\n"
		: "=r"(ret)
		:
	);

	return ret;
}

int _execve(const char *filename, char *const argv[], char *const envp[])
{
	int ret = 0;

	asm volatile
	(
		"mov rdi, %0\n"
		"mov rsi, %1\n"
		"mov rdx, %2\n"

		"mov rax, 0x3b\n"
		"syscall\n"
		:
		: "g"(filename), "g"(argv), "g"(envp)
	);
	asm volatile
	(
		"mov %0, eax\n"
		: "=r"(ret)
		:
	);

	return ret;
}

pid_t _wait4(pid_t pid, int *wstatus, int options, struct rusage *rusage)
{
	int ret = 0;

	asm volatile
	(
		"mov edi, %0\n"
		"mov rsi, %1\n"
		"mov edx, %2\n"
		"mov r10, %3\n"

		"mov rax, 0x3d\n"
		"syscall\n"
		:
		: "g"(pid), "g"(wstatus), "g"(options), "g"(rusage)
	);
	asm volatile
	(
		"mov %0, eax\n"
		: "=r"(ret)
		:
	);

	return ret;
}

long _ptrace(long request, long pid ,unsigned long addr, unsigned long data)
{
	long ret;

	asm volatile
	(
	 	"mov rdi, %0;"
		"mov rsi, %1;"
		"mov rdx, %2;"
		"mov r10, %3;"

		"mov rax, 101;"
		"syscall;"
		:
		: "g"(request), "g"(pid), "g"(addr), "g"(data)
	);
	asm volatile
	(
		"mov %0, rax"
		: "=r"(ret)
		:
	);

	return ret;
}

pid_t _getppid(void)
{
	pid_t ret;

	asm volatile
	(
	 	"mov rax, 0x6e\n"
		"syscall\n"
	);
	asm volatile
	(
	 	"mov %0, eax\n"
		: "=r"(ret)
		:
	);

	return ret;
}
