
/*COP3502C:   Assignment 2*/
/*This program is written by: Braedon Watkins*/

/*================================================================================================*/



#define _CRT_SECURE_NO_WARNINGS
#define GROUNDSIZE 10
#include <stdio.h>
#include <stdlib.h>
#include "leak_detector_c.h"

typedef struct soldier
{
	int data;
	struct soldier* next;
	struct soldier* previous;
}soldier;

typedef struct q
{
	soldier* front;
	soldier* back;

	int n, k, th, groupNum;
	char* name;
}queue;



/*------------------------------------------------------------------------------------------------*/

//Check if empty, return 1 if empty, return 0 if not
int isEmpty(queue* myQueue)
{
	//Empty if front and back are NULL, which makes && statement true or 1, otherwise false or 0
	return(myQueue->front == NULL && myQueue->back == NULL);
}

//If not empty print the data in a queue
void display(queue* myQueue)
{
	soldier* traverse;
	if (isEmpty(myQueue) == 1)
	{
		printf("Invalid Queue for Printing\n");
	}
	else
	{
		printf("Queue values are as follows:\n");
		
		//print first node at front then move to the one directly after
		traverse = myQueue->front;
		printf("%d ", traverse->data);
		traverse = traverse->next;

		//iterate through queue nodes printing the data at each then moving to next node until it reaches the node at front
		while (traverse != myQueue->front)
		{
			printf("%d ", traverse->data);
			traverse = traverse->next;
		}
		printf("\n");
	}
}

//Initializes queue pointers to NULL
void init(queue* myQueue)
{
	myQueue->front = NULL;
	myQueue->back = NULL;
}

//Dequeues for phase1, seperated because it needs to print differently to file
void phase1DeQueue(queue* myQueue, soldier* execute, FILE* output)
{
	soldier* temp;
	soldier* traverse;

	//Decrement n because dequeuing causes number of soldiers to go down by one
	myQueue->n--;

	//Dequeue depending if node to be executed is only node left, at front, at back, or needs to be searched for
	if (myQueue->front == execute && myQueue->back == execute)
	{
		//Only node remaining so free it and set front and back pointers to NULL so it will register as empty
		fprintf(output, "Soldier# %d executed\n", myQueue->front->data);
		free(myQueue->front);
		myQueue->front = NULL;
		myQueue->back = NULL;
	}
	else if (myQueue->front == execute)
	{

		//Soldier at front. Point next and previous pointers such that the list is not lost
		temp = myQueue->front;
		myQueue->back->next = myQueue->front->next;
		myQueue->front = myQueue->front->next;
		myQueue->front->previous = myQueue->back;

		//Free soldier
		fprintf(output, "Soldier# %d executed\n", temp->data);
		free(temp);
	}
	else if (myQueue->back == execute)
	{		
		//Soldier at back. Point next and previous pointers such that the list is not lost
		temp = myQueue->back;
		myQueue->front->previous = myQueue->back->previous;
		myQueue->back = myQueue->back->previous;
		myQueue->back->next = myQueue->front;

		//Free soldier
		fprintf(output, "Soldier# %d executed\n", temp->data);
		free(temp);
	}
	else
	{
		//IF HERE NEED TO SEARCH FOR NODE TO EXECUTE

		//initialize traverse to search the queue
		traverse = myQueue->front->next;

		//While traverse hasn't reached the execute node, go to the next one
		while (traverse != execute)
		{
			traverse = traverse->next;
		}

		//Traverse found execute node and will move pointers such that the list will not be lost
		temp = traverse;
		traverse->previous->next = traverse->next;
		traverse->next->previous = traverse->previous;
		traverse = traverse->next;

		//Free soldier
		fprintf(output, "Soldier# %d executed\n", temp->data);
		free(temp);
	}
}

