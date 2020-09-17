#define GL_SILENCE_DEPRECATION

#include "Raytracer.h"

#include "../environment/Walls.h"
#include "../map/map.h"
#include "../texturing/ColourUtils.h"
#include "../texturing/TextureColours.h"
#include "../texturing/texture.h"

using namespace std;

#define radToCoord(r) (int)(r) >> 6

vector<Texture> textures(1);

// Screen
int screenW = 1024;
int screenH = 512;
Colour bg_colour = {0.3, 0.3, 0.3, 0.0};

// Player
float p_x, p_y, p_dx, p_dy, p_a;
float fov = 70.0f;
float p_dof = 8.0f;
bool renderRays = true;

// Map
bool render2DMap = true;
int mapScreenW = screenW >> 1; // Same as div by 2
int mapScreenH = screenH;

GameMap gameMap = GameMap();

double posX = 22.0, posY = 11.5;     //x and y start position
double dirX = -1.0, dirY = 0.0;      //initial direction vector
double planeX = 0.0, planeY = 0.66;  //the 2d raycaster version of camera plane

double time_l = 0;     //time of current frame
double oldTime = 0;  //time of previous frame


///
/// Render a square at coodinates with top-left origin
///
/// @param int x: X coordinate
/// @param int y: Y coordinate
/// @param int sidelength: Side length of the square
///
/// @return void
///
void drawSquare(int x, int y, int sidelength) {
    glBegin(GL_QUADS);

    glVertex2i(x + 1, y + 1);                            // Top right
    glVertex2i(x + 1, y + sidelength - 1);               // Top left
    glVertex2i(x + sidelength - 1, y + sidelength - 1);  // Bottom left
    glVertex2i(x + sidelength - 1, y + 1);               // Bottom right

    glEnd();
}

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
void drawPlayer() {
    toColour(YELLOW);
    glPointSize(8);

    // Draw player point
    glBegin(GL_POINTS);
    glVertex2d(p_x, p_y);

    glEnd();

    // Draw direction vector
    renderRay(p_x, p_y, p_x + p_dx * 5, p_y + p_dy * 5, 3);
}

