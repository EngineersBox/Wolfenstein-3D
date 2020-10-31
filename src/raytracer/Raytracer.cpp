#define GL_SILENCE_DEPRECATION

#include "Raytracer.hpp"

#define CEILING_COLOUR LIGHT_GREY
#define FLOOR_COLOUR DARK_GREY

#define SIGMOID(x, mult) 1 / 1 + exp(mult *x)
#define DIST_SHADING_MULTIPLIER 0.01
#define DIST_SHADING_THRESHOLD 1.6666666666666667

#define SCREEN_WIDTH glutGet(GLUT_WINDOW_WIDTH)
#define SCREEN_HEIGHT glutGet(GLUT_WINDOW_HEIGHT)

#define TEXTURE_WIDTH 64
#define TEXTURE_HEIGHT 64

#define SPRITE_U_DIV 1
#define SPRITE_V_DIV 1
#define SPRITE_V_MOVE 0.0

#define DARK_SHADER 8355711

using namespace std;

// Screen
int screenW = 1024;
int screenH = 512;
Colour bg_colour = {0.3, 0.3, 0.3, 0.0};

TextureLoader texLoader;
HashTable<Texture> textures;
AStar astar;
vector<Coords> *path = new vector<Coords>();
// Player
Player player;

// Map
int mapScreenW = screenW;  // x >> 1 == x / 2
int mapScreenH = screenH;

float mapScalingX;
float mapScalingY;

// Configs
ConfigInit cfgInit;

GameMap gameMap = GameMap();
vector<Ray> rays(0);

vector<double> ZBuffer(screenW);

vector<int> spriteOrder;
vector<double> spriteDistance;

double planeX = 0.0, planeY = 0.66;

double newTime = 0;
double oldTime = 0;

double frameTime = 0;

double moveSpeed = frameTime * 3.0;
double rotSpeed = frameTime * 2.0;

///
/// Render the player
///
/// @return void
///
inline static void renderPlayerPos(int sw = screenW, int sh = screenH) {
    int xOffset = minimapCfg.isLeft() ? 0 : sw - (gameMap.map_width * minimapCfg.size);
    int yOffset = minimapCfg.isTop() ? 0 : sh - (gameMap.map_height * minimapCfg.size);

    toColour(RED);
    glPointSize(8);

    // Draw player point
    glBegin(GL_POINTS);
    glVertex2d(xOffset + (player.x * mapScalingX), yOffset + (player.y * mapScalingY));

    glEnd();

    // Draw direction vector
    renderRay(
        xOffset + (player.x * mapScalingX),
        yOffset + (player.y * mapScalingY),
        xOffset + ((player.x + player.dx * 5) * mapScalingX),
        yOffset + ((player.y + player.dy * 5) * mapScalingY),
        3, RED);
}

///
/// Render the map as squares
///
/// @return void
///
static void renderMap2D(int sw = screenW, int sh = screenH) {
    int xOffset = minimapCfg.isLeft() ? 0 : sw - (gameMap.map_width * minimapCfg.size);
    int yOffset = minimapCfg.isTop() ? 0 : sh - (gameMap.map_height * minimapCfg.size);
    int x, y;
    for (y = 0; y < gameMap.map_height; y++) {
        for (x = 0; x < gameMap.map_width; x++) {
            // Change to colour coresponding to map location
            toColour(gameMap.getAt(x, y).wf_left.f_colour);
            drawRectangle(xOffset + x * minimapCfg.size, yOffset + y * minimapCfg.size, minimapCfg.size, minimapCfg.size);
        }
    }
}

void sortSprites(int amount) {
    vector<pair<double, int>> sprites(amount);
    for (int i = 0; i < amount; i++) {
        sprites[i].first = spriteDistance[i];
        sprites[i].second = spriteOrder[i];
    }
    sort(sprites.begin(), sprites.end());
    for (int i = 0; i < amount; i++) {
        spriteDistance[i] = sprites[amount - i - 1].first;
        spriteOrder[i] = sprites[amount - i - 1].second;
    }
}

static void reshape(int width, int height) {
    screenW = width;
    screenH = height;
    ZBuffer.resize(width);
}

inline void drawPixel(int x, int y, PNG::ColorRGB colour) {
    glScissor(x, y, 1, 1);
    glClearColor(
        colour.r / 255.0,
        colour.g / 255.0,
        colour.g / 255.0,
        0);
    glClear(GL_COLOR_BUFFER_BIT);
}

