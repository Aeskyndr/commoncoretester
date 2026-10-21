/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_tests_simple.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkudelka <gkudelka@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2026/10/15 12:12:11 by gkudelka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*                                                                            */
/*   Better (the sanitizer finds hidden bugs: reading past a buffer,          */
/*   signed overflow, memory leaks, use after free):                          */
/*                                                                            */
/*     clang -g -fsanitize=address,undefined -I. libft_tests_simple.c ft_*.c  */
/*        -o tests                                                            */
/*     ./tests                                                                */
/*                                                                            */
/*   HOW TO READ THE OUTPUT:                                                  */
/*     ft_strlen      OK          -> every check of this function passed      */
/*     ft_strlen      KO          -> followed by the line(s) that failed      */
/*   The name of a function is printed BEFORE its test runs. If the program   */
/*   crashes, the last name on the screen is the function that crashed.       */
/*   To skip a test, put // in front of its line in main().                   */
/*                                                                            */
/*   The functions at the very end ("extras") pass NULL pointers and so on.   */
/*   The real libc crashes on those too, so the subject does not require      */
/*   them. If the program stops there, it is NOT a failure.                   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <ctype.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int	g_ko_total;
int	g_ko_here;

/* ------------------------------------------------------------------------- */
/*  Small helpers used by every test                                         */
/* ------------------------------------------------------------------------- */

/* Prints the name of the function we are about to test. */
static void	start(char *name)
{
	printf("%-20s", name);
	g_ko_here = 0;
}

/* If condition is false, the test is KO and we print what went wrong. */
static void	check(int condition, char *description)
{
	if (condition == 0)
	{
		if (g_ko_here == 0)
			printf("KO\n");
		printf("      -> %s\n", description);
		g_ko_here++;
		g_ko_total++;
	}
}

/* If nothing failed during this test, print OK. */
static void	end_test(void)
{
	if (g_ko_here == 0)
		printf("OK\n");
}

/* Returns -1, 0 or 1. Some functions only promise the SIGN of the result. */
static int	sign(int n)
{
	if (n < 0)
		return (-1);
	if (n > 0)
		return (1);
	return (0);
}

/* Compares a result string with the expected one (safe if got is NULL). */
static int	same(char *got, char *expected)
{
	if (got == NULL)
		return (0);
	if (strcmp(got, expected) == 0)
		return (1);
	return (0);
}

/* ========================================================================= */
/*  PART 1 - libc functions                                                  */
/* ========================================================================= */

static void	test_is_and_to(void)
{
	int	c;

	start("ft_is*, ft_to*");
	check(ft_isalpha('a') == 1, "ft_isalpha('a') must return exactly 1");
	check(ft_isalpha('z') == 1 && ft_isalpha('A') == 1 && ft_isalpha('Z') == 1,
		"ft_isalpha must accept a z A Z");
	check(ft_isalpha('@') == 0 && ft_isalpha('[') == 0,
		"ft_isalpha: '@' and '[' are just next to the capitals, not letters");
	check(ft_isalpha('`') == 0 && ft_isalpha('{') == 0,
		"ft_isalpha: '`' and '{' are just next to the lowercase, not letters");
	check(ft_isdigit('0') == 1 && ft_isdigit('9') == 1, "ft_isdigit 0 and 9");
	check(ft_isdigit('/') == 0 && ft_isdigit(':') == 0,
		"ft_isdigit: '/' and ':' are just next to the digits");
	check(ft_isalnum('a') == 1 && ft_isalnum('5') == 1, "ft_isalnum a and 5");
	check(ft_isalnum('_') == 0 && ft_isalnum(' ') == 0, "ft_isalnum '_' ' '");
	check(ft_isascii(0) == 1 && ft_isascii(127) == 1, "ft_isascii 0 and 127");
	check(ft_isascii(128) == 0 && ft_isascii(-1) == 0,
		"ft_isascii: 128 and -1 are not ascii");
	check(ft_isprint(31) == 0 && ft_isprint(32) == 1,
		"ft_isprint: 31 is not printable, 32 (space) is");
	check(ft_isprint(126) == 1 && ft_isprint(127) == 0,
		"ft_isprint: 126 (~) is printable, 127 (DEL) is not");
	check(ft_toupper('a') == 'A' && ft_toupper('z') == 'Z', "ft_toupper a z");
	check(ft_toupper('A') == 'A' && ft_toupper('1') == '1',
		"ft_toupper must not change non lowercase letters");
	check(ft_toupper('`') == '`' && ft_toupper('{') == '{',
		"ft_toupper: characters next to a..z must not change");
	check(ft_tolower('A') == 'a' && ft_tolower('Z') == 'z', "ft_tolower A Z");
	check(ft_tolower('a') == 'a' && ft_tolower('1') == '1',
		"ft_tolower must not change non uppercase letters");
	check(ft_tolower('@') == '@' && ft_tolower('[') == '[',
		"ft_tolower: characters next to A..Z must not change");
	c = -1;
	while (c <= 255)
	{
		check((ft_isalpha(c) != 0) == (isalpha(c) != 0),
			"ft_isalpha differs from the real isalpha for a value in -1..255");
		check((ft_isdigit(c) != 0) == (isdigit(c) != 0),
			"ft_isdigit differs from the real isdigit for a value in -1..255");
		check((ft_isalnum(c) != 0) == (isalnum(c) != 0),
			"ft_isalnum differs from the real isalnum for a value in -1..255");
		check((ft_isprint(c) != 0) == (isprint(c) != 0),
			"ft_isprint differs from the real isprint for a value in -1..255");
		check(ft_toupper(c) == toupper(c),
			"ft_toupper differs from the real toupper for a value in -1..255");
		check(ft_tolower(c) == tolower(c),
			"ft_tolower differs from the real tolower for a value in -1..255");
		c++;
	}
	end_test();
}

