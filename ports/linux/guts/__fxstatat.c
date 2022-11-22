/* 
 * Copyright (c) 2008-2010 Wind River Systems; see
 * guts/COPYRIGHT for information.
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 * static int
 * wrap___fxstatat(int ver, int dirfd, const char *path, struct stat *buf, int flags) {
 *	int rc = -1;
 */

	struct stat64 buf64;
	char *realpath;

	/* populate buffer with complete data */
#ifdef PSEUDO_NO_REAL_AT_FUNCTIONS
	if (dirfd != AT_FDCWD) {
		errno = ENOSYS;
		return -1;
	}
#endif

	/* This hack is needed when someone calls the *at() functions
	 * for a file handle that is not *normally* linked to a path,
	 * e.g., stdout when redirected. The problem occurs because
	 * base_path then doesn't know about a path and turns this to
	 * 0 from "", but the real function still wants to see the ""
	 * or it returns an error that messes with the caller. So we
	 * pretend "" for 0, just in case. This is ugly and
	 * the handling of pseudo_root_path and base_path should be
	 * updated, but that is a much more complex topic. FIX
	 */
	realpath = path;
	if (!realpath && dirfd != -1 && dirfd != AT_FDCWD) {
		realpath = "";
	}

#ifdef PSEUDO_NO_REAL_AT_FUNCTIONS
	if (flags & AT_SYMLINK_NOFOLLOW) {
		rc = real___lxstat(ver, realpath, buf);
	} else {
		rc = real___xstat(ver, realpath, buf);
	}
#else
	real___fxstatat(ver, dirfd, realpath, buf, flags);
#endif
	/* obtain fake data */
	rc = wrap___fxstatat64(ver, dirfd, path, &buf64, flags);
	/* overwrite */
	pseudo_stat32_from64(buf, &buf64);

/*	return rc;
 * }
 */
