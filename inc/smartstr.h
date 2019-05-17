/* SPDX-License-Identifier: GPL-2.0 */

#ifndef SMARTSTR_H
# define SMARTSTR_H
# define SMARTSTR_SIZE 1024

typedef struct		s_smartstr
{
	char		*str;
	size_t		size;
	size_t		actual_size;
	size_t		pos;
}			t_smartstr;

t_smartstr		*init_smartstr(void);
void			append_ul(t_smartstr *sstr, unsigned long value);
void			append_bytes(t_smartstr *sstr, char *buff, size_t size);
void			join_smartstrs(t_smartstr *left, t_smartstr *right);
void			delete_smartstr(t_smartstr *sstr);
void			append_str(t_smartstr *sstr, char *str);
char			get_current(t_smartstr *sstr);
void			smartstr_move(t_smartstr *sstr, size_t mov);
unsigned long		get_ul(t_smartstr *sstr);

# endif
