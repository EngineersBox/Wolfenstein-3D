#define GL_SILENCE_DEPRECATION

#include "Raytracer.h"

#define radToCoord(r) (int)(r) >> 6

#define CEILING_COLOUR LIGHT_GREY
#define FLOOR_COLOUR DARK_GREY

#define SCREEN_WIDTH glutGet(GLUT_WINDOW_WIDTH)
#define SCREEN_HEIGHT glutGet(GLUT_WINDOW_HEIGHT)

#define FUNC_ADDR(f) string("0x") + to_string((unsigned long int) (&f))

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

GameMap gameMap = GameMap();
vector<Ray> rays(0);

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
inline void renderRay(float ax, float ay, float bx, float by, int line_width) {
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
void renderMap2D(int sw = SCREEN_WIDTH, int sh = SCREEN_HEIGHT) {
    int xSize = 20;
    int ySize = 20;
    int xOffset = minimapCfg.isLeft() ? sw - (gameMap.map_width * xSize) : 0;
    int yOffset = minimapCfg.isTop() ? 0 : sh - (gameMap.map_height * ySize);
    int x, y;
    for (y = 0; y < gameMap.map_height; y++) {
        for (x = 0; x < gameMap.map_width; x++) {
            // Change to colour coresponding to map location
            toColour(gameMap.getAt(x, y).wf_left.f_colour);
            drawRectangle(xOffset + x * xSize, yOffset + y * ySize, xSize, ySize);
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
inline float dist(float ax, float ay, float bx, float by, float ang) {
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
static void checkHorizontal(int &mx, int &my, int &mp, float &dof,
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

        if (mp > 0 && mp < gameMap.size && gameMap.getAt(mx, my).wf_left.f_colour != NONE) {
            dof = playerCfg.dof;
            hx = rx;
            hy = ry;
            disH = dist(player.x, player.y, hx, hy, ra);
        } else {
            rx += x_off;
            ry += y_off;
            dof++;
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
static void checkVertical(int &mx, int &my, int &mp, float &dof,
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

        if (mp > 0 && mp < gameMap.size && gameMap.getAt(mx, my).wf_left.f_colour != NONE) {
            dof = playerCfg.dof;
            vx = rx;
            vy = ry;
            disV = dist(player.x, player.y, vx, vy, ra);
        } else {
            rx += x_off;
            ry += y_off;
            dof++;
        }
    }
}

inline float validateAngle(float angle) {
    if (angle < 0) {
        return angle + (float)(2 * M_PI);
    } else if (angle > 2 * M_PI) {
        return angle - (float)(2 * M_PI);
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
void draw3DWalls(int &r, float &ra, float &distT, vector<Colour> *colourStrip, const Colour polygonShader, PWOperator<GLdouble> shaderOperator) {
    // Draw 3D walls
    const int SH = SCREEN_HEIGHT;
    const int SW = SCREEN_WIDTH;
    distT *= cos(validateAngle(player.angle - ra));

    float lineH = (gameMap.size * SH) / distT;
    float lineInViewPercentage = 1;
    if (lineH > SH) {
        lineInViewPercentage = 1 - ((lineH - ((float) SH)) / lineH);
        lineH = (float) SH;
    }

    const float line_off = (SH / 2) - (lineH / 2);
    const float screen_off = (SW / playerCfg.fov ) * r;

    int cStripSize = colourStrip->size();
    int cOffset = 0;
    if (lineInViewPercentage < 1) {
        cStripSize = cStripSize * lineInViewPercentage;
        cOffset = (colourStrip->size() - cStripSize) >> 1;
    }
    float pixelOffset = 0;
    const float pixelStepSize = cStripSize / lineH;
    const float x_width = (SW / playerCfg.fov) * 2.1;
    const float y_height = (SH / playerCfg.fov) * 2;
    const int cStripLast = cStripSize - 1;
    glEnable(GL_SCISSOR_TEST);
    for (int yPos = line_off; yPos < line_off + lineH; yPos++) {
        Colour c = colourStrip->at(cOffset + min((int)floor(pixelOffset), cStripLast));
        glScissor(screen_off * 2, yPos * 2, x_width, y_height);
        toClearColour(
            colourMask<GLdouble>(
                c,
                polygonShader,
                shaderOperator));
        glClear(GL_COLOR_BUFFER_BIT);
        pixelOffset += pixelStepSize;
    }
    glDisable(GL_SCISSOR_TEST);
}

inline Wall validateSideRender(float &rx, float &ry, float &disH, float &hx, float &hy, float &disV, float &vx, float &vy, float &distT, bool &shouldRender) {
    if (disV < disH) {
        rx = vx;
        ry = vy;
        distT = disV;
        return gameMap.getAt(radToCoord(rx), radToCoord(ry));
    } else if (disH < disV) {
        rx = hx;
        ry = hy;
        distT = disH;
        return gameMap.getAt(radToCoord(rx), radToCoord(ry));
    } else {
        rx = 0;
        ry = 0;
        shouldRender = false;
        return Wall();
    }
}

inline void updatePrevious(const bool isLR, float ry, float rx, WallFace prev_wall_face, Texture &wall_texture, vector<Colour> &bmpColStrip, vector<Colour> &prevCol, Wall &prev_wall, Wall hitWall, NormalDir &prev_dir, NormalDir nDir) {
    const int wallIntersectPoint = isLR ? ry : rx;
    const int wallSize = (isLR ? mapScreenW : mapScreenH) / (isLR ? gameMap.map_width : gameMap.map_height);
    const float wallOffset = ((wallIntersectPoint - (radToCoord(wallIntersectPoint))) % wallSize) / (float)wallSize;
    prev_wall_face = hitWall.getFace(nDir);
    textures.get(prev_wall_face.f_texture, wall_texture);
    bmpColStrip = wall_texture.texture.getCol(1.0 - wallOffset);
    prevCol = bmpColStrip;
    prev_wall = hitWall;
    prev_dir = nDir;
}

///
/// Cast rays from the player and render walls
///
/// @return void
///
void renderRays2Dto3D(vector<Ray>& rays) {
    int r{0}, mx{0}, my{0}, mp{0};
    float dof, rx{0}, ry{0}, ra, x_off{0}, y_off{0}, distT{0};
    vector<Colour> prevCol = emptyCol;
    float prev_wall_offset;
    Wall prev_wall;
    NormalDir prev_dir;
    WallFace prev_wall_face;
    Texture wall_texture;
    string prev_tex_name;
    Colour lr_shader = {0.9, 0.9, 0.9, 1.0};
    Colour ud_shader = {0.7, 0.7, 0.7, 1.0};

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
            rays.at(r) = {player.x, player.y, rx, ry, 1};
        }

        const NormalDir nDir = hitWall.getNormDir(rx, ry, gameMap.wall_width, gameMap.wall_height);
        const bool isLR = nDir == NormalDir::LEFT || nDir == NormalDir::RIGHT;
        vector<Colour> bmpColStrip;
        if (shouldRender) {
            if (hitWall == prev_wall) {
                if (nDir == prev_dir) {
                    if (bmpColStrip == prevCol) {
                        bmpColStrip = prevCol;
                    } else {
                        updatePrevious(isLR, ry, rx, prev_wall_face, wall_texture,
                            bmpColStrip, prevCol, prev_wall, prev_wall, prev_dir, prev_dir);
                    }
                } else {
                    updatePrevious(isLR, ry, rx, prev_wall_face, wall_texture,
                        bmpColStrip, prevCol, prev_wall, prev_wall, prev_dir, nDir);
                }
            } else {
                updatePrevious(isLR, ry, rx, prev_wall_face, wall_texture,
                    bmpColStrip, prevCol, prev_wall, hitWall, prev_dir, nDir);
            }
        } else {
            if (bmpColStrip == prevCol) {
                bmpColStrip = prevCol;
            } else {
                updatePrevious(isLR, ry, rx, prev_wall_face, wall_texture,
                    bmpColStrip, prevCol, prev_wall, prev_wall, prev_dir, prev_dir);
            }
        }
        const Colour shader = isLR ? lr_shader : ud_shader;
        prevCol = bmpColStrip;

        draw3DWalls(r, ra, distT, &bmpColStrip, shader, PW_mul<GLdouble>);

        ra += DR;
        ra = validateAngle(ra);
    }
}

///
/// Render ceiling quad
///
inline void drawCeiling(int width = SCREEN_WIDTH, int height = SCREEN_HEIGHT) {
    toColour(CEILING_COLOUR);
    drawRectangle(0, 0, width, IDIV_2(height), true);
}

///
/// Render floor quad
///
inline void drawFloor(int width = SCREEN_WIDTH, int height = SCREEN_HEIGHT) {
    toColour(FLOOR_COLOUR);
    drawRectangle(0, IDIV_2(height), width, height, true);
}

inline void renderMapRays(vector<Ray>& rays) {
    for (Ray ray : rays) {
        renderRay(ray.ax, ray.ay, ray.bx, ray.by, ray.line_width);
    }
}

static void reshape(int width, int height) {
    drawCeiling(width, height);
    drawFloor(width, height);
    if (minimapCfg.enable) {
        renderMap2D(width, height);
        renderPlayerPos();
        if (minimapCfg.render_rays) {
            renderMapRays(rays);
        }
    }
}

///
/// Render the scene
///
/// @return void
///
static void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawCeiling();
    drawFloor();

    vector<Ray> rays(playerCfg.fov);
    renderRays2Dto3D(rays);
    if (minimapCfg.enable) {
        renderMap2D();
        renderPlayerPos();
        if (minimapCfg.render_rays) {
            renderMapRays(rays);
        }
    }
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
static void keyPress(unsigned char key, int x, int y) {
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
        mapScreenW = IDIV_2(mapScreenW);
    }
    debugContext = GLDebugContext(&loggingCfg);
    debugContext.logAppInfo("Loaded debug context");

    texLoader = TextureLoader();
    texLoader.loadTextures(textures);
    debugContext.logAppInfo(string("Loaded " + to_string(textures.size()) + " textures"));

    string map_filename = MAPS_DIR + "map1.json";
    gameMap.readMapFromJSON(map_filename);
    debugContext.logAppInfo(string("Loaded map: " + map_filename));

    rays = vector<Ray>(playerCfg.fov);

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

inline static void idle(void) {
    glutPostRedisplay();
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
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(screenW, screenH);
    glutCreateWindow("Ray Tracer");

    init(bg_colour);
    debugContext.logAppInfo("---- COMPLETED APPLICATION INIT PHASE ----");

    glutDisplayFunc(display);
    debugContext.logAppInfo("Initialised glutDisplayFunc at: " + FUNC_ADDR(display));
    glutReshapeFunc(reshape);
    debugContext.logAppInfo("Initialised glutReshapeFunc at: " + FUNC_ADDR(reshape));
    glutKeyboardFunc(keyPress);
    debugContext.logAppInfo("Initialised glutKeyboardFunc at: " + FUNC_ADDR(keyPress));
    glutIdleFunc(idle);
    debugContext.logAppInfo("Initialised glutIdleFunc at: " + FUNC_ADDR(idle));
    glutPostRedisplay();
    debugContext.logAppInfo("---- COMPLETED OpenGL/GLUT INIT PHASE ----");
    glutMainLoop();

    return 0;
}
