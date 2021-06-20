#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <time.h>
#include <sys/wait.h>
#include <string.h>

#define ARRAY 50000
#define NUMBEROFCLIENTS 10000
#define NUMBEROFPRODUCTS 250
#define STOCK 30


struct vendas {
 int customer_code;
 int product_code;
 int quantity;
};


int main(){
    
   pid_t arraypids[10];
   int fd[2];
   struct vendas array[ARRAY];
   int produtos[ARRAY];
   int product_code;
   size_t bytes;
   
   srand(time(NULL));

   for(int i = 0; i < ARRAY; i++){
       produtos[i] = -1;
       array[i].customer_code = rand() % NUMBEROFCLIENTS;
       array[i].product_code = rand() % NUMBEROFPRODUCTS;
       array[i].quantity = 1 + rand() % STOCK;
   }

   if (pipe(fd) == -1) {
        perror("Erro");
        return 0;
    }

   for(int i = 0; i < 10; i++){
       arraypids[i] = fork();

       if(arraypids[i] == 0){
           close(fd[0]);
           for(int j = 5000 * i;j < 5000 * i + 5000; j++){
            if(array[j].quantity > 20){
                write(fd[1], &array[j].product_code, sizeof(int));
            }

           }
        exit(0);
       }
   }
close(fd[1]);

    int i = 0;
    while((bytes = read(fd[0], &product_code, sizeof(int))) > 0){
        produtos[i] = product_code;
        i++;

    }
    int status;
    for(i = 0; i < 10; i++){
        waitpid(arraypids[i], &status, 0);
    }

int count = 0;

    for(i = 0; i < ARRAY; i++){
        if (produtos[i] != -1){
            printf("Product_code: %d \n", produtos[i]);
            count++;
        
        }
        
    }
    printf("Number of sales that meet the conditions: %d \n", count);

}