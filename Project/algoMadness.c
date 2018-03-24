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
int l=0;								
int edge=0;

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
	int initRow=0;
	int initColumn=0;
	
	for(int i=0;i<edge;i++)
	{
		if(i%l==0)
			initRow=i;
		for(int j=0;j<edge;j++)
		{	
			if(j%l==0)
				initColumn=j;
			if(auxMatrix[i][j]==0)
			{
				int num=0;
				while(num<=edge)
				{	
					num++;
					if(existsInBlock(initRow,initColumn,num))
						continue;
					if(existsInRow(i,j,num))
						continue;
					if(existsInColumn(i,j,num))
						continue;
						
					if(num>edge)
					{
						_Bool nextC=0;
						_Bool nextR=0;
						num=0;
						do
						{
							j--;
							if(j<0)
							{
								j=edge-1; i--;
								initColumn=edge-l;
							}
							if(i<0)
							{
								printf("Can't solve\n");
								exit(1);
							}
							
							if(j%l==0)
							{
								initColumn=j;
								nextC=1;
							}
							else if(nextC==1)
							{
								initColumn-=l;
								nextC=0;
							}
							
							if(i%l==0)
							{
								initRow=i;
								nextR=1;
							}
							else if(nextR==1)
							{
								initRow-=l;
								nextR=0;
							}
							
						}
					}
					else
					{
						matrix[i][j]=num;
						break;
					}
						
				}
			}
			
		}
		initColumn=0;
	}
	printMatrix();
}


void fillAuxMatrix()
{
	for(int i = 0; i<l*l; i++ )
	{
		for(int j = 0; j < l*l; j++)
		{
			auxMatrix[i][j]=matrix[i][j];		
		}
	}
}


_Bool existsInColumn(int i,int j, int num)
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

_Bool existsInRow(int i,int j, int num)
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

_Bool existsInBlock(int initRow,int initColumn, int num)
{
	_Bool existInBlock=0;
	for(int xB=0;xB<l;xB++)
	{				
		for(int yB=0;yB<l;yB++)
		{

			if(matrix[initRow+xB][initColumn+yB]==num)
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