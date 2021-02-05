#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int pow2 (int x) { return x*x;}
int main(int argc , char** argv)
{
    MPI_Init(&argc,&argv);
    double begint; 
    double endt;
    double a;
    double b;
    double h;
    int count;
    int rank;
    double tmp1;
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD , &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD , &rank);
    int m= world_size;
    double myarr [3];
    if (rank ==0) 
    {
        int n;
        printf("             Please enter a , b , n \n");
        if (scanf ("%lf" ,&a)) {};
        if (scanf ("%lf" ,&b)) {};
        if (scanf ("%d" ,&n)) {};
        begint = MPI_Wtime();
        count= n/world_size;
        h= (a-b)/n;
        tmp1 = (b-a) / (2*n);
        myarr[0]=a;
        myarr[1]=h;
        myarr[2]=count;
    }

    MPI_Bcast (myarr , 4 , MPI_DOUBLE , 0 , MPI_COMM_WORLD );

    if (rank!= 0)
    {
        a= myarr[0];
        h= myarr[1];
        count= myarr[2];
        count= (int) count;
    }
    double sumarr;
    int mystart;
    int myend;
    mystart= rank*count;
    myend = mystart + count;
    double tmp2;
    double tmp3=0;
    int i;
    for (i=mystart ; i<myend ; i++)
    {
   	 tmp2= (i*h)+a;
   	 tmp3+= pow2 (tmp2);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Reduce (&tmp3 , &sumarr , 1 , MPI_DOUBLE , MPI_SUM, 0, MPI_COMM_WORLD );
    if (rank ==0)
    {

        double res;
        sumarr= 2*sumarr;
        res= tmp1 * (pow2(a) + sumarr + pow2(b));  
        endt = MPI_Wtime();
        printf ("          Result is : %f  and spent time is : %f \n " , res , endt - begint);
    }

MPI_Finalize();
return 0;
}