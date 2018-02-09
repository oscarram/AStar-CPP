#define goal 519833
#define start  461929

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h> 
#include <cstring>
#include <climits>
#include <cfloat>
//#include <math.h>
#include <cmath>
#include <omp.h>

#include "Structures.cpp"
#include "Heuristic.cpp"
#include "BinarySearch.cpp"
#include "Queues.cpp"


node *nodes;
connection *conns;

AStarNode *astar;


unsigned long nodeIndex=0;
unsigned long connsCounter=0;

unsigned long* openListBase;
unsigned long* closedListBase;
unsigned long MaxListSize;

QueueStar openList;
QueueStar closedList;

unsigned long CurrentNode;
bool flag_for_break=false;




int main(){

omp_set_num_threads(4);
/*******************************************Reading the files***********************************************/
	std::FILE *Conns_infile;
	std::FILE *Nodes_infile;

	if((Nodes_infile=std::fopen("NodesSpain.dat","rb")) == NULL){ 
    	std::cerr << "The input binary data file cannot be opened.\n" << std::endl;
		return 1;
	}
	else{
		if (fread(&nodeIndex, sizeof(unsigned long), 1, Nodes_infile)!=1){
			std::cerr << "Impossible to read the size.\n" << std::endl;
			return 1;			
		}
		if((nodes = (node*)malloc(nodeIndex*(sizeof(node))))==NULL){
			std::cerr << "Cannot allocate enough memory.\n" << std::endl;
			return 1;
		}
		else if (fread(nodes, sizeof(node), nodeIndex, Nodes_infile)!=nodeIndex){
			std::cerr << "Impossible to read the nodes.\n" << std::endl;
			free(nodes);
			std::fclose(Nodes_infile);
			return 1;			
		}
		std::fclose(Nodes_infile);
	}

	if((Conns_infile=std::fopen("ConnectionsSpain.dat","rb")) == NULL){ 
    	std::cerr << "The input binary data file cannot be opened.\n" << std::endl;
		return 1;
	}
	else{
		if (fread(&connsCounter, sizeof(unsigned long), 1, Conns_infile)!=1){
			std::cerr << "Impossible to read the size.\n" << std::endl;
			return 1;			
		}
		if((conns = (connection*)malloc(connsCounter*(sizeof(connection))))==NULL){
			std::cerr << "Cannot allocate enough memory.\n" << std::endl;
			return 1;
		}
		else if (fread(conns, sizeof(connection), connsCounter, Conns_infile)!=connsCounter){
			std::cerr << "Impossible to read the nodes.\n" << std::endl;
			free(nodes);
			free(conns);
			std::fclose(Conns_infile);
			return 1;			
		}
		std::fclose(Conns_infile);
	}

	printf("%lu\n", nodeIndex);
	printf("%lu\n", connsCounter);
/*******************************************Reading the files***********************************************/
/****************************************Constructing A* First Conditions***********************************/
	if((astar = (AStarNode*)malloc(nodeIndex*(sizeof(AStarNode))))==NULL){
			std::cerr << "Cannot allocate enough memory.\n" << std::endl;
			free(nodes);
			free(conns);
			return 1;
	}

	for (int counter=0; counter<nodeIndex; counter++){
		(astar+counter)->whq=NONE;
		(astar+counter)->PosInQueue=0;
	}
	if((openListBase = (unsigned long *)malloc(nodeIndex*(sizeof(unsigned long))))==NULL){
			std::cerr << "Cannot allocate enough memory.\n" << std::endl;
			free(nodes);
			free(conns);
			return 1;
	}
	if((closedListBase = (unsigned long *)malloc(nodeIndex*(sizeof(unsigned long))))==NULL){
			std::cerr << "Cannot allocate enough memory.\n" << std::endl;
			free(nodes);
			free(conns);
			free(openListBase);
			return 1;
	}

//	printf("%u\n", binarySearch(nodes, 0, nodeIndex-1,  771979683));
//	printf("%u\n", binarySearch(nodes, 0, nodeIndex-1,  429854583));
/****************************************Constructing A* First Conditions***********************************/
/***********************************************Initializing Queues*****************************************/
    double tm = omp_get_wtime();    
	openList.base=openListBase;
	openList.QSize=0;
	openList.MaxQSize=nodeIndex;
	openList.whq=OPEN;

	closedList.base=closedListBase;
	closedList.QSize=0;
	closedList.MaxQSize=nodeIndex;
	closedList.whq=CLOSED;
/***********************************************Initializing Queues*****************************************/
/***********************************************  A Estrella       *****************************************/
	double goalLat=(nodes+goal)->lat;
	double goalLon=(nodes+goal)->lon;
	//Setting start node in OpenList

	(astar+start)->g=0.0;
	(astar+start)->h=HarvesineD((nodes+start)->lat, (nodes+start)->lon, goalLat, goalLon);

	printf("Heuristic %f\n", (astar+start)->f);
	(astar+start)->f=(astar+start)->h;
	//Push after updating since updates the minimum
	push2Queue(&openList, astar, start);
	
/**********************************************************************************DEBUGGING OF EXTRACTION**********************************************************************************************
	(astar+start+2)->f=(astar+start)->h+1;
	(astar+start+3)->f=(astar+start)->h-1;
	(astar+start+90)->f=(astar+start)->h-10;
	(astar+start+50)->f=(astar+start)->h+8;

	push2Queue(&openList, astar, start+2);
	push2Queue(&openList, astar, start+3);
	push2Queue(&openList, astar, start+90);
	push2Queue(&openList, astar, start+50);
********
	for (int counter=0; counter<openList.QSize; counter++){
		printf("%d, %f, %u, %f, %u, %u\n", counter, openList.MinF, openList.PointsMinF, (astar+*(openList.base+counter))->f,(astar+*(openList.base+counter))->PosInQueue, *(openList.base+counter));
	}


	extract4Open(&openList, astar, 2);	
	for (int counter=0; counter<openList.QSize; counter++){
		printf("%d, %f, %u, %f, %u, %u\n", counter, openList.MinF, openList.PointsMinF, (astar+*(openList.base+counter))->f,(astar+*(openList.base+counter))->PosInQueue, *(openList.base+counter));
	}

	printf("%u\n", ExtractMinimum(&openList));
*******************************************************************************************************************************************************************************************************/	
	//Pushing to OpenList the startNode 

//	printf("Error=%hu\n", extract4Open(&openList, astar, start));

	//Pushing to OpenList the startNode 
//	push2Queue(&openList, astar, start);
	/*****************************************SUPER WHILE****************************************************/
	double current_succesor_cost=DBL_MAX;	
	unsigned long NodeSuccesor=CurrentNode;


	while (!flag_for_break){
		//printf("%u\n", CurrentNode);
		set_Maximum_node_Descendant_N(openList.QSize, &MaxListSize);
		if((CurrentNode==goal) || (openList.QSize==0)){
			flag_for_break=true;
		}
		else{

			CurrentNode=ExtractMinimum(&openList);
			extract4Open(&openList, astar, CurrentNode);
			for(unsigned long nnsuc=nodes[CurrentNode].offset; nnsuc<(nodes[CurrentNode].offset+nodes[CurrentNode].nsucc); nnsuc++){
				
				NodeSuccesor=conns[nnsuc].destination;
				//printf("%u, %u\n", nnsuc, CurrentNode);
				current_succesor_cost=(astar+CurrentNode)->g+PitagorasD((nodes+CurrentNode)->lat, (nodes+CurrentNode)->lon, (nodes+NodeSuccesor)->lat, (nodes+NodeSuccesor)->lon);
				if((astar+NodeSuccesor)->whq==NONE){

					(astar+NodeSuccesor)->h=HarvesineD( (nodes+NodeSuccesor)->lat, (nodes+NodeSuccesor)->lon,  goalLat,  goalLon);
					(astar+NodeSuccesor)->g=current_succesor_cost;
					(astar+NodeSuccesor)->f=(astar+NodeSuccesor)->h+(astar+NodeSuccesor)->g;
					(astar+NodeSuccesor)->parent=CurrentNode;
					push2Queue(&openList, astar, NodeSuccesor);

				}
				else if ((astar+NodeSuccesor)->g>current_succesor_cost){
					
					if((astar+NodeSuccesor)->whq==CLOSED){

						extract4Closed(&closedList, astar , NodeSuccesor);

						(astar+NodeSuccesor)->g=current_succesor_cost;
						(astar+NodeSuccesor)->f=(astar+NodeSuccesor)->h+(astar+NodeSuccesor)->g;
						(astar+NodeSuccesor)->parent=CurrentNode;
										
						push2Queue(&openList, astar, NodeSuccesor);

					}else if ((astar+NodeSuccesor)->whq==OPEN){
						extract4Open(&openList, astar , NodeSuccesor);
						(astar+NodeSuccesor)->g=current_succesor_cost;
						(astar+NodeSuccesor)->f=(astar+NodeSuccesor)->h+(astar+NodeSuccesor)->g;
						(astar+NodeSuccesor)->parent=CurrentNode;
						push2Queue(&openList, astar, NodeSuccesor);
					}
				}
			}
			push2Queue(&closedList, astar, CurrentNode);
		}
	}
	printf("%f", omp_get_wtime() - tm);
	if(CurrentNode!=goal){
		printf("No Solutions");
	}
	else{
		printf("%lu\n", MaxListSize);
		printf("%f\n", astar[astar[goal].parent].g);
		unsigned long traveler=goal;
		unsigned long travelerDad=0;
		double j=0;
		while(traveler!=start){
			travelerDad=astar[traveler].parent;
			j=j+PitagorasD(nodes[traveler].lat, nodes[traveler].lon, nodes[travelerDad].lat, nodes[travelerDad].lon);
			traveler=travelerDad;
		}
		printf("%f\n", j);
	}
	/*****************************************SUPER WHILE****************************************************/
/***********************************************  A Estrella       *****************************************/
	free(astar);
	free(nodes);
	free(conns);
	free(openListBase);
	free(closedListBase);
	return 0;
}
