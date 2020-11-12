#define GL_SILENCE_DEPRECATION

#include "Raytracer.hpp"

#define SPRITE_U_DIV 1
#define SPRITE_V_DIV 1
#define SPRITE_V_MOVE 0.0

#define DARK_SHADER 0x7F7F7F

#define __DEFAULT_DISPLAY_FUNC [](){}

#define __DEFAULT_SCREEN_WIDTH 1024
#define __DEFAULT_SCREEN_HEIGHT 512

using namespace std;

// Screen
int screen_width = __DEFAULT_SCREEN_WIDTH;
int screen_height = __DEFAULT_SCREEN_HEIGHT;

ResourceManager::TextureLoader texLoader;
HashTable<Texture> textures;
AStar astar;
vector<Coords> *path = new vector<Coords>();
// Player
Player player;

// Configs
ResourceManager::ConfigInit cfgInit;

World world = World();

double new_time = 0;
double old_time = 0;

double frame_time = 0;

Rendering::PBO pixelBuffer;
Rendering::RayBuffer rays;
Rendering::ZBuffer zBuf;

GUI::Canvas canvas;

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

        floor_x = player.location.x + dist * ray_dir_x0;
        floor_y = player.location.y + dist * ray_dir_y0;

        for (int x = screen_width - 1; x >= 0; --x) {
            cell_x = (int)(floor_x);
            cell_y = (int)(floor_y);

            tex_coord_x = (int)(renderCfg.texture_width * (floor_x - cell_x)) & (renderCfg.texture_width - 1);
            tex_coord_y = (int)(renderCfg.texture_height * (floor_y - cell_y)) & (renderCfg.texture_height - 1);

            floor_x += step_x;
            floor_y += step_y;

            textures.get(world.floor_texture, tex);
            color = tex.texture[renderCfg.texture_width * tex_coord_y + tex_coord_x];
            color = (color >> 1) & DARK_SHADER;
            pixelBuffer.pushToBuffer(screen_width - x, screen_height - y, Colour::INTtoRGB(color));

            textures.get(world.ceiling_texture, tex);
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

        map_x = (int)player.location.x;
        map_y = (int)player.location.y;

        delta_x = abs(1 / ray_dir_x);
        delta_y = abs(1 / ray_dir_y);

        hit = 0;
        step_x = copysign(1.0, ray_dir_x);
        side_dist_x = delta_x * (ray_dir_x < 0 ? player.location.x - map_x : map_x + 1.0 - player.location.x);
        step_y = copysign(1.0, ray_dir_y);
        side_dist_y = delta_y * (ray_dir_y < 0 ? player.location.y - map_y : map_y + 1.0 - player.location.y);

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
            hit = world.getAt(map_x, map_y).wf_left.texture != "";
        }

        if (side == 0) {
            perp_wall_dist = (map_x - player.location.x + IDIV_2((1 - step_x))) / ray_dir_x;
        } else {
            perp_wall_dist = (map_y - player.location.y + IDIV_2((1 - step_y))) / ray_dir_y;
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
        wall_tex = world.getAt(map_x, map_y).wf_left.texture;

        wall_x = side == 0 ? player.location.y + perp_wall_dist * ray_dir_y : player.location.x + perp_wall_dist * ray_dir_x;
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

inline static void renderSprites() {
    world.sortSprites(player.location);

    double sprite_x, sprite_y, transform_x, transform_y;
    int sprite_screen_x, vert_move_screen, sprite_height, sprite_width, draw_start_pos_y, draw_end_pos_y, draw_start_pos_x, draw_end_pos_x, tex_coord_x, tex_coord_y, d;
    Texture tex;
    uint32_t color;
    double inverse_det = 1.0 / (player.camera.clip_plane_x * player.camera.frustrum.getFovY() - player.camera.frustrum.getFovX() * player.camera.clip_plane_y);
    for (int i = world.sprites.size() - 1; i >= 0; i--) {
        sprite_x = world.sprites[i].location.x - player.location.x;
        sprite_y = world.sprites[i].location.y - player.location.y;

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
        textures.get(world.sprites[i].texture, tex);
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

    global_tick++;
    world.updateSprites();
    player.update();
}

static void display(void) {
    if (renderCfg.render_floor_ceiling) {
        renderFloorCeiling();
    }
    if (renderCfg.render_walls) {
        renderWalls();
    }
    if (renderCfg.render_sprites) {
        renderSprites();
    }

    pixelBuffer.swapBuffer();
    updateTimeTick();
    
    // canvas.getMinimap().render(screen_width, screen_height);
    canvas.render(frame_time);
    astar.renderPath(
        path, Colour::RGB_Blue,
        screen_width, screen_height,
        canvas.getMinimap().getScalingX(), canvas.getMinimap().getScalingY()
    );
    // canvas.getDebugOverlay().render(frame_time);
    // canvas.getStatsBar().render(screen_width, screen_height);
    glutSwapBuffers();
}

static void __KEY_HANDLER(unsigned char key, int x, int y) {
    player.handleKeyPress(key, x, y, world);
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

    world.readMapFromJSON(MAPS_DIR + "map2.json");

    rays = Rendering::RayBuffer(playerCfg.fov);
    zBuf = Rendering::ZBuffer(screen_width);

    astar = AStar(world);
    path = astar.find(world.start, world.end);

    player.moveSpeed = frame_time * playerCfg.move_speed;
    player.rotSpeed = frame_time * playerCfg.rotation_speed;

    gluOrtho2D(0, screen_width, screen_height, 0);
    player = Player(
        world.start.x,
        world.start.y,
        -playerCfg.fov,
        0.0,
        0);
    debugContext.logAppInfo("Initialised Player object [" + to_string(player.id) + "] at: " + ADDR_OF(player));

    canvas.setMinimap(GUI::Minimap(&player, &world, screen_width, screen_height));
    debugContext.logAppInfo("Initialised Minimap object at: " + ADDR_OF(canvas.getMinimap()));

    canvas.setDebugOverlay(GUI::DebugOverlay(&player, &canvas.getMinimap(), &world, GLUT_BITMAP_HELVETICA_18));
    debugContext.logAppInfo("Initialised DebugOverlay object at: " + ADDR_OF(canvas.getDebugOverlay()));

    canvas.setStatsBar(GUI::StatsBar(screen_width, screen_height,
        Colour::RGB_Blue, Colour::RGB_Navy, Colour::RGB_White));
    debugContext.logAppInfo("Initialised StatsBar object at: " + ADDR_OF(canvas.getStatsBar()));

    canvas.addButton(GUI::Button(100, 100, 100, 20, "Button",
        Colour::RGB_Blue, Colour::RGB_Red, Colour::RGB_Green,
    [](int_id id){
        cout << "Button " << id << " pressed" << endl;
    }));

    pixelBuffer = Rendering::PBO(screen_width, screen_height);
    pixelBuffer.init();

    global_tick = 0;
    debugContext.logAppInfo("Initialised global tick");
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

static void __MOUSE_HANDLER(int button, int state, int x, int y) {
    canvas.handleMouse(button, state, x, y);
}

static void __ACTIVE_MOTION_HANDLER(int x, int y) {
    canvas.handleMouse(GLUT_LEFT_BUTTON, GLUT_UP, x, y);
}

static void __PASSIVE_MOTION_HANDLER(int x, int y) {
    canvas.handleMouse(GLUT_LEFT_BUTTON, GLUT_UP, x, y);
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

    if (renderCfg.headless_mode) {
        debugContext.logAppInfo("Running in headless mode, glutDisplayFun not initialised");
        glutDisplayFunc(__DEFAULT_DISPLAY_FUNC);
    } else {
        glutDisplayFunc(display);
        debugContext.logApiInfo("Initialised glutDisplayFunc [display] at: " + ADDR_OF(display));
    }
    glutReshapeFunc(__WINDOW_RESHAPE);
    debugContext.logApiInfo("Initialised glutReshapeFunc [__WINDOW_RESHAPE] at: " + ADDR_OF(__WINDOW_RESHAPE));
    glutKeyboardFunc(__KEY_HANDLER);
    debugContext.logApiInfo("Initialised glutKeyboardFunc [__KEY_HANDLER] at: " + ADDR_OF(__KEY_HANDLER));
    glutMouseFunc(__MOUSE_HANDLER);
    debugContext.logApiInfo("Initialised glutMouseFunc [__MOUSE_HANDLER] at: " + ADDR_OF(__MOUSE_HANDLER));
    glutMotionFunc(__ACTIVE_MOTION_HANDLER);
    debugContext.logApiInfo("Initialised glutMouseFunc [__ACTIVE_MOTION_HANDLER] at: " + ADDR_OF(__ACTIVE_MOTION_HANDLER));
    glutPassiveMotionFunc(__PASSIVE_MOTION_HANDLER);
    debugContext.logApiInfo("Initialised glutMouseFunc [__PASSIVE_MOTION_HANDLER] at: " + ADDR_OF(__PASSIVE_MOTION_HANDLER));
    glutIdleFunc(__GLUT_IDLE);
    debugContext.logApiInfo("Initialised glutIdleFunc [__GLUT_IDLE] at: " + ADDR_OF(__GLUT_IDLE));
    glutPostRedisplay();
    debugContext.logApiInfo("---- COMPLETED OpenGL/GLUT INIT PHASE ----");
    debugContext.logApiInfo("Started glutMainLoop()");
    glutPostRedisplay();
    glutMainLoop();
    return 0;
}