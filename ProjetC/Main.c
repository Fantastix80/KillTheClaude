#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Player.h"
#include "Attack.h"
#include "Type.h"
#include "Monstre.h"
#include "HealthKit.h"

// Définition des variables globales
#define MAX_PLAYERS 1
#define MAX_ATTACKS 2
#define MAX_TYPES 2
#define MAX_MONSTRES 1
#define MAX_HEALTH_KITS 1
#define HAUTEUR_MAP 20
#define LARGEUR_MAP 40


// Définition des fonctions
void StartGame();
void afficheMap();
void CheckPlayerOutOfBounds();
void MakePlayerMove();
void ClearScreen();
void CheckIfCollision();
void Fight();
void Attack(int AttackNumber);
void EndFight(bool PlayerWon);
void CreateMonster();
int GetRandomInt(int MinInt, int MaxInt);
void CreateSafePosition(char* Type);
void SpawnHealthKit();
void HealPlayer();
int CheckTypeSensivity(int AttackNumber);

// Définition de la partie
extern struct Game
{
	struct Player* ListeJoueurs[MAX_PLAYERS];
	struct Attack* ListeAttaques[MAX_ATTACKS];
	struct Type* ListeTypes[MAX_TYPES];
	struct Monstre* ListeMonstres[MAX_MONSTRES];
	struct HealthKit* ListeHealthKits[MAX_HEALTH_KITS];
	int Score;
};

