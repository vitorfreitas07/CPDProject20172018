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
int **matrix;	
int **auxMatrix;						
int l=0,edge=0;

// Main function
int main(int argc, char *argv[])
{
	readFile(argv[1]);
	solveSudoku();
	return 0;
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

void solveSudoku()
{

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
					while(matrix[i][j]<=edge)
					{	
						matrix[i][j]++;
						if(existsInBlock(matrix[i][j],i,j))
							continue;
						if(existsInRow(i,j,matrix[i][j]))
							continue;
						if(existsInColumn(i,j,matrix[i][j]))
							continue;
						break;
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
							}
							if(i<0)
							{
								printf("No solution\n");
								exit(1);
							}
								
						}	
						while(auxMatrix[i][j]!=0);
						
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


_Bool existsInColumn(int i,int j, int num) //identify if the number is already present in the column
{
	_Bool existInColumn=0;
	for(int row=0;row<edge;row++)
	{
		if(row==i)
			continue;
									
		if(matrix[row][j]==num)
		{
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
			existInRow=1;
			break;
		}
	}
	return existInRow;
}

_Bool existsInBlock(int num,int i,int j) //identify if the number is already present in the block
{
	_Bool existInBlock=0;
	for(int row = 0; row < l; row++)
	{
		for(int col = 0; col < l; col++)
		{
			if(matrix[(i/l)*l+row][(j/l)*l+col] == num && (i != (l*(i/l) + row) && j != (l*(j/l) + col)))
			{
				existInBlock=1;
				break;
			}
		}
		if(existInBlock)
			break;
	}
	return existInBlock;
}