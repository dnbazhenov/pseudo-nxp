/* 
 * Copyright (c) 2010 Wind River Systems; see
 * guts/COPYRIGHT for information.
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 * static int
 * wrap_execv(const char *file, char *const *argv) {
 *	int rc = -1;
 */
	const char *path_guess = file;
    if (antimagic == 0) {
	    path_guess = pseudo_exec_path(&file, 0, &argv);
        pseudo_client_op(OP_EXEC, PSA_EXEC, -1, -1, path_guess, 0);
    }

	if (path_guess) {
        pseudo_setupenv();
        if (pseudo_has_unload(NULL))
            pseudo_dropenv();

        /* if exec() fails, we may end up taking signals unexpectedly...
         * not much we can do about that.
         */

        sigprocmask(SIG_SETMASK, &pseudo_saved_sigmask, NULL);
        rc = real_execv(path_guess, argv);
    }
    else
        errno = ENOENT;

/*	return rc;
 * }
 */
