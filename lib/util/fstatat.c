/*
 * SPDX-License-Identifier: ISC
 *
 * Copyright (c) 2019-2020 Todd C. Miller <Todd.Miller@sudo.ws>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */

#include <config.h>

#include <sys/stat.h>

#include <fcntl.h>
#include <unistd.h>

#include <sudo_compat.h>

#ifndef HAVE_FSTATAT
int
sudo_fstatat(int dfd, const char * restrict path, struct stat * restrict sb, int flag)
{
    int odfd, ret = -1;

    if (dfd == (int)AT_FDCWD) {
	if (ISSET(flag, AT_SYMLINK_NOFOLLOW))
	    return lstat(path, sb);
	else
	    return stat(path, sb);
    }

    /* Save cwd */
    if ((odfd = open(".", O_RDONLY)) == -1)
	goto done;

    if (fchdir(dfd) == -1)
	goto done;

    if (ISSET(flag, AT_SYMLINK_NOFOLLOW))
	ret = lstat(path, sb);
    else
	ret = stat(path, sb);

    /* Restore cwd */
    if (fchdir(odfd) == -1) {
	/* Should not happen */
	ret = -1;
    }

done:
    if (odfd != -1)
	close(odfd);

    return ret;
}
#endif /* HAVE_FSTATAT */
