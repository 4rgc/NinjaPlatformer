#include "Header.h"

Level::Level(b2World* world, const Angine::Window* window, Angine::Camera2D camera) :
			p_world(world),
			p_window(window),
			p_camera(&camera),
			xOffset(-((window->getScreenW() / 2.0f) / camera.GetScale())),
			yOffset(-((window->getScreenH() / 2.0f) / camera.GetScale()))
{
	p_spriteBatch = new Angine::SpriteBatch();
	p_spriteBatch->Init();
}

Level::Level(const Level& obj) : xOffset(obj.xOffset), yOffset(obj.yOffset) {
	p_window = obj.p_window;
	p_world = obj.p_world;
	p_camera = obj.p_camera;
}

void Level::Load(const std::string& levelPath) {
	std::string unmagnagedData;
	Angine::IOManager::ReadFileToBuffer(unmagnagedData, levelPath);

	size_t pNLine = 0;
	size_t nLine = 0;
	while ((nLine = unmagnagedData.find('\n', nLine + 1)) != std::string::npos) {
		p_levelData.emplace_back(unmagnagedData.substr(pNLine, nLine - pNLine));
		pNLine = nLine;
	}

	for (int i = 0; i < p_levelData.size(); i++) {
		p_levelData[i].erase(std::remove(p_levelData[i].begin(), p_levelData[i].end(), '\n'), p_levelData[i].end());
	}
	for (int i = 0; i < p_levelData.size(); i++) {
		p_levelData[i].erase(std::remove(p_levelData[i].begin(), p_levelData[i].end(), '\r'), p_levelData[i].end());
	}

	bool found = false;
	for (int i = 0; i < p_levelData.size(); i++) {
		if(!found)
		for (int j = 0; j < p_levelData[i].size(); j++) {
			if (p_levelData[i][j] == 'P') {
				p_startPlayerPos = b2Vec2(j + xOffset, i + yOffset);
				found = true;
				break;
			}
		}
	}
}

void Level::Init() {
	if (!p_world) {
		std::cout << "World is not set";
		system("PAUSE");
		return;
	}
	p_tileSheet.Init(Angine::ResourceManager::GetTexture("Textures/textures/tiles.png"), glm::ivec2(8, 8));
	p_boxSheet.Init(Angine::ResourceManager::GetTexture("Textures/textures/boxsheet.png"), glm::ivec2(8, 1));

	Box box;
	BoxDef bDef(p_world);
	b2Vec2 pos;
	b2Vec2 dims;
	for (int i = 0; i < p_levelData.size(); i++) {
		int a = 5;
		for (int j = 0; j < p_levelData[i].size(); j++) {
			switch (p_levelData[i][j]) {
				case 'W':
					bDef = BoxDef(p_world, b2Vec2(j + xOffset, i + yOffset), b2Vec2(1.0f, 1.0f));
					box.Init(&bDef, true, true, p_tileSheet.texture, p_tileSheet.getUV(14));
					box.SetColor(Angine::ColorRGBA8(120,120,120,255));
					p_staticBoxes.push_back(box);
					break;
				case 'O': 
					bDef = BoxDef(p_world, b2Vec2(j + xOffset, i + yOffset + 0.05f), b2Vec2(1.0f, 0.9f));
					box.Init(&bDef, true, true, p_tileSheet.texture, p_tileSheet.getUV(17));
					box.SetColor(Angine::ColorRGBA8(140, 140, 140, 255));
					p_staticBoxes.push_back(box);
					break;
				case 'b': ///< group of small boxes
					pos = b2Vec2(j + xOffset, i + yOffset);
					dims = b2Vec2(0.5f, 0.5f);
					SpawnBoxGroup(pos, dims);
					break;
				case 'B': ///< group of large boxes
					pos = b2Vec2(j + xOffset, i + yOffset);
					dims = b2Vec2(0.8f, 0.8f);
					SpawnBoxGroup(pos, dims);
					break;
			}
		}
	}
	CapsuleDef player(p_world, p_startPlayerPos, b2Vec2(1.0f, 2.0f), 1.0f, 0.1f);
	player.drawDims = glm::vec2(2.0f);
	p_player.Init(&player);

}

