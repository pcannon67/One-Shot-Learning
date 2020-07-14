#include <stdio.h>
#include <stdlib.h>

double** train;        //freed
double** ttrain;       //freed
double** mtrain;       //freed
double** test;         //freed
double** aug;          //freed
double** inverse;      //freed
double** prices;       //freed
double** W;            //freed
double** answer;       //freed
int rows2; 

void transposeTrain(int, int);
void multiplyTrains(int, int);
void createTestMatrix(char** argv, int);
void makeInverse(int, int);
void CalculateInverse(int, int);
void Multiply(int, int);

void transposeTrain(int rows, int cols){
	int i;
	int j;
	ttrain = (double**)malloc(cols * sizeof(double));
	
	for(i = 0; i < cols; i++){
		ttrain[i] = (double*)malloc(sizeof(double) * rows);
	}
	
	
	for(i = 0; i < cols; i++){
		for(j = 0; j < rows; j++){
			ttrain[i][j] = train[j][i];
			//printf("%lf ", ttrain[i][j]);
   	}
   	//printf("\n");
	}
}

void multiplyTrains(int rows, int cols){
	int i;
	int j;
	int k;
	
   mtrain = (double**)malloc(cols * sizeof(double));
   
   for(i = 0; i < cols; i++){
   	mtrain[i] = (double*)malloc(cols * sizeof(double));
   }
   
  
	
	for(i = 0; i < cols; i++){
		for(j = 0; j < cols; j++){
			for(k = 0; k < rows; k++){
				mtrain[i][j] += ttrain[i][k] * train[k][j];
			}
		}
	}
}

void createTestMatrix(char** argv, int cols){
	int i;
	int j;
	rows2 = 0;
	
	FILE * fptr;
	
	fptr = fopen(argv[2],"r");
	 fscanf(fptr, "%d\n", &rows2);
	 
	 test = (double**)malloc(rows2 * sizeof(double));
	 
	 for(i = 0; i < rows2; i++){
	 	test[i] = (double*)malloc(cols * sizeof(double));
	 }
	 
	 for(i = 0; i < rows2; i++){ 
   	for(j = 0; j < cols; j++){
   		if(j == 0) {
   			test[i][j] = 1;
   		}else{
   		fscanf(fptr, "%lf%*c", &test[i][j]); 
   		}
   	}
   }	
   	
 fclose(fptr);
}
 
void makeInverse(int rows, int cols){
	int i;
	int j;
	int augSize = cols * 2;

	double** identity = (double**)malloc(cols * sizeof(double)); //freed
	inverse = (double**)malloc(cols * sizeof(double));
	
	for(i = 0; i < cols; i++){
		for(j = 0; j < cols; j++){
			identity[i] = (double*)malloc(cols * sizeof(double));
			inverse[i] = (double*)malloc(cols * sizeof(double));
		}
	}
	
	for(i = 0; i < cols; i++){ //sets up identity matrix	
		for(j = 0; j < cols; j++){
			if( i == j){
				identity[i][j] = 1;
			}else{
				identity[i][j] = 0;
			}
		}
	}
	

   aug = (double**)malloc(cols * sizeof(double));
   
   for(i = 0; i < cols; i++){ //allocate memory of inverse
   	aug[i] = (double*)malloc(augSize * sizeof(double));
   	}
   	

   	
   for(i = 0; i < cols; i++){
   	for(j = 0; j < cols; j++){
   		aug[i][j] = mtrain[i][j];
   		aug[i][j+cols] = identity[i][j];
   	}
   }
  
 for(i = 0; i < cols; i++){
 	free(identity[i]);
 	free(mtrain[i]);
 }
 free(identity);
 free(mtrain);
  

	CalculateInverse(augSize, cols);
}
 
void CalculateInverse(int augSize, int cols){ 
	int i;
	int j;
	int k;
	//int m;
	int n;
	double num = 0;
	double num2 = 0;
	double pivot = 0;
	

	for(i = 0; i < cols; i++){ //pivot = 1
	  pivot = aug[i][i]; //sets pivot to diagonal
	  
		for(j = 0; j < augSize; j++){ //divides entire row by value at pivot position 
				aug[i][j] = aug[i][j] / pivot; // 
		}
		
			for(n = i+1; n < cols; n++){ //starts looping at next row
				 num = aug[n][i];
				for(k = 0; k < augSize; k++){
						aug[n][k] = aug[n][k] - aug[i][k] * num; 
					
			
						}
               
					}
			}
	
	 for(i = 0; i < cols; i++){
		    for(n = i+1; n < cols; n++){ //creates ut
				num2 = aug[i][n];
				for(k = 0; k < augSize; k++){
						aug[i][k] = aug[i][k] - aug[n][k] * num2;			
					}
				}
			}
			
   for(i = 0; i < cols; i++){
		for(j = 0; j < cols; j++){
			inverse[i][j] = aug[i][j+cols];
		}
	}
	for(i = 0; i < cols; i++){
		free(aug[i]);
	}
	free(aug);
				
}

