#include "game.h"

void deleteFile(int choice, char* filename)
{
	char** n;
	int* s;	

	n = malloc(sizeof(char*)*MAXNUMSAVEFILES);
	int i;
	for(i = 0; i < MAXNUMSAVEFILES; i++)
	{
		n[i] = malloc(sizeof(char)*MAXPLAYERNAME);
	}

	s = malloc(sizeof(int)*MAXNUMSAVEFILES);

	FILE* fp = fopen("gamestate.txt", "r");

	int numFiles;
	fscanf(fp, "%d", &numFiles);

	for(i = 0; i < MAXNUMSAVEFILES; i++)
	{
		fscanf(fp, "%s | %d", n[i], &s[i]);
	}
	
	fclose(fp);

	fp = fopen("gamestate.txt", "w");

	fprintf(fp, "%d\n", numFiles-1);
	for(i = 0; i < MAXNUMSAVEFILES; i++)
	{
		if(i != choice - 1)
		{
			fprintf(fp, "%s | %d\n", n[i], s[i]);
		}
		else
		{
			fprintf(fp, "000 | 0\n");
		}
	}
	fclose(fp);

	remove(filename);

}

void saveFileData(int choice, char* filename, Hero* heroes, char* playerName, int progress)
{
	char** n;
	int* s;	

	n = (char**)malloc(sizeof(char*)*MAXNUMSAVEFILES);
	int i;
	for(i = 0; i < MAXNUMSAVEFILES; i++)
	{
		n[i] = (char*)malloc(sizeof(char)*MAXPLAYERNAME);
	}

	s = (int*)malloc(sizeof(int)*MAXNUMSAVEFILES);

	FILE* fp = fopen("gamestate.txt", "r");

	int numFiles;
	fscanf(fp, "%d", &numFiles);

	for(i = 0; i < MAXNUMSAVEFILES; i++)
	{
		fscanf(fp, "%s | %d", n[i], &s[i]);
	}
	
	fclose(fp);

	fp = fopen("gamestate.txt", "w");
	FILE* fp2;

	switch(progress)
	{
		case 1:
			fprintf(fp, "%d\n", numFiles+1);
			for(i = 0; i < MAXNUMSAVEFILES; i++)
			{
				if(i == choice - 1)
				{
					fprintf(fp, "%s | %d\n", playerName, 1);
				}
				else
				{
					fprintf(fp, "%s | %d\n", n[i], s[i]);
				}
			}
			break;
		case 2:
			fprintf(fp, "%d\n", numFiles);
			for(i = 0; i < MAXNUMSAVEFILES; i++)
			{
				if(strcmp(n[i], playerName) == 0)
				{
					s[i] = 2;
				}
				fprintf(fp, "%s | %d\n", n[i], s[i]);
			}
			fp2 = fopen(filename, "w");
			for(i = 0; i < NUMCHOSENHEROES; i++)
			{
				fprintf(fp2, "%s | %d | %d | %d | %d\n", heroes[i].name,
					(heroes[i].health), (heroes[i].baseDefense), (heroes[i].baseAttack),
					(heroes[i].baseSpeed));
			}
			fclose(fp2);
			break;
		case 3:
			fprintf(fp, "%d\n", numFiles);
			for(i = 0; i < MAXNUMSAVEFILES; i++)
			{
				if(strcmp(n[i], playerName) == 0)
				{
					s[i] = 3;
				}
				fprintf(fp, "%s | %d\n", n[i], s[i]);
			}
			fp2 = fopen(filename, "a");
			for(i = 0; i < NUMCHOSENHEROES; i++)
			{
				if(heroes[i].heroSword != NULL)
				{
					fprintf(fp2, "%s | %d | %d | %s \r\n", 
						heroes[i].heroSword->name, heroes[i].heroSword->cost,
						heroes[i].heroSword->attack, heroes[i].heroSword->description);
				}
				else
				{
					fprintf(fp2, "NULL | 0 | 0 | NULL \r\n");
				}
			}

			for(i = 0; i < NUMCHOSENHEROES; i++)
			{
				if(heroes[i].heroShield != NULL)
				{
					fprintf(fp2, "%s | %d | %d | %s \r\n", 
						heroes[i].heroShield->name, heroes[i].heroShield->cost,
						heroes[i].heroShield->defense, heroes[i].heroShield->description);
				}
				else
				{
					fprintf(fp2, "NULL | 0 | 0 | NULL \r\n");
				}

			}
			fclose(fp2);
	}
	fclose(fp);
}

