#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#define BOARD_SIZE 8

int RowStart,ColStart,RowEnd,ColEnd;
int LastRowStart,LastColStart,LastRowEnd,LastColEnd;
bool WhiteWin=false,BlackWin=false;
char Tura='W';
bool TrueGetMove=false;
char TargetedPiece=' ';
int kingRowState,kingColState;
char piece;
bool WhiteCheck;
bool BlackCheck;
bool checking=false;

void getMove();
bool isCheck(char (*)[8],char,int,int);

char board[BOARD_SIZE][BOARD_SIZE]={
    {'R','H','B','K','Q','B','H','R'},
    {'P','P','P','P','P','P','P','P'},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {'p','p','p','p','p','p','p','p'},
    {'r','h','b','k','q','b','h','r'}
};

void printBoard()
{
    printf("      0    1    2    3    4    5    6    7\n");
    printf("   ------------------------------------------\n");
    int i=0,n=0;
    for(i;i<BOARD_SIZE;i++){
        printf(" %d || %c || %c || %c || %c || %c || %c || %c || %c ||\n",i,board[i][0],board[i][1],board[i][2],board[i][3],board[i][4],board[i][5],board[i][6],board[i][7]);
        printf("   ------------------------------------------\n");
    }
}

void undoMove()
{
    board[RowStart][ColStart]=board[RowEnd][ColEnd];
    board[RowEnd][ColEnd]=TargetedPiece;
}

void undoMoveAndTurn()
{
    board[RowStart][ColStart]=board[RowEnd][ColEnd];
    board[RowEnd][ColEnd]=TargetedPiece;
    switch(Tura)
    {
        case 'W':
            Tura='B';
            break;
        case 'B':
            Tura='W';
            break;
        default:
            printf("Error!(Kogo Tura!?)\n");
            break;
    }
    
}

void getMove()
{
    TrueGetMove=0;
    while(!TrueGetMove){
        printf("Podaj ruch wedlug schematu(RowStart ColumnStart RowEnd Column End):");
        scanf("%d %d %d %d",&RowStart,&ColStart,&RowEnd,&ColEnd);
        if(RowStart==9 && ColStart==9 && RowEnd==9 && ColEnd==9)
        {
            if(Tura=='B'){
                WhiteWin=1;
            }
            if(Tura=='W'){
                BlackWin=1;
            }
            return;
        }
        if(RowStart==8 && ColStart==8 && RowEnd==8 && ColEnd==8)
        {
            RowStart=LastRowStart;
            RowEnd=LastRowEnd;
            ColStart=LastColStart;
            ColEnd=LastColEnd;
            undoMoveAndTurn();
            printBoard();
            printf("Tura: %c\n",Tura);
            getMove();
            return;
        }
        if(RowStart<8 && RowStart>-1 && ColStart<8 && ColStart>-1 && RowEnd<8 && RowEnd>-1 && ColEnd<8 && ColEnd>-1)
        {
            TrueGetMove=1;
        }
    }
    LastRowStart=RowStart;
    LastColStart=ColStart;
    LastRowEnd=RowEnd;
    LastColEnd=ColEnd;
    piece=board[RowStart][ColStart];
    TargetedPiece=board[RowEnd][ColEnd];
}


// Funkcja do sprawdzania ruchu pionka
int isValidPawnMove(char board[BOARD_SIZE][BOARD_SIZE], int RowStart, int ColStart, int RowEnd, int ColEnd, char piece) {
    int direction=0;
    if(piece=='P'){direction = -1;}
    if(piece=='p'){direction = 1;}
    if(ColStart!=ColEnd && RowEnd!=RowStart-direction){
        if(ColStart!=ColEnd && (board[RowStart][ColStart]==' ' || (islower(piece)&&islower(board[RowEnd][ColEnd]))))
        {
            if(checking==1){return 0;}
            printf("Error!11\n");
            return 0;
        }
        if(ColStart!=ColEnd && (board[RowStart][ColStart]==' ' || (isupper(piece)&&isupper(board[RowEnd][ColEnd]))))
        {
            if(checking==1){return 0;}
            printf("Error!12\n");
            return 0;
        }
    }
    
    if(abs(RowEnd-RowStart)>2){if(checking==1){return 0;}printf("Error!1\n");return 0;}
    if(abs(RowEnd-RowStart)==2 && (RowStart!=1 && RowStart!=6)){if(checking==1){return 0;}printf("Error!2\n");return 0;}
    if(ColStart==ColEnd && board[RowEnd][ColEnd]!=' '){if(checking==1){return 0;}printf("Error!3\n");return 0;}
    if(ColStart!=ColEnd && board[RowEnd][ColEnd]==' '){if(checking==1){return 0;}printf("Error!4\n");return 0;}
    if(ColStart==ColEnd && RowEnd==RowStart+direction){if(checking==1){return 0;}printf("Direction is %d Error!5\n",direction);return 0;}

    return 1;
}

