#ifndef CC_WORLD_H
#define CC_WORLD_H
#include "Vectors.h"
#include "PackedCol.h"
/* Represents a fixed size 3D array of blocks.
   Also contains associated environment metadata.
   Copyright 2014-2017 ClassicalSharp | Licensed under BSD-3
*/
struct AABB;

/* Unpacka an index into x,y,z (slow!) */
#define World_Unpack(idx, x, y, z) x = idx % World.Width; z = (idx / World.Width) % World.Length; y = (idx / World.Width) / World.Length;
/* Packs an x,y,z into a single index */
#define World_Pack(x, y, z) (((y) * World.Length + (z)) * World.Width + (x))

CC_VAR extern struct _WorldData {
	/* The blocks in the world. */
	BlockRaw* Blocks;
#ifdef EXTENDED_BLOCKS
	/* The upper 8 bit of blocks in the world. */
	/* If only 8 bit blocks are used, equals World_Blocks. */
	BlockRaw* Blocks2;
#endif
	/* Volume of the world. */
	int Volume;

	/* Dimensions of the world. */
	int Width, Height, Length;
	/* Maximum X/Y/Z coordinate in the world. */
	/* (i.e. Width - 1, Height - 1, Length - 1) */
	int MaxX, MaxY, MaxZ;
	/* Adds one Y coordinate to a packed index. */
	int OneY;
	/* Unique identifier for this world. */
	uint8_t Uuid[16];
} World;
extern String World_TextureUrl;

/* Frees the blocks array, sets dimensions to 0, resets environment to default. */
CC_API void World_Reset(void);
/* Sets the blocks array and dimensions of the map. */
/* May also sets some environment settings like border/clouds height, if they are -1 */
CC_API void World_SetNewMap(BlockRaw* blocks, int width, int height, int length);
/* Sets the various dimension and max coordinate related variables. */
/* NOTE: This is an internal API. Use World_SetNewMap instead. */
CC_NOINLINE void World_SetDimensions(int width, int height, int length);

#ifdef EXTENDED_BLOCKS
extern int Block_IDMask;
/* Sets World.Blocks2 and updates internal state for more than 256 blocks. */
void World_SetMapUpper(BlockRaw* blocks);

/* Gets the block at the given coordinates. */
/* NOTE: Does NOT check that the coordinates are inside the map. */
static CC_INLINE BlockID World_GetBlock(int x, int y, int z) {
	int i = World_Pack(x, y, z);
	return (BlockID)((World.Blocks[i] | (World.Blocks2[i] << 8)) & Block_IDMask);
}
#else
#define World_GetBlock(x, y, z) World_Blocks[World_Pack(x, y, z)]
#endif

/* If Y is above the map, returns BLOCK_AIR. */
/* If coordinates are outside the map, returns BLOCK_AIR. */
/* Otherwise returns the block at the given coordinates. */
BlockID World_GetPhysicsBlock(int x, int y, int z);
/* Sets the block at the given coordinates. */
/* NOTE: Does NOT check that the coordinates are inside the map. */
void World_SetBlock(int x, int y, int z, BlockID block);
/* If coordinates are outside the map, returns BLOCK_AIR. */
/* Otherwise returns the block at the given coordinates. */
BlockID World_SafeGetBlock_3I(Vector3I p);

/* Whether the given coordinates lie inside the map. */
static CC_INLINE bool World_Contains(int x, int y, int z) {
	return (unsigned)x < (unsigned)World.Width
		&& (unsigned)y < (unsigned)World.Height
		&& (unsigned)z < (unsigned)World.Length;
}
/* Whether the given coordinates lie horizontally inside the map. */
static CC_INLINE bool World_ContainsXZ(int x, int z) {
	return (unsigned)x < (unsigned)World.Width 
		&& (unsigned)z < (unsigned)World.Length;
}

enum EnvVar_ {
	ENV_VAR_EDGE_BLOCK, ENV_VAR_SIDES_BLOCK, ENV_VAR_EDGE_HEIGHT, ENV_VAR_SIDES_OFFSET,
	ENV_VAR_CLOUDS_HEIGHT, ENV_VAR_CLOUDS_SPEED, ENV_VAR_WEATHER_SPEED, ENV_VAR_WEATHER_FADE,
	ENV_VAR_WEATHER, ENV_VAR_EXP_FOG, ENV_VAR_SKYBOX_HOR_SPEED, ENV_VAR_SKYBOX_VER_SPEED,
	ENV_VAR_SKY_COL, ENV_VAR_CLOUDS_COL, ENV_VAR_FOG_COL, ENV_VAR_SUN_COL, ENV_VAR_SHADOW_COL
};

