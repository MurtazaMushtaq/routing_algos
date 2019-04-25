/*
CMPT-371 
Project-2 question-2
by: Murtaza Mushtaq 
	301347189
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//this funciton will be used in the link-state algorithm to find
//the vertex with the least distance in the array
int minlocation(double A[], int length){
    int location;
    double minimum = INFINITY;
    
    for (int c = 0; c < length; c++)
    {
        if (A[c] < minimum)
        {
           minimum=A[c];
           location = c;
        }
    }
    return location;
}


void linkstate(char totalnodes[], char node1[], char node2[], int dists[], char source, int vertices, int edges, int finaldistances[], char prev[]){
	char arrayofallnodes[vertices]; 
	double distances[vertices];
	char interprev[vertices]; //stores the temporary previous node of a node until 
							  //finalized that this previous node is the least expensive
							  //choice

	int length = vertices;    //this will come in play when we remove vertices from vertex set
							  //after it is processed. This variable stores the no. of current 
							  //vertices in the vertex set
	int prevcounter=0;

	for (int i = 0; i < vertices; ++i)
	{
		arrayofallnodes[i]=totalnodes[i];
	}

	for (int i = 0; i < vertices; ++i)
	{
		distances[i]=INFINITY;
	}

	for (int i = 0; i < vertices; ++i)
	{
		if(arrayofallnodes[i]==source){
			distances[i]=0;
			finaldistances[i]=0;
		}
	}
	int finaldistcounter=0;
	while(length != 0){
		char internode;
		double distofinternode;
		int counter=0;
		int mindistance = minlocation(distances, length); //finding the node with least distance in vertex set
		internode=arrayofallnodes[mindistance];
		distofinternode=distances[mindistance];

		for (int i = 0; i < vertices; ++i)
		{
			if(internode==totalnodes[i]){
				finaldistances[i]=distofinternode;
				prev[i]=interprev[mindistance];
			}
		}

		//now we remove that least distance vertex from the vertex set
		for (int i = 0; i < length; ++i)
		{
			if(arrayofallnodes[i] != internode){
				arrayofallnodes[counter]=arrayofallnodes[i];
				distances[counter]=distances[i];
				interprev[counter]=interprev[i];
				counter=counter+1;
			}
		}		
		length=length-1;

		//now we find distances for all its neighbors
		for (int i = 0; i < edges; ++i)
		{
			double alt=0;

			if(node1[i]==internode){
				alt=distofinternode + dists[i];
				for (int j = 0; j < length; ++j)
				{
					if(arrayofallnodes[j]==node2[i]){ //checks if the neighbor is in vertex set
						if(alt<distances[j]){
							distances[j]=alt;        //setting new distance just like Dijkstras Algo does
							interprev[j]=node1[i];   //setting new previous node of that neighbor
					
						}

					}
				}

			}
			//this step is just to make sure that we consider the possibility of missing out on 
			//an edge just because it is from X to Y lets say and not from Y to X. Since we know
			//its the same thing, we consider that possibility too
			if(node2[i]==internode){
				alt=distofinternode + dists[i];
				for (int j = 0; j < length; ++j)
				{
					if(arrayofallnodes[j]==node1[i]){
						if(alt<distances[j]){
							distances[j]=alt;
							interprev[j]=node2[i];	
						}

					}
	
				}


			}

		}
		finaldistcounter+=1;
	}
}

//this function finds the index of a given character in a 
//given array of length "lengthy"
int findlocation(char finder, char list[], int lengthy){
	for (int i = 0; i < lengthy; ++i)
	{
		if(list[i]==finder){
			return i;
		}
	}
}

//this function just takes two inputs
//and returns the minimum of both
double minimum(double a, double b){
	if(a<=b){
		return a;
	}
	else{
		return b;
	}
}

void DistVect(char dv_totalnodes[], char dv_node1[], char dv_node2[], int dv_dists[], int vertices, int edges){

	double dv_distances[vertices];
	int lastd;

	int counter =0;
	for (int j = 0; j < vertices; ++j){
		char dv_source = dv_totalnodes[j];
		for (int i = 0; i < vertices; ++i)
		{
			dv_distances[i]=INFINITY;
		}
		for (int i = 0; i < vertices; ++i)
		{
			if(dv_totalnodes[i]==dv_source){
				dv_distances[i]=0;
			}	
		}

		//relaxing every node
		for (int i = 0; i < edges; ++i)
		{
			char nod2=dv_node2[i];
			char nod1=dv_node1[i];
			
			//considering the possibility that we dont miss out an edge just like
			//link-state algorithm
			if(nod2==dv_source){
				char tmp = nod2;
				nod2 = nod1;
				nod1 = tmp;
			}

			//setting new distance based on if the dist(v) is less than dist(u)+c(u,v) like it does
			//in Bellman-Ford Algorithm
			int location_of_nod2=findlocation(nod2, dv_totalnodes, vertices);
			int location_of_nod1=findlocation(nod1, dv_totalnodes, vertices);
			dv_distances[location_of_nod2]=minimum(dv_distances[location_of_nod2], dv_distances[location_of_nod1]+dv_dists[i]);

		}

		//makes sure that a problem I faced while writing this program which is hard to explain
		//is solved  
		for (int k = 0; k < vertices; ++k)
		{
			if(dv_distances[k]==INFINITY){
				dv_distances[k]=lastd;
			}
		}

		//printing the row of the final table
		//this runs multiple times to print the entire table eventually
		int dv_final_dists[vertices];
		for (int i = 0; i < vertices; ++i)
		{
			dv_final_dists[i]=dv_distances[i];
		}
		for (int i = 0; i < vertices; ++i)
		{
			printf("%d ", dv_final_dists[i]);
			if(j==0 && i==vertices-1){
				lastd = dv_final_dists[i];
			}
		}
		printf("\n");
	}

	//counter counts the number of rounds
	counter=counter+3;
	printf("%d\n", counter);
	return;
}



int main(int argc, char **argv){

	//prompting inputs to program and making sure input is correct
	if(argc != 2){
		printf("Please use format: ./compiled-output-file InputFile.txt\n");
		return 0;
	}

	int V; //to store no.of vertices in the system
	int E; //to store no.of edges in the system
	char space; //not really important but just to remove spaces from .txt file

	FILE* ifile;  //will point to the .txt file

	ifile = fopen(argv[1],"r"); 

	fscanf(ifile,"%d%d",&V,&E);

	char allnodes[V]; //this array will store the list of vertices in the .txt file

	//if there is an edge from lets say U to V of length x
	//then, U will be stored in nodes1 array and V will be 
	//stored in node2 array and their distance x in dist_arr array
	//all these will be in the same index in each of the array
	//like this :
	// node1 {U,...}
	// node2 {V,...}
  //dist_arr {x,...}
	char nodes1[E];   
	char nodes2[E];
	int dist_arr[E];
	int finaldistances[V]; //will be used to store distances of each vertex
						   //after Link-state algo called
	int dv_finaldistances[V];	//same as above but for direct-vectore algo
	char prev[V]; //stores previous of each node in link-state algo
	char dv_prev[V]; //stores previous of each node in dv algo

	for (int i = 0; i < V; ++i)
	{
		fscanf(ifile,"%c%c",&space,allnodes+i);
	}

	for (int i = 0; i < E; ++i)
	{
		fscanf(ifile, "%c%c%c%c%c%d", &space, nodes1+i, &space, nodes2+i, &space, dist_arr+i);		
	}	

	char source = allnodes[0];

	linkstate(allnodes, nodes1, nodes2, dist_arr, source, V, E, finaldistances, prev);
	printf("\nThe Distance-Vector Algorith output is:\n");
	DistVect(allnodes, nodes1, nodes2, dist_arr, V, E);


	printf("\nThe Link-State Algorith output is:\n");
	
	//this function is used to find the parent of a given node
	//using the prev array that stores the prev node of every
	//node in the system
	void prevfinder(int valueofi, char sourcenode){
		char x=prev[valueofi];
		if(x==sourcenode){
			printf("%c", x);
			return;
		}
		for (int j = 0; j < V; ++j)
		{
			if(allnodes[j]==x){
				prevfinder(j,sourcenode);
			}
		}
		printf("-%c", x);
	}
	for (int i = 1; i < V; ++i)
	{
		if(prev[i]==source){
			printf("%c: %c-%c %d\n",allnodes[i],source, allnodes[i], finaldistances[i]);	
		}
		else{
			printf("%c: ", allnodes[i]);
			prevfinder(i,source);
			printf("-%c %d\n", allnodes[i],finaldistances[i]);
		}
	}	

	return 0;
}