Hero* loadSavedData(char* filename, int haveItems)
{
	Hero* heroes = malloc(sizeof(Hero)*NUMCHOSENHEROES);
	FILE* fp = fopen(filename, "r");
	int i;
	for(i = 0; i < NUMCHOSENHEROES; i++)
	{
		fscanf(fp, "%s | %d | %d | %d | %d\n", heroes[i].name,
			&(heroes[i].health), &(heroes[i].baseDefense), &(heroes[i].baseAttack),
			&(heroes[i].baseSpeed));
		heroes[i].heroSword = NULL;
		heroes[i].heroShield = NULL;
	}
	if(haveItems == 1)
	{
		for(i = 0; i < NUMCHOSENHEROES; i++)
		{
			heroes[i].heroSword = malloc(sizeof(Sword));
			//For some reason, it won't read past the newline character from the above fscanf 
			//for the last hero, so this is a workaround...I think. Or it's adding the new line to be 
			//scanned in the fscanf above. Either way, needed cause fgets won't work correctly then. 
			char c;
			fgets(&c, 1, fp);
			ParseSwordInfo(heroes[i].heroSword, fp);
			if(strcmp(heroes[i].heroSword->name, "NULL") == 0 ||
				strcmp(heroes[i].heroSword->name, "NULL ") == 0 ||
				strcmp(heroes[i].heroSword->name, " NULL") == 0 ||
				strcmp(heroes[i].heroSword->name, " NULL ") == 0)
			{
				free(heroes[i].heroSword);
				heroes[i].heroSword = NULL;
			}
			else
			{
				heroes[i].heroSword->nextSword = NULL;
				heroes[i].heroSword->prevSword = NULL;
			}
		}
		for(i = 0; i < NUMCHOSENHEROES; i++)
		{
			heroes[i].heroShield = malloc(sizeof(Shield));
			ParseShieldInfo(heroes[i].heroShield, fp);
			if(strcmp(heroes[i].heroShield->name, "NULL") == 0 ||
				strcmp(heroes[i].heroShield->name, " NULL") == 0 ||
				strcmp(heroes[i].heroShield->name, "NULL ") == 0 ||
				strcmp(heroes[i].heroShield->name, " NULL ") == 0)
			{
				free(heroes[i].heroShield);
				heroes[i].heroShield = NULL;
			}
			else
			{
				heroes[i].heroShield->nextShield = NULL;
				heroes[i].heroShield->prevShield = NULL;
			}
		}
	}

	fclose(fp);
	return heroes;
}

Hero* loadInitialData()
{
	Hero* heroes = malloc(sizeof(Hero)*NUMHEROES);
	FILE* fp = fopen("Heroes.txt", "r");
	int i;
	for(i = 0; i < NUMHEROES; i++)
	{
		fscanf(fp, "%s | %d | %d | %d | %d", heroes[i].name,
			&(heroes[i].health), &(heroes[i].baseDefense), &(heroes[i].baseAttack),
			(&heroes[i].baseSpeed));
		heroes[i].heroShield = NULL;
		heroes[i].heroSword = NULL;
	}
	fclose(fp);
	return heroes;
}

