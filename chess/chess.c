#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
// ADDED FOR ENCODING
#include <fcntl.h>
#include <io.h>
// For Matrix Copy memcpy
#include <string.h>
// Boolean
#include <stdbool.h>
#include <time.h>

int possible_moves[100];
int possible_moves_index;
int previous_position;
int new_position;
int display_convert(char ); // Converts the Matrix elements to Unicode representations.
// Eliminated pieces :
typedef struct eliminated_pieces
{
    char piece_type;
    struct eliminated_piece *next;
} eliminated_piece;

// Generating lists
typedef struct eliminated_pieces_lists
{
    eliminated_piece *start;
} eliminated_pieces_list;

// Generating piece node
eliminated_piece* create_piece(char value)
{
  eliminated_piece *piece;
  if((piece = (eliminated_piece *)malloc(sizeof(eliminated_piece))) != NULL)
  {
    piece->piece_type = value;
    piece->next = NULL;
  }
  return piece;
}

void insert_eliminated_list(char value, eliminated_pieces_list *l)
{
  eliminated_piece *temp;
  eliminated_piece *piece;
  piece = create_piece(value);
  // Empty list :
  if(l->start==NULL)
  {
    
    l->start=piece;
    piece->next=NULL;
  }
  else // list not Empty
  {
    temp = l->start;
    while (temp->next!=NULL)
    {
      temp = temp->next;
    }
    temp->next=piece;
    piece->next=NULL;
    
  }

}

void display_eliminated_list(eliminated_pieces_list l)
{
  eliminated_piece* temp = l.start;
  int converted_value;
  while (temp != NULL)
  {
    converted_value = display_convert(temp->piece_type);
    wprintf(L"%c ", converted_value);
    temp = temp->next;
  }
   wprintf(L"\n");
  
}


eliminated_pieces_list eliminated_pieces_white; 
eliminated_pieces_list eliminated_pieces_black;
char *piece;



char board[8][8] = { // Matrix 8x8 representing the board.
                    { 'R' , 'H' , 'C' , 'Q' , 'K' , 'C' , 'H' , 'R' },
                    { 'P' , 'P' , 'P' , 'P' , 'P' , 'P' , 'P' , 'P' },
                    { ' ' , ' ' , ' ' , ' ' , ' ' , ' ' , ' ' , ' ' },
                    { ' ' , ' ' , ' ' , ' ' , ' ' , ' ' , ' ' , ' ' },
                    { ' ' , ' ' , ' ' , ' ' , ' ' , ' ' , ' ' , ' ' },
                    { ' ' , ' ' , ' ' , ' ' , ' ' , ' ' , ' ' , ' ' },
                    { 'p' , 'p' , 'p' , 'p' , 'p' , 'p' , 'p' , 'p' },
                    { 'r' , 'h' , 'c' , 'q' , 'k' , 'c' , 'h' , 'r' }
                  };

void display();

void change( int , int , int , int, int) ;
void pawn(int , int ) ;
void rook(int , int, int) ;
void knight(int , int, int ) ;
void bishop( int , int, int ) ;
void king( int , int , int) ;
void queen( int , int , int ) ;
void pawnb( int , int ) ;
void player_white(); //player1
void player_black(); //player2
int check(int , int ) ; // Check if there is a piece at (x,y) => 1 else 0
int check2(int , int ) ;


int display_convert(char symbol)
{
    switch (symbol)
    {
    case 'p':
        return 0x265F;
        break;
    case 'r':
        return 0x265C;
        break;
    case 'h':
        return 0x265E;
        break;
    case 'c':
        return 0x265D;
        break;
    case 'k':
        return 0x265A;
        break;
    case 'q':
        return 0x265B;
        break;
    case 'P':
        return 0x2659;
        break;
    case 'R':
        return 0x2656;
        break;
    case 'H':
        return 0x2658;
        break;
    case 'C':
        return 0x2657;
        break;
    case 'K':
        return 0x2654;
        break;
    case 'Q':
        return 0x2655;
        break;
    case 'x':
        return 0x2716;
        break;
    default:
        return 0x0020; // space unicode
        break;
    }   
}


void intro()
{
    wprintf(L"     \n");
    wprintf(L"      __ \n");
    wprintf(L"     /  \\ \n");
    wprintf(L"     \\__/                             _               \n");
    wprintf(L"    /____\\             __          __| |__   ___  ___ ___            \n");
    wprintf(L"     |__|            / __| ___   / __| '_ \\ / _ \\/ __/ __|                 \n");
    wprintf(L"    (====)          | (__ |___| | (__| | | |  __/\\__ \\__ \\      \n");
    wprintf(L"    }===={           \\___|       \\___|_| |_|\\___||___/___/          - Kahouli Wajd. 死 \n");
    wprintf(L"  (________) \n");
}



main()
{
    // Init lists
    eliminated_pieces_white.start=NULL;
    eliminated_pieces_black.start=NULL;
    piece = (char *)malloc(sizeof(char));

int  x = 0 ; // Sets Turns
char ch ;
char showcase_option;

    _setmode(_fileno(stdout), 0x00020000); // SET ENCODING TO UNICODE
    system( "cls" ) ;
    intro();
    wprintf(L" \nPress Any Key To Continue...  \n  " ) ;
    showcase_option = getch();
    system( "cls" ) ;
    if(showcase_option==32)
    {
        showcase_game();
    }
    else
    {
        do
        {
        x++ ;
        remove_possible_moves();
        system( "cls" ) ;
        display(); // Displays the Board

        if( (x%2) == 0 )
        {
            player_white();
            
        }
        else
        {
            player_black();
        }

        wprintf(L" \nPress Any Key To Continue... -> \n " ) ;

        ch = getch();
        }while( ch != 27 ) ;
    }



}

void showcase_game()
{
    FILE *fp = fopen("chess_game_notes.txt", "r");
     if(fp == NULL) {
         perror("Unable to open file!");
         exit(1);
     }

    char chunk[128];
    char white_input[2];
    char white_output[2];
    char black_input[2];
    char black_output[2];

   while(fgets(chunk, sizeof(chunk), fp) != NULL) {
        //fputs(chunk, stdout);
        white_input[0] = chunk[1];
        white_input[1] = chunk[2];
        white_output[0] = chunk[4];
        white_output[1] = chunk[5];
        wprintf(L"%c%c.%c%c\n", white_input[0], white_input[1],white_output[0], white_output[1]);

        showcase_board(white_input[0], white_input[1], white_output[0], white_output[1], 2);
        delay(2);
        black_input[0] = chunk[7];
        black_input[1] = chunk[8];
        black_output[0] = chunk[10];
        black_output[1] = chunk[11];
        wprintf(L"%c%c.%c%c\n", black_input[0], black_input[1], black_output[0], black_output[1]);
        showcase_board(black_input[0], black_input[1], black_output[0], black_output[1], 1);
        delay(2);
        
         // marker string used to show where the content of the chunk array has ended
     }
 
    fclose(fp);
}

