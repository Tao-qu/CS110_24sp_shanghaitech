#include "lcd/lcd.h"
#include "utils.h"
#include "assembly/example.h"

#define TRACE_SIZE 16
#define GRAVITY 1
#define JUMP_SPEED 5
#define MAX_SPEED 10

enum Difficulty
{
  EASY,
  NORMAL,
  HARD
};

struct Point
{
  uint32_t x;
  uint32_t y;
};

struct Player
{
  struct Point position;
  struct Point trace[TRACE_SIZE];
  uint32_t trace_index;
  int speed;
  uint32_t life;
  uint32_t score;
  uint32_t guard;
};

struct Wall
{
  struct Point top;
  struct Point bottom;
};

struct Game
{
  struct Player player;
  struct Wall walls[2];
  uint32_t wall_speed;
  uint32_t wall_half_width;
  uint32_t wall_half_gap;
};

enum Difficulty difficulty = EASY;
uint32_t guard_ctr = 0;
uint32_t guard_up = 0;
uint32_t guard_down = 0;

void Inp_init(void)
{
  rcu_periph_clock_enable(RCU_GPIOA);
  rcu_periph_clock_enable(RCU_GPIOC);

  gpio_init(GPIOA, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ,
            GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
  gpio_init(GPIOC, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ,
            GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
}

void IO_init(void)
{
  Inp_init(); // inport init
  Lcd_Init(); // LCD init
}

void delay_stable(uint64_t start_stamp)
{
  uint64_t current_stamp = get_timer_value();
  if (current_stamp - start_stamp < SystemCoreClock / 50)
  {
    delay_1ms((uint32_t)((SystemCoreClock / 50 - (current_stamp - start_stamp)) * 4000 / SystemCoreClock));
  }
}

void splash_screen(void)
{
  LCD_Clear(BLACK);
  draw();
  while (!Get_Button(JOY_CTR))
  {
    continue;
  }
  for (int i = 0; i < 5; ++i)
  {
    uint64_t start_stamp = get_timer_value();
    LCD_Fill(2, 2, 158, 78, WHITE);
    LCD_ShowString(30, 32, "Flappy Bird", BLACK);
    delay_stable(start_stamp);
    start_stamp = get_timer_value();
    LCD_Fill(2, 2, 158, 78, BLACK);
    LCD_ShowString(30, 32, "Flappy Bird", WHITE);
    delay_stable(start_stamp);
  }
  guard_ctr = 3;
  LCD_Fill(2, 2, 158, 78, BLACK);
}

void difficulty_selection(void)
{
  while (1)
  {
    uint64_t start_stamp = get_timer_value();
    LCD_ShowString(5, 5, "Easy", difficulty == EASY ? WHITE : GRAY);
    LCD_ShowString(5, 25, "Normal", difficulty == NORMAL ? WHITE : GRAY);
    LCD_ShowString(5, 45, "Hard", difficulty == HARD ? WHITE : GRAY);

    if (Get_Button(JOY_UP) && guard_up == 0)
    {
      difficulty = (difficulty + 2) % 3;
      guard_up = 3;
    }
    else if (Get_Button(JOY_DOWN) && guard_down == 0)
    {
      difficulty = (difficulty + 1) % 3;
      guard_down = 3;
    }
    else if (Get_Button(JOY_CTR) && guard_ctr == 0)
    {
      for (int i = 0; i < 5; ++i)
      {
        start_stamp = get_timer_value();
        LCD_ShowString(5, 5, "Easy", difficulty == EASY ? BLACK : GRAY);
        LCD_ShowString(5, 25, "Normal", difficulty == NORMAL ? BLACK : GRAY);
        LCD_ShowString(5, 45, "Hard", difficulty == HARD ? BLACK : GRAY);
        delay_stable(start_stamp);
        start_stamp = get_timer_value();
        LCD_ShowString(5, 5, "Easy", difficulty == EASY ? WHITE : GRAY);
        LCD_ShowString(5, 25, "Normal", difficulty == NORMAL ? WHITE : GRAY);
        LCD_ShowString(5, 45, "Hard", difficulty == HARD ? WHITE : GRAY);
        delay_stable(start_stamp);
      }
      break;
    }

    if (guard_up > 0)
    {
      --guard_up;
    }
    if (guard_down > 0)
    {
      --guard_down;
    }
    if (guard_ctr > 0)
    {
      --guard_ctr;
    }
    delay_stable(start_stamp);
  }
  LCD_Clear(BLACK);
}

void Flappy_Bird(void)
{
  struct Game curr;
  curr.player.position.x = 80;
  curr.player.position.y = 20;
  curr.player.speed = 0;
  curr.player.life = 3;
  curr.player.score = 0;
  curr.player.guard = 0;
  for (int i = 0; i < TRACE_SIZE; ++i)
  {
    curr.player.trace[i].x = 0;
    curr.player.trace[i].y = 0;
  }
  curr.player.trace_index = 0;
  if (difficulty == EASY)
  {
    curr.wall_speed = 1;
    curr.wall_half_width = 2;
    curr.wall_half_gap = 20;
  }
  else if (difficulty == NORMAL)
  {
    curr.wall_speed = 2;
    curr.wall_half_width = 3;
    curr.wall_half_gap = 15;
  }
  else
  {
    curr.wall_speed = 2;
    curr.wall_half_width = 5;
    curr.wall_half_gap = 10;
  }
  curr.walls[0].top.x = 160;
  curr.walls[0].bottom.x = 160;
  uint32_t y_temp = rand() % (80 - 10 - 2 * curr.wall_half_gap) + 5 + curr.wall_half_gap;
  curr.walls[0].top.y = y_temp - curr.wall_half_gap;
  curr.walls[0].bottom.y = y_temp + curr.wall_half_gap;
  curr.walls[1].top.x = 210;
  curr.walls[1].bottom.x = 210;
  y_temp = rand() % (80 - 10 - 2 * curr.wall_half_gap) + 5 + curr.wall_half_gap;
  curr.walls[1].top.y = y_temp - curr.wall_half_gap;
  curr.walls[1].bottom.y = y_temp + curr.wall_half_gap;
  struct Game pre = curr;

  LCD_DrawRectangle(61, 1, 159, 79, WHITE);
  LCD_ShowString(5, 0, "Life:", WHITE);
  LCD_ShowString(5, 40, "Score:", WHITE);

  uint32_t life_temp = curr.player.life;
  u8 len_curr = 1;
  while (life_temp >= 10)
  {
    life_temp /= 10;
    ++len_curr;
  }
  LCD_ShowNum(52 - 8 * len_curr, 20, curr.player.life, len_curr, WHITE);

  uint32_t score_temp = curr.player.score;
  len_curr = 1;
  while (score_temp >= 10)
  {
    score_temp /= 10;
    ++len_curr;
  }
  LCD_ShowNum(52 - 8 * len_curr, 60, curr.player.score, len_curr, WHITE);

  while (1)
  {
    uint64_t start_stamp = get_timer_value();

    if (curr.player.life != pre.player.life)
    {
      uint32_t life_temp = pre.player.life;
      u8 len_pre = 1;
      while (life_temp >= 10)
      {
        life_temp /= 10;
        ++len_pre;
      }
      life_temp = curr.player.life;
      u8 len_curr = 1;
      while (life_temp >= 10)
      {
        life_temp /= 10;
        ++len_curr;
      }
      LCD_ShowNum(52 - 8 * len_pre, 20, pre.player.life, len_pre, BLACK);
      LCD_ShowNum(52 - 8 * len_curr, 20, curr.player.life, len_curr, WHITE);
    }

    if (curr.player.score != pre.player.score)
    {
      uint32_t score_temp = pre.player.score;
      u8 len_pre = 1;
      while (score_temp >= 10)
      {
        score_temp /= 10;
        ++len_pre;
      }
      score_temp = curr.player.score;
      u8 len_curr = 1;
      while (score_temp >= 10)
      {
        score_temp /= 10;
        ++len_curr;
      }
      LCD_ShowNum(52 - 8 * len_pre, 60, pre.player.score, len_pre, BLACK);
      LCD_ShowNum(52 - 8 * len_curr, 60, curr.player.score, len_curr, WHITE);
    }

    LCD_DrawPoint_big(pre.player.position.x, pre.player.position.y, BLACK);
    if (curr.player.guard == 0 || curr.player.guard % 6 < 3)
    {
      LCD_DrawPoint_big(curr.player.position.x, curr.player.position.y, WHITE);
    }

    if (pre.player.trace_index != 0 && pre.player.trace[TRACE_SIZE - 1].x > 61)
    {
      LCD_DrawLine(pre.player.trace[0].x, pre.player.trace[0].y, pre.player.trace[TRACE_SIZE - 1].x, pre.player.trace[TRACE_SIZE - 1].y, BLACK);
    }
    for (int i = 0; i < TRACE_SIZE - 1; ++i)
    {
      if (pre.player.trace[i].x > 61 && pre.player.trace[i + 1].x > 61)
      {
        LCD_DrawLine(pre.player.trace[i].x, pre.player.trace[i].y, pre.player.trace[i + 1].x, pre.player.trace[i + 1].y, BLACK);
      }
    }
    if (curr.player.trace_index != 0 && curr.player.trace[TRACE_SIZE - 1].x > 61)
    {
      LCD_DrawLine(curr.player.trace[0].x, curr.player.trace[0].y, curr.player.trace[TRACE_SIZE - 1].x, curr.player.trace[TRACE_SIZE - 1].y, WHITE);
    }
    if (curr.player.trace_index != 0)
    {
      for (int i = 0; i < curr.player.trace_index - 1; ++i)
      {
        if (curr.player.trace[i].x > 61 && curr.player.trace[i + 1].x > 61)
        {
          LCD_DrawLine(curr.player.trace[i].x, curr.player.trace[i].y, curr.player.trace[i + 1].x, curr.player.trace[i + 1].y, WHITE);
        }
      }
    }
    for (int i = curr.player.trace_index; i < TRACE_SIZE - 1; ++i)
    {
      if (curr.player.trace[i].x > 61 && curr.player.trace[i + 1].x > 61)
      {
        LCD_DrawLine(curr.player.trace[i].x, curr.player.trace[i].y, curr.player.trace[i + 1].x, curr.player.trace[i + 1].y, WHITE);
      }
    }

    for (int i = 0; i < 2; ++i)
    {
      if (pre.walls[i].top.x - pre.wall_half_width < 159)
      {
        LCD_DrawRectangle(pre.walls[i].top.x - pre.wall_half_width < 62 ? 62 : pre.walls[i].top.x - pre.wall_half_width, 2,
                          pre.walls[i].top.x + pre.wall_half_width > 158 ? 158 : pre.walls[i].top.x + pre.wall_half_width, pre.walls[i].top.y, BLACK);
        LCD_DrawRectangle(pre.walls[i].bottom.x - pre.wall_half_width < 62 ? 62 : pre.walls[i].bottom.x - pre.wall_half_width, pre.walls[i].bottom.y,
                          pre.walls[i].bottom.x + pre.wall_half_width > 158 ? 158 : pre.walls[i].bottom.x + pre.wall_half_width, 78, BLACK);
      }
      if (curr.walls[i].top.x - curr.wall_half_width < 159)
      {
        LCD_DrawRectangle(curr.walls[i].top.x - curr.wall_half_width < 62 ? 62 : curr.walls[i].top.x - curr.wall_half_width, 1,
                          curr.walls[i].top.x + curr.wall_half_width > 158 ? 158 : curr.walls[i].top.x + curr.wall_half_width, curr.walls[i].top.y, WHITE);
        LCD_DrawRectangle(curr.walls[i].bottom.x - curr.wall_half_width < 62 ? 62 : curr.walls[i].bottom.x - curr.wall_half_width, curr.walls[i].bottom.y,
                          curr.walls[i].bottom.x + curr.wall_half_width > 158 ? 158 : curr.walls[i].bottom.x + curr.wall_half_width, 79, WHITE);
      }
    }

    pre = curr;

    curr.player.trace[curr.player.trace_index].x = curr.player.position.x;
    curr.player.trace[curr.player.trace_index].y = curr.player.position.y;
    curr.player.trace_index = (curr.player.trace_index + 1) % TRACE_SIZE;
    for (int i = 0; i < TRACE_SIZE; ++i)
    {
      curr.player.trace[i].x = curr.player.trace[i].x == 0 ? 0 : curr.player.trace[i].x - curr.wall_speed;
    }

    for (int i = 0; i < 2; ++i)
    {
      if (curr.player.position.x > curr.walls[i].top.x - curr.wall_half_width && curr.player.position.x < curr.walls[i].top.x + curr.wall_half_width)
      {
        if ((curr.player.position.y < curr.walls[i].top.y || curr.player.position.y > curr.walls[i].bottom.y))
        {
          if (curr.player.guard == 0)
          {
            --curr.player.life;
            curr.player.guard = 60;
          }
        }
        else if (curr.player.position.x == curr.walls[i].top.x)
        {
          ++curr.player.score;
        }
      }
    }

    if (Get_Button(JOY_UP))
    {
      curr.player.speed = -JUMP_SPEED;
      curr.player.speed = curr.player.speed < -MAX_SPEED ? -MAX_SPEED : curr.player.speed;
    }
    else
    {
      curr.player.speed += GRAVITY;
      curr.player.speed = curr.player.speed > MAX_SPEED ? MAX_SPEED : curr.player.speed;
    }

    curr.player.position.y += curr.player.speed;

    if (curr.player.position.y < 3 || curr.player.position.y > 77)
    {
      curr.player.position.y = 20;
      curr.player.trace_index = 0;
      for (int i = 0; i < TRACE_SIZE; ++i)
      {
        curr.player.trace[i].x = 0;
        curr.player.trace[i].y = 0;
      }
      curr.player.speed = 0;
      if (curr.player.guard == 0)
      {
        --curr.player.life;
        curr.player.guard = 60;
      }
    }

    if (curr.player.life == 0)
    {
      curr.player.life = 3;
      curr.player.score = 0;
    }

    if (curr.player.guard > 0)
    {
      --curr.player.guard;
    }

    for (int i = 0; i < 2; ++i)
    {
      curr.walls[i].top.x -= curr.wall_speed;
      curr.walls[i].bottom.x -= curr.wall_speed;
      if (curr.walls[i].top.x < 61)
      {
        curr.walls[i].top.x = 160;
        curr.walls[i].bottom.x = 160;
        uint32_t y_temp = rand() % (80 - 10 - 2 * curr.wall_half_gap) + 5 + curr.wall_half_gap;
        curr.walls[i].top.y = y_temp - curr.wall_half_gap;
        curr.walls[i].bottom.y = y_temp + curr.wall_half_gap;
      }
    }

    delay_stable(start_stamp);
  }
}

int main(void)
{
  IO_init();
  splash_screen();
  difficulty_selection();
  Flappy_Bird();
}