static void	test_strlen(void)
{
	char	with_nul[5];

	start("ft_strlen");
	check(ft_strlen("") == 0, "empty string must give 0");
	check(ft_strlen("a") == 1, "one character");
	check(ft_strlen("hello") == 5, "hello");
	with_nul[0] = 'a';
	with_nul[1] = 'b';
	with_nul[2] = '\0';
	with_nul[3] = 'c';
	with_nul[4] = '\0';
	check(ft_strlen(with_nul) == 2, "must stop at the first '\\0'");
	end_test();
}

static void	test_memset_bzero(void)
{
	char	buf[10];

	start("ft_memset");
	memset(buf, 'x', 10);
	check(ft_memset(buf, 'a', 3) == buf, "must return the first argument");
	check(buf[0] == 'a' && buf[2] == 'a' && buf[3] == 'x',
		"must fill exactly 3 bytes and stop");
	memset(buf, 'x', 10);
	ft_memset(buf, 'a', 0);
	check(buf[0] == 'x', "len 0 must not touch anything");
	ft_memset(buf, 321, 2);
	check(buf[0] == 'A' && buf[1] == 'A',
		"the value is converted to unsigned char (321 -> 65 -> 'A')");
	ft_memset(buf, -1, 1);
	check((unsigned char)buf[0] == 255, "-1 becomes the byte 255");
	end_test();
	start("ft_bzero");
	memset(buf, 'x', 10);
	ft_bzero(buf, 3);
	check(buf[0] == 0 && buf[2] == 0 && buf[3] == 'x',
		"must zero exactly 3 bytes");
	memset(buf, 'x', 10);
	ft_bzero(buf, 0);
	check(buf[0] == 'x', "len 0 must not touch anything");
	end_test();
}

static void	test_memcpy(void)
{
	char	dst[10];

	start("ft_memcpy");
	memset(dst, 'x', 10);
	check(ft_memcpy(dst, "abcde", 5) == dst, "must return dest");
	check(memcmp(dst, "abcde", 5) == 0 && dst[5] == 'x',
		"must copy exactly 5 bytes");
	memset(dst, 'x', 10);
	ft_memcpy(dst, "abc", 0);
	check(dst[0] == 'x', "n = 0 must copy nothing");
	memset(dst, 'x', 10);
	ft_memcpy(dst, "ab\0cd", 5);
	check(dst[3] == 'c' && dst[4] == 'd',
		"must not stop at '\\0' (it copies bytes, not a string)");
	end_test();
}

static void	test_memmove(void)
{
	char	buf[20];

	start("ft_memmove");
	strcpy(buf, "0123456789");
	check(ft_memmove(buf + 2, buf, 5) == buf + 2, "must return dest");
	check(strcmp(buf, "0101234789") == 0,
		"overlap, dest AFTER src: must copy from the end (expected 0101234789)");
	strcpy(buf, "0123456789");
	ft_memmove(buf, buf + 2, 5);
	check(strcmp(buf, "2345656789") == 0,
		"overlap, dest BEFORE src: copy from the start (expected 2345656789)");
	strcpy(buf, "0123456789");
	ft_memmove(buf, buf, 10);
	check(strcmp(buf, "0123456789") == 0, "same pointer: nothing changes");
	ft_memmove(buf + 1, buf, 0);
	check(strcmp(buf, "0123456789") == 0, "n = 0: nothing changes");
	end_test();
}

static void	test_strlcpy(void)
{
	char	dst[20];

	start("ft_strlcpy");
	memset(dst, '#', 20);
	check(ft_strlcpy(dst, "hello", 0) == 5, "size 0: return strlen(src) = 5");
	check(dst[0] == '#', "size 0: dst must not be touched at all");
	memset(dst, '#', 20);
	check(ft_strlcpy(dst, "hello", 1) == 5, "size 1: still returns 5");
	check(dst[0] == '\0' && dst[1] == '#',
		"size 1: only room for the '\\0', nothing else is written");
	memset(dst, '#', 20);
	ft_strlcpy(dst, "hello", 3);
	check(strcmp(dst, "he") == 0 && dst[3] == '#',
		"size 3: copies 'he' + '\\0' and writes nothing after");
	memset(dst, '#', 20);
	check(ft_strlcpy(dst, "hello", 5) == 5, "size 5: returns 5");
	check(strcmp(dst, "hell") == 0, "size 5: 'hell' + '\\0' (no room for o)");
	memset(dst, '#', 20);
	ft_strlcpy(dst, "hello", 6);
	check(strcmp(dst, "hello") == 0 && dst[6] == '#',
		"size 6: exactly fits, 'hello' + '\\0'");
	memset(dst, '#', 20);
	ft_strlcpy(dst, "hello", 20);
	check(strcmp(dst, "hello") == 0 && dst[6] == '#',
		"big size: copies the string and does NOT fill the rest");
	memset(dst, '#', 20);
	check(ft_strlcpy(dst, "", 5) == 0 && dst[0] == '\0',
		"empty src: returns 0 and dst becomes an empty string");
	end_test();
}

