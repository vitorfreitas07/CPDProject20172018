#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include <string.h>

//Global Variables

int **matrix;							//Declaration of structure of Sudoku
int edge;	
int l;							


void readFile();
void placeFindingRow();
void placeFindingColumn();
void printMatrix();
void placeFindingSquareHorizontal();

// Main function
int main(int argc, char *argv[])
{
	readFile(argv[1]);
	/*
	placeFindingRow();
	placeFindingColumn();
	placeFindingRow();
	placeFindingColumn();*/
	placeFindingSquareHorizontal();
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
	l = value;
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

void placeFindingSquareHorizontal() //vê se falta um elemento num quadrado e põe se faltar
{
	/*int **squareValues = (int **) malloc(l*sizeof(int *));
	for(int i=0;i<l;i++)
	{
		squareValues[i]=(int *) malloc(l*sizeof(int));
	}*/
	int *squareValues = malloc(edge*sizeof(int)); //vetor com os numeros presentes no quadrado
	int rowToChange;     //indice da linha onde vai ser inserido o numero (dentro do quadrado)
	int colToChange;     //indice da coluna onde vai ser inserido o numero (dentro do quadrado)
	int valueToInsert;	 //valor para ser inserido na posiçao matrix[rowToChange][j]
	int counter = 0;	 //counter para contar quantos numeros estao inseridos na coluna
	
	for(int i = 0; i < l; i++)
	{
		for(int j = 0; j < l; j++)
		{			
			for(int k = 0; k < l; k++)
			{
				for(int t = 0; t < l; t++)
				{
					if(matrix[i*l+k][j*l+t]!=0)
					{
						counter++;
						//printf("Adicionei o %d Counter: %d\n", matrix[i*l+k][j*l+t], counter);
						
						squareValues[matrix[i*l+k][j*l+t] - 1] = matrix[i*l+k][j*l+t]; //talvez n seja preciso usar um vetor auxiliar..
					}
					else
					{
						rowToChange = k;
						colToChange = t;
					}
				}
			}
			
			if(counter == edge-1)
			{
				//printf("Counter deu 3! %d\n", counter);
				for(int k = 0; k < edge; k++)
				{
					
					if(squareValues[k] == 0)
					{
						//printf("Valor a ser inserido: %d\n", k + 1);
						valueToInsert = k + 1;
					}					
				}
				
				matrix[i*l + rowToChange][j*l + colToChange] = valueToInsert;
			}
			
			//reset counter and squareValues
			counter = 0;
			//printf("Resetei\n");
			for(int i=0; i < edge;i++)
			{
				squareValues[i] = 0;
			}
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