//Dequeues for phase2, seperated because it needs to print differently to file
void phase2DeQueue(queue* myQueue, soldier* execute, FILE* output)
{
	soldier* temp;
	soldier* traverse;

	//Decrement n because dequeuing causes number of soldiers to go down by one
	myQueue->n--;

	//Dequeue depending if node to be executed is only node left or at the front, no other cases needed
	if (myQueue->front == execute && myQueue->back == execute)
	{
		//Only node remaining so free it and set front and back pointers to NULL so it will register as empty
		fprintf(output, "Executed Soldier %d from line %d\n",
			myQueue->front->data, myQueue->groupNum);
		free(myQueue->front);
		myQueue->front = NULL;
		myQueue->back = NULL;
	}
	else if (myQueue->front == execute)
	{

		//Soldier at front. Point next and previous pointers such that the list is not lost
		temp = myQueue->front;
		myQueue->back->next = myQueue->front->next;
		myQueue->front = myQueue->front->next;
		myQueue->front->previous = myQueue->back;

		//Free soldier
		fprintf(output, "Executed Soldier %d from line %d\n",
			temp->data, myQueue->groupNum);
		free(temp);
	}
}

//Adds new soldier nodes
void enQueue(queue* myQueue, soldier* newSoldier)
{
	//Soldier is placed with next pointing to front node and previous pointing to back
	newSoldier->next = myQueue->front;
	newSoldier->previous = myQueue->back;

	//New Soldier becomes back node. If back was NOT empty then make it point to new soldier before doing so.
	if (myQueue->back != NULL)
	{
		myQueue->back->next = newSoldier;
	}
	myQueue->back = newSoldier;

	//Front's previous pointer points to the new back. If front was empty then make it equal to new soldier before doing so.
	if (myQueue->front == NULL)
	{
		myQueue->front = newSoldier;
	}
	myQueue->front->previous = myQueue->back;

}



//Flips order of nodes in an array to be increasing, not decreasing, order
void rearrange_circle(queue* myQueue)
{
	soldier* traverse;
	soldier* temp;

	//Flip front and back pointers
	temp = myQueue->front;
	myQueue->front = myQueue->back;
	myQueue->back = temp;

	//Flip next and previous pointers for front node
	traverse = myQueue->front;

	temp = traverse->next;
	traverse->next = traverse->previous;
	traverse->previous = temp;

	//Move to the back node
	traverse = traverse->previous;

	//Until traverse returns to front node, iterate through list downawards
	while (traverse != myQueue->front)
	{
		//Flip next and previous pointers for every node
		temp = traverse->next;
		traverse->next = traverse->previous;
		traverse->previous = temp;

		//Iterate downwards
		traverse = traverse->previous;
	}
}

//Create Soldier node
soldier* createSoldier(int sequence)
{
	//Allocate space for soldier node then add in its data
	soldier* newSoldier = malloc(sizeof(soldier));
	newSoldier->data = sequence;

	return newSoldier;
}

//Creates queue before reversing it
void create_reverse_circle(queue* myQueue)
{
	//initialize the queue
	init(myQueue);

	//For the number of soldiers in a given queue, create the soldiers
	for (int i = myQueue->n; i > 0; i--)
	{
		//Call soldier function to allocate space for new soldier
		soldier* newSoldier = createSoldier(i);

		enQueue(myQueue, newSoldier);
	
	}
}

//Execute regularly intervals of units from each queue
void phaseOne(queue** myQueueArray, int numGroups, FILE* output)
{
	for (int i = 0; i < numGroups; i++)
	{
		//Initializing and printing 
		int count = 1;
		soldier* traverse = myQueueArray[i]->front;
		fprintf(output, "Line# %d %s\n", myQueueArray[i]->groupNum, myQueueArray[i]->name);


		//While soldier pop for a given queue is above threshold, continue to iterate
		while (myQueueArray[i]->n > myQueueArray[i]->th)
		{
			//Traverse through the soldiers as many times dictated by k
			if (count == myQueueArray[i]->k)
			{
				//When equal to k, dequeue that node and move to the next
				traverse = traverse->next;
				phase1DeQueue(myQueueArray[i], traverse->previous, output);

				//Reset counter for next iterations
				count = 1;
			}
			else
			{
				//If not at count k just keep traversing and increment count
				traverse = traverse->next;
				count++;
			}
		}

		fprintf(output, "\n");

	}
}

//Given Group info, allocate memory and assign variables
queue* createGroups(char* name, int n, int k, int th, int groupNum)
{
	queue* myQueue = malloc(sizeof(queue));

	myQueue->name = name;
	myQueue->groupNum = groupNum;
	myQueue->th = th;
	myQueue->k = k;
	myQueue->n = n;

	return myQueue;
}

