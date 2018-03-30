#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include <string.h>

void readFile();
void printMatrix();
void solveSudoku();
void freeMatrix();
_Bool existsInBlock();
_Bool existsInRow();
_Bool existsInColumn();


//Global Variables
int ***matrix;	
int **auxMatrix;						
int l=0,edge=0;
int firstI, firstJ, lastI, lastJ;

// Main function
int main(int argc, char *argv[])
{
<<<<<<< HEAD
	//printf("numThreads: %d\n", omp_get_num_threads());
	//omp_set_num_threads(16);
	
	/*#pragma omp parallel
	{
		printf("numThreads: %d\n", omp_get_num_threads());
	}*/
=======
	//omp_set_num_threads(2);
>>>>>>> 771e0a25cd6e456a4945e18abadd610db76a87b5
	readFile(argv[1]);
	solveSudoku();
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
	
	//to obtain the first and last cell where there is a zero 
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

void printMatrix(int k)
{
	
	for(int i = 0; i<l*l; i++ )
	{
		for(int j = 0; j < l*l; j++)
		{
			printf("%d ",matrix[i][j][k]);
		}
		printf("\n");
	}
		
	printf("\n");
	
}


void solveSudoku()
{
	_Bool end = 0;
	_Bool rollBack=1;
	
<<<<<<< HEAD
	#pragma omp parallel for firstprivate(rollBack, end)
=======
	#pragma omp parallel for firstprivate(rollBack,end)
>>>>>>> 771e0a25cd6e456a4945e18abadd610db76a87b5
	for(int n = 1; n <= edge; n++)
	{
		end = 0;
		if(existsInBlock(n, firstI, firstJ, n))
			continue;
		if(existsInRow(firstI, firstJ, n, n))
			continue;
		if(existsInColumn(firstI, firstJ, n, n))
			continue;
		
		matrix[firstI][firstJ][n-1] = n;

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
						
						while( matrix[i][j][n - 1] <= edge)
						{						
							matrix[i][j][n-1]++;
							if(existsInBlock(matrix[i][j][n-1],i,j, n))
								continue;
							if(existsInRow(i,j,matrix[i][j][n-1], n))
								continue;
							if(existsInColumn(i,j,matrix[i][j][n-1], n))
								continue;
							break;
						}
						

						if(matrix[i][j][n-1]>edge)
						{
							
							rollBack=1;
							matrix[i][j][n-1]=0;
							do
							{	
								j--;
								if(j<0)
								{
									j=edge-1; i--;
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
								printMatrix(n-1);
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
		printf("Can't solve with %d\n",n);
	}
	printf("No solution\n");
}

_Bool existsInColumn(int i,int j, int num, int n) //identify if the number is already present in the column
{
	_Bool existInColumn=0;
	for(int row=0;row<edge;row++)
	{
		if(row==i)
			continue;
									
		if(matrix[row][j][n-1]==num)
		{
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
			existInRow=1;
			break;
		}
	}
	return existInRow;
}

_Bool existsInBlock(int num,int i,int j, int n) //identify if the number is already present in the block
{
	_Bool existInBlock=0;
	for(int row = 0; row < l; row++)
	{
		for(int col = 0; col < l; col++)
		{
			if(matrix[(i/l)*l+row][(j/l)*l+col][n-1] == num && (i != (l*(i/l) + row) && j != (l*(j/l) + col)))
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