static void	test_strlcat(void)
{
	char	dst[20];

	start("ft_strlcat");
	memset(dst, '#', 20);
	strcpy(dst, "abcd");
	check(ft_strlcat(dst, "efgh", 0) == 4,
		"size 0: returns strlen(src) = 4 and changes nothing");
	check(strcmp(dst, "abcd") == 0, "size 0: dst unchanged");
	check(ft_strlcat(dst, "efgh", 4) == 8,
		"size 4 (= strlen(dst)): returns 4 + 4 = 8");
	check(strcmp(dst, "abcd") == 0, "size 4: dst unchanged");
	check(ft_strlcat(dst, "efgh", 5) == 8, "size 5: returns 8");
	check(strcmp(dst, "abcd") == 0, "size 5: no room, dst unchanged");
	check(ft_strlcat(dst, "efgh", 7) == 8, "size 7: returns 8");
	check(strcmp(dst, "abcdef") == 0, "size 7: 'abcdef' + '\\0'");
	strcpy(dst, "abcd");
	check(ft_strlcat(dst, "efgh", 9) == 8, "size 9: returns 8");
	check(strcmp(dst, "abcdefgh") == 0, "size 9: everything fits exactly");
	strcpy(dst, "abcd");
	ft_strlcat(dst, "efgh", 20);
	check(strcmp(dst, "abcdefgh") == 0 && dst[9] == '#',
		"big size: result is abcdefgh and nothing is written after it");
	strcpy(dst, "");
	check(ft_strlcat(dst, "xy", 10) == 2 && strcmp(dst, "xy") == 0,
		"empty dst: acts like a copy");
	strcpy(dst, "abcd");
	check(ft_strlcat(dst, "", 10) == 4 && strcmp(dst, "abcd") == 0,
		"empty src: nothing changes, returns 4");
	memset(dst, '#', 20);
	strcpy(dst, "abcdefg");
	check(ft_strlcat(dst, "xyz", 3) == 6,
		"size smaller than strlen(dst): returns size + strlen(src) = 3 + 3");
	check(strcmp(dst, "abcdefg") == 0,
		"size smaller than strlen(dst): dst must NOT be touched");
	end_test();
}

static void	test_strchr(void)
{
	char	*s;

	start("ft_strchr");
	s = "hello";
	check(ft_strchr(s, 'h') == s, "first character");
	check(ft_strchr(s, 'l') == s + 2, "must return the FIRST 'l'");
	check(ft_strchr(s, 'o') == s + 4, "last character");
	check(ft_strchr(s, 'z') == NULL, "not found -> NULL");
	check(ft_strchr(s, '\0') == s + 5,
		"searching '\\0' must return a pointer to the end of the string");
	check(ft_strchr("", '\0') != NULL, "empty string, searching '\\0'");
	check(ft_strchr("", 'a') == NULL, "empty string, searching 'a'");
	check(ft_strchr(s, 'l' + 256) == s + 2,
		"c is converted to char: 'l' + 256 behaves like 'l'");
	end_test();
}

static void	test_strrchr(void)
{
	char	*s;

	start("ft_strrchr");
	s = "hello";
	check(ft_strrchr(s, 'l') == s + 3, "must return the LAST 'l'");
	check(ft_strrchr(s, 'h') == s, "only at index 0 (loop must not skip it)");
	check(ft_strrchr(s, 'o') == s + 4, "last character");
	check(ft_strrchr(s, 'z') == NULL, "not found -> NULL");
	check(ft_strrchr(s, '\0') == s + 5, "searching '\\0' -> end of the string");
	check(ft_strrchr("", '\0') != NULL, "empty string, searching '\\0'");
	check(ft_strrchr("", 'a') == NULL, "empty string, searching 'a'");
	end_test();
}

static void	test_strncmp(void)
{
	start("ft_strncmp");
	check(ft_strncmp("abc", "abc", 3) == 0, "equal strings");
	check(ft_strncmp("abc", "abd", 3) < 0, "'c' < 'd' -> negative");
	check(ft_strncmp("abd", "abc", 3) > 0, "'d' > 'c' -> positive");
	check(ft_strncmp("abc", "abd", 2) == 0, "n = 2: only 'ab' is compared");
	check(ft_strncmp("abc", "xyz", 0) == 0, "n = 0 always gives 0");
	check(ft_strncmp("abc", "ab", 5) > 0, "s1 longer than s2 -> positive");
	check(ft_strncmp("ab", "abc", 5) < 0, "s1 shorter than s2 -> negative");
	check(ft_strncmp("", "", 5) == 0, "two empty strings");
	check(ft_strncmp("abc", "abc", 100) == 0,
		"n bigger than the strings: must stop at '\\0'");
	check(sign(ft_strncmp("\xff", "\x01", 1)) == 1,
		"characters are compared as UNSIGNED char (255 > 1)");
	check(sign(ft_strncmp("a\0b", "a\0c", 3)) == 0,
		"must stop at the first '\\0', even if n is bigger");
	end_test();
}

