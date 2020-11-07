#define GL_SILENCE_DEPRECATION

#include "Raytracer.hpp"

#define SPRITE_U_DIV 1
#define SPRITE_V_DIV 1
#define SPRITE_V_MOVE 0.0

#define DARK_SHADER 8355711

using namespace std;

// Screen
int screen_width = 1024;
int screen_height = 512;

ResourceManager::TextureLoader texLoader;
HashTable<Texture> textures;
AStar astar;
vector<Coords> *path = new vector<Coords>();
// Player
Player player;

// Configs
ResourceManager::ConfigInit cfgInit;

GameMap gameMap = GameMap();
Minimap minimap;
DebugOverlay debugOverlay;
StatsBar statsBar;

// vector<int> spriteOrder;
// vector<double> spriteDistance;

double new_time = 0;
double old_time = 0;

double frame_time = 0;

Rendering::PBO pixelBuffer;
Rendering::RayBuffer rays;
Rendering::ZBuffer zBuf;

inline static void renderFloorCeiling() {
    float ray_dir_x0, ray_dir_y0, ray_dir_x1, ray_dir_y1, pos_z, dist, step_x, step_y, floor_x, floor_y;
    int p, cell_x, cell_y, tex_coord_x, tex_coord_y;
    Texture tex;
    uint32_t color;
    for (int y = screen_height - 1; y >= IDIV_2(screen_height) + 1; --y) {
        ray_dir_x0 = player.camera.frustrum.getFovX() - player.camera.clip_plane_x;
        ray_dir_y0 = player.camera.frustrum.getFovY() - player.camera.clip_plane_y;
        ray_dir_x1 = player.camera.frustrum.getFovX() + player.camera.clip_plane_x;
        ray_dir_y1 = player.camera.frustrum.getFovY() + player.camera.clip_plane_y;

        p = y - IDIV_2(screen_height);

        pos_z = 0.5 * screen_height;

        dist = pos_z / p;

        step_x = dist * (ray_dir_x1 - ray_dir_x0) / screen_width;
        step_y = dist * (ray_dir_y1 - ray_dir_y0) / screen_width;

        floor_x = player.x + dist * ray_dir_x0;
        floor_y = player.y + dist * ray_dir_y0;

        for (int x = screen_width - 1; x >= 0; --x) {
            cell_x = (int)(floor_x);
            cell_y = (int)(floor_y);

            tex_coord_x = (int)(renderCfg.texture_width * (floor_x - cell_x)) & (renderCfg.texture_width - 1);
            tex_coord_y = (int)(renderCfg.texture_height * (floor_y - cell_y)) & (renderCfg.texture_height - 1);

            floor_x += step_x;
            floor_y += step_y;

            textures.get(gameMap.floor_texture, tex);
            color = tex.texture[renderCfg.texture_width * tex_coord_y + tex_coord_x];
            color = (color >> 1) & DARK_SHADER;
            pixelBuffer.pushToBuffer(screen_width - x, screen_height - y, Colour::INTtoRGB(color));

            textures.get(gameMap.ceiling_texture, tex);
            color = tex.texture[renderCfg.texture_width * tex_coord_y + tex_coord_x];
            color = (color >> 1) & DARK_SHADER;
            pixelBuffer.pushToBuffer(screen_width - x, y - 1, Colour::INTtoRGB(color));
        }
    }
}

