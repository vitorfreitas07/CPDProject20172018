#include<stdio.h>
#include<stdlib.h>

#define max 81

void readFile();

int l=0;

int main(int argc, char *argv[]) {
	int index = 0, square=1;
	int *matrix =  malloc(max * sizeof(int));
	readFile(matrix,argv[1]);
	printf("INPUT:\n");
	//mudar isto
	while(index<sizeof(matrix)*2){
		if(4==square && l==2){
			printf("%d \n",matrix[index]);
			index++;
			square=1;
			continue;
		}else if(9==square && l==3){
			printf("%d \n",matrix[index]);
			index++;
			square=1;
			continue;
		}else{
			printf("%d ",matrix[index]);
		}
		square++;
		index++;
	}
	
	
	
	return 0;
}

void readFile(int* matrix, char file[]){
	int value;
    FILE* input = fopen(file, "r"); // read only  
       
    if (! input ) {  
        printf("oops, file can't be read\n"); 
        exit(-1); 
    } 
 
	int i=0;
    while ( fscanf(input, "%d", &value ) == 1 ){ 
        if(i==0){
			l=value;
			i++;
			continue;
		}
		matrix[i-1]=value;
		i++;	
    } 
}
