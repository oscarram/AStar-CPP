#define nnodes 23895681
#define connNumber 25394533

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
		}	
	}

	std::FILE *Nodes_outfile;
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
	}

	printf("%lu\n", nodeIndex);
 	free(nodes);
	free(conns);
  	return 0;
}
