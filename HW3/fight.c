#include "game.h"
//HI! I'm guessing you're here to satisfy curiosity, or to make your own additions.
//Feel free to change anything you want, but do so at your own risk. A few things 
//are a little complex (maybe too complex), but it's pretty straightforward. Knock yourself out. 

//Or you are here to find a seg fault. That's cool too. Keep in mind that this code works,
//so you will need to stick mostly to your code to find problems. 

//Okay, so the fight is turn-based. BUT, the hero and the boss aren't 
//the same type, so you can't just throw everything in an array. 
//So the heroes are copied into a separate array, and the boss is tacked
//on at the end as a 'dummy hero' just to find its place in the turn order. 
int fight(Boss* boss, Hero* heroes)
{
	//copy everything over
	Hero* h = malloc(sizeof(Hero)*(NUMCHOSENHEROES+1));
	int i;
	for(i = 0; i < NUMCHOSENHEROES; i++)
	{
		h[i] = heroes[i];
		h[i].hasHealed = false;
		h[i].maxHealth = h[i].health;
	}
	h[NUMCHOSENHEROES].health = -1;
	h[NUMCHOSENHEROES].baseSpeed = boss->baseSpeed;
	strcpy(h[NUMCHOSENHEROES].name, boss->name);	
	//sort based on speed, so that the fastest character goes first
	sort(h);
	//a test to end the fight
	bool allHeroesDead = false;
	//to keep track of the boss's actions
	ActionNode* currentList = NULL;
	while(1)
	{
		//Now here's where things start to get weird. 
		//I use the original array of heroes when printing 
		//the battlefield to conserve the order of heroes chosen,
		//but that means that I have to update the health values of 
		//the original array. That gets a little weird later.
		printFightStatus(boss, heroes);
		//user chooses the next action for each hero. The boss is automatic. 
		getHeroDecisions(h);
		//YOU LOSE!
		if(allHeroesDead == true)
		{
			return 1;
		}
		//flavor? 
		printf("***********EVENTS***********\n");
		allHeroesDead = true;
		//prep the list of boss actions that will be used next if
		//the old list is used up. 
		if(currentList == NULL)
		{
			currentList = fetchNewList(boss, boss->root);
		}
		for(i = 0; i < NUMCHOSENHEROES + 1; i++)
		{
			//boss is acting
			if(strcmp(h[i].name, boss->name) == 0)
			{
				//YOU WIN!
				if(boss->health == 0)
				{
					return 0;
				}
				else
				{
					
					int place = 0;
					//okay, so you need the list of heroes so the boss
					//can know who he/she is attacking, but you also need to know
					//which hero was affected (which is what place is for)
					handleBossAction(boss, h, currentList->decision, &place);
					int d;
					//so the place is the hero index that got affected, so 
					//update the old array with the health value of the new array. 
					for(d = 0; d < NUMCHOSENHEROES; d++)
					{
						if(strcmp(h[place].name, heroes[d].name) == 0) 
						{
							heroes[d].health = h[place].health;
							break;
						}

						//bug fix: if boss kills hero that goes before him,
						//and that hero was the last alive, boss infinitely 
						//loops trying to find a hero that's alive
						if((d == i - 1) && heroes[d].health == 0)
						{
							allHeroesDead = true;
						}

					}
				}
			}
			//hero is acting. 
			else
			{
				//the hero is dead, so no action will be taken
				if(h[i].health == 0) 
					continue;
				else
				{
					allHeroesDead = false;
					//need the boss's decision because defending causes less damage
					handleHeroAction(&h[i], boss, currentList->decision);
					int d;
					//same thing as before with the boss, have to find the hero 
					//that was affected and update the health. This is because
					//at this point, the hero could've healed. 
					for(d = 0; d < NUMCHOSENHEROES; d++)
					{
						if(strcmp(h[i].name, heroes[d].name) == 0) 
						{
							heroes[d].health = h[i].health;
							break;
						}
					}

				}
			}
		}
		//the boss's action can't change until all the heroes have done
		//their action, since the list could've ended and caused a seg fault
		//before a hero is done. 
		currentList = currentList->next;
		printf("************************************\n\n");
	}
}

//test
int debugPrint(Hero* h)
{
	int i;
	for(i = 0; i < NUMCHOSENHEROES + 1; i++)
	{
		printf("%s -> ", h[i].name);
	}
	printf("\n");
	return 0;
}


void getHeroDecisions(Hero* heroes)
{
	int i;
	int choice;

	printf("1: Attack\n2: Defend\n3: Heal\n");
	for(i = 0; i < NUMCHOSENHEROES + 1; i++)
	{
		//hero has to be alive to get a decision for him/her
		if(heroes[i].health > 0)
		{
			printf("Acting in position %d: %s.\n", i+1, heroes[i].name); 
			printf("Choose your action for %s:\n>", heroes[i].name);
			scanf("%d", &choice);
			while(choice < 1 || choice > 3)
			{
				printf("Incorrect option. Choose again: ");
				scanf("%d", &choice);
			}
			heroes[i].decision = choice - 1;
		}
	}
}

