#define GL_SILENCE_DEPRECATION

#include "Raytracer.h"

#define RES_DIR string("resources/")
#define MAPS_DIR RES_DIR + "maps/"
#define TEX_DIR RES_DIR + "textures/"
#define SRC_DIR string("src/")

#define radToCoord(r) (int)(r) >> 6
#define IMUL_2(x) x << 1
#define IDIV_2(x) x >> 1
#define IMUL_4(x) x << 2
#define IDIV_4(x) x >> 2
#define IMUL_8(x) x << 3
#define IDIV_8(x) x >> 3
#define IIMUL_16(x) x << 4
#define IDIV_16(x) x >> 4
#define IMUL_32(x) x << 5
#define IDIV_32(x) x >> 5;

#define CEILING_COLOUR LIGHT_GREY
#define FLOOR_COLOUR DARK_GREY

using namespace std;

// Screen
int screenW = 1024;
int screenH = 512;
Colour bg_colour = {0.3, 0.3, 0.3, 0.0};

TextureLoader texLoader;
HashTable<Texture> textures;
vector<Colour> emptyCol(screenH);

// Player
Player player;

// Map
int mapScreenW = screenW; // x >> 1 == x / 2
int mapScreenH = screenH;

// Configs
ConfigInit cfgInit;

PlayerCfg playerCfg;
MinimapCfg minimapCfg;
LoggingCfg loggingCfg;
RenderCfg renderCfg;

GLDebugContext debugContext;

GameMap gameMap = GameMap();

///
/// Render a line between two points (ax, ay) and (bx, by) with a given width
///
/// @param float ax: X-axis value for point A
/// @param float ay: Y-axis value for point A
/// @param float bx: X-axis value for point B
/// @param float by: Y-axis value for point B
/// @param float line_width: Width of the ray to draw
///
/// @return void
///
void renderRay(float ax, float ay, float bx, float by, int line_width) {
    toColour(WHITE);
    glLineWidth((float)line_width);

    glBegin(GL_LINES);

    glVertex2f(ax, ay);
    glVertex2f(bx, by);

    glEnd();
}

///
/// Render the player
///
/// @return void
///
void renderPlayerPos() {
    toColour(YELLOW);
    glPointSize(8);

    // Draw player point
    glBegin(GL_POINTS);
    glVertex2d(player.x, player.y);

    glEnd();

    // Draw direction vector
    renderRay(player.x, player.y, player.x + player.dx * 5, player.y + player.dy * 5, 3);
}

///
/// Render the map as squares
///
/// @return void
///
void renderMap2D() {
    int x, y;
    for (y = 0; y < gameMap.map_height; y++) {
        for (x = 0; x < gameMap.map_width; x++) {
            // Change to colour coresponding to map location
            toColour(gameMap.getAt(x, y).texColour);
            drawRectangle(x * gameMap.wall_width, y * gameMap.wall_height, gameMap.wall_width, gameMap.wall_height);
        }
    }
}

///
/// Euclidean distance between two points (ax, ay) and (bx, by)
///
/// @param float ax: X-axis value for point A
/// @param float ay: Y-axis value for point A
/// @param float bx: X-axis value for point B
/// @param float by: Y-axis value for point B
/// @param float ang: Ray angle
///
/// @return float
///
float dist(float ax, float ay, float bx, float by, float ang) {
    return sqrtf((bx - ax) * (bx - ax) + (by - ay) * (by - ay));
}

