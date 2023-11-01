#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


struct Node{
	int state[16];
	struct Node* Parent;
	int moves; //0 for down, 1 for up, 2 for right, 3 for left
	int depth;
	int fn;
	int gn;
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
int initState[16] ={5,6,2,3,1,0,10,7,4,13,9,15,8,12,11,14};

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


struct Node* createNode(int state[], struct Node* parent, int moves, int depth, int fn, int gn){
	
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
	tempNode->fn = fn;
	tempNode->gn = gn;

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
/*
struct Node* DLS(struct Node* state, int limit){
	
	int counter = 0;
	struct Node* root;
	int loc, j;
	int possibleMoves[4] = {0,0,0,0};
	
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
		loc = validActions(state->state, state->moves, &possibleMoves);
		for(j=0; j<4; j++){
			//printf("%d\n", j);
			if(possibleMoves[j] == 0){
				printf("%d\n", j);
				root = createNode(state->state, state, j, state->depth+1, 0, (state->depth)+1);
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
/*
void iterativeDeepeningSearch(){
	int limit = 0;
	int counter = 1;
	struct Node* result;
	
	while(counter){
		struct Node* root = createNode(initState, NULL, -1, 0, 0, 0);
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
*/

int manhattanDist(int state[]){
	
	int i,j, temp = 0, counter = 0;
	int manhattanScore = 0;
	int goalI, goalJ;
	int tempArr[4][4];
	int goalArr[4][4];
	
	for(i=0; i<4; i++){
		for(j=0; j<4; j++){
			goalArr[i][j] = temp;
			tempArr[i][j] = state[temp];
			temp++;
		}
	}
	
	
	for(temp = 0; temp<16; temp++){
		counter = 0;
		for(i=0; i<4; i++){
			for(j=0; j<4; j++){
				if(tempArr[i][j] == temp && temp == goalArr[i][j]){			//number is in the correct order
					//printf("temp is %d\n", temp);
					counter = 1;
					break;
				}else if(tempArr[i][j] == temp){
					goalI = (temp)/4;
					goalJ = temp%4;
					//printf("i is %d and j is %d and temp is %d\n", goalI, goalJ, temp);
					manhattanScore = manhattanScore + abs(goalI - i) + abs(goalJ - j);
					//printf("%d\n", manhattanScore);
					counter = 1;
					break;
					
				}
			}
			if(counter)
				break;
		}
	}
	
	return manhattanScore;
	
}

void sortOpen(){
	struct Fringe* prev_i;
	struct Fringe* ptr;
	struct Fringe* i;
	struct Fringe* j;
	int swap = 1;
	
	i = head;
	prev_i = i;
	j = i->next; 
	
	if(head->next == NULL)
		return;
		
	else{
		while(swap){
			swap = 0;
			i = head;
			prev_i = head;
			j = i->next; 
			
			while(j!=NULL){
				if(i->nodeAddress->fn > j->nodeAddress->fn){
					i->next = j->next;
					j->next = i;
					swap = 1;
					if(i!=head){
						prev_i->next = j;
						j = i->next;
					}
					else{
						head = j;
						prev_i = j;
						j = i->next;
					}
				}else{
					prev_i = i;
					i=j;
					j=i->next;
				}
			}
			
			
		}
		
		
	}
	
}

void addLists(struct Node* state, int change){
	
	struct closedList* ptr = (struct closedList*)malloc(sizeof(struct closedList));
	struct Fringe* ptr1 = (struct Fringe*)malloc(sizeof(struct Fringe));
	
	switch(change){
		case 0: 
				if(headCl==NULL){
					headCl = ptr;
					ptr->nodeAddress = state;
					ptr->next = NULL;
				}else{
					ptr->nodeAddress = state;
					ptr->next = headCl;
					headCl = ptr;
				}
				break;
		case 1: 
				if(head==NULL){
					head = ptr1;
					ptr1->nodeAddress = state;
					ptr1->next = NULL;
				}else{
					ptr1->nodeAddress = state;
					ptr1->next = head;
					head = ptr1;
				}
				break;
	}

}

void removeLists(struct Node* state, int change) {
    switch (change) {
        case 0: {
            struct closedList* ptr = headCl;
            struct closedList* prev = NULL;
            
            if (ptr == NULL) {
                return; // Nothing to remove.
            }

            // If the head is the target state.
            if (ptr->nodeAddress == state) {
                headCl = ptr->next;
                free(ptr);
                return;
            }

            while (ptr != NULL) {
                if (ptr->nodeAddress == state) {
                    prev->next = ptr->next;
                    free(ptr);
                    return;
                }
                prev = ptr;
                ptr = ptr->next;
            }
            break;
        }
        case 1: {
            struct Fringe* ptr1 = head;
            struct Fringe* prev1 = NULL;
            
            if (ptr1 == NULL) {
                return; // Nothing to remove.
            }

            // If the head is the target state.
            if (ptr1->nodeAddress == state) {
                head = ptr1->next;
                free(ptr1);
                return;
            }

            while (ptr1 != NULL) {
                if (ptr1->nodeAddress == state) {
                    prev1->next = ptr1->next;
                    free(ptr1);
                    return;
                }
                prev1 = ptr1;
                ptr1 = ptr1->next;
            }
            break;
        }
    }
}

struct Node* checkLists(struct Node* state, int change){
	
