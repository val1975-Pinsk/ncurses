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

#define S_WINTER            0
#define S_SUMMER            1
#define DIRECT_VALUE        2
#define DIST_ROUT_VALUE     3
#define DIST_PINSK_VALUE    4
#define DIST_CITY_2_VALUE   5

#define gotoStartPosition   move (wi_list[0].row, wi_list[0].col +1);

typedef struct {
    int row;
    int col;
    char * name;
    char * label;
}windowItem;

typedef struct {
    float city300; //  Норма для населённых пунктов с населением 100 - 300 тыс. человек.
    float city1000;//  Норма для населённых пунктов с населением 300 - 1000 тыс. человек.
    float city3000;//  Норма для населённых пунктов с населением 1000 - 3000 тыс. человек.
    float route;   //  Норма по трассе.
}YUTONG;

typedef struct {
    char route[3];
    char Pinsk[3];
    char DirCity[3];
}DISTANCE;

typedef struct {
    char * string;
    int strLen;
}BUFFER;

/*
 *  Функция строит интерфейс главного окна.
 * Параметры:
 *      wi_list - список виджетов.
 *      wi_listLen - количество виджетов в списке.
 */
void initMainWindow (windowItem * wi_list, int wi_listLen){
    char header[] = "Calculator";
    int len = strlen (header);
    box (stdscr, 0, 0);
    move (0, 1);
    int pos = 0;
    while (len)
    {
        waddch (stdscr, header[pos]);
        refresh ();
        pos ++;
        len --;
    }
    mvprintw (2, 2, "Seazon");
    int equal;
    int row = 4;
    /*
     *  Здесь располагаем виджеты в глвном окне интерфйеса.
     */
    for (int index = 0; index < wi_listLen; index ++){
        equal = strcmp ((*(wi_list + index)).name, "radioButton");
        if (equal == 0){
            // Здесь строится интерфейс выбора сезона.
            mvprintw ((*(wi_list + index)).row, (*(wi_list + index)).col, "[ ]%s", (*(wi_list + index)).label);
        }else{
            equal = strcmp ((*(wi_list + index)).name, "editText");
            if (equal == 0){
                mvprintw ((*(wi_list + index)).row, (*(wi_list + index)).col, "%s", (*(wi_list + index)).label);
                equal = strcmp ((*(wi_list + index)).label, "Direct");
                attrset (A_REVERSE);
                if (equal == 0){
                    mvprintw ((*(wi_list + index)).row, (*(wi_list + index)).col + 7, "               ");
                }else{
                    mvprintw ((*(wi_list + index)).row + 1, (*(wi_list + index)).col + 7, "       ");
                }
                attrset (A_NORMAL);
            }
        }
    }
}// initMainW

void cursorGoTo (windowItem wi, int *row, int *col){
    int equal = strcmp (wi.name, "radioButton");
    if (equal == 0){
        // move (wi.row, wi.col + 1);
        *row = wi.row;
        *col = wi.col + 1;
    }else{
        equal = strcmp (wi.label, "Direct");
        if (equal == 0){
            // move (wi.row, wi.col + 7);
            *row = wi.row;
            *col = wi.col + 7;
        }else{
            // move (wi.row + 1, wi.col + 7);
            *row = wi.row + 1;
            *col = wi.col + 7;
        }
    }
    move (*row, *col);
}// cursorGoTo

