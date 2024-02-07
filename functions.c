#include "functions.h"

// Loads the game map based on the selected level
mapinfo load(int level)
{
    FILE *mapFile;
    mapinfo Map;
    char temp;
    int x = 0, y = 0;

    // Open the file corresponding to the selected level
    switch (level)
    {
    case 1:
        mapFile = fopen("mapA.txt", "r");
        break;
    case 2:
        mapFile = fopen("mapB.txt", "r");
        break;
    default:
        mapFile = fopen("mapC.txt", "r");
        break;
    }

    // Read the dimensions of the map
    fscanf(mapFile, "%d %d", &Map.x, &Map.y);

    // Allocate memory for the map
    Map.map = (char **)calloc(Map.x, sizeof(char *));
    for (int i = 0; i < Map.x; i++)
    {
        Map.map[i] = (char *)calloc(Map.y + 1, sizeof(char));
    }

    // Read the characters of the map
    while (fscanf(mapFile, " %c", &temp) == 1)
    {
        if (temp == '\n' || temp == ' ')
        {
            continue;
        }

        Map.map[x][y] = temp;

        if (y == Map.y - 1)
        {
            x++;
            y = 0;
        }
        else
        {
            y++;
        }
    }

    // Close the file
    fclose(mapFile);

    // Return the loaded map
    return Map;
}

// Loads player account information and the game map state
account loadAcc(char *id, mapinfo *map, int *level)
{
    FILE *file = NULL, *gameFile = NULL;
    char *fileName = (char *)calloc(strlen(id) + 5, sizeof(char));
    char *LastGame = (char *)calloc(strlen(id) + 13, sizeof(char));
    account player;

    // Create file names for player data and last game of the player
    strcpy(fileName, id);
    strcpy(LastGame, id);
    strcat(fileName, ".txt");
    strcat(LastGame, "LastGame.txt");

    // Open the player data file
    file = fopen(fileName, "r");

    // Check if the file exists
    if (file == NULL)
    {
        player.lastGame = 2; // Player not found
        fclose(file);
        free(LastGame);
        return player;
    }

    // Open the last game of the player file
    gameFile = fopen(LastGame, "r");

    // Read the dimensions of the map and determine the game level
    fscanf(gameFile, "%d %d", &map->x, &map->y);
    switch (map->x)
    {
    case 5:
        *level = 1;
        break;
    case 8:
        *level = 2;
        break;
    default:
        *level = 3;
        break;
    }

    // Allocate memory for the map
    map->map = (char **)calloc(map->x, sizeof(char *));
    for (int i = 0; i < map->x + 1; i++)
    {
        map->map[i] = (char *)calloc(map->y + 1, sizeof(char));
    }

    // Read the characters of the map from the last game of the player file
    for (int i = 0; i < map->x; i++)
    {
        for (int j = 0; j < map->y; j++)
        {
            fscanf(gameFile, " %c", &map->map[i][j]);
        }
    }

    // Read player data from the last game of the player file
    fscanf(file, "%s %d %d %d %s %d %d", player.id, &player.level, &player.lastGame, &player.score, player.filename, &player.password, &player.valid);

    // Check if the player account is valid
    if (player.valid == 0)
    {
        printf("NO ACCOUNT WAS FOUND WITH THIS ID.\n");
        fclose(file);
        fclose(gameFile);
        free(LastGame);
        exit(0);
    }

    // Close the files
    fclose(file);
    fclose(gameFile);

    // Free allocated memory
    free(LastGame);

    // Return the loaded player data
    return player;
}

