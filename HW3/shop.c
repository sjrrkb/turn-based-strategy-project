#include "game.h"

//I TAKE ABSOLUTELY NO CREDIT FOR ANY OF THE CODE BELOW EXCEPT FOR BELOW THE LINE OF GIVEN FUNCTIONS. 
//I changed a few lines for where the parsing functions are called. The rest is from a student submission
//that I thought was excellent in code and comments. 

Shop* loadShop()
{
/*The loadShop function prepares a shop that players will buy weapons and shields from. It reads in shield and sword information from a .txt file
  and creates a linked list for both. The function returns a shop struct pointer that holds the front of both linked lists.*/
  
	/**********OPEN FILE, INITIALIZE UTILITY POINTERS************/
	
		//Open file for reading
		FILE* fp = fopen("Items.txt","r");
		
		//If the file is NULL, give an error message and quit.
		if(fp == NULL)
		{
			printf("\nError, could not open item file.");
			return NULL;
		}
		
		//This function will be returning a shop struct pointer. Initialize head pointers to be stored in the shop struct
		Shield* startShield = NULL;
		Sword*  startSword 	= NULL;
		
		//Initialize utility pointers for shields and swords. (Remember, each node has two additional utility pointers, next and prev)
			Shield* current 	 = NULL;
			Shield* new 		 = NULL;
			Shield* previous 	 = NULL;
			
			Sword* currentSword  = NULL;
			Sword* newSword 	 = NULL;
			Sword* previousSword = NULL;
		
	/**********LOAD SHIELDS**********/
		int i;
		for(i = 0; i < NUMSHIELDS ; i++)
		{
			new = (Shield*)malloc(sizeof(Shield));
			if(new != NULL)
			{
				//Scan each node's info from file, and set inner utility pointers to NULL
				ParseShieldInfo(new, fp);
				new->prevShield = NULL;
				new->nextShield = NULL;
				
				//Give current the front of the list, and then walk through until NULL. (so it can add the new node to the end of the list)
				current = startShield;
				while(current != NULL)
				{
					previous = current;
					current = current->nextShield;
				}
				
				//At this point, previous will be the final node of the list. If there are no pre-existing nodes, let the new node be the front.
				if(previous == NULL)
				{
					startShield = new;
				}
				else //Otherwise, add a new node and double link it to the previous node.
				{
					previous->nextShield = new; //Attaches the new node to the old one.
					new->prevShield = previous; //Points the new node's "prev" pointer to the former end of the list.
					new->nextShield = current;  //Lets the new node's next pointer be NULL.
				}
			}
			else //If malloc failed, none of this will happen. It will just print an error.
			{
				printf("\nError, not enough memory for new shield. Malloc failed.");
				return NULL;
			}
		}			
		
		/**********LOAD SWORDS**********/
			//Perform the same loading technique for swords as it did for shields..
			for(i = 0 ; i < NUMSWORDS ; i++)
			{
				newSword = (Sword*)malloc(sizeof(Sword));
				if(newSword != NULL)
				{
					//Read sword info from file and set inner utility pointers to NULL
					ParseSwordInfo(newSword, fp);
					newSword->nextSword = NULL;
					newSword->prevSword = NULL;
					
					//Set current to the front and then walk through to the end of the list. previousSword will be the final node in the list.
					currentSword = startSword;
					while(currentSword != NULL)
					{
						previousSword = currentSword;
						currentSword = currentSword->nextSword;
					}
					
					//If there were no pre-existing nodes, let the new node be the front.
					if(previousSword == NULL)
					{
						startSword = newSword;
					}
					else //Otherwise, attach the new node to the end and link it both ways.
					{
						previousSword->nextSword = newSword; //Attaches new node to the last one.
						newSword->prevSword = previousSword; //Lets the new node also point backwards to the last one.
						newSword->nextSword = currentSword;  //Lets the new node point to NULL by giving it current.
					}
				}
				else //If malloc failed, only send an error message and then quit.
				{
					printf("\nCould not find space for sword list. Malloc failed");
					return NULL;
				}
			}
	
	/*******************************************PREPARE SHOP************************************************/
		//Initialize shop pointer, malloc space, and give it the heads of the list.
		Shop* shopStock;
		shopStock = malloc(sizeof(Shop));
		
		if(shopStock != NULL)
		{
			shopStock->headShield = startShield; //The shop now has the front of the shield list.
			shopStock->headSword = startSword;   //The shop now has the front of the sword list.
		}
		else //If malloc failed, exit disappointed
		{
			printf("\nInsufficent memory for malloc.");
			return NULL;
		}
		
		//Close file and return Shop pointer
		fclose(fp);
		return shopStock;
}		
			
			

