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
void placeFindingSquare();
void availablePosition();

// Main function
int main(int argc, char *argv[])
{
	readFile(argv[1]);
	
	//placeFindingRow();
	//placeFindingColumn();
	//placeFindingRow();
	//placeFindingColumn();
	//placeFindingSquare();
	//availablePosition();
	
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
	int repeat = 0;
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
			repeat = 1;
		}
		
		//reset counter and rowValues
		counter = 0;
		for(int j=0; j < edge;j++)
		{
			rowValues[j]= 0;
		}
	}
	
	if(repeat == 1)
	{
		placeFindingSquare();
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

void placeFindingSquare() //vê se falta um elemento num quadrado e põe se faltar
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

void availablePosition()
{
	int repeat = 0;
	
	for(int i = 0; i < l; i++) //percorrer os blocos por linhas
	{
		for(int j = 0; j < l; j++) //percorrer os blocos nas colunas
		{			
			for(int k = 0; k < l; k++) //percorrer as linhas dentro do bloco
			{
				for(int t = 0; t < l; t++) //percorrer as colunas dentro do bloco
				{
					for(int n = 1; n <= edge; n++)	//para cada numero de 1 a L, tentar por no bloco de possivel
					{
						if(matrix[i*l+k][j*l+t] == 0) //se o espaço estiver vazio..
						{
							int boolean = 0;
							
							int **blockValues = (int **) malloc(l*sizeof(int *)); //bloco auxiliar para saber onde será possivel por o n, se sobrar um lugar disponivel pomos o n
							for(int v=0; v<l; v++)
							{
								blockValues[v]=(int *) malloc(l*sizeof(int));
							}
							
							/*for(int row = 0; row < l; row++) //começar o bloco a 0's //parece já começa a 0's
							{
								for(int col = 0; col < l; col++)
								{
									blockValues[row][col] = 0;									
								}
							}	*/
							
							for(int row = 0; row < l; row++) //ver se já existe o n no bloco e preencher bloco auxiliar
							{
								for(int col = 0; col < l; col++)
								{
									if(matrix[i*l+row][j*l+col] == n)
									{
										boolean = 1; //o n já existe no bloco
									}
									
									if(matrix[i*l+row][j*l+col] != 0) //preencher o bloco auxiliar com 1's onde existem numeros
									{
										blockValues[row][col] = 1;
									}
								}
							}
							
							if(boolean == 0) //se não existir o n no bloco..
							{
								//ver linhas e colunas adjacentes a ver onde por 1's no bloco auxiliar para dps descobrir onde por o n, se possivel
								
								for(int row = 0; row < l; row++) //Preencher linhas com 1's no bloco auxiliar em linhas em que o n nao pode estar (devido ao n estar noutro bloco, na mesma linha)
								{
									for(int col = 0; col < edge; col++)
									{
										if(matrix[i*l+row][col] == n) //se ouver um n na linha..
										{
											for(int v = 0; v < l; v++) //preencher essa linha com 1's
											{
												blockValues[row][v] = 1;
											}											
										}
									}
								}
								
								for(int col = 0; col < l; col++) //Preencher colunas com 1's no bloco auxiliar em colunas em que o n nao pode estar (devido ao n estar noutro bloco, na mesma coluna)
								{
									for(int row = 0; row < edge; row++)
									{
										if(matrix[row][j*l+col] == n) //se ouver um n na coluna..
										{
											for(int v = 0; v < l; v++) //preencher essa coluna com 1's
											{
												blockValues[v][col] = 1;
											}											
										}
									}
								}
								
								
								// ver se tem um campo no bloco com 0, se tiver, por o n lá
								int counter0 = 0;
								int x = 0, y = 0;
								for(int row = 0; row < l; row++) //começar o bloco a 0's //parece já começa a 0's
								{
									for(int col = 0; col < l; col++)
									{
										if(blockValues[row][col] == 0)
										{
											counter0++;
											x = row;
											y = col;
										}											
									}
								}
								
								if(counter0 == 1)
								{
									matrix[i*l+x][j*l+y] = n;
									repeat = 1;
								}
								counter0 = 0;
								
								/*
								for(int row = 0; row < l; row++) //começar o bloco a 0's //parece já começa a 0's
								{
									for(int col = 0; col < l; col++)
									{
										printf("%d", blockValues[row][col]);									
									}
									printf("\n");
								}
								printf("\n");*/
							}
						}
					}
				}
			}
		}
	}
	
	if(repeat == 1)
	{
		availablePosition();
	}
	else
	{
		placeFindingRow();
	}
	
	
}

void printMatrix()
{
	int i, j;
	#pragma omp parallel for private (i, j, matrix)
	for(i = 0; i<edge; i++ )
	{
		for(j = 0; j < edge; j++)
		{
			printf("%d ",matrix[i][j]);
		}
		printf("\n");
	}
}