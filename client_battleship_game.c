#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <errno.h>
#include <unistd.h>
#define MAX 80 
#define PORT 8080 
#define SA struct sockaddr 

int translate_position(char letter)
{
    switch(letter)
    {
        case 'A'-1: return 0;
        case 'A': return 1;
        case 'B': return 2;
        case 'C': return 3;
        case 'D': return 4;
        case 'E': return 5;
        case 'F': return 6;
        case 'G': return 7;
        case 'H': return 8;
        case 'I': return 9;
        case 'J': return 10;
        case 'K': return 11;
        case 'K'+1: return 12;
        default: return 42;
    }
}

int translate_position_game_core(char letter)
{
    switch(letter)
    {
        case 'A'-1: return 0;
        case 'A': return 1;
        case 'B': return 2;
        case 'C': return 3;
        case 'D': return 4;
        case 'E': return 5;
        case 'F': return 6;
        case 'G': return 7;
        case 'H': return 8;
        case 'I': return 9;
        case 'J': return 10;
        default: return 42;
    }
}

void error_handler(char *ship_types, char player_ships[13][13])
{
    char rules_message[1024];
    bzero(rules_message, sizeof(rules_message));
    printf("Wprowadzono niepoprawne polozenie %s\n", ship_types);
    printf("\nTwoja plansza: \n\n");
    printf("   A B C D E F G H I J\n\n");
    for(int i=1; i<11; ++i) //rysowanie planszy
    {
        for(int j=0; j<11; ++j)
        {
            if(j==0) printf("%d ",i);
            else if (i!=10 && j==1) printf(" %c ", player_ships[i][j]);
            else printf("%c ", player_ships[i][j]);
        }
        printf("\n");
        }
    printf("\n");					
}
  
