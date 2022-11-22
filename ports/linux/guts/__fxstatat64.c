/* 
 * Copyright (c) 2008-2010 Wind River Systems; see
 * guts/COPYRIGHT for information.
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 * static int
 * wrap___fxstatat64(int ver, int dirfd, const char *path, struct stat64 *buf, int flags) {
 *	int rc = -1;
 */
	pseudo_msg_t *msg;
	int save_errno;
	char *realpath;

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

	if (flags & AT_SYMLINK_NOFOLLOW) {
#ifdef PSEUDO_NO_REAL_AT_FUNCTIONS
		rc = real___lxstat64(ver, realpath, buf);
#else
		rc = real___fxstatat64(ver, dirfd, realpath, buf, flags);
#endif
		if (rc == -1) {
			return rc;
		}
	} else {
#ifdef PSEUDO_NO_REAL_AT_FUNCTIONS
		rc = real___xstat64(ver, realpath, buf);
#else
		rc = real___fxstatat64(ver, dirfd, realpath, buf, flags);
#endif
		if (rc == -1) {
			return rc;
		}
	}
	save_errno = errno;

	if (ver != _STAT_VER) {
		pseudo_debug(PDBGF_CLIENT, "version mismatch: got stat version %d, only supporting %d\n", ver, _STAT_VER);
		errno = save_errno;
		return rc;
	}

	/* query database
	 * note that symlink canonicalizing is now automatic, so we
	 * don't need to check for a symlink on this end
	 */
	msg = pseudo_client_op(OP_STAT, 0, -1, dirfd, path, buf);
	if (msg && msg->result == RESULT_SUCCEED) {
		pseudo_stat_msg(buf, msg);
	}

	errno = save_errno;

/*	return rc;
 * }
 */
