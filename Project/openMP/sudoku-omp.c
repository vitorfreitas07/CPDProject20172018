#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include <string.h>

void readFile();
void printMatrix();
void fillAuxMatrix();
void solveSudoku();
void freeMatrix();
_Bool existsInBlock();
_Bool existsInRow();
_Bool existsInColumn();


//Global Variables
int ***matrix;	
int **auxMatrix;						
int l=0,edge=0;
int firstI;
int firstJ;	
int lastI;
int lastJ;

// Main function
int main(int argc, char *argv[])
{
	readFile(argv[1]);
	solveSudoku();
	//printMatrix();
	//freeMatrix();
	return 0;
}


void freeMatrix()
{
	for(int i=0;i<edge;i++)
	{
		
		for(int j = 0; i < edge; j++)
		{
			free(matrix[i][j]);
		}
		
		free(matrix[i]);
		free(auxMatrix[i]);
	}
	
	free(matrix);
	free(auxMatrix);
}


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

	//printf("Edge: %d\n", edge);
	for(int i = 0; i < edge; i++)
	{
		matrix[i]=(int **) malloc(edge*sizeof(int *));
		auxMatrix[i]=(int *) malloc(edge*sizeof(int));
		for(int j = 0; j < edge; j++)
		{
			matrix[i][j]=(int *) malloc(edge*sizeof(int));
		}
		
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
			auxMatrix[i][j] = value;
			
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
			auxMatrix[i][j] = value;
			j++;
		}
    } 
	//fillAuxMatrix();
	
	_Bool boolean = 0;
	for(int i = 0; i < edge; i++)
	{
		for(int j = 0; j < edge; j++)
		{
			if(auxMatrix[i][j] == 0)
			{
				lastI = i;
				lastJ = j;
				
				if(boolean == 0)
				{
					firstI = i;
					firstJ = j;
					boolean = 1;
				}
			}
		}
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
	
	printf("----------------\n");printf("----------------\n");printf("----------------\n");
}


void solveSudoku()
{
	_Bool end = 0;
	_Bool rollBack=1;
	
	#pragma omp parallel for 
	for(int n = 1; n <= edge; n++)
	{
		end = 0;
		
		
		//printMatrix();
		//printf("vou verificar o bloco para n = %d\n", n);
		if(existsInBlock(n, firstI, firstJ, n))
			continue;
		//printf("vou verificar a linha para n = %d\n", n);
		if(existsInRow(firstI, firstJ, n, n))
			continue;
		//printf("vou verificar a coluna para n = %d\n", n);
		if(existsInColumn(firstI, firstJ, n, n))
			continue;
		
		matrix[firstI][firstJ][n-1] = n;
		//printf("%d\n",n);
		
		
		for(int i=0;i<edge;i++)
		{
			for(int j=0;j<edge;j++)
			{	
				
				if(auxMatrix[i][j]==0)
				{
					
					//printf("i: %d, j: %d\n", i, j);
					rollBack=1;
					while(rollBack==1)
					{
						rollBack=0;
						_Bool rowE=1,columnE=1,blockE=1;
						
						while((rowE == 1 || columnE == 1 || blockE == 1) && matrix[i][j][n - 1] <= edge)
						{						
							matrix[i][j][n-1]++;
							//printMatrix();
							blockE=existsInBlock(matrix[i][j][n-1],i,j, n);
							rowE=existsInRow(i,j,matrix[i][j][n-1], n); 
							columnE=existsInColumn(i,j,matrix[i][j][n-1], n);
						}
						

						if(matrix[i][j][n-1]>edge)
						{
							//printf("Fiz Rollback: %d\n", matrix[i][j][n-1]);
							
							rollBack=1;
							matrix[i][j][n-1]=0;
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
						else
						{
							if(i == lastI && j == lastJ)
							{
								printMatrix();
								exit(0);
							}
							
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


/*void fillAuxMatrix() //fill auxiliary matrix with original matrix values
{

	for(int i = 0; i<l*l; i++ )
	{
		for(int j = 0; j < l*l; j++)
		{
			auxMatrix[i][j]=matrix[i][j];		
		}
	}
}*/


_Bool existsInColumn(int i,int j, int num, int n) //identify if the number is already present in the column
{
	_Bool existInColumn=0;
	for(int row=0;row<edge;row++)
	{
		if(row==i)
			continue;
									
		if(matrix[row][j][n-1]==num)
		{
			//printf("Nao posso por o %d em [%d][%d] (coluna)\n", num, i, j);
			existInColumn=1;
			break;
		}
	}
	return existInColumn;
}

_Bool existsInRow(int i,int j, int num, int n) //identify if the number is already present in the row
{
	_Bool existInRow=0;
	for(int column=0;column<edge;column++)
	{
		if(column==j)
			continue;
									
		if(matrix[i][column][n-1]==num)
		{
			//printf("Nao posso por o %d em [%d][%d] (linha)\n", num, i, j);
			existInRow=1;
			break;
		}
	}
	return existInRow;
}

_Bool existsInBlock(int num,int i,int j, int n) //identify if the number is already present in the block
{
	
	
	_Bool existInBlock=0;
	for(int row = 0; row < l; row++) //ver se já existe o n no bloco
	{
		for(int col = 0; col < l; col++)
		{
			if(matrix[(i/l)*l+row][(j/l)*l+col][n-1] == num && (i != (l*(i/l) + row) && j != (l*(j/l) + col)))
			{
				
				//printf("Nao posso por o %d no bloco [%d][%d]\n", num, i/l, j/l);
				existInBlock=1;
				break;
			}
		}
		if(existInBlock)
			break;
	}
	
	
	
	
	/*
	for(int xB=0;xB<edge;xB++)
	{				
		for(int yB=0;yB<edge;yB++)
		{
			if(xB!=i&&yB!=j && (i/l==xB/l&&j/l==yB/l) && num ==matrix[xB][yB][n-1])
			{
				
				existInBlock=1;
				break;
			}								
		}
		if(existInBlock)
			break;
	}*/
	if(!existInBlock)
	{
		//printf("Posso por o %d no bloco [%d][%d]\n", num, i/l, j/l);
	}
	return existInBlock;
}