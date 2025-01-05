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
int TuraBool=1;
bool TrueGetMove=false;
char TargetedPiece=' ';
bool CheckOnWhite=false,CheckOnBlack=false;
char helpPiece;
int kingRowState,kingColState;

void getMove();

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
            break;
        }
        if(RowStart==8 && ColStart==8 && RowEnd==8 && ColEnd==8)
        {
            RowStart=LastRowStart;
            RowEnd=LastRowEnd;
            ColStart=LastColStart;
            ColEnd=LastColEnd;
            undoMove();
            printBoard();
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
    TargetedPiece=board[RowStart][ColStart];
}


// Funkcja do sprawdzania ruchu pionka
int isValidPawnMove(char board[BOARD_SIZE][BOARD_SIZE], int RowStart, int ColStart, int RowEnd, int ColEnd, char piece) {
    int direction;
    if(piece='P'){direction=-1;}
    if(piece='p'){direction=1;}
    if(ColStart!=ColEnd && RowEnd!=RowStart+direction){
        if(ColStart!=ColEnd && (board[RowStart][ColStart]==' ' || (islower(piece)&&islower(board[RowEnd][ColEnd]))))
        {
            return 0;
        }
        if(ColStart!=ColEnd && (board[RowStart][ColStart]==' ' || (isupper(piece)&&isupper(board[RowEnd][ColEnd]))))
        {
            return 0;
        }
    }
    
    if(abs(RowEnd-RowStart)>2){return 0;}
    if(abs(RowEnd-RowStart)==2 && (RowStart!=1 && RowStart!=6)){return 0;}
    if(ColStart==ColEnd && board[RowEnd][ColEnd]!=' '){return 0;}
    if(ColStart!=ColEnd && board[RowEnd][ColEnd]==' '){return 0;}
    if(ColStart==ColEnd && RowEnd==RowStart+direction){return 0;}

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
int isValidKingMove(int RowStart, int ColStart, int RowEnd, int ColEnd) {
    int dx = abs(RowEnd - RowStart);
    int dy = abs(ColEnd - ColStart);

    // Król porusza się o jedno pole w dowolnym kierunku
    return (dx <= 1 && dy <= 1);
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
            printf("Nieprawidłowy ruch!1\n");
            return 0;
        }
    }

    if (piece == 'P' || piece == 'p') {
        if (!isValidPawnMove(board, RowStart, ColStart, RowEnd, ColEnd, piece)) {
            printf("Nieprawidłowy ruch pionka!\n");
            return 0;
        }
    }

    switch (piece)
    {
        case 'R':
        if (!isValidRookMove(board, RowStart, ColStart, RowEnd, ColEnd)) {
            printf("Nieprawidłowy ruch wieży!\n");
            return 0;
        }
        break;

        case 'r':
        if (!isValidRookMove(board, RowStart, ColStart, RowEnd, ColEnd)) {
            printf("Nieprawidłowy ruch wieży!\n");
            return 0;
        }
        break;

        case 'H':
        if (!isValidKnightMove(board, RowStart, ColStart, RowEnd, ColEnd)) {
            printf("Nieprawidłowy ruch konia!\n");
            return 0;
        }
        break;
        
        case 'h':
        if (!isValidKnightMove(board, RowStart, ColStart, RowEnd, ColEnd)) {
            printf("Nieprawidłowy ruch konia!\n");
            return 0;
        }
        break;
        
        case 'B':
        if (!isValidBishopMove(board, RowStart, ColStart, RowEnd, ColEnd)) {
            printf("Nieprawidłowy ruch gońca!\n");
            return 0;
        }
        break;
        //////////////////
        case 'b':
        if (!isValidBishopMove(board, RowStart, ColStart, RowEnd, ColEnd)) {
            printf("Nieprawidłowy ruch gońca!\n");
            return 0;
        }
        break;
        
        case 'Q':
        if (!isValidQueenMove(board, RowStart, ColStart, RowEnd, ColEnd)) {
            printf("Nieprawidłowy ruch królowej!\n");
            return 0;
        }
        break;
        
        case 'q':
        if (!isValidQueenMove(board, RowStart, ColStart, RowEnd, ColEnd)) {
            printf("Nieprawidłowy ruch królowej!\n");
            return 0;
        }
        break;
        
        case 'K':
        if (!isValidKingMove(RowStart, ColStart, RowEnd, ColEnd)) {
            printf("Nieprawidłowy ruch króla!\n");
            return 0;
        }
        break;
        
        case 'k':
        if (!isValidKingMove(RowStart, ColStart, RowEnd, ColEnd)) {
            printf("Nieprawidłowy ruch króla!\n");
            return 0;
        }
        break;
        

        case 'p':
        break;

        case 'P':
        break;

        default:
        printf("Nieprawidłowa figura do ruchu!777\n");
        return 0;
        break;
    }







    board[RowEnd][ColEnd] = board[RowStart][ColStart];
        board[RowStart][ColStart] = ' ';
    
        switch(TuraBool)
        {
            case 1:
                TuraBool=0;
                Tura='B';
                break;
            case 0:
                TuraBool=1;
                Tura='W';
                break;
            default:
                break;
        }

    return 1;
}