inline static void renderFloorCeiling() {
    float rayDirX0, rayDirY0, rayDirX1, rayDirY1, posZ, rowDistance, floorStepX, floorStepY, floorX, floorY;
    int p, cellX, cellY, tx, ty, checkerBoardPattern;
    Texture tex;
    string floorTexture, ceilingTexture;
    uint32_t color;
    for (int y = (IDIV_2(screenH)) + 1; y < screenH; ++y) {
        rayDirX0 = player.dx - planeX;
        rayDirY0 = player.dy - planeY;
        rayDirX1 = player.dx + planeX;
        rayDirY1 = player.dy + planeY;

        p = y - (IDIV_2(screenH));

        posZ = 0.5 * screenH;

        rowDistance = posZ / p;

        floorStepX = rowDistance * (rayDirX1 - rayDirX0) / screenW;
        floorStepY = rowDistance * (rayDirY1 - rayDirY0) / screenW;

        floorX = player.x + rowDistance * rayDirX0;
        floorY = player.y + rowDistance * rayDirY0;

        for (int x = 0; x < screenW; ++x) {
            cellX = (int)(floorX);
            cellY = (int)(floorY);

            tx = (int)(TEXTURE_WIDTH * (floorX - cellX)) & (TEXTURE_WIDTH - 1);
            ty = (int)(TEXTURE_HEIGHT * (floorY - cellY)) & (TEXTURE_HEIGHT - 1);

            floorX += floorStepX;
            floorY += floorStepY;

            checkerBoardPattern = (int(cellX + cellY)) & 1;
            floorTexture = checkerBoardPattern == 0 ? "greystone" : "mossy";
            ceilingTexture = "wood";

            textures.get(floorTexture, tex);
            color = tex.texture[TEXTURE_WIDTH * ty + tx];
            color = (color >> 1) & DARK_SHADER;
            drawPixel(x, screenH - y, PNG::INTtoRGB(color));

            textures.get(ceilingTexture, tex);
            color = tex.texture[TEXTURE_WIDTH * ty + tx];
            color = (color >> 1) & DARK_SHADER;
            drawPixel(x, y - 1, PNG::INTtoRGB(color));
        }
    }
}

inline static void renderWalls() {
    double cameraX, rayDirX, rayDirY, sideDistX, sideDistY, deltaDistX, deltaDistY, perpWallDist, wallX, step, texPos;
    int mapX, mapY, stepX, stepY, hit, side, lineHeight, drawStart, drawEnd, texX, texY;
    string wallTex;
    uint32_t color;
    Texture tex;
    for (int x = 0; x < screenW; x++) {
        cameraX = 2 * x / double(screenW) - 1;
        rayDirX = player.dx + planeX * cameraX;
        rayDirY = player.dy + planeY * cameraX;

        mapX = (int)player.x;
        mapY = (int)player.y;

        deltaDistX = abs(1 / rayDirX);
        deltaDistY = abs(1 / rayDirY);

        hit = 0;
        if (rayDirX < 0) {
            stepX = -1;
            sideDistX = (player.x - mapX) * deltaDistX;
        } else {
            stepX = 1;
            sideDistX = (mapX + 1.0 - player.x) * deltaDistX;
        }
        if (rayDirY < 0) {
            stepY = -1;
            sideDistY = (player.y - mapY) * deltaDistY;
        } else {
            stepY = 1;
            sideDistY = (mapY + 1.0 - player.y) * deltaDistY;
        }

        while (hit == 0) {
            if (sideDistX < sideDistY) {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            } else {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }
            hit = gameMap.getAt(mapX, mapY).wf_left.f_texture != "";
        }

        if (side == 0) {
            perpWallDist = (mapX - player.x + (IDIV_2((1 - stepX)))) / rayDirX;
        } else {
            perpWallDist = (mapY - player.y + (IDIV_2((1 - stepY)))) / rayDirY;
        }
        lineHeight = (int)(screenH / perpWallDist);

        drawStart = (IDIV_2(-lineHeight)) + (IDIV_2(screenH));
        if (drawStart < 0) {
            drawStart = 0;
        }
        drawEnd = (IDIV_2(lineHeight)) + (IDIV_2(screenH));
        if (drawEnd >= screenH) {
            drawEnd = screenH - 1;
        }
        wallTex = gameMap.getAt(mapX, mapY).wf_left.f_texture;

        wallX = side == 0 ? player.y + perpWallDist* rayDirY : player.x + perpWallDist * rayDirX;
        wallX -= floor((wallX));

        texX = (int)(wallX * double(TEXTURE_WIDTH));
        if (side == 0 && rayDirX > 0) {
            texX = TEXTURE_WIDTH - texX - 1;
        }
        if (side == 1 && rayDirY < 0) {
            texX = TEXTURE_WIDTH - texX - 1;
        }

        step = 1.0 * TEXTURE_HEIGHT / lineHeight;
        texPos = (drawStart - (IDIV_2(screenH)) + (IDIV_2(lineHeight))) * step;
        for (int y = drawStart; y < drawEnd; y++) {
            texY = (int)texPos & (TEXTURE_HEIGHT - 1);
            texPos += step;
            textures.get(wallTex, tex);
            color = tex.texture[TEXTURE_HEIGHT * texY + texX];
            if (side == 1) {
                color = (color >> 1) & DARK_SHADER;
            }
            drawPixel(x, screenH - y, PNG::INTtoRGB(color));
        }

        ZBuffer[x] = perpWallDist;
    }
}