void destroyShop(Shop* shop)
{
/*The destroyShop function frees all space that was allocated by malloc for the shop. It frees the sword and shield linked lists separately, and then
  frees the shop pointer that held them.*/
  
	/**********FREE SWORDS**********/
		//Prepare a sword starting point for freeing all swords
		Sword* swords;
		swords = shop->headSword; //(shop stores the front of each list)
		
		//Prepare utility pointers
		Sword* tempSword;
		
		//Walk through the list while destroying it.
		while(swords != NULL)
		{
			tempSword = swords;				//Set up a temporary pointer to hold current
			swords = swords->nextSword;		//Move current to the next node
			free(tempSword);				//Scorch the earth behind you to protect Mother Russia
		}
		
	/**********FREE SHIELDS**********/
		//Prepare a shield start point for freeing all shields.
		Shield* shields;
		shields = shop->headShield; //(shop stores the front of each list)
		
		//Prepare utility pointer
		Shield* tempShield;
		
		//Walk through the list while destroying it.
		while(shields != NULL)
		{
			tempShield = shields;				//Sets up a temporary node to hold current
			shields = shields->nextShield;		//Moves current to the next node
			free(tempShield);					//Burn your bridges and don't call her back.
		}
	
	/*****FREE SHOP*****/
		//After successfully freeing both shield and sword lists, free the shop.
		free(shop);
}


void sortShields(Shield** shields)
{
/*The sortShields function sorts a linked list of shield nodes. It puts the nodes into ascending order without breaking the list.*/

	//Initialize utility pointers for use in sorting..
	Shield* currentShield;
	Shield* moveShield;
	Shield* rightLink;
	Shield* leftLink;
	
	//Set current at the beginning.
	currentShield = *shields;

	int i;
	for(i = 0; i < NUMSHIELDS ; i++)
	{
		/*The following while statement moves through the list until it reaches the end or finds a case where the cost is not yet sorted.
		  When the while statement breaks, the node that needs to be moved is currentShield->nextShield)*/
		 
		/**********LOCATE A NODE THAT NEEDS TO BE MOVED**********/
		
		currentShield = *shields;
		while(currentShield != NULL && currentShield->nextShield != NULL && currentShield->cost < currentShield->nextShield->cost)
		{
			currentShield = currentShield->nextShield;
		}
		
		//If there was a number that needs to be sorted, currentShield will not be NULL.
		if(currentShield != NULL && currentShield->nextShield != NULL)
		{
			//The shield that needs to be moved is the currentShield->nextShield since its cost is greater than currentShield.
			moveShield = currentShield->nextShield;
			
			/*Set up holder nodes. Whenever a spot is found to move the shield to, the list will need a way to move it without breaking the list.
			  After the node is moved, it will be pointed to in two different placed. To get rid of the duplicate, leftLink and rightLink will connect
			  to fill in the node's old spot.*/
			  
			leftLink = moveShield->prevShield;
			rightLink = moveShield->nextShield;
			
			//Keep track of the cost as a key so that a new location for the shield can be found
			int cost;
			cost = moveShield->cost;
			
		/**********MOVE THE NODE TO ITS NEW LOCATION**********/
		
			/*There is a possibility that the shield being moved has the minimum value, and will need to be placed at the front of the list.
			  if the shield needs to be placed at the front, a special case occurs.*/
			 
			currentShield = *shields;
			if(cost < currentShield->cost) //if the shield needs to be placed at the front of the list.
			{
				moveShield->prevShield = NULL;				//moveShield will be at the front, so it's left pointer can be NULL
				moveShield->nextShield = currentShield;	    //moveShield should now point to the old front of the list
				currentShield->prevShield = moveShield;		//the old front of the list should now point back to the moved node.
				*shields = moveShield;						//Let moveShield be called the head.
				
				leftLink->nextShield = rightLink;			//To get rid of a duplicate node, allow the established left and right holders to be linked.
				rightLink->prevShield = leftLink;
			}
			else //else, if the shield needs to be place within the list.
			{
				//go through the list until the correct spot it found.
				Shield* holdShield;
				while(currentShield != NULL && cost > currentShield->cost)
				{
					holdShield = currentShield;
					currentShield = currentShield->nextShield;
				}
				
				//Insert the node.
				if(currentShield != NULL)
				{
					moveShield->nextShield = currentShield; //Let the moved node point forward to a node that costs more than itself
					moveShield->prevShield = holdShield;	//Let the moved node point back to a node that costs less than itself
					holdShield->nextShield = moveShield;	//Complete the list by making the previous pointer point towards the inserted node
					currentShield->prevShield = moveShield; //Complete the list by making the forward pointer also point back to the inserted node
					
					//Without breaking the list, get rid of the duplicate node by linking the established holders.
					leftLink->nextShield = rightLink;
					
					//At the end of the list, rightLink will be NULL, and will not need to be altered.
					if(rightLink == NULL)
					{
						return;
					}
					else
					{
						rightLink->prevShield = leftLink; //If the rightLink was NULL this would cause a segmentation fault.
					}
				}
			}
			
		}
		else
		{
			//If the list is sorted before the "for" loop ends, go ahead and end it!
			return;
		}
	}
	return;
}