// Funkcja do sprawdzania ruchu wieży
int isValidRookMove(char board[BOARD_SIZE][BOARD_SIZE], int RowStart, int ColStart, int RowEnd, int ColEnd) {
    if (RowStart != RowEnd && ColStart != ColEnd) {
        return 0;  // Wieża porusza się tylko w linii poziomej lub pionowej
    }

    // Sprawdzanie, czy pole jest wolne
    int stepRow = (RowEnd > RowStart) ? 1 : (RowEnd < RowStart) ? -1 : 0;
    int stepCol = (ColEnd > ColStart) ? 1 : (ColEnd < ColStart) ? -1 : 0;

    int row = RowStart + stepRow;
    int col = ColStart + stepCol;
    
    while (row != RowEnd || col != ColEnd) {
        if (board[row][col] != ' ') {
            return 0;  // Znaleziono inną figurę na drodze
        }
        row += stepRow;
        col += stepCol;
    }
    
    return 1;
}

// Funkcja do sprawdzania ruchu konia
int isValidKnightMove(char board[BOARD_SIZE][BOARD_SIZE], int RowStart, int ColStart, int RowEnd, int ColEnd) {
    int dx = abs(RowEnd - RowStart);
    int dy = abs(ColEnd - ColStart);
    
    // Koń porusza się w kształcie litery "L" (2 w jednym kierunku, 1 w innym lub odwrotnie)
    return (dx == 2 && dy == 1) || (dx == 1 && dy == 2);
}

// Funkcja do sprawdzania ruchu gońca
int isValidBishopMove(char board[BOARD_SIZE][BOARD_SIZE], int RowStart, int ColStart, int RowEnd, int ColEnd) {
    int dx = abs(RowEnd - RowStart);
    int dy = abs(ColEnd - ColStart);

    if (dx != dy) {
        return 0;  // Goniec porusza się tylko po przekątnych
    }

    // Sprawdzanie, czy pole jest wolne
    int stepRow = (RowEnd > RowStart) ? 1 : (RowEnd < RowStart) ? -1 : 0;
    int stepCol = (ColEnd > ColStart) ? 1 : (ColEnd < ColStart) ? -1 : 0;

    int row = RowStart + stepRow;
    int col = ColStart + stepCol;
    
    while (row != RowEnd || col != ColEnd) {
        if (board[row][col] != ' ') {
            return 0;  // Znaleziono inną figurę na drodze
        }
        row += stepRow;
        col += stepCol;
    }

    return 1;
}

// Funkcja do sprawdzania ruchu króla
int isValidKingMove(char board[BOARD_SIZE][BOARD_SIZE], int RowStart, int ColStart, int RowEnd, int ColEnd, char piece) {
    int dx = abs(RowEnd - RowStart);
    int dy = abs(ColEnd - ColStart);

    // Król porusza się o jedno pole w dowolnym kierunku
    if (dx > 1 || dy > 1) {
        return 0;
    }

    // Tymczasowo wykonaj ruch króla
    char originalPiece = board[RowEnd][ColEnd];
    board[RowEnd][ColEnd] = board[RowStart][ColStart];
    board[RowStart][ColStart] = ' ';

    // Sprawdź, czy król jest pod szachem po ruchu
    bool isInCheck;
    isInCheck=isCheck(board,piece,kingRowState,kingColState);

    // Cofnij ruch
    board[RowStart][ColStart] = board[RowEnd][ColEnd];
    board[RowEnd][ColEnd] = originalPiece;

    // Jeśli jest pod szachem, ruch jest nieprawidłowy
    if (isInCheck) {
        return 0;
    }

    return 1;
}


// Funkcja do sprawdzania ruchu królowej
int isValidQueenMove(char board[BOARD_SIZE][BOARD_SIZE], int RowStart, int ColStart, int RowEnd, int ColEnd) {
    // Królowa porusza się jak wieża lub goniec
    if (RowStart == RowEnd || ColStart == ColEnd) {
        return isValidRookMove(board, RowStart, ColStart, RowEnd, ColEnd);  // Sprawdzamy jak wieża
    }
    if (abs(RowEnd - RowStart) == abs(ColEnd - ColStart)) {
        return isValidBishopMove(board, RowStart, ColStart, RowEnd, ColEnd);  // Sprawdzamy jak goniec
    }
    return 0;
}

