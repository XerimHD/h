#include <iostream>
#include <vector>
#include <time.h>
#include <E:\Desktop\school\ctest\lib\PDCurses-3.9\curses.h>
using namespace std;
void game_loop(bool &staircase_placed, int &c, bool &t_placed, int &r_placed, bool &p_placed, int rows, int cols, vector<vector<char>> &map);
void dungeon_graw(bool &p_placed, int rows, int cols, vector<vector<char>> &map);
void dungeon_gen(bool &staircase_placed, bool &p_placed, bool &t_placed, int &r_placed, int rows, int cols, vector<vector<char>> &map);
void movement(bool &staircase_placed, bool &t_placed, int &r_placed, bool &p_placed, int &c, vector<vector<char>> &map);
void attack(int rows, int &c, int dir_y, int dir_x, vector<vector<char>> &map);
int px, py; // координата игрока
int p_gold = 0;

class monsters
{
public:
    int y;
    int x;
    int hp;
};
class monsters monster[10];
int main()
{

    int c = 0;
    int cols, rows;
    bool t_placed = false;
    bool p_placed = false;
    int r_placed = 0;
    bool staircase_placed = false;

    initscr();
    keypad(stdscr, 1);
    noecho();
    curs_set(0);
    getmaxyx(stdscr, rows, cols);
    vector<vector<char>> map(rows, vector<char>(cols));

    do
    {

        //  erase(); // очистка екрана
        game_loop(staircase_placed, c, t_placed, r_placed, p_placed, rows, cols, map);
        refresh();                 // Обновление экрана
    } while ((c = getch()) != 27); // 27 - ESC

    getch();
    endwin();
    return 0;
}

/////////////////////

void game_loop(bool &staircase_placed, int &c, bool &t_placed, int &r_placed, bool &p_placed, int rows, int cols, vector<vector<char>> &map)
{

    movement(staircase_placed, t_placed, r_placed, p_placed, c, map);
    dungeon_gen(staircase_placed, p_placed, t_placed, r_placed, rows, cols, map);
    dungeon_graw(p_placed, rows, cols, map);

    if (c == 'a')
    {
        attack(c, py, px, rows, map);
    }
}