bool isCheckWhite(char board[BOARD_SIZE][BOARD_SIZE], char king, int kingRow, int kingCol) {
    char opponentPieces[] = {'P', 'R', 'H', 'B', 'Q', 'K'};

    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            char piece = board[row][col];
            if (strchr(opponentPieces, piece)) {
                if (movePiece(board, row, col, kingRow, kingCol, piece)) {
                    undoMove();
                    return true;
                }
            }
        }
    }
    return false;
}

bool isCheckBlack(char board[BOARD_SIZE][BOARD_SIZE], char king, int kingRow, int kingCol) {
    char opponentPieces[] = {'p', 'r', 'h', 'b', 'q', 'k'};

    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            char piece = board[row][col];
            if (strchr(opponentPieces, piece)) {
                if (movePiece(board, row, col, kingRow, kingCol, piece)) {
                    undoMove();
                    return true;
                }
            }
        }
    }
    return false;
}

bool isCheckmate(char board[BOARD_SIZE][BOARD_SIZE], char king, int kingRow, int kingCol) {
    if (!isCheckWhite(board, king, kingRow, kingCol)|| !isCheckBlack(board,king,kingRow,kingCol)) return false;

    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            char piece = board[kingRow][kingCol];
            if (movePiece(board, kingRow, kingCol, row, col, piece)) {
                if (!isCheckWhite(board, king, row, col)||!isCheckBlack(board,king,row,col)) {
                    undoMove();
                    return false;
                }
                undoMove();
            }
        }
    }
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

int main()
{
    char piece;
    bool next;
    printf("Jeżeli gra nie wykryła checkmate to niech przeciwnik za ruch poda 9 9 9 9!(Jeżeli taki błąd wystąpi)\n");
    printf("Przy wpisaniu 8 8 8 8 cofnie się ostatni ruch\n");
    while(!BlackWin && !WhiteWin)
    {
    printBoard();
    printf("Tura: %c\n",Tura);
    next=0;
    getMove();
    if(BlackWin==1 || WhiteWin==1){
        break;
    }
    piece=board[RowStart][ColStart];
    if(!movePiece(board,RowStart,ColStart,RowEnd,ColEnd,piece)){
        while(!movePiece(board,RowStart,ColStart,RowEnd,ColEnd,piece))
        {
            getMove();
        }
    }
    
    if(!Tura=='W')
    {
        getKingPosition('k');
    }
    if(!Tura=='B')
    {
        getKingPosition('K');
    }

    while(!Tura=='W' && isCheckWhite(board,'k',kingRowState,kingColState))
    {
        if(isCheckmate(board,'k',kingRowState,kingColState))
        {
            BlackWin=1;
            break;
        }
        undoMove();
        Tura='W';
        if(!movePiece(board,RowStart,ColStart,RowEnd,ColEnd,piece)){
        while(!movePiece(board,RowStart,ColStart,RowEnd,ColEnd,piece))
        {
            getMove();
        }
        }
        getKingPosition('k');
    }

    while(!Tura=='B' && isCheckBlack(board,'K',kingRowState,kingColState))
    {
        if(isCheckmate(board,'K',kingRowState,kingColState))
        {
            WhiteWin=1;
            break;
        }
        undoMove();
        Tura='B';
        if(!movePiece(board,RowStart,ColStart,RowEnd,ColEnd,piece)){
        while(!movePiece(board,RowStart,ColStart,RowEnd,ColEnd,piece))
        {
            getMove();
        }
        }
        getKingPosition('K');
    }

    }
    if(WhiteWin){
        printf("Wygrały Białe");
    }
    if(BlackWin){
        printf("Wygrały Czarne");
    }
    return 0;
}
