#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include <string.h>

void readFile();
void printMatrix();
void fillAuxMatrix();
void solveSudoku();
int canNbeHere();

//Global Variables
int **matrix;	
int **auxMatrix;						
int l=0,edge=0;
int *firstZeros;

// Main function
int main(int argc, char *argv[])
{
	readFile(argv[1]);
	solveSudoku();
	return 0;
}

void readFile(char file[])
{
	firstZeros=(int*) malloc(edge*sizeof(int *));
	int value;
    FILE *input = fopen(file, "r"); // read only  
       
    if (! input ) {  
        printf("oops, file can't be read\n"); 
        exit(-1); 
    } 
 
	fscanf(input, "%d", &value );
	edge=value*value;
	l=value;
	//Allocation of structure for Sudoku
	matrix=(int **) malloc(edge*sizeof(int *));
	auxMatrix=(int **) malloc(edge*sizeof(int *));
	for(int i=0;i<edge;i++)
	{
		matrix[i]=(int *) malloc(edge*sizeof(int));
		auxMatrix[i]=(int *) malloc(edge*sizeof(int));
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
	fillAuxMatrix();
	 
}

void printMatrix()
{
	for(int i = 0; i<l*l; i++ )
	{
		for(int j = 0; j < l*l; j++)
		{
			printf("%d ",matrix[i][j]);
		}
		printf("\n");
	}
	printf("----------------\n");
}

void findFirstZeros()
{
	int index=0;
	for(int i=0;i<edge;i++)
	{
		for(int j=0;j<edge;j++)
		{
			if(auxMatrix[i][j]==0)
			{
				firstZeros[i] = j;
				break;
			}
		}
	}
}

void solveSudoku()
{

	findFirstZeros();
	_Bool rollBack=1;
	for(int i=0;i<edge;i++)
	{
		for(int j=0;j<edge;j++)
		{	
			if(auxMatrix[i][j]==0)
			{
				rollBack=1;
				while(rollBack==1)
				{
					rollBack=0;
					int try = matrix[i][j];
					while(try<=edge)
					{	
						try++;
						if(canNbeHere(i,j,try))
							continue;
						matrix[i][j] = try;
						break;
					}

					if(try>edge)
					{
							
						rollBack=1;
						matrix[i][j]=0;
						_Bool escape=0;
							
				
						if(firstZeros[i]==j&&i!=0)
						{
							
	
							int previousJ=(j/l)* l+l;
							int previousI=i-1;
	
							
							while(!(previousJ==j&&previousI==i))
							{
								j--;
								if(j<0)
								{
									j=edge-1; i--;
								}
							
								if(auxMatrix[i][j]==0&&!(previousJ==j&&previousI==i))
								{
									matrix[i][j]=0;
								}
								
							}
							
							while(auxMatrix[i][j]!=0)
							{
								j--;
								if(j<0)
								{
									j=edge-1; i--;
								}
								if(i<0)
								{
									printf("No solution\n");
									exit(0);
								}
							}
							
							escape=1;
							
	
						}
						
						//printMatrix();
							
						if(!escape)
						{

							do
							{
								j--;
								if(j<0)
								{
									j=edge-1; i--;
								}
								if(i<0)
								{
									printf("No solution\n");
									exit(0);
								}
									
							}	
							while(auxMatrix[i][j]!=0);
						}
						
						
					}
				}
			}
	
		}
	
	}
	printMatrix();
}


void fillAuxMatrix() //fill auxiliary matrix with original matrix values
{

	for(int i = 0; i<l*l; i++ )
	{
		for(int j = 0; j < l*l; j++)
		{
			auxMatrix[i][j]=matrix[i][j];		
		}
	}
}


int canNbeHere(int i,int j, int num)
{
	
	//Checks Block
	for(int row = 0; row < l; row++)
	{
		for(int col = 0; col < l; col++)
		{
			if(matrix[(i/l)*l+row][(j/l)*l+col] == num ) 
			{
				return 1;
			}
		}
	}
	
	//Cecks Row
	for(int column=0;column<edge;column++)
	{			
		if(matrix[i][column]==num)
		{
			return 1;
		}
	}

	//Checks Column
	for(int row=0;row<edge;row++)
	{
	
		if(matrix[row][j]==num)
		{
			return 1;
		}
	}
	
	return 0;
}