void dungeon_gen(bool &staircase_placed, bool &p_placed, bool &t_placed, int &r_placed, int rows, int cols, vector<vector<char>> &map)
{
    srand(time(0));
    int rx = 0, ry = 0;     // кордината комнаты
    int r_size_x, r_size_y; // размер комнаты
    bool collision = false;
    if (!r_placed)
    {

        int room_num = rand() % 2 + 3; // число комнат
        // коридоры
        int r_old_center_y, r_old_center_x, r_center_y, r_center_x;

        /// база даных
        for (int y = 0; y < rows; y++)
        {
            for (int x = 0; x < cols; x++)
            {

                if (y == 0 || y == rows - 2 || x == 0 || x == cols - 1)
                {
                    map[y][x] = '%'; // записываем  барьер в базу
                }
                else
                {
                    map[y][x] = '#'; // записываем  стену в базу
                }
            }
        }
        int try_counter = 0; //  изменяряет попытки генерации комнат

        while (r_placed < room_num)
        {

            do
            {
                collision = false;
                // делаем кординаты комнате
                ry = rand() % (rows - 4) + 1;
                rx = rand() % (cols - 4) + 1;
                r_size_y = rand() % 5 + 4;
                r_size_x = rand() % 10 + 8;
                for (int y = ry; y < ry + r_size_y; y++)
                {

                    for (int x = rx; x < rx + r_size_x; x++)
                    {
                        if (map[y][x] == '%' || map[y][x] == ' ' || map[y + 2][x] == ' ' || map[y - 2][x] == ' ' || map[y][x + 2] == ' ' || map[y][x - 2] == ' ')
                        {
                            y = ry + r_size_y;
                            collision = true;
                            break;
                        }
                    }
                }
                try_counter++;
                if (try_counter > 100)
                {
                    r_placed = room_num;
                }

            } while (collision == true);

            // записывыем в базу комнаты
            for (int y = ry; y < ry + r_size_y; y++)
            {
                for (int x = rx; x < rx + r_size_x; x++)
                {
                    if (map[y][x] == '%')
                    {
                        y = ry + r_size_y;
                        break;
                    }
                    else
                    {
                        map[y][x] = ' ';
                    }
                }
            }
            r_placed++;
            if (r_placed > 1)
            {
                r_old_center_y = r_center_y;
                r_old_center_x = r_center_x;
            }
            r_center_y = ry + (r_size_y / 2);
            r_center_x = rx + (r_size_x / 2);
            if (r_placed > 1)
            {
                int path_y;
                for (path_y = r_old_center_y; path_y != r_center_y;)
                {
                    map[path_y][r_old_center_x] = ' ';
                    if (r_old_center_y < r_center_y)
                    {
                        path_y++;
                    }
                    else if (r_old_center_y > r_center_y)
                    {
                        path_y--;
                    }
                }
                for (int x = r_old_center_x; x != r_center_x;)
                {
                    map[path_y][x] = ' ';
                    if (r_old_center_x < r_center_x)
                    {
                        x++;
                    }
                    else if (r_old_center_x > r_center_x)
                    {
                        x--;
                    }
                }
            }
        }
    }
    if (!t_placed)
    {
        int monster_y, monster_x;
        for (int m = 0; m < 10; m++)
        {

            do
            {
                monster_x = rand() % cols;
                monster_y = rand() % rows;
            } while (map[monster_y][monster_x] != ' ');
            monster[m].y = monster_y;
            monster[m].x = monster_x;
            monster[m].hp = 2; // хп монстров
            map[monster[m].y][monster[m].x] = 't';
        }
        t_placed = true;
    }
    if (!p_placed)
    {

        do
        {
            px = rand() % cols;
            py = rand() % rows;
        } while (map[py][px] != ' ');
        map[py][px] = '@';
        p_placed = true;
    }
    if (!staircase_placed)
    {
        int center_y, center_x;

        do
        {

            center_y = ry + (r_size_y / 2);
            center_x = rx + (r_size_x / 2);

        } while (map[center_y][center_x] != ' ');
        map[center_y][center_x] = '>';
        staircase_placed = true;
    }
}
void dungeon_graw(bool &p_placed, int rows, int cols, vector<vector<char>> &map)
{
    srand(time(0));
    ///// рисования данжа
    for (int y = 0; y < rows; y++)
    {
        for (int x = 0; x < cols; x++)
        {

            if (map[y][x] == '%')
            {
                mvaddch(y, x, '%'); // рисуем  барьер
            }
            else if (map[y][x] == '@')
            {
                mvaddch(y, x, '@');
            }
            else if (map[y][x] == 't')
            {
                mvaddch(y, x, 't');
            }
            else if (map[y][x] == ' ') // рисуем комнату
            {
                mvaddch(y, x, ' ');
            }
            else if (map[y][x] == '>')
            {
                mvaddch(y, x, '>');
            }

            else
            {
                mvaddch(y, x, '#'); // рисуем  стену
            }
        }
    }

    //////////////////////////////////////////////////////////
    // чистим последню строку для информации
    for (int x = 0; x < cols; x++)
    {
        mvaddch(rows - 1, x, ' ');
    }

    mvprintw(rows - 1, 0, "Gold %d", p_gold); // вывод золота
}
void movement(bool &staircase_placed, bool &t_placed, int &r_placed, bool &p_placed, int &c, vector<vector<char>> &map)

{
    int old_player_x = px, old_player_y = py;
    int dir_y = py, dir_x = px;

    if (c == KEY_UP)
        dir_y--;
    else if (c == KEY_DOWN)
        dir_y++;
    else if (c == KEY_LEFT)
        dir_x--;
    else if (c == KEY_RIGHT)
        dir_x++;

    if (map[dir_y][dir_x] == ' ')
    {

        map[old_player_y][old_player_x] = ' ';
        py = dir_y;
        px = dir_x;
        map[py][px] = '@';
    }
    else if (map[dir_y][dir_x] == '>')
    {

        t_placed = false;
        p_placed = false;
        staircase_placed = false;
        r_placed = 0;
    }
}
void attack(int rows, int &c, int dir_y, int dir_x, vector<vector<char>> &map)
{
    int attack_y = py, attack_x = px;

    for (int m = 0; m < 10; m++)
    {
        int dist_y = abs(attack_y - monster[m].y);
        int dist_x = abs(attack_x - monster[m].x);

        if (dist_y <= 1 && dist_x <= 1 && map[monster[m].y][monster[m].x] == 't')
        {
            if (monster[m].hp > 0)
            {
                monster[m].hp -= 1;
            }

            if (monster[m].hp <= 0)
            {
                map[monster[m].y][monster[m].x] = ' ';
                monster[m].y = -1; // Помечаем, что гоблин мертв
                monster[m].x = -1;
                p_gold += rand() % 9 + 1; // получения золота
            }
        }
    }
}

void respawn_monster()
{
}