void showcase_board(char row, char col, char new_row, char new_col, int id)
{
    int c1 , r1 , c2 , r2;
    
    r1 = (int)(row - '0');
    c1 = (int)(col - '0');
    r2 = (int)(new_row - '0');
    c2 = (int)(new_col - '0');
    if(r1==0 && c1==0 && r2==0 && c2==0 && id==2)
    {
        change(7,4,7,6,2);
        change(7,7,7,5,2);
    }
    else if(r1==0 && c1==0 && r2==0 && c2==0 && id==1)
    {
        change(0,4,0,2,1);
        change(0,0,0,3,1);
    }
    else
    {
        change(r1,c1,r2,c2,id);
       // system( "cls" );
       // wprintf(L"Hikaru Nakamura -\n\n");
        display(); // Displays the Board
       // wprintf(L"Magnus Carlsen -\n");
    }
    


}
void delay(int number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;
 
    // Storing start time
    clock_t start_time = clock();
 
    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds);
}

void append_possible_moves()
{
    int x,y;
    for(int i=0; i<possible_moves_index; i++)
    {
        x = possible_moves[i]/10; // row
        y = possible_moves[i]%10; // column
        if(board[x][y] == ' ')
        {
            board[x][y] = 'x';
        }
        
    }
}
void remove_possible_moves()
{
    int x,y;
    for(int i=0; i<possible_moves_index; i++)
    {

        x = possible_moves[i]/10; // row
        y = possible_moves[i]%10; // column
        if((x*10+y)!=new_position)
        {
            board[x][y] = '  ';
        }
        
    }
    if(x!=0) 
    {
        board[previous_position/10][previous_position%10]=' ';
    }
    
}
void display_possible_moves(char board[8][8])
{
      int i , j , k ;

    wprintf(L" ") ;
    for( i=0 ; i<8 ; i++ )
    {
        wprintf(L"   %d" , i );
       
    } 
    wprintf(L"\n" ) ;

    for( k=0 ; k<8 ; k++ )
    {
    wprintf(L"  " ) ;
    for( i=0 ; i<34 ; i++ ) { wprintf(L"-" ) ; } wprintf(L"\n" ) ; wprintf(L"%d " , k ) ;

    for( j=0 ; j<8 ; j++ ) { wprintf(L"||%c " , display_convert(board[k][j]) ) ; }
    wprintf(L"|| \n" ) ;
    }

    wprintf(L"  " ) ;
    for( i=0 ; i<34 ; i++ ) { wprintf(L"-" ) ; } wprintf(L"\n" ) ;
}
void display()
{
    int i , j , k ;
    

    display_eliminated_list(eliminated_pieces_white);
    wprintf(L" ") ;
    for( i=0 ; i<8 ; i++ )
    {
        wprintf(L"   %d" , i );
       
    } 
    wprintf(L"\n" ) ;

    for( k=0 ; k<8 ; k++ )
    {
    wprintf(L"  " ) ;
    for( i=0 ; i<34 ; i++ ) { wprintf(L"-" ) ; } wprintf(L"\n" ) ; wprintf(L"%d " , k ) ;

    for( j=0 ; j<8 ; j++ ) { wprintf(L"||%c " , display_convert(board[k][j]) ) ; }
    wprintf(L"|| \n" ) ;
    }

    wprintf(L"  " ) ;
    for( i=0 ; i<34 ; i++ ) { wprintf(L"-" ) ; } wprintf(L"\n" ) ;
    display_eliminated_list(eliminated_pieces_black);
    wprintf(L"\n");

}


void change( int r1 , int c1 , int r2 , int c2 , int player_id)
{
    char temp ;
    temp = board[r1][c1] ;
    if(player_id==1)
    {
        if(check(r2, c2)==1)
        {
            insert_eliminated_list(board[r2][c2], &eliminated_pieces_white);
            board[r1][c1] = ' ';
            board[r2][c2] = temp; 
            // Add Elimination action
            
        }
        else
        {
            board[r1][c1] = board[r2][c2] ;
            board[r2][c2] = temp ;
        }
    }
    else if(player_id==2)
    {
        if(check2(r2, c2) == 1)
        {
            insert_eliminated_list(board[r2][c2], &eliminated_pieces_black);
            board[r1][c1] = ' ';
            board[r2][c2] = temp;
            // Add Elimination action

        }
        else
        {
            board[r1][c1] = board[r2][c2] ;
            board[r2][c2] = temp ;
        }
    }
    else
    {
        wprintf(L"Error : Take action !");
    }
}

