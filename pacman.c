#include "functions.h"

int main()
{
    // changes the input system ;
    // Initialize termios structures for managing terminal settings
    struct termios old_settings, new_settings;
    tcgetattr(STDIN_FILENO, &old_settings);
    new_settings = old_settings;
    new_settings.c_lflag &= (!ICANON & !ECHO); /*you don't see what char you have entered and every char will be processed at the time it has pressed*/

    // Initialize player, map, and other variables
    account player, temp;
    mapinfo map;
    int level, command, password, tme;
    player.valid = 1;

    // Main game loop
    while (1)
    {
        system("clear");

        // Display the main menu
        player.level = 0;
        printf(GREEN);
        printf("*WELCOME TO PACMAN!*\n\nPLEASE SELECT THE OPTION YOU WANT\n\n1.SIGN UP\n2.LOGIN\n3.DELETE ACCOUNT\n4.EXIT\n");
        scanf("%d", &command);
        system("clear");

        // Get player ID
        printf(CYAN);
        printf("PLEASE ENTER YOUR ID.\n");
        scanf("%s", player.id);
        system("clear");

        // Process the selected command
        switch (command)
        {
        case 1:
            // Sign up
            while (1)
            {
                temp = loadAcc(player.id, &map, &level);
                if (temp.lastGame != 2)
                {
                    printf(RED);
                    printf("THIS ID IS ALREADY TAKEN!\nPLEASE ENTER YOUR ID AGAIN.\n");
                    scanf("%s", player.id);
                    system("clear");
                }
                else
                {
                    break;
                }
            }
            printf(MAGENTA);
            printf("PLEASE ENTER YOUR PASSWORD.IT MUST BE 4 NUMBERS\n");
            scanf("%d", &password);
            system("clear");
            player.password = password;
            break;

        case 2:
            // Login
            printf(MAGENTA);
            printf("PLEASE ENTER YOUR PASSWORD.IT MUST BE 4 NUMBERS\n");
            scanf("%d", &password);
            system("clear");
            player = loadAcc(player.id, &map, &level);
            if (player.lastGame == 2)
            {
                printf("User not Found!\n");
                continue;
            }
            while (1)
            {
                if (player.password == password)
                {
                    break;
                }
                else
                {
                    printf(RED);
                    printf("YOUR PASSWORD IS NOT CORRECT!\nPLEASE ENTER YOUR PASSWORD AGAIN\n");
                    scanf("%d", &password);
                    system("clear");
                }
            }
            break;

        case 3:
            // Delete account
            printf(MAGENTA);
            printf("PLEASE ENTER YOUR PASSWORD.IT MUST BE 4 NUMBERS\n");
            scanf("%d", &password);
            system("clear");
            printf(RED);
            printf("ACCOUNT DELETED SUCCESSFULLY.\n");
            player = loadAcc(player.id, &map, &level);
            while (1)
            {
                if (player.password == password)
                {
                    printf("Password was correct\n");
                    break;
                }
                else
                {
                    printf("YOUR PASSWORD IS NOT CORRECT!\nPLEASE ENTER YOUR PASSWORD AGAIN\n");
                    scanf("%d", &password);
                    system("clear");
                }
            }
            player.valid = 0;
            store(&map, &player);
            printf(" -- %d\n", player.valid);
            continue;
            break;

        default:
            // Exit the game
            exit(0);
        }

        // Player options after login/signup
        printf(GREEN);
        printf("PLEASE SELECT THE OPTION YOU WANT\n\n1.PLAY NEW GAME\n2.RESTORE THE LAST GAME\n");
        scanf("%d", &command);
        system("clear");

        switch (command)
        {
        case 1:
            // Play a new game
            player.score = 0;
            printf(YELLOW);
            printf("SELECT THE LEVEL GAME.\n1.EASY\n2.MEDIUM\n3.HARD\n");
            scanf("%d", &level);
            map = load(level);
            break;

        default:
            // Restore the last game
            player = loadAcc(player.id, &map, &level);
            if (player.lastGame != 0)
            {
                if (player.lastGame == 1)
                {
                    printf("YOUR LAST GAME IS COMPLETED.\n");
                }
                else
                {
                    printf("THERE IS NO LAST GAME FOR YOU.\n");
                }
                printf("SELECT THE LEVEL GAME.\n1.EASY\n2.MEDIUM\n3.HARD\n");
                scanf("%d", &level);
                map = load(level);
            }
        }

        player.lastGame = 2;
        tme = 0;

        // Game loop based on selected level
        switch (level)
        {
        case 1:
            while (1)
            {
                // changes the input system ;
                printf(BLUE);
                struct termios old_settings, new_settings;
                tcgetattr(STDIN_FILENO, &old_settings);
                new_settings = old_settings;
                new_settings.c_lflag &= (!ICANON & !ECHO);       // you don't see what char you have entered and every char will be processed at the time it has pressed
                tcsetattr(STDIN_FILENO, TCSANOW, &new_settings); // changes setting to new one

                print(&map, &player);                            // prints the board
                initscr();                                       // enters curses mode
                nodelay(stdscr, TRUE);                           // runs every function one after another without any delay
                timeout(0);                                      // doesn't block user input
                playA(&map, &player, &tme);                      // enters the game board for the next moves
                tcsetattr(STDIN_FILENO, TCSANOW, &old_settings); // changes setting to old one
                endwin();                                        // ends curses mode

                if (player.lastGame == 1)
                {
                    store(&map, &player);
                    break;
                }
                else if (player.lastGame == 0)
                {
                    printf("IF YOU WANT TO SAVE THE GAME PRESS 1 Press any other key to resume\n");
                    scanf("%d", &command); 
                    if (command == 1)
                    {
                        store(&map, &player);
                        exit(0);
                    }
                    else {
                        player.lastGame = 2; 
                        
                    }
                }

                if (tme == 6e5)
                {
                    tme = 0;
                }
                tme += 2e4;  // Increment time
                usleep(2e4); // Sleep for 20 milliseconds
                system("clear");
            }
            tcsetattr(STDIN_FILENO, TCSANOW, &old_settings); // changes setting to old one
            break;

        default:
            while (1)
            {
                tcsetattr(STDIN_FILENO, TCSANOW, &new_settings); // changes setting to new one

                print(&map, &player);
                initscr();             // enters curses mode
                nodelay(stdscr, TRUE); // runs every function one after another without any delay
                timeout(0);            // doesn't block user input
                playB(&map, &player, level, &tme);
                tcsetattr(STDIN_FILENO, TCSANOW, &old_settings); // changes setting to old one
                endwin();                                        // ends curses mode

                if (player.lastGame == 1)
                {
                    store(&map, &player);
                    break;
                }
                else if (player.lastGame == 0)
                {
                    printf("IF YOU WANT TO SAVE THE GAME PRESS 1 Press any other key to resume.\n");
                    // command = getc(stdin); 
                    scanf("%d", &command); 
                    if (command == 1)
                    {
                        store(&map, &player);
                        exit(0);
                    }
                    else {
                        player.lastGame = 2; 
                        // continue; 
                    }
                }

                if (tme == 8e5)
                {
                    tme = 0;
                }
                tme += 2e4;  // Increment time
                usleep(2e4); // Sleep for 20 milliseconds
                system("clear");
            }
            tcsetattr(STDIN_FILENO, TCSANOW, &old_settings); // changes setting to old one
            break;
        }
    }
    return 0;
}
