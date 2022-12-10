#pragma once
#include <string>

class Map
{
public:
	Map(std::string texture, int ms, int ts); //sprite sheet, mapscale, maptilesize
	~Map();

	void loadMap(std::string path, int sizeX, int sizeY);
	void AddTile(int srcX, int srcY, int xpos, int ypos);

private:
	std::string mapFilePath;
	int mapScale;
	int tileSize;
	int scaledSize;
};