void sortSwords(Sword** swords)
{
/*The sortSwords function sorts a linked list of sword nodes. It puts the nodes into ascending order without breaking the list.*/

	//Initialize utility pointers for use in sorting..
	Sword* currentSword;
	Sword* moveSword;
	Sword* rightLink;
	Sword* leftLink;
	
	//Set current at the beginning.
	currentSword = *swords;

	int i;
	for(i = 0; i < NUMSWORDS ; i++)
	{
		/*The following while statement moves through the list until it reaches the end or finds a case where the cost is not yet sorted.
		  When the while statement breaks, the node that needs to be moved is currentSword->nextSword)*/
		 
		/**********LOCATE A NODE THAT NEEDS TO BE MOVED**********/
		
		currentSword = *swords;
		while(currentSword != NULL && currentSword->nextSword != NULL && currentSword->cost < currentSword->nextSword->cost)
		{
			currentSword = currentSword->nextSword;
		}
		
		//If there was a number that needs to be sorted, currentSword will not be NULL.
		if(currentSword != NULL && currentSword->nextSword != NULL)
		{
			//The sword that needs to be moved is the currentSword->nextSword since its cost is greater than currentSword.
			moveSword = currentSword->nextSword;
			
			/*Set up holder nodes. Whenever a spot is found to move the sword to, the list will need a way to move it without breaking the list.
			  After the node is moved, it will be pointed to in two different placed. To get rid of the duplicate, leftLink and rightLink will connect
			  to fill in the node's old spot.*/
			  
			leftLink = moveSword->prevSword;
			rightLink = moveSword->nextSword;
			
			//Keep track of the cost as a key so that a new location for the sword can be found
			int cost;
			cost = moveSword->cost;
			
		/**********MOVE THE NODE TO A NEW LOCATION**********/
		
			/*There is a possibility that the sword being moved has the minimum value, and will need to be placed at the front of the list.
			  if the sword needs to be placed at the front, a special case occurs.*/
			 
			currentSword = *swords;
			if(cost < currentSword->cost) //if the sword needs to be placed at the front of the list.
			{
				moveSword->prevSword = NULL;				//moveSword will be at the front, so it's left pointer can be NULL
				moveSword->nextSword = currentSword;	    //moveSword should now point to the old front of the list
				currentSword->prevSword = moveSword;		//the old front of the list should now point back to the moved node.
				*swords = moveSword;						//Let moveSword be called the head.
				
				leftLink->nextSword = rightLink;			//To get rid of a duplicate node, allow the established left and right holders to be linked.
				rightLink->prevSword = leftLink;
			}
			else //else, if the sword needs to be place within the list.
			{
				//go through the list until the correct spot it found.
				Sword* holdSword;
				while(currentSword != NULL && cost > currentSword->cost)
				{
					holdSword = currentSword;
					currentSword = currentSword->nextSword;
				}
				
				//Insert the node.
				if(currentSword != NULL)
				{
					moveSword->nextSword = currentSword; //Let the node point forward to a node that costs more than itself
					moveSword->prevSword = holdSword;	//Let the node point back to a node that costs less than itself
					holdSword->nextSword = moveSword;	//Complete the list by making the previous pointer point towards the inserted node
					currentSword->prevSword = moveSword; //Complete the list by making the forward pointer also point back to the inserted node
					
					//Without breaking the list, get rid of the duplicate node by linking the holders.
					leftLink->nextSword = rightLink;
					
					//At the end of the list, rightLink will be NULL.
					if(rightLink == NULL)
					{
						return;
					}
					else
					{
						rightLink->prevSword = leftLink; //If the rightLink was NULL this would cause a segmentation fault.
					}
					
				}
			}
			
		}
		else
		{
			//If the list is sorted before the "for" loop ends, go ahead and end it!
			return;
		}
	}
	return;
}

