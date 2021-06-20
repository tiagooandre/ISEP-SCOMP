#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    int fd[6][2]; //5 pipes parent-child and +1 between childs
    pid_t p;
    int barcode, index;

    struct product {
        char name[20];
        float price;
        int barcode;
    };

    struct product database_prod[5];
    strcpy(database_prod[0].name, "iPhone");
    database_prod[0].price = 1279.00;
    database_prod[0].barcode = 1;
    strcpy(database_prod[1].name, "iPad Air");
    database_prod[1].price = 679.00;
    database_prod[1].barcode = 2;
    strcpy(database_prod[2].name, "Apple Watch");
    database_prod[2].price = 489.00;
    database_prod[2].barcode = 3;
    strcpy(database_prod[3].name, "Airpods Pro");
    database_prod[3].price = 279.00;
    database_prod[3].barcode = 4;
    strcpy(database_prod[4].name, "Macbook Pro");
    database_prod[4].price = 1479.00;
    database_prod[4].barcode = 5;

    for (int i = 0; i < 6; i++){
		if (pipe(fd[i]) == -1){
			perror("Pipe failed.\n");
			return 1;
		}
	}
    
    for (int i = 0; i < 5; i++) {
        p = fork();

        if (p < 0) {
            perror("Fork failed.\n");
            return 1;
        }

        if (p == 0) {
            close(fd[0][0]);
            close(fd[i+1][1]);

            printf("Reader %d\n", i+1);
            printf("-> Input barcode (1-5):\n\n");
            scanf("%d", &barcode);

            write(fd[0][1], &barcode, sizeof(barcode));
            write(fd[0][1], &i, sizeof(i));

            struct product product1;
            read(fd[i+1][0], &product1, sizeof(struct product));
            printf("Reader %d\n", i+1);
            printf("Product: %s | Price: %f\n", product1.name, product1.price);

            close(fd[0][1]);
            close(fd[i+1][0]);
            exit(0);
        }
    }

    for (int i = 0; i < 5; i++) {
        close(fd[0][1]);
        close(fd[i+1][0]);

        read(fd[0][0], &barcode, sizeof(barcode));
		read(fd[0][0], &index, sizeof(index));

        int found = 0;

        for (int j = 0; j < 5; j++) {
            if (barcode == database_prod[j].barcode) {
                write(fd[index+1][1], &database_prod[j], sizeof(struct product));
                found = 1;
            }
        }

        if (found == 0) {
            printf("The product with that barcode doesn't exist.\n");
        }
    }
    return 0;
}