// Play the game for level A (single ghost)
void playA(mapinfo *map, account *player, int *tme)
{
    coordinate pacman, ghost, pacmanBef, ghostBef;
    int move;
    int GhostMove;

    // find the initial positions of pacman and the ghost
    for (int i = 0; i < map->x; i++)
    {
        for (int j = 0; j < map->y; j++)
        {
            if (map->map[i][j] == '@')
            {
                pacman.i = i;
                pacman.j = j;
            }
            else if (map->map[i][j] == 'F')
            {
                ghost.i = i;
                ghost.j = j;
            }
        }
    }
    ghostBef = ghost;
    pacmanBef = pacman;

    // Move the ghost if the time condition is correct
    if (*tme == 6e5)
    {
        while (1)
        {
            GhostMove = (rand() % 4);
            switch (GhostMove)
            {
            case 1:
                ghost.j++;
                break;
            case 2:
                ghost.j--;
                break;
            case 3:
                ghost.i--;
                break;
            case 0:
                ghost.i++;
                break;
            }
            if (map->map[ghost.i][ghost.j] == '|' || map->map[ghost.i][ghost.j] == '-' || map->map[ghost.i][ghost.j] == 'P')
            {
                ghost = ghostBef;
                continue;
            }
            else
            {
                map->map[ghostBef.i][ghostBef.j] = '.';
                map->map[ghost.i][ghost.j] = 'F';
                break;
            }
        }
    }

    // Wait for user input
    while (1)
    {
        move = getch();

        if (move != '\n' || move == ERR)
            break;
    }

    // Update pacman's position based on user input
    if (move == 'w')
        pacman.i--;
    else if (move == 's')
        pacman.i++;
    else if (move == 'a')
        pacman.j--;
    else if (move == 'd')
        pacman.j++;
    else if (move == 'p')
    {
        player->lastGame = 0; // Pause the game
        return;
    }

    if (map->map[pacman.i][pacman.j] == '|' || map->map[pacman.i][pacman.j] == '-')
    {
        pacman = pacmanBef;
        return;
    }
    else if (map->map[pacman.i][pacman.j] == 'F')
    {
        usleep(3e6);
        player->lastGame = 1; // Game over
        player->level--;
        printf("GAME OVER!");
        usleep(3e6);
        return;
    }
    else if (map->map[pacman.i][pacman.j] == 'P')
    {
        player->lastGame = 1;
        player->level += 3;
        player->score += 1;
        system("clear");
        printf("YOU WON!\n");
        usleep(2e6);
        return;
    }
    else if (map->map[pacmanBef.i][pacmanBef.j] == '@')
    {
        map->map[pacmanBef.i][pacmanBef.j] = '.';
        map->map[pacman.i][pacman.j] = '@';
        return;
    }
    else
    {
        map->map[pacman.i][pacman.j] = '@';
        return;
    }
}

// Play the game for level B & C (two ghosts)
void playB(mapinfo *map, account *player, int gameLevel, int *tme)
{
    coordinate pacman, ghost, ghost2, pacmanBef, ghostBef, ghost2Bef;
    int move;
    int GhostMove, flag = 0;

    // find the initial positions of pacman and two ghosts
    for (int i = 0; i < map->x; i++)
    {
        for (int j = 0; j < map->y; j++)
        {
            if (map->map[i][j] == '@')
            {
                pacman.i = i;
                pacman.j = j;
            }
            else if (map->map[i][j] == 'F')
            {
                if (flag == 0)
                {
                    ghost.i = i;
                    ghost.j = j;
                    flag = 1;
                }
                else
                {
                    ghost2.i = i;
                    ghost2.j = j;
                }
            }
        }
    }
    ghost2Bef = ghost2;
    ghostBef = ghost;
    pacmanBef = pacman;

    // Move the ghosts if the time condition is correct
    if (*tme == 6e5)
    {
        while (1)
        {
            GhostMove = (rand() % 4);
            switch (GhostMove)
            {
            case 1:
                ghost2.j++;
                break;
            case 2:
                ghost2.j--;
                break;
            case 3:
                ghost2.i--;
                break;
            default:
                ghost2.i++;
                break;
            }
            GhostMove = (rand() % 4);
            switch (GhostMove)
            {
            case 1:
                ghost.j++;
                break;
            case 2:
                ghost.j--;
                break;
            case 3:
                ghost.i--;
                break;
            case 0:
                ghost.i++;
                break;
            }
            if (map->map[ghost.i][ghost.j] == '|' || map->map[ghost.i][ghost.j] == '-' || map->map[ghost2.i][ghost2.j] == '|' || map->map[ghost2.i][ghost2.j] == '-' || (ghost.i == ghost2.i && ghost.j == ghost2.j) || map->map[ghost.i][ghost.j] == 'P' || map->map[ghost2.i][ghost2.j] == 'P')
            {
                ghost = ghostBef;
                ghost2 = ghost2Bef;
                continue;
            }
            else
            {
                map->map[ghostBef.i][ghostBef.j] = '.';
                map->map[ghost.i][ghost.j] = 'F';
                map->map[ghost2Bef.i][ghost2Bef.j] = '.';
                map->map[ghost2.i][ghost2.j] = 'F';
                break;
            }
        }
    }

    // Wait for user input
    while (1)
    {
        move = getch();
        if (move != '\n' || move == ERR)
            break;
    }

    // Update pacman's position based on user input
    if (move == 'w')
        pacman.i--;
    else if (move == 's')
        pacman.i++;
    else if (move == 'a')
        pacman.j--;
    else if (move == 'd')
        pacman.j++;
    else if (move == 'p')
    {
        player->lastGame = 0; // Pause the game
        return;
    }

    if (map->map[pacman.i][pacman.j] == '|' || map->map[pacman.i][pacman.j] == '-')
    {
        pacman = pacmanBef;
        return;
    }
    else if (map->map[pacman.i][pacman.j] == 'F')
    {
        player->lastGame = 1; // Game over
        player->level--;
        system("clear");
        printf("GAME OVER!\n");
        usleep(2e6);
        system("clear");
        if (map->map[pacmanBef.i][pacmanBef.j] == '@')
        {
            map->map[pacmanBef.i][pacmanBef.j] = '.';
            map->map[pacman.i][pacman.j] = '@';
            return;
        }
        else
        {
            map->map[pacman.i][pacman.j] = '@';
            return;
        }
        return;
    }
    else if (map->map[pacman.i][pacman.j] == 'P')
    {
        if (map->map[pacmanBef.i][pacmanBef.j] == '@')
        {
            map->map[pacmanBef.i][pacmanBef.j] = '.';
            map->map[pacman.i][pacman.j] = '@';
        }
        else
        {
            map->map[pacman.i][pacman.j] = '@';
        }
        player->score++;

        // Check if the player won the game
        if (gameLevel == 2)
        {
            if (player->score == 2)
            {
                player->lastGame = 1;
                player->level += 3;
                system("clear");
                printf("YOU WON!\n");
                usleep(2e6);
            }
        }
        else
        {
            if (player->score == 3)
            {
                player->lastGame = 1;
                player->level += 3;
                system("clear");
                printf("YOU WON!\n");
                usleep(2e6);
            }
        }
        return;
    }
    else if (map->map[pacmanBef.i][pacmanBef.j] == '@')
    {
        map->map[pacmanBef.i][pacmanBef.j] = '.';
        map->map[pacman.i][pacman.j] = '@';
        return;
    }
    else
    {
        map->map[pacman.i][pacman.j] = '@';
        return;
    }
}