// Funkcja do ruchu dowolnej figury
int movePiece(char board[BOARD_SIZE][BOARD_SIZE], int RowStart, int ColStart, int RowEnd, int ColEnd, char piece) {
    if(board[RowEnd][ColEnd]!=' '){
        if((islower(board[RowStart][ColStart]) && islower(board[RowEnd][ColEnd]))||(isupper(board[RowStart][ColStart]) && isupper(board[RowEnd][ColEnd])))
        {
            if(checking==1)
            {
                return 0;
            }
            printf("Nieprawidłowy ruch!1\n");
            return 0;
        }
    }

     if ((Tura == 'W' && !islower(piece)) || (Tura == 'B' && !isupper(piece))) {
        if(checking==1)
            {
                return 0;
            }
        printf("Nie możesz ruszyć figurą przeciwnika!\n");
        return 0;
    }

    if (piece == 'P' || piece == 'p') {
        if (!isValidPawnMove(board, RowStart, ColStart, RowEnd, ColEnd, piece)) {
            if(checking==1)
            {
                return 0;
            }
            printf("Nieprawidłowy ruch pionka!\n");
            return 0;
        }
    }

    switch (piece)
    {
        case 'R':
        if (!isValidRookMove(board, RowStart, ColStart, RowEnd, ColEnd)) {
            if(checking==1)
            {
                return 0;
            }
            printf("Nieprawidłowy ruch wieży!\n");
            return 0;
        }
        break;

        case 'r':
        if (!isValidRookMove(board, RowStart, ColStart, RowEnd, ColEnd)) {
            if(checking==1)
            {
                return 0;
            }
            printf("Nieprawidłowy ruch wieży!\n");
            return 0;
        }
        break;

        case 'H':
        if (!isValidKnightMove(board, RowStart, ColStart, RowEnd, ColEnd)) {
            if(checking==1)
            {
                return 0;
            }
            printf("Nieprawidłowy ruch konia!\n");
            return 0;
        }
        break;
        
        case 'h':
        if (!isValidKnightMove(board, RowStart, ColStart, RowEnd, ColEnd)) {
            if(checking==1)
            {
                return 0;
            }
            printf("Nieprawidłowy ruch konia!\n");
            return 0;
        }
        break;
        
        case 'B':
        if (!isValidBishopMove(board, RowStart, ColStart, RowEnd, ColEnd)) {
            if(checking==1)
            {
                return 0;
            }
            printf("Nieprawidłowy ruch gońca!\n");
            return 0;
        }
        break;
        //////////////////
        case 'b':
        if (!isValidBishopMove(board, RowStart, ColStart, RowEnd, ColEnd)) {
            if(checking==1)
            {
                return 0;
            }
            printf("Nieprawidłowy ruch gońca!\n");
            return 0;
        }
        break;
        
        case 'Q':
        if (!isValidQueenMove(board, RowStart, ColStart, RowEnd, ColEnd)) {
            if(checking==1)
            {
                return 0;
            }
            printf("Nieprawidłowy ruch królowej!\n");
            return 0;
        }
        break;
        
        case 'q':
        if (!isValidQueenMove(board, RowStart, ColStart, RowEnd, ColEnd)) {
            if(checking==1)
            {
                return 0;
            }
            printf("Nieprawidłowy ruch królowej!\n");
            return 0;
        }
        break;
        
        case 'K':
        if (!isValidKingMove(board,RowStart, ColStart, RowEnd, ColEnd,piece)) {
            if(checking==1)
            {
                return 0;
            }
            printf("Nieprawidłowy ruch króla!\n");
            return 0;
        }
        break;
        
        case 'k':
        if (!isValidKingMove(board,RowStart, ColStart, RowEnd, ColEnd,piece)) {
            if(checking==1)
            {
                return 0;
            }
            printf("Nieprawidłowy ruch króla!\n");
            return 0;
        }
        break;
        

        case 'p':
        break;

        case 'P':
        break;

        default:
        if(checking==1)
            {
                return 0;
            }
        printf("Nieprawidłowa figura do ruchu!777\n");
        return 0;
        break;
    }







    board[RowEnd][ColEnd] = board[RowStart][ColStart];
        board[RowStart][ColStart] = ' ';
    

    return 1;
}