void ship_initialize(char player_ships[13][13], int position_of_battleship[4], int position_of_destroyer[8], int position_of_submarine[12], int position_of_patrol_board[8])
{
    printf("\nTwoja plansza: \n\n");
    printf("   A B C D E F G H I J\n\n");
	for(int i=1; i<11; ++i) //rysowanie planszy
	{
		for(int j=0; j<11; ++j)
		{
			if(j==0) printf("%d ",i);
            else if (i!=10 && j==1) printf(" %c ", player_ships[i][j]);
			else printf("%c ", player_ships[i][j]);
		}
		printf("\n");
	}
    printf("\n");

    char ship_position[10];
    char ship_types[10][40]={"czteromasztowca", "pierwszego trojmasztowca","drugiego trojmasztowca", "pierwszego dwumasztowca", "drugiego dwumasztowca", 
    "trzeciego dwumasztowca","pierwszego jednomasztowca", "drugiego jednomasztowca", 
    "trzeciego jednomasztowca", "czwartego jednomasztowca"};

    printf("Wprowadz gdzie chcesz postawic statki w formacie xx-xx (np. A4-B6)\n");

   int error_flag = 0;

    //na poczatku gracz ustawia statki
    for(int i=0; i<10; i++)
    {
        error_flag = 0;

        printf("Wprowadz polozenie %s\n", ship_types[i]);
        fgets(ship_position, 10, stdin);

		//FIRST CASE (e.g. "A4-A7") //pionowe i poziome przypadki
		if ( (strlen(ship_position)-1 == 5) && ((ship_position[0] >= 'A') && (ship_position[0] <= 'J'))
		&& ((ship_position[1] >= '1') && (ship_position[1] <= '9')) && (ship_position[2] == '-')
        && ((ship_position[3] >= 'A') && (ship_position[3] <= 'J')) && ((ship_position[4] >= '1') && (ship_position[4] <= '9')) )
		{
			if (i == 0) {

				if(ship_position[0] == ship_position[3]) // pionowo
				{
					if(abs(ship_position[4]-ship_position[1])==3)
					{
						if(ship_position[1] < ship_position[4]) 
                        {	
                            for(int j = ship_position[1]-'0'-1; j <= ship_position[4]-'0'+1; ++j) //rzędy
                            {
                                if (error_flag == 1) break;

                                for(int k = (translate_position(ship_position[0])-1); k <= translate_position(ship_position[0]+1); ++k) //kolumny
                                {
                                    if (error_flag == 1) break;

                                    if(player_ships[j][k]!='*') 
                                    {
                                        error_flag = 1;
                                    }
                                }
                            }
                        }
                        else if(ship_position[1] < ship_position[4])
                        {
                            for(int j = ship_position[1]-'0'+1; j >= ship_position[4]-'0'-1; --j) //rzędy
                            {
                                if (error_flag == 1) break;

                                for(int k = (translate_position(ship_position[0])-1); k <= translate_position(ship_position[0]+1); ++k) //kolumny
                                {
                                    if (error_flag == 1) break;

                                    if(player_ships[j][k]!='*')
                                    {
                                        error_flag = 1;
                                    }
                                }
                            }
                        }
                        else error_flag = 1;

                        if(error_flag == 0)
                        {
                            for(int j=0; j<4; ++j)
                            {
                                player_ships[ship_position[1]-'0'+j][translate_position(ship_position[0])]='X';
                            }
                            position_of_battleship[0] = (int)ship_position[1]-'0';
                            position_of_battleship[1] = translate_position(ship_position[0]);
                            position_of_battleship[2] = (int)ship_position[1]-'0'+3;
                            position_of_battleship[3] = translate_position(ship_position[0]); //zapisywanie położenia czteromasztowca
                            printf("\nTwoja plansza: \n\n");
                            printf("   A B C D E F G H I J\n\n");
                                for(int i=1; i<11; ++i) //rysowanie planszy
                                {
                                    for(int j=0; j<11; ++j)
                                    {
                                        if(j==0) printf("%d ",i);
                                        else if (i!=10 && j==1) printf(" %c ", player_ships[i][j]);
                                        else printf("%c ", player_ships[i][j]);
                                    }
                                    printf("\n");
                                }
                            printf("\n");
                            printf("Wykonaj kolejny ruch\n");
                        }
                    }
                    else error_flag = 1;
                }
				
				else if(ship_position[1] == ship_position[4]) //poziomo
				{
                    if(abs(ship_position[3]-ship_position[0])==3)
					{
						if(ship_position[0] < ship_position[3]) 
                        {	
                            for(int j = ship_position[1]-'0'-1; j <= ship_position[1]-'0'+1; ++j) //rzędy
                            {
                                if (error_flag == 1) break;

                                for(int k = translate_position(ship_position[0])-1; k <= translate_position(ship_position[3])+1; ++k) //kolumny
                                {
                                    if (error_flag == 1) break;

                                    if(player_ships[j][k]!='*')
                                    {
                                        error_flag = 1;
                                    }
                                }
                            }
                        }
                        else if(ship_position[1] < ship_position[4])
                        {
                            for(int j = ship_position[1]-'0'-1; j >= ship_position[1]-'0'+1; ++j) //rzędy
                            {
                                if (error_flag == 1) break;

                                for(int k = (translate_position(ship_position[0])-1); k <= translate_position(ship_position[3]+1); ++k) //kolumny
                                {
                                    if (error_flag == 1) break;

                                    if(player_ships[j][k]!='*') 
                                    {
                                        error_flag = 1;
                                    }
                                }
                            }
                        }	
                        else error_flag = 1;

                        if(error_flag == 0)
                        {
                            for(int j=0; j<4; ++j)
                            {
                                player_ships[ship_position[1]-'0'][translate_position(ship_position[0])+j]='X';
                            }
                            position_of_battleship[0] = (int)ship_position[1]-'0';
                            position_of_battleship[1] = translate_position(ship_position[0]);
                            position_of_battleship[2] = (int)ship_position[1]-'0';
                            position_of_battleship[3] = translate_position(ship_position[0])+3; //zapisywanie położenia czteromasztowca
                            printf("\nTwoja plansza: \n\n");
                            printf("   A B C D E F G H I J\n\n");
                                for(int i=1; i<11; ++i) //rysowanie planszy
                                {
                                    for(int j=0; j<11; ++j)
                                    {
                                        if(j==0) printf("%d ",i);
                                        else if (i!=10 && j==1) printf(" %c ", player_ships[i][j]);
                                        else printf("%c ", player_ships[i][j]);
                                    }
                                    printf("\n");
                                }
                            printf("\n");
                            printf("Wykonaj kolejny ruch\n");
                        }
                    }
                    else error_flag = 1;    
    			}
                else error_flag = 1;  
                
                if (error_flag == 1) 
                {
                    error_handler(ship_types[i], player_ships);
                    --i;
                    printf("ERROR!!!\n");
                }
            }
			else if ( i == 1 || i == 2 ) 
            {
                if(ship_position[0] == ship_position[3]) // pionowo
				{
					if(abs(ship_position[4]-ship_position[1])==2)
					{
						if(ship_position[1] < ship_position[4]) 
                        {	
                            for(int j = ship_position[1]-'0'-1; j <= ship_position[4]-'0'+1; ++j) //rzędy
                            {
                                if (error_flag == 1) break;

                                for(int k = (translate_position(ship_position[0])-1); k <= translate_position(ship_position[0]+1); ++k) //kolumny
                                {
                                    if (error_flag == 1) break;

                                    if(player_ships[j][k]!='*') 
                                    {
                                        error_flag = 1;
                                    }
                                }
                            }
                        }

                        else if(ship_position[1] > ship_position[4])
                        {
                            for(int j = ship_position[1]-'0'+1; j >= ship_position[4]-'0'-1; --j) //rzędy
                            {
                                if (error_flag == 1) break;

                                for(int k = (translate_position(ship_position[0])-1); k <= translate_position(ship_position[0]+1); ++k) //kolumny
                                {
                                    if (error_flag == 1) break;

                                    if(player_ships[j][k]!='*') 
                                    {
                                        error_flag = 1;
                                    }
                                }
                            }
                        }
                        else error_flag = 1;

                        if(error_flag == 0)
                        {
                            for(int j=0; j<3; ++j)
                            {
                                player_ships[ship_position[1]-'0'+j][translate_position(ship_position[0])]='X';
                            }
                            if(i == 1)
                            {
                                position_of_destroyer[0] = (int)ship_position[1]-'0';
                                position_of_destroyer[1] = translate_position(ship_position[0]);
                                position_of_destroyer[2] = (int)ship_position[1]-'0'+2;
                                position_of_destroyer[3] = translate_position(ship_position[0]);
                            }
                            if(i == 2)
                            {
                                position_of_destroyer[4] = (int)ship_position[1]-'0';
                                position_of_destroyer[5] = translate_position(ship_position[0]);
                                position_of_destroyer[6] = (int)ship_position[1]-'0'+2;
                                position_of_destroyer[7] = translate_position(ship_position[0]);
                            }
                            printf("\nTwoja plansza: \n\n");
                            printf("   A B C D E F G H I J\n\n");
                                for(int i=1; i<11; ++i) //rysowanie planszy
                                {
                                    for(int j=0; j<11; ++j)
                                    {
                                        if(j==0) printf("%d ",i);
                                        else if (i!=10 && j==1) printf(" %c ", player_ships[i][j]);
                                        else printf("%c ", player_ships[i][j]);
                                    }
                                    printf("\n");
                                }
                            printf("\n");
                            printf("Wykonaj kolejny ruch\n");
                        }
                    }
                    else error_flag = 1;
                }
				else if(ship_position[1] == ship_position[4]) //poziomo
				{
                    if(abs(ship_position[3]-ship_position[0])==2)
					{
						if(ship_position[0] < ship_position[3]) 
                        {	
                            for(int j = ship_position[1]-'0'-1; j <= ship_position[1]-'0'+1; ++j) //rzędy
                            {
                                if (error_flag == 1) break;

                                for(int k = translate_position(ship_position[0])-1; k <= translate_position(ship_position[3])+1; ++k) //kolumny
                                {
                                    if (error_flag == 1) break;

                                    if(player_ships[j][k]!='*') 
                                    {
                                        error_flag = 1;
                                    }
                                }
                            }
                        }
                        else if(ship_position[0] > ship_position[3])
                        {
                            for(int j = ship_position[1]-'0'-1; j <= ship_position[1]-'0'+1; ++j) //rzędy
                            {
                                if (error_flag == 1) break;

                                for(int k = (translate_position(ship_position[0])+1); k <= translate_position(ship_position[3]-1); ++k) //kolumny
                                {
                                    if (error_flag == 1) break;

                                    if(player_ships[j][k]!='*') 
                                    {
                                        error_flag = 1;
                                    }
                                }
                            }
                        }	
                        if(error_flag == 0)
                        {
                            for(int j=0; j<3; ++j)
                            {
                                player_ships[ship_position[1]-'0'][translate_position(ship_position[0])+j]='X';
                            }
                            if(i == 1)
                            {
                                position_of_destroyer[0] = (int)ship_position[1]-'0';
                                position_of_destroyer[1] = translate_position(ship_position[0]);
                                position_of_destroyer[2] = (int)ship_position[1]-'0';
                                position_of_destroyer[3] = translate_position(ship_position[0])+2;
                            }
                            if(i == 2)
                            {
                                position_of_destroyer[4] = (int)ship_position[1]-'0';
                                position_of_destroyer[5] = translate_position(ship_position[0]);
                                position_of_destroyer[6] = (int)ship_position[1]-'0';
                                position_of_destroyer[7] = translate_position(ship_position[0])+2;
                            }
                            printf("\nTwoja plansza: \n\n");
                            printf("   A B C D E F G H I J\n\n");
                                for(int i=1; i<11; ++i) //rysowanie planszy
                                {
                                    for(int j=0; j<11; ++j)
                                    {
                                        if(j==0) printf("%d ",i);
                                        else if (i!=10 && j==1) printf(" %c ", player_ships[i][j]);
                                        else printf("%c ", player_ships[i][j]);
                                    }
                                    printf("\n");
                                }
                            printf("\n");
                            printf("Wykonaj kolejny ruch\n");
                        }
                    }
                    else error_flag = 1;    
    			}
                else error_flag = 1;

                if (error_flag == 1) 
                {
                    error_handler(ship_types[i], player_ships);
                    --i;
                    printf("ERROR!!!\n");
                }
            }
            else if ( i == 3 || i == 4 || i == 5 ) 
            {
                if(ship_position[0] == ship_position[3]) // pionowo
                {
                    if(abs(ship_position[4]-ship_position[1])==1)
                    {
                        if(ship_position[1] < ship_position[4]) 
                        {	
                            for(int j = ship_position[1]-'0'-1; j <= ship_position[4]-'0'+1; ++j) //rzędy
                            {
                                if (error_flag == 1) break;

                                for(int k = (translate_position(ship_position[0])-1); k <= translate_position(ship_position[0]+1); ++k) //kolumny
                                {
                                    if (error_flag == 1) break;

                                    if(player_ships[j][k]!='*') 
                                    {
                                        error_flag = 1;
                                    }
                                }
                            }
                        }
                        else if(ship_position[1] > ship_position[4])
                        {
                            for(int j = ship_position[1]-'0'+1; j >= ship_position[4]-'0'-1; --j) //rzędy
                            {
                                if (error_flag == 1) break;

                                for(int k = (translate_position(ship_position[0])-1); k <= translate_position(ship_position[0]+1); ++k) //kolumny
                                {
                                    if (error_flag == 1) break;

                                    if(player_ships[j][k]!='*') 
                                    {
                                        error_flag = 1;
                                    }
                                }
                            }
                        }
                        else error_flag = 1;

                        if(error_flag == 0)
                        {
                            for(int j=0; j<2; ++j)
                            {
                                player_ships[ship_position[1]-'0'+j][translate_position(ship_position[0])]='X';
                            }
                            if(i==3)
                            {
                                position_of_submarine[0] = (int)ship_position[1]-'0';
                                position_of_submarine[1] = translate_position(ship_position[0]);
                                position_of_submarine[2] = (int)ship_position[1]-'0'+1;
                                position_of_submarine[3] = translate_position(ship_position[0]);
                            }
                            if(i==4)
                            {
                                position_of_submarine[4] = (int)ship_position[1]-'0';
                                position_of_submarine[5] = translate_position(ship_position[0]);
                                position_of_submarine[6] = (int)ship_position[1]-'0'+1;
                                position_of_submarine[7] = translate_position(ship_position[0]);
                            }
                            if(i==5)
                            {
                                position_of_submarine[8] = (int)ship_position[1]-'0';
                                position_of_submarine[9] = translate_position(ship_position[0]);
                                position_of_submarine[10] = (int)ship_position[1]-'0'+1;
                                position_of_submarine[11] = translate_position(ship_position[0]);
                            }
                            printf("\nTwoja plansza: \n\n");
                            printf("   A B C D E F G H I J\n\n");
                                for(int i=1; i<11; ++i) //rysowanie planszy
                                {
                                    for(int j=0; j<11; ++j)
                                    {
                                        if(j==0) printf("%d ",i);
                                        else if (i!=10 && j==1) printf(" %c ", player_ships[i][j]);
                                        else printf("%c ", player_ships[i][j]);
                                    }
                                    printf("\n");
                                }
                            printf("\n");
                            printf("Wykonaj kolejny ruch\n");
                        }
                    }
                    else error_flag = 1;
                }

                else if(ship_position[1] == ship_position[4]) //poziomo
                {
                    if(abs(ship_position[3]-ship_position[0])==1)
                    {
                        if(ship_position[0] < ship_position[3]) 
                        {	
                            for(int j = ship_position[1]-'0'-1; j <= ship_position[1]-'0'+1; ++j) //rzędy
                            {
                                if (error_flag == 1) break;

                                for(int k = translate_position(ship_position[0])-1; k <= translate_position(ship_position[3])+1; ++k) //kolumny
                                {
                                    if (error_flag == 1) break;

                                    if(player_ships[j][k]!='*') 
                                    {
                                        error_flag = 1;
                                    }
                                }
                            }
                        }
                        else if(ship_position[0] > ship_position[3])
                        {
                            for(int j = ship_position[1]-'0'-1; j <= ship_position[1]-'0'+1; ++j) //rzędy
                            {
                                if (error_flag == 1) break;

                                for(int k = (translate_position(ship_position[0])+1); k <= translate_position(ship_position[3]-1); ++k) //kolumny
                                {
                                    if (error_flag == 1) break;

                                    if(player_ships[j][k]!='*') 
                                    {
                                        error_flag = 1;
                                    }
                                }
                            }
                        }
                        else error_flag = 1;
        
                        if(error_flag == 0)
                        {
                            for(int j=0; j<2; ++j)
                            {
                                player_ships[ship_position[1]-'0'][translate_position(ship_position[0])+j]='X';
                            }
                            if(i==3)
                            {
                                position_of_submarine[0] = (int)ship_position[1]-'0';
                                position_of_submarine[1] = translate_position(ship_position[0]);
                                position_of_submarine[2] = (int)ship_position[1]-'0';
                                position_of_submarine[3] = translate_position(ship_position[0])+1;
                            }
                            if(i==4)
                            {
                                position_of_submarine[4] = (int)ship_position[1]-'0';
                                position_of_submarine[5] = translate_position(ship_position[0]);
                                position_of_submarine[6] = (int)ship_position[1]-'0';
                                position_of_submarine[7] = translate_position(ship_position[0])+1;
                            }
                            if(i==5)
                            {
                                position_of_submarine[8] = (int)ship_position[1]-'0';
                                position_of_submarine[9] = translate_position(ship_position[0]);
                                position_of_submarine[10] = (int)ship_position[1]-'0';
                                position_of_submarine[11] = translate_position(ship_position[0])+1;
                            }
                            printf("\nTwoja plansza: \n\n");
                            printf("   A B C D E F G H I J\n\n");
                                for(int i=1; i<11; ++i) //rysowanie planszy
                                {
                                    for(int j=0; j<11; ++j)
                                    {
                                        if(j==0) printf("%d ",i);
                                        else if (i!=10 && j==1) printf(" %c ", player_ships[i][j]);
                                        else printf("%c ", player_ships[i][j]);
                                    }
                                    printf("\n");
                                }
                            printf("\n");
                            printf("Wykonaj kolejny ruch\n");
                        }
                    }
                    else error_flag = 1;    
    			}
                else error_flag = 1;

                if (error_flag == 1) 
                {
                    error_handler(ship_types[i], player_ships);
                    --i;
                    printf("ERROR!!!\n");
                }
            }
            else if ( i == 6 || i == 7 || i == 8 || i == 9 ) 
            {
				if(ship_position[0] == ship_position[3] && ship_position[1] == ship_position[4])
				{
                    for(int j = ship_position[1]-'0'-1; j <= ship_position[4]-'0'+1; ++j) //rzędy
                    {
                        if (error_flag == 1) break;

                        for(int k = (translate_position(ship_position[0])-1); k <= translate_position(ship_position[0]+1); ++k) //kolumny
                        {
                            if (error_flag == 1) break;

                            if(player_ships[j][k]!='*') 
                            {
                                error_flag = 1;
                            }
                        }
                    }

                    if(error_flag == 0)
                    {
                        player_ships[ship_position[1]-'0'][translate_position(ship_position[0])]='X';
                    
                        if(i==6)
                        {
                            position_of_patrol_board[0] = (int)ship_position[1]-'0';
                            position_of_patrol_board[1] = translate_position(ship_position[0]);
                        }
                        if(i==7)
                        {
                            position_of_patrol_board[2] = (int)ship_position[1]-'0';
                            position_of_patrol_board[3] = translate_position(ship_position[0]);
                        }
                        if(i==8)
                        {
                            position_of_patrol_board[4] = (int)ship_position[1]-'0';
                            position_of_patrol_board[5] = translate_position(ship_position[0]);
                        }
                        if(i==9)
                        {
                            position_of_patrol_board[6] = (int)ship_position[1]-'0';
                            position_of_patrol_board[7] = translate_position(ship_position[0]);
                        }
                        printf("\nTwoja plansza: \n\n");
                            printf("   A B C D E F G H I J\n\n");
                                for(int i=1; i<11; ++i) //rysowanie planszy
                                {
                                    for(int j=0; j<11; ++j)
                                    {
                                        if(j==0) printf("%d ",i);
                                        else if (i!=10 && j==1) printf(" %c ", player_ships[i][j]);
                                        else printf("%c ", player_ships[i][j]);
                                    }
                                    printf("\n");
                                }
                            printf("\n");
                        printf("Wykonaj kolejny ruch\n");
                    }

                }
                else error_flag = 1;

                if (error_flag == 1) 
                {
                    error_handler(ship_types[i], player_ships);
                    --i;
                    printf("ERROR!!!\n");
                }
            }
        }

				
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//SECOND CASE (e.g. "A10-A7")   //tylko pionowe przypadki
		else if (((strlen(ship_position)-1) == 6) && ((ship_position[0] >= 'A') && (ship_position[0] <= 'J'))
		&& (ship_position[1] == '1') && (ship_position[2] == '0') && (ship_position[3] == '-') && ((ship_position[4] >= 'A') && (ship_position[4] <= 'J'))
		&& ((ship_position[5] >= '1') && (ship_position[5] <= '9')))
        {
			if (i == 0) 
            {
				if(abs(ship_position[5]-'0'-10)==3)
				{
                    for(int j = 11; j >= 6; --j) //rzędy
                    {
                        if(error_flag==1) break;

                        for(int k = (translate_position(ship_position[0])-1); k <= translate_position(ship_position[0]+1); ++k) //kolumny
                        {
                            if(error_flag==1) {
                                printf("Blad, j: %d, k: %d",j,k);
                                break;
                            }
                            if(player_ships[j][k]!='*') 
                            {
                                error_flag = 1;
                            }
                        }
                    }

                    if(error_flag == 0)
                    {
                        for(int j=0; j<4; ++j)
                        {
                            player_ships[ship_position[5]-'0'+j][translate_position(ship_position[0])]='X';
                        }
                        printf("\nTwoja plansza: \n\n");
                        printf("   A B C D E F G H I J\n\n");
                            for(int i=1; i<11; ++i) //rysowanie planszy
                            {
                                for(int j=0; j<11; ++j)
                                {
                                    if(j==0) printf("%d ",i);
                                    else if (i!=10 && j==1) printf(" %c ", player_ships[i][j]);
                                    else printf("%c ", player_ships[i][j]);
                                }
                                printf("\n");
                            }
                            printf("\n");
                            printf("Wykonaj kolejny ruch\n");
                    }
                }
                else error_flag = 1;

                if (error_flag == 1) {
                    error_handler(ship_types[i], player_ships);
                    --i;
                    printf("ERROR!!!\n");
                }
            }
			else if ( i == 1 || i == 2 ) 
            {
				if(abs(ship_position[5]-'0'-10)==2)
				{
					for(int j = 11; j >= 7; --j) //rzędy
                    {
                        if (error_flag == 1) break;

                        for(int k = (translate_position(ship_position[0])-1); k <= translate_position(ship_position[0]+1); ++k) //kolumny
                        {
                            if (error_flag == 1) break;

                            if(player_ships[j][k]!='*') 
                            {
                                error_flag = 1;
                            }
                        }
                    }

                    if(error_flag == 0)
                    {
                        for(int j=0; j<3; ++j)
                        {
                            player_ships[ship_position[5]-'0'+j][translate_position(ship_position[0])]='X';
                        }
                        printf("\nTwoja plansza: \n");
                        printf("\nTwoja plansza: \n\n");
                        printf("   A B C D E F G H I J\n\n");
                            for(int i=1; i<11; ++i) //rysowanie planszy
                            {
                                for(int j=0; j<11; ++j)
                                {
                                    if(j==0) printf("%d ",i);
                                    else if (i!=10 && j==1) printf(" %c ", player_ships[i][j]);
                                    else printf("%c ", player_ships[i][j]);
                                }
                                printf("\n");
                            }
                            printf("\n");
                            printf("Wykonaj kolejny ruch\n");
                    }
                }
                else error_flag = 1;

                if (error_flag == 1) {
                    error_handler(ship_types[i], player_ships);
                    --i;
                    printf("ERROR!!!\n");
                }
            }
            else if ( i == 3 || i == 4 || i == 5 ) 
            {
				if(abs(ship_position[5]-'0'-10)==1)
				{
					for(int j = 11; j >= 8; --j) //rzędy
                    {
                        if (error_flag == 1) break;

                        for(int k = (translate_position(ship_position[0])-1); k <= translate_position(ship_position[0]+1); ++k) //kolumny
                        {
                            if (error_flag == 1) break;

                            if(player_ships[j][k]!='*') 
                            {
                                error_flag = 1;
                            }
                        }
                    }
                    
                    if(error_flag == 0)
                    {
                        for(int j=0; j<2; ++j)
                        {
                            player_ships[ship_position[5]-'0'+j][translate_position(ship_position[0])]='X';
                        }
                        printf("\nTwoja plansza: \n\n");
                        printf("   A B C D E F G H I J\n\n");
                            for(int i=1; i<11; ++i) //rysowanie planszy
                            {
                                for(int j=0; j<11; ++j)
                                    {
                                    if(j==0) printf("%d ",i);
                                    else if (i!=10 && j==1) printf(" %c ", player_ships[i][j]);
                                    else printf("%c ", player_ships[i][j]);
                                }
                                printf("\n");
                            }
                        printf("\n");
                        printf("Wykonaj kolejny ruch\n");
                    }
                }
                else error_flag = 1;

                if (error_flag == 1) {
                    error_handler(ship_types[i], player_ships);
                    --i;
                    printf("ERROR!!!\n");
                }
            }
            

            else if ( i == 6 || i == 7 || i == 8 || i == 9 ) 
            {
				if(abs(ship_position[5]-'0'-10)==0)
				{
					for(int j = 11; j >= 9; --j) //rzędy
                    {
                        if (error_flag == 1) break;

                        for(int k = (translate_position(ship_position[0])-1); k <= translate_position(ship_position[0]+1); ++k) //kolumny
                        {
                            if (error_flag == 1) break;

                            if(player_ships[j][k]!='*') 
                            {
                                error_flag = 1;
                            }
                        }
                    }
                    
                    if(error_flag == 0)
                    {
                        for(int j=0; j<2; ++j)
                        {
                            player_ships[ship_position[5]-'0'+j][translate_position(ship_position[0])]='X';
                        }
                        printf("\nTwoja plansza: \n");
                        printf("\nTwoja plansza: \n\n");
                            printf("   A B C D E F G H I J\n\n");
                                for(int i=1; i<11; ++i) //rysowanie planszy
                                {
                                    for(int j=0; j<11; ++j)
                                    {
                                        if(j==0) printf("%d ",i);
                                        else if (i!=10 && j==1) printf(" %c ", player_ships[i][j]);
                                        else printf("%c ", player_ships[i][j]);
                                    }
                                    printf("\n");
                                }
                            printf("\n");
                            printf("Wykonaj kolejny ruch\n");
                    }
                }
                else error_flag = 1;

                if (error_flag == 1) {
                    error_handler(ship_types[i], player_ships);
                    --i;
                    printf("ERROR!!!\n");
                }
            }
            else error_flag = 1;
        }
        

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

		//THIRD CASE (e.g. "A7-A10")    //tylko pionowe przypadki
		else if (((strlen(ship_position)-1) == 6) && ((ship_position[0] >= 'A') && (ship_position[0] <= 'J'))
		&& ((ship_position[1] >= '1') && (ship_position[1] <= '9')) && (ship_position[2] == '-') 
        && ((ship_position[3] >= 'A') && (ship_position[3] <= 'J')) && (ship_position[4] == '1') && (ship_position[5] == '0'))
		{
			if (i == 0) 
            {
				if(abs(ship_position[1]-'0'-10)==3)
				{	
                    for(int j = 11; j >= 6; --j) //rzędy
                    {
                        if(error_flag==1) break;

                        for(int k = (translate_position(ship_position[0])-1); k <= translate_position(ship_position[0]+1); ++k) //kolumny
                        {
                            if(error_flag==1) break;

                            if(player_ships[j][k]!='*') 
                            {
                                error_flag = 1;
                            }
                        }
                    }

                    if(error_flag == 0)
                    {
                        for(int j=0; j<4; ++j)
                        {
                            player_ships[ship_position[1]-'0'+j][translate_position(ship_position[0])]='X';
                        }
                        printf("\nTwoja plansza: \n\n");
                            printf("   A B C D E F G H I J\n\n");
                                for(int i=1; i<11; ++i) //rysowanie planszy
                                {
                                    for(int j=0; j<11; ++j)
                                    {
                                        if(j==0) printf("%d ",i);
                                        else if (i!=10 && j==1) printf(" %c ", player_ships[i][j]);
                                        else printf("%c ", player_ships[i][j]);
                                    }
                                    printf("\n");
                                }
                            printf("\n");
                            printf("Wykonaj kolejny ruch\n");
                    }
                    else error_flag = 1;
                }
                else error_flag = 1;

                if (error_flag == 1) {
                    error_handler(ship_types[i], player_ships);
                    --i;
                    printf("ERROR!!!\n");
                }
            }
            
			else if ( i == 1 || i == 2 ) 
            {
				if(abs(ship_position[1]-'0'-10)==2)
				{
					for(int j = 11; j >= 7; --j) //rzędy
                    {
                        if (error_flag == 1) break;

                        for(int k = (translate_position(ship_position[0])-1); k <= translate_position(ship_position[0]+1); ++k) //kolumny
                        {
                            if (error_flag == 1) break;

                            if(player_ships[j][k]!='*') 
                            {
                                error_flag = 1;
                            }
                        }
                    }

                    if(error_flag == 0)
                    {
                        for(int j=0; j<3; ++j)
                        {
                            player_ships[ship_position[1]-'0'+j][translate_position(ship_position[0])]='X';
                        }
                        
                       printf("\nTwoja plansza: \n\n");
                            printf("   A B C D E F G H I J\n\n");
                                for(int i=1; i<11; ++i) //rysowanie planszy
                                {
                                    for(int j=0; j<11; ++j)
                                    {
                                        if(j==0) printf("%d ",i);
                                        else if (i!=10 && j==1) printf(" %c ", player_ships[i][j]);
                                        else printf("%c ", player_ships[i][j]);
                                    }
                                    printf("\n");
                                }
                            printf("\n");
                            printf("Wykonaj kolejny ruch\n");
                    }
                }
                else error_flag = 1;

                if (error_flag == 1) {
                    error_handler(ship_types[i], player_ships);
                    --i;
                    printf("ERROR!!!\n");
                }
            }
            

            else if ( i == 3 || i == 4 || i == 5 ) 
            {
				if(abs(ship_position[1]-'0'-10)==1)
				{
					for(int j = 11; j >= 8; --j) //rzędy
                    {
                        if (error_flag == 1) break;

                        for(int k = (translate_position(ship_position[0])-1); k <= translate_position(ship_position[0]+1); ++k) //kolumny
                        {
                            if (error_flag == 1) break;

                            if(player_ships[j][k]!='*') 
                            {
                                error_flag = 1;
                            }
                        }
                    }
                    
                    if(error_flag == 0)
                    {
                        for(int j=0; j<2; ++j)
                        {
                            player_ships[ship_position[1]-'0'+j][translate_position(ship_position[0])]='X';
                        }
                        printf("\nTwoja plansza: \n\n");
                            printf("   A B C D E F G H I J\n\n");
                                for(int i=1; i<11; ++i) //rysowanie planszy
                                {
                                    for(int j=0; j<11; ++j)
                                    {
                                        if(j==0) printf("%d ",i);
                                        else if (i!=10 && j==1) printf(" %c ", player_ships[i][j]);
                                        else printf("%c ", player_ships[i][j]);
                                    }
                                    printf("\n");
                                }
                            printf("\n");
                            printf("Wykonaj kolejny ruch\n");
                    }
                    else error_flag = 1;
                }
                else error_flag = 1;

                if (error_flag == 1) {
                    error_handler(ship_types[i], player_ships);
                    --i;
                    printf("ERROR!!!\n");
                }
            }
        
            else if ( i == 6 || i == 7 || i == 8 || i == 9 ) 
            {
				if(abs(ship_position[1]-'0'-10)==0)
				{
					for(int j = 11; j >= 9; --j) //rzędy
                    {
                        if (error_flag == 1) break;

                        for(int k = (translate_position(ship_position[0])-1); k <= translate_position(ship_position[0]+1); ++k) //kolumny
                        {
                            if (error_flag == 1) break;
                            
                            if(player_ships[j][k]!='*') 
                            {
                                error_flag = 1;
                            }
                        }
                    }
                    
                    if(error_flag == 0)
                    {
                        for(int j=0; j<2; ++j)
                        {
                            player_ships[ship_position[1]-'0'+j][translate_position(ship_position[0])]='X';
                        }
                        printf("\nTwoja plansza: \n\n");
                            printf("   A B C D E F G H I J\n\n");
                                for(int i=1; i<11; ++i) //rysowanie planszy
                                {
                                    for(int j=0; j<11; ++j)
                                    {
                                        if(j==0) printf("%d ",i);
                                        else if (i!=10 && j==1) printf(" %c ", player_ships[i][j]);
                                        else printf("%c ", player_ships[i][j]);
                                    }
                                    printf("\n");
                                }
                            printf("\n");
                            printf("Wykonaj kolejny ruch\n");
                    }
                }
                else error_flag = 1;

                if (error_flag == 1) {
                    error_handler(ship_types[i], player_ships);
                    --i;
                    printf("ERROR!!!\n");
                }
            }
            else error_flag = 1;
        }
    

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//FOURTH CASE (e.g. "A10-D10")  //tylko poziome przypadki
		else if ( ((strlen(ship_position)-1) == 7) && (ship_position[0] >= 'A' && (ship_position[0] <= 'J'))
        && (ship_position[1] == '1') && (ship_position[2] == '0') && (ship_position[3] == '-')
        && ((ship_position[4] >= 'A') && (ship_position[4] <= 'J'))
        && (ship_position[5] == '1') && (ship_position[6] == '0') )
		{
			if (i == 0) 
            {
				if(abs(ship_position[4]-'0'-ship_position[0]+48)==3)
				{
                    for(int j = 11; j >= 9; --j)
                    {
                        if(error_flag==1) break;

                        for(int k = (translate_position(ship_position[0])-1); k <= translate_position(ship_position[4]+1); ++k) //kolumny
                        {
                            if(error_flag==1) break;

                            if(player_ships[10][k]!='*') 
                            {
                                error_flag = 1;
                            }
                        }
                    }
                    if(error_flag == 0)
                    {
                        for(int j=0; j<4; ++j)
                        {
                            player_ships[10][translate_position(ship_position[0])+j]='X';
                        }
                        printf("\nTwoja plansza: \n\n");
                            printf("   A B C D E F G H I J\n\n");
                                for(int i=1; i<11; ++i) //rysowanie planszy
                                {
                                    for(int j=0; j<11; ++j)
                                    {
                                        if(j==0) printf("%d ",i);
                                        else if (i!=10 && j==1) printf(" %c ", player_ships[i][j]);
                                        else printf("%c ", player_ships[i][j]);
                                    }
                                    printf("\n");
                                }
                            printf("\n");
                            printf("Wykonaj kolejny ruch\n");
                    }
                    else error_flag = 1;
                }
                else error_flag = 1;

                if (error_flag == 1) {
                    error_handler(ship_types[i], player_ships);
                    --i;
                    printf("ERROR!!!\n");
                }
            }
        
			else if ( i == 1 || i == 2 ) 
            {
				if(abs(ship_position[4]-'0'-ship_position[0]+48)==2)
				{
                    for(int j=11; j>=9; --j)
                    {
                        if(error_flag==1) break;

                        for(int k = (translate_position(ship_position[0])-1); k <= translate_position(ship_position[4]+1); ++k) //kolumny
                        {
                            if(error_flag==1) break;

                            if(player_ships[j][k]!='*') 
                            {
                                error_flag = 1;
                            }
                        }
                    }

                    if(error_flag == 0)
                    {
                        for(int j=0; j<3; ++j)
                        {
                            player_ships[10][translate_position(ship_position[0])+j]='X';
                        }
                        printf("\nTwoja plansza: \n\n");
                        printf("   A B C D E F G H I J\n\n");
                            for(int i=1; i<11; ++i) //rysowanie planszy
                            {
                                for(int j=0; j<11; ++j)
                                {
                                    if(j==0) printf("%d ",i);
                                    else if (i!=10 && j==1) printf(" %c ", player_ships[i][j]);
                                    else printf("%c ", player_ships[i][j]);
                                }
                                printf("\n");
                            }
                            printf("\n");
                            printf("Wykonaj kolejny ruch\n");
                    }
                    else error_flag = 1;
                }
                else error_flag = 1;

                if (error_flag == 1) {
                    error_handler(ship_types[i], player_ships);
                    --i;
                    printf("ERROR!!!\n");
                }
            }
        

            else if ( i == 3 || i == 4 || i == 5 ) 
            {
				if(abs(ship_position[4]-'0'-ship_position[0]+48)==1)
				{
                    for(int j=11; j>=9; --j)
                    {
                        if(error_flag==1) break;

                        for(int k = (translate_position(ship_position[0])-1); k <= translate_position(ship_position[4]+1); ++k) //kolumny
                        {
                            if(error_flag==1) break;

                            if(player_ships[j][k]!='*') 
                            {
                                error_flag = 1;
                            }
                        }
                    }
                    if(error_flag == 0)
                    {
                        for(int j=0; j<2; ++j)
                        {
                            player_ships[10][translate_position(ship_position[0])+j]='X';
                        }
                        printf("\nTwoja plansza: \n\n");
                            printf("   A B C D E F G H I J\n\n");
                                for(int i=1; i<11; ++i) //rysowanie planszy
                                {
                                    for(int j=0; j<11; ++j)
                                    {
                                        if(j==0) printf("%d ",i);
                                        else if (i!=10 && j==1) printf(" %c ", player_ships[i][j]);
                                        else printf("%c ", player_ships[i][j]);
                                    }
                                    printf("\n");
                                }
                            printf("\n");
                            printf("Wykonaj kolejny ruch\n");
                    }
                    else error_flag = 1;
                }
                else error_flag = 1;

                if (error_flag == 1) {
                    error_handler(ship_types[i], player_ships);
                    --i;
                    printf("ERROR!!!\n");
                }
            }
        

            else if ( i == 6 || i == 7 || i == 8 || i == 9 ) 
            {
				if(abs(ship_position[4]-'0'-ship_position[0]+48)==0)
				{
                    for(int j=11; j>=9; --j)
                    {
                        if(error_flag==1) break;

                        for(int k = (translate_position(ship_position[0])-1); k <= translate_position(ship_position[4]+1); ++k) //kolumny
                        {
                            if(error_flag==1) break;

                            if(player_ships[j][k]!='*') 
                            {
                                error_flag = 1;
                            }
                        }
                    }

                    if(error_flag == 0){
                    
                        player_ships[10][translate_position(ship_position[0])]='X';

                        printf("\nTwoja plansza: \n\n");
                            printf("   A B C D E F G H I J\n\n");
                                for(int i=1; i<11; ++i) //rysowanie planszy
                                {
                                    for(int j=0; j<11; ++j)
                                    {
                                        if(j==0) printf("%d ",i);
                                        else if (i!=10 && j==1) printf(" %c ", player_ships[i][j]);
                                        else printf("%c ", player_ships[i][j]);
                                    }
                                    printf("\n");
                                }
                            printf("\n");
                        printf("Wykonaj kolejny ruch\n");
                    }
                    else error_flag = 1;
                }
                else error_flag = 1;

                if (error_flag == 1) {
                    error_handler(ship_types[i], player_ships);
                    --i;
                    printf("ERROR!!!\n");
                }
            }
            
        }
        else 
        {
            error_handler(ship_types[i], player_ships);
            --i;
            printf("ERROR!!!\n");
        }
    }
                printf("\nTwoja finalna plansza: \n\n");
                printf("   A B C D E F G H I J\n\n");
                for(int i=1; i<11; ++i) //rysowanie planszy
                {
                    for(int j=0; j<11; ++j)
                    {
                        if(j==0) printf("%d ",i);
                        else if (i!=10 && j==1) printf(" %c ", player_ships[i][j]);
                        else printf("%c ", player_ships[i][j]);
                    }
                    printf("\n");
                }
                printf("\n");
}
 

