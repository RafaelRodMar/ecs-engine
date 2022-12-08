#pragma once

#include "ECS.h"
#include "sdl.h"

class TileComponent : public Component {
public:

	/*SDL_Texture* texture;*/
	std::string textureID;
	SDL_Rect srcRect, destRect;
	
	TileComponent() = default;

	~TileComponent() {
		/*SDL_DestroyTexture(texture);*/
	}

	TileComponent(int srcX, int srcY, int xpos, int ypos, std::string path) {
		textureID = path;

		srcRect.x = srcX;
		srcRect.y = srcY;
		srcRect.w = srcRect.h = 32;

		destRect.x = xpos;
		destRect.y = ypos;
		destRect.w = destRect.h = 64;
	}

	void draw() override {
		AssetsManager::Instance()->drawTileScl(textureID, 0, 0, destRect.x, destRect.y, srcRect.w, srcRect.h, destRect.w, destRect.h, (srcRect.y / 32), (srcRect.x / 32), Game::Instance()->getRenderer());
	}
};