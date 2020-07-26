#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/stat.h>

#define PID

int main(){
	
	int segment_id;
	int childpid;
	int stat;
	int soma;
	pid_t pid[5];

	segment_id = shmget (IPC_PRIVATE, 5*sizeof(int), S_IRUSR | S_IWUSR);
	float *shared_memory = (float*) shmat (segment_id, NULL, 0);

	FILE* fileptr;		
	int cont_lines = 0, soma_total = 0;
	char chr;
	char filename[10];
	int i = 0;
	for(i=0; i<5; i++){
		if((pid[i] = fork()) == 0){
			sprintf(filename, "arq%d.txt",i);
			fileptr = fopen(filename, "r");
			chr = getc(fileptr);
			while (chr != EOF){
				if (chr == '\n'){
					cont_lines += 1;
				}
				chr = getc(fileptr);
			}
			fclose(fileptr);
			printf("Processo %d: total de linhas %d\n", i, cont_lines);	
			shared_memory[i] = cont_lines;				
			fclose(fileptr);
		}
	}
	for(i=0; i<5; i++){
		pid_t cpid = waitpid(pid[i], &stat, 0);
	}
	soma = 0.0;
	for(i=0; i<5; i++){
		soma += shared_memory[i];
	}
	printf("Quantidade total: %d\n", soma);
	
	shmdt(shared_memory);
	shmctl(segment_id, IPC_RMID, NULL);
	return 0;
}


