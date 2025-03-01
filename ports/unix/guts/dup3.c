/*
 * Copyright (c) 2022 NXP; see
 * guts/COPYRIGHT for information.
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 * static int
 * wrap_dup3(int oldfd, int newfd, int flags) {
 *	int rc = -1;
 */
	int save_errno;

	/* Shortcut avoids messing up our database */
	if (oldfd == newfd) {
		errno = EINVAL;
		return -1;
	}

	/* close existing one first - this also causes the socket to the
	 * server to get moved around if someone tries to overwrite it. */
	pseudo_debug(PDBGF_CLIENT | PDBGF_VERBOSE, "dup3: %d->%d (%d)\n", oldfd, newfd, flags);
	pseudo_client_op(OP_CLOSE, 0, newfd, -1, 0, 0);
	rc = real_dup3(oldfd, newfd, flags);
	save_errno = errno;
	pseudo_client_op(OP_DUP, 0, oldfd, newfd, 0, 0);
	errno = save_errno;

/*	return rc;
 * }
 */
