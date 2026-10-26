/**
  ******************************************************************************
  * @file    syscalls.c
  * @brief   Minimal newlib system call stubs for the GNU Arm Embedded toolchain.
  *
  *          The Keil target retargets nothing: it links the Arm C library with
  *          the startup file's fixed 8 KiB heap (Heap_Size 0x2000).  To keep
  *          the same runtime behaviour under newlib:
  *            - _sbrk() serves malloc/calloc/free from the 8 KiB heap window
  *              reserved by the linker script between __HeapBase/__HeapLimit;
  *            - host I/O calls fail, so printf() output is discarded exactly
  *              like an Arm C library build without an fputc retarget and
  *              without a debugger attached (no semihosting is emitted).
  ******************************************************************************
  */

#include <errno.h>
#include <stddef.h>
#include <sys/stat.h>
#include <sys/types.h>

extern char __HeapBase; /* defined in cmake/stm32f407ig.ld */
extern char __HeapLimit;

static char *heapEnd = &__HeapBase;

/**
  * @brief  Increase program data space (used by newlib malloc).
  * @param  incr  number of bytes requested
  * @retval previous heap end, or -1 when the 8 KiB heap window is exhausted
  */
caddr_t _sbrk(int incr)
{
    char *previousHeapEnd = heapEnd;
    char *newHeapEnd = (char *)(((unsigned long)heapEnd + (unsigned long)incr + 7UL) & ~7UL);

    if (newHeapEnd > &__HeapLimit) {
        errno = ENOMEM;
        return (caddr_t)-1;
    }

    heapEnd = newHeapEnd;
    return (caddr_t)previousHeapEnd;
}

int _close(int file)
{
    (void)file;
    return -1;
}

int _fstat(int file, struct stat *st)
{
    (void)file;
    st->st_mode = S_IFCHR;
    return 0;
}

int _isatty(int file)
{
    (void)file;
    return 1;
}

int _lseek(int file, int ptr, int dir)
{
    (void)file;
    (void)ptr;
    (void)dir;
    return 0;
}

int _read(int file, char *ptr, int len)
{
    (void)file;
    (void)ptr;
    (void)len;
    return 0;
}

int _write(int file, char *ptr, int len)
{
    (void)file;
    (void)ptr;
    return len; /* discard output, matching a target without stdio retarget */
}

int _kill(int pid, int sig)
{
    (void)pid;
    (void)sig;
    errno = EINVAL;
    return -1;
}

int _getpid(void)
{
    return 1;
}