FileFinder findSaveFile()
{
	printf("Checking save file...\n\n");
	//This should exist. Make sure the file is with your HW. 
	FILE* fp = fopen("gamestate.txt", "r");
	if(!fp)
	{
		//You will have to debug this...it could be for a variety of reasons. 
		perror("gamestate.txt read error");
		exit(1);
	}

	int numFiles;

	//read the number of save file slots that are taken up, and 
	//interpret accordingly. 
	if(fscanf(fp, "%d", &numFiles) < 1)
	{
		printf("gamestate.txt: failed to read number of save files. Check input file.\n");
		exit(1);
	}

	FileFinder f;
	if (numFiles == 0) 
	{ 
		
		f = DOESNOTEXIST;
	}
	else 
	{ 
		f = EXIST; 
	}
	
	fclose(fp);
	
	printf("Check complete.\n\n");
	return f;
}

void loadSavedGameHandler()
{
	//initial load of all data that might be needed. 
	char** names;
	int* states;
	int numFiles;
	int choice;

	loadGameStateData(&names, &states, &numFiles);

	while(1)
	{
		printf("\nChoose a file you would like to load: \n");
		printf("(-1 to return to main menu)\n");
		printf(">");
		scanf("%d", &choice);
		while((choice < 1 || choice > MAXNUMSAVEFILES) && choice != -1)
		{
			printf("Invalid choice. Please choose a different option:\n ");
			printf(">");
			scanf("%d", &choice);
	
		}
	
		if(choice == -1)
		{
			freeMemory(names, states);
			return;
		}
	
		if(states[choice-1] == 0)
		{
			printf("There is no saved file present in this slot.\n");
		}
		else
		{
			//From here, a valid saved file has been selected, so we need to interpret where
			//the save file is in terms of game progression. This way we know what data to load

			//the state value will never be anything other than 0, 1, or 2. 
			if(states[choice-1] == 1)
			{
				//Heroes have not been selected yet.
				playGame(loadInitialData(), names[choice-1], NULL, states[choice-1], choice);
			}
			else
			{
				//Heroes have already been selected. 
				Hero* heroes = loadSavedData(choiceToFile(choice-1), states[choice-1] == 3 ? 1 : 0);
				if(!heroes)
				{
					printf("Failed to load heroes. Exiting...\n");
					freeMemory(names, states);
					exit(1);
				}
				playGame(heroes, names[choice-1], NULL, states[choice-1], choice);
			}
	
			freeMemory(names, states);
			return;
		}
	}
}

void DeleteSavedGameHandler()
{

	//Everything in this file is almost identical to the function above, just different ending actions.
	//Could move a lot of this to its own function, but eh
	char** names;
	int* states;
	int numFiles;
	int flag = 0;
	int choice = -1;

	while (1)
	{
		//Small optimization. If we didn't delete a file, we don't have to reload the data
		//since nothing changed. 
		if(flag == 0)
		{
			//to cover the first time the loop goes through.
			//Can't free if nothing's been loaded yet.
			if(choice != -1)
			{
				freeMemory(names, states);
			}
			loadGameStateData(&names, &states, &numFiles);
		}

		printf("\nChoose a file you would like to delete: \n");
		printf("(-1 to return to main menu)\n");
		printf(">");
		scanf("%d", &choice);
		while((choice < 1 || choice > MAXNUMSAVEFILES) && choice != -1)
		{
			printf("Invalid choice. Please choose a different option:\n ");
			printf(">");
			scanf("%d", &choice);
		}

		if(choice == -1)
		{
			//This is the only way to get out of this function. 
			freeMemory(names, states);
			return;
		}

		if(states[choice-1] == 0)
		{
			//shouldn't get here, but just in case. 
			//can't get here because the numFiles controls what's 
			//a valid value for chioce.
			printf("There is no saved file present in this slot.\n");
			flag = 1;
		}
		else
		{
			deleteFile(choice, choiceToFile(choice-1));
			flag = 0;
		}
	}
}

