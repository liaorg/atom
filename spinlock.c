/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2007 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Liexusong <280259971@qq.com>                                 |
  +----------------------------------------------------------------------+
*/

/* GCC support */

#include <stdlib.h>


extern int ncpu;


void spin_lock(int *lock, int which)
{
    int i, n;

    for ( ;; ) {

        if (*lock == 0 && 
            __sync_bool_compare_and_swap(lock, 0, which)) {
            return;
        }

        if (ncpu > 1) {

            for (n = 1; n < 129; n << 1) {
    
                for (i = 0; i < n; i++) {
                    __asm("pause");
                }
    
                if (*lock == 0 && 
                    __sync_bool_compare_and_swap(lock, 0, which)) {
                    return;
                }
            }
        }

        sched_yield();
    }
}


void spin_unlock(int *lock, int which)
{
    __sync_bool_compare_and_swap(lock, which, 0);
}

