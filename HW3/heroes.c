#include "game.h"

Hero* pickHeroes(Hero* heroes)
{
	sortHeroes(heroes);
	Hero* chosenHeroes = malloc(sizeof(Hero) * NUMCHOSENHEROES);
	//could print all the heroes every time a user goes to pick another 
	//hero, maybe later
	printHeroes(heroes, NUMHEROES);

	int heroespicked = 0;
	int chosenpicks[NUMCHOSENHEROES] = { -1, -1, -1 };
	int chosenHero;
	while (heroespicked != NUMCHOSENHEROES)
	{
		heroespicked == 0 ? printf("\nPlease pick a hero: ") : printf("\nPlease pick another hero: ");
		scanf("%d", &chosenHero);
		//user has to pick a number from the group, and the user cannot pick the same hero twice. 
		while (chosenHero < 0 || chosenHero > NUMHEROES || chosenpicks[0] == chosenHero
			|| chosenpicks[1] == chosenHero || chosenpicks[2] == chosenHero)
		{
			printf("Invalid choice. Please pick a valid hero option: ");
			scanf("%d", &chosenHero);
		}
		chosenHeroes[heroespicked] = heroes[chosenHero - 1];
		chosenpicks[heroespicked] = chosenHero;
		heroespicked++;
		printf("Excellent choice. Hero saved.\n");
	}

	return chosenHeroes;
}

void sortHeroes(Hero* heroes)
{
	int i;
	int j;
	Hero tmp;
	//selection sort
	int min;
	for (i = 0; i < NUMHEROES - 1; i++)
	{
		min = i;
		for (j = i + 1; j < NUMHEROES; j++)
		{
			if (strcmp(heroes[j].name, heroes[min].name) < 0)
			{
				min = j;
			}
		}

		if (min != i)
		{
			tmp = heroes[i];
			heroes[i] = heroes[min];
			heroes[min] = tmp;
		}
	}
}

void printHeroes(Hero* heroes, int numHeroes)
{
	int i; 
	for (i = 0; i < numHeroes; i++)
	{
		printf("%d: %s\n\tHealth: %d\n\tDefense: %d\n\tAttack: %d\n\tSpeed: %d\n", i + 1,
			heroes[i].name, heroes[i].health, heroes[i].baseDefense, heroes[i].baseAttack,
			heroes[i].baseSpeed);
		
		if(heroes[i].heroSword != NULL)
		{
			printf("\tSword: %s\n\t\tAttack: %d\n", 
				heroes[i].heroSword->name, heroes[i].heroSword->attack);
		}
		else
		{
			printf("\tSword: None.\n");
		}

		if(heroes[i].heroShield != NULL)
		{
			printf("\tShield: %s\n\t\tDefense: %d\n", 
				heroes[i].heroShield->name, heroes[i].heroShield->defense);
		}
		else
		{
			printf("\tShield: None.\n");
		}
	}
}