static void	test_memchr(void)
{
	char	buf[6];

	start("ft_memchr");
	buf[0] = 'a';
	buf[1] = 'b';
	buf[2] = '\0';
	buf[3] = 'c';
	buf[4] = 'd';
	buf[5] = 'c';
	check(ft_memchr(buf, 'a', 6) == buf, "first byte");
	check(ft_memchr(buf, 'c', 6) == buf + 3, "must return the FIRST 'c'");
	check(ft_memchr(buf, 'd', 4) == NULL, "'d' is at index 4, outside n = 4");
	check(ft_memchr(buf, 'd', 5) == buf + 4, "n = 5 includes index 4");
	check(ft_memchr(buf, '\0', 6) == buf + 2, "must find '\\0' in the middle");
	check(ft_memchr(buf, 'a', 0) == NULL, "n = 0 never finds anything");
	check(ft_memchr(buf, 'z', 6) == NULL, "not found -> NULL");
	check(ft_memchr(buf, 'a' + 256, 6) == buf,
		"c is converted to unsigned char: 'a' + 256 behaves like 'a'");
	end_test();
}

static void	test_memcmp(void)
{
	start("ft_memcmp");
	check(ft_memcmp("abc", "abc", 3) == 0, "equal");
	check(ft_memcmp("abc", "abd", 3) < 0, "negative");
	check(ft_memcmp("abd", "abc", 3) > 0, "positive");
	check(ft_memcmp("abc", "abd", 2) == 0, "n = 2: only 'ab' is compared");
	check(ft_memcmp("abc", "xyz", 0) == 0, "n = 0 always gives 0");
	check(ft_memcmp("a\0b", "a\0c", 3) < 0,
		"must NOT stop at '\\0' (it compares bytes, not strings)");
	check(sign(ft_memcmp("\xff", "\x01", 1)) == 1,
		"bytes are compared as UNSIGNED (255 > 1)");
	end_test();
}

static void	test_strnstr(void)
{
	char	*big;
	char	*buf;

	start("ft_strnstr");
	big = "hello world";
	check(ft_strnstr(big, "", 0) == big, "empty little -> returns big (len 0)");
	check(ft_strnstr(big, "", 5) == big, "empty little -> returns big");
	check(ft_strnstr(big, "hello", 11) == big, "found at the start");
	check(ft_strnstr(big, "world", 11) == big + 6, "found at the end");
	check(ft_strnstr(big, "world", 10) == NULL,
		"len 10 cuts 'world' (needs 11): NULL");
	check(ft_strnstr(big, "hello", 4) == NULL, "len 4 cuts 'hello': NULL");
	check(ft_strnstr(big, "hello", 0) == NULL, "len 0: nothing can be found");
	check(ft_strnstr(big, "xyz", 11) == NULL, "not found");
	check(ft_strnstr("abc", "abcd", 10) == NULL,
		"little longer than big: NULL, and no reading past the '\\0'");
	check(ft_strnstr("", "a", 5) == NULL, "empty big");
	check(ft_strnstr("", "", 5) != NULL, "empty big and empty little");
	check(ft_strnstr("aaab", "aab", 4) != NULL,
		"partial match first ('aaa' then 'aab'): must try the next position");
	check(ft_strnstr(big, "hello", SIZE_MAX) == big,
		"huge len: must stop at the '\\0' of big");
	check(ft_strnstr(big, "xyz", SIZE_MAX) == NULL, "huge len, not found");
	buf = malloc(3);
	memcpy(buf, "abc", 3);
	check(ft_strnstr(buf, "bc", 3) == buf + 1, "buffer WITHOUT '\\0' (found)");
	check(ft_strnstr(buf, "zz", 3) == NULL,
		"buffer WITHOUT '\\0': never read big[len]! (only visible with "
		"-fsanitize=address)");
	free(buf);
	end_test();
}

static void	test_atoi(void)
{
	char	*strings[20];
	int		i;

	start("ft_atoi");
	strings[0] = "0";
	strings[1] = "42";
	strings[2] = "-42";
	strings[3] = "+42";
	strings[4] = " \t\n\v\f\r 42";
	strings[5] = "  -  4";
	strings[6] = "--1";
	strings[7] = "+-1";
	strings[8] = "12abc";
	strings[9] = "abc";
	strings[10] = "";
	strings[11] = "-";
	strings[12] = "+";
	strings[13] = "0000000012";
	strings[14] = "- 5";
	strings[15] = "1 2";
	strings[16] = "-0";
	strings[17] = "   -12";
	strings[18] = "2147483647";
	strings[19] = "-2147483648";
	i = 0;
	while (i < 20)
	{
		if (ft_atoi(strings[i]) != atoi(strings[i]))
		{
			printf("KO\n      -> ft_atoi(\"%s\") gives %d, atoi gives %d\n",
				strings[i], ft_atoi(strings[i]), atoi(strings[i]));
			g_ko_here++;
			g_ko_total++;
		}
		i++;
	}
	check(ft_atoi("2147483647") == INT_MAX, "INT_MAX");
	check(ft_atoi("-2147483648") == INT_MIN,
		"INT_MIN (careful: 2147483648 does not fit in an int!)");
	end_test();
}

