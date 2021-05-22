#include "Level.h"

Level::Level(const int& tilesWide, const int& tilesHigh)
{
	_width = tilesWide;
	_height = tilesHigh;

	for (int y = 0; y < tilesHigh; y++) {
		std::vector<EnvTile> envline;
		std::vector<Container> cline;
		std::vector<Entity> entline;

		for (int x = 0; x < tilesWide; x++) {
			envline.emplace_back(x, y, '?');
			cline.emplace_back(nullptr, '?', x, y);
			entline.emplace_back('?', x, y);
		}
		_envMap.push_back(envline);
		_containerMap.push_back(cline);
		_entityMap.push_back(entline);
	}
}
