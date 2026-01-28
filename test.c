/*
 * 4.01.2025
 * Программа для расчета путевых листов.
 */
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define TAB     9
#define ENTER   10
#define RIGHT   5
#define LEFT    4

// #define S_WINTER            0
// #define S_SUMMER            1
// #define DIRECT_VALUE        2
// #define DIST_ROUT_VALUE     3
// #define DIST_PINSK_VALUE    4
// #define DIST_CITY_2_VALUE   5

#define gotoStartPosition   move (5, 13);

typedef struct {
    int row;
    int col;
    int buffLength;
    char name[10];
    char buffer[15];
}EDITTEXT;

/*
 *  Функция строит интерфейс главного окна.
 * Параметры:
 *      editTextList - список виджетов.
 *      listLength - количество виджетов в списке.
 */
void initMainWindow (EDITTEXT * editTextList, int listLength){
    char header[] = "Calculator";
    int len = strlen (header);
    box (stdscr, 0, 0);
    move (0, 1);
    int pos = 0;
    int equal;
    while (len)
    {
        waddch (stdscr, header[pos]);
        refresh ();
        pos ++;
        len --;
    }
    mvprintw (2, 2, "Seazon");
    mvprintw (3, 4, "[ ]Winter\t[ ]Summer");
    mvprintw (5, 6, "Direct");
    mvprintw (7, 6, "Distance route");
    mvprintw (9, 6, "Distance around the city Pinsk");
    mvprintw (11, 6, "Distanse around the city");
    attrset (A_REVERSE);
    for (int index = 0; index < listLength; index ++){
        equal = strcmp ((*(editTextList + index)).name, "direction");
        if (equal == 0){
            mvprintw ((*(editTextList + index)).row, (*(editTextList + index)).col, "               ");
        }else{
            mvprintw ((*(editTextList + index)).row, (*(editTextList + index)).col, "     ");
        }
    }
    attrset (A_NORMAL);
}// initMainW

void toDo (EDITTEXT * editText){
    int equal = strcmp ("direction", editText->name);
    char * spaceStrLong = "               ";
    char * spaceStrShort = "     ";
    move (editText->row, editText->col);
    attrset (A_REVERSE);
    if (equal == 0){
        printw ("%s", spaceStrLong);
        attrset (A_NORMAL);
        mvprintw (11, 31, "%s", spaceStrLong);
    }else{
        printw ("%s", spaceStrShort);
    }
}// toDo

int main (){
    int seazon = -1;
    /*
     * Здесь создаём текстовые поля и заносим в список editTextList
     */
    EDITTEXT direction = {5, 13, 0, "direction"};
    EDITTEXT distRoute = {8, 13, 0, "distRoute"};
    EDITTEXT distPinsk = {10, 13, 0, "distPinsk"};
    EDITTEXT distCity = {12, 13, 0, "distCity"};
    EDITTEXT editTextList[] = {direction, distRoute, distPinsk, distCity};
    int eTLLength = sizeof (editTextList) / sizeof (editTextList[0]);
    initscr ();
    refresh ();
    EDITTEXT editText = editTextList[0];
    EDITTEXT * p_editText;
    int eTLIndex = 0;
    char inChar;
    int equal;
    int offset = 0;
    char * stroka = malloc (sizeof (char) * 15);
    char * spaceStr = "               ";
    initMainWindow (&editTextList[0], eTLLength);

    gotoStartPosition;

    keypad (stdscr, TRUE);
    while (1){
        inChar = getch ();
        switch (inChar){
            case ENTER:
                equal = strcmp ("direction", editText.name);
                if (equal == 0){
                    attrset (A_NORMAL);
                    mvprintw (11, 31, "%s", editText.buffer);
                }
                // break;
            case RIGHT:
                eTLIndex += 1;
                if (eTLIndex > 3) eTLIndex = 0;
                editText = editTextList[eTLIndex];
                offset = 0;
                toDo (&editText);
                move (editText.row, editText.col);
                break;
            case LEFT:
                eTLIndex -= 1;
                if (eTLIndex < 0) eTLIndex = 4;
                editText = editTextList[eTLIndex];
                offset = 0;
                toDo (&editText);
                move (editText.row, editText.col);
                break;
            default:
                equal = strcmp ("direction", editText.name);
                if (equal == 0){
                    if ((inChar > 64 && inChar < 91 ) || (inChar > 96 && inChar < 123)){
                        attrset (A_REVERSE);
                        move (editText.row, editText.col + offset);
                        waddch (stdscr, inChar);
                        editText.buffer[offset] = inChar;
                        offset ++;
                        editText.buffLength = offset;
                        // mvprintw (15, 3, "%s", editText.buffer);
                    }
                }else{
                    // if (inChar > 47 && inChar < 58){
                    //     attrset (A_REVERSE);
                    // }
                }
        }
    }
}