void pawn( int r1 , int c1) // paido
{

    possible_moves_index=0;
   if( r1 == 1 )
    {
        if( board[r1+1][c1] == ' ' )
        {
            //wprintf(L"%d%d , " , r1+1 , c1 );
            if( r1+1 <= 7 && c1 <= 7 )
            {
                possible_moves[possible_moves_index] = (r1+1)*10+c1;
                possible_moves_index++;
            }
            
            //possible_moves[possible_moves_index]=(r1+1)*10+c1; 
            //board_possible_moves[r1+1][c1]='x';
            //possible_moves_index++;  
        }
        

        if( board[r1+2][c1] == ' ' )
        {
            //wprintf(L"%d%d , " , r1+2 , c1 );
            if( r1+2 <= 7 && c1 <= 7 )
            {
                possible_moves[possible_moves_index] = (r1+2)*10+c1;
                possible_moves_index++;
            }
            //board_possible_moves[r1+2][c1]='x';
            //possible_moves[possible_moves_index]=(r1+2)*10+c1; 
            //possible_moves_index++;  
        }
        
        
        if( check(r1+1 , c1+1) == 1 )
        {
             //wprintf(L"%d%d* , " , r1+1 , c1+1 );
             if( r1+1 <= 7 && c1+1 <= 7 )
             {
                possible_moves[possible_moves_index] = (r1+1)*10+c1+1;
                possible_moves_index++;
             }
             
             //board_possible_moves[r1+1][c1+1]='x';
             //possible_moves[possible_moves_index]=(r1+1)*10+c1+1; 
             //possible_moves_index++;  
        }
       

        if( check(r1+1 , c1-1) == 1 )
        {
            //wprintf(L"%d%d* , " , r1+1 , c1-1 );
            if( r1+1 <= 7 && c1-1 <= 7 && c1-1 >=0)
            {
                possible_moves[possible_moves_index] = (r1+1)*10+c1-1;
                possible_moves_index++;
            }
            
            //board_possible_moves[r1+1][c1-1]='x';
            //possible_moves[possible_moves_index]=(r1+1)*10+c1-1; 
            //possible_moves_index++;  
        }
        

    }
    else
    {
        if(board[r1+1][c1] == ' ' )
        {
            //wprintf(L"%d%d , " , r1+1 , c1 ) ;
            if( r1+1 <= 7 && c1 <= 7 )
            {
                possible_moves[possible_moves_index] = (r1+1)*10+c1;
                possible_moves_index++;
            }
            
            
        }
        
        

        if( check(r1+1 , c1+1) == 1 )
        {
            //wprintf(L"%d%d* , " , r1+1 , c1+1 ) ;
            if( r1+1 <= 7 && c1+1 <= 7 )
            {
                possible_moves[possible_moves_index] = (r1+1)*10+c1+1;
                possible_moves_index++;
            }
            
        }
        

        if( check(r1+1 , c1-1) == 1 )
        {
            //wprintf(L"%d%d* , " , r1+1 , c1-1 ) ;
            if( r1+1 <= 7 && c1-1 <= 7  && c1-1 >=0)
            {
                possible_moves[possible_moves_index] = (r1+1)*10+c1-1;
                possible_moves_index++;
            }
            
        }
        
    }
    //display_possible_moves(board_possible_moves);
    wprintf(L"Possible moves %c : \n", display_convert(board[r1][c1]));
    if(possible_moves_index >= 1)
    {
        for (int i = 0; i < possible_moves_index; i++)
        {
            wprintf(L" %d :", possible_moves[i]);
        }
    }
    else
    {
        wprintf(L"No possible moves -> ");
    }
    
    

}

void rook( int r1 , int c1, int player_id)
{

    possible_moves_index=0;
    
    int i , j , n ;

    n=c1;

    if(player_id==1)
    {
        if(n!=0)
    {
        while( board[r1][n-1] == ' ' || check(r1, n-1)==1)
        {
            if( n == 0 ) { break ; }
            possible_moves[possible_moves_index] = (r1)*10+n-1;
            possible_moves_index++;
            if(check(r1, n-1)==1) { break ; }
            //wprintf(L"%d%d , " , r1 , n-1 ) ;
            n-- ;
        }
    }
    
    
    

    n=c1 ;
    if(n!=7)
    {
        while( board[r1][n+1] == ' '|| check(r1, n+1)==1)
        {
            possible_moves[possible_moves_index] = (r1)*10+n+1;
            possible_moves_index++;
            if(check(r1, n+1)==1) {break;}
            //wprintf(L"%d%d , " , r1 , n+1 ) ;
            n++;
        }
    }
    


    n = r1 ;
    if(n!=0)
    {
        while( board[n-1][c1] == ' ' || check(n-1, c1)==1)
        {
            if( n == 0 ) { break ; }
            possible_moves[possible_moves_index] = (n-1)*10+c1;
            possible_moves_index++;
            if( check(n-1, c1)==1 ) { break; }
            //wprintf(L"%d%d , " , r1 , n-1 ) ;
            n-- ;
        }
    }

    n = r1 ;
    if(n!=7)
    {
        while( board[n+1][c1] == ' ' || check(n+1, c1)==1)
        {
            if( n == 7 ) { break ; }
            possible_moves[possible_moves_index] = (n+1)*10+c1;
            possible_moves_index++;
            if(check(n+1, c1)==1) {break;}
            //wprintf(L"%d%d , " , r1 , n-1 ) ;
            n++ ;
        }
    }
    }
    else if(player_id==2)
    {
        if(n!=0)
    {
        while( board[r1][n-1] == ' ' || check2(r1, n-1)==1)
        {
            if( n == 0 ) { break ; }
            possible_moves[possible_moves_index] = (r1)*10+n-1;
            possible_moves_index++;
            if(check2(r1, n-1)==1) { break ; }
            //wprintf(L"%d%d , " , r1 , n-1 ) ;
            n-- ;
        }
    }
    
    
    

    n=c1 ;
    if(n!=7)
    {
        while( board[r1][n+1] == ' '|| check2(r1, n+1)==1)
        {
            possible_moves[possible_moves_index] = (r1)*10+n+1;
            possible_moves_index++;
            if(check2(r1, n+1)==1) {break;}
            //wprintf(L"%d%d , " , r1 , n+1 ) ;
            n++;
        }
    }
    



    n = r1 ;
    if(n!=0)
    {
        while( board[n-1][c1] == ' ' || check2(n-1, c1)==1)
        {
            if( n == 0 ) { break ; }
            possible_moves[possible_moves_index] = (n-1)*10+c1;
            possible_moves_index++;
            if( check2(n-1, c1)==1 ) { break; }
            //wprintf(L"%d%d , " , r1 , n-1 ) ;
            n-- ;
        }
    }

    n = r1 ;
    if(n!=7)
    {
        while( board[n+1][c1] == ' ' || check2(n+1, c1)==1)
        {
            if( n == 7 ) { break ; }
            possible_moves[possible_moves_index] = (n+1)*10+c1;
            possible_moves_index++;
            if(check2(n+1, c1)==1) {break;}
            //wprintf(L"%d%d , " , r1 , n-1 ) ;
            n++ ;
        }
    }
    }else
    {
        wprintf(L"\nError : Rook ");
    }
    //display_possible_moves(board_possible_moves);
    wprintf(L"Possible moves %c : \n", display_convert(board[r1][c1]));
    if(possible_moves_index >= 1)
    {
        for (int i = 0; i < possible_moves_index; i++)
        {
            wprintf(L" %d :", possible_moves[i]);
        }
    }
    else
    {
        wprintf(L"No possible moves -> ");
    }

}