inline static void renderWalls() {
    double ray_dir_x, ray_dir_y, side_dist_x, side_dist_y, delta_x, delta_y, perp_wall_dist, wall_x, step, tex_pos;
    int map_x, map_y, step_x, step_y, hit, side, line_height, draw_start_pos, draw_end_pos, tex_coord_x, tex_coord_y;
    string wall_tex;
    uint32_t color;
    Texture tex;
    for (int x = screen_width - 1; x >= 0; x--) {
        player.camera.x = 2 * x / double(screen_width) - 1;
        ray_dir_x = player.camera.frustrum.getFovX() + player.camera.clip_plane_x * player.camera.x;
        ray_dir_y = player.camera.frustrum.getFovY() + player.camera.clip_plane_y * player.camera.x;

        map_x = (int)player.x;
        map_y = (int)player.y;

        delta_x = abs(1 / ray_dir_x);
        delta_y = abs(1 / ray_dir_y);

        hit = 0;
        step_x = copysign(1.0, ray_dir_x);
        side_dist_x = delta_x * (ray_dir_x < 0 ? player.x - map_x : map_x + 1.0 - player.x);
        step_y = copysign(1.0, ray_dir_y);
        side_dist_y = delta_y * (ray_dir_y < 0 ? player.y - map_y : map_y + 1.0 - player.y);

        while (hit == 0) {
            if (side_dist_x < side_dist_y) {
                side_dist_x += delta_x;
                map_x += step_x;
                side = 0;
            } else {
                side_dist_y += delta_y;
                map_y += step_y;
                side = 1;
            }
            hit = gameMap.getAt(map_x, map_y).wf_left.texture != "";
        }

        if (side == 0) {
            perp_wall_dist = (map_x - player.x + IDIV_2((1 - step_x))) / ray_dir_x;
        } else {
            perp_wall_dist = (map_y - player.y + IDIV_2((1 - step_y))) / ray_dir_y;
        }
        line_height = (int)(screen_height / perp_wall_dist);

        draw_start_pos = IDIV_2(-line_height) + IDIV_2(screen_height);
        if (draw_start_pos < 0) {
            draw_start_pos = 0;
        }
        draw_end_pos = IDIV_2(line_height) + IDIV_2(screen_height);
        if (draw_end_pos >= screen_height) {
            draw_end_pos = screen_height - 1;
        }
        wall_tex = gameMap.getAt(map_x, map_y).wf_left.texture;

        wall_x = side == 0 ? player.y + perp_wall_dist * ray_dir_y : player.x + perp_wall_dist * ray_dir_x;
        wall_x -= floor((wall_x));

        tex_coord_x = (int)(wall_x * double(renderCfg.texture_width));
        if (side == 0 && ray_dir_x > 0) {
            tex_coord_x = renderCfg.texture_width - tex_coord_x - 1;
        }
        if (side == 1 && ray_dir_y < 0) {
            tex_coord_x = renderCfg.texture_width - tex_coord_x - 1;
        }

        step = 1.0 * renderCfg.texture_height / line_height;
        tex_pos = (draw_start_pos - IDIV_2(screen_height) + IDIV_2(line_height)) * step;
        for (int y = draw_end_pos - 1; y >= draw_start_pos; y--) {
            tex_coord_y = (int)tex_pos & (renderCfg.texture_height - 1);
            tex_pos += step;
            textures.get(wall_tex, tex);
            color = tex.texture[renderCfg.texture_height * tex_coord_y + tex_coord_x];
            if (side == 1) {
                color = (color >> 1) & DARK_SHADER;
            }
            pixelBuffer.pushToBuffer(x, y, Colour::INTtoRGB(color));
        }

        zBuf[x] = perp_wall_dist;
    }
}

inline double sqDist(double ax, double ay, double bx, double by) {
    return pow(bx - ax, 2) + pow(by - ay, 2);
}

inline void sortSprites() {
    accumulate(gameMap.sprites.begin(), gameMap.sprites.end(), 0, [](int i, Constructs::Sprite& sprite) -> int {
        sprite.distance = sqDist(sprite.location.x, player.x, sprite.location.y, player.y);
        sprite.order = i;
        return i++;
    });
    sort(gameMap.sprites.begin(), gameMap.sprites.end());
}

