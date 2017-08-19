//don't add to this list. You won't need anything else.
//add to it as you wish if you are here after HW3 has been due
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
//for expandable use.
#define NUMHEROES 6
#define NUMCHOSENHEROES 3
#define MAXNUMSAVEFILES 5
//length of username
#define MAXPLAYERNAME 20
#define NUMSWORDS 10
#define NUMSHIELDS 8
//For the item shop
#define STARTINGBANKVALUE 53000
//The longest line that will be in an input file
#define MAXFILELINELENGTH 150

//C is weird. This is in C99 but we aren't using C99.
//Feel free to compile with C99 later if you want.
typedef enum b_
{
	false,
	true
} bool;

//Used to locate a saved game file
typedef enum f_
{
	EXIST,
	DOESNOTEXIST
} FileFinder;

//Types of actions to take during the fight
typedef enum a_
{
	ATTACK,
	DEFEND,
	HEAL
} Decision;

//Items. Can be expanded to accommodate special stuff
//like block chance, etc.
typedef struct shield_
{
	char name[50];
	int cost;
	int defense;
	char description[100];
	struct shield_* nextShield;
	struct shield_* prevShield;
} Shield;

typedef struct sword_
{
	char name[50];
	int cost;
	int attack;
	char description[100];
	struct sword_* nextSword;
	struct sword_* prevSword;
} Sword;

//Can be expanded to have a status, etc
typedef struct hero_
{
	char name[40];
	Sword* heroSword;
	Shield* heroShield;
	Decision decision;
	int health;
	int maxHealth;
	bool hasHealed;
	int baseDefense;
	int baseAttack;
	int baseSpeed;
} Hero;

//Linked list node in the linked list that is connected
//to a tree node in the boss decision tree
typedef struct aNode_
{
	Decision decision;
	struct aNode_* next;
} ActionNode;

//boss decision tree node. Has the link to the head of the
//action linked list
typedef struct dTree_
{
	int healthFloor;
	int healthCeiling;
	struct dTree_* left;
	struct dTree_* right;
	ActionNode* FirstAction;
} DecisionTreeNode;

//can be expanded similar to heroes.
typedef struct boss_
{
	char name[40];
	int health;
	int maxHealth;
	int baseDefense;
	int baseAttack;
	int baseSpeed;
	DecisionTreeNode* root;
} Boss;

//wrapper struct
typedef struct shop_
{
	Sword* headSword;
	Shield* headShield;
} Shop;


void deleteFile(int choice, char* filename);
void saveFileData(int choice, char* filename, Hero* heroes, char* playerName, int progress);
char* getPlayerName();
Hero* loadSavedData(char* filename, int haveItems);
Hero* loadInitialData();
Hero* pickHeroes(Hero* heroes);
void sortHeroes(Hero* heroes);
void printHeroes(Hero* heroes, int numHeroes);

FileFinder findSaveFile();
void loadSavedGameHandler();
void DeleteSavedGameHandler();
void loadGameStateData(char*** names, int** states, int* fileCount);
char* choiceToFile(int num);
void freeMemory(char** names, int* states);
void startNewGame();
void playGame(Hero* heroes, char* playerName, Boss* boss, int progress, int choice);
int chooseSaveFile();


Shop* loadShop();
void destroyShop(Shop* shop);
void goShopping(Hero* heroes, Shop* shop);

void printShields(Shield* shields);
void printSwords(Sword* swords);

Sword* removeSwordFromList(Sword** swords, int choice);
Shield* removeShieldFromList(Shield** shields, int choice);

void freeHeroesAndItems(Hero* heroes);

void sortShields(Shield**);
void sortSwords(Sword**);

int findSwordPrice(Sword*, int);
int findShieldPrice(Shield*, int);


//HW3 functions
Boss* loadBoss();
ActionNode* addActionToList(ActionNode*, ActionNode*);
DecisionTreeNode* addNodeToTree(DecisionTreeNode*, DecisionTreeNode*);
ActionNode* fetchNewList(Boss*, DecisionTreeNode*);
void freeBossTree(DecisionTreeNode*);
void freeActionListInNode(ActionNode*);

void sort(Hero*);

int fight(Boss*, Hero*);
void getHeroDecisions(Hero*);
void printFightStatus(Boss*, Hero*);
void handleBossAction(Boss*, Hero*, Decision, int*);
void handleHeroAction(Hero*, Boss*, Decision);

void ParseShieldInfo(Shield*, FILE*);
void ParseSwordInfo(Sword*, FILE*);


//Extra
int debugPrint(Hero*);