void printShields(Shield* shields)
{
	//Initialize a utility pointer to keep track of the beginning.
	Shield* start;
	start = shields;

	//Move through the linked list while printing shield data.
	int i = 1;
	while(shields != NULL) //Conditioned to NULL so that it is not dependent on number of items.
	{
		printf("%d - %s\n\t Cost: %d\n\t Defense: %d\n",i,shields->name, shields->cost, shields->defense);
		printf("%s\n\n", shields->description);
		shields = shields->nextShield;
		i++;
	}
	
	//Reset the shield list at the beginning.
	shields = start;
}

void printSwords(Sword* swords)
{
	//Initialize a utility pointer to keep track of the beginning.
	Sword* start;
	start = swords;
	
	//Move through the linked list while printing sword data.
	int i = 1;
	while(swords != NULL) //Conditioned to NULL so that it is not dependent on number of items.
	{
		printf("%d - %s\n\t Cost: %d\n\t Attack: %d\n",i,swords->name,swords->cost,swords->attack);
		printf("%s\n\n", swords->description);
		swords = swords->nextSword;
		i++;
	}
	
	//Reset the sword list to the beginning.
	swords = start;
}

Sword* removeSwordFromList(Sword** swords, int choice) //Removes a chosen sword from the list.
{
	//BONUS: Utilized both prevSword and nextSword to remove swords from list.
	//Initialize utility pointers
	Sword* holdSword;
	Sword* currentSword;
	
	//Give current the front of the list.
	currentSword = *swords;
	
	//The original code did not account for a list of changing size. Find it again.
	int size = 0;
	while(currentSword != NULL)
	{
		currentSword = currentSword->nextSword;
		size++;
	}
	 
	//If the size of the list is now less than the choice value, ask for another input.
	while(size < choice || choice < 1)
	{
		printf("\nInvalid choice. Please choose again.\n> ");
		scanf("%d",&choice);	
	}
	
	//Set current to front again.
	currentSword = *swords;
	
	//If the choice is the first in the list, a special case is triggered:
	if(choice == 1)
	{
		//Set the hold at the front and move current by one node.
		holdSword = currentSword;
		currentSword = currentSword->nextSword;
		
		//Detach the node:
		holdSword->nextSword = NULL; //Detach the front from the list.
		currentSword->prevSword = NULL; //Detach the list from the front.
		
		//The new front of the list is now moved by one.
		*swords = currentSword;
		
		//Return the information from the old front.
		return holdSword;
	}
	else //If the choice is not the first, we may move through the list.
	{
		//Set a counter and move through the list based on choice.
		int i = 0;
		for(i = 0; i < choice - 1; i++)
		{
			holdSword = currentSword;
			currentSword = currentSword->nextSword;
		}
		
		//Change around the links in the list so that it no longer includes the chosen sword:
		
		//This if statement will determine if the choice is at the end of the list and act accordingly.
		if(currentSword->nextSword != NULL)
		{
			holdSword->nextSword = currentSword->nextSword; //The next pointers are modified
			currentSword->nextSword->prevSword = holdSword; //The previous pointers are modified
		}
		else //If the choice is the final one in the list, it is detached.
		{
			holdSword->nextSword 	= NULL;		//Detach the choice from the list.
			currentSword->prevSword = NULL;		//Detach the list from the choice.
		}
	}
	//Return the information for the chosen sword.
	return currentSword;
}

