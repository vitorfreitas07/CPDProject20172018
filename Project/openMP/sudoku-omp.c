#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include <string.h>

//funtiopm prototypes
void readFile();
void printMatrix();
int canNbeHere();
void solveSudoku();

//global Variables
int ***matrix;	
int **auxMatrix;						
int l=0,edge=0;
int firstI, firstJ, lastI, lastJ, numThreads;

//main function
int main(int argc, char *argv[])
{
	numThreads = omp_get_max_threads();
	readFile(argv[1]);
	solveSudoku();
	return 0;
}

//read the file and fill the matrix with its value
void readFile(char file[])
{
	int value;
    FILE *input = fopen(file, "r"); //read only  
       
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
			matrix[i][j]=(int *) malloc(numThreads*sizeof(int)); 
		}	
	}
	
	int i=0;	
	int j=0;
    while ( fscanf(input, "%d", &value ) == 1 ){ 
		if(j < edge) 
		{
			for(int k = 0; k < numThreads; k++)
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
			
			for(int k = 0; k < numThreads; k++)
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
	
	#pragma omp parallel for firstprivate(end, rollBack) schedule(dynamic,1) //num_threads(numThreads)
	for(int n = 1; n <= edge; n++) //test all the possible numbers from 1 to edge in the first cell where there's a 0
	{
		int myId = omp_get_thread_num();
		end = 0;
		if(canNbeHere(firstI, firstJ, n, myId+1))
		{
			//printf("Thread %d can't solve with %d\n", myId,n);
			continue;
		}
		matrix[firstI][firstJ][myId] = n;

		for(int i=0;i<edge;i++) //to go through every row of the matrix
		{
			for(int j=0;j<edge;j++) //to go through every column of the matrix
			{	
				
				if(auxMatrix[i][j]==0)  //if a given cell started with zero
				{
					rollBack=1;
					while(rollBack==1)
					{
						rollBack=0;
						int try = matrix[i][j][myId];
						
						while( try <= edge)  //try every number from 1 to edge
						{						
							try++;
							if(canNbeHere(i,j,try, myId))
								continue;
							matrix[i][j][myId] = try;
							break;
						}

						if(try>edge) //if the try variable is edge+1
						{
							rollBack=1;
							matrix[i][j][myId]=0;
							do
							{	//go to the previous cell
								j--;
								if(j<0)
								{
									j=edge-1; i--;
								}
								if(i == firstI && j == firstJ) 
								{
									end = 1;  //can't solve with this n
									break;
								}	
									
							}	
							while(auxMatrix[i][j]!=0);
						}
						else
						{
							if(i == lastI && j == lastJ) //if it's the last cell
							{
								#pragma omp critical
								{
									printMatrix(myId);
									exit(0);
								}
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
		//printf("Thread %d can't solve with %d\n", myId,n);
	}
	printf("No solution\n");
}


int canNbeHere(int i,int j, int num, int n) //check if there's an equal value in the same row, column and block
{
	//Checks Block
	for(int row = 0; row < l; row++)
	{
		for(int col = 0; col < l; col++)
		{
			if(matrix[(i/l)*l+row][(j/l)*l+col][n] == num )
			{
				return 1;
			}
		}
	}
	//Cecks Row
	for(int column=0;column<edge;column++)
	{						
		if(matrix[i][column][n]==num)
		{
			return 1;
		}
	}
	//Checks Column
	for(int row=0;row<edge;row++)
	{				
		if(matrix[row][j][n]==num)
		{
			return 1;
		}
	}
	return 0;
}