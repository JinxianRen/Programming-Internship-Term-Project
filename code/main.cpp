#include <bits/stdc++.h>
using namespace std;

const int DRAGON = 0, NINJIA = 1, ICEMAN = 2, LION = 3, WOLF = 4;
const int RED = 0, BLUE = 1;
const int line[2][5] = {{2, 3, 4, 1, 0}, {3, 0, 1, 2, 4}};
const char *part[2] = {"red", "blue"};
const char *arms[5] = {"dragon", "ninja", "iceman", "lion", "wolf"};
const char *weapons[3] = {"sword", "bomb", "arrow"};

int total[2], need[5], initial_attack[5], order[2] = {0}, number[2];
int now_time = 0, loyalty_minus, arrow_attack, city_num;
bool game_over = 0;

class sword
{
private:
  int force;

public:
  sword(int x) : force(x) {}
  void used() { force = force * 0.8; }
  bool durability() { return (force == 0); }
  int get_force() { return force; }
};
class arrow
{
private:
  int used_times;
  int force;

public:
  arrow(int x, int y) : used_times(x), force(y) {}
  void used() { used_times--; }
  bool is_deserted() { return (used_times == 0); }
  int get_force() { return force; }
  int get_used_time() { return used_times; }
};
class worriors
{
public:
  int type;
  int id;
  int hp;
  int color;
  int attack;
  sword *_sword;
  arrow *_arrow;
  bool has_bomb;
  worriors(int _color, int _id, int _type)
  {
    printf("%03d:00 %s %s %d born\n", now_time, part[_color], arms[_type], _id);
    total[_color] -= need[_type];
    id = _id;
    color = _color;
    type = _type;
    hp = need[_type];
    attack = initial_attack[_type];
    _sword = NULL;
    has_bomb = false;
    _arrow = NULL;
    if (_type == DRAGON || _type == ICEMAN)
    {
      switch (id % 3)
      {
      case 0:
        if ((int)(attack * 0.2) != 0)
          _sword = new sword(attack * 0.2);
        break;
      case 1:
        has_bomb = true;
        break;
      case 2:
        _arrow = new arrow(3, arrow_attack);
        break;
      }
    }
    else if (_type == NINJIA)
    {
      switch (id % 3)
      {
      case 0:
        if ((int)(attack * 0.2) != 0)
          _sword = new sword(attack * 0.2);
        has_bomb = true;
        break;
      case 1:
        has_bomb = true;
        _arrow = new arrow(3, arrow_attack);
        break;
      case 2:
        _arrow = new arrow(3, arrow_attack);
        if ((int)(attack * 0.2) != 0)
          _sword = new sword(attack * 0.2);
        break;
      }
    }
  }
  ~worriors()
  {
    if (_sword != NULL)
      delete _sword;
    if (_arrow != NULL)
      delete _arrow;
  }
  void get_hp(int a) { hp += a; }
  virtual void happy(int) {}
  virtual void change_morale(int) {}
  virtual bool nigero() { return false; }
  virtual void change_loyalty() {}
  friend class city;
}; //只有dragon和lion有特殊属性
class dragon : public worriors
{
private:
  double morale;

public:
  dragon(int _color, int _id, int _type) : worriors(_color, _id, _type)
  {
    morale = (double)total[_color] / (double)need[0];
    printf("Its morale is %.2lf\n", morale);
  }
  void happy(int _id)
  {
    if (morale > 0.8)
      printf("%03d:40 %s dragon %d yelled in city %d\n", now_time, part[color], id, _id);
  }
  void change_morale(int a) { morale += ((a == 1) ? 0.2 : -0.2); }
};
class lion : public worriors
{
private:
  int loyalty;

public:
  lion(int _color, int _id, int _type) : worriors(_color, _id, _type)
  {
    loyalty = total[_color];
    printf("Its loyalty is %d\n", loyalty);
  }
  bool nigero()
  {
    if (loyalty > 0)
      return false;
    else
      return true;
  }
  void change_loyalty() { loyalty -= loyalty_minus; }
};