inline double sqDist(double ax, double ay, double bx, double by) {
    return pow(bx - ax, 2) + pow(by - ay, 2);
}

inline static void renderSprites() {
    for (int i = 0; i < gameMap.sprites.size(); i++) {
        spriteOrder[i] = i;
        spriteDistance[i] = sqDist(gameMap.sprites[i].x, player.x, gameMap.sprites[i].y, player.y);
    }
    sortSprites(gameMap.sprites.size());

    double spriteX, spriteY, invDet, transformX, transformY;
    int spriteScreenX, V_MOVEScreen, spriteHeight, drawStartY, drawEndY, spriteWidth, drawStartX, drawEndX, texX, texY, d;
    Texture tex;
    uint32_t color;
    for (int i = 0; i < gameMap.sprites.size(); i++) {
        spriteX = gameMap.sprites[spriteOrder[i]].x - player.x;
        spriteY = gameMap.sprites[spriteOrder[i]].y - player.y;

        invDet = 1.0 / (planeX * player.dy - player.dx * planeY);

        transformX = invDet * (player.dy * spriteX - player.dx * spriteY);
        transformY = invDet * (-planeY * spriteX + planeX * spriteY);

        spriteScreenX = (int)((IDIV_2(screenW)) * (1 + transformX / transformY));

        V_MOVEScreen = (int)(SPRITE_V_MOVE / transformY);

        spriteHeight = abs((int)(screenH / (transformY))) / SPRITE_V_DIV;
        drawStartY = -(IDIV_2(spriteHeight)) + (IDIV_2(screenH)) + V_MOVEScreen;
        if (drawStartY < 0) {
            drawStartY = 0;
        }
        drawEndY = (IDIV_2(spriteHeight)) + (IDIV_2(screenH)) + V_MOVEScreen;
        if (drawEndY >= screenH) {
            drawEndY = screenH - 1;
        }

        spriteWidth = abs((int)(screenH / (transformY))) / SPRITE_U_DIV;
        drawStartX = (IDIV_2(-spriteWidth)) + spriteScreenX;
        if (drawStartX < 0) {
            drawStartX = 0;
        }
        drawEndX = (IDIV_2(spriteWidth)) + spriteScreenX;
        if (drawEndX >= screenW) {
            drawEndX = screenW - 1;
        }
        textures.get(gameMap.sprites[spriteOrder[i]].texture, tex);
        for (int stripe = drawStartX; stripe < drawEndX; stripe++) {
            texX = (int)IDIV_256((IMUL_256((stripe - ((IDIV_2(-spriteWidth)) + spriteScreenX))) * TEXTURE_WIDTH / spriteWidth));
            if (!(transformY > 0 && stripe > 0 && stripe < screenW && transformY < ZBuffer[stripe])) {
                continue;
            }
            for (int y = drawStartY; y < drawEndY; y++) {
                d = IMUL_256((y - V_MOVEScreen)) - IMUL_128(screenH) + IMUL_128(spriteHeight);
                texY = IDIV_256((d * TEXTURE_HEIGHT) / spriteHeight);
                color = tex.texture[TEXTURE_WIDTH * texY + texX];
                if ((color & 0x00FFFFFF) != 0) {
                    drawPixel(stripe, screenH - y, PNG::INTtoRGB(color));
                }
            }
        }
    }
}

inline static void updateTimeTick() {
    oldTime = newTime;
    newTime = glutGet(GLUT_ELAPSED_TIME);
    frameTime = (newTime - oldTime) / 1000.0;
    // print(1.0 / frameTime);
    // IMPL: USE https://learnopengl.com/In-Practice/Text-Rendering TO PRINT OUT THE TEXT TO SCREEN

    moveSpeed = frameTime * 3.0;  // 3 squares per second
    rotSpeed = frameTime * 2.0;   // 2 radians per second
}

