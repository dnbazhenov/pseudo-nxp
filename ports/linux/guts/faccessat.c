/*
 * Copyright (c) 2022 Peter Seebach/Seebs <seebs@seebs.net>; see
 * guts/COPYRIGHT for information.
 *
 * [Note: copyright added by code generator, may be
 * incorrect. Remove this if you fix it.]
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 * int faccessat(int dirfd, const char *path, int mode, int flags)
 *	int rc = -1;
 */

	rc = real_faccessat(dirfd, path, mode, flags);

/*	return rc;
 * }
 */