void knight( int r1 , int c1, int player_id )
{
    possible_moves_index=0;

    if(player_id==1)
    {
        if( board[r1+2][c1+1] == ' ' || check(r1+2, c1+1)==1 )
        {
            if(r1+2 <= 7 && r1+2 >= 0 && c1+1 >= 0 && c1+1 <=7)
            {
                possible_moves[possible_moves_index] = (r1+2)*10+c1+1;
                possible_moves_index++;
            }
            
            //wprintf(L"%d%d, " , r1+2 ,c1+1) ;
        }
         
        if( board[r1+2][c1-1] == ' ' || check(r1+2, c1-1)==1) 
        { 
            if(r1+2 <= 7 && r1+2 >= 0 && c1-1 >= 0 && c1-1 <=7)
            {
                possible_moves[possible_moves_index] = (r1+2)*10+c1-1;
                possible_moves_index++;
            }
        }

        if( board[r1+1][c1+2] == ' ' || check(r1+1, c1+2)==1) 
        {  
            if(r1+1 <= 7 && r1+1 >= 0 && c1+2 >= 0 && c1+2 <=7)
            {
                possible_moves[possible_moves_index] = (r1+1)*10+c1+2;
                possible_moves_index++;
            }
        }
        if( board[r1-1][c1+2] == ' ' || check(r1-1, c1+2)==1) 
        {  
            if(r1-1 <= 7 && r1-1 >= 0 && c1+2 >= 0 && c1+2 <=7)
            {
                possible_moves[possible_moves_index] = (r1-1)*10+c1+2;
                possible_moves_index++;
            }
        }

        if( board[r1-2][c1-1] == ' ' || check(r1-2, c1-1)==1 )
        {
            if(r1-2 <= 7 && r1-2 >= 0 && c1-1 >= 0 && c1-1 <=7)
            {
                possible_moves[possible_moves_index] = (r1-2)*10+c1-1;
                possible_moves_index++;
            }
        }

        if( board[r1-2][c1+1] == ' ' || check(r1-2, c1+1)==1) 
        {
            if(r1-2 <= 7 && r1-2 >= 0 && c1+1 >= 0 && c1+1 <=7)
            {
                possible_moves[possible_moves_index] = (r1-2)*10+c1+1;
                possible_moves_index++;
            }
        }
        

        if( board[r1+1][c1-2] == ' ' || check(r1+1, c1-2)==1) 
        {
            if(r1+1 <= 7 && r1+1 >= 0 && c1-2 >= 0 && c1-2 <=7)
            {
                possible_moves[possible_moves_index] = (r1+1)*10+c1-2;
                possible_moves_index++;
            }
        }
    

        if( board[r1-1][c1-2] == ' ' || check(r1-1, c1-2)==1 )
        {
            if(r1-1 <= 7 && r1-1 >= 0 && c1-2 >= 0 && c1-2 <=7)
            {
                possible_moves[possible_moves_index] = (r1-1)*10+c1-2;
                possible_moves_index++;
            }
                
        }
    }
    else if(player_id==2)
    {
        if( board[r1+2][c1+1] == ' ' || check2(r1+2, c1+1)==1 )
        {
            if(r1+2 <= 7 && r1+2 >= 0 && c1+1 >= 0 && c1+1 <=7)
            {
                possible_moves[possible_moves_index] = (r1+2)*10+c1+1;
                possible_moves_index++;
            }
            
            //wprintf(L"%d%d, " , r1+2 ,c1+1) ;
        }
         
        if( board[r1+2][c1-1] == ' ' || check2(r1+2, c1-1)==1) 
        { 
            if(r1+2 <= 7 && r1+2 >= 0 && c1-1 >= 0 && c1-1 <=7)
            {
                possible_moves[possible_moves_index] = (r1+2)*10+c1-1;
                possible_moves_index++;
            }
        }

        if( board[r1+1][c1+2] == ' ' || check2(r1+1, c1+2)==1) 
        {  
            if(r1+1 <= 7 && r1+1 >= 0 && c1+2 >= 0 && c1+2 <=7)
            {
                possible_moves[possible_moves_index] = (r1+1)*10+c1+2;
                possible_moves_index++;
            }
        }
        if( board[r1-1][c1+2] == ' ' || check2(r1-1, c1+2)==1) 
        {  
            if(r1-1 <= 7 && r1-1 >= 0 && c1+2 >= 0 && c1+2 <=7)
            {
                possible_moves[possible_moves_index] = (r1-1)*10+c1+2;
                possible_moves_index++;
            }
        }

        if( board[r1-2][c1-1] == ' ' || check2(r1-2, c1-1)==1 )
        {
            if(r1-2 <= 7 && r1-2 >= 0 && c1-1 >= 0 && c1-1 <=7)
            {
                possible_moves[possible_moves_index] = (r1-2)*10+c1-1;
                possible_moves_index++;
            }
        }

        if( board[r1-2][c1+1] == ' ' || check2(r1-2, c1+1)==1) 
        {
            if(r1-2 <= 7 && r1-2 >= 0 && c1+1 >= 0 && c1+1 <=7)
            {
                possible_moves[possible_moves_index] = (r1-2)*10+c1+1;
                possible_moves_index++;
            }
        }
        

        if( board[r1+1][c1-2] == ' ' || check2(r1+1, c1-2)==1) 
        {
            if(r1+1 <= 7 && r1+1 >= 0 && c1-2 >= 0 && c1-2 <=7)
            {
                possible_moves[possible_moves_index] = (r1+1)*10+c1-2;
                possible_moves_index++;
            }
        }
    

        if( board[r1-1][c1-2] == ' ' || check2(r1-1, c1-2)==1 )
        {
            if(r1-1 <= 7 && r1-1 >= 0 && c1-2 >= 0 && c1-2 <=7)
            {
                possible_moves[possible_moves_index] = (r1-1)*10+c1-2;
                possible_moves_index++;
            }
                
        }
    }
    else{
        wprintf(L"Error : Knight");
    }
    

    //display_possible_moves(board_possible_moves);
    wprintf(L"Possible moves %c : \n", display_convert(board[r1][c1]));
    if(possible_moves_index >= 1)
    {
        for (int i = 0; i < possible_moves_index; i++)
        {
            wprintf(L" %d :", possible_moves[i]);
        }
    }
    else
    {
        wprintf(L"No possible moves -> ");
    }
}


