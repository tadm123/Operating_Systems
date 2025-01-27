#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include "common_threads.h"

// If done correctly, each child should print their "before" message
// before either prints their "after" message. Test by adding sleep(1)
// calls in various locations.

sem_t *s1, *s2;

sem_t *Sem_open(const char *name, int val) {
  sem_t *sem = sem_open(name, O_CREAT, S_IRWXU, val);
  if (sem == SEM_FAILED) {
    printf("failed to create a semaphore.\n");
    exit(0);
  }
  return sem;
}
void *child_1(void *arg) {
    printf("child 1: before\n");
    // what goes here?
    sleep(1);
    Sem_post(s2); 
    Sem_wait(s1);

    printf("child 1: after\n");
    return NULL;
}

void *child_2(void *arg) {
    printf("child 2: before\n");
    // what goes here?
    sleep(1);
    Sem_post(s1);
    Sem_wait(s2);
    printf("child 2: after\n");
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t p1, p2;
    printf("parent: begin\n");
    // init semaphores here
    s1 = Sem_open("/s1", 0);
    s2 = Sem_open("/s2", 0);
    
    Pthread_create(&p1, NULL, child_1, NULL);
    Pthread_create(&p2, NULL, child_2, NULL);
    Pthread_join(p1, NULL);
    Pthread_join(p2, NULL);
    printf("parent: end\n");

    Sem_close(s1);
    Sem_close(s2);
    return 0;
}