struct Game Game = { {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, 0 };

int main()
{

	// Création de notre premier type et ajout de celui-ci à la partie
	struct Type Sol = { "Sol", {NULL}, 1.5, {NULL}, 0.5 };
	struct Type Air = { "Air", {NULL}, 1.5, {&Sol}, 0.5 };

	Air.TypeVulnerable[0] = &Air;

	Game.ListeTypes[0] = &Sol;
	Game.ListeTypes[1] = &Air;

	// Création de notre première attaque et ajout de celle-ci à la partie
	struct Attack Melee = {"Melee", 50.0, Game.ListeTypes[0] };
	struct Attack LameAir = {"Lame d'air", 30.0, Game.ListeTypes[1] };
	Game.ListeAttaques[0] = &Melee;
	Game.ListeAttaques[1] = &LameAir;

	// Création de notre premier Joueur et ajout de celui-ci à la partie
	struct Player Player1 = { "Jean", 100.0, {Game.ListeAttaques[0], Game.ListeAttaques[1]}, 1, 1};
	Game.ListeJoueurs[0] = &Player1;

	// Création de notre premier Monstre et ajout de celui-ci à la partie
	CreateMonster();
	
	StartGame();

	return 0;
}

void StartGame()
{
	while (1)
	{
		// On vient nettoyer l'écran
		ClearScreen();

		printf("Score: %d\n", Game.Score);
		printf("HP: %.2f\n\n", Game.ListeJoueurs[0]->Health);
		afficheMap();

		// Vérifier que le joueur ne rentre pas en combat
		CheckIfCollision();

		// On vient faire jouer le joueur

		MakePlayerMove();

		// On vient nettoyer l'écran
		ClearScreen();
	}
}

void afficheMap()
{
	char tableau[HAUTEUR_MAP][LARGEUR_MAP];

	// On vient remplir la map
	for (int i = 0; i < HAUTEUR_MAP; i++)
	{
		for (int j = 0; j < LARGEUR_MAP; j++)
		{
			if (i == 0 || j == 0 || i == HAUTEUR_MAP - 1 || j == LARGEUR_MAP - 1)
			{
				tableau[i][j] = '*';
			}
			else
			{
				tableau[i][j] = '-';
			}
		}
	}

	// On vient placer le joueur sur la map
	if (Game.ListeJoueurs[0])
	{
		tableau[Game.ListeJoueurs[0]->Y][Game.ListeJoueurs[0]->X] = 'J';
	}

	// On vient placer le monstre sur la map
	if (Game.ListeMonstres[0])
	{
		tableau[Game.ListeMonstres[0]->Y][Game.ListeMonstres[0]->X] = 'M';
	}

	// On vient afficher le kit de soin sur la map
	if (Game.ListeHealthKits[0])
	{
		tableau[Game.ListeHealthKits[0]->Y][Game.ListeHealthKits[0]->X] = 'S';
	}

	// On vient afficher la map
	for (int i = 0; i < HAUTEUR_MAP; i++)
	{
		for (int j = 0; j < LARGEUR_MAP; j++)
		{
			printf("%c", tableau[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void CheckPlayerOutOfBounds()
{
	if (Game.ListeJoueurs[0])
	{
		if (Game.ListeJoueurs[0]->Y == (HAUTEUR_MAP - HAUTEUR_MAP))
		{
			Game.ListeJoueurs[0]->Y = HAUTEUR_MAP - 2;
		}
		else if (Game.ListeJoueurs[0]->Y == HAUTEUR_MAP - 1)
		{
			Game.ListeJoueurs[0]->Y = (HAUTEUR_MAP - HAUTEUR_MAP) + 1;
		}

		if (Game.ListeJoueurs[0]->X == (LARGEUR_MAP - LARGEUR_MAP))
		{
			Game.ListeJoueurs[0]->X = LARGEUR_MAP - 2;
		}
		else if (Game.ListeJoueurs[0]->X == LARGEUR_MAP - 1)
		{
			Game.ListeJoueurs[0]->X = (LARGEUR_MAP - LARGEUR_MAP) + 1;
		}
	}
}

void MakePlayerMove()
{
	
	// On vient attendre l'input du joueur
	char PlayerInput;
	printf("z,q,s,d pour se deplacer: ");
	while (1)
	{
		if (scanf_s("%c", &PlayerInput) == 1)
		{
			if (PlayerInput == 'z' || PlayerInput == 'q' || PlayerInput == 's' || PlayerInput == 'd')
			{
				break;
			}
		}
		else
		{
			// Nettoyer le tampon d'entrée
			while (getchar() != '\n');
		}
	}
	while (getchar() != '\n');
	if (Game.ListeJoueurs[0])
	{
		switch (PlayerInput)
		{
		case 'z':
			Game.ListeJoueurs[0]->Y--;
			CheckPlayerOutOfBounds();
			break;
		case 'q':
			Game.ListeJoueurs[0]->X--;
			CheckPlayerOutOfBounds();
			break;
		case 's':
			Game.ListeJoueurs[0]->Y++;
			CheckPlayerOutOfBounds();
			break;
		case 'd':
			Game.ListeJoueurs[0]->X++;
			CheckPlayerOutOfBounds();
			break;
		}
	}
}

void ClearScreen()
{
	system("cls");
}

void CheckIfCollision()
{
	if (Game.ListeJoueurs[0] && Game.ListeMonstres[0])
	{
		if (abs(abs(Game.ListeJoueurs[0]->X) - abs(Game.ListeMonstres[0]->X)) == 1 && Game.ListeJoueurs[0]->Y == Game.ListeMonstres[0]->Y)
		{
			Fight();
			return;
		}
		else if (abs(abs(Game.ListeJoueurs[0]->Y) - abs(Game.ListeMonstres[0]->Y)) == 1 && Game.ListeJoueurs[0]->X == Game.ListeMonstres[0]->X)
		{
			Fight();
			return;
		}
	}
	if (Game.ListeJoueurs[0] && Game.ListeHealthKits[0])
	{
		if (abs(abs(Game.ListeJoueurs[0]->X) - abs(Game.ListeHealthKits[0]->X)) == 1 && Game.ListeJoueurs[0]->Y == Game.ListeHealthKits[0]->Y)
		{
			HealPlayer();
			return;
		}
		else if (abs(abs(Game.ListeJoueurs[0]->Y) - abs(Game.ListeHealthKits[0]->Y)) == 1 && Game.ListeJoueurs[0]->X == Game.ListeHealthKits[0]->X)
		{
			HealPlayer();
			return;
		}
	}
}

void Fight()
{
	if (Game.ListeJoueurs[0] && Game.ListeMonstres[0])
	{
		while (1)
		{
			ClearScreen();

			// On vient afficher toutes les informations concernant le combat
			printf("%s\t\t\tVS\t\t\t%s\n", Game.ListeJoueurs[0]->Name, Game.ListeMonstres[0]->Name);
			printf("HP: %.2f\t\t\t\t\t%.2f : HP\n", Game.ListeJoueurs[0]->Health, Game.ListeMonstres[0]->Health);
			printf("\t\t\t\t\t\t%s : Type\n\n", Game.ListeMonstres[0]->Type->Name);
			printf("Vos attaques:\n\n");

			// On vient afficher les attaques disponibles du joueur
			for (int i = 0; i < sizeof(Game.ListeJoueurs[0]->PlayerAttack) / sizeof(Game.ListeJoueurs[0]->PlayerAttack[0]); i++)
			{
				if (Game.ListeJoueurs[0]->PlayerAttack[i])
				{
					printf("\t%d: %s (%.2f, %s)\n", i + 1, Game.ListeJoueurs[0]->PlayerAttack[i]->Name, Game.ListeJoueurs[0]->PlayerAttack[i]->Damage, Game.ListeJoueurs[0]->PlayerAttack[i]->Type);
				}
			}
			printf("\nVeuillez selectionner une attaque: ");

			// On vient écouter l'input du joueur
			int PlayerInput;
			while (1)
			{
				if (scanf_s("%d", &PlayerInput) == 1)
				{
					if (PlayerInput == 1 || PlayerInput == 2)
					{
						if (Game.ListeJoueurs[0]->PlayerAttack[PlayerInput - 1])
						{
							break;
						}
					}
				}
				else
				{
					while (getchar() != '\n');
				}
			}

			// On vient lancer l'attaque sélectionné
			Attack(PlayerInput - 1);
		}
	}
}

void Attack(int AttackNumber)
{
	if (Game.ListeJoueurs[0] && Game.ListeMonstres[0])
	{
		// On vient faire attaquer le joueur
		switch (CheckTypeSensivity(AttackNumber))
		{
		case 0:
			Game.ListeMonstres[0]->Health -= Game.ListeJoueurs[0]->PlayerAttack[AttackNumber]->Damage * Game.ListeMonstres[0]->Type->MultiplierVulnerable;
			break;
		case 1:
			Game.ListeMonstres[0]->Health -= Game.ListeJoueurs[0]->PlayerAttack[AttackNumber]->Damage * Game.ListeMonstres[0]->Type->MultiplierResistant;
			break;
		case 2:
			Game.ListeMonstres[0]->Health -= Game.ListeJoueurs[0]->PlayerAttack[AttackNumber]->Damage;
		}
		

		// On vient vérifier que le monstre n'est pas mort
		if (Game.ListeMonstres[0]->Health <= 0.00)
		{
			EndFight(true);
			return;
		}

		// Si il est encore en vie, on vient le faire attaquer
		Game.ListeJoueurs[0]->Health -= Game.ListeMonstres[0]->Damage;

		// On vérifie que le joueur n'est pas mort
		if (Game.ListeJoueurs[0]->Health <= 0.00)
		{
			EndFight(false);
			return;
		}
	}
}

void EndFight(bool PlayerWon)
{
	ClearScreen();
	printf("Fin du combat\n\n");
	if (PlayerWon)
	{
		Game.Score++;
		printf("Victoire de %s", Game.ListeJoueurs[0]->Name);

		// On vient supprimer l'ennemi
		free(Game.ListeMonstres[0]);
		Game.ListeMonstres[0] = NULL;

		// On vient en faire spawn un nouveau
		CreateMonster();

		Sleep(1500);

		// On vient faire spawn un kit de soin si les conditions sont remplies
		if (Game.Score % 2 == 0)
		{
			if (!Game.ListeHealthKits[0])
			{
				SpawnHealthKit();
			}
		}

		StartGame();
	}
	else
	{
		if (Game.ListeMonstres[0])
		{
			printf("Victoire de %s\n", Game.ListeMonstres[0]->Name);
			printf("Score: %d", Game.Score);
		}
		Sleep(3000);
		exit(0);
	}
}

void CreateMonster()
{
	// On vient créer un nouveau monstre en lui allouant de la mémoire afin qu'il ne soit pas supprimé lorsqu'on sortira de la fonction
	struct Monstre* Monstre1 = (struct Monstre*)malloc(sizeof(struct Monstre));

	// On vérifie que l'allocation de mémoire a bien fonctionné
	if (Monstre1 != NULL)
	{
		strcpy(Monstre1->Name, "Claude");
		Monstre1->Health = 100.0;
		Monstre1->Damage = 20.0 * (((float)Game.Score/10) + 1);
		Monstre1->Type = Game.ListeTypes[GetRandomInt(0, 1)];

		Game.ListeMonstres[0] = Monstre1;

		CreateSafePosition("Monstre");
	}
	else
	{
		printf("Erreur lors de l'allouement de mémoire pour la création du nouveau monstre.\n");
	}
}

int GetRandomInt(int MinInt, int MaxInt)
{
	int TempInt = MaxInt - MinInt;
	int RandomInt = rand() % (TempInt + 1);
	int ResultInt = RandomInt + MinInt;

	return ResultInt;
}

void SpawnHealthKit()
{
	// On vient créer un nouveau kit de soin en lui allouant de la mémoire afin qu'il ne soit pas supprimé lorsqu'on sortira de la fonction
	struct HealthKit* HealthKit1 = (struct HealthKit*)malloc(sizeof(struct HealthKit));

	// On vérifie que l'allocation de mémoire a bien fonctionné
	if (HealthKit1 != NULL)
	{
		strcpy(HealthKit1->Name, "Kit de soin");
		HealthKit1->HealthRegain = GetRandomInt(50, 100);

		Game.ListeHealthKits[0] = HealthKit1;

		CreateSafePosition("HealthKit");
	}
	else
	{
		printf("Erreur lors de l'allouement de mémoire pour la création du nouveau monstre.\n");
	}
}

void CreateSafePosition(char* Type)
{
	int X = GetRandomInt((LARGEUR_MAP - LARGEUR_MAP) + 1, LARGEUR_MAP - 2);
	int Y = GetRandomInt((HAUTEUR_MAP - HAUTEUR_MAP) + 1, HAUTEUR_MAP - 2);

	if (Game.ListeJoueurs[0])
	{
		if (Game.ListeJoueurs[0]->X == X && Game.ListeJoueurs[0]->Y == Y)
		{
			if (Type == "HealthKit")
			{
				if (Game.ListeMonstres[0])
				{
					if (Game.ListeMonstres[0]->X == X && Game.ListeMonstres[0]->Y == Y)
					{
						CreateSafePosition("HealthKit");
					}
				}
				else
				{
					CreateSafePosition("HealthKit");
				}
			}
			else
			{
				if (Game.ListeHealthKits[0])
				{
					if (Game.ListeHealthKits[0]->X == X && Game.ListeHealthKits[0]->Y == Y)
					{
						CreateSafePosition("Monstre");
					}
				}
				else
				{
					CreateSafePosition("Monstre");
				}
			}
		}
		else
		{
			if (Type == "HealthKit")
			{
				Game.ListeHealthKits[0]->X = X;
				Game.ListeHealthKits[0]->Y = Y;
			}
			else
			{
				Game.ListeMonstres[0]->X = X;
				Game.ListeMonstres[0]->Y = Y;
			}
		}
	}
}

void HealPlayer()
{
	ClearScreen();

	Game.ListeJoueurs[0]->Health += Game.ListeHealthKits[0]->HealthRegain;
	if (Game.ListeJoueurs[0]->Health > 100.00)
	{
		Game.ListeJoueurs[0]->Health = 100.00;
	}
	printf("Vous venez de rammasser un kit de soin.\nVous avez desormais %.2f HP.", Game.ListeJoueurs[0]->Health);

	// On vient supprimer le kit de soin
	free(Game.ListeHealthKits[0]);
	Game.ListeHealthKits[0] = NULL;

	Sleep(1500);
	StartGame();
}

int CheckTypeSensivity(int AttackNumber)
{
	// return 0 si il est vulnérable
	// return 1 si il est résistant
	// return 2 si il est neutre
	for (int i = 0; i < 5; i++)
	{
		if (Game.ListeMonstres[0]->Type->TypeVulnerable[i])
		{
			if (Game.ListeMonstres[0]->Type->TypeVulnerable[i] == Game.ListeJoueurs[0]->PlayerAttack[AttackNumber]->Type)
			{
				return 0;
			}
		}
		if (Game.ListeMonstres[0]->Type->TypeResistant[i])
		{
			if (Game.ListeMonstres[0]->Type->TypeResistant[i] == Game.ListeJoueurs[0]->PlayerAttack[AttackNumber]->Type)
			{
				return 1;
			}
		}
	}

	return 2;
}
