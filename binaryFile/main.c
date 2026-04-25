#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#define __unused __attribute__((unused))
#define DATA_BIN "data.bin"

struct foo
{
    int id;
    char username[24];
    char firstName[24];
    char lastName[60];
    time_t created;
} __attribute__((packed));

static int wfd = -1;
static int rfd = -1;
static unsigned int num_of_entries = 0;
static struct foo entry = {0};

void showEntry(struct foo *entry)
{
    time_t created = entry->created;
    fprintf(stdout,
            "Entry:\n"
            "    ID: %d\n"
            "    Username: %s\n"
            "    Name: %s %s\n"
            "    Created: %s\n",
            entry->id, entry->username, entry->firstName,
            entry->lastName, ctime(&created));
}

off_t findEntry(struct foo *find)
{
    if(rfd < 0)
    {
        return -1;
    }

    lseek(rfd, 0, SEEK_SET); // Reset file offset
    memset(&entry, 0, sizeof(struct foo));
    while(read(rfd, &entry, sizeof(struct foo)) == sizeof(struct foo))
    {
        if(!strncmp(entry.username, find->username, sizeof(entry.username)))
        {
            return lseek(rfd, (-1 * sizeof(struct foo)), SEEK_CUR);
        }
    }

    return -1;
;
}

int insertDB(char *username, char *firstName, char *lastName)
{
    off_t offset = 0;
    struct foo newEntry = {0};

    if(wfd < 0)
    {
        fprintf(stderr, "DB not initialized\n");
        return -1;
    }

    snprintf(newEntry.username, sizeof(newEntry.username), "%s", username);
    snprintf(newEntry.firstName, sizeof(newEntry.firstName), "%s", firstName);
    snprintf(newEntry.lastName, sizeof(newEntry.lastName), "%s", lastName);
    offset = findEntry(&newEntry);

    if(offset < 0)
    {
        newEntry.id = num_of_entries+1;
        newEntry.created = time(NULL);
        if(lseek(wfd, 0, SEEK_END) < 0)
        {
            goto err;
        }
        num_of_entries++;
    }
    else
    {
        newEntry.id = entry.id;
        newEntry.created = entry.created;
        if(lseek(wfd, offset, SEEK_SET) < 0)
        {
            goto err;
        }
    }

    if(write(wfd, &newEntry, sizeof(struct foo)) != sizeof(struct foo))
    {
        goto err;
    }

    return 0;

err:
        fprintf(stderr, "Error: %s\n", strerror(errno));
        return -1;
}

void showAll(void)
{
    if(rfd < 0)
    {
        return;
    }

    lseek(rfd, 0, SEEK_SET); // Reset file offset
    memset(&entry, 0, sizeof(struct foo));
    while(read(rfd, &entry, sizeof(struct foo)) > 0)
    {
        showEntry(&entry);
        memset(&entry, 0, sizeof(struct foo));
    }
}

void deinitDB(void)
{
    if(wfd >= 0)
    {
        close(wfd);
    }

    if(rfd >= 0)
    {
        close(rfd);
    }
}

int initDB()
{
    size_t len = 0;

    if(wfd != -1 && rfd != -1)
    {
        return 0;
    }

    wfd = open(DATA_BIN, O_WRONLY | O_CREAT, 0644);
    if(wfd < 0)
    {
        fprintf(stderr, "Error: %s\n", strerror(errno));
        return -1;
    }

    rfd = open(DATA_BIN, O_RDONLY);
    if(rfd < 0)
    {
        fprintf(stderr, "Error: %s\n", strerror(errno));
        return -1;
    }

    memset(&entry, 0, sizeof(struct foo));
    num_of_entries = 0;
    while((len = read(rfd, &entry, sizeof(struct foo))) > 0)
    {
        if(len < sizeof(struct foo))
        {
            fprintf(stderr, "Error: %s is corrupted\n", DATA_BIN);
            return -1;
        }

        num_of_entries++;
    }

    return 0;
}

void usage(char *exec)
{
    fprintf(stdout, "Usage: %s [-L] <username> <firstname> <lastname>\n", exec);
}

int main(int __unused argc, char __unused **argv)
{
    int c = 0;
    //struct foo foos[3] = {0};

    if(initDB() < 0)
    {
        deinitDB();
        return -1;
    }

    if(argc < 2)
    {
        usage(argv[0]);
        return 0;
    }

    while((c = getopt(argc, argv, "hL")) != -1)
    {
        switch(c)
        {
            case 'h':
                usage(argv[0]);
                return 0;
            case 'L':
                showAll();
                return 0;
            default:
                usage(argv[0]);
                return -1;
        }
    }

    for(int i = optind; i < argc; i++)
    {
        if(!argv[i])
        {
            return -1;
        }
    }

    insertDB(argv[optind], argv[optind+1], argv[optind+2]);

    deinitDB();
    return 0;
}