void bishop( int r1 , int c1, int player_id)
{
    int a , b , c , d ;
    possible_moves_index=0;

    
    if(player_id==1)
    {
        a = 1 , b = 1 ;

    while( board[r1-a][c1+b] == ' ' || check(r1-a, c1+b)==1)
    {
        if( (r1-a) == -1 || (c1+b) == 8 ) break ;
        possible_moves[possible_moves_index] = (r1-a)*10+c1+b;
        possible_moves_index++;
        if(check(r1-a, c1+b)==1) {break;}
        //wprintf(L"%d%d , " , r1-a , c1+b ) ;
        a++ ;
        b++ ;
    }


    a = 1 , b = 1 ;

    while( board[r1+a][c1-b] == ' ' || check(r1+a, c1-b)==1)
    {
        if( (r1+a) == 8 || (c1-b) == -1 ) break ;
        possible_moves[possible_moves_index] = (r1+a)*10+c1-b;
        possible_moves_index++;
        if(check(r1+a, c1-b)==1) {break;}
        //wprintf(L"%d%d , " , r1+a , c1-b ) ;
        a++ ;
        b++ ;
    }

    a = 1 , b = 1 ;


    while( board[r1+a][c1+b] == ' ' || check(r1+a, c1+b)==1 )
    {
        if( (r1+a) == 8 || (c1+b) == 8 ) break ;
        possible_moves[possible_moves_index] = (r1+a)*10+c1+b;
        possible_moves_index++;
        if(check(r1+a, c1+b)==1) {break;}
        //wprintf(L"%d%d , " , r1+a , c1+b ) ;
        a++ ;
        b++ ;
    }

    a = 1 ;
    b = 1 ;

    while( board[r1-a][c1-b] == ' ' || check(r1-a, c1-b)==1 )
    {
        if( (r1-a) == -1 || (c1-b) == -1 ) break ;
        possible_moves[possible_moves_index] = (r1-a)*10+c1-b;
        possible_moves_index++;
        if(check(r1-a, c1-b)==1) {break;}
        //wprintf(L"%d%d , " , r1-a , c1-b ) ;
        a++ ;
        b++ ;
    }
    }else if(player_id==2)
    {
           a = 1 , b = 1 ;

    while( board[r1-a][c1+b] == ' ' || check2(r1-a, c1+b)==1)
    {
        if( (r1-a) == -1 || (c1+b) == 8 ) break ;
        possible_moves[possible_moves_index] = (r1-a)*10+c1+b;
        possible_moves_index++;
        if(check2(r1-a, c1+b)==1) {break;}
        //wprintf(L"%d%d , " , r1-a , c1+b ) ;
        a++ ;
        b++ ;
    }


    a = 1 , b = 1 ;

    while( board[r1+a][c1-b] == ' ' || check2(r1+a, c1-b)==1)
    {
        if( (r1+a) == 8 || (c1-b) == -1 ) break ;
        possible_moves[possible_moves_index] = (r1+a)*10+c1-b;
        possible_moves_index++;
        if(check2(r1+a, c1-b)==1) {break;}
        //wprintf(L"%d%d , " , r1+a , c1-b ) ;
        a++ ;
        b++ ;
    }

    a = 1 , b = 1 ;


    while( board[r1+a][c1+b] == ' ' || check2(r1+a, c1+b)==1 )
    {
        if( (r1+a) == 8 || (c1+b) == 8 ) break ;
        possible_moves[possible_moves_index] = (r1+a)*10+c1+b;
        possible_moves_index++;
        if(check2(r1+a, c1+b)==1) {break;}
        //wprintf(L"%d%d , " , r1+a , c1+b ) ;
        a++ ;
        b++ ;
    }

    a = 1 ;
    b = 1 ;

    while( board[r1-a][c1-b] == ' ' || check2(r1-a, c1-b)==1 )
    {
        if( (r1-a) == -1 || (c1-b) == -1 ) break ;
        possible_moves[possible_moves_index] = (r1-a)*10+c1-b;
        possible_moves_index++;
        if(check2(r1-a, c1-b)==1) {break;}
        //wprintf(L"%d%d , " , r1-a , c1-b ) ;
        a++ ;
        b++ ;
    }

    }
    else 
    {
        wprintf(L"Error : Bishop ");
    }
    



    //display_possible_moves(board_possible_moves);
    wprintf(L"Possible moves %c : \n", display_convert(board[r1][c1]));
    if(possible_moves_index >= 1)
    {
        for (int i = 0; i < possible_moves_index; i++)
        {
            wprintf(L" %d :", possible_moves[i]);
        }
    }
    else
    {
        wprintf(L"No possible moves -> ");
    }

}