inline static void renderSprites() {
    sortSprites();

    double sprite_x, sprite_y, transform_x, transform_y;
    int sprite_screen_x, vert_move_screen, sprite_height, sprite_width, draw_start_pos_y, draw_end_pos_y, draw_start_pos_x, draw_end_pos_x, tex_coord_x, tex_coord_y, d;
    Texture tex;
    uint32_t color;
    double inverse_det = 1.0 / (player.camera.clip_plane_x * player.camera.frustrum.getFovY() - player.camera.frustrum.getFovX() * player.camera.clip_plane_y);
    for (int i = gameMap.sprites.size() - 1; i >= 0; i--) {
        sprite_x = gameMap.sprites[i].location.x - player.x;
        sprite_y = gameMap.sprites[i].location.y - player.y;

        transform_x = inverse_det * (player.camera.frustrum.getFovY() * sprite_x - player.camera.frustrum.getFovX() * sprite_y);
        transform_y = inverse_det * (-player.camera.clip_plane_y * sprite_x + player.camera.clip_plane_x * sprite_y);

        sprite_screen_x = (int)(IDIV_2(screen_width) * (1 + transform_x / transform_y));

        vert_move_screen = (int)(SPRITE_V_MOVE / transform_y);

        sprite_height = abs((int)(screen_height / (transform_y))) / SPRITE_V_DIV;
        draw_start_pos_y = -IDIV_2(sprite_height) + IDIV_2(screen_height) + vert_move_screen;
        if (draw_start_pos_y < 0) {
            draw_start_pos_y = 0;
        }
        draw_end_pos_y = IDIV_2(sprite_height) + IDIV_2(screen_height) + vert_move_screen;
        if (draw_end_pos_y >= screen_height) {
            draw_end_pos_y = screen_height - 1;
        }

        sprite_width = abs((int)(screen_height / (transform_y))) / SPRITE_U_DIV;
        draw_start_pos_x = IDIV_2(-sprite_width) + sprite_screen_x;
        if (draw_start_pos_x < 0) {
            draw_start_pos_x = 0;
        }
        draw_end_pos_x = IDIV_2(sprite_width) + sprite_screen_x;
        if (draw_end_pos_x >= screen_width) {
            draw_end_pos_x = screen_width - 1;
        }
        textures.get(gameMap.sprites[i].texture, tex);
        for (int pixel_row = draw_end_pos_x - 1; pixel_row >= draw_start_pos_x; pixel_row--) {
            tex_coord_x = (int)IDIV_256((IMUL_256((pixel_row - (IDIV_2(-sprite_width) + sprite_screen_x))) * renderCfg.texture_width / sprite_width));
            if (!(transform_y > 0 && pixel_row > 0 && pixel_row < screen_width && transform_y < zBuf[pixel_row])) {
                continue;
            }
            for (int pixel_column = draw_end_pos_y - 1; pixel_column >= draw_start_pos_y; pixel_column--) {
                d = IMUL_256((pixel_column - vert_move_screen)) - IMUL_128(screen_height) + IMUL_128(sprite_height);
                tex_coord_y = IDIV_256((d * renderCfg.texture_height) / sprite_height);
                color = tex.texture[renderCfg.texture_width * tex_coord_y + tex_coord_x];
                if ((color & 0x00FFFFFF) != 0) {
                    pixelBuffer.pushToBuffer(pixel_row, screen_height - pixel_column, Colour::INTtoRGB(color));
                }
            }
        }
    }
}

inline static void updateTimeTick() {
    old_time = new_time;
    new_time = glutGet(GLUT_ELAPSED_TIME);
    frame_time = (new_time - old_time) / 1000.0;

    player.moveSpeed = frame_time * playerCfg.move_speed;
    player.rotSpeed = frame_time * playerCfg.rotation_speed;
}

static void display(void) {
    if (renderCfg.headless_mode) {
        return;
    }

    if (renderCfg.render_floor_ceiling) {
        renderFloorCeiling();
    }
    if (renderCfg.render_walls) {
        renderWalls();
    }
    if (renderCfg.render_sprites) {
        renderSprites();
    }

    pixelBuffer.pushBufferToGPU();
    updateTimeTick();
    
    minimap.render(screen_width, screen_height);
    astar.renderPath(
        path, Colour::RGB_Blue,
        screen_width, screen_height,
        minimap.getScalingX(), minimap.getScalingY()
    );
    debugOverlay.render(frame_time);
    statsBar.render(screen_width, screen_height);
    glutSwapBuffers();
}

static void __KEY_HANDLER(unsigned char key, int x, int y) {
    if (key == 'w') {
        if (gameMap.getAt((int)(player.x + player.camera.frustrum.getFovX() * player.moveSpeed), (int)player.y).wf_left.texture == "") {
            player.x += player.camera.frustrum.getFovX() * player.moveSpeed;
        }
        if (gameMap.getAt((int)player.x, (int)(player.y + player.camera.frustrum.getFovY() * player.moveSpeed)).wf_left.texture == "") {
            player.y += player.camera.frustrum.getFovY() * player.moveSpeed;
        }
    } else if (key == 's') {
        if (gameMap.getAt((int)(player.x - player.camera.frustrum.getFovX() * player.moveSpeed), (int)player.y).wf_left.texture == "") {
            player.x -= player.camera.frustrum.getFovX() * player.moveSpeed;
        }
        if (gameMap.getAt((int)player.x, (int)(player.y - player.camera.frustrum.getFovY() * player.moveSpeed)).wf_left.texture == "") {
            player.y -= player.camera.frustrum.getFovY() * player.moveSpeed;
        }
    } else if (key == 'd') {
        double old_dir_x = player.camera.frustrum.getFovX();
        player.camera.frustrum.setFovX(player.camera.frustrum.getFovX() * cos(-player.rotSpeed) - player.camera.frustrum.getFovY() * sin(-player.rotSpeed));
        player.camera.frustrum.setFovY(old_dir_x * sin(-player.rotSpeed) + player.camera.frustrum.getFovY() * cos(-player.rotSpeed));

        double old_plane_x = player.camera.clip_plane_x;
        player.camera.clip_plane_x = player.camera.clip_plane_x * cos(-player.rotSpeed) - player.camera.clip_plane_y * sin(-player.rotSpeed);
        player.camera.clip_plane_y = old_plane_x * sin(-player.rotSpeed) + player.camera.clip_plane_y * cos(-player.rotSpeed);
    } else if (key == 'a') {
        double old_dir_x = player.camera.frustrum.getFovX();
        player.camera.frustrum.setFovX(player.camera.frustrum.getFovX() * cos(player.rotSpeed) - player.camera.frustrum.getFovY() * sin(player.rotSpeed));
        player.camera.frustrum.setFovY(old_dir_x * sin(player.rotSpeed) + player.camera.frustrum.getFovY() * cos(player.rotSpeed));

        double old_plane_x = player.camera.clip_plane_x;
        player.camera.clip_plane_x = player.camera.clip_plane_x * cos(player.rotSpeed) - player.camera.clip_plane_y * sin(player.rotSpeed);
        player.camera.clip_plane_y = old_plane_x * sin(player.rotSpeed) + player.camera.clip_plane_y * cos(player.rotSpeed);
    }
    glutPostRedisplay();
}

