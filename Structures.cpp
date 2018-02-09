typedef struct {
	unsigned long id;          // Node identification
	char name='a';
	double lat, lon;           // Node position
	unsigned long nsucc=0;      // Number of node successors; i. e. length of successors
	unsigned long offset;
} node;

typedef struct {
	unsigned long source;
	unsigned long destination;
} connection;


typedef char Queue;
enum whichQueue {NONE, OPEN, CLOSED};

typedef struct {
	double g, h, f;
	unsigned long parent; //Vector Position
	unsigned long PosInQueue;
	Queue whq;
} AStarNode;

void set_Maximum_node_Descendant_N(unsigned long comp, unsigned long *counter){
	if(comp> (*counter)){
		(*counter)=comp;					
	}
}