static void	test_calloc(void)
{
	char	*p;
	int		i;
	int		all_zero;
	void	*a;
	void	*b;

	start("ft_calloc");
	p = ft_calloc(10, 4);
	check(p != NULL, "10 * 4 bytes must work");
	all_zero = 1;
	i = 0;
	while (p != NULL && i < 40)
	{
		if (p[i] != 0)
			all_zero = 0;
		i++;
	}
	check(all_zero == 1, "all 40 bytes must be zero");
	free(p);
	a = ft_calloc(0, 5);
	check(a != NULL, "nmemb = 0 must still return a pointer that can be freed");
	b = ft_calloc(5, 0);
	check(b != NULL, "size = 0 must still return a pointer that can be freed");
	check(a != b, "the two pointers must be different (unique)");
	free(a);
	free(b);
	a = ft_calloc(0, 0);
	check(a != NULL, "0 * 0 must also return a pointer that can be freed");
	free(a);
	check(ft_calloc(SIZE_MAX, 2) == NULL, "SIZE_MAX * 2 overflows -> NULL");
	check(ft_calloc(2, SIZE_MAX) == NULL, "2 * SIZE_MAX overflows -> NULL");
	check(ft_calloc(SIZE_MAX / 2 + 1, 2) == NULL,
		"the smallest overflowing product -> NULL");
	end_test();
}

static void	test_strdup(void)
{
	char	original[6];
	char	*copy;

	start("ft_strdup");
	strcpy(original, "hello");
	copy = ft_strdup(original);
	check(copy != NULL && copy != original, "must return a NEW pointer");
	check(same(copy, "hello"), "same content");
	original[0] = 'J';
	check(same(copy, "hello"), "the copy must be independent of the original");
	free(copy);
	copy = ft_strdup("");
	check(same(copy, ""), "empty string -> a new empty string (not NULL)");
	free(copy);
	end_test();
}

/* ========================================================================= */
/*  PART 2 - additional functions                                            */
/* ========================================================================= */

static void	test_substr(void)
{
	char	*p;

	start("ft_substr");
	p = ft_substr("hello", 1, 3);
	check(same(p, "ell"), "(\"hello\", 1, 3) -> ell");
	free(p);
	p = ft_substr("hello", 0, 5);
	check(same(p, "hello"), "(\"hello\", 0, 5) -> whole string");
	free(p);
	p = ft_substr("hello", 0, 100);
	check(same(p, "hello"), "len bigger than the string: stops at the end");
	free(p);
	p = ft_substr("hello", 3, 100);
	check(same(p, "lo"), "(\"hello\", 3, 100) -> lo");
	free(p);
	p = ft_substr("hello", 5, 3);
	check(same(p, ""), "start == strlen: empty string (not NULL)");
	free(p);
	p = ft_substr("hello", 100, 3);
	check(same(p, ""), "start after the end: empty string (not NULL)");
	free(p);
	p = ft_substr("hello", 1, 0);
	check(same(p, ""), "len 0: empty string");
	free(p);
	p = ft_substr("hello", UINT_MAX, 3);
	check(same(p, ""), "start = UINT_MAX: empty string, no crash");
	free(p);
	p = ft_substr("hello", 1, SIZE_MAX);
	check(same(p, "ello"), "len = SIZE_MAX: no overflow when adding start");
	free(p);
	p = ft_substr("", 0, 5);
	check(same(p, ""), "empty source string");
	free(p);
	end_test();
}

static void	test_strjoin(void)
{
	char	*p;

	start("ft_strjoin");
	p = ft_strjoin("hello ", "world");
	check(same(p, "hello world"), "normal case");
	free(p);
	p = ft_strjoin("", "world");
	check(same(p, "world"), "empty s1");
	free(p);
	p = ft_strjoin("hello", "");
	check(same(p, "hello"), "empty s2");
	free(p);
	p = ft_strjoin("", "");
	check(same(p, ""), "both empty: empty string (not NULL)");
	free(p);
	end_test();
}

static void	test_strtrim(void)
{
	char	*p;

	start("ft_strtrim");
	p = ft_strtrim("  hi  ", " ");
	check(same(p, "hi"), "spaces on both sides");
	free(p);
	p = ft_strtrim("xxhixx", "x");
	check(same(p, "hi"), "another set");
	free(p);
	p = ft_strtrim("abcba", "ab");
	check(same(p, "c"), "set of several characters, both sides");
	free(p);
	p = ft_strtrim("  a b  ", " ");
	check(same(p, "a b"), "must keep the space in the middle");
	free(p);
	p = ft_strtrim("   ", " ");
	check(same(p, ""), "everything trimmed: empty string (not NULL)");
	free(p);
	p = ft_strtrim("abc", "abc");
	check(same(p, ""), "everything is in the set: empty string");
	free(p);
	p = ft_strtrim("", " ");
	check(same(p, ""), "empty s1");
	free(p);
	p = ft_strtrim("abc", "");
	check(same(p, "abc"), "empty set: nothing trimmed");
	free(p);
	p = ft_strtrim("abc", "xyz");
	check(same(p, "abc"), "set not found in s1: nothing trimmed");
	free(p);
	p = ft_strtrim("xhix", "x");
	check(same(p, "hi"), "one char on each side");
	free(p);
	end_test();
}

/* Splits s with c, and compares the result with the expected NULL-ended
   array of strings. Returns 1 if they are the same. Frees the result. */
static int	split_is(char *s, char c, char **expected)
{
	char	**result;
	int		i;
	int		ok;

	result = ft_split(s, c);
	if (result == NULL)
		return (0);
	ok = 1;
	i = 0;
	while (expected[i] != NULL)
	{
		if (result[i] == NULL || strcmp(result[i], expected[i]) != 0)
		{
			ok = 0;
			break ;
		}
		i++;
	}
	if (ok == 1 && result[i] != NULL)
		ok = 0;
	i = 0;
	while (result[i] != NULL)
	{
		free(result[i]);
		i++;
	}
	free(result);
	return (ok);
}

