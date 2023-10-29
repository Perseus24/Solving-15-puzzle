#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


struct Node{
	int state[16];
	struct Node* Parent;
	int moves; //0 for down, 1 for up, 2 for right, 3 for left
};

struct Fringe{
	struct Node* nodeAddress;
	struct Fringe* next;
	int totalSearchCost;
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
int initState[16] = {4,2,3,0,5,1,6,7,8,9,10,11,12,13,14,15};
int possibleMoves[4];

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

int validActions(int state[], int prevMove){			//calculates what actions are valid
	
	int loc = 0;
	int i;

	for(i=0; i<16; i++){				//finds the index of the blank space (zero)
		if(state[i] == 0){
			loc = i;
			break;
		}
	}
	
	for(i=0; i<4; i++){
		possibleMoves[i] = 0;
	}

	switch(prevMove){					//removes the inverse of the parent's move to the valid actions of the child's move
								//e.g. UP for the parent's move, DOWN is no longer a valid action for the child
		case 0: possibleMoves[1] = -1;
				break;
		case 1: possibleMoves[0] = -1;
				break;
		case 2: possibleMoves[3] = -1;
				break;
		case 3: possibleMoves[2] = -1;
				break;
	}
					//also removes valid actions for the tiles or indexes that is impossible to move in a specific way
	if((loc-4)< 0){				//such as all the bottom tiles cannot move DOWN
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


struct Node* createNode(int state[], struct Node* parent, int moves){		//create node for the state
			
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
	return tempNode;
	
}
void addFringe(struct Node* state){				//adds the node in front of the head
	
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
		head = ptr->next;
		ptr->next = NULL;
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


int checkClosedList(struct Node* state){		//checks if the state is already in the closed list
	
	if(head == NULL)
		return 0;
	
	struct closedList* ptr = headCl;
	int i, temp;
	
	while(ptr!=NULL){
		temp = 16;
		for(i=0; i<16; i++){			//iterates through the state's index to compare to all the nodes' states
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
	
	for(i=0; i<16; i++){			//checks if the state is already a goal state
		if(state->state[i] != goalState[i]){
			return -1;		//immediately returns -1 if one of the number is on the wrong tile
		}
	}
	return 0;
}

void resetClosedList(){				//removes all the nodes in the closed list
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



int checkDepth(){			//checks what depth the node is in through the closed list
	int temp = 0;
	struct closedList* ptr;
	struct closedList* prev;
	ptr = headCl;
	prev = ptr->next;
	
	while(prev!=NULL){
		while(prev->nodeAddress != ptr->nodeAddress->Parent){			//backtracks the number of edges from the node to the parent
			prev=prev->next;
		}
		temp++;
		ptr = prev;
		prev = ptr->next;
	}
	return temp;
}

void printGoal(){			//copies the goal node and its parents to determine the solution from the closed list
	
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

int DFS(struct Node* state, int limit){
	
	addFringe(state);					//adds the root to the fringe
	int move = 0;
	int i=0,j, index=0, fringeIndex = 0, loc=0;
	struct Node* root = NULL;

	while(head!=NULL){					//iterates until the fringe is empty
		state = head->nodeAddress;				//gets the head of the fringe
		
		if(checkGoal(state) == 0){
			addClosedList(state);
			printGoal();
			resetClosedList();
			return 1;
		}	
		
		removeFromFringe();				//removes the head of the fringe
		
		
		if(checkClosedList(state) == 0){		//checks if the state is already in the closed list			
			addClosedList(state);
			
			if(checkDepth()<limit){			//checks if the current node is in the limit. Otherwise, it would not be expanded
				loc = validActions(state->state, state->moves);
				for(j = 0; j<4; j++){
					if(possibleMoves[j] == 0){
						root = createNode(state->state, state, j);
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
					}
				}
			}
		}	
	}
	
	resetClosedList();		//resets the closed list for every iteration of IDS when goal node is not found
	return 0;
	
	
}
void iterativeDeepeningSearch(){
	struct Node* root;
	int limit = 0;
	int i, j, col=0;
	int counter = 1;

	while(counter){
		if(limit==0){
			root = createNode(initState, NULL, -1);
			checkGoal(root);
			limit++;
		}else{
			root = createNode(initState, NULL, -1);
			while(DFS(root, limit)!=1){		//enters a while loop and increases the limit until the goal node is found
				limit++;
			}
			
			counter=0;
		}
		
	}
	return;
	
}
void main(){

	iterativeDeepeningSearch();
	int i,j;
	int solCost = 0, searchCost = 0;

	struct closedList* ptr;
	ptr = headCl;	
	
	struct solution* ptr1;
	ptr1 = headSol;
	printf("solution HERE!\n");
	
	while(ptr1!=NULL){
		
		system("cls");
		//system("clear"); for linux
		printf("\t\t");
		for(i=0; i<16; i++){
			if((i)%4 == 0 && i!=0)
				printf("\n\t\t");
			printf("%d ", ptr1->nodeAddress->state[i]);
		}
		switch(ptr1->nodeAddress->moves){
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
		ptr1 = ptr1->next;
		sleep(1);  // Sleep for 1 second

	}
	
	printf("Solution cost: %d\n", solCost);
	printf("Search cost: %d\n", totalSearchCost);
	
	
}
