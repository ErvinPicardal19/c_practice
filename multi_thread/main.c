#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include "bank/bank.h"

int main(void)
{
    pthread_t t1, t2;

    printf("Balance: %u\n", read_balance());

    pthread_create(&t1, NULL, deposit, "200");
    pthread_create(&t2, NULL, deposit, "800");
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Balance: %u\n", read_balance());

    return 0;
}