///
/// Check for intersections on the horizonal plane
///
/// @param int mx: X-axis location on map
/// @param int my: Y-axis location on map
/// @param int mp: Map array index relative to (x,y) position
/// @param float dof: Current dof value to calculate ray at
/// @param float rx: X-axis location of ray
/// @param float ry: Y-axis location of ray
/// @param float ra: Ray angle (radians)
/// @param float x_off: X-axis location offset
/// @param float y_off: Y-axis location offset
/// @param float hx: X-axis location of horizontal intersection
/// @param float hx: Y-axis location of horizontal intersection
/// @param float disH: Distance from ray start to horizontal intersection
///
/// @return void
///
void checkHorizontal(int &mx, int &my, int &mp, float &dof,
                     float &rx, float &ry, float &ra, float &x_off, float &y_off,
                     float &hx, float &hy, float &disH) {
    float aTan = -1 / tan(ra);
    if (ra > M_PI) {
        // Looking up
        ry = ((radToCoord(player.y)) << 6) - 0.0001f;
        rx = (player.y - ry) * aTan + player.x;
        y_off = -gameMap.wall_height;
        x_off = -y_off * aTan;
    }

    if (ra < M_PI) {
        // Looking down
        ry = (float)((radToCoord(player.y)) << 6) + gameMap.wall_height;
        rx = (player.y - ry) * aTan + player.x;
        y_off = gameMap.wall_height;
        x_off = -y_off * aTan;
    }

    if (ra == 0 || ra == M_PI) {
        // Looking left or right
        rx = player.x;
        ry = player.y;
        dof = playerCfg.dof;
    }

    while (dof < playerCfg.dof) {
        mx = radToCoord(rx);
        my = radToCoord(ry);
        mp = my * gameMap.map_width + mx;

        if (mp > 0 && mp < gameMap.size && gameMap.getAt(mx, my).texColour != NONE) {
            dof = playerCfg.dof;
            hx = rx;
            hy = ry;
            disH = dist(player.x, player.y, hx, hy, ra);
        } else {
            rx += x_off;
            ry += y_off;
            dof += 1;
        }
    }
}

///
/// Check for intersections on the vertical plane
///
/// @param int mx: X-axis location on map
/// @param int my: Y-axis location on map
/// @param int mp: Map array index relative to (x,y) position
/// @param float dof: Current dof value to calculate ray at
/// @param float rx: X-axis location of ray
/// @param float ry: Y-axis location of ray
/// @param float ra: Ray angle (radians)
/// @param float x_off: X-axis location offset
/// @param float y_off: Y-axis location offset
/// @param float vx: X-axis location of vertical intersection
/// @param float vx: Y-axis location of vertical intersection
/// @param float disV: Distance from ray start to vertical intersection
///
/// @return void
///
void checkVertical(int &mx, int &my, int &mp, float &dof,
                   float &rx, float &ry, float &ra, float &x_off, float &y_off,
                   float &vx, float &vy, float &disV) {
    float nTan = -tan(ra);
    if (ra > M_PI_2 && ra < THREE_HALF_PI) {
        // Looking left
        rx = ((radToCoord(player.x)) << 6) - 0.0001f;
        ry = (player.x - rx) * nTan + player.y;
        x_off = -gameMap.wall_width;
        y_off = -x_off * nTan;
    }

    if (ra < M_PI_2 || ra > THREE_HALF_PI) {
        // Looking right
        rx = (float)((radToCoord(player.x)) << 6) + gameMap.wall_width;
        ry = (player.x - rx) * nTan + player.y;
        x_off = gameMap.wall_width;
        y_off = -x_off * nTan;
    }

    if (ra == 0 || ra == M_PI) {
        // Looking up or down
        rx = player.x;
        ry = player.y;
        dof = playerCfg.dof;
    }

    while (dof < playerCfg.dof) {
        mx = radToCoord(rx);
        my = radToCoord(ry);
        mp = my * gameMap.map_width + mx;

        if (mp > 0 && mp < gameMap.size && gameMap.getAt(mx, my).texColour != NONE) {
            dof = playerCfg.dof;
            vx = rx;
            vy = ry;
            disV = dist(player.x, player.y, vx, vy, ra);
        } else {
            rx += x_off;
            ry += y_off;
            dof += 1;
        }
    }
}

float validateAngle(float angle) {
    if (angle < 0) {
        angle += (float)(2 * M_PI);
    } else if (angle > 2 * M_PI) {
        angle -= (float)(2 * M_PI);
    }
    return angle;
}

