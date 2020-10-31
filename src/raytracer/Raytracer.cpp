#define GL_SILENCE_DEPRECATION

#include "Raytracer.hpp"

#define CEILING_COLOUR LIGHT_GREY
#define FLOOR_COLOUR DARK_GREY

#define SIGMOID(x, mult) 1 / 1 + exp(mult *x)
#define DIST_SHADING_MULTIPLIER 0.01
#define DIST_SHADING_THRESHOLD 1.6666666666666667

#define SCREEN_WIDTH glutGet(GLUT_WINDOW_WIDTH)
#define SCREEN_HEIGHT glutGet(GLUT_WINDOW_HEIGHT)

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

#define TEXTURE_WIDTH 64   // must be power of two
#define TEXTURE_HEIGHT 64  // must be power of two
#define MAP_WIDTH 24
#define MAP_HEIGHT 24

string worldMap[MAP_WIDTH][MAP_HEIGHT] = {
    {"colorstone", "colorstone", "colorstone", "colorstone", "colorstone", "colorstone", "colorstone", "colorstone", "colorstone", "colorstone", "colorstone", "greystone", "greystone", "mossy", "greystone", "greystone", "mossy", "greystone", "mossy", "greystone", "greystone", "greystone", "mossy", "greystone"},
    {"colorstone", "none", "none", "none", "none", "none", "none", "none", "none", "none", "colorstone", "greystone", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "greystone"},
    {"colorstone", "none", "purplestone", "purplestone", "none", "none", "none", "none", "none", "colorstone", "colorstone", "greystone", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "mossy"},
    {"colorstone", "none", "none", "purplestone", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "mossy"},
    {"colorstone", "none", "purplestone", "purplestone", "none", "none", "none", "none", "none", "colorstone", "colorstone", "greystone", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "greystone"},
    {"colorstone", "none", "none", "none", "none", "none", "none", "none", "none", "none", "colorstone", "greystone", "none", "none", "none", "none", "none", "mossy", "mossy", "mossy", "none", "mossy", "greystone", "mossy"},
    {"colorstone", "colorstone", "colorstone", "colorstone", "none", "colorstone", "colorstone", "colorstone", "colorstone", "colorstone", "colorstone", "greystone", "greystone", "greystone", "greystone", "greystone", "greystone", "mossy", "none", "none", "none", "none", "none", "mossy"},
    {"wood", "wood", "wood", "wood", "none", "wood", "wood", "wood", "wood", "none", "colorstone", "none", "colorstone", "none", "colorstone", "none", "colorstone", "greystone", "none", "greystone", "none", "mossy", "none", "mossy"},
    {"wood", "wood", "none", "none", "none", "none", "none", "none", "wood", "colorstone", "none", "colorstone", "none", "colorstone", "none", "colorstone", "colorstone", "mossy", "none", "none", "none", "none", "none", "mossy"},
    {"wood", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "colorstone", "mossy", "none", "none", "none", "none", "none", "greystone"},
    {"wood", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "colorstone", "mossy", "none", "mossy", "none", "mossy", "none", "mossy"},
    {"wood", "wood", "none", "none", "none", "none", "none", "none", "wood", "colorstone", "none", "colorstone", "none", "colorstone", "none", "colorstone", "colorstone", "mossy", "greystone", "mossy", "none", "mossy", "mossy", "mossy"},
    {"wood", "wood", "wood", "wood", "none", "wood", "wood", "wood", "wood", "colorstone", "colorstone", "greystone", "none", "mossy", "colorstone", "greystone", "colorstone", "purplestone", "purplestone", "purplestone", "none", "purplestone", "purplestone", "purplestone"},
    {"redbrick", "redbrick", "redbrick", "redbrick", "none", "redbrick", "redbrick", "redbrick", "redbrick", "greystone", "mossy", "greystone", "none", "none", "mossy", "none", "mossy", "purplestone", "none", "none", "none", "none", "none", "purplestone"},
    {"redbrick", "redbrick", "none", "none", "none", "none", "none", "redbrick", "redbrick", "greystone", "none", "none", "none", "none", "none", "none", "greystone", "purplestone", "none", "none", "none", "none", "none", "purplestone"},
    {"redbrick", "none", "none", "none", "none", "none", "none", "none", "redbrick", "greystone", "none", "none", "none", "none", "none", "none", "greystone", "purplestone", "none", "none", "none", "none", "none", "purplestone"},
    {"eagle", "none", "none", "none", "none", "none", "none", "none", "eagle", "greystone", "greystone", "greystone", "greystone", "greystone", "mossy", "none", "mossy", "purplestone", "purplestone", "none", "none", "none", "purplestone", "purplestone"},
    {"redbrick", "none", "none", "none", "none", "none", "none", "none", "redbrick", "redbrick", "redbrick", "eagle", "redbrick", "redbrick", "redbrick", "mossy", "mossy", "none", "none", "bluestone", "none", "bluestone", "none", "bluestone"},
    {"redbrick", "redbrick", "none", "none", "none", "none", "none", "redbrick", "redbrick", "redbrick", "none", "none", "none", "redbrick", "redbrick", "none", "bluestone", "none", "bluestone", "none", "none", "none", "bluestone", "bluestone"},
    {"redbrick", "none", "none", "none", "none", "none", "none", "none", "redbrick", "none", "none", "none", "none", "none", "redbrick", "bluestone", "none", "bluestone", "none", "bluestone", "none", "bluestone", "none", "bluestone"},
    {"eagle", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "bluestone"},
    {"redbrick", "none", "none", "none", "none", "none", "none", "none", "redbrick", "none", "none", "none", "none", "none", "redbrick", "bluestone", "none", "bluestone", "none", "bluestone", "none", "bluestone", "none", "bluestone"},
    {"redbrick", "redbrick", "none", "none", "none", "none", "none", "redbrick", "redbrick", "redbrick", "none", "none", "none", "redbrick", "redbrick", "none", "bluestone", "none", "bluestone", "none", "none", "none", "bluestone", "bluestone"},
    {"redbrick", "redbrick", "redbrick", "redbrick", "eagle", "redbrick", "redbrick", "redbrick", "redbrick", "redbrick", "redbrick", "eagle", "redbrick", "redbrick", "redbrick", "bluestone", "bluestone", "bluestone", "bluestone", "bluestone", "bluestone", "bluestone", "bluestone", "bluestone"}};