static void display(void) {
    glEnable(GL_SCISSOR_TEST);

    renderFloorCeiling();
    renderWalls();
    renderSprites();

    glDisable(GL_SCISSOR_TEST);
    updateTimeTick();

    glutSwapBuffers();
}

static void idle(void) {
    glutPostRedisplay();
}

static void keyPress(unsigned char key, int x, int y) {
    if (key == 'w') {
        if (gameMap.getAt((int)(player.x + player.dx * moveSpeed), (int)player.y).wf_left.f_texture != "")
            player.x += player.dx * moveSpeed;
        if (gameMap.getAt((int)player.x, (int)(player.y + player.dy * moveSpeed)).wf_left.f_texture != "")
            player.y += player.dy * moveSpeed;
    }
    if (key == 's') {
        if (gameMap.getAt((int)(player.x - player.dx * moveSpeed), (int)player.y).wf_left.f_texture != "")
            player.x -= player.dx * moveSpeed;
        if (gameMap.getAt((int)player.x, (int)(player.y - player.dy * moveSpeed)).wf_left.f_texture != "")
            player.y -= player.dy * moveSpeed;
    }
    if (key == 'd') {
        double oldDirX = player.dx;
        player.dx = player.dx * cos(-rotSpeed) - player.dy * sin(-rotSpeed);
        player.dy = oldDirX * sin(-rotSpeed) + player.dy * cos(-rotSpeed);

        double oldPlaneX = planeX;
        planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
        planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
    }
    if (key == 'a') {
        double oldDirX = player.dx;
        player.dx = player.dx * cos(rotSpeed) - player.dy * sin(rotSpeed);
        player.dy = oldDirX * sin(rotSpeed) + player.dy * cos(rotSpeed);

        double oldPlaneX = planeX;
        planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
        planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
    }
    glutPostRedisplay();
}

///
/// Initialise the display rendering and player position
///
/// @return void
///
void init() {
    cfgInit.initAll(playerCfg, minimapCfg, loggingCfg, renderCfg);
    if (minimapCfg.enable) {
        mapScreenW = IDIV_2(mapScreenW);
    }
    debugContext = GLDebugContext(&loggingCfg);
    debugContext.logAppInfo("Loaded debug context");

    texLoader = TextureLoader();
    texLoader.loadTextures(textures);
    debugContext.logAppInfo(string("Loaded " + to_string(textures.size()) + " textures"));

    gameMap.readMapFromJSON(MAPS_DIR + "map2.json");

    spriteOrder.resize(gameMap.sprites.size());
    spriteDistance.resize(gameMap.sprites.size());

    rays = vector<Ray>(playerCfg.fov);

    gameMap.wall_width = mapScreenW / gameMap.map_width;
    gameMap.wall_height = mapScreenH / gameMap.map_height;

    mapScalingX = (minimapCfg.size / (float)mapScreenW) * gameMap.map_width;
    mapScalingY = (minimapCfg.size / (float)mapScreenH) * gameMap.map_height;

    // astar = AStar(gameMap);
    // path = astar.find(gameMap.start, gameMap.end);

    toClearColour(bg_colour);
    gluOrtho2D(0, screenW, screenH, 0);
    player = Player(
        gameMap.start.x,
        gameMap.start.y,
        -1.0,
        0.0,
        0);
    debugContext.logAppInfo("Initialised player object");
}

///
/// Main execution
///
/// @param int argc: Call value
/// @param char argv: Program parameters
///
/// @return int
///
int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(screenW, screenH);
    glutCreateWindow("Ray Caster");

    init();
    debugContext.logAppInfo("---- COMPLETED APPLICATION INIT PHASE ----");

    glutDisplayFunc(display);
    debugContext.logApiInfo("Initialised glutDisplayFunc at: " + ADDR_OF(display));
    glutReshapeFunc(reshape);
    debugContext.logApiInfo("Initialised glutReshapeFunc at: " + ADDR_OF(reshape));
    glutKeyboardFunc(keyPress);
    debugContext.logApiInfo("Initialised glutKeyboardFunc at: " + ADDR_OF(keyPress));
    glutIdleFunc(idle);
    debugContext.logApiInfo("Initialised glutIdleFunc at: " + ADDR_OF(idle));
    glutPostRedisplay();
    debugContext.logApiInfo("---- COMPLETED OpenGL/GLUT INIT PHASE ----");
    debugContext.logApiInfo("Started glutMainLoop()");
    glutPostRedisplay();
    glutMainLoop();
    return 0;
}