void king( int r1 , int c1, int player_id )
{
    
    possible_moves_index=0;
    if(player_id==1)
    {
        if( board[r1][c1+1] == ' ' ||  check(r1, c1+1)==1)
        {
            if(r1>=0 && r1 <=7 && c1+1 >=0 && c1+1 <=7)
            {
                possible_moves[possible_moves_index] = r1*10+c1+1;
                possible_moves_index++;
                
            }
            //wprintf(L"%d%d , " , r1 , c1+1 ) ;
            
        } 
        if( board[r1][c1-1] == ' ' ||  check(r1, c1-1)==1)
        {
            //wprintf(L"%d%d , " , r1 , c1+1 ) ;
            if(r1>=0 && r1 <=7 && c1-1 >=0 && c1-1 <=7)
            {
                possible_moves[possible_moves_index] = r1*10+c1-1;
                possible_moves_index++;
            }
                
        } 
        if( board[r1-1][c1] == ' ' ||  check(r1-1, c1)==1)
        {
            if(r1-1>=0 && r1-1 <=7 && c1>=0 && c1<=7)
            {
                possible_moves[possible_moves_index] = (r1-1)*10+c1;
                possible_moves_index++;
            }
            //wprintf(L"%d%d , " , r1 , c1+1 ) ;
            
        } 
        if( board[r1-1][c1+1] == ' ' ||  check(r1-1, c1+1)==1)
        {
            if(r1-1>=0 && r1-1 <=7 && c1+1 >=0 && c1+1 <=7)
            {
                possible_moves[possible_moves_index] = (r1-1)*10+c1+1;
                possible_moves_index++;
            }
            //wprintf(L"%d%d , " , r1 , c1+1 ) ;
            
        } 
        if( board[r1-1][c1-1] == ' ' ||  check(r1-1, c1-1)==1)
        {
            if(r1-1>=0 && r1-1<=7 && c1-1 >=0 && c1-1 <=7)
            {
                possible_moves[possible_moves_index] = (r1-1)*10+c1-1;
                possible_moves_index++;
            }
            //wprintf(L"%d%d , " , r1 , c1+1 ) ;
            
        } 
        if( board[r1+1][c1] == ' ' ||  check(r1+1, c1)==1)
        {
            if(r1+1>=0 && r1+1<=7 && c1 >=0 && c1 <=7)
            {
                possible_moves[possible_moves_index] = (r1+1)*10+c1;
                possible_moves_index++;
            }
            //wprintf(L"%d%d , " , r1 , c1+1 ) ;
         
        } 
        if( board[r1+1][c1+1] == ' ' ||  check(r1+1, c1+1)==1)
        {
            if(r1+1>=0 && r1+1 <=7 && c1+1 >=0 && c1+1 <=7)
            {
                possible_moves[possible_moves_index] = (r1+1)*10+c1+1;
                possible_moves_index++;
            }
            //wprintf(L"%d%d , " , r1 , c1+1 ) ;
            
        } 
        if( board[r1+1][c1-1] == ' ' ||  check(r1+1, c1-1)==1)
        {
            if(r1+1>=0 && r1+1<=7 && c1-1 >=0 && c1-1 <=7)
            {
                possible_moves[possible_moves_index] = (r1+1)*10+c1-1;
                possible_moves_index++;
            }
            //wprintf(L"%d%d , " , r1 , c1+1 ) ;
            
        } 
    }
    else if(player_id==2)
    {

        if(r1>=0 && r1 <=7 && c1+1 >=0 && c1+1 <=7)
        {
            if( board[r1][c1+1] == ' ' ||  check2(r1, c1+1)==1)
            {
                possible_moves[possible_moves_index] = r1*10+c1+1;
                possible_moves_index++;
            }
        } 
        if(r1>=0 && r1 <=7 && c1-1 >=0 && c1-1 <=7)
        {
            //wprintf(L"%d%d , " , r1 , c1+1 ) ;
            if( board[r1][c1-1] == ' ' ||  check2(r1, c1-1)==1)
            {
                possible_moves[possible_moves_index] = r1*10+c1-1;
                possible_moves_index++;
            }
        } 
        if(r1-1>=0 && r1-1 <=7 && c1>=0 && c1<=7)
        {
            
            if( board[r1-1][c1] == ' ' ||  check2(r1-1, c1)==1)
            {
                possible_moves[possible_moves_index] = (r1-1)*10+c1;
                possible_moves_index++;

            }
        } 
        if(r1-1>=0 && r1-1 <=7 && c1+1 >=0 && c1+1 <=7)
        {
            
            if( board[r1-1][c1+1] == ' ' ||  check2(r1-1, c1+1)==1)
            {
                possible_moves[possible_moves_index] = (r1-1)*10+c1+1;
                possible_moves_index++;

            }
        } 
        if(r1-1>=0 && r1-1<=7 && c1-1 >=0 && c1-1 <=7)
        {
            if( board[r1-1][c1-1] == ' ' ||  check2(r1-1, c1-1)==1)
            {
                possible_moves[possible_moves_index] = (r1-1)*10+c1-1;
                possible_moves_index++;
            }
        } 
        if(r1+1>=0 && r1+1<=7 && c1 >=0 && c1 <=7)
        {
            
            if( board[r1+1][c1] == ' ' ||  check2(r1+1, c1)==1)
            {
                possible_moves[possible_moves_index] = (r1+1)*10+c1;
                possible_moves_index++;
             
            }
        } 
        if(r1+1>=0 && r1+1 <=7 && c1+1 >=0 && c1+1 <=7)
        {
            if( board[r1+1][c1+1] == ' ' ||  check2(r1+1, c1+1)==1)
            {
                possible_moves[possible_moves_index] = (r1+1)*10+c1+1;
                possible_moves_index++;

            }
        } 
        if(r1+1>=0 && r1+1<=7 && c1-1 >=0 && c1-1 <=7)
        {
            
            if( board[r1+1][c1-1] == ' ' ||  check2(r1+1, c1-1)==1)
            {
                possible_moves[possible_moves_index] = (r1+1)*10+c1-1;
                possible_moves_index++;

            }
        } 
    }
    else
    {
        wprintf(L"king : Error");
    }
    //display_possible_moves(board_possible_moves);
    wprintf(L"Possible moves %c : \n", display_convert(board[r1][c1]));
    if(possible_moves_index >= 1)
    {
        for (int i = 0; i < possible_moves_index; i++)
        {
            wprintf(L" %d :", possible_moves[i]);
        }
    }
    else
    {
        wprintf(L"No possible moves -> ");
    }

    
}

