#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>


typedef struct{
    int numero;
    char nome[100];
}student;



int escritor() {
    int fd, size = sizeof(student);
    student *aluno;
    shm_unlink ("/shm");

    fd = shm_open("/shm", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);

    aluno = (student*) mmap(NULL, size, PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if(fd == -1) {
        return fd;
    }

    ftruncate(fd, size);

    if(aluno == NULL) {
        return -1;
    }

    printf("Introduza algum nome \n");
    scanf("%s", aluno->nome);

    printf("Introduza algum número \n");
    scanf("%d", &aluno->numero);

    return 0;
}



int leitor() {
    int fd, size = sizeof(student*);
    student *aluno;
    fd = shm_open("/shm", O_RDWR, S_IRUSR|S_IWUSR);

    aluno = (student*) mmap(NULL, size, PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if(fd == -1) {
        return fd;
    }

    if(aluno == NULL) {
        return -1;
    }

    printf("Os dados introduzidos foram os seguintes:\n");
    printf("%s \n", aluno->nome);
    printf("%d \n", aluno->numero);

    return 0;
}



int main(void) {

    int x = escritor();
    if(x == -1) {
        return x;
    }

    int y = leitor();
    if(y == -1) {
        return y;
    }

    return 0;
}