void toDo (int wi_listIndex, DISTANCE * dist, BUFFER * buff){
    switch (wi_listIndex){
        case S_WINTER:
            mvprintw (15, 3, "S_WINTER");
            break;
        case S_SUMMER:
            mvprintw (15, 3, "S_SUMMER");
            break;
        case DIRECT_VALUE:
            mvprintw (15, 3, "%d", buff->strLen);
            char * dirValue;
            // strncpy (dirValue, buff->string, buff->strLen);
            attrset (A_NORMAL);
            // mvprintw (12, 31, "%s", dirValue);
            break;
        case DIST_ROUT_VALUE:
            // mvprintw (15, 3, "DIST_ROUT_VALUE");
            strncpy (dist->route, buff->string, 3);
            break;
        case DIST_PINSK_VALUE:
            strncpy (dist->Pinsk, buff->string, 3);
            // mvprintw (15, 3, "DIST_PINSK_VALUE");
            break;
        case DIST_CITY_2_VALUE:
            strncpy (dist->DirCity, buff->string, 3);
            // mvprintw (15, 3, "DIST_CITY_2_VALUE");
            break;
    }
}// toDo

void calculate (DISTANCE * dist){
    long result = strtol (dist->route, NULL, 10);
    mvprintw (15, 3, "%s", dist->route);
    mvprintw (16, 3, "%ld", result);

}// calculate

int main (){
    int seazon = -1;
    // int charBuffCount = 0;//Количество символов, введеных с клавиатуры.
    char * buffer = malloc (sizeof (char) * 15);//Буффер для хранения символов введеных с клавиатуры.

    initscr ();
    refresh ();
    /*
     * Создаём составляющие виджеты интерфйеса.
     */
    windowItem wi_winter = {3, 4, "radioButton", "Winter"};
    windowItem wi_summer = {3, 17, "radioButton", "Summer"};
    windowItem wi_direct = {6, 6, "editText", "Direct"};
    windowItem wi_rout = {8, 6, "editText", "Distance route"};
    windowItem wi_distanceCity_Pinsk = {10, 6, "editText", "Distance around the city Pinsk"};
    windowItem wi_distanceCity = {12, 6, "editText", "Distance around the city "};
    windowItem wi_list[] = {wi_winter, wi_summer, wi_direct, wi_rout, wi_distanceCity_Pinsk, wi_distanceCity};

    BUFFER strBuffer = {buffer, 0};
    DISTANCE dist;
    YUTONG lNorma;
    int wiListLen = sizeof (wi_list) / sizeof (wi_list[0]);
    initMainWindow (&wi_list[0], wiListLen);

    gotoStartPosition;
    int inChar = 0;
    int wi_listIndex = 0;
    int row, col;
    keypad (stdscr, TRUE);
    while (1){
        inChar = getch ();
        switch (inChar){
            case ENTER:
                toDo (wi_listIndex, &dist, &strBuffer);
                // charBuffCount = 0;
                // mvprintw (15, 3, "%d", wi_listIndex);
                break;
            case KEY_RIGHT:
                wi_listIndex += 1;
                if (wi_listIndex > 5){
                    // calculate (&dist);
                    wi_listIndex = 0;
                }
                cursorGoTo (wi_list[wi_listIndex], &row, &col);
                //mvprintw (15, 3, "%d", row);
                break;
            case KEY_LEFT:
                wi_listIndex -= 1;
                if (wi_listIndex < 0) wi_listIndex = 5;
                cursorGoTo (wi_list[wi_listIndex], &row, &col);
                break;
            default:
                /*
                 * Здесь сбрасываем в буффер символы, которые вводят с клавиатуры.
                 * Пока только буквы, без цифр.
                 */
                if ((inChar > 64 && inChar < 91 ) || (inChar > 96 && inChar < 123)){
                    attrset (A_REVERSE);
                    mvprintw (row, col, "%c", inChar);
                    strBuffer.string[strBuffer.strLen] = inChar;
                    col ++;
                    strBuffer.strLen ++;
                    // charBuffCount ++;
                    move (row, col);
                    mvprintw (15, 3, "%s", strBuffer.string);
                }
                /**/
                // if (inChar > 47 && inChar < 58){
                //     attrset (A_REVERSE);
                //     mvprintw (row, col, "%c", inChar);
                //     *(buffer + charBuffCount) = inChar;
                //     col ++;
                //     charBuffCount ++;
                //     move (row, col);
                // }
            }
        }
}
