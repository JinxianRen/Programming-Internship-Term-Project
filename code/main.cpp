#include <bits/stdc++.h>
using namespace std;

const int DRAGON = 0, NINJIA = 1, ICEMAN = 2, LION = 3, WOLF = 4;
const int RED = 0, BLUE = 1;
const int line[2][5] = {{2, 3, 4, 1, 0}, {3, 0, 1, 2, 4}};
const char *part[2] = {"red", "blue"};
const char *arms[5] = {"dragon", "ninja", "iceman", "lion", "wolf"};
const char *weapons[3] = {"sword", "bomb", "arrow"};

int total[2], need[5], initial_attack[5], order[2] = {0}, number[2];
int now_time = 0, loyalty_minus, arrow_attack, city_num, temp_life[2];
bool game_over = 0;

void created();
void shot_arrow();
void escape(int id);
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
  bool is_winner;
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
    is_winner = 0;
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
  friend class citys;
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

class citys
{
public:
  int id;
  int life;
  int color;
  int winner;
  bool is_head;
  int last_winner;

  worriors *warr[2];
  worriors *temp_warr[2];
  citys()
  {
    last_winner = winner = color = -1;
    life = 0;
    warr[0] = warr[1] = NULL;
    temp_warr[0] = temp_warr[1] = NULL;
  }
  ~citys() {}
  void go(citys *next, int _color);
  void produce_life() { life += 10; }
  void get_life();
  void tell(int _color);
  void ttk();
  void win(int, int, int);
  void draw(int);
  void award();
} city[22];
void created() //司令部生成士兵，因为只在司令部生成所以全局函数
{
  for (int i = 0; i <= 1; i++)
  {
    if (total[i] >= need[line[i][order[i]]])
    {
      number[i]++;
      int _id = i == 0 ? 0 : (city_num + 1);
      if (line[i][order[i]] == DRAGON)
        city[_id].warr[i] = new dragon(i, number[i], line[i][order[i]]);
      else if (line[i][order[i]] == LION)
        city[_id].warr[i] = new lion(i, number[i], line[i][order[i]]);
      else
        city[_id].warr[i] = new worriors(i, number[i], line[i][order[i]]);
      order[i]++;
      order[i] %= 5;
    }
  }
}
void escape(int id) // lion跑路
{
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
void citys::go(citys *next, int _color) //移动
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
void go_ahead() //前进
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
void reach() //抵达
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
void citys::get_life() // 30分获取无人生命
{
  int turn = -1;
  if (warr[0] != NULL && warr[1] == NULL)
    turn = 0;
  if (warr[1] != NULL && warr[0] == NULL)
    turn = 1;
  if (turn != -1)
  {
    printf("%03d:30 %s %s %d earned %d elements for his headquarter\n", now_time, part[turn], arms[warr[turn]->type], warr[turn]->id, life);
    total[turn] += life;
    life = 0;
  }
}
void shot(int i, int j, citys targ)
{
  printf("%03d:35 %s %s %d shot\n", now_time, part[j], arms[city[i].warr[j]->type], city[i].warr[j]->id);
  targ.warr[1 - j]->hp -= city[i].warr[j]->_arrow->get_force();
  city[i].warr[j]->_arrow->used();
  if (city[i].warr[j]->_arrow->is_deserted())
  {
    delete city[i].warr[j]->_arrow;
    city[i].warr[j]->_arrow = NULL;
  }
  if (targ.warr[1 - j]->hp <= 0)
  {
    targ.warr[1 - j]->hp = 0;
    printf(" and killed %s %s %d\n", part[1 - j], arms[targ.warr[1 - j]->type], targ.warr[1 - j]->id);
  }
}
void shot_arrow() //射箭 未测试
{
  for (int i = 1; i <= city_num; i++)
  {
    for (int j = 0; j <= 1; j++)
    {
      if (city[i].warr[j] == NULL)
        break;
      if (city[i].warr[j]->_arrow == NULL)
        break;
      if (j == 0 && city[i + 1].warr[1 - j] != NULL)
      {
        shot(i, j, city[i + 1]);
      }
      else if (j == 1 && city[i - 1].warr[1 - j] != NULL)
      {
        shot(i, j, city[i - 1]);
      }
    }
  }
}
void citys::tell(int _color) //报告武器
{
  bool have_wep = false;
  if (warr[_color] == NULL)
    return;
  printf("%03d:55 %s %s %d has ", now_time, part[_color], arms[warr[_color]->type], warr[_color]->id);
  if (warr[_color]->_arrow != NULL)
  {
    printf("arrow(%d)", warr[_color]->_arrow->get_used_time());
    have_wep = true;
  }
  if (warr[_color]->has_bomb)
  {
    if (have_wep)
      printf(",");
    printf("bomb");
    have_wep = true;
  }
  if (warr[_color]->_sword != NULL)
  {
    if (have_wep)
      printf(",");
    printf("sword(%d)", warr[_color]->_sword->get_force());
    have_wep = true;
  }
  if (!have_wep)
    printf("no weapon");
  printf("\n");
}
void use_bomb() //爆炸 未测试
{
  for (int i = 1; i <= city_num; i++)
  {
    if (city[i].warr[0] == NULL || city[i].warr[1] == NULL || city[i].warr[0]->hp == 0 || city[i].warr[1]->hp == 0)
      break;
    int turn = 0;
    if (city[i].color == 1 || (city[i].color == -1 && city[i].id % 2 == 0))
      turn = 1;
    int demage = city[i].warr[turn]->attack;
    if (city[i].warr[turn]->_sword != NULL)
      demage += city[i].warr[turn]->_sword->get_force();
    if (city[i].warr[1 - turn]->hp <= demage && city[i].warr[1 - turn]->has_bomb)
    {
      printf("%03d:38 %s %s %d used a bomb and killed %s %s %d\n", now_time, part[1 - turn],
             arms[city[i].warr[1 - turn]->type], city[i].warr[1 - turn]->id, part[turn], arms[city[i].warr[turn]->type], city[i].warr[turn]->id);
      delete city[i].warr[0];
      delete city[i].warr[1];
      city[i].warr[0] = city[i].warr[1] = NULL;
      break;
    }
    if (city[i].warr[1 - turn]->type == NINJIA)
      break;
    demage = city[i].warr[1 - turn]->attack / 2;
    if (city[i].warr[1 - turn]->_sword != NULL)
      demage += city[i].warr[1 - turn]->_sword->get_force();
    if (city[i].warr[turn]->hp <= demage && city[i].warr[turn]->has_bomb)
    {
      printf("%03d:38 %s %s %d used a bomb and killed %s %s %d\n", now_time, part[turn],
             arms[city[i].warr[turn]->type], city[i].warr[turn]->id, part[1 - turn], arms[city[i].warr[1 - turn]->type], city[i].warr[1 - turn]->id);
      delete city[i].warr[0];
      delete city[i].warr[1];
      city[i].warr[0] = city[i].warr[1] = NULL;
      break;
    }
  }
}
void citys::win(int winner_color, int turn, int lion_hp)
{
  if (warr[winner_color]->type == WOLF)
  {
    if (warr[winner_color]->_sword == NULL && warr[1 - winner_color]->_sword != NULL)
      warr[winner_color]->_sword = warr[1 - winner_color]->_sword;
    if (!warr[winner_color]->has_bomb && warr[1 - winner_color]->has_bomb)
      warr[winner_color]->has_bomb = true;
    if (warr[winner_color]->_arrow == NULL && warr[1 - winner_color]->_arrow != NULL)
      warr[winner_color]->_arrow = warr[1 - winner_color]->_arrow;
  }
  warr[winner_color]->change_morale(1);
  if (winner_color == turn)
    warr[winner_color]->happy(id);
  warr[winner_color]->is_winner = 1;
  warr[winner_color]->hp += lion_hp;
  printf("%03d:40 %s %s %d earned %d elements for his headquarter\n", now_time, part[winner_color], arms[warr[winner_color]->type], warr[winner_color]->id, life);
  temp_life[winner_color] += life;
  life = 0;
  winner = winner_color;
}
void citys::draw(int turn)
{
  warr[0]->change_loyalty();
  warr[1]->change_loyalty();
  warr[0]->change_morale(-1);
  warr[1]->change_morale(-1);
  warr[turn]->happy(id);
  winner = -1;
}
void citys::ttk()
{
  if (warr[0] == NULL) //一个被射死，或者只有一个人
  {
    if (warr[1] != NULL && warr[1]->hp == 0)
    {
      delete warr[1];
      warr[1] = NULL;
    }
    return;
  }
  if (warr[1] == NULL)
  {
    if (warr[0]->hp == 0)
    {
      delete warr[0];
      warr[0] = NULL;
    }
    return;
  }
  if (warr[0]->hp == 0 && warr[1]->hp == 0) //都被射死
  {
    delete warr[0];
    delete warr[1];
    warr[0] = warr[1] = NULL;
    return;
  }
  last_winner = winner;
  int turn = 0;
  if (color == 1 || (color == -1 && id % 2 == 0))
    turn = 1;
  if (warr[0]->hp == 0)
  {
    win(1, turn, 0);
    return;
  }
  if (warr[1]->hp == 0)
  {
    win(0, turn, 0);
    return;
  }
  printf("%03d:40 %s %s %d ", now_time, part[turn], arms[warr[turn]->type], warr[turn]->id);
  printf("attacked %s %s %d in city %d with %d elements and force %d\n", part[1 - turn], arms[warr[1 - turn]->type], warr[1 - turn]->id, id, warr[turn]->hp, warr[turn]->attack);
  //主动攻击
  int f_atk = warr[turn]->attack;
  int lion_hp = 0;
  if (warr[1 - turn]->type == LION)
    lion_hp = warr[1 - turn]->hp;
  if (warr[turn]->_sword != NULL)
  { //有剑
    f_atk += warr[turn]->_sword->get_force();
    warr[turn]->_sword->used();
    if (warr[turn]->_sword->durability())
    {
      delete warr[turn]->_sword;
      warr[turn]->_sword = NULL;
    }
  }
  warr[1 - turn]->hp -= f_atk;
  if (warr[1 - turn]->hp <= 0)
  {
    printf("%03d:40 %s %s %d was killed in city %d\n", now_time, part[1 - turn], arms[warr[1 - turn]->type], warr[1 - turn]->id, id);
    win(turn, turn, lion_hp);
    return;
  }
  if (warr[1 - turn]->type != NINJIA) //反击
  {
    int s_atk = warr[1 - turn]->attack / 2;
    if (warr[turn]->type == LION)
      lion_hp = warr[turn]->hp;
    if (warr[1 - turn]->_sword != NULL)
    { //有剑
      s_atk += warr[1 - turn]->_sword->get_force();
      warr[1 - turn]->_sword->used();
      if (warr[1 - turn]->_sword->durability())
      {
        delete warr[1 - turn]->_sword;
        warr[1 - turn]->_sword = NULL;
      }
    }
    warr[turn]->hp -= s_atk;
    if (warr[turn]->hp <= 0)
    {
      printf("%03d:40 %s %s %d was killed in city %d\n", now_time, part[turn], arms[warr[turn]->type], warr[turn]->id, id);
      win(1 - turn, turn, lion_hp);
      return;
    }
  }
  draw(turn);
}
void citys::award()
{
  for (int i = 0; i <= 1; i++)
    if (warr[i]!=NULL&&warr[i]->is_winner && total[i] >= 8)
    {
      total[i] -= 8;
      warr[i]->hp += 8;
    }
  if (winner == last_winner)
    color = winner;
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
      if (game_over) //游戏结束
        break;
      if (now_min + 10 <= total_min) //产出生命;
      {
        now_min += 10;
        for (int i = 1; i <= city_num; i++)
          city[i].produce_life();
      }
      else
        break;
      if (now_min + 10 <= total_min) //取走生命;
      {
        now_min += 10;
        for (int i = 1; i <= city_num; i++)
          city[i].get_life();
      }
      else
        break;
      if (now_min + 5 <= total_min) //武士放箭;
      {
        now_min += 5;
        shot_arrow();
      }
      else
        break;
      if (now_min + 3 <= total_min) //使用炸弹;
      {
        now_min += 3;
        use_bomb();
      }
      else
        break;
      if (now_min + 2 <= total_min)
      {
        now_min += 2;
        for (int i = 1; i <= city_num; i++)
          city[i].ttk();
        for (int i = 1; i <= city_num; i++)
          city[i].award();
        for (int i = 0; i <= 2; i++)
          total[i] += temp_life[i];

      } //主动进攻，反击，战死，欢呼，获取生命，升旗;
      else
        break;
      if (now_min + 2 <= total_min) //司令部报告;
      {
        now_min += 10;
        for (int i = 0; i <= 1; i++)
          printf("%03d:50 %d elements in %s headquarter\n", now_time, total[i], part[i]);
      }
      else
        break;
      if (now_min + 5 <= total_min) //武士报告
      {
        now_min += 5;
        for (int j = 0; j <= 1; j++)
          for (int i = 0; i <= city_num + 1; i++)
            city[i].tell(j);
      }
      else
        break;
      now_min += 5;

      now_time++;
    }
  }
  system("pause");
}