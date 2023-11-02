#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

/*											Artificial Intelligence Project #1

					The program solves the 15-puzzle using Iterative Deepening Search and A* algorithm with Manhattan Distance
					
					Programmed by:
					Bron, Greg Enrico
					Cleofe, Erjay
					Deduque, Lean Jedfrey
					Herrera, Cy Jay
					Tabug, Ignacio
					
*/

struct Node{
	int state[16];
	struct Node* Parent;
	int moves; //0 for down, 1 for up, 2 for right, 3 for left
	int depth;
	int fn; //f(n) = g(n) + manhattan distance
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
// EASY {4,2,3,0,5,1,6,7,8,9,10,11,12,13,14,15};
// MEDIUM {5,6,2,3,1,0,10,7,4,13,9,15,8,12,11,14};
// HARD {1,5,2,3,6,0,7,11,4,12,14,10,9,8,13,15};
// WORST {13,8,4,6,14,12,3,0,15,11,5,7,9,10,2,1};
//{4,1,2,3,8,5,6,7,0,9,10,11,12,13,14,15};
int initState[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
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

	for(i=0; i<16; i++){ 				//finds the location of the blank tile (zero)
		if(state[i] == 0){
			loc = i;
			break;
		}
	}
	
                                    //the possibleMoves array would contain 2 values:
									// 0 if the action is valid, -1 if otherwise

	for(i=0; i<4; i++){
		possibleMoves[i] = 0;
	}

	switch(prevMove){               //disregards the inverse of the parent's move to the child's valid actions
									//e.g. if the parent's move was move UP, the child will not move DOWN
		case 0: possibleMoves[1] = -1;
				break;
		case 1: possibleMoves[0] = -1;
				break;
		case 2: possibleMoves[3] = -1;
				break;
		case 3: possibleMoves[2] = -1;
				break;
	}                                 //mathematically calculates the impossible move depending on the location
	if((loc-4)< 0){                   // of the blank tile
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
	
										//creation of the node and its corresponding details

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
	
	                        //specifically exclusive to the IDS
	totalSearchCost++;		//increments the total nodes expanded every time the IDS call the function

	if(headNode == NULL){
		headNode = state;	
		state->next = NULL;
	}else{
		state->next = headNode;
		headNode = state;	
	}
}

int checkGoal(struct Node* state){
							//checks if the state is the goal state
	int i; 					
	
	for(i=0; i<16; i++){
		if(state->state[i] != goalState[i]){
			return 0;
		}
	}
	return 1;
}

void resetClosedList(){
	                                //The function deletes all the nodes in the closed lists
	struct closedList* ptr;			// once the search is completed
	
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

int manhattanDist(int state[]){
											//calculates the manhattan distance of the given state
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
					counter = 1;
					break;
				}else if(tempArr[i][j] == temp){
					goalI = (temp)/4;						//mathematically gives the required moves using the row and column number
					goalJ = temp%4;
					manhattanScore = manhattanScore + abs(goalI - i) + abs(goalJ - j);
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


void addLists(struct Node* state, int change){
   												 //adds nodes to the fringe or to the closed list
	
	struct closedList* ptr = (struct closedList*)malloc(sizeof(struct closedList));
	struct Fringe* ptr1 = (struct Fringe*)malloc(sizeof(struct Fringe));
	
	switch(change){
		case 0: 
				totalSearchCost++;					//increments the total nodes expanded once it's added to the closed list
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
    //removes node from the fringe or the closed list
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
    //check the fringe or the closed list if the node already exist
	
	struct closedList* ptr = headCl;
	struct Fringe* ptr1 = head;
	
	int i, temp = 1;
	switch(change){
		case 0: if(ptr == NULL)
					return NULL;
		
				while(ptr!=NULL){ 				//iterates through all the nodes and comparing their states from the state to be checked
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
    								//gets the node that has the lowest f(n) value in the fringe
    if (head == NULL) {
        return NULL; 				// Return NULL if the list is empty.
    }

    struct Fringe* ptr = head;
    int temp = head->nodeAddress->fn;     		//initializes the first node's f(n) to be the smallest f(n)
    struct Node* state = head->nodeAddress;

    if (head->next == NULL) {
        return state;
    }

    ptr = ptr->next;
    while (ptr != NULL) {                   //iterates through the fringe to check each node's f(n)
        if (ptr->nodeAddress->fn < temp) {  // if the node's f(n) is less than the temp variable, the node's f(n)
            state = ptr->nodeAddress;       // will be the next temp value
            temp = ptr->nodeAddress->fn;
        }
        ptr = ptr->next;
    }

	return state;                           //returns the state with the lowest f(n)
}

void Astar_Solution(){
	
							//gets the solution of the A star algorithm from the closed list
							
	struct closedList* ptr = headCl;
	struct closedList* prev = ptr->next;
	
	struct solution* ptrSol = (struct solution*)malloc(sizeof(struct solution));
	
	if(ptrSol == NULL)
		exit(1);
	
	ptrSol->nodeAddress = ptr->nodeAddress;
	ptrSol->next = NULL;
	headSol = ptrSol;
	
	while(prev!=NULL){
		while(ptr->nodeAddress->Parent != prev->nodeAddress){		//tracks the nodes from the goal node to its parents
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

int A_star(){
   					 //implementation of the standard A star algorithm
	
	struct Node* state = createNode(initState, NULL, -1, 0, 0, 0); 		//creates and adds initial state to the fringe
	state->fn =  manhattanDist(state->state);
	addLists(state,1);
	
	int possibleMoves[4] = {0,0,0,0};
	int loc, j;
	struct Node* root;
	
	
	while(1){
		if(head == NULL) //if the fringe is empty
			exit(1);
		
		state = getLowestFn();				//gets the head of the fringe	
		removeLists(state, 1);				//removes the state from the fringe
		addLists(state, 0);					//adds it to the closed list
		
		if(checkGoal(state)){				//checks if the state is the goal state
			return 1;
		}
		
		loc = validActions(state->state, state->moves, possibleMoves); //calls the function to modify the possibleMoves array
        															   // and gets the location of the blank tile
		for(j = 0; j<4; j++){
			if(possibleMoves[j] == 0){ 											//if the action is valid, create a node for the child 
				root = createNode(state->state, state, j, 0, 0, state->gn+1);	//creates the node for the child, with the g(n) be increased by 1
				switch(j){
					case 0: moveDOWN(root, loc);
							break;
					case 1: moveUP(root, loc);                                  //each of these updates the states by moving the blank tile
							break;                                              //depending on the move
					case 2: moveRIGHT(root, loc);
							break;
					case 3: moveLEFT(root, loc);
							break;	
				}
				root->fn =  root->gn + manhattanDist(root->state);              //updates the f(n) of the child by adding g(n) and the state's 
                                                                                //manhattan distance
				struct Node* checkC = checkLists(root, 0);                      
				if(checkC!=NULL){
					if(root->fn < checkC->fn){                                  //if it exists, and the child's f(n) is less than the existing node's f(n)
						removeLists(checkC, 0);                                 //remove the existing node from the closed list
						addLists(checkC, 1);                                    //and add it back to the open list
					}		
				}else{
					
					struct Node* checkF = checkLists(root, 1);                 //else, if the child node is already in the fringe
					if(checkF!=NULL){
						if(root->fn < checkF->fn){                             //if it exists, and the child's f(n) is less than the existing node's f(n)
							removeLists(checkF, 1);                            //remove the existing node from the fringe
						}
					}
					addLists(root, 1);                                         //else, add the child node to the fringe
				}
			}
		}
	}		
}

int DLS(struct Node* state, int limit){
			
									//implementation of the recursive Depth Limited Search accompanying the IDS
	int counter = 0;
	struct Node* root;
	int loc, j;
	int possibleMoves[4] = {0,0,0,0};
	int result;

	if(checkGoal(state))			//if the state is already the goal node, return 1
		return 1;
		
	else if(state->depth == limit){			//else if the state's depth is already equal to the limit
		if(state->next == NULL)				//remove the state and return 0
			free(state);
		else{
			headNode = headNode->next;
			state->next = NULL;
			free(state);
		}
		return 0;
	}
	else{
		counter=0;
		loc = validActions(state->state, state->moves, possibleMoves);
		for(j=0; j<4; j++){
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
				addNode(root);								//adds the child node to the Node list. 
															//Through recursion, the Node list will only contain nodes
															//in the solution path
				result = DLS(root, limit);					//When the recursive call returns 1, it means that the algorithm finds the goal node
	            if (result) {								
	    	        return 1;
	            }											//otherwise, it will continue expanding the other child nodes
	                
			}
		}
				
		if(state->next == NULL){							//if the parent node exhausts all possible moves and it does not produce
			free(state);									//any goal node, the parent state will be deleted
		}
		else{
			headNode = headNode->next;
			state->next = NULL;
			free(state);
		}
		return 0;
	}
		
}

void iterativeDeepeningSearch(){
			
								//implements the Iterative Deepening Search
	int limit = 0;
	int counter = 1;
	int result;
	
	while(counter){					//depth limit will start by 0
	
		struct Node* root = createNode(initState, NULL, -1, 0, 0, 0);
		addNode(root);
		result = DLS(root, limit);		//calls the DLS
		if(result)					
			counter = 0;		
		else{							//if the DLS does not find a goal, the limit will increase by one
			headNode = NULL;
			limit++;
		}
	}
}

void IDSsolution(){
	
										//copies the solution path from the IDS and copies to the solution list
	struct Node* temp = headNode;
	
	
	while(temp!=NULL){
		struct solution* ptr = (struct solution*)malloc(sizeof(struct solution));
		if(headSol==NULL){
			headSol = ptr;
			ptr->nodeAddress = temp;
			ptr->next = NULL;
		}else{
			ptr->nodeAddress = temp;
			ptr->next = headSol;
			headSol = ptr;
		}
		temp = temp->next;
	}
	
	
	
}

void printSolution(){
    											// Print the solution path and other needed details
	struct solution* ptr1 = headSol;
   	int i, solCost = 0;
	
	printf("\n\tSolution path: ");
    while (ptr1 != NULL) {						//iterates through the solution list
		
        switch (ptr1->nodeAddress->moves){
            case 0: printf("Down -> ");
                break;
            case 1: printf("Up -> ");
                break;
            case 2: printf("Right -> ");
                break;
            case 3: printf("Left -> ");
                break;
        }
        
        if((solCost+1)%7 == 0)
        	printf("\n\t\t\t");

		solCost++;								//calculates the total nodes from initial to goal state
        ptr1 = ptr1->next;
    }
    printf(" GOAL!\n");
    
	printf("\n\tSolution cost: %d\n", solCost - 1);			//-1 as the cost also considers the goal Node
	printf("\tSearch cost: %d\n", totalSearchCost - 1);	//-1 as the cost also considers the goal Node
}

void resetSolution(){
										//the function removes all the node from the solution of IDS
										//so that it can be used again by the A*
	struct solution* ptr = headSol;
	
	while(headSol!=NULL){
		if(ptr->next == NULL){
			free(ptr);
			headSol = NULL;
		}else{
			headSol = headSol->next;
			ptr->next = NULL;
			free(ptr);
		}
		ptr = headSol;
	}

}

void addInitState(int state[]){
	int i;

	for(i = 0; i<16; i++){
		initState[i] = state[i];
	}
}

void printAnimation(){
								//prints a sample animation movement from initial state to goal state
	int i;
	struct solution* ptr1 = headSol;
		while(ptr1!=NULL){

		system("cls");			//for windows
		//system("clear");		//for linux
		printf("\n\n\t\t");
		printf(" -------------------------------\n\t\t");
		for(i=0; i<16; i++){
			if((i)%4 == 0 && i!=0)
				printf(" \n\t\t -------------------------------\n\t\t");
			if(ptr1->nodeAddress->state[i]==0)
				printf(" | __ |");
			else
				printf(" | %2d | ", ptr1->nodeAddress->state[i]);
		}
		printf("\n\t\t -------------------------------\n\t\t");

		printf("\n\n");
		ptr1 = ptr1->next;
		sleep(1);  // Sleep for 1 second

	}
}

void main() {
    clock_t start, end;
    double cpu_time_used;
    int choice, i;
    char yn;

    while (choice != 6) {
        printf("\n\n\n\t\t--------------------------------------------------------------------");
        printf("\n\t\t ||Welcome to 15-puzzle solver using Iterative Deepening Search ||\n\t\t");
        printf(" ||\tand A star Algorithm with Manhattan distance heuristic  ||");
        printf("\n\t\t--------------------------------------------------------------------\n\n");

        printf("\n\n\tCHOOSE WHAT TO DO:");
        printf("\n\n\t\t[1] SOLVE EASY STATE"
            "\n\n\t\t[2] SOLVE MEDIUM STATE"
            "\n\n\t\t[3] SOLVE HARD STATE"
            "\n\n\t\t[4] SOLVE WORST STATE"
            "\n\n\t\t[5] YOUR PREFERRED INITIAL CONFIGURATION"
            "\n\n\t\t[6] EXIT THE PROGRAM");

        printf("\n\n\t\tENTER YOUR CHOICE: ");
        scanf("%d", &choice);

        if (choice == 1) {
            int state[16] = {4, 2, 3, 0, 5, 1, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
            addInitState(state);
        } else if (choice == 2) {
            int state[16] = {5, 6, 2, 3, 1, 0, 10, 7, 4, 13, 9, 15, 8, 12, 11, 14};
            addInitState(state);
        } else if (choice == 3) {
            int state[16] = {1, 5, 2, 3, 6, 0, 7, 11, 4, 12, 14, 10, 9, 8, 13, 15};
            addInitState(state);
        } else if (choice == 4) {
            int state[16] = {13, 8, 4, 6, 14, 12, 3, 0, 15, 11, 5, 7, 9, 10, 2, 1};
            addInitState(state);
        } else if (choice == 5) {
            int state[16];
            printf("\n\tInput your preferred configuration for the 15-puzzle separated by a comma\n\n\t[e.g. 1,2,3,4,0,6,7,5,8,9,10,11,12,13,14,15]: ");
            for (i = 0; i < 16; i++) {
                scanf(" %d,", &state[i]);
            }
            addInitState(state);
        } else if (choice == 6) {
            exit(0);
        } else {
            printf("\n\tINVALID CHOICE!\n\n");
            continue;
        }

        start = clock();
        iterativeDeepeningSearch();					//calls the iterative deepening search
        end = clock();
        IDSsolution();

        printf("\n\tThe algorithm has finished solving!\n\tWould you like to see the animation"
               "from the initial state to the goal state? [Y/n]: ");
        scanf(" %c", &yn);
        if (yn == 'Y' || yn == 'y')
            printAnimation();

        struct solution* ptr1 = headSol;
        
        printf("\n\n\t\t--------------------------------------------");
        printf("\n\t\t || ITERATIVE DEEPENING SEARCH ALGORITHM ||");
        printf("\n\t\t--------------------------------------------");
        printSolution();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("\tRunning Time: \t%.10lf seconds", cpu_time_used);

        totalSearchCost = 0;							//resets the total nodes expanded to 0 for the next algorithm
        resetSolution();								//resets the solution list for te next algoritm

        start = clock();
        A_star();
        end = clock();
        Astar_Solution();

        printf("\n\n\n\tThe algorithm has finished solving!\n\tWould you like to see the animation "
               "from the initial state to the goal state? [Y/n]: ");
        scanf(" %c", &yn);
        if (yn == 'Y' || yn == 'y')
            printAnimation();

        printf("\n\n\t\t------------------------------");
		printf("\n\t\t || A STAR SEARCH ALGORITHM ||");
		printf("\n\t\t-------------------------------");
        printSolution();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("\tRunning Time: \t%.10lf seconds", cpu_time_used);
    }
}