///
/// Initialise the display rendering and player position
///
/// @return void
///
void __INIT() {
    cfgInit.initAll(playerCfg, minimapCfg, loggingCfg, renderCfg);

    debugContext = GLDebugContext(&loggingCfg);
    debugContext.logAppInfo("Loaded debug context");

    texLoader = ResourceManager::TextureLoader();
    texLoader.loadTextures(textures);
    debugContext.logAppInfo(string("Loaded " + to_string(textures.size()) + " textures"));

    gameMap.readMapFromJSON(MAPS_DIR + "map2.json");

    rays = Rendering::RayBuffer(playerCfg.fov);
    zBuf = Rendering::ZBuffer(screen_width);

    astar = AStar(gameMap);
    path = astar.find(gameMap.start, gameMap.end);

    player.moveSpeed = frame_time * playerCfg.move_speed;
    player.rotSpeed = frame_time * playerCfg.rotation_speed;

    // toClearColour(bg_colour);
    gluOrtho2D(0, screen_width, screen_height, 0);
    player = Player(
        gameMap.start.x,
        gameMap.start.y,
        1.0,
        0.0,
        0);
    debugContext.logAppInfo("Initialised Player object at: " + ADDR_OF(player));

    minimap = Minimap(&player, &gameMap, screen_width, screen_height);
    debugContext.logAppInfo("Initialised Minimap object at: " + ADDR_OF(minimap));

    debugOverlay = DebugOverlay(&player, &minimap, &gameMap, GLUT_BITMAP_HELVETICA_18);
    debugContext.logAppInfo("Initialised DebugOverlay object at: " + ADDR_OF(debugOverlay));

    statsBar = StatsBar(screen_width, screen_height,
        Colour::RGB_Blue, Colour::RGB_Navy, Colour::RGB_White);
    debugContext.logAppInfo("Initialised StatsBar object at: " + ADDR_OF(statsBar));

    pixelBuffer = Rendering::PBO(screen_width, screen_height);
    pixelBuffer.init();
}

static void __WINDOW_RESHAPE(int width, int height) {
    screen_width = width;
    screen_height = height;
    zBuf.resize(width);
    pixelBuffer.resize(width, height);
}

static void __GLUT_IDLE(void) {
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
int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(screen_width, screen_height);
    glutCreateWindow("Ray Caster");

    __INIT();
    debugContext.logAppInfo("---- COMPLETED APPLICATION INIT PHASE ----");

    glutDisplayFunc(display);
    debugContext.logApiInfo("Initialised glutDisplayFunc [display] at: " + ADDR_OF(display));
    glutReshapeFunc(__WINDOW_RESHAPE);
    debugContext.logApiInfo("Initialised glutReshapeFunc [__WINDOW_RESHAPE] at: " + ADDR_OF(__WINDOW_RESHAPE));
    glutKeyboardFunc(__KEY_HANDLER);
    debugContext.logApiInfo("Initialised glutKeyboardFunc [__KEY_HANDLER] at: " + ADDR_OF(__KEY_HANDLER));
    glutIdleFunc(__GLUT_IDLE);
    debugContext.logApiInfo("Initialised glutIdleFunc [__GLUT_IDLE] at: " + ADDR_OF(__GLUT_IDLE));
    glutPostRedisplay();
    debugContext.logApiInfo("---- COMPLETED OpenGL/GLUT INIT PHASE ----");
    debugContext.logApiInfo("Started glutMainLoop()");
    glutPostRedisplay();
    glutMainLoop();
    return 0;
}