#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include <string.h>

//Global Variables

int **matrix;							//Declaration of structure of Sudoku
int edge;								

int c;
//cool stuff
void readFile();
void placeFindingRow();
void placeFindingColumn();
void printMatrix();

// Main function
int main(int argc, char *argv[])
{
	readFile(argv[1]);
	//placeFindingRow();
	placeFindingColumn();
	printMatrix();
	return 0;
}


void readFile(char file[]){
	
	int value;
    FILE *input = fopen(file, "r"); // read only  
       
    if (! input ) {  
        printf("oops, file can't be read\n"); 
        exit(-1); 
    } 
 
	fscanf(input, "%d", &value );
	edge=value*value;
	
	//Allocation of structure for Sudoku
	matrix=(int **) malloc(edge*sizeof(int *));
	for(int i=0;i<edge;i++)
	{
		matrix[i]=(int *) malloc(edge*sizeof(int));
	}
	
	
	int i=0;	
	int j=0;
    while ( fscanf(input, "%d", &value ) == 1 ){ 
		if(j < edge) 
		{
			matrix[i][j]=value;
			j++;
		}
		else
		{
			j=0;
			i++;
			matrix[i][j]=value;
			j++;
		}
    }	 
}


void placeFindingRow()
{
	int *rowValues = malloc(edge*sizeof(int)); //vetor com os numeros presentes na linha
	int colToChange;     //indice da coluna onde vai ser inserido o numero
	int valueToInsert;	 //valor para ser inserido na posiçao matrix[i][colToChange]
	int counter = 0;	 //counter para contar quantos numeros estao inseridos na linha
	
	for(int i = 0; i < edge; i++)
	{
		for(int j = 0; j < edge; j++)
		{
			if(matrix[i][j] != 0)
			{
				counter++;
				rowValues[matrix[i][j] - 1] = matrix[i][j];
			}
			else
			{
				colToChange = j;
			}
		}
		
		if(counter == (edge - 1)) // Se faltar um numero na linha..
		{
			for(int j = 0; j < edge; j++)
			{
				if(rowValues[j] == 0)
				{
					valueToInsert = j+1;
				}
			}
			matrix[i][colToChange] = valueToInsert;
		}
		
		//reset counter and rowValues
		counter = 0;
		for(int j=0; j < edge;j++)
		{
			rowValues[j]= 0;
		}
	}
	
}

void placeFindingColumn()
{
	int *colValues = malloc(edge*sizeof(int)); //vetor com os numeros presentes na coluna
	int rowToChange;     //indice da linha onde vai ser inserido o numero
	int valueToInsert;	 //valor para ser inserido na posiçao matrix[rowToChange][j]
	int counter = 0;	 //counter para contar quantos numeros estao inseridos na coluna
	
	for(int j = 0; j < edge; j++)
	{
		for(int i = 0; i < edge; i++)
		{
			if(matrix[i][j] != 0)
			{
				counter++;
				colValues[matrix[i][j] - 1] = matrix[i][j];
			}
			else
			{
				rowToChange = i;
			}
		}
		
		if(counter == (edge - 1)) // Se faltar um numero na coluna..
		{
			for(int i = 0; i < edge; i++)
			{
				if(colValues[i] == 0)
				{
					valueToInsert = i+1;
				}
			}
			matrix[rowToChange][j] = valueToInsert;
		}
		
		//reset counter and colValues
		counter = 0;
		for(int i=0; i < edge;i++)
		{
			colValues[i]= 0;
		}
	}
}

void printMatrix()
{
	for(int i = 0; i<edge; i++ )
	{
		for(int j = 0; j < edge; j++)
		{
			printf("%d ",matrix[i][j]);
		}
		printf("\n");
	}
}