Shield* removeShieldFromList(Shield** shields, int choice) //Removes a chosen shield from the list.
{
	//BONUS: Utilized both prevShield and nextShield to remove swords from list.
	
	//Initialize utility pointers
	Shield* holdShield;
	Shield* currentShield;
	
	//Give current the front of the list.
	currentShield = *shields;
	
	//The original code did not account for a list of changing size. Find it again.
	int size = 0;
	while(currentShield != NULL)
	{
		currentShield = currentShield->nextShield;
		size++;
	}
	 
	//If the size of the list is now less than the choice value, ask for another input.
	while(size < choice || choice < 1)
	{
		printf("\nInvalid choice. Please choose again.\n> ");
		scanf("%d",&choice);	
	}
	
	//Set current to front again.
	currentShield = *shields;
	
	//If the choice is at the front of the list, it is removed from the front.
	if(choice == 1)
	{
		//Set the hold at the front and move current by one node.
		holdShield = currentShield;
		currentShield = currentShield->nextShield;
		
		//Detach the node:
		holdShield->nextShield = NULL; //Detach the front from the list.
		currentShield->prevShield = NULL; //Detach the list from the front.
		
		//The new front of the list is now moved by one.
		*shields = currentShield;
		
		//Return the information from the old front.
		return holdShield;
	}
	else
	{
	//Set a counter and move through the list based on choice.
		int i = 0;
		for(i = 0; i < choice - 1; i++)
		{
			holdShield = currentShield;
			currentShield = currentShield->nextShield;
		}
		
		//Change around the links in the list so that it no longer includes the chosen shield.
		
		//This if statement will determine if the choice is at the end of the list and act accordingly.
		if(currentShield->nextShield != NULL)
		{
			holdShield->nextShield = currentShield->nextShield; //The next pointers are modified
			currentShield->nextShield->prevShield = holdShield; //The previous pointers are modified
		}
		else //If the choice is the final one in the list, it is detached.
		{
			holdShield->nextShield = NULL;		//Detach the choice from the list.
			currentShield->prevShield = NULL;	//Detach the list from the choice.
		}
	}
	
	//Return the information for the chosen sword.
	return currentShield;
}

/***************************START OF FUNCTIONS GIVEN TO YOU**********************************/

void freeHeroesAndItems(Hero* heroes)
{
	int i;
	for(i = 0; i < NUMCHOSENHEROES; i++)
	{
		if(heroes[i].heroSword != NULL)
		{
			free(heroes[i].heroSword);
		}
		if(heroes[i].heroShield != NULL)
		{
			free(heroes[i].heroShield);
		}
	}
	free(heroes);
}

