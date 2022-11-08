/* 
 * Copyright (c) 2008-2010 Wind River Systems; see
 * guts/COPYRIGHT for information.
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 * static int
 * wrap_execve(const char *file, char *const *argv, char *const *envp) {
 *	int rc = -1;
 */
    const char *path_guess = file;
        if (antimagic == 0) {
            path_guess = pseudo_exec_path(&file, 0, &argv);
            pseudo_client_op(OP_EXEC, PSA_EXEC, -1, -1, path_guess, 0);
        }

    if (path_guess) {
        char * const *new_environ = pseudo_setupenvp(envp);
        if (pseudo_has_unload(new_environ))
            new_environ = pseudo_dropenvp(new_environ);

        /* if exec() fails, we may end up taking signals unexpectedly...
         * not much we can do about that.
         */
        sigprocmask(SIG_SETMASK, &pseudo_saved_sigmask, NULL);
        rc = real_execve(path_guess, argv, new_environ);
    }
    else
        errno = ENOENT;
/*	return rc;
 * }
 */