///
/// Render a column (wall)
///
/// @param int r: Current ray index
/// @param float ra: Ray angle (radians)
/// @param float distT: Distance to wall
///
/// @return void
///
void draw3DWalls(int &r, float &ra, float &distT, vector<Colour> *colourStrip, Colour polygonShader, PWOperator<GLdouble> shaderOperator) {
    // Draw 3D walls
    float ca = validateAngle(player.angle - ra);
    distT *= cos(ca);

    float lineH = (gameMap.size * mapScreenH) / distT;
    float lineInViewPercentage = 1;
    if (lineH > mapScreenH) {
        lineInViewPercentage = 1 - ((lineH - ((float) mapScreenH)) / lineH);
        lineH = (float) mapScreenH;
    }

    float line_off = (mapScreenH >> 1) - (lineH / 2);
    bool isInViewFully = line_off == 0;
    float screen_off = minimapCfg.enable ? (float)mapScreenW : 0;

    int cStripSize = colourStrip->size();
    int cOffset = 0;
    if (lineInViewPercentage < 1) {
        cStripSize = cStripSize * lineInViewPercentage;
        cOffset = (colourStrip->size() - cStripSize) >> 1;
    }
    float pixelOffset = 0;
    float pixelStepSize = cStripSize / lineH;
    glEnable(GL_SCISSOR_TEST);
    for (int yPos = line_off; yPos < line_off + lineH; yPos++) {
        Colour c = colourStrip->at(cOffset + min((int)floor(pixelOffset), cStripSize - 1));
        glScissor(r * (mapScreenW / playerCfg.fov) * 2 + screen_off, yPos, (mapScreenW / playerCfg.fov) * 2.1, mapScreenH / playerCfg.fov);
        toClearColour(
            colourMask<GLdouble>(
                c,
                polygonShader,
                shaderOperator
            )
        );
        glClear(GL_COLOR_BUFFER_BIT);
        pixelOffset += pixelStepSize;
    }
    glDisable(GL_SCISSOR_TEST);
}

Wall validateSideRender(float &rx, float &ry, float &disH, float &hx, float &hy, float &disV, float &vx, float &vy, float &distT, bool &shouldRender) {
    Wall hitWall;
    if (disV < disH) {
        rx = vx;
        ry = vy;
        distT = disV;
        hitWall = gameMap.getAt(radToCoord(rx), radToCoord(ry));
    } else if (disH < disV) {
        rx = hx;
        ry = hy;
        distT = disH;
        hitWall = gameMap.getAt(radToCoord(rx), radToCoord(ry));
    } else {
        rx = 0;
        ry = 0;
        shouldRender = false;
    }
    return hitWall;
}

///
/// Cast rays from the player and render walls
///
/// @return void
///
void renderRays2Dto3D() {
    int r{0}, mx{0}, my{0}, mp{0};
    float dof, rx{0}, ry{0}, ra, x_off{0}, y_off{0}, distT{0};
    vector<Colour> prevCol = emptyCol;
    Texture* wall_texture;
    string prev_tex_name;

    ra = validateAngle(player.angle - (DR * (playerCfg.fov / 2)));

    for (r = 0; r < playerCfg.fov; r++) {
        // Check horizontal lines
        dof = 0;
        float disH = numeric_limits<float>::max();
        float hx = player.x;
        float hy = player.y;

        checkHorizontal(mx, my, mp, dof, rx, ry, ra, x_off, y_off, hx, hy, disH);

        // Check vertical lines
        dof = 0;
        float disV = numeric_limits<float>::max();
        float vx = player.x;
        float vy = player.y;

        checkVertical(mx, my, mp, dof, rx, ry, ra, x_off, y_off, vx, vy, disV);

        bool shouldRender = true;
        Wall hitWall = validateSideRender(rx, ry, disH, hx, hy, disV, vx, vy, distT, shouldRender);
        
        if (minimapCfg.enable && minimapCfg.render_rays) {
            renderRay(player.x, player.y, rx, ry, 1);
        }

        NormalDir nDir = hitWall.getNormDir(rx, ry, gameMap.wall_width, gameMap.wall_height);
        bool isLR = nDir == NormalDir::LEFT || nDir == NormalDir::RIGHT;

        int wallIntersectPoint = isLR ? ry : rx;
        int wallSize = (isLR ? mapScreenW : mapScreenH) / (isLR ? gameMap.map_width : gameMap.map_height);
        float wallOffset = ((wallIntersectPoint - (radToCoord(wallIntersectPoint))) % wallSize) / (float) wallSize;

        if (shouldRender && (wall_texture == nullptr || wall_texture->name != prev_tex_name)) {
            wall_texture = textures.get(hitWall.texture_name);
            prev_tex_name = wall_texture->name;
        }
        vector<Colour> bmpColStrip = shouldRender ? wall_texture->texture.getCol(1.0 - wallOffset) : prevCol;
        Colour shader = isLR ? Colour{0.9, 0.9, 0.9, 1.0} : Colour{0.7, 0.7, 0.7, 1.0};

        prevCol = bmpColStrip;

        draw3DWalls(r, ra, distT, &bmpColStrip, shader, PW_mul<GLdouble>);

        ra += DR;
        ra = validateAngle(ra);
    }
}