void queen( int r1 , int c1, int player_id)
{
   int a , b , c , d ;
   int i , j , n ;
   n=c1;


    possible_moves_index=0;

    
    if(player_id==1)
    {
        a = 1 , b = 1 ;

    while( board[r1-a][c1+b] == ' ' || check(r1-a, c1+b)==1)
    {
        if( (r1-a) == -1 || (c1+b) == 8 ) break ;
        possible_moves[possible_moves_index] = (r1-a)*10+c1+b;
        possible_moves_index++;
        if(check(r1-a, c1+b)==1) {break;}
        //wprintf(L"%d%d , " , r1-a , c1+b ) ;
        a++ ;
        b++ ;
    }


    a = 1 , b = 1 ;

    while( board[r1+a][c1-b] == ' ' || check(r1+a, c1-b)==1)
    {
        if( (r1+a) == 8 || (c1-b) == -1 ) break ;
        possible_moves[possible_moves_index] = (r1+a)*10+c1-b;
        possible_moves_index++;
        if(check(r1+a, c1-b)==1) {break;}
        //wprintf(L"%d%d , " , r1+a , c1-b ) ;
        a++ ;
        b++ ;
    }

    a = 1 , b = 1 ;


    while( board[r1+a][c1+b] == ' ' || check(r1+a, c1+b)==1 )
    {
        if( (r1+a) == 8 || (c1+b) == 8 ) break ;
        possible_moves[possible_moves_index] = (r1+a)*10+c1+b;
        possible_moves_index++;
        if(check(r1+a, c1+b)==1) {break;}
        //wprintf(L"%d%d , " , r1+a , c1+b ) ;
        a++ ;
        b++ ;
    }

    a = 1 ;
    b = 1 ;

    while( board[r1-a][c1-b] == ' ' || check(r1-a, c1-b)==1 )
    {
        if( (r1-a) == -1 || (c1-b) == -1 ) break ;
        possible_moves[possible_moves_index] = (r1-a)*10+c1-b;
        possible_moves_index++;
        if(check(r1-a, c1-b)==1) {break;}
        //wprintf(L"%d%d , " , r1-a , c1-b ) ;
        a++ ;
        b++ ;
    }
        if(n!=0)
    {
        while( board[r1][n-1] == ' ' || check(r1, n-1)==1)
        {
            if( n == 0 ) { break ; }
            possible_moves[possible_moves_index] = (r1)*10+n-1;
            possible_moves_index++;
            if(check(r1, n-1)==1) { break ; }
            //wprintf(L"%d%d , " , r1 , n-1 ) ;
            n-- ;
        }
    }
    
    
    

    n=c1 ;
    if(n!=7)
    {
        while( board[r1][n+1] == ' '|| check(r1, n+1)==1)
        {
            possible_moves[possible_moves_index] = (r1)*10+n+1;
            possible_moves_index++;
            if(check(r1, n+1)==1) {break;}
            //wprintf(L"%d%d , " , r1 , n+1 ) ;
            n++;
        }
    }
    



    n = r1 ;
    if(n!=0)
    {
        while( board[n-1][c1] == ' ' || check(n-1, c1)==1)
        {
            if( n == 0 ) { break ; }
            possible_moves[possible_moves_index] = (n-1)*10+c1;
            possible_moves_index++;
            if( check(n-1, c1)==1 ) { break; }
            //wprintf(L"%d%d , " , r1 , n-1 ) ;
            n-- ;
        }
    }

    n = r1 ;
    if(n!=7)
    {
        while( board[n+1][c1] == ' ' || check(n+1, c1)==1)
        {
            if( n == 7 ) { break ; }
            possible_moves[possible_moves_index] = (n+1)*10+c1;
            possible_moves_index++;
            if(check(n+1, c1)==1) {break;}
            //wprintf(L"%d%d , " , r1 , n-1 ) ;
            n++ ;
        }
    }
    }else if(player_id==2)
    {
           a = 1 , b = 1 ;

    while( board[r1-a][c1+b] == ' ' || check2(r1-a, c1+b)==1)
    {
        if( (r1-a) == -1 || (c1+b) == 8 ) break ;
        possible_moves[possible_moves_index] = (r1-a)*10+c1+b;
        possible_moves_index++;
        if(check2(r1-a, c1+b)==1) {break;}
        //wprintf(L"%d%d , " , r1-a , c1+b ) ;
        a++ ;
        b++ ;
    }


    a = 1 , b = 1 ;

    while( board[r1+a][c1-b] == ' ' || check2(r1+a, c1-b)==1)
    {
        if( (r1+a) == 8 || (c1-b) == -1 ) break ;
        possible_moves[possible_moves_index] = (r1+a)*10+c1-b;
        possible_moves_index++;
        if(check2(r1+a, c1-b)==1) {break;}
        //wprintf(L"%d%d , " , r1+a , c1-b ) ;
        a++ ;
        b++ ;
    }

    a = 1 , b = 1 ;


    while( board[r1+a][c1+b] == ' ' || check2(r1+a, c1+b)==1 )
    {
        if( (r1+a) == 8 || (c1+b) == 8 ) break ;
        possible_moves[possible_moves_index] = (r1+a)*10+c1+b;
        possible_moves_index++;
        if(check2(r1+a, c1+b)==1) {break;}
        //wprintf(L"%d%d , " , r1+a , c1+b ) ;
        a++ ;
        b++ ;
    }

    a = 1 ;
    b = 1 ;

    while( board[r1-a][c1-b] == ' ' || check2(r1-a, c1-b)==1 )
    {
        if( (r1-a) == -1 || (c1-b) == -1 ) break ;
        possible_moves[possible_moves_index] = (r1-a)*10+c1-b;
        possible_moves_index++;
        if(check2(r1-a, c1-b)==1) {break;}
        //wprintf(L"%d%d , " , r1-a , c1-b ) ;
        a++ ;
        b++ ;
    }

     if(n!=0)
    {
        while( board[r1][n-1] == ' ' || check2(r1, n-1)==1)
        {
            if( n == 0 ) { break ; }
            possible_moves[possible_moves_index] = (r1)*10+n-1;
            possible_moves_index++;
            if(check2(r1, n-1)==1) { break ; }
            //wprintf(L"%d%d , " , r1 , n-1 ) ;
            n-- ;
        }
    }
    
    
    

    n=c1 ;
    if(n!=7)
    {
        while( board[r1][n+1] == ' '|| check2(r1, n+1)==1)
        {
            possible_moves[possible_moves_index] = (r1)*10+n+1;
            possible_moves_index++;
            if(check2(r1, n+1)==1) {break;}
            //wprintf(L"%d%d , " , r1 , n+1 ) ;
            n++;
        }
    }
    



    n = r1 ;
    if(n!=0)
    {
        while( board[n-1][c1] == ' ' || check2(n-1, c1)==1)
        {
            if( n == 0 ) { break ; }
            possible_moves[possible_moves_index] = (n-1)*10+c1;
            possible_moves_index++;
            if( check2(n-1, c1)==1 ) { break; }
            //wprintf(L"%d%d , " , r1 , n-1 ) ;
            n-- ;
        }
    }

    n = r1 ;
    if(n!=7)
    {
        while( board[n+1][c1] == ' ' || check2(n+1, c1)==1)
        {
            if( n == 7 ) { break ; }
            possible_moves[possible_moves_index] = (n+1)*10+c1;
            possible_moves_index++;
            if(check2(n+1, c1)==1) {break;}
            //wprintf(L"%d%d , " , r1 , n-1 ) ;
            n++ ;
        }
    }

    }
    else 
    {
        wprintf(L"Error : Bishop ");
    }
    



    //display_possible_moves(board_possible_moves);
    wprintf(L"Possible moves %c : \n", display_convert(board[r1][c1]));
    if(possible_moves_index >= 1)
    {
        for (int i = 0; i < possible_moves_index; i++)
        {
            wprintf(L" %d :", possible_moves[i]);
        }
    }
    else
    {
        wprintf(L"No possible moves -> ");
    }


}

