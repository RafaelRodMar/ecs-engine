#include "Map.h"
//#include "AssetsManager.h"
#include "game.h"
#include <fstream>

Map::Map() {
}

Map::~Map() {
}

void Map::loadMap(std::string path, int sizeX, int sizeY) {
	char tile;
	std::fstream mapFile;
	mapFile.open(path);

	for (int y = 0; y < sizeY; y++) {
		for (int x = 0; x < sizeX; x++) {
			mapFile.get(tile);
			Game::AddTile(atoi(&tile), x * 32, y * 32);
			mapFile.ignore();
		}
	}

	mapFile.close();
}

//void Map::loadMap(int arr[20][25]) {
//	for (int row = 0; row < 20; row++) {
//		for (int col = 0; col < 25; col++) {
//			map[row][col] = arr[row][col];
//		}
//	}
//}
//
//void Map::drawMap() {
//	int type = 0;
//	for (int row = 0; row < 20; row++) {
//		for (int col = 0; col < 25; col++) {
//			type = map[row][col];
//
//			switch (type) {
//			case 0:
//				AssetsManager::Instance()->drawTile("water", 0, 0, col * 32, row * 32, 32, 32, 0, 0, Game::Instance()->getRenderer());
//				break;
//			case 1:
//				AssetsManager::Instance()->drawTile("dirt", 0, 0, col * 32, row * 32, 32, 32, 0, 0, Game::Instance()->getRenderer());
//				break;
//			case 2:
//				AssetsManager::Instance()->drawTile("grass", 0, 0, col * 32, row * 32, 32, 32, 0, 0, Game::Instance()->getRenderer());
//				break;
//			default:
//				break;
//			}
//		}
//	}
//}
