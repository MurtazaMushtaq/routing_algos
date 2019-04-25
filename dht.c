/*
CMPT-371 
Project-1 question-3
by: Murtaza Mushtaq 
	301347189
This is an explanation for distributed hash table: As the nodes are read from the file
into an array, I sort the array since it will be easier to find the successive
nodes in that way in my opinion. Then as the keys are read, the node it will be
stored at is printed in the output file. More explanation is in comments. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//This function will return the node at which the key will be stored if a key
//is inputted as the parameter. If a node is inputted then returns the successor of this 
//node by simply comparing to the sorted array of nodes. k is the key/node, 
//A[] is the sorted array, N is the number of nodes and S is the Hash space
int nextnode(int k, int A[], int N, int S){
	for (int i = k; i <= S; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			if(i==A[j]){
				return i;
			}
		}
	}
	return A[0];
}

//Just an algorithm I wrote for sorting the array of nodes
//Might not be the most efficient one but certainly does the job
void sort(int A[], int n){
	int i=1;
	while(i<n){
		int j=i;
		while(j>0 && A[j-1]>A[j]){
			int k = A[j];
			A[j]=A[j-1];
			A[j-1]=k;
			j=j-1;
		}
		i=i+1;
	}
}

int main(int argc, char **argv){

	//prompting inputs to program and making sure input is correct
	if(argc != 3){
		printf("Please use format: ./compiled-output-file XXX.in XXX.out\n");
		return 0;
	}

	int S; //Hash Space
	int N; //Number of nodes
	int M; //NUmber of keys
	char com; //we will see why i have this later on

	//the array of nodes. The number 512 is because the max number of nodes we can
	//have is (S+1) and S=2^k-1 where max k can be 9. So, max S will be 2^9 and hence 512
	int node_arr[512]; 

	//Will be used to point to the input and output files
	FILE* inputfile;
	FILE* outputfile;

	//we know argv[0] will be the compiled-output-file argv[1] will be XXX.in 
	//and argv[2] will be XXX.out
	inputfile = fopen(argv[1],"r");
	outputfile = fopen(argv[2],"w");

	//just reading the input file for the values of S,N and M
	fscanf(inputfile,"%d %d %d",&S,&N,&M);

	//just reading the input file for the nodes and making an array 
	//we did mod S here since we wanna make sure all the nodes are
	//within the range [0...S]
	for (int i = 0; i < N; ++i)
	{
		fscanf(inputfile,"%d%c",node_arr+i,&com);
		node_arr[i]=node_arr[i]%S;	
	}

	//sorting the array to make sure that the node on the right of each node
	//is its successor
	sort(node_arr, N);

	//this variable node is where the keys about to be read from the input
	//file will be stored at one by one
	int node;
	int key;

	for(int j=0;j<M;++j){
		fscanf(inputfile,"%d%c",&key,&com);
		
		//once again making sure that every key is within range [0...S]
		key=key%S;

		//finding the node at which this key is stored
		//Recall this value of node is also equal to id to be used in fingertable
		int node=nextnode(key, node_arr, N, S);

		fprintf(outputfile,"%d\n", node);
		//our i values are 0,1 and 2 so our 2^i values are 1,2 and 4 as seen below
		//Mod S used to make sure the id+2^i is within the range [0...S]
		fprintf(outputfile,"0 %d %d\n", (node+1)%S, nextnode(((node+1)%S), node_arr, N, S));		
		fprintf(outputfile,"1 %d %d\n", (node+2)%S, nextnode(((node+2)%S), node_arr, N, S));
		fprintf(outputfile,"2 %d %d\n", (node+4)%S, nextnode(((node+4)%S), node_arr, N, S));

	}

	fclose(inputfile);
	fclose(outputfile);

	return 0;
}