///
/// Render ceiling quad
///
void drawCeiling() {
    toColour(CEILING_COLOUR);
    drawRectangle(0, 0, screenW, screenH >> 1, true);
}

///
/// Render floor quad
///
void drawFloor() {
    toColour(FLOOR_COLOUR);
    drawRectangle(0, screenH >> 1, screenW, screenH, true);
}

///
/// Render the scene
///
/// @return void
///
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawCeiling();
    drawFloor();
    if (minimapCfg.enable) {
        renderMap2D();
        renderPlayerPos();
    }
    renderRays2Dto3D();
    glutSwapBuffers();
}

///
/// Register key presses for movement and apply position changes accordingly
///
/// @param char key: Key code as a character
/// @param int x: Unused
/// @param int y: Unused
///
/// @return void
///
void buttons(unsigned char key, int x, int y) {
    if (key == 'a') {
        // Turn right
        player.angle -= 0.1f;
        if (player.angle < 0) {
            player.angle += (float)(2 * M_PI);
        }
        player.dx = cos(player.angle) * 5;
        player.dy = sin(player.angle) * 5;
    } else if (key == 'd') {
        // Turn left
        player.angle += 0.1f;
        if (player.angle > 2 * M_PI) {
            player.angle -= (float)(2 * M_PI);
        }
        player.dx = cos(player.angle) * 5;
        player.dy = sin(player.angle) * 5;
    } else if (key == 'w') {
        // Move forward
        player.x += player.dx;
        player.y += player.dy;
    } else if (key == 's') {
        // Move backward
        player.x -= player.dx;
        player.y -= player.dy;
    }
    glutPostRedisplay();
}

///
/// Initialise the display rendering and player position
///
/// @param Colour background_colour: Colour to set background of window to
///
/// @return void
///
void init(Colour background_colour) {
    cfgInit.initAll(playerCfg, minimapCfg, loggingCfg, renderCfg);
    if (minimapCfg.enable) {
        mapScreenW >>= 1;
    }
    debugContext = GLDebugContext(&loggingCfg);

    texLoader = TextureLoader();
    texLoader.loadTextures(textures);
    debugContext.logAppInfo(string("Loaded " + to_string(textures.size()) + " textures"));

    gameMap.readMapFromFile(MAPS_DIR + "map1.txt");
    debugContext.logAppInfo(string("Loaded map: " + MAPS_DIR + "map1.txt"));

    gameMap.wall_width = mapScreenW / gameMap.map_width;
    gameMap.wall_height = mapScreenH / gameMap.map_height;

    fill(emptyCol.begin(), emptyCol.end(), background_colour);

    toClearColour(background_colour);
    gluOrtho2D(0, screenW, screenH, 0);
    player = Player(
        250,
        250,
        cos(player.angle) * 5,
        sin(player.angle) * 5,
        0
    );
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
int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(screenW, screenH);
    glutCreateWindow("Ray Tracer");

    init(bg_colour);
    debugContext.logAppInfo("COMPLETED INIT PHASE");

    glutDisplayFunc(display);
    glutKeyboardFunc(buttons);
    glutPostRedisplay();
    debugContext.logAppInfo("Initialised OpenGL/GLUT display and buttons");
    glutMainLoop();

    return 0;
}
