#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include <string.h>

void readFile();
void printMatrix();
void solveSudoku();

//Global Variables
int **matrix;							//Declaration of structure of Sudoku
int l = 0;								
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
	_Bool pass=1;
	while(1)
	{
		pass=1;
		for(int i=0;i<edge;i=i+l)/////////////////////////////steps into block
		{
		
			for(int j=0;j<edge;j=j+l)
			{
				
				for(int x=0;x<l;x++)/////////////////////////identify a 0 in a block
				{
					
					for(int y=0;y<l;y++)
					{
						
						if(matrix[i+x][j+y]==0)
						{
							pass=0;
							for(int num=1;num<=edge;num++)//////////////////////try to put the number in the empty space
							{
								
								_Bool existInBlock=0;
								for(int xB=0;xB<l;xB++)/////////////////////////////////////to see if the number already exists in the block 
								{
									
									for(int yB=0;yB<l;yB++)
									{
										
										if(matrix[i+xB][j+yB]==num)
										{
											existInBlock=1;
											break;
										}
										
									}
									if(existInBlock)
										break;
								}///////////////////////////////////////////////////////////
								if(existInBlock)
									continue;
								
								_Bool existInRow=0;
								for(int column=0;column<edge;column++)/////////////////////////////////////to see if the number already exists in the row
								{
									if(column==y+j)
										continue;
									
									if(matrix[i+x][column]==num)
									{
										existInRow=1;
										break;
									}
								}//////////////////////////////////////////////////////////////////////////
								if(existInRow)
									continue;
								
								_Bool existInColumn=0;
								for(int row=0;row<edge;row++)/////////////////////////////////////to see if the number already exists in the column
								{
									if(row==x+i)
										continue;
									
									if(matrix[row][j+y]==num)
									{
										existInColumn=1;
										break;
									}
								}/////////////////////////////////////////////////////////////////
								if(existInColumn)
									continue;
								matrix[i+x][j+y]=num;
								break;
							}//////////////////////////////////////////////////
							
							
						}
					}
				}/////////////////////////////////////////////identify a 0 in a block
			}
		}/////////////////////////////steps into block
		if(pass)
			break;
	}
	printMatrix();
}