static void	test_split(void)
{
	char	*none[1];
	char	*one[2];
	char	*two[3];
	char	*three[4];

	start("ft_split");
	none[0] = NULL;
	one[0] = "abc";
	one[1] = NULL;
	two[0] = "hello";
	two[1] = "world";
	two[2] = NULL;
	three[0] = "a";
	three[1] = "b";
	three[2] = "c";
	three[3] = NULL;
	check(split_is("", ' ', none), "empty string -> empty array (just NULL)");
	check(split_is("     ", ' ', none), "only separators -> empty array");
	check(split_is("abc", ' ', one), "no separator -> one string");
	check(split_is("hello world", ' ', two), "two words");
	check(split_is("  hello   world  ", ' ', two),
		"extra separators at the start, middle and end");
	check(split_is("a b c", ' ', three), "three words");
	check(split_is("a,b,,c,", ',', three), "another separator, some doubled");
	check(split_is(",,,a,b,c", ',', three), "separators only at the start");
	check(split_is("a b c   ", ' ', three), "separators only at the end");
	check(split_is("abc", '\0', one),
		"separator '\\0': the whole string is one word");
	end_test();
}

static void	test_itoa(void)
{
	int		numbers[13];
	char	*texts[13];
	char	*p;
	int		i;

	start("ft_itoa");
	numbers[0] = 0;
	texts[0] = "0";
	numbers[1] = 1;
	texts[1] = "1";
	numbers[2] = -1;
	texts[2] = "-1";
	numbers[3] = 9;
	texts[3] = "9";
	numbers[4] = 10;
	texts[4] = "10";
	numbers[5] = -10;
	texts[5] = "-10";
	numbers[6] = 100;
	texts[6] = "100";
	numbers[7] = 12345;
	texts[7] = "12345";
	numbers[8] = -12345;
	texts[8] = "-12345";
	numbers[9] = 2147483647;
	texts[9] = "2147483647";
	numbers[10] = -2147483647;
	texts[10] = "-2147483647";
	numbers[11] = INT_MIN;
	texts[11] = "-2147483648";
	numbers[12] = 1000000000;
	texts[12] = "1000000000";
	i = 0;
	while (i < 13)
	{
		p = ft_itoa(numbers[i]);
		if (same(p, texts[i]) == 0)
		{
			printf("KO\n      -> ft_itoa(%d) should be \"%s\"\n", numbers[i],
				texts[i]);
			g_ko_here++;
			g_ko_total++;
		}
		free(p);
		i++;
	}
	end_test();
}

/* Functions given to ft_strmapi / ft_striteri for the tests */
static char	map_to_upper(unsigned int i, char c)
{
	(void)i;
	if (c >= 'a' && c <= 'z')
		return (c - 32);
	return (c);
}

static char	map_index_digit(unsigned int i, char c)
{
	(void)c;
	return ('0' + i);
}

static void	iter_to_upper(unsigned int i, char *c)
{
	(void)i;
	if (*c >= 'a' && *c <= 'z')
		*c = *c - 32;
}

static void	iter_index_digit(unsigned int i, char *c)
{
	*c = '0' + i;
}

static void	test_strmapi(void)
{
	char	original[4];
	char	*p;

	start("ft_strmapi");
	strcpy(original, "abc");
	p = ft_strmapi(original, map_to_upper);
	check(same(p, "ABC"), "(\"abc\", to_upper) -> ABC");
	check(strcmp(original, "abc") == 0, "the original must NOT be modified");
	free(p);
	p = ft_strmapi("xxxxx", map_index_digit);
	check(same(p, "01234"), "f receives the index as first argument");
	free(p);
	p = ft_strmapi("", map_to_upper);
	check(same(p, ""), "empty string -> empty string (not NULL)");
	free(p);
	end_test();
}

static void	test_striteri(void)
{
	char	s1[4];
	char	s2[6];
	char	empty[1];

	start("ft_striteri");
	strcpy(s1, "abc");
	ft_striteri(s1, iter_to_upper);
	check(strcmp(s1, "ABC") == 0, "the string itself must be modified");
	strcpy(s2, "xxxxx");
	ft_striteri(s2, iter_index_digit);
	check(strcmp(s2, "01234") == 0, "f receives the index as first argument");
	empty[0] = '\0';
	ft_striteri(empty, iter_to_upper);
	check(empty[0] == '\0', "empty string: nothing happens, no crash");
	end_test();
}

/* These four print on the screen: compare "expected" and "got" with your eyes */
static void	test_put_fd(void)
{
	printf("\n--- ft_put*_fd: check with your eyes (fd 1 = screen) ---\n");
	printf("ft_putchar_fd expected: x\n                got: ");
	ft_putchar_fd('x', 1);
	printf("\nft_putstr_fd  expected: hello world\n                got: ");
	ft_putstr_fd("hello world", 1);
	printf("\nft_putstr_fd  expected: (nothing between the brackets) []\n");
	printf("                got: [");
	ft_putstr_fd("", 1);
	printf("]\n");
	printf("ft_putendl_fd expected: hi + a new line, then 'next'\n");
	printf("                got: ");
	ft_putendl_fd("hi", 1);
	printf("next\n");
	printf("ft_putnbr_fd  expected: 0 -1 42 -42 2147483647 -2147483648\n");
	printf("                got: ");
	ft_putnbr_fd(0, 1);
	printf(" ");
	ft_putnbr_fd(-1, 1);
	printf(" ");
	ft_putnbr_fd(42, 1);
	printf(" ");
	ft_putnbr_fd(-42, 1);
	printf(" ");
	ft_putnbr_fd(2147483647, 1);
	printf(" ");
	ft_putnbr_fd(INT_MIN, 1);
	printf("\n--- end of ft_put*_fd ---\n\n");
}