void pawnb( int r1 , int c1 ) // paido black
{
    possible_moves_index=0;


    if( r1 == 6 )
    {
        if( board[r1-1][c1] == ' ' )
        {
            //wprintf(L"%d%d , " , r1-2 , c1 ) ;
            if( r1-1 >= 0 && c1 >= 0 )
            {
                possible_moves[possible_moves_index] = (r1-1)*10+c1;
                possible_moves_index++;
            }
        }
        
        if( board[r1-2][c1] == ' ' )
        {
            //wprintf(L"%d%d , " , r1-2 , c1 ) ;
            if( r1-2 >= 0 && c1 >= 0 )
            {
                possible_moves[possible_moves_index] = (r1-2)*10+c1;
                possible_moves_index++;
            }
        }
        if( check2(r1-1 , c1+1) == 1 )
        {
             //wprintf(L"%d%d* , " , r1+1 , c1+1 );
             if( r1-1 >= 0 && c1+1 >= 0 )
             {
                possible_moves[possible_moves_index] = (r1-1)*10+c1+1;
                possible_moves_index++;
             }
             
        }
       

        if( check2(r1-1 , c1-1) == 1 )
        {
            //wprintf(L"%d%d* , " , r1+1 , c1-1 );
            if( r1-1 >= 0 && c1-1 <= 7 && c1-1 >=0) // check for outside board
            {
                possible_moves[possible_moves_index] = (r1-1)*10+c1-1;
                possible_moves_index++;
            }
            
        }
        
    }
    else
    {
        if( board[r1-1][c1] == ' ' )
        {
            //wprintf(L"%d%d , " , r1-1 , c1 ) ;
            if( r1-1 >= 0 && c1 >= 0 )
            {
                possible_moves[possible_moves_index] = (r1-1)*10+c1;
                possible_moves_index++;
            }
        }
         
        
        if( check2(r1-1 , c1+1) == 1 )
        {
             //wprintf(L"%d%d* , " , r1+1 , c1+1 );
             if( r1-1 >= 0 && c1+1 >= 0 )
             {
                possible_moves[possible_moves_index] = (r1-1)*10+c1+1;
                possible_moves_index++;
             }
             
        }
       

        if( check2(r1-1 , c1-1) == 1 )
        {
            //wprintf(L"%d%d* , " , r1+1 , c1-1 );
            if( r1-1 >= 0 && c1-1 <= 7 && c1-1 >=0) // check for outside board
            {
                possible_moves[possible_moves_index] = (r1-1)*10+c1-1;
                possible_moves_index++;
            }
            
        }
    }


    //display_possible_moves(board_possible_moves);
    wprintf(L"Possible moves %c : \n", display_convert(board[r1][c1]));
    if(possible_moves_index >= 1)
    {
        for (int i = 0; i < possible_moves_index; i++)
        {
            wprintf(L" %d :", possible_moves[i]);
        }
    }
    else
    {
        wprintf(L"No possible moves -> ");
    }

}


bool verify_possible_move(int position)
{
    for (int i = 0; i < possible_moves_index; i++)
    {
        if(position==possible_moves[i])
        return false;
    }
    return true;
    
}
void player_white()
{
    int p1 , p2 , c1 , r1 , c2 , r2, input_control;
    wprintf(L"\n\x26AB Black to move ..." ) ;
    again1:
    do
    {
        fflush(stdin);
        wprintf(L"\nEnter Position of Piece : [row.index column.index] : " ) ;
        input_control = scanf( "%d" , &p1 );
        if(input_control==0 || (p1!=0 && p1>7 && p1 < 10) || p1 > 77 || p1%10 > 7) wprintf(L"\x26A0 Input incorrect : [row.index column.index] - index 0->7");
        
    } while ( input_control==0 ||  (p1!=0 && p1>7&&p1 < 10) || p1 > 77 || p1%10 > 7 );

    c1 = p1 % 10 ;
    r1 = p1 / 10 ;

    switch( board[r1][c1] ) // Select only player's pieces.
    {
        case 'P': pawn( r1 , c1);
                  break ;
        case 'R': rook( r1 , c1, 1);
                  break ;
        case 'H': knight( r1 , c1, 1);
                  break ;
        case 'C': bishop( r1 , c1, 1 );
                  break ;
        case 'K': king( r1 , c1, 1 ) ;
                  break ;
        case 'Q': queen( r1 , c1, 1 ) ;
                  break ;
        default: wprintf(L"\x26A0 Invalid Position ! ") ; goto again1 ;
    }
    if(possible_moves_index>=1)
    {
        system( "cls" );
        append_possible_moves();
        display(); // Displays the Board
        wprintf(L"Possible moves %c : \n", display_convert(board[r1][c1]));
        for (int i = 0; i < possible_moves_index; i++)
        {
            wprintf(L" %d :", possible_moves[i]);
        }
    do
        {
            wprintf(L"\nEnter New Position of Piece : [row.index column.index] : " ) ;
            scanf( "%d" , &p2 ) ;
        } while (verify_possible_move(p2));
    }
    else
    {
        goto again1;
    }
    

    c2 = p2 % 10 ;
    r2 = p2 / 10  ;
    previous_position = r1*10+c1;
    new_position = r2*10+c2;

    change(r1,c1,r2,c2,1) ;
}

void player_black()
{
    int p1 , p2 , c1 , r1 , c2 , r2, input_control;

    wprintf(L"\n\x26AA White to move ...\n") ;
    again2:
    do
    {
        fflush(stdin);
        wprintf(L"\nEnter Position of Piece : [row.index column.index] : " ) ;
        input_control = scanf( "%d" , &p1 );
        if(input_control==0 || p1 < 10 || p1 > 77 || p1%10 > 7) 
        wprintf(L"\x26A0 Input incorrect : [row.index column.index] - index 0->7");
        
    } while ( input_control==0 || p1 < 10 || p1 > 77 || p1%10 > 7 );
    

    c1 = p1 % 10 ;
    r1 = p1 / 10 ;

    switch( board[r1][c1] )
    {
        case 'p': pawnb( r1 , c1 ) ;
                  break ;
        case 'r': rook( r1 , c1, 2) ;
                  break ;
        case 'h': knight( r1 , c1, 2) ;
                  break ;
        case 'c': bishop( r1 , c1, 2) ;
                  break ;
        case 'k': king( r1 , c1, 2 ) ;
                  break ;
        case 'q': queen( r1 , c1, 2 ) ;
                  break ;
        default: wprintf(L"\x26A0 Invalid Position ! " ) ; goto again2 ;
    }
    if(possible_moves_index>=1)
    {
        system( "cls" );
        append_possible_moves();
        display(); // Displays the Board
        //display_possible_moves(board_possible_moves);
        wprintf(L"Possible moves %c : \n", display_convert(board[r1][c1]));
        for (int i = 0; i < possible_moves_index; i++)
        {
            wprintf(L" %d :", possible_moves[i]);
        }

    do
        {
            wprintf(L"\nEnter New Position of Piece : [row.index column.index] : " ) ;
            scanf( "%d" , &p2 ) ;
        } while (verify_possible_move(p2));
    }
    else
    {
        goto again2;
    }

    c2 = p2 % 10 ;
    r2 = p2 / 10  ;

    previous_position = r1*10+c1;
    new_position = r2*10+c2;

    change(r1,c1,r2,c2,2) ;
}

// Check if there is a piece at x,y
int check(int x , int y )
{
    switch( board[x][y] )
    {
    case 'p':
    case 'r':
    case 'h':
    case 'c':
    case 'k':
    case 'q': return 1 ; break ;
    default: return 0 ;
    }
}

int check2(int x , int y )
{
    switch( board[x][y] )
    {
    case 'P':
    case 'R':
    case 'H':
    case 'C':
    case 'K':
    case 'Q': return 1 ; break ;
    default: return 0 ;
    }
}



