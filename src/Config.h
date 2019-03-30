#pragma once

#define MOUSE_DELTA_JUMP        60.0f // the distance between the old mouse position and the new one is to big
#define MOUSE_MOVEMENT_SPEED    0.0033f
#define CAMERA_MOVEMENT_SPEED   1.35f
#define WORLD_ROTATION_SPEED    1.0f

#define PROCEDURAL_TERRAIN false

#define RANDOM_TERRAIN_LIGHT_GEN_MAX 150
#define RANDOM_TERRAIN_LIGHT_GEN_MIN -150

#define RANDOM_TERRAIN_TREE_GEN_MAX 200
#define RANDOM_TERRAIN_TREE_GEN_MIN -200
#define RANDOM_MAX_TRESS 40

#define WINDOW_WIDTH    1920
#define WINDOW_HEIGHT   1080

#define SHADOW_WIDTH 4096
#define SHADOW_HEIGHT 4096

#define M_PI           3.14159265358979323846

#define POWER_2_10 1024

#define BACKGROUND_RED      0.8f
#define BACKGROUND_GREEN    0.8f
#define BACKGROUND_BLUE     0.8f
#define BACKGROUND_ALFA     1.0f

#define TERRAIN_SIZE 800.0f

#define FOG_RED     0.5f
#define FOG_GREEN   0.5f
#define FOG_BLUE    0.5f


#define TERRAIN_TEXTURE_1       "Objects/terrain/grass.png"
#define TERRAIN_TEXTURE_2       "Objects/terrain/grassFlowers.png"
#define TERRAIN_TEXTURE_3       "Objects/terrain/mud.png"       
#define TERRAIN_TEXTURE_4       "Objects/terrain/path.png"
#define TERRAIN_BLEND_TEXTURE   "Objects/terrain/blendMap.png"

#define TERRAIN_TEXTURE_1_TYPE       "backgroundTexture"
#define TERRAIN_TEXTURE_2_TYPE       "textureG"
#define TERRAIN_TEXTURE_3_TYPE       "textureR"
#define TERRAIN_TEXTURE_4_TYPE       "textureB"
#define TERRAIN_BLEND_TEXTURE_TYPE   "blendMap"

#define TERRAIN_HEIGHT_MAP      "Objects/terrain/heightmap.png"
#define MAX_PIXEL_COLOR 256 * 256 * 256
#define MAX_HEIGHT 50.0f;

#define GRAVITY -10.0f // MAI VEZI

#define PLAYER_RUN_SPEED    20  // unitati pe secunda
#define PLAYER_TURN_SPEED   160 // grade pe secunda
#define PLAYER_JUMP_POWER   5  // unitati pe secunda
#define PLAYER_CENTER_HEIGHT 4.0f

#define MAX_LIGHT_SOURCES 6

#define NEAR_PLANE 0.1f
#define FAR_PLANE 1000.0F

#define MEMORY_ALLOC_ERROR "cannot allocate memory"


#define SKYBOX_DAY_RIGHT "Objects/Skybox/day/right.tga"
#define SKYBOX_DAY_LEFT "Objects/Skybox/day/left.tga"
#define SKYBOX_DAY_TOP "Objects/Skybox/day/top.tga"
#define SKYBOX_DAY_DOWN "Objects/Skybox/day/bottom.tga"
#define SKYBOX_DAY_BACK "Objects/Skybox/day/back.tga"
#define SKYBOX_DAY_FRONT "Objects/Skybox/day/front.tga"

#define SKYBOX_NIGHT_RIGHT "Objects/Skybox/night/oasisnight_rt.tga"
#define SKYBOX_NIGHT_LEFT "Objects/Skybox/night/oasisnight_lf.tga"
#define SKYBOX_NIGHT_TOP "Objects/Skybox/night/oasisnight_up.tga"
#define SKYBOX_NIGHT_DOWN "Objects/Skybox/night/oasisnight_dn.tga"
#define SKYBOX_NIGHT_BACK "Objects/Skybox/night/oasisnight_bk.tga"
#define SKYBOX_NIGHT_FRONT "Objects/Skybox/night/oasisnight_ft.tga"

#define SKY_DAY_NIGHT_SPEED 500
#define SKY_DAY_COLOR 1.0f
#define SKY_NIGHT_COLOR 0.3f
#define SKY_LIGHT_FACTOR 1.0f / 5.0f 
#define SKY_ROTATION_SPEED 0.3f


