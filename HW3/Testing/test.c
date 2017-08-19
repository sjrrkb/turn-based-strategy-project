#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "game.h"

#define assert(condition) ((condition) ? (true) : \
		(fprintf(stderr, "Assertion failed! Check test.c.\nLine number: %d: %s\n", __LINE__, #condition),  \
		abort()))

DecisionTreeNode* flattenTree(DecisionTreeNode* root);


int main(void)
{
	//PARSE TESTS
	char data[] = "G G G GARBAGE | 343 | 23 | This is a test";
	char data2[] = "MORE GARBAGE | 21 | 84 | More tests";
	FILE* fp = fopen("text.txt", "w");
	fprintf(fp, "%s\n", data);
	fprintf(fp, "%s\n", data2);
	fclose(fp);
	fp = fopen("text.txt", "r");
	Shield* sh = malloc(sizeof(Shield));
	Sword* sw = malloc(sizeof(Sword));
	ParseShieldInfo(sh, fp);

	//The following sword and shield should match the stats given
	assert(strcmp(sh->name, "G G G GARBAGE") == 0);
	assert(sh->cost == 343);
	assert(sh->defense == 23);
	assert(strcmp(sh->description, "This is a test") == 0);

	ParseSwordInfo(sw, fp);
	assert(strcmp(sw->name, "MORE GARBAGE") == 0);
	assert(sw->cost == 21);
	assert(sw->attack == 84);
	assert(strcmp(sw->description, "More tests") == 0);

	free(sh);
	free(sw);
	fclose(fp);
	remove("text.txt");


	//BST TESTS

	//If these fail, you built your tree wrong
	DecisionTreeNode* root = NULL;
	root = addNodeToTree(root, root);
	assert(root == NULL);

	DecisionTreeNode nodes[3] = { { 100, 500, NULL, NULL, NULL },  {50, 51, NULL, NULL, NULL}, {501, 600, NULL, NULL, NULL} };
	root = addNodeToTree(root, &nodes[0]);
	assert(root == &nodes[0]);

	root = addNodeToTree(root, &nodes[1]);
	assert(root == &nodes[0]);
	assert(root->left == &nodes[1]);

	root = addNodeToTree(root, &nodes[2]);
	assert(root == &nodes[0]);
	assert(root->left == &nodes[1]);
	assert(root->right == &nodes[2]);


	//ACTION LIST TESTS

	//same as BST, but with the list.
	ActionNode aNodes[2] = { {0, NULL}, {1, NULL} } ;
	ActionNode* head = NULL;
	head = addActionToList(head, head);
	assert(head == NULL);

	head = addActionToList(head, &aNodes[0]);
	assert(head == &aNodes[0]);

	head = addActionToList(head, &aNodes[1]);
	assert(head == &aNodes[0]);
	assert(head->next == &aNodes[1]);

	//FETCH TEST

	//If this fails, something is likely wrong with your tree.
	//Debug accordingly.
	nodes[2].FirstAction = head;
	Boss b1 = {"Sephiroth", 550, 7500, 7, 250, 6};
	ActionNode* t = fetchNewList(&b1, root);
	assert(t != NULL);
	assert(t == head);

	//CONTROL TESTS
	Boss* b = loadBoss();
	Boss b2 = {"Sephiroth", 7500, 7500, 7, 250, 6};
	//Makes sure you read in the stats correctly.
	assert(strcmp(b->name, b2.name) == 0);
	assert(b->health == b2.health);
	assert(b->baseDefense == b2.baseDefense);
	assert(b->baseAttack == b2.baseAttack);
	assert(b->baseSpeed == b2.baseSpeed);

	//Tests root of tree to make sure that's right
	assert(b->root->healthFloor == 4541);
	assert(b->root->FirstAction->next->decision == 0);

	//Tests to make sure the tree you built was actually a BST,
	//and the layout of the BST matches what it should be.
	//Mainly checks for correct implementation of the load function in
	//conjunction with addNodeToTree
	DecisionTreeNode* list = flattenTree(b->root);
	DecisionTreeNode* next = list->right;
	DecisionTreeNode* start = list;
	while(next)
	{
		assert(list->healthCeiling < next->healthFloor);
		assert(list->FirstAction != NULL);
		list = next;
		next = next->right;
	}

	//FREE TEST (can't really test, but shouldn't seg fault)
	freeBossTree(b->root);

	//CLEANUP
	free(b);
	while(start)
	{
		DecisionTreeNode* t = start;
		start = start->right;
		free(t);
	}

	printf("Tests passed! Good job!\n");

	return 0;
}

//Neat. Makes a linked list out of a BST. I take no credit for coming up with this.
DecisionTreeNode* flattenTree(DecisionTreeNode* root)
{
	DecisionTreeNode* L1 = root->left ? flattenTree(root->left) : NULL;
	DecisionTreeNode* L2 = root->right ? flattenTree(root->right) : NULL;
	DecisionTreeNode* L3 = malloc(sizeof(DecisionTreeNode));
	memcpy(L3, root, sizeof(DecisionTreeNode));
	L3->right = L2;
	if(!L1) return L3;
	DecisionTreeNode* cursor = L1;
	while(cursor->right) cursor = cursor->right;
	cursor->right = L3;
	return L1;
}
