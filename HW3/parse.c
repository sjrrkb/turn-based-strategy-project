#include "game.h"

//If you haven't knocked yourself out in boss.c, do it here.
//Otherwise, knock yourself out again.

void ParseShieldInfo(Shield* s, FILE* fp)
{
	if(fp==NULL)
	{
		puts("sword info could not be opened");
		exit(0);
	}
	char line[MAXFILELINELENGTH];
	char name[50]; //making buffers here
	char cost[50];
	char defense[50];
	char description[100];
	fgets(line,MAXFILELINELENGTH,fp); //grabbing the line of the file
	sscanf(line,"%[^|]|%[^|]|%[^|]|%[^\n]\n",name,cost,defense,description); //so found out about this way of making delimiters with sscanf...
	strcpy(s->name,name);													 //its quite handy if the pattern of your file is unchanging
	s->cost=atoi(cost);
	s->defense=atoi(defense);
	strcpy(s->description,description);
}

void ParseSwordInfo(Sword* s, FILE* fp)//basically just a copy of above
{
	if(fp==NULL)
	{
		puts("sword info could not be opened");
		exit(0);
	}
	char line[MAXFILELINELENGTH];
	char name[50];
	char cost[50];
	char attack[50];
	char description[100];
	fgets(line,MAXFILELINELENGTH,fp);
	sscanf(line,"%[^|]|%[^|]|%[^|]|%[^\n]\n",name,cost,attack,description);
	strcpy(s->name,name);
	s->cost=atoi(cost);
	s->attack=atoi(attack);
	strcpy(s->description,description);
}