void goShopping(Hero* heroes, Shop* shop)
{


	sortShields(&(shop->headShield));
	sortSwords(&(shop->headSword));

	printf("\n\nNow, you will select a sword and shield for your heroes.\n");

	int firstPass = 1; 

	int bank = STARTINGBANKVALUE;

	int shieldsBought = 0;
	int swordsBought = 0; 

	while(1)
	{
		printf("Which would you like to look at?\n");
		printf("1: Swords\n2: Shields\n3: Continue to fight\n");
		printf("> ");
		int choice; 
		scanf("%d", &choice);
		while(choice < 1 || choice > 3)
		{
			printf("Invalid choice. Select again: \n");
			printf("> ");
			scanf("%d", &choice);
		}

		if(choice == 3)
		{
			if(firstPass == 1)
			{
				printf("Are you sure you don't want to buy anything?\n");
				printf("1: yes\n2: no\n");
				printf("> ");
				scanf("%d", &choice);
				while(choice < 1 || choice > 2)
				{
					printf("Invalid choice. Select again: \n");
					printf("> ");
					scanf("%d", &choice);
				}
			}
			else
			{
				choice = 1;
			}

			if(choice == 1)
			{
				destroyShop(shop);
				return;
			}
		}
		else if(choice == 2)
		{	


			while(1)
			{
				printShields(shop->headShield);
				
				while(1)
				{
					printf("\nBudget: %d\n", bank);
					printf("Select a shield (-1 to go back): \n");
					printf("> ");
					scanf("%d", &choice);
	
					while((choice < 1 || choice > NUMSHIELDS - shieldsBought) && choice != -1)
					{
						printf("Invalid choice. Select again: \n");
						printf("> ");
						scanf("%d", &choice);
					}
				
					if(choice != -1)
					{
						if(findShieldPrice(shop->headShield, choice) > bank)
						{
							printf("You don't have enough money for that!\n");
						}
						else break;
					}
					else break;
				}

				if(choice == -1)
				{
					break;
				}
				else
				{
					int heroChoice; 

					while(1)
					{
						printHeroes(heroes, NUMCHOSENHEROES);
						printf("Which hero do you want to have the item? (-1 to go back): \n");
						printf("> ");
						scanf("%d", &heroChoice);

						while((heroChoice < 1 || heroChoice > NUMCHOSENHEROES) && heroChoice != -1)
						{
							printf("Invalid choice. Select again: \n");
							printf("> ");
							scanf("%d", &heroChoice);
						}

						if(heroChoice == -1)
						{
							break;
						}

						if(heroes[heroChoice - 1].heroShield != NULL)
						{
							printf("That hero already has a shield. Choose again.\n");
						}
						else
						{
							break;
						}

					}
					

					if(heroChoice == -1)
					{
						break;
					}
					else
					{
						Shield* item = removeShieldFromList(&(shop->headShield), choice);
						heroes[heroChoice - 1].heroShield = item;
						bank -= item->cost;
						shieldsBought++;
						firstPass = 0;
					}
				}
			}
		}
		else if(choice == 1)
		{
			
			while(1)
			{
				printSwords(shop->headSword);
				
				while(1)
				{
					printf("\nBudget: %d\n", bank);
					printf("Select a sword (-1 to go back): \n");
					printf("> ");
					scanf("%d", &choice);
	
					while((choice < 1 || choice > NUMSWORDS - swordsBought) && choice != -1)
					{
						printf("Invalid choice. Select again: \n");
						printf("> ");
						scanf("%d", &choice);
					}
				
					if(choice != -1)
					{
						if(findSwordPrice(shop->headSword, choice) > bank)
						{
							printf("You don't have enough money for that!\n");
						}
						else break;
					}
					else break;
				}

				if(choice == -1)
				{
					break;
				}
				else
				{
					int heroChoice; 

					while(1)
					{
						printHeroes(heroes, NUMCHOSENHEROES);
						printf("Which hero do you want to have the item? (-1 to go back): \n");
						printf("> ");
						scanf("%d", &heroChoice);

						while((heroChoice < 1 || heroChoice > NUMCHOSENHEROES) && heroChoice != -1)
						{
							printf("Invalid choice. Select again: \n");
							printf("> ");
							scanf("%d", &heroChoice);
						}

						if(heroChoice == -1)
						{
							break;
						}

						if(heroes[heroChoice - 1].heroSword != NULL)
						{
							printf("That hero already has a sword. Choose again.\n");
						}
						else
						{
							break;
						}
					}
					if(heroChoice == -1)
					{
						break;
					}
					else
					{
						Sword* item = removeSwordFromList(&(shop->headSword), choice);
						heroes[heroChoice - 1].heroSword = item;
						bank -= item->cost;
						swordsBought++;
						firstPass = 0;
					}
				}
			}
		}
	}
}

int findSwordPrice(Sword* swords, int choice)
{
	int i = 1;
	while(i < choice)
	{
		swords = swords->nextSword;
		i++;
	}
	return swords->cost;
}

int findShieldPrice(Shield* shields, int choice)
{
	int i = 1;
	while(i < choice)
	{
		shields = shields->nextShield;
		i++;
	}
	return shields->cost;
}