// Print the current game state, including the player's score, id, and the map
void print(mapinfo *map, account *player)
{
    printf(" 🍒 = %d   %s\n\n", player->score, player->id);
    for (int i = 0; i < map->x; i++)
    {
        for (int j = 0; j < map->y; j++)
        {
            // Print different characters based on map elements
            if (map->map[i][j] == '|' || map->map[i][j] == '-')
                printf("🪨 ");
            else if (map->map[i][j] == 'F')
                printf("👻");
            else if (map->map[i][j] == 'P')
                printf("🍒");
            else if (map->map[i][j] == '@')
                printf("👾");
            else if (map->map[i][j] == '.')
                printf("  ");
        }
        printf("\n");
    }
    printf("please press p to pause.\n");
}

// Store the current game state and player information to files
void store(mapinfo *map, account *player)
{
    FILE *file = NULL, *gameFile = NULL;
    char *name, *LastGame;

    // Check if the player account is valid
    if (player->valid == 0)
    {
        return;
    }

    // Allocate memory for file names
    LastGame = (char *)calloc(strlen(player->id) + 13, sizeof(char));
    name = (char *)calloc(strlen(player->id) + 5, sizeof(char));

    // Create file names based on the player's id
    strcpy(name, player->id);
    strcpy(LastGame, player->id);
    strcat(name, ".txt");
    strcat(LastGame, "LastGame.txt");

    // Open files for writing
    file = fopen(name, "w");
    gameFile = fopen(LastGame, "w");

    // Update the player's filename
    strcpy(player->filename, LastGame);

    // Write map dimensions and elements to the gameFile
    fprintf(gameFile, "%d %d", map->x, map->y);
    for (int i = 0; i < map->x; i++)
        for (int j = 0; j < map->y; j++)
        {
            fprintf(gameFile, "%c", map->map[i][j]);
        }

    // Write player information to the file
    fprintf(file, "%s %d %d %d %s %d %d", player->id, player->level, player->lastGame, player->score, player->filename, player->password, player->valid);

    // Close files
    fclose(file);
    fclose(gameFile);

    // Free allocated memory
    free(name);
    free(LastGame);
}
