#include <stdio.h>
#include <string.h>
#include "mpi.h"




int countPrime(int a,int b)
{
    int numOfPrimes=0;
    int i=a;
    for (i ; i < b; i++)
    {
        if (i == 1 || i == 0)
        continue;

        int prime = 1;

        int j=2;
        for (; j <= i / 2; ++j)
        {
            if (i % j == 0) {
                prime = 0;
                break;
            }
        }
        if (prime == 1)
            numOfPrimes++;
    }
    return numOfPrimes;
}

int main(int argc , char * argv[])
{
	int my_rank;		/* rank of process	*/
	int p;			/* number of process	*/
	int source;		/* rank of sender	*/
	int dest;		/* rank of reciever	*/
	int tag = 0;		/* tag for messages	*/
	char message[100];	/* storage for message	*/
	MPI_Status status;	/* return status for 	*/
				/* recieve		*/

	/* Start up MPI */
	MPI_Init( &argc , &argv );

	/* Find out process rank */
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	/* Find out number of process */
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	if( my_rank == 0)
	{
		int x=0,y=20,totalSum=0;

		scanf("%d",&x);
		scanf("%d",&y);

		//calculate r
		int r=(y-x)/(p);
		int i=0;
		//send x and r
		for(i=1;i<p;i++)
        {
            MPI_Send( &y, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send( &x, 1, MPI_INT, i, 0, MPI_COMM_WORLD);

            if(y-x>=p)
            {
                MPI_Send( &r, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
            else
            {
                int newRange=1;
                MPI_Send( &newRange, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
        }

        //int numOfPrimes=countPrime(r*(p-1),y+1);

        int newY=x+r;
        if(y-x<p)
            newY=1+x;
        int numOfPrimes=countPrime(x,newY);
        totalSum+=numOfPrimes;



        printf("number of prime numbers in process %d",my_rank);
        printf(" is:%d\n",numOfPrimes);
        /*printf("R:%d\n",r);
        printf("A:%d\n",x);
        printf("B:%d\n",newY);*/




        //calculate total sum
        for(i=1;i<p;i++)
        {
            int sum;
            MPI_Recv( &sum, 1, MPI_INT, i, 0, MPI_COMM_WORLD,&status);
            totalSum+=sum;
        }

        //printing total sum
        printf("Total number of prime numbers is:%d\n",totalSum);
	}else
	{
	    //receive y and r
	    int y,x,r;
        MPI_Recv(&y, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status );
        MPI_Recv(&x, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status );
        MPI_Recv(&r, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status );


        //calculate upper and lower
        int a=(my_rank)*r+x;
        int b=a+r;
        if(my_rank==p-1 || b>y)
            b=y;


        //count primes
        int numOfPrimes=countPrime(a,b);

        //printing number of prime in each process
        printf("number of prime numbers in process %d",my_rank);
        printf(" is:%d\n",numOfPrimes);
        /*printf("R:%d\n",r);
        printf("A:%d\n",a);
        printf("B:%d\n",b);*/

        //sending partial count
        MPI_Send( &numOfPrimes, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

	}

	/* shutdown MPI */
	MPI_Finalize();
	return 0;
}