struct Sprite {
    double x;
    double y;
    string texture;
};

#define SPRITE_COUNT 19

//parameters for scaling and moving the sprites
#define SPRITE_U_DIV 1
#define SPRITE_V_DIV 1
#define SPRITE_V_MOVE 0.0

Sprite sprite[SPRITE_COUNT] = {
    {20.5, 11.5, "greenlight"},
    {18.5, 4.5, "greenlight"},
    {10.0, 4.5, "greenlight"},
    {10.0, 12.5, "greenlight"},
    {3.5, 6.5, "greenlight"},
    {3.5, 20.5, "greenlight"},
    {3.5, 14.5, "greenlight"},
    {14.5, 20.5, "greenlight"},
    {18.5, 10.5, "pillar"},
    {18.5, 11.5, "pillar"},
    {18.5, 12.5, "pillar"},
    {21.5, 1.5, "barrel"},
    {15.5, 1.5, "barrel"},
    {16.0, 1.8, "barrel"},
    {16.2, 1.2, "barrel"},
    {3.5, 2.5, "barrel"},
    {9.5, 15.5, "barrel"},
    {10.0, 15.1, "barrel"},
    {10.5, 15.8, "barrel"},
};

vector<uint32_t> texture[11];

//1D Zbuffer
vector<double> ZBuffer(screenW);

//arrays used to sort the sprites
int spriteOrder[SPRITE_COUNT];
double spriteDistance[SPRITE_COUNT];

double posX = 22.0, posY = 11.5;     //x and y start position
double dirX = -1.0, dirY = 0.0;      //initial direction vector
double planeX = 0.0, planeY = 0.66;  //the 2d raycaster version of camera plane

double newTime = 0;  //time of current frame
double oldTime = 0;  //time of previous frame

double frameTime = 0;

double moveSpeed = frameTime * 3.0;  //the constant value is in squares/second
double rotSpeed = frameTime * 2.0;   //the constant value is in radians/second

#define DARK_SHADER 8355711

//sort the sprites based on distance
void sortSprites(int* order, double* dist, int amount) {
    vector<pair<double, int>> sprites(amount);
    for (int i = 0; i < amount; i++) {
        sprites[i].first = dist[i];
        sprites[i].second = order[i];
    }
    sort(sprites.begin(), sprites.end());
    // restore in reverse order to go from farthest to nearest
    for (int i = 0; i < amount; i++) {
        dist[i] = sprites[amount - i - 1].first;
        order[i] = sprites[amount - i - 1].second;
    }
}