//Take Groups and rearrange them based on order of group number instead of order of reading
queue** OrderQueues(queue** myQueueArray, int numGroups)
{
	//Initializing
	queue** myQueueTemp = malloc(sizeof(queue*) * numGroups);
	queue* testing;
	int counter = 0;

	//For every possible grounds num search for the group with that ground number
	//Stop when number of groups found == numGroups
	while (counter < numGroups)
	{
		for (int i = 0; i < GROUNDSIZE + 1; i++)
		{
			for (int j = 0; j < numGroups; j++)
			{
				//This ensures myQueueTemp is a continuous array in order of group number
				if (myQueueArray[j]->groupNum == i)
				{
					myQueueTemp[counter] = myQueueArray[j];
					counter++;
				}
			}
		}
	}

	//Pass back to original array then free the temp array
	for (int i = 0; i < numGroups; i++)
	{
		myQueueArray[i] = myQueueTemp[i];
	}
	free(myQueueTemp);

	return myQueueArray;
}

//Read in info for groups from the file
queue** readGroups(int numGroups, FILE* input)
{	
	//Initializing
	queue** myQueueArray = malloc(sizeof(queue*) * numGroups);

	//For number of groups take in info
	for (int i = 0; i < numGroups; i++)
	{
		//Initialize and allocate space for char array
		char* tempName = malloc(51);
		int tempN, tempK, tempTH, tempGroupNum;
		
		//Read in info
		fscanf(input, "%d", &tempGroupNum);
		fscanf(input, "%s",	tempName);
		fscanf(input, "%d", &tempN);
		fscanf(input, "%d", &tempK);
		fscanf(input, "%d", &tempTH);

		//Create Groups by passing info to createGroups function
		myQueueArray[i] = createGroups(tempName, tempN, tempK, tempTH, tempGroupNum);
	}

	//Ensure the order of the queues are correct by passing to OrderQueues
	myQueueArray = OrderQueues(myQueueArray, numGroups);


	//Create the queue for each group by calling create_reverse_circle
	for (int i = 0; i < numGroups; i++)
	{
		create_reverse_circle(myQueueArray[i]);
	}

	return myQueueArray;
}

//See what data is at the front of the queue
int peek(queue* myQueue)
{
	return myQueue->front->data;
}

//Execute groups based on largest leading number
void phaseTwo(queue** myQueueArray, int numGroups, FILE* output)
{
	//Initializing + Printing
	int peekCompare;
	int counter = 0;
	fprintf(output, "Phase2 execution\n");

	//Continue executing while number of groups executed is < numGroups
	while (counter < numGroups)
	{
		//Temp number used to find the largest in a set
		int temp = 0;

		//Peek each front node for all groups
		for (int i = 0; i < numGroups; i++)
		{
			//If empty skip it
			if (isEmpty(myQueueArray[i]) == 1)
			{
				continue;
			}

			//Peek first node
			peekCompare = peek(myQueueArray[i]);

			//Compare values of peekcompare to find the largest in a set
			if (temp < peekCompare)
			{
				temp = peekCompare;
			}
		}


		//Take queues with peek value at front and dequeue it entirely, in order of group number
		for (int i = 0; i < numGroups; i++)
		{
			//If empty, skip it
			if (isEmpty(myQueueArray[i]) == 1)
			{
				continue;
			}

			//When a queue front node is the largest, dequeue it entirely
			if (peek(myQueueArray[i]) == temp)
			{
				//For all soldiers in the queue, dequeue it
				for (int j = myQueueArray[i]->n; j > 0; j--)
				{
					phase2DeQueue(myQueueArray[i], myQueueArray[i]->front, output);

					//If we only have one remaining queue, dequeue all but last node
					if (counter == numGroups - 1)
					{
						//If we only have one remaining node, of one remaining queue, declare survivor
						if (myQueueArray[i]->front == myQueueArray[i]->back)
						{
							//Declare survivor
							fprintf(output, "\nSoldier %d from line %d will survive", 
								myQueueArray[i]->front->data, myQueueArray[i]->groupNum);

							//Key to break out of while loop
							counter++;

							//Key to break out of for loop
							break;
							
						}
					}

					//When a queue has no remaining nodes, up counter of destroyed groups
					if (myQueueArray[i]->n == 0)
					{
						counter++;
					}

				}
			}
		}
	}
}