/* ========================================================================= */
/*  PART 3 - linked lists                                                    */
/* ========================================================================= */

/* Builds the list "a" -> "b" -> "c" (each content is a malloc'd string) */
static t_list	*make_list(void)
{
	t_list	*list;

	list = NULL;
	ft_lstadd_back(&list, ft_lstnew(ft_strdup("a")));
	ft_lstadd_back(&list, ft_lstnew(ft_strdup("b")));
	ft_lstadd_back(&list, ft_lstnew(ft_strdup("c")));
	return (list);
}

/* Function given to ft_lstiter: makes the string uppercase */
static void	upper_content(void *content)
{
	char	*str;
	int		i;

	str = content;
	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] >= 'a' && str[i] <= 'z')
			str[i] = str[i] - 32;
		i++;
	}
}

/* Function given to ft_lstmap: returns an uppercase COPY of the string */
static void	*upper_copy(void *content)
{
	char	*copy;

	copy = ft_strdup(content);
	if (copy == NULL)
		return (NULL);
	upper_content(copy);
	return (copy);
}

static void	test_lstnew(void)
{
	t_list	*node;
	int		x;

	start("ft_lstnew");
	x = 42;
	node = ft_lstnew(&x);
	check(node != NULL, "must return a node");
	check(node->content == &x, "content must be the given pointer");
	check(node->next == NULL, "next must be NULL");
	free(node);
	node = ft_lstnew(NULL);
	check(node != NULL && node->content == NULL && node->next == NULL,
		"NULL content is allowed");
	free(node);
	end_test();
}

static void	test_lstadd_front(void)
{
	t_list	*list;
	t_list	*first;
	t_list	*second;

	start("ft_lstadd_front");
	list = NULL;
	first = ft_lstnew(NULL);
	ft_lstadd_front(&list, first);
	check(list == first, "adding to an EMPTY list: list becomes the new node");
	check(first->next == NULL, "the only node has next == NULL");
	second = ft_lstnew(NULL);
	ft_lstadd_front(&list, second);
	check(list == second, "the new node is now the head");
	check(second->next == first, "the new node points to the old head");
	free(first);
	free(second);
	end_test();
}

static void	test_lstadd_back(void)
{
	t_list	*list;
	t_list	*node;

	start("ft_lstadd_back");
	list = NULL;
	node = ft_lstnew(ft_strdup("x"));
	ft_lstadd_back(&list, node);
	check(list == node, "adding to an EMPTY list: list becomes the new node");
	ft_lstclear(&list, free);
	list = make_list();
	node = ft_lstnew(ft_strdup("d"));
	ft_lstadd_back(&list, node);
	check(ft_lstlast(list) == node, "the new node must be the last one");
	check(node->next == NULL, "the new last node must have next == NULL");
	check(ft_lstsize(list) == 4, "the list must now have 4 nodes");
	check(same(list->content, "a"), "the head must not change");
	ft_lstclear(&list, free);
	end_test();
}

static void	test_lstsize(void)
{
	t_list	*list;

	start("ft_lstsize");
	check(ft_lstsize(NULL) == 0, "NULL list -> 0");
	list = ft_lstnew(NULL);
	check(ft_lstsize(list) == 1, "one node -> 1");
	free(list);
	list = make_list();
	check(ft_lstsize(list) == 3, "three nodes -> 3");
	ft_lstclear(&list, free);
	end_test();
}

static void	test_lstlast(void)
{
	t_list	*list;
	t_list	*single;

	start("ft_lstlast");
	check(ft_lstlast(NULL) == NULL, "NULL list -> NULL");
	single = ft_lstnew(NULL);
	check(ft_lstlast(single) == single, "one node -> that node");
	free(single);
	list = make_list();
	check(ft_lstlast(list) != NULL && same(ft_lstlast(list)->content, "c"),
		"must return the last node (content \"c\")");
	ft_lstclear(&list, free);
	end_test();
}

static void	test_lstdelone(void)
{
	t_list	*list;
	t_list	*second;

	start("ft_lstdelone");
	list = make_list();
	second = list->next;
	ft_lstdelone(list, free);
	check(same(second->content, "b"),
		"must NOT free the next node (its content must still be \"b\")");
	check(second->next != NULL && same(second->next->content, "c"),
		"the rest of the list must still be intact");
	ft_lstclear(&second, free);
	end_test();
}

static void	test_lstclear(void)
{
	t_list	*list;

	start("ft_lstclear");
	list = make_list();
	ft_lstclear(&list, free);
	check(list == NULL, "the pointer to the list must be set to NULL");
	ft_lstclear(&list, free);
	check(list == NULL, "clearing an already empty list: no crash");
	list = ft_lstnew(ft_strdup("one"));
	ft_lstclear(&list, free);
	check(list == NULL, "a list of one node");
	end_test();
}

static void	test_lstiter(void)
{
	t_list	*list;

	start("ft_lstiter");
	list = make_list();
	ft_lstiter(list, upper_content);
	check(same(list->content, "A"), "first node must be \"A\"");
	check(same(list->next->content, "B"), "second node must be \"B\"");
	check(same(list->next->next->content, "C"), "third node must be \"C\"");
	ft_lstclear(&list, free);
	end_test();
}