static void reshape(int width, int height) {
    screenW = width;
    screenH = height;
    ZBuffer.resize(width);
    return;
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
    for (int y = screenH / 2 + 1; y < screenH; ++y) {
        // rayDir for leftmost ray (x = 0) and rightmost ray (x = w)
        rayDirX0 = dirX - planeX;
        rayDirY0 = dirY - planeY;
        rayDirX1 = dirX + planeX;
        rayDirY1 = dirY + planeY;

        // Current y position compared to the center of the screen (the horizon)
        p = y - screenH / 2;

        // Vertical position of the camera.
        posZ = 0.5 * screenH;

        // Horizontal distance from the camera to the floor for the current row.
        // 0.5 is the z position exactly in the middle between floor and ceiling.
        rowDistance = posZ / p;

        // calculate the real world step vector we have to add for each x (parallel to camera plane)
        // adding step by step avoids multiplications with a weight in the inner loop
        floorStepX = rowDistance * (rayDirX1 - rayDirX0) / screenW;
        floorStepY = rowDistance * (rayDirY1 - rayDirY0) / screenW;

        // real world coordinates of the leftmost column. This will be updated as we step to the right.
        floorX = posX + rowDistance * rayDirX0;
        floorY = posY + rowDistance * rayDirY0;

        for (int x = 0; x < screenW; ++x) {
            // the cell coord is simply got from the integer parts of floorX and floorY
            cellX = (int)(floorX);
            cellY = (int)(floorY);

            // get the texture coordinate from the fractional part
            tx = (int)(TEXTURE_WIDTH * (floorX - cellX)) & (TEXTURE_WIDTH - 1);
            ty = (int)(TEXTURE_HEIGHT * (floorY - cellY)) & (TEXTURE_HEIGHT - 1);

            floorX += floorStepX;
            floorY += floorStepY;

            // choose texture and draw the pixel
            checkerBoardPattern = (int(cellX + cellY)) & 1;
            floorTexture = checkerBoardPattern == 0 ? "greystone" : "mossy";
            ceilingTexture = "wood";
            // floor
            textures.get(floorTexture, tex);
            color = tex.texture[TEXTURE_WIDTH * ty + tx];
            color = (color >> 1) & DARK_SHADER;  // make a bit darker
            drawPixel(x, screenH - y, PNG::INTtoRGB(color));

            //ceiling (symmetrical, at screenH - y - 1 instead of y)
            textures.get(ceilingTexture, tex);
            color = tex.texture[TEXTURE_WIDTH * ty + tx];
            color = (color >> 1) & DARK_SHADER;  // make a bit darker
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
        //calculate ray position and direction
        cameraX = 2 * x / double(screenW) - 1;  //x-coordinate in camera space
        rayDirX = dirX + planeX * cameraX;
        rayDirY = dirY + planeY * cameraX;

        //which box of the map we're in
        mapX = int(posX);
        mapY = int(posY);

        //length of ray from one x or y-side to next x or y-side
        deltaDistX = abs(1 / rayDirX);
        deltaDistY = abs(1 / rayDirY);

        //what direction to step in x or y-direction (either +1 or -1)

        hit = 0;  //was there a wall hit?

        //calculate step and initial sideDist
        if (rayDirX < 0) {
            stepX = -1;
            sideDistX = (posX - mapX) * deltaDistX;
        } else {
            stepX = 1;
            sideDistX = (mapX + 1.0 - posX) * deltaDistX;
        }
        if (rayDirY < 0) {
            stepY = -1;
            sideDistY = (posY - mapY) * deltaDistY;
        } else {
            stepY = 1;
            sideDistY = (mapY + 1.0 - posY) * deltaDistY;
        }
        //perform DDA
        while (hit == 0) {
            //jump to next map square, OR in x-direction, OR in y-direction
            if (sideDistX < sideDistY) {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            } else {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }
            //Check if ray has hit a wall
            if (worldMap[mapX][mapY] != "none") hit = 1;
        }

        //Calculate distance of perpendicular ray (Euclidean distance will give fisheye effect!)
        if (side == 0)
            perpWallDist = (mapX - posX + (1 - stepX) / 2) / rayDirX;
        else
            perpWallDist = (mapY - posY + (1 - stepY) / 2) / rayDirY;

        //Calculate height of line to draw on screen
        lineHeight = (int)(screenH / perpWallDist);

        //calculate lowest and highest pixel to fill in current stripe
        drawStart = -lineHeight / 2 + screenH / 2;
        if (drawStart < 0) drawStart = 0;
        drawEnd = lineHeight / 2 + screenH / 2;
        if (drawEnd >= screenH) drawEnd = screenH - 1;
        //texturing calculations
        wallTex = worldMap[mapX][mapY];  //1 subtracted from it so that texture 0 can be used!
        
        //calculate value of wallX
        if (side == 0)
            wallX = posY + perpWallDist * rayDirY;
        else
            wallX = posX + perpWallDist * rayDirX;
        wallX -= floor((wallX));

        //x coordinate on the texture
        texX = int(wallX * double(TEXTURE_WIDTH));
        if (side == 0 && rayDirX > 0) texX = TEXTURE_WIDTH - texX - 1;
        if (side == 1 && rayDirY < 0) texX = TEXTURE_WIDTH - texX - 1;

        // TODO: an integer-only bresenham or DDA like algorithm could make the texture coordinate stepping faster
        // How much to increase the texture coordinate per screen pixel
        step = 1.0 * TEXTURE_HEIGHT / lineHeight;
        // Starting texture coordinate
        texPos = (drawStart - screenH / 2 + lineHeight / 2) * step;
        for (int y = drawStart; y < drawEnd; y++) {
            // Cast the texture coordinate to integer, and mask with (TEXTURE_HEIGHT - 1) in case of overflow
            texY = (int)texPos & (TEXTURE_HEIGHT - 1);
            texPos += step;
            textures.get(wallTex, tex);
            color = tex.texture[TEXTURE_HEIGHT * texY + texX];
            //make color darker for y-sides: R, G and B byte each divided through two with a "shift" and an "and"
            if (side == 1) color = (color >> 1) & DARK_SHADER;
            drawPixel(x, screenH - y, PNG::INTtoRGB(color));
        }

        //SET THE ZBUFFER FOR THE SPRITE CASTING
        ZBuffer[x] = perpWallDist;  //perpendicular distance is used
    }
}

inline double sqDist(double ax, double ay, double bx, double by) {
    return pow(bx - ax, 2) + pow(by - ay, 2);
}

inline static void renderSprites() {
    //sort sprites from far to close
    for (int i = 0; i < SPRITE_COUNT; i++) {
        spriteOrder[i] = i;
        spriteDistance[i] = sqDist(sprite[i].x, posX, sprite[i].y, posY);
    }
    sortSprites(spriteOrder, spriteDistance, SPRITE_COUNT);

    double spriteX, spriteY, invDet, transformX, transformY;
    int spriteScreenX, V_MOVEScreen, spriteHeight, drawStartY, drawEndY, spriteWidth, drawStartX, drawEndX, texX, texY, d;
    Texture tex;
    uint32_t color;
    //after sorting the sprites, do the projection and draw them
    for (int i = 0; i < SPRITE_COUNT; i++) {
        //translate sprite position to relative to camera
        spriteX = sprite[spriteOrder[i]].x - posX;
        spriteY = sprite[spriteOrder[i]].y - posY;

        //transform sprite with the inverse camera matrix
        // [ planeX   dirX ] -1                                       [ dirY      -dirX ]
        // [               ]       =  1/(planeX*dirY-dirX*planeY) *   [                 ]
        // [ planeY   dirY ]                                          [ -planeY  planeX ]

        invDet = 1.0 / (planeX * dirY - dirX * planeY);  //required for correct matrix multiplication

        transformX = invDet * (dirY * spriteX - dirX * spriteY);
        transformY = invDet * (-planeY * spriteX + planeX * spriteY);  //this is actually the depth inside the screen, that what Z is in 3D, the distance of sprite to player, matching sqrt(spriteDistance[i])

        spriteScreenX = int((screenW / 2) * (1 + transformX / transformY));

        V_MOVEScreen = int(SPRITE_V_MOVE / transformY);

        //calculate height of the sprite on screen
        spriteHeight = abs(int(screenH / (transformY))) / SPRITE_V_DIV;  //using "transformY" instead of the real distance prevents fisheye
        //calculate lowest and highest pixel to fill in current stripe
        drawStartY = -spriteHeight / 2 + screenH / 2 + V_MOVEScreen;
        if (drawStartY < 0) drawStartY = 0;
        drawEndY = spriteHeight / 2 + screenH / 2 + V_MOVEScreen;
        if (drawEndY >= screenH) drawEndY = screenH - 1;

        //calculate width of the sprite
        spriteWidth = abs(int(screenH / (transformY))) / SPRITE_U_DIV;
        drawStartX = -spriteWidth / 2 + spriteScreenX;
        if (drawStartX < 0)
            drawStartX = 0;
        drawEndX = spriteWidth / 2 + spriteScreenX;
        if (drawEndX >= screenW) drawEndX = screenW - 1;
        textures.get(sprite[spriteOrder[i]].texture, tex);
        //loop through every vertical stripe of the sprite on screen
        for (int stripe = drawStartX; stripe < drawEndX; stripe++) {
            texX = int(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * TEXTURE_WIDTH / spriteWidth) / 256;
            //the conditions in the if are:
            //1) it's in front of camera plane so you don't see things behind you
            //2) it's on the screen (left)
            //3) it's on the screen (right)
            //4) ZBuffer, with perpendicular distance
            if (transformY > 0 && stripe > 0 && stripe < screenW && transformY < ZBuffer[stripe]) {
                for (int y = drawStartY; y < drawEndY; y++)  //for every pixel of the current stripe
                {
                    d = (y - V_MOVEScreen) * 256 - screenH * 128 + spriteHeight * 128;  //256 and 128 factors to avoid floats
                    texY = ((d * TEXTURE_HEIGHT) / spriteHeight) / 256;
                    // cout << "HERE4 " << tex.name <<  endl;
                    color = tex.texture[TEXTURE_WIDTH * texY + texX];  //get current color from the texture
                    if ((color & 0x00FFFFFF) != 0) {
                        //paint pixel if it isn't black, black is the invisible color
                        drawPixel(stripe, screenH - y, PNG::INTtoRGB(color));
                    }
                }
            }
        }
    }
}

inline static void updateTimeTick() {
    oldTime = newTime;
    newTime = glutGet(GLUT_ELAPSED_TIME);
    frameTime = (newTime - oldTime) / 1000.0;  //frametime is the time this frame has taken, in seconds
    // print(1.0 / frameTime);                 //FPS counter
    // IMPL: USE https://learnopengl.com/In-Practice/Text-Rendering TO PRINT OUT THE TEXT TO SCREEN

    //speed modifiers
    moveSpeed = frameTime * 3.0;  //the constant value is in squares/second
    rotSpeed = frameTime * 2.0;   //the constant value is in radians/second
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
        if (worldMap[int(posX + dirX * moveSpeed)][int(posY)] != "none")
            posX += dirX * moveSpeed;
        if (worldMap[int(posX)][int(posY + dirY * moveSpeed)] != "none")
            posY += dirY * moveSpeed;
    }
    //move backwards if no wall behind you
    if (key == 's') {
        if (worldMap[int(posX - dirX * moveSpeed)][int(posY)] != "none")
            posX -= dirX * moveSpeed;
        if (worldMap[int(posX)][int(posY - dirY * moveSpeed)] != "none")
            posY -= dirY * moveSpeed;
    }
    //rotate to the right
    if (key == 'd') {
        //both camera direction and camera plane must be rotated
        double oldDirX = dirX;
        dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
        dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);

        double oldPlaneX = planeX;
        planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
        planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
    }
    //rotate to the left
    if (key == 'a') {
        //both camera direction and camera plane must be rotated
        double oldDirX = dirX;
        dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
        dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);

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

    gameMap.readMapFromJSON(MAPS_DIR + "map1.json");

    rays = vector<Ray>(playerCfg.fov);

    gameMap.wall_width = mapScreenW / gameMap.map_width;
    gameMap.wall_height = mapScreenH / gameMap.map_height;

    mapScalingX = (minimapCfg.size / (float)mapScreenW) * gameMap.map_width;
    mapScalingY = (minimapCfg.size / (float)mapScreenH) * gameMap.map_height;

    astar = AStar(gameMap);
    path = astar.find(gameMap.start, gameMap.end);

    toClearColour(bg_colour);
    gluOrtho2D(0, screenW, screenH, 0);
    player = Player(
        gameMap.start.x * gameMap.wall_width + IDIV_2(gameMap.wall_width),
        gameMap.start.y * gameMap.wall_height + IDIV_2(gameMap.wall_height),
        cos(player.angle) * 5,
        sin(player.angle) * 5,
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