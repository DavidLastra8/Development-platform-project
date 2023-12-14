#ifndef __MAP_H__
#define __MAP_H__

#include "Module.h"
#include "List.h"
#include "Point.h"

#include "DynArray.h"
#include "../Pathfinding.h"

#include "PugiXml\src\pugixml.hpp"

enum MapOrientation
{
	ORTOGRAPHIC = 0,
	ISOMETRIC
};

// Ignore Terrain Types and Tile Types for now, but we want the image!
struct TileSet
{
	SString	name;
	int	firstgid;
	int margin;
	int	spacing;
	int	tileWidth;
	int	tileHeight;
	int columns;
	int tilecount;

	SDL_Texture* texture;
	SDL_Rect GetRectangle(uint gid) {
		SDL_Rect rect = { 0 };

		int relativeIndex = gid - firstgid;
		rect.w = tileWidth;
		rect.h = tileHeight;
		rect.x = margin + (tileWidth + spacing) * (relativeIndex % columns);
		rect.y = margin + (tileHeight + spacing) * (relativeIndex / columns);

		return rect;
	}
};

//  We create an enum for map type, just for convenience,
// NOTE: Platformer game will be of type ORTHOGONAL
enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};

struct Properties
{
	struct Property
	{
		SString name;
		bool value;
	};
	
	List<Property*> list;

	~Properties()
	{
		//...
		ListItem<Property*>* item;
		item = list.start;

		while (item != NULL)
		{
			RELEASE(item->data);
			item = item->next;
		}

		list.Clear();
	}

	Property* GetProperty(const char* name);

	
};

struct MapLayer
{
	SString	name;
	int id; 
	int width;
	int height;
	uint* data;

	Properties properties;

	MapLayer() : data(NULL)
	{}

	~MapLayer()
	{
		RELEASE(data);
	}

	inline uint Get(int x, int y) const
	{
		return data[(y * width) + x];
	}
};

struct MapData
{
	int width;
	int	height;
	int	tileWidth;
	int	tileHeight;
	List<TileSet*> tilesets;
	MapTypes type;
	MapOrientation orientation;

	List<MapLayer*> maplayers;
};

class Map : public Module
{
public:

    Map();

    // Destructor
    virtual ~Map();

    // Called before render is available
    bool Awake(pugi::xml_node& conf);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

    // Called before quitting
    bool CleanUp();

    // Load new map
    bool Load();

	iPoint MapToWorld(int x, int y) const;
	iPoint Map::WorldToMap(int x, int y);

	// L13: Create navigation map for pathfinding
	void CreateNavigationMap(int& width, int& height, uchar** buffer) const;

	int GetTileWidth();
	int GetTileHeight();

private:

	bool LoadMap(pugi::xml_node mapFile);
	bool GetRect(int gid) const;
	bool LoadTileSet(pugi::xml_node mapFile);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadAllLayers(pugi::xml_node mapNode);
	TileSet* GetTilesetFromTileId(int gid) const;
	bool LoadProperties(pugi::xml_node& node, Properties& properties);

public: 

	SString name;
	SString path;
	PathFinding* pathfinding;
	MapData mapData;

private:

    SString mapFileName;
	SString mapFolder;
	MapLayer* navigationLayer;
    bool mapLoaded;
	int blockedGid = 49;
};

#endif // __MAP_H__