void loadGameStateData(char*** names, int** states, int* fileCount)
{
	printf("\n\n");
	//All memory is allocated here, and once the data is loaded 
	//The variables passed by reference into this function will be set up. 
	char** n = malloc(sizeof(char*)*MAXNUMSAVEFILES);
	int i;
	for(i = 0; i < MAXNUMSAVEFILES; i++)
	{
		n[i] = malloc(sizeof(char)*MAXPLAYERNAME);
	}

	int* s = malloc(sizeof(int)*MAXNUMSAVEFILES);

	//again, should not fail. 
	FILE* fp = fopen("gamestate.txt", "r");
	if(!fp)
	{
		perror("gamestate.txt");
		freeMemory(n, s);
		exit(1);
	}

	int numFiles;
	if(fscanf(fp, "%d", &numFiles) < 1)
	{
		printf("gamestate.txt: could not read the number of save files. Check input file.\n");
		freeMemory(n,s);
		exit(1);
	}
	//Load all the data, interpret the data, and print to the user what
	//was found in the list of save file slots. 
	for(i = 0; i < MAXNUMSAVEFILES; i++)
	{
		if(fscanf(fp, "%s | %d", n[i], &s[i]) < 2)
		{
			printf("gamestate.txt: could not read save file data #%d. Check input file.\n", i+1);
			freeMemory(n,s);
			exit(1);
		}
		//NOTE: everything shown to the user is for human readability. 
		//Stuff starts at 1, but internally stuff still starts at 0. 
		switch(s[i])
		{
			case 0: 
				printf("%d: [No File Present]\n", i+1);
				break;
			case 1: 
				printf("%d: %s - ", i+1, n[i]);
				printf("Heroes have not been selected.\n"); 
				break;
			case 2: 
				printf("%d: %s - ", i+1, n[i]);
				printf("Heroes have been selected.\n"); 
				break;
			case 3: 
				printf("%d: %s - ", i+1, n[i]);
				printf("Items have been selected.\n");
				break;
		}
	}
	
	fclose(fp);

	//complete the links after everything is loaded. 
	*names = n;
	*states = s;
	*fileCount = numFiles;
}

//helpful function to know what file we are loading from/saving to
char* choiceToFile(int num)
{
	switch(num)
	{
		case 0: return "file1.txt"; break;
		case 1: return "file2.txt"; break;
		case 2: return "file3.txt"; break;
		case 3: return "file4.txt"; break;
		case 4: return "file5.txt"; break;
		default: 
			printf("Something happened with the save file choice. See Daniel.\n"); 
			exit(1); break;
	}
}

void startNewGame()
{
	//I don't think anything else needs to be here?
	Hero* heroes = loadInitialData();
	//doesn't matter what the value of choice is, since progress = 0 means get a choice value first. 
	playGame(heroes, NULL, NULL, 0, -1);
}

int chooseSaveFile()
{
	//Initial load for data management. 
	char** names;
	int* states;
	int numFiles;
	int choice;
	int confirm = 1;

	loadGameStateData(&names, &states, &numFiles);
	
	//Get a chosen file slot to save to. The file slot must be valid, and 
	//old files can be overriden. 
	while(1)
	{
		confirm = 1;
		printf("\nChoose a file you would like to save to: \n");
		printf(">");
		scanf("%d", &choice);
		while((choice < 1 || choice > MAXNUMSAVEFILES))
		{
			printf("Invalid choice. Please choose a different option:\n ");
			printf(">");
			scanf("%d", &choice);
		}

		if(states[choice-1] != 0)
		{
			printf("There is a file here. Would you like to override it? (1 = yes, 0 = no)\n");
			printf(">");
			scanf("%d", &confirm);
			while(confirm != 1 && confirm != 0)
			{
				printf("Invalid choice. Please choose a different option:\n");
				printf(">");
				scanf("%d", &confirm);
			}
		}

		if(confirm == 1)
		{
			//Go ahead and delete the old hero file immediately if there is one. 
			if(states[choice-1] == 2)
			{
				deleteFile(choice, choiceToFile(choice-1));
			}
			freeMemory(names, states);
			//I haven't found a bug yet with the state = 1, but there might be one somewhere. 
			return choice;
		}
	}
}
