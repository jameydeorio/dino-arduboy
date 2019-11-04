#include <Arduboy2.h>
 
Arduboy2 arduboy;

static const int GAME_TITLE = 0;
static const int GAME_PLAY = 1;
static const int GAME_OVER = 2;
static const int GAME_SCORE = 3;
int gameState = GAME_TITLE;

int mapX = 0;
int mapY = 0;
static const int WORLD_WIDTH = 14;
static const int WORLD_HEIGHT = 7;
static const int TILE_SIZE = 16;
static const int GRASS = 0;
static const int WATER = 1;
static const int TREES = 2;
static const int STONE = 3;
static const int world[WORLD_HEIGHT][WORLD_WIDTH] = {
  { TREES, GRASS, GRASS, WATER, GRASS, GRASS, GRASS, TREES, GRASS, GRASS, GRASS, GRASS, GRASS, TREES },
  { GRASS, WATER, WATER, WATER, GRASS, WATER, GRASS, GRASS, GRASS, GRASS, GRASS, STONE, GRASS, GRASS },
  { GRASS, GRASS, GRASS, GRASS, GRASS, WATER, STONE, GRASS, GRASS, GRASS, TREES, GRASS, GRASS, GRASS },
  { STONE, GRASS, GRASS, STONE, TREES, WATER, WATER, WATER, GRASS, WATER, WATER, GRASS, TREES, GRASS },
  { GRASS, GRASS, GRASS, GRASS, TREES, GRASS, GRASS, GRASS, TREES, WATER, GRASS, GRASS, STONE, TREES },
  { GRASS, GRASS, GRASS, WATER, STONE, GRASS, GRASS, TREES, TREES, TREES, GRASS, GRASS, WATER, WATER },
  { GRASS, WATER, WATER, TREES, GRASS, WATER, WATER, TREES, TREES, GRASS, GRASS, GRASS, GRASS, STONE }
};

static const int TILES_WIDE = WIDTH / TILE_SIZE + 1;
static const int TILES_TALL = HEIGHT / TILE_SIZE + 1;

const unsigned char PROGMEM tiles[] =
{
  // width, height,
  16, 16,

  // grass
  0xff, 0x7f, 0xfb, 0xff, 0xff, 0xbf, 0xff, 0xff, 0xf7, 0xff, 0xfd, 0xff, 0xff, 0xf7, 0x7f, 0xff, 
  0xdf, 0xff, 0xff, 0xfb, 0x7f, 0xff, 0xff, 0xff, 0xef, 0xfe, 0xff, 0xff, 0xfb, 0xff, 0x7f, 0xff, 

  // water
  0x08, 0x10, 0x10, 0x08, 0x10, 0x08, 0x10, 0x10, 0x10, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x20, 0x40, 0x40, 0x20, 0x00, 0x01, 0x02, 0x02, 0x01, 0x02, 0x02, 0x01, 0x02, 0x21, 0x40, 0x40, 

  // trees
  0xff, 0x1f, 0x5b, 0x3f, 0xeb, 0xdd, 0xff, 0xf7, 0xbb, 0xef, 0xfd, 0x7f, 0xe3, 0xcb, 0xe3, 0xff, 
  0xff, 0xc7, 0x96, 0xc7, 0xff, 0xff, 0xef, 0xfd, 0xff, 0xe3, 0xcb, 0xe3, 0xff, 0xff, 0x7b, 0xff, 

  // stone
  0xff, 0xdf, 0x7b, 0x3f, 0x9f, 0x6f, 0x77, 0xab, 0xdb, 0xd7, 0xcd, 0x5f, 0xbf, 0x77, 0xff, 0xff, 
  0xff, 0xc1, 0xdc, 0xd3, 0xaf, 0x9f, 0xae, 0xb0, 0xbb, 0xbd, 0xbd, 0xba, 0xd7, 0xcc, 0x63, 0xff, 
};

void drawWorld() {
  for (int y = 0; y < TILES_TALL; y++) {
    for (int x = 0; x < TILES_WIDE; x++) {
      Sprites::drawOverwrite(x * TILE_SIZE + mapX % TILE_SIZE, y * TILE_SIZE + mapY % TILE_SIZE, tiles, world[y][x]);
      //arduboy.drawBitmap(x * TILE_SIZE + mapX, y * TILE_SIZE + mapY, tiles[world[y][x]], TILE_SIZE, TILE_SIZE, WHITE);
    }
  }
}

void titleScreen() {
  arduboy.setCursor(0, 0);
  arduboy.println("title");

  if (arduboy.justPressed(A_BUTTON)) {
    gameState = GAME_PLAY;
  }
}

void playerInput() {
  if (arduboy.pressed(UP_BUTTON)) {
    mapY += 1;
  }
  if (arduboy.pressed(DOWN_BUTTON)) {
    mapY -= 1;
  }
  if (arduboy.pressed(LEFT_BUTTON)) {
    mapX += 1;
  }
  if (arduboy.pressed(RIGHT_BUTTON)) {
    mapX -= 1;
  }
}

void gamePlay() {
  arduboy.setCursor(0, 0);
  arduboy.println("game");

  playerInput();
  drawWorld();

  if (arduboy.justPressed(A_BUTTON)) {
    gameState = GAME_OVER;
  }
}

void gameOver() {
  arduboy.setCursor(0, 0);
  arduboy.println("game over");
  if (arduboy.justPressed(A_BUTTON)) {
    gameState = GAME_SCORE;
  }
}

void gameScore() {
  arduboy.setCursor(0, 0);
  arduboy.println("high score");
  if (arduboy.justPressed(A_BUTTON)) {
    gameState = GAME_TITLE;
  }
}

void gameLoop() {
  switch(gameState) {
    case GAME_TITLE:
      {
        titleScreen();
        break;
      }

    case GAME_PLAY:
      {
        gamePlay();
        break;
      }

    case GAME_OVER:
      {
        gameOver();
        break;
      }

    case GAME_SCORE:
      {
        gameScore();
        break;
      }
  }
}

void setup() {
  arduboy.begin();
  arduboy.setFrameRate(45);
  arduboy.initRandomSeed();
}

void loop() {
  if (!(arduboy.nextFrame()))
    return;
  arduboy.pollButtons();

  arduboy.clear();

  gameLoop();

  arduboy.display();
}

