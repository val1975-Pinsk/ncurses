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
    float pinsk;//    Линейная норма расхода топлива по Пинску.
    float route;//    Линейная норма расхода топлива по трассе.
    float city;//    Линейная норма расхода топлива по городу назначения.
}LINE_NORMA;

typedef struct {
    float pinsk;
    float route;
    float city;
}FUEL_CONSUMPTION;// Расход топлива.

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
    float totalConsumption = 0.0;

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
                    /*---------------------------------------------------------------------------------*/

                    equal = strcmp ("direction", editText.name);
                    if (equal != 0){
                        /*------------Здесь считаем расход топлива по трассе-----------------*/
                        digitFromStr = strtol(editText.buffer, NULL, 10);
                        // dist.route = (int)strtol(editText.buffer, NULL, 10);
                        equal = strcmp ("distRoute", editText.name);
                        if (equal == 0){
                            fuelConsn.route = ((float)digitFromStr/100) * lNorma.route;
                            totalConsumption = fuelConsn.route;
                            mvprintw (editText.row, editText.col + 6, "%.2f", fuelConsn.route);
                        }
                        /*-------------------------------------------------------------------*/

                        /*------------Здесь считаем расход топлива по Пинску-----------------*/
                        equal = strcmp ("distPinsk", editText.name);
                        digitFromStr = strtol(editText.buffer, NULL, 10);
                        if (equal == 0){
                            fuelConsn.pinsk= ((float)digitFromStr/100) * lNorma.pinsk;
                            totalConsumption += fuelConsn.pinsk;
                            mvprintw (editText.row, editText.col + 6, "%.2f", fuelConsn.pinsk);
                        }
                        /*-------------------------------------------------------------------*/

                        /*---------Здесь считаем расход топлива по пункту назначения---------*/
                        equal = strcmp ("distCity", editText.name);
                        digitFromStr = strtol(editText.buffer, NULL, 10);
                        if (equal == 0){
                            fuelConsn.city= ((float)digitFromStr/100) * lNorma.city;
                            totalConsumption += fuelConsn.city;
                            mvprintw (editText.row, editText.col + 6, "%.2f", fuelConsn.city);
                            digitFromStr = strtol(editTextList[1].buffer, NULL, 10);
                            digitFromStr += strtol(editTextList[2].buffer, NULL, 10);
                            digitFromStr += strtol(editTextList[3].buffer, NULL, 10);
                            mvprintw (15, 30, "          ");
                            mvprintw (16, 30, "          ");
                            mvprintw (15, 30, "%ld km.", digitFromStr);
                            mvprintw (16, 30, "%.2f l.", totalConsumption);
                            mvprintw (18, 6, "Continue ? (y/n):");
                            inChar = getch ();
                            if (inChar == 'n'){
                                printw ("\n");
                                endwin ();
                                return 0;
                            }
                        }
                        /*-------------------------------------------------------------------*/

                    }else{
                        // attrset (A_REVERSE);
                        mvprintw (11, 31, "%s", editText.buffer);
                    }
                    attrset (A_REVERSE);

                }
                /*Здесь выбираем текущий editText*/
                eTLIndex ++;
                if (eTLIndex > 3) eTLIndex = 0;
                editText = editTextList[eTLIndex];
                editText.charCount = 0;

                /*---------------------Здесь очищаем буффер----------------------*/
                clearLength = getClearLength (&editText.name[0]);
                strncpy (&editTextList[eTLIndex].buffer[0], emptyStr, clearLength);
                strncpy (&editText.buffer[0], emptyStr, clearLength);
                /*---------------------------------------------------------------*/
                /*---------------------Здесь очищаем строки----------------------*/
                mvprintw (editText.row, editText.col, "%s", editText.buffer);
                attrset (A_NORMAL);
                if (eTLIndex == 0){
                    mvprintw (11, 31, "%s", editText.buffer);
                }else{
                    mvprintw (editText.row, editText.col + 6, "%s", "       ");
                }
                attrset (A_REVERSE);
                /*---------------------------------------------------------------*/

                move (editText.row, editText.col);
                break;
            default:
                attrset (A_REVERSE);
                if ((inChar > 64 && inChar < 91 ) || (inChar > 96 && inChar < 123)){
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
    attrset (A_NORMAL);
    mvprintw (15, 6, "Total distance:");
    mvprintw (16, 6, "Total fuel consumption:");
    attrset (A_REVERSE);
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