/////////////////////////////////////////////////////////GAME CORE///////////////////////////////////////////////////////////////////////////////////////


 int game_core(char player_ships[13][13], char enemy_ships_board[13][13], int sockfd, int position_of_battleship[4], int position_of_destroyer[8], int position_of_submarine[12], int position_of_patrol_board[8])
{
    int win_message = 77;
    int if_battleship = 0;
    int read_mes = 0;
    int ready_mess = 0;
    int if_destroyer = 0;
    int if_destroyer_1 = 0;
    int if_submarine = 0;
    int if_submarine_1 = 0;
    int if_submarine_2 = 0;
    int if_patrol_boat = 0;
    int X_shot=0;
    int X_amount = 20;
    int X_left_on_your_board = 20;
    int flag_horizontal = 0;
    int flag_vertical = 0;
    int flag_of_replay = 0;
    int flag_first = 0;
    int n = 0;
    //bufor, do którego wpisywana jest pozycja do zestrzelenia podana przez klienta
    char player_shot_position[10]; 
    //bufor, do którego wpisywana jest pozycja do zestrzelenia odebrana od serwera
    char enemy_shot_position[10];
    //tablica 2D, w której przechowywana jest plansza przeciwnika, nie jest ona widoczna dla gracza         
    char enemy_ships_positions[13][13];     
    //tablica 1D, w której przechowywane są koordynaty pozycji, w które strzelił przeciwnik, jako zmienne typu int
    int place_where_enemy_shot[40];
    //tablica 1D, w której przechowywane są koordynaty pozycji, w które strzelił przeciwnik, jako zmienne typu int
    int place_where_player_shot[40];
    //liczba strzałów jakie wykonał przeciwnik
    int number_of_enemy_shots = 0;

    printf("Zaczynamy gre z przeciwnikiem!\n");
    
    for(int test = 0; test < 4; ++test){
        if(recv(sockfd, enemy_ships_positions, 169, MSG_DONTWAIT) > 0){
            //gracz otrzyma pierwszenstwo w grze
        }
    }
     //otrzymanie flagi określającej czy to gracz ma zacząć grę, czy poczekać na ruch przeciwnika
    //zaczyna ten kto pierwszy wysłał tablicę statków się do serwera, enemy_ships_positions spełnia tylko rolę bufora


    if (strlen(enemy_ships_positions) != 0) 
    {
        flag_first = 0;
    }
    else flag_first = 1;

    //printf("FLAGA: %d", flag_first);
    write(sockfd, player_ships, 169); //przesłanie swojej planszy do serwera

    
    if(flag_first == 1) {
        do
        {
            bzero(enemy_ships_positions, 169);
            recv(sockfd, enemy_ships_positions, 169, MSG_DONTWAIT); //odebranie planszy wroga, gracz bezpośrednio jej nie widzi
            
            if (enemy_ships_positions[0][0] == 60) 
            {
                printf("\nTwoj przeciwnik opuscil gre! Polacz sie ponownie z serwerem, aby rozpoczac gre z innym graczem.\n");
                close(sockfd);
                exit(0);
            }
        } while (strlen(enemy_ships_positions) == 0); //dopóki nie zostaną odebrana plansza przeciwnika, czytaj bufor
    }


    if(flag_first == 1) //wykonaj, jeśli graczowi przysługuje wykonanie pierwszego ruchu
    {
        printf("\nPlansza przeciwnika:\n\n");
        printf("   A B C D E F G H I J\n\n");
        for(int i=1; i<11; ++i) //rysowanie planszy
        {
            for(int j=0; j<11; ++j)
            {
                if(j==0) printf("%d ",i);
                else if (i!=10 && j==1) printf(" %c ", enemy_ships_board[i][j]);
                else printf("%c ", enemy_ships_board[i][j]);
            }
            printf("\n");
        }
        printf("\n");

        printf("\nTwoja plansza: \n\n");
        printf("   A B C D E F G H I J\n\n");
        for(int i=1; i<11; ++i) //rysowanie planszy
        {
            for(int j=0; j<11; ++j)
            {
                if(j==0) printf("%d ",i);
                else if (i!=10 && j==1) printf(" %c ", player_ships[i][j]);
                else printf("%c ", player_ships[i][j]);
            }
            printf("\n");
        }
        printf("\n");
        printf("Wykonaj ruch! Wprowadź pozycję do zestrzelenia w formacie XX \n");
        fflush(stdin);


    // wprowadzanie pozycji do zestrzelenia, wraz z zabezpieczeniami przed wpisaniem czegos niepopranwego
    // jeśli wprowadzone dane będą niepoprawne, program poprosi o wprowadzenie jeszcze raz

        flag_of_replay = 0;
        do
        {
            flag_of_replay = 0;
            while(1)
            {
                fgets(player_shot_position, 10, stdin);
                if(translate_position_game_core(player_shot_position[0]) == 42 || strlen(player_shot_position) < 3 || player_shot_position[1] < '1' || player_shot_position[1] > '9')
                {
                    fflush(stdin);
                    printf("Wprowadzono niepoprawna pozycje do zestrzelenia\n");
                }
                else if(strlen(player_shot_position) > 4 || strlen(player_shot_position) < 3)
                {
                    fflush(stdin);
                    printf("Wprowadzono niepoprawna pozycje do zestrzelenia\n");
                }
                else
                {
                    break;
                }
                
                
            }

            for(int i=0; i<40; ++i)
            {
                place_where_player_shot[i]=99; //tablicę z koordynatami pozycji do strzału, wypełniamy liczbą '99'
            }
            //celem tej operacji jest umożliwienie poprawnego odczytu zadanych koordynat


            //przypadek, w górym gracz celuje w pozycję o indeksie 10
            if(strlen(player_shot_position) == 4 && player_shot_position[1] == '1' && player_shot_position[2] == '0') 
            {
                //zapisywanie jako zmienna int, w tablicy położeń, położenia pola w które strzeliliśmy
                //w tym wypadku będzie to zawsze 10
                place_where_player_shot[0] = 10; 
                //zapisywanie jako zmienna int położenia podanego jako litera np. A
                place_where_player_shot[1] = translate_position_game_core(player_shot_position[0]);
            }

            //w przypadku, w którym tablica znaków ma długość 4, 
            if(strlen(player_shot_position) == 4 && (player_shot_position[1]!='1' || player_shot_position[2]!='0'))
            {
                flag_of_replay = 1;
                printf("Wprowadzono niepoprawne położenie\n");
                continue; 
            }

            
            if(strlen(player_shot_position) == 3)
            {
                //zapisywanie jako zmienna int, położenia pola w które strzeliliśmy
                place_where_player_shot[0] = (int)player_shot_position[1]-'0'; 
                //tłumaczenie położenia, podanego jako litera od A do J
                place_where_player_shot[1] = translate_position_game_core(player_shot_position[0]);
            }
        } while(flag_of_replay == 1);

        //umieszczenie znaku '0' na widocznej tablicy, na której gracz wykonuje strzały
        enemy_ships_board[place_where_player_shot[0]][place_where_player_shot[1]]='0';



        printf("Wykonales ruch.\n");
        printf("\n\nPlansza przeciwnika:\n\n");
        printf("   A B C D E F G H I J\n\n");
        for(int i=1; i<11; ++i) //rysowanie planszy
        {
            for(int j=0; j<11; ++j)
            {
                if(j==0) printf("%d ",i);
                else if (i!=10 && j==1) printf(" %c ", enemy_ships_board[i][j]);
                else printf("%c ", enemy_ships_board[i][j]);
            }
            printf("\n");
        }
        printf("\n");
            
        // w momencie kiedy trafimy, przysługuje nam kolejny strzał, aż do momentu, w którym spudłujemy

        int case_no = 2; //pozycje o indeksach 0 i 1  mamy już zapełnione, przez uprzednio wykonany ruch
        flag_of_replay = 0;

        //dopóki trafiamy w X na planszy przeciwnika, przysługuje nam kolejny ruch,
        //koordynaty każdego kolejnego trafienia są zapisywane w tablicy intów, która po spudłowaniu jest wysyłana do przeciwnika
        //w przypadku zakończenia gry poprzez zestrzelenie wszystkich statków wroga w serii strzałów,
        //jako place_where_player_shot[0] jest wysłana liczba 77, która ma za zadanie poinformować przeciwnika o przegranej
        while((enemy_ships_positions[place_where_player_shot[case_no - 2]][place_where_player_shot[case_no - 1]]=='X' || flag_of_replay == 1) && X_shot!=X_amount && ((case_no + 1)<40))
        {
            
            if(flag_of_replay!=1) printf("TRAFIONY! \n");
            if(flag_of_replay!=1) enemy_ships_board[place_where_player_shot[case_no - 2]][place_where_player_shot[case_no-1]]='T';
            ++X_shot;
            if(flag_of_replay!=1)
            {
                printf("Wykonales ruch trafiony.\n");
                printf("\n\nPlansza przeciwnika:\n\n");
                printf("   A B C D E F G H I J\n\n");
                for(int i=1; i<11; ++i) //rysowanie planszy
                {
                    for(int j=0; j<11; ++j)
                    {
                        if(j==0) printf("%d ",i);
                        else if (i!=10 && j==1) printf(" %c ", enemy_ships_board[i][j]);
                        else printf("%c ", enemy_ships_board[i][j]);
                    }
                    printf("\n");
                }
                printf("\n");
            }
            flag_of_replay = 0;
            printf("Wykonaj ruch! Wprowadź pozycję do zestrzelenia w formacie XX \n");

        //wprowadzanie pozycji do zestrzelenia oraz sprawdzanie poprawności wprowadzonych danych
        while(1)
        {
            fgets(player_shot_position, 10, stdin); 
            if(translate_position_game_core(player_shot_position[0]) == 42 || strlen(player_shot_position) < 3 || strlen(player_shot_position) > 4 || player_shot_position[1] < '1' || player_shot_position[1] > '9')
            {
                fflush(stdin);
                printf("Wprowadzono niepoprawna pozycje do zestrzelenia\n");
            }
            else
            {
                break;
            }
            
        }

        if(strlen(player_shot_position) == 4 && player_shot_position[1] == '1' && player_shot_position[2] == '0') 
        {
            place_where_player_shot[case_no] = 10; //zapisywanie, gdzie strzeliliśmy w celu do przeciwnika
            place_where_player_shot[case_no + 1] = translate_position_game_core(player_shot_position[0]);
        }

        if(strlen(player_shot_position) == 4 && (player_shot_position[1]!='1' || player_shot_position[2]!='0'))
        {
            flag_of_replay = 1;
            printf("Wprowadzono niepoprawne położenie\n");
            if(X_shot!=0) --X_shot;
            continue;
        }

        if(strlen(player_shot_position) == 3)
        {
            place_where_player_shot[case_no] = (int)player_shot_position[1]-'0'; //zapisywanie gdzie strzeliliśmy po to by to wysłać do przeciwnika
            place_where_player_shot[case_no + 1] = translate_position_game_core(player_shot_position[0]);
        }

        if(enemy_ships_board[place_where_player_shot[case_no]][place_where_player_shot[case_no+1]]=='T' || enemy_ships_board[place_where_player_shot[case_no]][place_where_player_shot[case_no+1]]=='0')
        {
            flag_of_replay = 1;
            printf("Wprowadzono niepoprawne położenie\n");
            if(X_shot!=0) --X_shot;
            continue;
        }

            enemy_ships_board[place_where_player_shot[case_no]][place_where_player_shot[case_no+1]]='0';

            printf("Wykonales ruch\n");
            printf("\n\nPlansza przeciwnika:\n\n");
            printf("   A B C D E F G H I J\n\n");
            for(int i=1; i<11; ++i) //rysowanie planszy
            {
            for(int j=0; j<11; ++j)
            {
                if(j==0) printf("%d ",i);
                else if (i!=10 && j==1) printf(" %c ", enemy_ships_board[i][j]);
                else printf("%c ", enemy_ships_board[i][j]);
                }
                printf("\n");
            }
            printf("\n");
            printf("\nTwoja plansza: \n\n");
            printf("   A B C D E F G H I J\n\n");
            for(int i=1; i<11; ++i) //rysowanie planszy
            {
                for(int j=0; j<11; ++j)
                {
                    if(j==0) printf("%d ",i);
                    else if (i!=10 && j==1) printf(" %c ", player_ships[i][j]);
                    else printf("%c ", player_ships[i][j]);
                }
                printf("\n");
            }
            printf("\n");
			
            if(enemy_ships_positions[place_where_player_shot[case_no]][place_where_player_shot[case_no+1]]!='X') 
            {
                break;
            }
            if(X_shot >= X_amount) break;
            case_no+=2;
            
        }
        if(X_shot<X_amount)
        {
            printf("PUDLO!\n");
            if(write(sockfd, place_where_player_shot, sizeof(place_where_player_shot))<0)
            {
                perror("write error");
            }
        }
        else if(X_shot >= X_amount)
        {
            place_where_player_shot[0]=win_message;
            if(write(sockfd, place_where_player_shot, sizeof(place_where_player_shot))<0)
            {
                perror("write error");
            }
            printf("WYGRANA\n");
            return 0;
        }

    }
////////////////

    //tu zaczyna się właściwy game core, przeciwnik wykonuje ruch, potem gracz naprzemiennie
while(X_shot<X_amount || X_left_on_your_board!=0) //dopóki nie zestrzelisz wszystkich statków przeciwnika, lub przeciwnik ze zestrzeli Twoich
{

        printf("Czekaj na ruch przeciwnika...\n"); //czyli czekanie aż przeciwnik skończy wykonywać ruch

        do
        {
            bzero(place_where_enemy_shot, sizeof(place_where_enemy_shot));
            read_mes = read(sockfd, place_where_enemy_shot, sizeof(place_where_enemy_shot));
            if (place_where_enemy_shot[0] == 60) 
            {
                printf("\nTwoj przeciwnik opuscil gre! Polacz sie ponownie z serwerem, aby rozpoczac gre z innym graczem.\n");
                close(sockfd);
                exit(0);
            }
        } while(read_mes<=0);
        
        //jeśli otrzymasz jako pierwszy element tablicy określoną liczbę (77), to znaczy, że przeciwnik zbił wszystkie Twoje statki
        if(place_where_enemy_shot[0] == win_message)
        {
            printf("PRZEGRANA\n");
            return 0;
        }
if(read_mes>0)
    {
            number_of_enemy_shots=0;
            //najpierw wyśietlona plansza, na której wykonywane są strzały, następnie plansza z Twoimi statkami
            printf("\nPlansza przeciwnika:\n\n");
            printf("   A B C D E F G H I J\n\n");
            for(int i=1; i<11; ++i) //rysowanie planszy
            {
                for(int j=0; j<11; ++j)
                {
                    if(j==0) printf("%d ",i);
                    else if (i!=10 && j==1) printf(" %c ", enemy_ships_board[i][j]);
                    else printf("%c ", enemy_ships_board[i][j]);
                }
                printf("\n");
            }
            printf("\n");

/////////////////////////
    //pokazujemy graczowi, czy czy został trafiony
    //wyciągamy dane z place_where_enemy_shot

    for(int i=0; i<40; ++i)
    {
        //jeśli place_where_enemy_shot[i]!=99 to oznacza, że w danej komórce tablicy znajduje się koordynata położenia
        if(place_where_enemy_shot[i]!=99)
        {
                ++number_of_enemy_shots;
        }
    }

    //iterujemy po kolei po wszystkich koordynatach (elementach place_where_enemy_shot)
    //tablica na której znajdują się statki jest 2D, więc położenie punktu określają dwie współrzędne
    //dlatego shots_no zwiększamy o dwa

    for(int shots_no = 0; shots_no<number_of_enemy_shots; shots_no+=2)
    {
        if(player_ships[place_where_enemy_shot[shots_no]][place_where_enemy_shot[shots_no+1]]=='X')
        {
            printf("Zostales trafiony!\n");
            player_ships[place_where_enemy_shot[shots_no]][place_where_enemy_shot[shots_no+1]]='T'; //Trafienie pojawia się T
            --X_left_on_your_board;
            //te zmienne muszą być zerowane w tym miejscu, inaczej trafienia mogłyby się kumulować 
            //i w konsekwencji dostalibyśmy zestrzeony statek w miejscu, gdzie nic takiego się nie wydarzyło
            if_battleship = 0;
            if_destroyer = 0;
            if_destroyer_1 = 0;
            if_submarine = 0;
            if_submarine_1 = 0;
            if_submarine_2 = 0;

            // jeśli na początku i końcu statku są literki 'T', sprawdzamy, czy w środku też są - jeśli tak - statek zatopiony
            // czteromasztowiec
            if((player_ships[position_of_battleship[0]][position_of_battleship[1]] == 'T') && (player_ships[position_of_battleship[2]][position_of_battleship[3]] == 'T'))
            {
                flag_horizontal = 0;
                flag_vertical = 0;
                if(position_of_battleship[0]==position_of_battleship[2]) //poziomo
                {
                    flag_horizontal = 1;
                    for(int k=0; k<4; ++k)
                    {
                        if(player_ships[position_of_battleship[0]][position_of_battleship[1]+k]== 'T')
                        {
                            ++if_battleship;
                        }
                    }
                }
                else if(position_of_battleship[1]==position_of_battleship[3]) //pionowo
                {
                    flag_vertical = 1;
                    for(int k=0; k<4; ++k)
                    {
                        if(player_ships[position_of_battleship[0]+k][position_of_battleship[1]] == 'T')
                        {
                            ++if_battleship;
                        }
                    }
                }
                if(if_battleship == 4)
                {
                    if(flag_horizontal == 1)
                    {
                        for(int k=0; k<4; ++k)
                        {
                            player_ships[position_of_battleship[0]][position_of_battleship[1]+k]= 'Z';
                        }
                    }
                    else if(flag_vertical == 1)
                    {
                        for(int k=0; k<4; ++k)
                        {
                            player_ships[position_of_battleship[0]+k][position_of_battleship[1]] = 'Z';
                        }
                    }
                    printf("TWOJ CZTEROMASZTOWIEC TRAFIONY-ZATOPIONY\n");
                }
            }
            
            //trojmasztowiec jeden
            else if((player_ships[position_of_destroyer[0]][position_of_destroyer[1]] == 'T') && (player_ships[position_of_destroyer[2]][position_of_destroyer[3]] == 'T'))
            {
                flag_horizontal = 0;
                flag_vertical = 0;
                if(position_of_destroyer[0]==position_of_destroyer[2]) //poziomo
                {
                    flag_horizontal = 1; 
                    for(int k=0; k<3; ++k)
                    {
                        if(player_ships[position_of_destroyer[0]][position_of_destroyer[1]+k]== 'T')
                        {
                            ++if_destroyer;
                        }
                    }
                }
                else if(position_of_destroyer[1]==position_of_destroyer[3]) //pionowo
                {
                    flag_vertical = 1;
                    for(int k=0; k<3; ++k)
                    {
                        if(player_ships[position_of_destroyer[0]+k][position_of_destroyer[1]] == 'T')
                        {
                            ++if_destroyer;
                        }
                    }
                }
                if(if_destroyer == 3)
                {
                    if(flag_horizontal == 1)
                    {
                        for(int k=0; k<3; ++k)
                        {
                            player_ships[position_of_destroyer[0]][position_of_destroyer[1]+k]= 'Z';
                        }
                    }
                    else if(flag_vertical == 1)
                    {
                        for(int k=0; k<3; ++k)
                        {
                            player_ships[position_of_destroyer[0]+k][position_of_destroyer[1]] ='Z';
                        }
                    }
                    printf("TWOJ TROJMASZTOWIEC TRAFIONY-ZATOPIONY\n");
                }
            }

            //trojmasztowiec drugi
            else if((player_ships[position_of_destroyer[4]][position_of_destroyer[5]] == 'T') && (player_ships[position_of_destroyer[6]][position_of_destroyer[7]] == 'T'))
            {
                flag_horizontal = 0;
                flag_vertical = 0;
                if(position_of_destroyer[4]==position_of_destroyer[6]) //poziomo
                {
                    flag_horizontal = 1;
                    for(int k=0; k<3; ++k)
                    {
                        if(player_ships[position_of_destroyer[4]][position_of_destroyer[5]+k]== 'T')
                        {
                            ++if_destroyer_1;
                        }
                    }
                }
                else if(position_of_destroyer[5]==position_of_destroyer[7]) //pionowo
                {
                    flag_vertical = 1;
                    for(int k=0; k<3; ++k)
                    {
                        if(player_ships[position_of_destroyer[4]+k][position_of_destroyer[5]] == 'T')
                        {
                            ++if_destroyer_1;
                        }
                    }
                }
                if(if_destroyer_1 == 3)
                {
                    if(flag_horizontal == 1)
                    {
                        for(int k=0; k<3; ++k)
                        {
                            player_ships[position_of_destroyer[4]][position_of_destroyer[5]+k]= 'Z';
                        }
                    }
                    else if(flag_vertical == 1)
                    {
                        for(int k=0; k<3; ++k)
                        {
                            player_ships[position_of_destroyer[4]+k][position_of_destroyer[5]] = 'Z';
                        }
                    }
                    printf("TWOJ TROJMASZTOWIEC TRAFIONY-ZATOPIONY\n");
                }
            }
            
            //dwumasztowiec pierwszy

            else if((player_ships[position_of_submarine[0]][position_of_submarine[1]] == 'T') && (player_ships[position_of_submarine[2]][position_of_submarine[3]] == 'T'))
            {
                flag_horizontal = 0;
                flag_vertical = 0;
                if(position_of_submarine[0]==position_of_submarine[2]) //poziomo
                {
                    flag_horizontal = 1;
                    for(int k=0; k<2; ++k)
                    {
                        if(player_ships[position_of_submarine[0]][position_of_submarine[1]+k]== 'T')
                        {
                            ++if_submarine;
                        }
                    }
                }
                else if(position_of_submarine[1]==position_of_submarine[3]) //pionowo
                {
                    flag_vertical = 1;
                    for(int k=0; k<2; ++k)
                    {
                        if(player_ships[position_of_submarine[0]+k][position_of_submarine[1]] == 'T')
                        {
                            ++if_submarine;
                        }
                    }
                }
                if(if_submarine == 2)
                {
                    if(flag_horizontal == 1)
                    {
                        for(int k=0; k<2; ++k)
                        {
                            player_ships[position_of_submarine[0]][position_of_submarine[1]+k]= 'Z';
                        }
                    }
                    else if(flag_vertical == 1)
                    {
                        for(int k=0; k<2; ++k)
                        {
                            player_ships[position_of_submarine[0]+k][position_of_submarine[1]] = 'Z';
                        }
                    }
                    printf("TWOJ DWUMASZTOWIEC TRAFIONY-ZATOPIONY\n");
                }
            }
            

            //dwumasztowiec drugi

            else if((player_ships[position_of_submarine[4]][position_of_submarine[5]] == 'T') && (player_ships[position_of_submarine[6]][position_of_submarine[7]] == 'T'))
            {
                flag_horizontal = 0;
                flag_vertical = 0;
                if(position_of_submarine[4]==position_of_submarine[6]) //poziomo
                {
                    flag_horizontal = 1;
                    for(int k=0; k<2; ++k)
                    {
                        if(player_ships[position_of_submarine[4]][position_of_submarine[5]+k]== 'T')
                        {
                            ++if_submarine_1;
                        }
                    }
                }
                else if(position_of_submarine[5]==position_of_submarine[7]) //pionowo
                {
                    flag_vertical = 1;
                    for(int k=0; k<2; ++k)
                    {
                        if(player_ships[position_of_submarine[4]+k][position_of_submarine[5]] == 'T')
                        {
                            ++if_submarine_1;
                        }
                    }
                }
                if(if_submarine_1 == 2)
                {
                    if(flag_horizontal == 1)
                    {
                        for(int k=0; k<2; ++k)
                        {
                            player_ships[position_of_submarine[4]][position_of_submarine[5]+k]= 'Z';
                        }
                    }
                    else if(flag_vertical == 1)
                    {
                        for(int k=0; k<2; ++k)
                        {
                            player_ships[position_of_submarine[4]+k][position_of_submarine[5]] = 'Z';
                        }
                    }
                    printf("TWOJ DWUMASZTOWIEC TRAFIONY-ZATOPIONY\n");
                }
            }
            

            //dwumasztowiec trzeci

            else if((player_ships[position_of_submarine[8]][position_of_submarine[9]] == 'T') && (player_ships[position_of_submarine[10]][position_of_submarine[11]] == 'T'))
            {
                flag_horizontal = 0;
                flag_vertical = 0;
                if(position_of_submarine[8]==position_of_submarine[10]) //poziomo
                {
                    flag_horizontal = 1;
                    for(int k=0; k<2; ++k)
                    {
                        if(player_ships[position_of_submarine[8]][position_of_submarine[9]+k]== 'T')
                        {
                            ++if_submarine_2;
                        }
                    }
                }
                else if(position_of_submarine[9]==position_of_submarine[11]) //pionowo
                {
                    flag_vertical = 1;
                    for(int k=0; k<2; ++k)
                    {
                        if(player_ships[position_of_submarine[8]+k][position_of_submarine[9]] == 'T')
                        {
                            ++if_submarine_2;
                        }
                    }
                }
                if(if_submarine_2 == 2 )
                {
                    if(flag_horizontal == 1)
                    {
                        for(int k=0; k<2; ++k)
                        {
                            player_ships[position_of_submarine[8]][position_of_submarine[9]+k]='Z';
                        }
                    }
                    else if(flag_vertical == 1)
                    {
                        for(int k=0; k<2; ++k)
                        {
                            player_ships[position_of_submarine[8]+k][position_of_submarine[9]] ='Z';
                        }
                    }
                    printf("TWOJ DWUMASZTOWIEC TRAFIONY-ZATOPIONY\n");
                }
            }
            

            //sprawdzanie zatopionego jednomasztowca odbywa się inaczej
            else if (player_ships[position_of_patrol_board[0]][position_of_patrol_board[1]] == 'T')
            {
                player_ships[position_of_patrol_board[0]][position_of_patrol_board[1]] = 'Z';
                printf("TWOJ JEDNOMASZTOWIEC TRAFIONY-ZATOPIONY\n");
            }
            else if (player_ships[position_of_patrol_board[2]][position_of_patrol_board[3]] == 'T')
            {
                player_ships[position_of_patrol_board[2]][position_of_patrol_board[3]] = 'Z';
                printf("TWOJ JEDNOMASZTOWIEC TRAFIONY-ZATOPIONY\n");
            }
            else if (player_ships[position_of_patrol_board[4]][position_of_patrol_board[5]] == 'T')
            {
                player_ships[position_of_patrol_board[4]][position_of_patrol_board[5]] = 'Z';
                printf("TWOJ JEDNOMASZTOWIEC TRAFIONY-ZATOPIONY\n");
            }
            else if (player_ships[position_of_patrol_board[6]][position_of_patrol_board[7]] == 'T')
            {
                player_ships[position_of_patrol_board[6]][position_of_patrol_board[7]] = 'Z';
                printf("TWOJ JEDNOMASZTOWIEC TRAFIONY-ZATOPIONY\n");
            }

        }
        else if(player_ships[place_where_enemy_shot[shots_no]][place_where_enemy_shot[shots_no+1]]!='X') //Pudło przeciwnika
        {
            printf("Przeciwnik spudlowal\n");
            printf("%d\n", shots_no);
            player_ships[place_where_enemy_shot[shots_no]][place_where_enemy_shot[shots_no+1]]='0';
            break; 
        }

    }
            
    printf("\nTwoja plansza: \n\n");
    printf("   A B C D E F G H I J\n\n");
    for(int i=1; i<11; ++i) //rysowanie planszy
    {
        for(int j=0; j<11; ++j)
        {
            if(j==0) printf("%d ",i);
            else if (i!=10 && j==1) printf(" %c ", player_ships[i][j]);
            else printf("%c ", player_ships[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    printf("Wykonaj ruch! Wprowadź pozycję do zestrzelenia w formacie XX \n");

    //ten sam mechanizm wpisywania pozycji, wraz z zabezpieczeniami jak wyżej
    flag_of_replay = 0;
    do
    {
        flag_of_replay = 0;
        while(1)
        {
            fgets(player_shot_position, 10, stdin); 
            if(translate_position_game_core(player_shot_position[0]) == 42 || strlen(player_shot_position) < 3 || strlen(player_shot_position) > 4 || player_shot_position[1] < '1' || player_shot_position[1] > '9')
            {
                fflush(stdin);
                printf("Wprowadzono niepoprawna pozycje do zestrzelenia\n");
            }
            else
            {
                break;
            }
            
        }

        
        for(int i=0; i<40; ++i)
        {
            place_where_player_shot[i]=99;
        }

        if(strlen(player_shot_position) == 4 && player_shot_position[1] == '1' && player_shot_position[2] == '0') 
        {
            place_where_player_shot[0] = 10; //zapisywanie gdzie strzeliliśmy po to by to wysłać do przeciwnika
            place_where_player_shot[1] = translate_position_game_core(player_shot_position[0]);
        }

        if(strlen(player_shot_position) == 4 && (player_shot_position[1]!='1' || player_shot_position[2]!='0'))
        {
            flag_of_replay = 1;
            printf("Wprowadzono niepoprawne położenie\n");
            continue;
        }
        if(strlen(player_shot_position) == 3)
        {
            place_where_player_shot[0] = (int)player_shot_position[1]-'0'; //zapisywanie gdzie strzeliliśmy po to by to wysłać do przeciwnika
            place_where_player_shot[1] = translate_position_game_core(player_shot_position[0]);
        }

        if(enemy_ships_board[place_where_player_shot[0]][place_where_player_shot[1]]=='T' || enemy_ships_board[place_where_player_shot[0]][place_where_player_shot[1]]=='0')
            {
            flag_of_replay = 1;
            printf("Wprowadzono niepoprawne położenie (strzal w miejsce uprzednio trafione)\n");
            }
    
    } while(flag_of_replay == 1);

    enemy_ships_board[place_where_player_shot[0]][place_where_player_shot[1]]='0';


    printf("Wykonales ruch\n");
    printf("\nPlansza przeciwnika:\n\n");
    printf("   A B C D E F G H I J\n\n");
    for(int i=1; i<11; ++i) //rysowanie planszy
    {
        for(int j=0; j<11; ++j)
        {
            if(j==0) printf("%d ",i);
            else if (i!=10 && j==1) printf(" %c ", enemy_ships_board[i][j]);
            else printf("%c ", enemy_ships_board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    printf("\nTwoja plansza: \n\n");
    printf("   A B C D E F G H I J\n\n");
    for(int i=1; i<11; ++i) //rysowanie planszy
    {
        for(int j=0; j<11; ++j)
        {
            if(j==0) printf("%d ",i);
            else if (i!=10 && j==1) printf(" %c ", player_ships[i][j]);
            else printf("%c ", player_ships[i][j]);
        }
        printf("\n");
        }
    printf("\n"); 
    int case_no = 2;
    flag_of_replay = 0;
    while((enemy_ships_positions[place_where_player_shot[case_no - 2]][place_where_player_shot[case_no - 1]]=='X' || flag_of_replay == 1) && X_shot!=X_amount && ((case_no+1)<40)) //jak trafisz
    {
            if(flag_of_replay != 1) printf("TRAFIONY!\n\n");
            if(flag_of_replay != 1) enemy_ships_board[place_where_player_shot[case_no - 2]][place_where_player_shot[case_no - 1]]='T';
            flag_of_replay = 0;
            ++X_shot;
            if(X_shot>=X_amount) break;
            printf("Wykonaj ruch! Wprowadź pozycję do zestrzelenia w formacie XX\n\n");

        while(1)
        {
            fgets(player_shot_position, 10, stdin); 
            if(translate_position_game_core(player_shot_position[0]) == 42 || strlen(player_shot_position) < 3 || strlen(player_shot_position) > 4 || player_shot_position[1] < '1' || player_shot_position[1] > '9')
            {
                fflush(stdin);
                printf("Wprowadzono niepoprawna pozycje do zestrzelenia\n");
            }
            else
            {
                break;
            }
            
        }

        if(strlen(player_shot_position) == 4 && player_shot_position[1] == '1' && player_shot_position[2] == '0') 
        {
            place_where_player_shot[case_no] = 10; 
            place_where_player_shot[case_no + 1] = translate_position_game_core(player_shot_position[0]);
        }

        if(strlen(player_shot_position) == 4 && (player_shot_position[1]!='1' || player_shot_position[2]!='0'))
        {
            flag_of_replay = 1;
            printf("Wprowadzono niepoprawne położenie\n");
            if(X_shot!=0) --X_shot;
            continue;
        }

        if(strlen(player_shot_position) == 3)
        {
            place_where_player_shot[case_no] = (int)player_shot_position[1]-'0'; 
            place_where_player_shot[case_no + 1] = translate_position_game_core(player_shot_position[0]);
        }

        if(enemy_ships_board[place_where_player_shot[case_no]][place_where_player_shot[case_no+1]]=='T' || enemy_ships_board[place_where_player_shot[case_no]][place_where_player_shot[case_no+1]]=='0')
        {
            flag_of_replay = 1;
            printf("Wprowadzono niepoprawne położenie (strzal w miejsce uprzednio trafione)\n");
           if(X_shot!=0) --X_shot;
            continue;
        }

        enemy_ships_board[place_where_player_shot[case_no]][place_where_player_shot[case_no + 1]]='0';


        printf("\nWykonales ruch\n");
        printf("\nPlansza przeciwnika:\n\n");
        printf("   A B C D E F G H I J\n\n");
        for(int i=1; i<11; ++i) //rysowanie planszy
        {
            for(int j=0; j<11; ++j)
            {
                if(j==0) printf("%d ",i);
                else if (i!=10 && j==1) printf(" %c ", enemy_ships_board[i][j]);
                else printf("%c ", enemy_ships_board[i][j]);
            }
            printf("\n");
        }
        printf("\n");
        printf("\nTwoja plansza: \n\n");
        printf("   A B C D E F G H I J\n\n");
        for(int i=1; i<11; ++i) //rysowanie planszy
        {
            for(int j=0; j<11; ++j)
            {
                if(j==0) printf("%d ",i);
                else if (i!=10 && j==1) printf(" %c ", player_ships[i][j]);
                else printf("%c ", player_ships[i][j]);
            }
        printf("\n");
        }
        printf("\n");
		
        if(enemy_ships_positions[place_where_player_shot[case_no]][place_where_player_shot[case_no+1]]!='X' && enemy_ships_positions[place_where_player_shot[case_no]][place_where_player_shot[case_no+1]]!='T' && enemy_ships_positions[place_where_player_shot[case_no]][place_where_player_shot[case_no+1]]!='0') 
        {
            break;
        }

        case_no+=2;
        if(X_shot>=X_amount) break;

    }
        if(X_shot<X_amount) 
        { 
            if(write(sockfd, place_where_player_shot, sizeof(place_where_player_shot))<0)
            {
                perror("write error");
            }
        }
        if(X_shot<X_amount) printf("PUDLO!\n");
        if(X_shot>=X_amount) break;
        
}
        
}
    if(X_left_on_your_board <= 0) printf("PRZEGRANA \n");
    if(X_shot >= X_amount) 
    {
        place_where_player_shot[0] = win_message;
        printf("WYGRANA\n");
        //wysyłanie komunikatu o wygranej do przeciwnika
        if(write(sockfd, place_where_player_shot, sizeof(place_where_player_shot))<0)
        {
            perror("write error");
        }
    }
}

 
int main(int argc, char **argv) 
{ 
    int sockfd, connfd; 
    struct sockaddr_in6 servaddr, cli;
    char player_ships[13][13];
    char enemy_ships_board[13][13];
    char mock_player_board[3][3];
    char mock_enemy_board[3][3];
    int position_of_battleship[4]; //czteromasztowiec
    int position_of_destroyer[8]; //trojmasztowiec
    int position_of_submarine[12]; //dwumasztowiec
    int position_of_patrol_board[8]; //jednomasztowce

    for (int i = 0; i < 13; ++i) {
        for (int j = 0; j < 13; ++j) {
            player_ships[i][j] = '*';
        }
    }

    for (int i = 0; i < 13; ++i) {
        for (int j = 0; j < 13; ++j) {
            enemy_ships_board[i][j] = '*';
        }
    }
  
    //stworzenie gniazda
    sockfd = socket(AF_INET6, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("Socket creation failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created.\n");

    bzero(&servaddr, sizeof(servaddr)); 
  
    //przypisanie IP, portu
    if (inet_pton(AF_INET6, argv[1], &servaddr.sin6_addr) < 0){
		fprintf(stderr,"AF_INET inet_pton error for %s : %s \n", argv[1], strerror(errno));
		return 1;
	}
    servaddr.sin6_family = AF_INET6; 
    servaddr.sin6_port = htons(PORT); 
  
    //połączenie klienta z gniazdem serwera 
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
        printf("Connection with the server failed...\n"); 
        exit(0); 
    } 
    else
        printf("Connected to the server!\n");

    mock_player_board[0][0] = 'R';
    mock_player_board[0][1] = 'D';

    printf("\nOczekiwanie na drugiego gracza...\n");

    do
    {
        recv(sockfd, mock_enemy_board, sizeof(mock_enemy_board), MSG_DONTWAIT);
        sleep(1);
        write(sockfd, mock_player_board, sizeof(mock_player_board));
        sleep(1);
    } while (mock_enemy_board[0][0] != 'R' && mock_enemy_board[0][1] != 'D');

    //klienci sparowani, przejście do gry
    ship_initialize(player_ships, position_of_battleship, position_of_destroyer, position_of_submarine, position_of_patrol_board);
    game_core(player_ships, enemy_ships_board, sockfd,  position_of_battleship, position_of_destroyer, position_of_submarine, position_of_patrol_board);
  
    //zamykanie gniazda
    close(sockfd); 
} 