#define nnodes 24895681
#define nmembers 30
#define connNumber 28394533
//#define nnodes 20

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h> 
#include <cstring>
#include <climits>
#include "Structures.cpp"
#include "BinarySearch.cpp"
#include "QuickSort.cpp"
//#include <omp.h>

//#include "QuickSort.cpp"

unsigned long nodeIndex=0;
node *nodes;
connection *conns;
unsigned long connscounter=0;

int main() {
//	printf("%lu", ULONG_MAX);
	printf("Allocating Memory\n");
	if((nodes = (node*)malloc(nnodes*(sizeof(node))))==NULL){
    	std::cerr << "Cannot allocate enough memory.\n" << std::endl;
		return 1;
	}
	if((conns = (connection *)malloc(connNumber*(sizeof(connection))))==NULL){
    	std::cerr << "Cannot allocate enough memory.\n" << std::endl;
		free(nodes);
		return 1;
	}
	printf("Succesful Allocating Memory\n");

	std::ifstream in("spain.csv");
  	if(!in) {
    	std::cerr << "Cannot open input file.\n" << std::endl;
		free(nodes);
		free(conns);
    	return 1;
  	}

	size_t pos=0;
	unsigned char field_splitter=0;
	std::string node1, node2, node3;

	unsigned long indexForNodes, indexForSuccesors;
	unsigned long binary_search_result, idwaymember;
	unsigned long connections[5306];
	unsigned long connectionsID[5306];
	unsigned long nodemembercounter=0;
	unsigned long  pos1Node, pos2Node; 
	unsigned long  pos1NodeSucc, pos2NodeSucc;

	unsigned long NConnections=0;

	std::string str;
	std::string delimiter = "|";

	std::string token_type;
	std::string token_id;
	std::string token_name;
	std::string token_oneway;

	std::string token_info;
	bool problem_flag=0;
	
	while (std::getline(in, str)) {
		// output the line
		token_type= "";
		//std::cout << str << std::endl;
	/*************************************************************************************************************/
		if((pos=str.find(delimiter)) != std::string::npos){
		/*********************************************************************************************************/
			token_type = str.substr(0, pos);	
			str.erase(0, pos + delimiter.length());	
			
			pos=str.find(delimiter);
			token_id = str.substr(0, pos);
			str.erase(0, pos + delimiter.length());	

			pos=str.find(delimiter);
			token_name = str.substr(0, pos);
			str.erase(0, pos + delimiter.length());	
		/*********************************************************************************************************/
		/*********************************************************************************************************/
			if(token_type.compare("node")==0){

				(*(nodes+nodeIndex)).id=std::stoul(token_id, nullptr, 10);
				//std::strcpy ((*(nodes+nodeIndex)).name, token_name.c_str());

				for (field_splitter=0; field_splitter <6; field_splitter++){
					pos=str.find(delimiter);
					str.erase(0, pos + delimiter.length());
				}
				
				pos=str.find(delimiter);
				token_info = str.substr(0, pos);
				str.erase(0, pos + delimiter.length());
				(*(nodes+nodeIndex)).lat=std::stod(token_info, nullptr);

				pos=str.find(delimiter);
				token_info = str.substr(0, pos);
				str.erase(0, pos + delimiter.length());	
				(*(nodes+nodeIndex)).lon=std::stod(token_info, nullptr);

				nodeIndex++;

			}
	/**********************************************************************************************************************/
			else if (token_type.compare("way")==0){
	/**********************************************************************************************************************/
				for (field_splitter=0; field_splitter < 4; field_splitter++){
					pos=str.find(delimiter);
					str.erase(0, pos + delimiter.length());
				}
	/**********************************************************************************************************************/
				pos=str.find(delimiter);
				token_oneway=str.substr(0, pos);
				str.erase(0, pos + delimiter.length());

				pos=str.find(delimiter);
				str.erase(0, pos + delimiter.length());

				indexForNodes=0;
	/**********************************************************************************************************************/
			/*************************************************************************************************************/
				while((pos=str.find(delimiter))!=std::string::npos){
					token_info=str.substr(0,pos);
					str.erase(0, pos + delimiter.length());					
					
					idwaymember=std::stoul(token_info,nullptr, 10);
					binary_search_result=binarySearch(nodes, 0, nodeIndex-1, idwaymember);

					if (binary_search_result<nodeIndex){
						connectionsID[indexForNodes]=idwaymember;
						connections[indexForNodes]=binary_search_result;
						indexForNodes++;					
					}					
				}
			/*************************************************************************************************************/
			/*************************************************************************************************************/	
				token_info=str;
				idwaymember=std::stoul(token_info, nullptr, 10);
				binary_search_result=binarySearch(nodes, 0, nodeIndex-1, idwaymember);

	
				if (binary_search_result<nodeIndex){
					connectionsID[indexForNodes]=idwaymember;
					connections[indexForNodes]=binary_search_result;
					indexForNodes++;
				}
			/*************************************************************************************************************/

			/**********************************************************************************************************************/
				if(indexForNodes>=2){				
			/*************************************************************************************************************/
					if(token_oneway.compare("oneway")==0){

						for(indexForSuccesors=0; indexForSuccesors<indexForNodes-1; indexForSuccesors++){

							pos1Node=connections[indexForSuccesors];
							pos1NodeSucc=nodes[pos1Node].nsucc;

							nodes[pos1Node].nsucc = nodes[pos1Node].nsucc+1;
							
							(conns+connscounter)->source=connections[indexForSuccesors];
							(conns+connscounter)->destination=connections[indexForSuccesors+1];
							connscounter++;
			/*************************************************************************************************************/
			/*************************************************************************************************************/
							set_Maximum_node_Descendant_N(pos1NodeSucc+1, &nodemembercounter);
			/*************************************************************************************************************/
						}
			/*************************************************************************************************************/
						NConnections=NConnections+(indexForNodes-1);
			/*************************************************************************************************************/
					}
					else {
			/*************************************************************************************************************/
						for(indexForSuccesors=0; indexForSuccesors<indexForNodes-1; indexForSuccesors++){
							pos1Node=connections[indexForSuccesors];
							pos1NodeSucc=nodes[pos1Node].nsucc;
	
							pos2Node=connections[indexForSuccesors+1];
							pos2NodeSucc=nodes[pos2Node].nsucc;

							nodes[pos1Node].nsucc = pos1NodeSucc+1;
							nodes[pos2Node].nsucc = pos2NodeSucc+1;
			/*************************************************************************************************************/
			/*************************************************************************************************************/
							(conns+connscounter)->source=connections[indexForSuccesors];
							(conns+connscounter)->destination=connections[indexForSuccesors+1];
							connscounter++;
							(conns+connscounter)->source=connections[indexForSuccesors+1];
							(conns+connscounter)->destination=connections[indexForSuccesors];
							connscounter++;
			/*************************************************************************************************************/
			/*************************************************************************************************************/
							set_Maximum_node_Descendant_N(nodes[pos1Node].nsucc, &nodemembercounter);
							set_Maximum_node_Descendant_N(nodes[pos2Node].nsucc, &nodemembercounter);
			/*************************************************************************************************************/
			/*************************************************************************************************************/
						}
			/*************************************************************************************************************/
						NConnections=NConnections+2*(indexForNodes-1);
			/*************************************************************************************************************/
					}
				}

			}
			/**********************************************************************************************************************/
			else{
//				std::cout<< 1 << std::endl;
			}
			//std::cout << token_id << std::endl;
		}		 
		// now we loop back and get the next line in 'str'
	}
	in.close();
	/**********************************Ordering Connections*****************************************************************/
	printf("Reaching QuickSort\n");
	printf("Number of descendants %lu\n", nodemembercounter);
	quickSort(conns, 0, NConnections-1);
	printf("QuickSortPassed\n");
	/**********************************Ordering Connections*****************************************************************/
	/**********************************Saving Connections******************************************************************/
	std::FILE *Conns_outfile;
	if((Conns_outfile=std::fopen("ConnectionsSpain.dat","wb")) == NULL){ 
    	std::cerr << "The output binary data file cannot be opened.\n" << std::endl;
		in.close();
 		free(nodes);
		free(conns);
		return 1;
	}
	else{
		if(std::fwrite(&NConnections, sizeof(unsigned long), 1, Conns_outfile)!=1){
			in.close();
	 		free(nodes);
			free(conns);
			std::cerr << "The output binary data file cannot be opened.\n" << std::endl;
			return 1;
		}
		else
		{
			if(std::fwrite(conns, sizeof(connection), NConnections, Conns_outfile)!=NConnections){
				in.close();
		 		free(nodes);
				free(conns);
				std::cerr << "The output binary data file cannot be opened.\n" << std::endl;
				return 1;
			}
		}
		std::fclose(Conns_outfile);
	}
	/**********************************Saving Connections******************************************************************/

	/***********************************SETTING OFFSET**********************************************************************/	
	printf("Stage of Setting Nodes\n");
	unsigned long j1=0, j2=0;
	unsigned long tmpIndex=0;
/*	for(connscounter=0; connscounter<NConnections; connscounter=connscounter+(nodes+(conns+connscounter)->source)->nsucc){
		nodes[conns[connscounter].source].offset=connscounter;
	}
*/
/* 	#pragma omp for private(j1)	
	for(j1=0; j1<nodeIndex; j1++){
		if (nodes[j1].nsucc==0){
			j2++;
		}
	}

	nodes[0].offset=0;*/
/*	for(j1=0; j1<NConnections; j1++){
		if(conns[j1].source != conns[j2].source){
			j2=j1;
			tmpIndex++;
			nodes[tmpIndex].offset=j1;
		}
	}
	printf("Stage Setting Nodes Passed\n");*/
	/***********************************SETTING OFFSET**********************************************************************/	

		/**********************************Saving Connections******************************************************************/
/*	std::FILE *Nodes_outfile;
	if((Nodes_outfile=std::fopen("NodesSpain.dat","wb")) == NULL){ 
    	std::cerr << "The output binary data file cannot be opened.\n" << std::endl;
		in.close();
 		free(nodes);
		free(conns);
		return 1;
	}
	else{
		if(std::fwrite(&nodeIndex, sizeof(unsigned long), 1, Nodes_outfile)!=1){
			in.close();
	 		free(nodes);
			free(conns);
			std::cerr << "The output binary data file cannot be opened.\n" << std::endl;
			return 1;
		}
		else
		{
			if(std::fwrite(nodes, sizeof(node), nodeIndex, Nodes_outfile)!=nodeIndex){
				in.close();
		 		free(nodes);
				free(conns);
				std::cerr << "The output binary data file cannot be opened.\n" << std::endl;
				return 1;
			}
		}
		std::fclose(Nodes_outfile);
	}*/
	/**********************************Saving Connections******************************************************************/




/*

	printf("%u\n", NConnections);
	printf("%u\n", connscounter);
	printf("%u, %hu, %u\n", nodes[binarySearch(nodes, 0, nodeIndex-1, 8670491)].id, nodes[binarySearch(nodes, 0, nodeIndex-1, 8670491)].nsucc, nodes[binarySearch(nodes, 0, nodeIndex-1, 8670491)].offset);
	printf("%u, %u\n", (conns)->source, (conns)->destination);
	printf("%u, %u\n", (conns+1)->source, (conns+1)->destination);
	printf("%u, %hu, %u\n", nodes[binarySearch(nodes, 0, nodeIndex-1, 8670492)].id, nodes[binarySearch(nodes, 0, nodeIndex-1, 8670492)].nsucc, nodes[binarySearch(nodes, 0, nodeIndex-1, 8670492)].offset);
	printf("%u, %u\n", (conns+2)->source, (conns+2)->destination);
	printf("%u, %u\n", (conns+3)->source, (conns+3)->destination);
	printf("%u, %u\n", (conns+4)->source, (conns+4)->destination);
	printf("%u, %hu, %u\n", nodes[binarySearch(nodes, 0, nodeIndex-1, 8670493)].id, nodes[binarySearch(nodes, 0, nodeIndex-1, 8670493)].nsucc, nodes[binarySearch(nodes, 0, nodeIndex-1, 8670493)].offset);
	printf("%u, %u\n", (conns+5)->source, (conns+5)->destination);
	printf("%u, %u\n", (conns+6)->source, (conns+6)->destination);
	printf("%u, %u\n", (conns+7)->source, (conns+7)->destination);
*/

/*
	printf("%lu, %hu\n", nodes[binarySearch(nodes, 0, nodeIndex-1, 8670491)].id, nodes[binarySearch(nodes, 0, nodeIndex-1, 8670491)].nsucc);
	printf("%hu\n", nodemembercounter);

	printf("%lu, %hu\n", nodes[binarySearch(nodes, 0, nodeIndex-1, 8774490)].id, nodes[binarySearch(nodes, 0, nodeIndex-1, 8774490)].nsucc);
	printf("%hu\n", nodemembercounter);
	printf("%lu, %hu\n", nodes[binarySearch(nodes, 0, nodeIndex-1, 8774494)].id, nodes[binarySearch(nodes, 0, nodeIndex-1, 8774494)].nsucc); 	
*/
 	free(nodes);
	free(conns);
  	return 0;
}
