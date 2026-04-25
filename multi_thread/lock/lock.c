#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>

enum LOCK_TYPES
{
    READ_LOCK,
    WRITE_LOCK,
    MAX_LOCK
};

struct mylock
{
    int fd;
    struct flock lck;
};

int try_lock(struct mylock *lock, char *name, int type, bool wait)
{
    int ret = 0;
    int cmd = F_SETLKW;
    char pid[16] = {0};
    char lck_fname[FILENAME_MAX] = {0};

    snprintf(lck_fname, sizeof(lck_fname), "%s.lock", name);
    lock->fd = open(lck_fname, O_RDWR | O_CREAT, 0666);
    if(lock->fd < 0)
    {
        ret = -1;
        goto EXIT;
    }

    lock->lck.l_type = (type == READ_LOCK) ? F_RDLCK : F_WRLCK;
    lock->lck.l_len = 0;
    lock->lck.l_whence = SEEK_SET;
    lock->lck.l_start = 0;

    cmd = (wait) ? F_SETLKW : F_SETLK;

    if(fcntl(lock->fd, cmd, &lock->lck) < 0)
    {
        ret = -(errno);
        close(lock->fd);
    }

EXIT:
    return ret;
}

void unlock(struct mylock *lock)
{
    lock->lck.l_type = F_UNLCK;
    fcntl(lock->fd, F_SETLK, &lock);
}
