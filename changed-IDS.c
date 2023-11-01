#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


struct Node{
	int state[16];
	struct Node* Parent;
	int moves; //0 for down, 1 for up, 2 for right, 3 for left
	int depth;
	struct Node* next;
};

struct Fringe{
	struct Node* nodeAddress;
	struct Fringe* next;
};

struct closedList{
	struct Node* nodeAddress;
	struct closedList* next;
};

struct solution{
	struct Node* nodeAddress;
	struct solution* next;
	int totalSearchCost;
};

int goalState[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
//{1,0,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
//{4,2,3,0,5,1,6,7,8,9,10,11,12,13,14,15};
//{1,5,2,3,6,0,7,11,4,12,14,10,9,8,13,15};
//5,6,2,3,1,0,10,7,4,13,9,15,8,12,11,14};
//{4,1,2,3,8,5,6,7,0,9,10,11,12,13,14,15};
//{13,8,4,6,14,12,3,0,15,11,5,7,9,10,2,1};
int initState[16] ={5,6,2,3,1,0,10,7,4,13,9,15,8,12,11,14};
int possibleMoves[4];

struct Node* headNode;
struct Fringe* head;
struct closedList* headCl;
struct solution* headSol;

int totalSearchCost;



void swap(int *a, int *b){
	int temp;
	
	temp = *a;
	*a = *b;
	*b = temp;
	
}

int moveUP(struct Node* root, int loc){
	
	swap(&root->state[loc], &root->state[loc-4]);	
}

int moveDOWN(struct Node* root, int loc){
	
	swap(&root->state[loc], &root->state[loc+4]);	
	
}

int moveLEFT(struct Node* root, int loc){

	swap(&root->state[loc], &root->state[loc-1]);

}

int moveRIGHT(struct Node* root, int loc){

	swap(&root->state[loc], &root->state[loc+1]);

}

int validActions(int state[], int prevMove, int possibleMoves[]){
	
	int loc = 0;
	int i;

	for(i=0; i<16; i++){
		if(state[i] == 0){
			loc = i;
			break;
		}
	}
	
	for(i=0; i<4; i++){
		possibleMoves[i] = 0;
	}

	switch(prevMove){
		case 0: possibleMoves[1] = -1;
				break;
		case 1: possibleMoves[0] = -1;
				break;
		case 2: possibleMoves[3] = -1;
				break;
		case 3: possibleMoves[2] = -1;
				break;
	}
	if((loc-4)< 0){
		possibleMoves[1] = -1;
	}
	if((loc+4)>15){
		possibleMoves[0] = -1;
	}
	if((loc%4) == 0){
		possibleMoves[3] = -1;
	}
	if(((loc+1) % 4) == 0){
		possibleMoves[2] = -1;
	}

	return loc;
	
	
}


struct Node* createNode(int state[], struct Node* parent, int moves, int depth){
	
	int i, j;
	struct Node* tempNode = (struct Node*)malloc(sizeof(struct Node));
	
	if(tempNode == NULL){
		exit(1);
	}
	for(i=0; i<16; i++){
		tempNode->state[i] = state[i];
	}
	tempNode->Parent = parent;
	tempNode->moves = moves;
	tempNode->depth = depth;

	return tempNode;
	
}

void addNode(struct Node* state){
		
	if(headNode == NULL){
		headNode = state;	
		state->next = NULL;
	}else{
		state->next = headNode;
		headNode = state;	
	}
}

void addFringe(struct Node* state){
	
	totalSearchCost++;
	
	struct Fringe* ptr = (struct Fringe*)malloc(sizeof(struct Fringe));
	
	if(ptr == NULL){
		exit(1);
	}
	if(head==NULL){
		head = ptr;
		ptr->nodeAddress = state;
		ptr->next = NULL;
	}else{
		ptr->nodeAddress = state;
		ptr->next = head;
		head = ptr;
	}
	
}

void removeFromFringe(){
	struct Fringe* ptr;
	
	ptr = head;

	if(ptr->next == NULL){
		free(ptr);
		head = NULL;
	}else{
		ptr=head;
		head = head->next;
		//ptr->next = NULL;
		free(ptr);
	}
	
	
}
void addClosedList(struct Node* state){
	
	struct closedList* ptr = (struct closedList*)malloc(sizeof(struct closedList));
	
	totalSearchCost++;		//THE GOAL NODE IS ALSO ADDED HERE, -1 IF NECESSARY
	
	
	if(ptr==NULL)
		exit(1);
	if(headCl==NULL){
		headCl = ptr;
		ptr->nodeAddress = state;
		ptr->next = NULL;
	}else{
		ptr->nodeAddress = state;
		ptr->next = headCl;
		headCl = ptr;
	}

}


int checkClosedList(struct Node* state){
	
	if(head == NULL)
		return 0;
	
	struct closedList* ptr = headCl;
	int i, temp;
	
	while(ptr!=NULL){
		temp = 16;
		for(i=0; i<16; i++){
			if(state->state[i] != ptr->nodeAddress->state[i]){
				temp--;
				break;
			}
		}
		if(temp == 16){
			return 1;
		}
		ptr = ptr->next;
	}
	
	return 0;
}

int checkGoal(struct Node* state){
	int i;
	
	for(i=0; i<16; i++){
		if(state->state[i] != goalState[i]){
			return 0;
		}
	}
	return 1;
}

void resetClosedList(){
	struct closedList* ptr;
	
	ptr = headCl;
	
	while(headCl!=NULL){
		if(ptr->next == NULL){
			free(ptr);
			headCl = NULL;
		}else{
			headCl = headCl->next;
			ptr->next = NULL;
			free(ptr);
		}
		ptr = headCl;
	}
}

void resetFringe(){
	struct Fringe* ptr;
	
	ptr = head;
	
	while(head!=NULL){
		if(ptr->next == NULL){
			free(ptr);
			head = NULL;
		}else{
			head = head->next;
			ptr->next = NULL;
			free(ptr);
		}
		ptr = head;
	}
}



int checkDepth(){
	int temp = 0;
	struct closedList* ptr;
	struct closedList* prev;
	ptr = headCl;
	prev = ptr->next;
	
	while(prev!=NULL){
		while(prev->nodeAddress != ptr->nodeAddress->Parent){
			prev=prev->next;
		}
		temp++;
		ptr = prev;
		prev = ptr->next;
	}
	printf("DEPTH %d\n", temp);
	return temp;
}

void printGoal(){
	
	struct closedList* ptr = headCl;
	struct closedList* prev = ptr->next;
	
	struct solution* ptrSol = (struct solution*)malloc(sizeof(struct solution));
	
	if(ptrSol == NULL)
		exit(1);
	
	ptrSol->nodeAddress = ptr->nodeAddress;
	ptrSol->next = NULL;
	headSol = ptrSol;
	
	while(prev!=NULL){
		while(ptr->nodeAddress->Parent != prev->nodeAddress){
			prev = prev->next;
		}
		struct solution* ptrSol = (struct solution*)malloc(sizeof(struct solution));
		
		ptrSol->nodeAddress = prev->nodeAddress;
		ptrSol->next = headSol;
		headSol = ptrSol;
		
		ptr = prev;
		prev = ptr->next;
	}

}

struct Node* DLS(struct Node* state, int limit){
	
	int counter = 0;
	struct Node* root;
	int loc, j;
	int possibleMoves[4] = {0,0,0,0};
	
	
	/*
	struct Node* ptr = headNode;
		while(ptr!=NULL){
			for(j=0; j<16; j++){
				printf("%d->", ptr->state[j]);
			}
			printf("\ndepth is %d", ptr->depth);
			ptr = ptr->next;
			printf("\n\n");
		}
		printf("\n\n");
	*/	
	if(checkGoal(state))
		return state;
		
	else if(state->depth == limit){
		if(state->next == NULL)
			free(state);
		else{
			headNode = headNode->next;
			state->next = NULL;
			free(state);
		}
		return NULL;
	}
	else{
		counter=0;
		loc = validActions(state->state, state->moves, possibleMoves);
		for(j=0; j<4; j++){
			//printf("%d\n", j);
			if(possibleMoves[j] == 0){
				root = createNode(state->state, state, j, (state->depth)+1);
				switch(j){
					case 0: moveDOWN(root, loc);
							break;
					case 1: moveUP(root, loc);
							break;
					case 2: moveRIGHT(root, loc);
							break;
					case 3: moveLEFT(root, loc);
							break;	
				}
				addNode(root);
				struct Node* result = DLS(root, limit);
	            if (result != NULL) {
	    	        return result;
	            }
	            else{
	              	counter=1;
	            }
	                
			}
		}
		if(counter == 1){
				
			if(state->next == NULL){
				free(state);
			}
			else{
				headNode = headNode->next;
				state->next = NULL;
				free(state);
			}
			return NULL;
		}
	}
		
		
}
	

/*
int DLS(struct Node* state, int limit){
		int loc, j, result = 0;
		int cutoff = 0;
		struct Node* root;
		
		
		
		//state = head->nodeAddress;
		
		if(checkGoal(state)){
			return 1;
		}
		else if(state->depth == limit){
			removeFromFringe();
			return 0;
		}
		else{
	  
			loc = validActions(state->state, state->moves);
			for(j=0; j<4; j++){
				 
				if(possibleMoves[j] == 0){
					root = createNode(state->state, state, j, (state->depth)+1);
					switch(j){
						case 0: moveDOWN(root, loc);
								break;
						case 1: moveUP(root, loc);
								break;
						case 2: moveRIGHT(root, loc);
								break;
						case 3: moveLEFT(root, loc);
								break;	
					}
					
					addFringe(root);
					result = DLS(root, limit);
	                if (result == 1) {
	                    return 1;
	                }
	                
				}
			}
			removeFromFringe();
			
			return 0;
		}
		
	}
*/
void iterativeDeepeningSearch(){
	int limit = 0;
	int counter = 1;
	struct Node* result;
	
	while(counter){
		struct Node* root = createNode(initState, NULL, -1, 0);
		addNode(root);
		printf("depth %d: \n\n", limit);
		result = DLS(root, limit);

		if(result!=NULL)
			counter = 0;
		else{
			headNode = NULL;
			limit++;
		}
		
	}
	
}

void manhattanDist(){
	
	
}

void A_star(){
	
	
	
         }


void main(){


	//Below is the implementation of IDS	
	
	iterativeDeepeningSearch();
	int i,j;
	int solCost = 0, searchCost = 0;
	
	
	struct Node* ptr;
	ptr = headNode;
	printf("solution HERE!\n");
	
	while(ptr!=NULL){
		
		//system("cls");
		//system("clear"); for linux
		printf("\t\t");
		for(i=0; i<16; i++){
			if((i)%4 == 0 && i!=0)
				printf("\n\t\t");
			printf("%d ", ptr->state[i]);
		}
		switch(ptr->moves){
			case 0: printf("\nMove down\n\n");
					break;
			case 1: printf("\nMove up\n\n");
					break;
			case 2: printf("\nMove right\n\n");
					break;
			case 3: printf("\nMove left\n\n");
					break;
		}
		printf("\n\n");
		solCost++;
		ptr = ptr->next;
		//sleep(1);  // Sleep for 1 second

	}
	
	printf("Solution cost: %d\n", solCost - 1);			//-1 as it also considers the goal node
	printf("Search cost: %d\n", totalSearchCost);
	
	printf("head is depth %d", headNode->depth);
	
	//here for A*
	
}