///
/// Render the map as squares
///
/// @return void
///
void drawMap2D() {
    int x, y, squareWidth = mapScreenH / gameMap.map_height;
    for (y = 0; y < gameMap.map_height; y++) {
        for (x = 0; x < gameMap.map_width; x++) {
            // Change to colour coresponding to map location
            toColour(gameMap.getAt(x, y).getColour());
            drawSquare(x * squareWidth, y * squareWidth, squareWidth);
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
        ry = (((int)p_y >> 6) << 6) - 0.0001f;
        rx = (p_y - ry) * aTan + p_x;
        y_off = -64;
        x_off = -y_off * aTan;
    }

    if (ra < M_PI) {
        // Looking down
        ry = (float)(((int)p_y >> 6) << 6) + 64;
        rx = (p_y - ry) * aTan + p_x;
        y_off = 64;
        x_off = -y_off * aTan;
    }

    if (ra == 0 || ra == M_PI) {
        // Looking left or right
        rx = p_x;
        ry = p_y;
        dof = p_dof;
    }

    while (dof < p_dof) {
        mx = radToCoord(rx);
        my = radToCoord(ry);
        mp = my * gameMap.map_width + mx;

        if (mp > 0 && mp < gameMap.map_width * gameMap.map_height && gameMap.getAt(mx, my).getColour() != NONE) {
            dof = p_dof;
            hx = rx;
            hy = ry;
            disH = dist(p_x, p_y, hx, hy, ra);
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
        rx = (((int)p_x >> 6) << 6) - 0.0001f;
        ry = (p_x - rx) * nTan + p_y;
        x_off = -64;
        y_off = -x_off * nTan;
    }

    if (ra < M_PI_2 || ra > THREE_HALF_PI) {
        // Looking right
        rx = (float)(((int)p_x >> 6) << 6) + 64;
        ry = (p_x - rx) * nTan + p_y;
        x_off = 64;
        y_off = -x_off * nTan;
    }

    if (ra == 0 || ra == M_PI) {
        // Looking up or down
        rx = p_x;
        ry = p_y;
        dof = 8;
    }

    while (dof < p_dof) {
        mx = radToCoord(rx);
        my = radToCoord(ry);
        mp = my * gameMap.map_width + mx;

        if (mp > 0 && mp < gameMap.map_width * gameMap.map_height && gameMap.getAt(mx, my).getColour() != NONE) {
            dof = p_dof;
            vx = rx;
            vy = ry;
            disV = dist(p_x, p_y, vx, vy, ra);
        } else {
            rx += x_off;
            ry += y_off;
            dof += 1;
        }
    }
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
void draw3DWalls(int &r, float &ra, float &distT) {
    // Draw 3D walls
    float ca = p_a - ra;
    if (ca < 0) {
        ca += (float)(2 * M_PI);
    } else if (ca > 2 * M_PI) {
        ca -= (float)(2 * M_PI);
    }
    distT *= cos(ca);

    int mapS = gameMap.map_height * gameMap.map_width;
    float lineH = (mapS * mapScreenH) / distT;
    if (lineH > mapScreenH) {
        lineH = (float)mapScreenH;
    }

    float line_off = (mapScreenH >> 1) - (lineH / 2);
    float screen_off = render2DMap ? (float)mapScreenW : 0;
    renderRay(r * gameMap.map_width + screen_off, line_off, r * gameMap.map_width + screen_off, line_off + lineH, (mapS / gameMap.map_width));
}

void renderRaysMap() {
    for (int x = 0; x < screenW; x ++) {
        //calculate ray position and direction
        double cameraX = 2 * x / (double) screenW - 1;  //x-coordinate in camera space
        double rayDirX = dirX + planeX * cameraX;
        double rayDirY = dirY + planeY * cameraX;

        //which box of the map we're in
        int mapX = int(posX);
        int mapY = int(posY);

        //length of ray from current position to next x or y-side
        double sideDistX;
        double sideDistY;

        //length of ray from one x or y-side to next x or y-side
        double deltaDistX = abs(1 / rayDirX);
        double deltaDistY = abs(1 / rayDirY);
        double perpWallDist;

        //what direction to step in x or y-direction (either +1 or -1)
        int stepX;
        int stepY;

        int hit = 0;  //was there a wall hit?
        int side;     //was a NS or a EW wall hit?

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
            int mp = mapY * gameMap.map_width + mapX;
            //Check if ray has hit a wall
            if (mp > 0 && mp < gameMap.map_width * gameMap.map_height && gameMap.getAt(mapX, mapY).getColour() != NONE) {
                hit = 1;
            }
        }

        //Calculate distance of perpendicular ray (Euclidean distance will give fisheye effect!)
        if (side == 0)
            perpWallDist = (mapX - posX + (1 - stepX) / 2) / rayDirX;
        else
            perpWallDist = (mapY - posY + (1 - stepY) / 2) / rayDirY;

        //Calculate height of line to draw on screen
        int lineHeight = (int)(screenH / perpWallDist);

        //calculate lowest and highest pixel to fill in current stripe
        int drawStart = -lineHeight / 2 + screenH / 2;
        if (drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + screenH / 2;
        if (drawEnd >= screenH) drawEnd = screenH - 1;

        //texturing calculations
        // TODO: CHANGE THIS TO GET TEXTURE FROM WALLS
        Texture tex = textures.at(0);  //1 subtracted from it so that texture 0 can be used!
        int texWidth = tex.width;
        int texHeight = tex.height;

        //calculate value of wallX
        double wallX;  //where exactly the wall was hit
        if (side == 0)
            wallX = posY + perpWallDist * rayDirY;
        else
            wallX = posX + perpWallDist * rayDirX;
        wallX -= floor((wallX));

        //x coordinate on the texture
        int texX = int(wallX * double(texWidth));
        if (side == 0 && rayDirX > 0) texX = texWidth - texX - 1;
        if (side == 1 && rayDirY < 0) texX = texWidth - texX - 1;

        // TODO: an integer-only bresenham or DDA like algorithm could make the texture coordinate stepping faster
        // How much to increase the texture coordinate per screen pixel
        double step = 1.0 * texHeight / lineHeight;
        // Starting texture coordinate
        double texPos = (drawStart - screenH / 2 + lineHeight / 2) * step;
        glBegin(GL_POINTS);
        for (int y = drawStart; y < drawEnd; y++) {
            // Cast the texture coordinate to integer, and mask with (texHeight - 1) in case of overflow
            int texY = (int)texPos & (texHeight - 1);
            texPos += step;
            if (texX >= tex.texture.bmp_info_header.width) {
                texX = 0;
            }
            if (texY >= tex.texture.bmp_info_header.height) {
                texY = 0;
            }
            Colour colour = RED;  // tex.texture.get_pixel(texX, texY);
            //make color darker for y-sides: R, G and B byte each divided through two with a "shift" and an "and"
            // if (side == 1) color = (color >> 1) & 8355711;
            // if (side == 1) {
            //     c_r *= 0.8;
            //     c_g *= 0.8;
            //     c_b *= 0.8;
            // }
            toColour(colour);
            glVertex2i(x, y);
            //buffer[y][x] = color;
        }
        glEnd();
        throw MemoryError("HALTING");
    }
}

///
/// Cast rays from the player and render walls
///
/// @return void
///
void renderRays2Dto3D() {
    int r{}, mx{}, my{}, mp{};
    float dof, rx{}, ry{}, ra, x_off{}, y_off{}, distT{};

    ra = p_a - (DR * (fov / 2));

    if (ra < 0) {
        ra += (float)(2 * M_PI);
    } else if (ra > 2 * M_PI) {
        ra -= (float)(2 * M_PI);
    }

    for (r = 0; r < fov; r++) {
        // Check horizontal lines
        dof = 0;
        float disH = numeric_limits<float>::max();
        float hx = p_x;
        float hy = p_y;

        checkHorizontal(mx, my, mp, dof, rx, ry, ra, x_off, y_off, hx, hy, disH);

        // Check vertical lines
        dof = 0;
        float disV = numeric_limits<float>::max();
        float vx = p_x;
        float vy = p_y;

        checkVertical(mx, my, mp, dof, rx, ry, ra, x_off, y_off, vx, vy, disV);

        if (disV < disH) {
            rx = vx;
            ry = vy;
            distT = disV;
            toColour(
                colourMask<GLdouble>(
                    gameMap.getAt(radToCoord(rx), radToCoord(ry)).getColour(),
                    {0.9, 0.9, 0.9, 1.0},
                    PW_mul<GLdouble>));
        } else if (disH < disV) {
            rx = hx;
            ry = hy;
            distT = disH;
            toColour(
                colourMask<GLdouble>(
                    gameMap.getAt(radToCoord(rx), radToCoord(ry)).getColour(),
                    {0.7, 0.7, 0.7, 1.0},
                    PW_mul<GLdouble>));
        }

        if (render2DMap && renderRays) {
            renderRay(p_x, p_y, rx, ry, 1);
        }

        draw3DWalls(r, ra, distT);

        ra += DR;
        if (ra < 0) {
            ra += (float)(2 * M_PI);
        } else if (ra > 2 * M_PI) {
            ra -= (float)(2 * M_PI);
        }
    }
}

///
/// Render the scene
///
/// @return void
///
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // if (render2DMap) {
    //     drawMap2D();
    //     drawPlayer();
    // }
    // renderRays2Dto3D();
    renderRaysMap();
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
        p_a -= 0.1f;
        if (p_a < 0) {
            p_a += (float)(2 * M_PI);
        }
        p_dx = cos(p_a) * 5;
        p_dy = sin(p_a) * 5;
    }
    if (key == 'd') {
        // Turn left
        p_a += 0.1f;
        if (p_a > 2 * M_PI) {
            p_a -= (float)(2 * M_PI);
        }
        p_dx = cos(p_a) * 5;
        p_dy = sin(p_a) * 5;
    }
    if (key == 'w') {
        // Move forward
        p_x += p_dx;
        p_y += p_dy;
    }
    if (key == 's') {
        // Move backward
        p_x -= p_dx;
        p_y -= p_dy;
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
    // throw MemoryError("test");
    gameMap.readMapFromFile("src/world_files/map1.txt");
    textures.at(0) = Texture("src/textures/wall-texture.bmp", "wall", 1024, 1024);
    glClearColor(
        (float)get<RED_IDX>(background_colour),
        (float)get<BLUE_IDX>(background_colour),
        (float)get<GREEN_IDX>(background_colour),
        (float)get<ALPHA_IDX>(background_colour));
    gluOrtho2D(0, screenW, screenH, 0);
    p_x = 250;
    p_y = 250;
    p_dx = cos(p_a) * 5;
    p_dy = sin(p_a) * 5;
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
    // moveCoords();

    glutDisplayFunc(display);
    glutKeyboardFunc(buttons);
    glutPostRedisplay();
    glutMainLoop();
    return 0;
}