void Level::SpawnBoxGroup(b2Vec2 & position, b2Vec2 & dims) {
	BoxDef bDef = BoxDef(p_world, b2Vec2(position.x - dims.x, position.y), dims);
	Box box;
	box.Init(&bDef, false, false, p_boxSheet.texture, p_boxSheet.getUV(7));
	p_boxes.push_back(box);
	bDef = BoxDef(p_world, b2Vec2(position.x, position.y), dims);
	box.Init(&bDef, false, false, p_boxSheet.texture, p_boxSheet.getUV(7));
	p_boxes.push_back(box);
	bDef = BoxDef(p_world, b2Vec2(position.x - dims.x /2.0f, position.y + dims.y), dims);
	box.Init(&bDef, false, false, p_boxSheet.texture, p_boxSheet.getUV(7));
	p_boxes.push_back(box);
}

Level Level::operator=(Level&& obj) {
	if (obj.p_window == nullptr)
		return Level();

	p_world = obj.p_world;
	obj.p_world = nullptr;
	p_window = obj.p_window;
	obj.p_window = nullptr;
	p_camera = obj.p_camera;
	obj.p_camera = nullptr;
	p_spriteBatch = obj.p_spriteBatch;
	obj.p_spriteBatch = nullptr;
	xOffset = obj.xOffset;
	yOffset = obj.yOffset;
	return *this;
}

Level &Level::operator=(const Level& obj) {
	if (this == &obj)
		return *this;

	p_world = obj.p_world;
	p_window = obj.p_window;
	p_camera = obj.p_camera;
	p_spriteBatch = obj.p_spriteBatch;
	return *this;
}


void Level::Draw(Angine::Camera2D& camera) {
	//Draw the background
	p_spriteBatch->Begin();	
	p_spriteBatch->Draw(glm::vec4(camera.GetPosition().x + xOffset, camera.GetPosition().y + yOffset, -xOffset * 2.0f, -yOffset * 2.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), Angine::ResourceManager::GetTexture("Textures/textures/bg.png").ID, 0.0f, Angine::ColorRGBA8(255, 255, 255, 255));
	p_spriteBatch->End();
	p_spriteBatch->RenderBatch();
	
	//Draw the boxes
	p_spriteBatch->Begin();
	//dynamic
	for each(Box b in p_boxes) {
		if(b.GetBody()->GetPosition().x > camera.GetPosition().x + xOffset -1.5f || 
			b.GetBody()->GetPosition().x < camera.GetPosition().x - xOffset + 1.5f)
		b.Draw(*p_spriteBatch);
	}
	//static
	for each(Box b in p_staticBoxes) {
		if (b.GetBody()->GetPosition().x > camera.GetPosition().x + xOffset - 1.5f ||
			b.GetBody()->GetPosition().x < camera.GetPosition().x - xOffset + 1.5f)
			b.Draw(*p_spriteBatch);
	}
	p_spriteBatch->End();
	p_spriteBatch->RenderBatch();
}

void Level::DrawDebug(Angine::DebugRenderer& debugRenderer) {
	glm::vec4 destRect;
	Angine::ColorRGBA8 color(0,255,0,255);
	Angine::ColorRGBA8 SColor(255, 0, 0, 255);
	//Dynamic boxes are green
	for each(Box b in p_boxes) {
		destRect.x = b.GetBody()->GetPosition().x - b.GetDims().x / 2.0f;
		destRect.y = b.GetBody()->GetPosition().y - b.GetDims().y / 2.0f;
		destRect.w = b.GetDims().x;
		destRect.z = b.GetDims().y;
		debugRenderer.DrawBox(destRect, color, 0);
	}
	//Static boxes are red
	for each(Box b in p_staticBoxes) {
		destRect.x = b.GetBody()->GetPosition().x - b.GetDims().x / 2.0f;
		destRect.y = b.GetBody()->GetPosition().y - b.GetDims().y / 2.0f;
		destRect.w = b.GetDims().x;
		destRect.z = b.GetDims().y;
		debugRenderer.DrawBox(destRect, SColor, 0);
	}
}