extern BlockID Env_EdgeBlock, Env_SidesBlock;
extern int Env_EdgeHeight;
extern int Env_SidesOffset;
#define Env_SidesHeight (Env_EdgeHeight + Env_SidesOffset)
extern int Env_CloudsHeight;
extern float Env_CloudsSpeed;

enum Weather_ { WEATHER_SUNNY, WEATHER_RAINY, WEATHER_SNOWY };
extern const char* Weather_Names[3];
extern float Env_WeatherSpeed;
extern float Env_WeatherFade;
extern int Env_Weather;
extern bool Env_ExpFog;
extern float Env_SkyboxHorSpeed, Env_SkyboxVerSpeed;

extern PackedCol Env_SkyCol, Env_FogCol, Env_CloudsCol;
extern PackedCol Env_DefaultSkyCol, Env_DefaultFogCol, Env_DefaultCloudsCol;
extern PackedCol Env_SunCol,    Env_SunXSide,    Env_SunZSide,    Env_SunYMin;
extern PackedCol Env_ShadowCol, Env_ShadowXSide, Env_ShadowZSide, Env_ShadowYMin;
extern PackedCol Env_DefaultSunCol, Env_DefaultShadowCol;

#define ENV_DEFAULT_SKYCOL_HEX "99CCFF"
#define ENV_DEFAULT_FOGCOL_HEX "FFFFFF"
#define ENV_DEFAULT_CLOUDSCOL_HEX "FFFFFF"
#define ENV_DEFAULT_SUNCOL_HEX "FFFFFF"
#define ENV_DEFAULT_SHADOWCOL_HEX "9B9B9B"

/* Resets all environment settings to default. */
/* NOTE: Unlike Env_Set functions, DOES NOT raise EnvVarChanged event. */
CC_API void Env_Reset(void);
/* Sets the edge/horizon block. (default water) */
CC_API void Env_SetEdgeBlock(BlockID block);
/* Sets the sides/border block. (default bedrock) */
CC_API void Env_SetSidesBlock(BlockID block);
/* Sets the edge/horizon height. (default height/2) */
CC_API void Env_SetEdgeHeight(int height);
/* Sets offset of sides/border from horizon. (default -2) */
CC_API void Env_SetSidesOffset(int offset);
/* Sets clouds height. (default height+2)*/
CC_API void Env_SetCloudsHeight(int height);
/* Sets how fast clouds move. (default 1) */
/* Negative speeds move in opposite direction. */
CC_API void Env_SetCloudsSpeed(float speed);

/* Sets how fast rain/snow falls. (default 1) */
/* Negative speeds makes rain/snow fall upwards. */
CC_API void Env_SetWeatherSpeed(float speed);
/* Sets how quickly rain/snow fades over distance. (default 1) */
CC_API void Env_SetWeatherFade(float rate);
/* Sets the weather of the map. (default sun) */
/* Can be sun/rain/snow, see WEATHER_ enum. */
CC_API void Env_SetWeather(int weather);
/* Sets whether exponential/smooth fog is used. (default false) */
CC_API void Env_SetExpFog(bool expFog);
/* Sets how quickly skybox rotates/spins horizontally. (default 0) */
/* speed is in rotations/second, so '2' completes two full spins per second. */
CC_API void Env_SetSkyboxHorSpeed(float speed);
/* Sets how quickly skybox rotates/spins vertically. (default 0) */
/* speed is in rotations/second, so '2' completes two full spins per second. */
CC_API void Env_SetSkyboxVerSpeed(float speed);

/* Sets colour of the sky above clouds. (default #99CCFF) */
CC_API void Env_SetSkyCol(PackedCol col);
/* Sets base colour of the horizon fog. (default #FFFFFF) */
/* Actual fog colour is blended between sky and fog colours, based on view distance. */
CC_API void Env_SetFogCol(PackedCol col);
/* Sets colour of the clouds and skybox. (default #FFFFFF) */
CC_API void Env_SetCloudsCol(PackedCol col);
/* Sets colour of sunlight. (default #FFFFFF) */
/* This is the colour used for lighting when not underground. */
CC_API void Env_SetSunCol(PackedCol col);
/* Sets colour of shadow. (default #9B9B9B) */
/* This is the colour used for lighting when underground. */
CC_API void Env_SetShadowCol(PackedCol col);

#define RESPAWN_NOT_FOUND -100000.0f
/* Finds the highest Y coordinate of any solid block that intersects the given bounding box */
/* So essentially, means max(Y + Block_MaxBB[block].Y) over all solid blocks the AABB touches */
/* Returns RESPAWN_NOT_FOUND when no intersecting solid blocks are found. */
float Respawn_HighestSolidY(struct AABB* bb);
/* Finds a suitable initial spawn position for the entity. */
/* Works by iterating downwards from top of world until solid ground is found. */
Vector3 Respawn_FindSpawnPosition(float x, float z, Vector3 modelSize);
#endif