static void	test_lstmap(void)
{
	t_list	*list;
	t_list	*mapped;

	start("ft_lstmap");
	list = make_list();
	mapped = ft_lstmap(list, upper_copy, free);
	check(mapped != NULL, "must return a new list");
	check(mapped != list, "the new list must be a different list");
	check(ft_lstsize(mapped) == 3, "same number of nodes");
	check(same(mapped->content, "A") && same(mapped->next->content, "B")
		&& same(mapped->next->next->content, "C"),
		"new contents must be A, B, C in the same order");
	check(same(list->content, "a") && same(list->next->content, "b"),
		"the ORIGINAL list must not be modified");
	ft_lstclear(&mapped, free);
	ft_lstclear(&list, free);
	end_test();
}

/* ========================================================================= */
/*  EXTRAS: not required by the subject (the real libc crashes on these).    */
/*  If the program stops in one of them, it is NOT a failure.                */
/* ========================================================================= */

static void	test_extras(void)
{
	t_list	*list;
	char	str[4];

	printf("\n--- EXTRAS (NULL pointers...): a crash here is allowed ---\n");
	start("NULL string args");
	check(ft_substr(NULL, 0, 1) == NULL, "ft_substr(NULL, ...) -> NULL");
	check(ft_strjoin(NULL, "a") == NULL, "ft_strjoin(NULL, \"a\") -> NULL");
	check(ft_strjoin("a", NULL) == NULL, "ft_strjoin(\"a\", NULL) -> NULL");
	check(ft_strtrim(NULL, "a") == NULL, "ft_strtrim(NULL, ...) -> NULL");
	check(ft_strtrim("a", NULL) == NULL, "ft_strtrim(..., NULL) -> NULL");
	check(ft_split(NULL, ' ') == NULL, "ft_split(NULL, ...) -> NULL");
	check(ft_strmapi(NULL, map_to_upper) == NULL, "ft_strmapi(NULL, f) -> NULL");
	check(ft_strmapi("a", NULL) == NULL, "ft_strmapi(s, NULL) -> NULL");
	end_test();
	start("NULL function args");
	strcpy(str, "abc");
	ft_striteri(NULL, iter_to_upper);
	ft_striteri(str, NULL);
	ft_lstiter(NULL, upper_content);
	ft_lstiter(NULL, NULL);
	check(1, "no crash");
	end_test();
	start("NULL list args");
	list = NULL;
	ft_lstadd_back(&list, NULL);
	ft_lstadd_front(&list, NULL);
	check(list == NULL, "adding a NULL node must change nothing");
	ft_lstdelone(NULL, free);
	ft_lstclear(NULL, free);
	check(ft_lstmap(NULL, upper_copy, free) == NULL, "ft_lstmap(NULL) -> NULL");
	end_test();
	start("NULL del");
	list = ft_lstnew(NULL);
	ft_lstdelone(list, NULL);
	free(list);
	list = ft_lstnew(NULL);
	ft_lstclear(&list, NULL);
	free(list);
	check(1, "no crash with del == NULL (we free the nodes ourselves after)");
	end_test();
	start("put* with NULL");
	ft_putstr_fd(NULL, 1);
	ft_putendl_fd(NULL, 1);
	ft_putchar_fd('a', -1);
	ft_putstr_fd("a", -1);
	check(1, "no crash (also with an invalid fd)");
	end_test();
	start("mem* with NULL");
	check(ft_memcpy(NULL, NULL, 5) == NULL, "ft_memcpy(NULL, NULL, 5)");
	check(ft_memmove(NULL, NULL, 5) == NULL, "ft_memmove(NULL, NULL, 5)");
	end_test();
	start("strnstr NULL len0");
	check(ft_strnstr(NULL, "a", 0) == NULL, "ft_strnstr(NULL, \"a\", 0) -> NULL");
	end_test();
}

/* ========================================================================= */

int	main(void)
{
	/* No buffering: printf (and the ft_put*_fd functions, which use write)
	   then show their text immediately and in the right order, even if the
	   program crashes right after. */
	setvbuf(stdout, NULL, _IONBF, 0);
	printf("\n=== PART 1: libc functions ===\n");
	test_is_and_to();
	test_strlen();
	test_memset_bzero();
	test_memcpy();
	test_memmove();
	test_strlcpy();
	test_strlcat();
	test_strchr();
	test_strrchr();
	test_strncmp();
	test_memchr();
	test_memcmp();
	test_strnstr();
	test_atoi();
	test_calloc();
	test_strdup();
	printf("\n=== PART 2: additional functions ===\n");
	test_substr();
	test_strjoin();
	test_strtrim();
	test_split();
	test_itoa();
	test_strmapi();
	test_striteri();
	test_put_fd();
	printf("=== PART 3: linked lists ===\n");
	test_lstnew();
	test_lstadd_front();
	test_lstadd_back();
	test_lstsize();
	test_lstlast();
	test_lstdelone();
	test_lstclear();
	test_lstiter();
	test_lstmap();
	printf("\n=================================\n");
	if (g_ko_total == 0)
		printf(" All required tests passed!\n");
	else
		printf(" %d check(s) failed (see KO above)\n", g_ko_total);
	printf("=================================\n");
	test_extras();
	return (g_ko_total != 0);
}