void Multiply(int rows, int cols){
	int i;
	int j;
	int k;
	double ** temp = (double**)malloc(sizeof(double) * cols);
	
	for(i = 0; i < cols; i++){
		temp[i] = (double*)malloc(sizeof(double) * rows);
	}
	
	for(i = 0; i < cols; i++){ //(X^T*X)^-1 * X^T 
		for(j = 0; j < rows; j++){
			for(k = 0; k < cols; k++){
				temp[i][j] += inverse[i][k] * ttrain[k][j]; 	
			}
		}
	}

	W = (double**)malloc(sizeof(double) * cols);
	
	for(i = 0; i < cols; i++){
		W[i] = (double*)malloc(sizeof(double) * 1);
	}
	
	for(i = 0; i < cols; i++){ //PREV * Y
		for(j = 0; j < 1; j++){
			for(k = 0; k < rows; k++){
				W[i][j] += temp[i][k] * prices[k][j]; 
			}
		}
	}
	
	answer = (double**)malloc(sizeof(double) * rows2);
	
 	for(i = 0; i < rows2; i++){
		answer[i] = (double*)malloc(sizeof(double) * 1);
	}
	
	 for(i = 0; i < rows2; i++){ //PREV * Y
		for(j = 0; j < 1; j++){
			for(k = 0; k < cols; k++){
				answer[i][j] += test[i][k] * W[k][j]; 
			}
		}
	}
		

	
	for(i = 0; i < rows2; i++){
		for(j = 0; j < 1; j++){
			printf("%0.0lf\n", answer[i][j]);
		}
	}
	
	for(i = 0; i < cols; i++){
		free(ttrain[i]);
		free(inverse[i]);
		free(W[i]);
	}
	free(ttrain);
	free(inverse);
	free(W);
	
	for(i = 0; i < rows2; i++){
		free(answer[i]);
		free(test[i]);
	}
	free(answer);
	free(test);
}
	


int main(int argc, char** argv){
	int i;
	int j;
	int k = 0;
	int cols = 0;
	int rows = 0;
	int pcol = 1;
	FILE * fptr;
	
	if(argc<3 || argv[1] == NULL || argv[2] == NULL){ //checks 2 files, first not empty
		printf("error\n");
		exit(0);
	}

	fptr = fopen(argv[1], "r");
 
   fscanf(fptr, "%d\n", &cols);
   fscanf(fptr, "%d\n", &rows);
   
   cols = cols + 1; 
 
   train = (double**)malloc(sizeof(double) * rows);
   prices = (double**)malloc(sizeof(double) * rows);
   
   for(i = 0; i < rows; i++){
   	train[i] = (double*)malloc(sizeof(double) * cols);
   	prices[i] = (double*)malloc(sizeof(double) * pcol);
   }
   
   for(i = 0; i < rows; i++){ //creates train matrix and price array
   	for(j = 0; j < cols; j++){
   		if(j == 0){ //appends 1 to beginning of matrix
   			train[i][j] = 1;
   		}else{
   		fscanf(fptr, "%lf%*c", &train[i][j]); //saves attributes in matrix
   		 }
   		 
   		if(j == cols - 1){
   			fscanf(fptr, "%lf\n", &prices[k][0]); //saves price of house in an array
   			k++;
   		}
   	}
  
   }
   
   /*for(i = 0; i < rows; i++){
   	for(j = 0; j < 1; j++){
   		printf("%lf ", prices[i][j]);
   	}
   	printf("\n");
   }*/
   
 
   
   transposeTrain(rows, cols);
   multiplyTrains(rows, cols);
   createTestMatrix(argv, cols);
   makeInverse(rows, cols);
   Multiply(rows, cols);
   
   //FREE EVERYTHING WHEN DONE
   
   for(i = 0; i < rows; i++){
   		free(train[i]);
   		free(prices[i]);
   }
   
   
   free(train);
   free(prices);

     
   fclose(fptr);
	return 0;
}