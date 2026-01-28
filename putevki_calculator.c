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

#define gotoStartPosition   move (5, 13);

typedef struct {
    int row;
    int col;
    int charCount;
    char name[10];
    char buffer[15];
}EDITTEXT;

typedef struct {
    float pinsk;
    float route;
    float city;
}LINE_NORMA;

typedef struct {
    float pinsk;
    float route;
    float sity;
}FUEL_CONSUMPTION;

/*
 *  Функция строит интерфейс главного окна.
 * Параметры:
 *      editTextList - список виджетов.
 *      listLength - количество виджетов в списке.
 */
void initMainWindow (EDITTEXT * editTextList, int listLength);

/**/
int getClearLength (char * name);

int main (){
    int seazon = -1;
    char inChar, clearLength, equal;
    char * p_buffer;
    char * emptyStr = "               ";
    long digitFromStr;

    LINE_NORMA lNorma = {33.0, 30.4, 33.6};
    FUEL_CONSUMPTION fuelConsn = {0, 0, 0};
    /*
     * Здесь создаём текстовые поля и заносим в список editTextList
     */
    EDITTEXT direction = {5, 13, 0, "direction"};
    EDITTEXT distRoute = {8, 13, 0, "distRoute"};
    EDITTEXT distPinsk = {10, 13, 0, "distPinsk"};
    EDITTEXT distCity = {12, 13, 0, "distCity"};
    EDITTEXT editTextList[] = {direction, distRoute, distPinsk, distCity};
    int eTLLength = sizeof (editTextList) / sizeof (editTextList[0]);
    int eTLIndex = 0;
    initscr ();
    refresh ();
    EDITTEXT editText = editTextList[eTLIndex];

    initMainWindow (&editTextList[0], eTLLength);

    gotoStartPosition;

    while (1){
        inChar = getch ();
        switch (inChar){
            case ENTER:
                if (editText.charCount != 0){

                    /*--------------------Здесь копируем в буффер--------------------------------------*/
                    attrset (A_NORMAL);
                    strncpy (&editTextList[eTLIndex].buffer[0], &editText.buffer[0], editText.charCount);
                    equal = strcmp ("direction", editText.name);
                    if (equal != 0){
                        digitFromStr = strtol(editText.buffer, NULL, 10);
                        // mvprintw (15, 10, "digitFromStr: %ld", digitFromStr);
                        equal = strcmp ("distRoute", editText.name);
                        if (equal == 0){
                            fuelConsn.route = ((float)digitFromStr/100) * lNorma.route;
                            mvprintw (editText.row, editText.col + 6, "%.2f", fuelConsn.route);
                        }
                        equal = strcmp ("distPinsk", editText.name);
                        if (equal == 0){
                            fuelConsn.pinsk= ((float)digitFromStr/100) * lNorma.pinsk;
                            mvprintw (editText.row, editText.col + 6, "%.2f", fuelConsn.pinsk);
                        }
                    }
                    // mvprintw (15, 10, "editText: %s", editText.buffer);
                    // mvprintw (16, 10, "editTextList: %s", editTextList[eTLIndex].buffer);
                    attrset (A_REVERSE);
                    /*---------------------------------------------------------------------------------*/
                }

                eTLIndex ++;
                if (eTLIndex > 3) eTLIndex = 0;
                editText = editTextList[eTLIndex];

                /*--------------------Здесь подтираем буффер---------------------*/
                clearLength = getClearLength (&editText.name[0]);
                strncpy (&editTextList[eTLIndex].buffer[0], emptyStr, clearLength);
                strncpy (&editText.buffer[0], emptyStr, clearLength);
                /*---------------------------------------------------------------*/

                editText.charCount = 0;
                mvprintw (editText.row, editText.col, "%s", editText.buffer);
                move (editText.row, editText.col);
                break;
            default:
                attrset (A_REVERSE);
                if ((inChar > 64 && inChar < 91 ) || (inChar > 96 && inChar < 123)){
                    // attrset (A_REVERSE);
                    editText.buffer[editText.charCount] = inChar;
                    editText.charCount ++;
                }
                if (inChar > 47 && inChar < 58){
                    editText.buffer[editText.charCount] = inChar;
                    editText.charCount ++;
                }
        }
    }
}// main



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
    // attrset (A_NORMAL);
}// initMainW

int getClearLength (char * name){
    int equal, result;
    // attrset (A_NORMAL);
    // mvprintw (17, 10, "%s", name);
    // attrset (A_REVERSE);
    result = 5;
    equal = strcmp ("direction", name);
    if (equal == 0) result = 15;
    return result;
}// getClearLength
