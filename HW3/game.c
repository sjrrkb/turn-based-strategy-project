#include "game.h"

//Here to make your own contributions? Have at it!

//(the first thing you'll want to do is add error checking where it's missing.
//A lot of it is missing across files.)

int main(void)
{
	srand(time(NULL));
	while (1)
	{
		FileFinder finder = findSaveFile();
		int choice; 

		printf("Welcome to the Gauntlet!\n");
		printf("Select your option.\n");

		printf("1: start a new game\n");
		if (finder == EXIST)
		{
			printf("2: start a saved game\n");
			printf("3: delete a saved file\n");
			printf("4: quit\n");
			printf(">");	
			scanf("%d", &choice);
			while(choice < 1 || choice > 5)
			{
				printf("Invalid option. Try again: \n");
				printf(">");
				scanf("%d", &choice);
			}
			switch (choice)
			{
			case 1:
				startNewGame();
				break;
			case 2:
				loadSavedGameHandler();
				break;
			case 3:
				DeleteSavedGameHandler();
				break;
			case 4:
				return 0;
			}
		}
		else
		{
			printf("2: quit\n");
			printf(">");
			scanf("%d", &choice);
			while(choice < 1 || choice > 2)
			{
				printf("Invalid option. Try again: \n");
				printf(">");
				scanf("%d", &choice);
			}
			switch (choice)
			{
			case 1:
				startNewGame();
				break;
			case 2:
				return 0;
			}
		}
	}
}

char* getPlayerName()
{
	char* name = malloc(sizeof(char)*MAXPLAYERNAME);
	printf("Please input your name: ");
	scanf("%s", name);
	printf("Hello %s. Welcome to the gauntlet.\n", name);
	return name;
}


//The primary conroller for the game. Using the progress value passed in,
//the function knows what the current stat of the game is. Currently, there
//is nothing after progress value = 3, and the game isn't saved after the fight
//ends, but this can be changed/expanded
void playGame(Hero* heroes, char* playerName, Boss* boss, int progress, int choice)
{

	//Added flavor 
	if(progress > 0)
	{
		printf("\nWelcome back %s.\n\n", playerName);
	}

	//holds what the last thing done was.
	int initialprogress = progress;

	Hero* chosenHeroes;
	Shop* s;
	//another check for how the data is laid out. This is so we can immediately print out
	//heroes if we have already selected heroes. 
	if(progress > 1)
	{
		chosenHeroes = heroes;
	}	
	
	switch(progress)
	{
		//initial player identification
		case 0:
			playerName = getPlayerName();
			choice = chooseSaveFile();
			saveFileData(choice, choiceToFile(choice-1), NULL, playerName, 1);
		//hero selection
		case 1:
			chosenHeroes = pickHeroes(heroes);
			saveFileData(choice, choiceToFile(choice-1), chosenHeroes, playerName, 2);
			free(heroes);
		//equip heroes with items 
		case 2:
			//shop
			s = loadShop();
			goShopping(chosenHeroes, s);
			saveFileData(choice, choiceToFile(choice-1), chosenHeroes, playerName, 3);
		//fight the boss (a boss? Maybe find some way to get more than one boss in there?)
		case 3: 
			boss = loadBoss();
			int result = fight(boss, chosenHeroes);
			result == 0 ? printf("You won! Congrats!\n") : printf("Oh no, you lost! Reload to try again.\n");
			//If you're in here, it's probably to delete this line first
			printf("\n\n**********CONGRATS! YOU MADE IT TO THE END OF 2050!*************\n\n");
			freeHeroesAndItems(chosenHeroes);
			freeBossTree(boss->root);
			free(boss);
			break;
		default:
			//What?
			printf("Something fatal happened with the progress value. Check gamestate.txt.\n");
			break;
	}
	

	//I don't actually remember why I did this, but it works. 
	//I think because if it's a save file, it's freed elsewhere?
	if(initialprogress == 0)
	{
		free(playerName);
	}
	return;
}

void freeMemory(char** names, int* states)
{
	int i;
	//seg faults? Check input file/gdb. 
	for(i = 0; i < MAXNUMSAVEFILES; i++)
	{
		free(names[i]);
	}
	free(names);
	free(states);
}