	struct closedList* ptr = headCl;
	struct Fringe* ptr1 = head;
	
	int i, temp = 1;
	switch(change){
		case 0: if(ptr == NULL)
					return NULL;
		
				while(ptr!=NULL){
					for(i=0; i<16; i++){
						if(ptr->nodeAddress->state[i] != state->state[i]){
							temp = 0;
							break;
						}
					}
					if(temp==1){
						return ptr->nodeAddress;
					}
					ptr = ptr->next;
				}
				return NULL;
				break;
				
		case 1: if(ptr1 == NULL)
					return NULL;
		
				while(ptr1!=NULL){
					for(i=0; i<16; i++){
						if(ptr1->nodeAddress->state[i] != state->state[i]){
							temp = 0;
							break;
						}
					}
					if(temp==1){
						return ptr1->nodeAddress;
					}
					ptr1 = ptr1->next;
				}
				return NULL;
				break;
	}
	
	
}


struct Node* getLowestFn() {
    if (head == NULL) {
        return NULL; // Return NULL if the list is empty.
    }

    struct Fringe* ptr = head;
    int temp = head->nodeAddress->fn;
    struct Node* state = head->nodeAddress;

    if (head->next == NULL) {
        return state;
    }

    ptr = ptr->next;
    while (ptr != NULL) {
        if (ptr->nodeAddress->fn < temp) {
            state = ptr->nodeAddress;
            temp = ptr->nodeAddress->fn;
        }
        ptr = ptr->next;
    }

	return state;
}

int A_star(){
	
	struct Node* state = createNode(initState, NULL, -1, 0, 0, 0);
	state->fn =  manhattanDist(state->state);
	addLists(state,1);
	//addFringe(state);
	
	//struct Node* checkF = NULL;
	//struct Node* checkC = NULL;
	
	
	int possibleMoves[4] = {0,0,0,0};
	int loc, j;
	struct Node* root;
	
	
	while(1){
		
		
		//printf("%d\n", head->nodeAddress->depth);
		if(head == NULL)
			exit(1);
		
		//sortOpen();
		state = getLowestFn();				//gets the head of the fringe	
		for(j=0; j<16; j++){
			printf("%d->", state->state[j]);
		}printf("\n\n=>\n");
		//removeLists(state, 1);
		removeLists(state, 1);
		//removeFromFringe();
		//addFringe(state);
		//removeFromFringe();
		addLists(state, 0);			//adds to the closed list
		
		if(checkGoal(state)){
			printf("hi");
			addLists(state, 0);
			printGoal();
			resetClosedList();
			//exit(1);
			return 1;
		}
		
		loc = validActions(state->state, state->moves, possibleMoves);
		for(j = 0; j<4; j++){
			if(possibleMoves[j] == 0){ 
				root = createNode(state->state, state, j, 0, 0, state->gn+1);
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
				root->fn =  root->gn + manhattanDist(root->state);
				struct Node* checkC = checkLists(root, 0);
				if(checkC!=NULL){
					if(root->fn < checkC->fn){
						removeLists(checkC, 0);
						addLists(checkC, 1);
						addLists(root, 0);
						//addClosed(root);
					}		
				}else{
					//addLists(root, 0);
					
					struct Node* checkF = checkLists(root, 1);
					if(checkF!=NULL){
						if(root->fn < checkF->fn){
							removeLists(checkF, 1);
							//addFringe(root);
						}
					}
					addLists(root, 1);
				}
			}
		}
		struct Fringe* ptr = head;
		while(ptr!=NULL){
			for(j=0; j<16; j++){
				printf("%d->", ptr->nodeAddress->state[j]);
			}printf("(%d)\n\n", ptr->nodeAddress->fn);
			ptr = ptr->next;	
		}
		
		printf("\n\n\n");
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
				root = createNode(state->state, state, j, (state->depth)+1, 0, 0);
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

void iterativeDeepeningSearch(){
	int limit = 0;
	int counter = 1;
	struct Node* result;
	
	while(counter){
		struct Node* root = createNode(initState, NULL, -1, 0, 0, 0);
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



void main(){


	//Below is the implementation of IDS	
	
	iterativeDeepeningSearch();
	//A_star();
	
	int i, solCost = 0;
	
	struct solution* ptr1;
	ptr1 = headSol;
	printf("solution HERE!\n");
	
	while(ptr1!=NULL){
		
		//system("cls");
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
