#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include <string.h>

void readFile();
void printMatrix();
void fillAuxMatrix();
_Bool existsInBlock();
_Bool existsInRow();
_Bool existsInColumn();
void solveSudoku();

//Global Variables
int ***matrix;	
int **auxMatrix;						
int l=0,edge=0;
int firstI;
int firstJ;	

// Main function
int main(int argc, char *argv[])
{
	readFile(argv[1]);
	//solveSudoku();
	//printMatrix();
	return 0;
}

/*
void freeMatrix()
{
	for(int i=0;i<edge;i++)
	{
		
		for(int j = 0; i < edge; j++)
		{
			free matrix[i][j];
		}
		
		matrix[i]=(int **) malloc(edge*sizeof(int *));
		auxMatrix[i]=(int *) malloc(edge*sizeof(int));
	}
}*/


void readFile(char file[])
{
	
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
	matrix=(int ***) malloc(edge*sizeof(int **));
	auxMatrix=(int **) malloc(edge*sizeof(int *));
	for(int i=0;i<edge;i++)
	{
		matrix[i]=(int **) malloc(edge*sizeof(int *));
		for(int j = 0; i < edge; j++)
		{
			matrix[i][j]=(int *) malloc(edge*sizeof(int));
		}
		auxMatrix[i]=(int *) malloc(edge*sizeof(int));
	}
	
	int i=0;	
	int j=0;
    while ( fscanf(input, "%d", &value ) == 1 ){ 
		if(j < edge) 
		{
			for(int k = 0; k < edge; k++)
			{
				matrix[i][j][k]=value;
			}
			
			j++;
		}
		else
		{
			j=0;
			i++;
			
			for(int k = 0; k < edge; k++)
			{
				matrix[i][j][k]=value;
			}
			j++;
		}
    } 
	fillAuxMatrix();
	 
	 
	_Bool boolean = 0;
	for(int i = 0; i < edge; i++)
	{
		for(int j = 0; j < edge; j++)
		{
			if(auxMatrix[i][j] == 0)
			{				
				firstI = i;
				firstJ = j;
				boolean = 1;
				break;
			}
		}
		if(boolean)
		{break;}
	}
	
	auxMatrix[firstI][firstJ] = 1;
}

void printMatrix()
{
	for(int k = 0; k < edge; k++)
	{
		for(int i = 0; i<l*l; i++ )
		{
			for(int j = 0; j < l*l; j++)
			{
				printf("%d ",matrix[i][j][k]);
			}
			printf("\n");
		}
		printf("----------------\n");
	}
	printf("----------------\n");
	printf("----------------\n");
	printf("----------------\n");
	printf("----------------\n");
}

void solveSudoku()
{
	_Bool end = 0;
	_Bool rollBack=1;
	
	#pragma omp parallel for private (matrix)
	for(int n = 1; n <= edge; n++)
	{
		end = 0;
		
		
		//printMatrix();
		//printf("%d\n", n);
		if(existsInBlock(n, firstI, firstJ))
			continue;
		if(existsInRow(firstI, firstJ, n))
			continue;
		if(existsInColumn(firstI, firstJ, n))
			continue;
		
		matrix[firstI][firstJ] = n;
		//printf("%d\n",n);
		
		
		for(int i=0;i<edge;i++)
		{
			for(int j=0;j<edge;j++)
			{	
				
				if(auxMatrix[i][j]==0)
				{
					//printf("lol\n");
					rollBack=1;
					while(rollBack==1)
					{
						rollBack=0;
						_Bool rowE=1,columnE=1,blockE=1;
						
						while((rowE == 1 || columnE == 1 || blockE == 1) && matrix[i][j] <= edge)
						{						
							matrix[i][j]++;
							
							blockE=existsInBlock(matrix[i][j],i,j);
							rowE=existsInRow(i,j,matrix[i][j]); 
							columnE=existsInColumn(i,j,matrix[i][j]);
						}
						

						if(matrix[i][j]>edge)
						{
							
							rollBack=1;
							matrix[i][j]=0;
							do
							{
								
								
								j--;
								
								
								if(j<0)
								{
									j=edge-1; i--;
									//printf("i: %d, j: %d, n: %d\n", i, j, n);
								}
								if(i == firstI && j == firstJ)
								{
									end = 1;
									break;
								}
								
									
							}	
							while(auxMatrix[i][j]!=0);
							
						}
						if(end)
							break;
						
					}
				}
				if(end)
					break;
			}
			if(end)
				break;
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


_Bool existsInColumn(int i,int j, int num) //identify if the number is already present in the column
{
	_Bool existInColumn=0;
	for(int row=0;row<edge;row++)
	{
		if(row==i)
			continue;
									
		if(matrix[row][j]==num)
		{
			//printf("Nao posso por o %d em [%d][%d] (coluna)\n", num, row, j);
			existInColumn=1;
			break;
		}
	}
	return existInColumn;
}

_Bool existsInRow(int i,int j, int num) //identify if the number is already present in the row
{
	_Bool existInRow=0;
	for(int column=0;column<edge;column++)
	{
		if(column==j)
			continue;
									
		if(matrix[i][column]==num)
		{
			//printf("Nao posso por o %d em [%d][%d] (linha)\n", num, i, column);
			existInRow=1;
			break;
		}
	}
	return existInRow;
}

_Bool existsInBlock(int num,int i,int j) //identify if the number is already present in the block
{
	_Bool existInBlock=0;
	for(int xB=0;xB<edge;xB++)
	{				
		for(int yB=0;yB<edge;yB++)
		{
			if(xB!=i&&yB!=j&& (i/l==xB/l&&j/l==yB/l) && num ==matrix[xB][yB])
			{
				//printf("xB: %d, yB\n", num, i/l, j/l);
				existInBlock=1;
				break;
			}								
		}
		if(existInBlock)
			break;
	}
	return existInBlock;
}