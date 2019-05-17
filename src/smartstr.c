// SPDX-License-Identifier: GPL-2.0

#include <string.h>
#include <stdlib.h>
#include "smartstr.h"

static void		expand(t_smartstr *sstr)
{
	size_t		tmp;
	char		*b;

	tmp = sstr->size << 1;
	if (tmp == 0) {
		sstr->actual_size = 0;
		return;
	}
	sstr->size = tmp;
	b = (char *)realloc(sstr->str, sizeof(char) * sstr->size);
	if (b == 0) {
		sstr->actual_size = 0;
		return;
	}
	sstr->str = b;
}

t_smartstr		*init_smartstr(void)
{
	t_smartstr	*res;

	res = malloc(sizeof(t_smartstr));
	if (res == NULL)
		return NULL;
	res->size = 0;
	res->actual_size = 0;
	res->pos = 0;
	res->str = (char *)malloc(sizeof(char) * SMARTSTR_SIZE);
	if (res->str == NULL)
		return res;
	res->size = SMARTSTR_SIZE;
	return res;
}

void			append_ul(t_smartstr *sstr, unsigned long value)
{
	char		buff[sizeof(unsigned long)];
	unsigned long	mask;
	size_t		i;

	memset(buff, 0, sizeof(unsigned long));
	mask = 0xFF;
	mask <<= ((sizeof(unsigned long) - 1) << 3);
	for (i = 1; i <= sizeof(unsigned long); i++) {
		buff[sizeof(unsigned long) - i] = (char) ((value & mask) >>
				 ((sizeof(unsigned long) - i) << 3));
		mask >>= 0x08;
	}
	append_bytes(sstr, buff, sizeof(unsigned long));
}

void			append_bytes(t_smartstr *sstr, char *buff, size_t size)
{
	while (sstr->size < sstr->actual_size + size + 1) {
		expand(sstr);
		if (sstr->size == 0)
			return;
	}
	memcpy(sstr->str + sstr->actual_size, buff, size);
	sstr->actual_size += size;
	sstr->str[sstr->actual_size] = '\0';
}

void			join_smartstrs(t_smartstr *left, t_smartstr *right)
{
	append_bytes(left, right->str, right->actual_size);
}

void			delete_smartstr(t_smartstr *sstr)
{
	free(sstr->str);
	sstr->size = 0;
	sstr->actual_size = 0;
	sstr->pos = 0;
	free(sstr);
}

void			append_str(t_smartstr *sstr, char *str)
{
	append_bytes(sstr, str, strlen(str));
}

char			get_current(t_smartstr *sstr)
{
	return sstr->str[sstr->pos];
}

void			smartstr_move(t_smartstr *sstr, size_t mov)
{
	sstr->pos += mov;
}

unsigned long		get_ul(t_smartstr *sstr)
{
	unsigned long	res;
	size_t		i;

	res = 0;
	for (i = 1; i <= sizeof(unsigned long); i++) {
		res <<= 8;
		res += (unsigned char) sstr->str[sizeof(unsigned long) + sstr->pos - i];
	}
	sstr->pos += sizeof(unsigned long);
	return res;
}
