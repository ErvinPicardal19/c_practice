#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
static unsigned int account_balance = 0;

unsigned int read_balance(void)
{
    usleep(250000);
    return account_balance;
}

static void write_balance(unsigned int new_balance)
{
    usleep(250000);
    account_balance = new_balance;
}

void *deposit(void *money_s)
{
    char *endptr = NULL;
    unsigned long money = 0;
    pthread_t tid = pthread_self();
    unsigned int curr_balance;

    if(money_s == NULL || *(char *)money_s == '\0' || *(char *)money_s == '-')
    {
        return NULL;
    }

    money = strtoul((char *)money_s, &endptr, 10);
    if(*endptr != '\0')
    {
        return NULL;
    }

    pthread_mutex_lock(&mut);

    printf("Start Deposit [0x%lx]\n", tid);
    curr_balance = read_balance();
    curr_balance += money;

    printf("Adding to balance... [0x%lx]\n", tid);
    write_balance(curr_balance);

    pthread_mutex_unlock(&mut);

    return NULL;
}
