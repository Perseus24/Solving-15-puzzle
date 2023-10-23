#include <stdio.h>
#include <stdlib.h>


struct Node{
	int state[16];
	struct Node* Parent;
	int moves; //0 for down, 1 for up, 2 for right, 3 for left
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
};

int goalState[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
int initState[16] = {2,0,3,1,4,10,9,12,15,13,8,5,7,6,14,11};
int possibleMoves[4];

struct Fringe* head;
struct closedList* headCl;
struct solution* headSol;



void swap(int *a, int *b){
	int temp;
	
	temp = *a;
	*a = *b;
	*b = temp;
	
}

int moveUP(int state[], int loc){
	
	swap(&state[loc], &state[loc-4]);	
}

int moveDOWN(int state[], int loc){
	
	swap(&state[loc], &state[loc+4]);	
	
}

int moveLEFT(int state[], int loc){

	swap(&state[loc], &state[loc-1]);

}

int moveRIGHT(int state[], int loc){

	swap(&state[loc], &state[loc+1]);

}

int validActions(int state[], int prevMove){
	
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
	if(loc-4 < 0){
		possibleMoves[1] = -1;
	}
	if(loc+4>14){
		possibleMoves[0] = -1;
	}
	if(loc%4 == 0){
		possibleMoves[3] = -1;
	}
	if(loc+1 % 4 == 0){
		possibleMoves[2] = -1;
	}

	return loc;
	
	
}


struct Node* createNode(int state[], struct Node* parent, int moves){
	
	int i, j;
	struct Node* tempNode = (struct Node*)malloc(sizeof(struct Node));
	
	for(i=0; i<16; i++){
		tempNode->state[i] = state[i];
	}
	tempNode->Parent = parent;
	tempNode->moves = moves;
	return tempNode;
	
}
void addFringe(struct Node* state){
	
	struct Fringe* ptr = (struct Fringe*)malloc(sizeof(struct Fringe));
	
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
	head = head->next;
	ptr->next = NULL;
	free(ptr);
	}
	
	
}
void addClosedList(struct Node* state){
	
	struct closedList* ptr = (struct closedList*)malloc(sizeof(struct closedList));
	
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
			return -1;
		}
	}
	return 0;
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
	printf("%d ", temp);
	return temp;
}

void printGoal(){
	
	struct closedList* ptr = headCl;
	struct closedList* prev = ptr->next;
	
	struct solution* ptrSol = (struct solution*)malloc(sizeof(struct solution));
	
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
	int i=0,j, index=0, fringeIndex = 0, loc;
	struct Node* root;

	while(head!=NULL){
		state = head->nodeAddress;				//gets the head of the fringe
		for(i=0; i<16; i++){
			printf("%d->", state->state[i]);
		}
		printf("\n");
		removeFromFringe();
		
		if(checkGoal(state) == 0){
			addClosedList(state);
			printGoal();
			return 1;
		}	
		
		if(checkClosedList(state) == 0){		//checks if the state is already in the closed list			
			addClosedList(state);
			if(checkDepth()<limit){
				loc = validActions(state->state, state->moves);
				for(j = 0; j<4; j++){
					if(possibleMoves[j] == 0){
						root = createNode(state->state, state, j);;
						switch(j){
							case 0: moveDOWN(root->state, loc);
									break;
							case 1: moveUP(root->state, loc);
									break;
							case 2: moveRIGHT(root->state, loc);
									break;
							case 3: moveLEFT(root->state, loc);
									break;	
						}
						addFringe(root);
					}
				}
			}
		}
		
	}
	free(root);
	resetClosedList();
	return 0;
	
	
}
void iterativeDeepeningSearch(){
	struct Node* root = createNode(initState, NULL, -1);
	int limit = 0;
	int i, j, col=0;
	int totalNodesExp = 0;
	int counter = 1;

	while(counter){
		if(limit==0){
			createNode(initState, NULL, -1);
			//checkGoal();
			totalNodesExp++;
			limit++;
		}else{
			root = createNode(initState, NULL, -1);
			
			while(DFS(root, limit)!=1){
				limit++;
			}
			counter=0;
			return;
		}
	}
	
}
void main(){
		
	int closedList[3];
	int fringe[100];
	
	iterativeDeepeningSearch();
	int i,j;

	struct closedList* ptr;
	ptr = headCl;	
	printf("CLOSED LIST HERE!\n");
	while(ptr!=NULL){
		for(i=0; i<16; i++){
			printf("%d->", ptr->nodeAddress->state[i]);
		}
		printf("\n\n");
		ptr = ptr->next;
	}
	
	struct solution* ptr1;
	ptr1 = headSol;
	printf("solution HERE!\n");
	while(ptr1!=NULL){
		for(i=0; i<16; i++){
			printf("%d->", ptr1->nodeAddress->state[i]);
		}
		printf("\n\n");
		ptr1 = ptr1->next;
	}
	
	
}