//Print to file the unsorted list
FILE* writeInitialList(queue** myQueueArray, int numGroups, FILE* output)
{
	fprintf(output, "Initial nonempty lists status\n");

	//For number of groups print groupnum, name, and nodes (descending)
	for (int i = 0; i < numGroups; i++)
	{
		//Create a node pointer to traverse through nodes, printing information
		soldier* traverse = myQueueArray[i]->front;

		//Group number and name only print once
		fprintf(output, "%d ", myQueueArray[i]->groupNum);
		fprintf(output, "%s ", myQueueArray[i]->name);

		//For all soldier nodes print their data
		for (int j = 0; j < myQueueArray[i]->n; j++)
		{
			fprintf(output, "%d", traverse->data);
			if (j < myQueueArray[i]->n - 1)
			{
				fprintf(output, " ");
			}
			traverse = traverse->next;
		}

		fprintf(output, "\n");
	}
	fprintf(output, "\n");
	
	return(output);
}

//Print to file the sorted list
FILE* writeReversedList(queue** myQueueArray, int numGroups, FILE* output)
{
	fprintf(output, "After ordering nonempty lists status\n");

	//For number of groups print groupnum, name, and nodes (ascending)
	for (int i = 0; i < numGroups; i++)
	{
		//Create a node pointer to traverse through nodes, printing information
		soldier* traverse = myQueueArray[i]->front;

		//Group number and name only print once
		fprintf(output, "%d ", myQueueArray[i]->groupNum);
		fprintf(output, "%s ", myQueueArray[i]->name);

		//For all soldier nodes print their data
		for (int j = 0; j < myQueueArray[i]->n; j++)
		{
			fprintf(output, "%d", traverse->data);
			if (j < myQueueArray[i]->n - 1)
			{
				fprintf(output, " ");
			}
			traverse = traverse->next;
		}

		fprintf(output, "\n");
	}

	fprintf(output, "\nPhase1 execution\n\n");

	return(output);
}

//Free remaining memory that hasn't been freed
void freeFunction(FILE* input, FILE* output, queue** goodRiddance, int numGroups)
{
	//Close file pointers
	fclose(input);
	fclose(output);

	//Close allocated memory for names, nodes, queue*, and queue** in that order
	soldier* temp;
	for (int i = 0; i < numGroups; i++)
	{
		free(goodRiddance[i]->name);

		//Avoid freeing more than available
		while (isEmpty(goodRiddance[i]) == 0)
		{
			//Determine if last node or not and free accordingly
			if (goodRiddance[i]->front == goodRiddance[i]->back)
			{
				free(goodRiddance[i]->front);
				goodRiddance[i]->front = NULL;
				goodRiddance[i]->back = NULL;

			}
			else
			{
				temp = goodRiddance[i]->front;

				goodRiddance[i]->back->next = goodRiddance[i]->front->next;
				goodRiddance[i]->front = goodRiddance[i]->front->next;
				goodRiddance[i]->front->previous = goodRiddance[i]->back;

				free(temp);
			}
		}
		//free queue*
		free(goodRiddance[i]);
	}
	//free queue**
	free(goodRiddance);
}




/*------------------------------------------------------------------------------------------------*/
void main()
{
	//leakDetector
	atexit(report_mem_leak);


	//Create file pointers and numGroups
	FILE* input = fopen("in.txt", "r");
	FILE* output = fopen("out.txt", "w");
	int numGroups = 0;


	//Initialize numGroups and myQueueArray
	fscanf(input, "%d", &numGroups);


	//Read in array information
	queue** myQueueArray = readGroups(numGroups, input);


	//Write myQueueArray before it is reversed
	output = writeInitialList(myQueueArray, numGroups, output);

	//Reverse all queues in myQueueArray
	for (int i = 0; i < numGroups; i++)
	{
		rearrange_circle(myQueueArray[i]);
	}


	//Write myQueueArray after it is reversed
	output = writeReversedList(myQueueArray, numGroups, output);


	//Run Phase1 on myQueueArray
	phaseOne(myQueueArray, numGroups, output);


	//Run Phase2 on myQueueArray
	phaseTwo(myQueueArray, numGroups, output);


	//FREEING MEMORY + CLOSE FILE POINTERS
	freeFunction(input, output, myQueueArray, numGroups);
}