bool isCheck(char board[BOARD_SIZE][BOARD_SIZE], char king, int kingRow, int kingCol) {
    checking = 1;
    char opponentPieces[] = {'P', 'R', 'H', 'B', 'Q', 'K'};
    if (islower(king)) { 
        for (int i = 0; i < 6; i++) opponentPieces[i] = tolower(opponentPieces[i]);
    }

    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            char piece = board[row][col];
            if (strchr(opponentPieces, piece)) {
                if (movePiece(board, row, col, kingRow, kingCol, piece)) {
                    undoMove();
                    printf("Krol %c jest pod szachem!\n",king);
                    checking=false;
                    return true;
                }
            }
        }
    }
    checking=false;
    return false;
}

bool isCheckmate(char board[BOARD_SIZE][BOARD_SIZE], char king, int kingRow, int kingCol) {
    checking=1;
    if (!isCheck(board, king, kingRow, kingCol)) return false;

    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            char piece = board[kingRow][kingCol];
            if (movePiece(board, kingRow, kingCol, row, col, piece)) {
                if (!isCheck(board, king, row, col)) {
                    undoMove();
                    checking=false;
                    return false;
                }
                undoMove();
            }
        }
    }
    checking=false;
    return true;
}

int getKingPosition(char whichKing)
{
    int i=0,n=0;
    for(i;i<BOARD_SIZE;i++)
    {
        for(n;n<BOARD_SIZE;n++)
        {
            if(board[i][n]==whichKing)
            {
                kingRowState=i;
                kingColState=n;
            }
        }
    }
}

void changeTurn()
{
    switch(Tura)
    {
        case 'W':
            Tura='B';
            break;
        case 'B':
            Tura='W';
            break;
        default:
            printf("Error!(Kogo Tura!?)\n");
            break;
    }
}

void checkingChecks()//sprawdzanie szachów
{
    getKingPosition('k');                           
        if(isCheck(board,'k',kingRowState,kingColState))
        {
            WhiteCheck=1;
            printf("Krol bialych jest pod szachem!\n");
            if(isCheckmate(board,'k',kingRowState,kingColState))
            {
                printf("Szach-mat!\n");
                BlackWin=1;
            }
        }else{
            WhiteCheck=0;
        }
        getKingPosition('K');
        if(isCheck(board,'K',kingRowState,kingColState))
        {
            BlackCheck=1;
            printf("Krol czarnych jest pod szachem");
            if(isCheckmate(board,'K',kingRowState,kingColState))
            {
                printf("Szach-mat!\n");
                WhiteWin=1;
            }
        }else{
            BlackCheck=0;
        }                 
}


int main()
{
    printf("Jeżeli gra nie wykryła checkmate to niech przeciwnik za ruch poda 9 9 9 9!(Jeżeli taki błąd wystąpi)\n");
    printf("Przy wpisaniu 8 8 8 8 cofnie się ostatni ruch\n");
    Tura='W';
    WhiteCheck=0;
    BlackCheck=0;
    while(!WhiteWin && !BlackWin)
    {
        checkingChecks();
        if(WhiteWin==1 || BlackWin==1)
        {
            break;
        }
        printBoard();
        if(Tura=='W'){
            printf("Tura Bialego\n");
        }else{
            if(Tura='B')
            {
                printf("Tura Czarnego\n");
            }
        }
        //Powtarzanie ruchu
        moveloop:
        getMove();
        if(BlackWin==1 || WhiteWin==1){goto koniec;}
        piece=board[RowStart][ColStart];
        if(!movePiece(board,RowStart,ColStart,RowEnd,ColEnd,piece))
        {
            while(!movePiece(board,RowStart,ColStart,RowEnd,ColEnd,piece))
            {   
                printf("Tura: %c\n",Tura);
                getMove();
                if(BlackWin==1 || WhiteWin==1){goto koniec;}
                piece=board[RowStart][ColStart];

            }
        }
        checkingChecks();
        if(Tura=='W' && isCheck(board,'K',kingRowState,kingColState))
        {
            undoMoveAndTurn();
            printf("Czarny krol jest pod szachem!\n");
            goto moveloop;
        }else
        {
            if(Tura=='B' && isCheck(board,'k',kingRowState,kingColState))
            {
                undoMoveAndTurn();
                printf("Bialy krol jest pod szachem!\n");
                goto moveloop;
            }
        }
        
        changeTurn();
    }
    koniec:
    if(WhiteWin){printf("Biale wygrywaja\n!");}
    if(BlackWin){printf("Czarne Wygrywaja\n");}
    return 0;
}
