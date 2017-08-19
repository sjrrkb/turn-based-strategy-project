#include "game.h"

//Knock yourself out boys and girls.

Boss* loadBoss()
{
	Boss* boss=malloc(sizeof(Boss)); //Sephiroth is born
	FILE* fptr=fopen("Boss.txt","r");
	int x=0;
	int numberOfActions=0;
	if(fptr==NULL)
	{
		puts("boss.txt could not be opened");
		exit(0);
	}
	fscanf(fptr,"%s | %d | %d | %d | %d",boss->name,&boss->health,&boss->baseDefense,&boss->baseAttack,&boss->baseSpeed); //Sephiroth is given attributes
	boss->maxHealth=boss->health; //did this because a 2050 email told me do it
	DecisionTreeNode* root=NULL;
	while(!feof(fptr))
	{
		DecisionTreeNode* newNode=malloc(sizeof(DecisionTreeNode)); //And god said let there be a left decision and a right decision
		newNode->FirstAction=NULL; //setting these to null here nothing special
		newNode->left=NULL;
		newNode->right=NULL;
		fscanf(fptr,"%d | %d | %d",&newNode->healthFloor,&newNode->healthCeiling,&numberOfActions); //giving Sephiroths decision's attributes and then
		for(x=0;x<numberOfActions;x++)																//scanning number of abilities available to him each decision
		{
			ActionNode *newAction=malloc(sizeof(ActionNode));// creating actions for each decision
			newAction->next=NULL;
			fscanf(fptr,"%d",(int*)&newAction->decision);
			newNode->FirstAction=addActionToList(newNode->FirstAction,newAction); //creating a linked list of actions here
		}
		root=addNodeToTree(root,newNode); //linking each node to the tree
	}
	boss->root=root; //connecting Sephiroth to his actions aka boss with root of the BST...I originally had this right before the while loop but it caused segfaults
	fclose(fptr);
	return boss;
}

ActionNode* addActionToList(ActionNode* front, ActionNode* newAction)
{
	if(front==NULL) //for when the first link in the list is made and needs its attributes
	{
		return newAction;
	}
	else
	{
		front->next = addActionToList(front->next, newAction); //tacking on actions to the end of the list
		return front;
	}

}

DecisionTreeNode* addNodeToTree(DecisionTreeNode* root, DecisionTreeNode* newNode)
{
	if(root==NULL)  //for when the first node is made
	{
		return newNode;
	}
	else if((newNode->healthCeiling<root->healthFloor)) //comparing newNode to rest of the tree and proceeding to left according to health levels
	{
		root->left=addNodeToTree(root->left,newNode);
	}
	else if((newNode->healthFloor>root->healthCeiling)) //comparing newNode to rest of the tree and proceeding to right according to health levels
	{
		root->right=addNodeToTree(root->right,newNode);
	}
	return root;
}

ActionNode* fetchNewList(Boss* boss, DecisionTreeNode* root) //the one function i dont call in loadboss here
{
	if(root==NULL)
	{
		return NULL;
	}
	else if(boss->health<root->healthFloor) //moving left on bst until finding appropriate health level
	{
		return fetchNewList(boss,root->left);
	}
	else if(boss->health> root->healthCeiling) //moving left on bst until finding appropriate health leve
	{
		return fetchNewList(boss,root->right);
	}
	else if(boss->health>=root->healthFloor && boss->health<=root->healthCeiling) //found correct health levels here
	{
		return root->FirstAction; //retreiving action to Sephiroth to take
	}
	return NULL;
}

void freeBossTree(DecisionTreeNode* root)
{
	if(root==NULL)
	{
		return;
	}
	freeBossTree(root->left); //moving to the left to free boss tree
	freeBossTree(root->right); //moving to the right to free boss tree
	freeActionListInNode(root->FirstAction); //freeing linked list in each node
	free(root); //removing nodes in bst
}

void freeActionListInNode(ActionNode* head)
{
	ActionNode* tempHead;
	while(head->next!=NULL)
	{
		tempHead=head;
		head=head->next;
		free(tempHead); //freeing linked list
	}
	free(head); //freeing head of linked list
}
