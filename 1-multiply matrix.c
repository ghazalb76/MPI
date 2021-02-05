#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc , char** argv)
{
	MPI_Init(&argc,&argv);
	int n=16;
	int npow2= 16*16;
	int world_size;
	int b[n][n];
	MPI_Comm_size(MPI_COMM_WORLD , &world_size);
	int m= world_size;
	int reg= npow2 / m;
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD , &rank);
	int fstart= rank*reg;
	int fend= fstart + reg;
	int myres[reg];
	int mystart = (int) (fstart/n);
	int myend = (int) (fend/n);
	int dim= myend - mystart;
	int a[dim][n];
	int i , j , k=0;
	if (world_size < 2)
	{
		fprintf(stderr , "world size must be more than 2 %s\n" , argv[0]);
		MPI_Abort(MPI_COMM_WORLD ,1);
	}

	//All processes

	for (i=0 ; i<n ; i++){
		for(j=0;j<n;j++){
			b[i][j]= (i+1)*(j+1);}
	}

	//Each process initializes its contribution

	int tmpi=0;
	for ( i =mystart ;i< myend ; i++){
		for ( j=0 ; j<n ;j++) {
			a[tmpi][j]= (i+1)*(j+1);}
		tmpi++;
	}
      int inx=0;
      for ( i =0 ;i< dim ; i++)
	 {
		for ( j=0 ; j<n;j++) 
		 {
                  myres[inx] = 0;
		  int tmpres=0;
		  for ( k=0 ; k<n ;k++) 
		   {
                     tmpres= a[i][k] * b[k][j];
		     myres[inx] = myres[inx] +tmpres;
                   }
		      inx++;
		      if (inx>= reg) goto xlabel;
            }
        }

xlabel:
	MPI_Barrier(MPI_COMM_WORLD);
	int *recvmatrix=NULL;		
	if (rank ==0 )
	{ 
		recvmatrix= malloc(sizeof(int) * npow2);
	}
	MPI_Gather(myres , reg, MPI_INT , recvmatrix , reg, MPI_INT , 0 , MPI_COMM_WORLD);	
	if (rank ==0 )
		{ 
		FILE *f = fopen("result.txt", "wb");
		for ( i =0 ; i<npow2 ; i++)
		  {
		  printf( "%d ," , recvmatrix[i]);
	  	  if (((i+1) % n) ==0) printf ("\n");
		  if (f) {
		       fprintf( f , "%d ," , recvmatrix[i]);
	  	       if (((i+1) % n) ==0) fprintf (f ,"\n");
		}}
	fclose(f);}
	MPI_Finalize();
	free(recvmatrix);
	return 0;
}