class city
{
private:
  int id;
  int color;
  int life;
  int winner;
  int last_winner;

public:
  bool is_head;
  worriors *warr[2];
  worriors *temp_warr[2];
  city()
  {
    last_winner = winner = color = -1;
    life = 0;
    warr[0] = warr[1] = NULL;
    temp_warr[0] = temp_warr[1] = NULL;
  }
  ~city() {}
  void go(city *next, int _color);
  void produce_life(){life+=10;}
  void get_life();
} city[22];
void created() //司令部生成士兵，因为只在司令部生成所以全局函数
{
  for (int i = 0; i <= 1; i++)
  {
    if (total[i] >= need[line[i][order[i]]])
    {

      int _id = i == 0 ? 0 : (city_num + 1);
      if (line[i][order[i]] == DRAGON)
        city[_id].warr[i] = new dragon(i, number[i], line[i][order[i]]);
      else if (line[i][order[i]] == LION)
        city[_id].warr[i] = new lion(i, number[i], line[i][order[i]]);
      else
        city[_id].warr[i] = new worriors(i, number[i], line[i][order[i]]);
      order[i]++;
      order[i] %= 5;
      number[i]++;
    }
  }
}
void escape(int id)
{ // lion跑路
  if (id == 0)
  {
    if (city[id].warr[0] != NULL && city[id].warr[0]->nigero())
    {
      printf("%03d:05 %s lion %d ran \n", now_time, part[0], city[id].warr[0]->id);
      city[id].warr[0] = NULL;
    }
  }
  else if (id == city_num + 1)
  {
    if (city[id].warr[1] != NULL && city[id].warr[1]->nigero())
    {
      printf("%03d:05 %s lion %d ran away\n", now_time, part[1], city[id].warr[1]->id);
      city[id].warr[1] = NULL;
    }
  }
  for (int i = 0; i <= 1; i++)
  {
    if (city[id].warr[i] != NULL && city[id].warr[i]->nigero())
    {
      printf("%03d:05 %s lion %d ran away\n", now_time, part[i], city[id].warr[i]->id);
      city[id].warr[i] = NULL;
    }
  }
}
void city::go(city *next, int _color)
{
  temp_warr[_color] = next->warr[_color];
  if (temp_warr[_color] != NULL && temp_warr[_color]->type == ICEMAN)
  {
    if ((_color == 0 && id % 2 == 0) || (_color == 1 && (city_num - id) % 2 == 1))
    {
      temp_warr[_color]->hp -= 9;
      if (temp_warr[_color]->hp <= 0)
        temp_warr[_color]->hp = 1;
      temp_warr[_color]->attack += 20;
    }
  }
}
void go_ahead()
{
  if (city[city_num].warr[0] != NULL)
    city[city_num + 1].go(&city[city_num], 0);
  for (int i = city_num; i >= 1; i--)
    city[i].go(&city[i - 1], 0);
  if (city[1].warr[1] != NULL)
    city[0].go(&city[1], 1);
  for (int i = 1; i <= city_num; i++)
    city[i].go(&city[i + 1], 1);
}
void reach()
{
  for (int i = 1; i <= city_num; i++)
  {
    for (int j = 0; j <= 1; j++)
    {
      city[i].warr[j] = city[i].temp_warr[j];
      if (city[i].warr[j] != NULL)
        printf("%03d:10 %s %s %d marched to city %d with %d elements and force %d\n", now_time, part[j], arms[city[i].warr[j]->type], city[i].warr[j]->id, i, city[i].warr[j]->hp, city[i].warr[j]->attack);
    }
  }
  if (city[city_num + 1].temp_warr[0] != NULL)
  {
    printf("%03d:10 redd %s %d reached blue headquarter with %d elements and force %d\n", now_time, arms[city[city_num + 1].temp_warr[0]->type], city[city_num + 1].temp_warr[0]->id, city[city_num + 1].temp_warr[0]->hp, city[city_num + 1].temp_warr[0]->attack);
  }
  if (city[0].temp_warr[1] != NULL)
  {
    printf("%03d:10 blue %s %d reached red headquarter with %d elements and force %d\n", now_time, arms[city[0].temp_warr[1]->type], city[0].temp_warr[1]->id, city[0].temp_warr[1]->hp, city[0].temp_warr[1]->attack);
  }
  if (city[city_num + 1].temp_warr[0] != NULL && city[city_num + 1].warr[0] != NULL)
  {
    printf("%03d:10 blue headquarter was taken\n", now_time);
    game_over = 1;
  }
  if (city[0].temp_warr[1] != NULL && city[0].warr[1] != NULL)
  {
    printf("%03d:10 red headquarter was taken\n", now_time);
    game_over = 1;
  }
  city[city_num + 1].warr[0] = city[city_num + 1].temp_warr[0];
  city[0].warr[1] = city[0].temp_warr[1];
  city[0].warr[0] = city[city_num + 1].warr[1] = NULL;
}
void city::get_life()
{
	int turn=-1;
	if (warr[0]!=NULL && warr[1]==NULL) turn=0;
	if (warr[1]!=NULL && warr[0]==NULL) turn=1;
	if (turn!=-1)
	{
		printf("%03d:30 %s %s %d earned %d elements for his headquarter\n",now_time,part[turn],arms[warr[turn]->type],warr[turn]->id,life);
		total[turn]+=life;
		life=0;
	}
}
int main()
{
  int times;
  cin >> times;
  for (int t = 0; t < times; t++)
  {
    cout << "Case " << t << endl;
    int now_min = 0;
    int initial_life, total_min;
    cin >> initial_life >> city_num >> arrow_attack >> loyalty_minus >> total_min;
    for (int i = 0; i <= 1; i++)
      total[i] = initial_life;
    for (int i = 0; i < 5; i++)
      cin >> need[i];
    for (int i = 0; i < 5; i++)
      cin >> initial_attack[i];
    city[0].is_head = city[city_num + 1].is_head = 1;
    while (1)
    {
      created();                    //暴兵
      if (now_min + 5 <= total_min) // lion跑路;
      {
        now_min += 5;
        for (int i = 0; i <= city_num + 1; i++)
          escape(i);
      }
      else
        break;
      if (now_min + 5 <= total_min) //武士前进,抵达司令部，司令部占领;
      {
        now_min += 5;
        go_ahead();
        reach();
      }
      else
        break;
        if(game_over)break;
      if (now_min + 10 <= total_min)//产出生命;
      {
        now_min += 10;
        for (int i=1;i<=city_num;i++)
				city[i].produce_life();
      } 
      if (now_min + 10 <= total_min)//取走生命;
      {
        now_min += 10;
        for (int i=1;i<=city_num;i++)
				city[i].get_life();
      } 
      if (now_min + 5 <= total_min)
      {
        now_min += 5;
        cout << now_min << endl
             << "arror" << endl;
      } //武士放箭;
      else
        break;
      if (now_min + 3 <= total_min)
      {
        now_min += 3;
        cout << now_min << endl
             << "bomb" << endl;
      } //使用炸弹;
      else
        break;
      if (now_min + 2 <= total_min)
      {
        now_min += 2;
        cout << now_min << endl
             << "henduo" << endl;
      } //主动进攻，反击，战死，欢呼，获取生命，升旗;
      else
        break;
      if (now_min + 2 <= total_min)
      {
        now_min += 10;
        for (int i = 0; i <= 1; i++)
          printf("%03d:50 %d elements in %s headquarter\n", now_time, total[i], part[i]);
      } //司令部报告;
      else
        break;
      if (now_min + 5 <= total_min)
      {
        now_min += 5;
        cout << now_min << endl
             << "warrior" << endl;
      } //武士报告
      else
        break;
      now_min += 5;

      now_time++;
    }
  }
  system("pause");
}