//I'm not good with printf formatting. There is so much you can do with this. 
//e.g.: there are colors you can do. 
void printFightStatus(Boss* boss, Hero* heroes)
{
	printf("\n\n***************THE BATTLEFIELD*************\n\n");
	printf("%-40s\nHealth: %d\n\n", heroes[0].name, heroes[0].health);
	printf("%-20s\t\t\t%-10s\nHealth: %d\t\t\t\tHealth: %d\n\n", heroes[1].name,	
		boss->name, heroes[1].health, boss->health);
	printf("%-40s\nHealth: %d\n\n", heroes[2].name, heroes[2].health);
	printf("***************************************************\n\n");
}

void handleBossAction(Boss* boss, Hero* heroes, Decision d, int* place)
{
	switch(d)
	{
		case HEAL:
			//the boss can heal for more than heroes, but can't heal to full automatically. 
			printf("%s has been healed ", boss->name);
			int x = rand()%200 + 75;
			if(boss->health + x >= boss->maxHealth)
			{
				printf("to full health!\n");
				boss->health = boss->maxHealth;
			}
			else
			{
				printf("for %dhp!\n", x);
				boss->health += x;
			}
			break;
		case ATTACK:
			printf("%s attacks! ", boss->name);
			//This is where things start to get weird again. 
			//I have to find a hero that is alive for the boss
			//to attack, but the array has the boss in it. 
			//So not only do you have to figure out if you randomed the 
			//boss, but you have to find the hero since some of 
			//the heroes are offsetted. 
			while(1)
			{
				x = rand()%NUMCHOSENHEROES;
				int i;
				for(i = 0; i <= x; i++)
				{
					if(strcmp(heroes[i].name, boss->name) == 0) break;
				}
				if(i != x + 1)
				{
					x += 1;
				}
				
				if(heroes[x].health != 0) break;
			}
			*place = x;

			int dmg; 
			//flag to see if the hero is defending or not.
			int t = 0;
			if(heroes[x].decision != DEFEND) t = 1;
			//if the hero has a shield, take its defense into account. If the hero is not defending, add extra dmg.  
			if(heroes[x].heroShield != NULL)
				dmg = boss->baseAttack - rand()%heroes[x].heroShield->defense - rand()%heroes[x].baseDefense + (t*rand()%heroes[x].baseDefense);
			else
				dmg = boss->baseAttack - rand()%heroes[x].baseDefense + (t*rand()%heroes[x].baseDefense);
			if(dmg < 0) dmg = 0;
			printf("%s takes %d damage.\n", heroes[x].name, dmg);
			heroes[x].health -= dmg;
			if(heroes[x].health < 0) heroes[x].health = 0;
			break;
		case DEFEND:	
			printf("%s is defending.\n", boss->name);
	}
}

void handleHeroAction(Hero* hero, Boss* boss, Decision decision)
{
	switch(hero->decision)
	{
		case HEAL:
			//The hero is set up so that it can be healed to full health once. 
			//Then, the hero heals for a random chunk each other heal. 
			if(hero->hasHealed)
			{
				int x = rand()%100 + 50;
				if(hero->health + x >= hero->maxHealth)
				{
					printf("%s has been healed to full health!\n", hero->name);
					hero->health = hero->maxHealth;
				}
				else
				{
					printf("%s has been healed for %dhp!\n", hero->name, x);
					hero->health += x;
				}
			}
			else
			{
				hero->hasHealed = true;
				printf("%s's exilir has been used, healing to full health!\n", hero->name);
				hero->health = hero->maxHealth;
			}
			break;
		case ATTACK:
			printf("%s attacks! ", hero->name);
			int x;
			//flag for if the boss is defending. 
			int t = 0;
			if(decision != DEFEND) t = 1;
			//if the hero has a sword, take it into account. If the boss is not defending, add extra dmg. 
			if(hero->heroSword != NULL)
				x = hero->baseAttack + rand()%hero->heroSword->attack - rand()%boss->baseDefense
					+ (t*rand()%boss->baseDefense);
			else
				x = hero->baseAttack - rand()%boss->baseDefense + (t*rand()%boss->baseDefense);
			if(x < 0) x = 0;
			printf("%s takes %d damage.\n", boss->name, x);
			boss->health -= x;
			if(boss->health < 0) boss->health = 0;
			break;
		case DEFEND:	
			printf("%s is defending.\n", hero->name);
			break;
		default:
			printf("What?\n");
			//what?
	}
}

//sort by speed, descending (fastest goes first)
void sort(Hero* heroes)
{
	int i, j;
	for(i = 0; i < NUMCHOSENHEROES + 1; i++)
	{
		for(j = 0; j < NUMCHOSENHEROES + 1; j++)
		{
			if(heroes[j].baseSpeed > heroes[j-1].baseSpeed)
			{
				Hero temp = heroes[j];
				heroes[j] = heroes[j-1];
				heroes[j-1] = temp;
			}
		}
	}
}
