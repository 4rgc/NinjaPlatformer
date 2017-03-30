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

	Box *box;
	BoxDef bDef(p_world);
	b2Vec2 pos;
	b2Vec2 dims;
	Box* boxes;
	Box* sBoxes;
	int count = 0, sCount = 0;
	//Count quantity of all boxes in the level to allocate cache-friendly memory
	for (int i = 0; i < p_levelData.size(); i++) {
		for (int j = 0; j < p_levelData[i].size(); j++) {
			switch (p_levelData[i][j]) {
			case 'W':
				sCount++;
				break;
			case 'O':
				sCount++;
				break;
			case 'b': ///< group of small boxes
				count += 3;
				break;
			case 'B': ///< group of large boxes
				count += 3;
				break;
			}
		}
	}
	boxes = new Box[count];
	sBoxes = new Box[sCount];
	int iBox = 0, iSBox = 0;
	for (int i = 0; i < p_levelData.size(); i++) {
		for (int j = 0; j < p_levelData[i].size(); j++) {
			switch (p_levelData[i][j]) {
				case 'W':
					bDef = BoxDef(p_world, b2Vec2(j + xOffset, i + yOffset), b2Vec2(1.0f, 1.0f));
					sBoxes[iSBox].Init(&bDef, true, true, p_tileSheet.texture, p_tileSheet.getUV(14));
					sBoxes[iSBox].SetColor(Angine::ColorRGBA8(120,120,120,255));
					p_staticBoxes.push_back(&sBoxes[iSBox]);
					iSBox++;
					break;
				case 'O': 
					bDef = BoxDef(p_world, b2Vec2(j + xOffset, i + yOffset + 0.05f), b2Vec2(1.0f, 0.9f));
					sBoxes[iSBox].Init(&bDef, true, true, p_tileSheet.texture, p_tileSheet.getUV(17));
					sBoxes[iSBox].SetColor(Angine::ColorRGBA8(140, 140, 140, 255));
					p_staticBoxes.push_back(&sBoxes[iSBox]);
					iSBox++;
					break;
				case 'b': ///< group of small boxes
					pos = b2Vec2(j + xOffset, i + yOffset);
					dims = b2Vec2(0.5f, 0.5f);
					SpawnBoxGroup(boxes, iBox, pos, dims);
					break;
				case 'B': ///< group of large boxes
					pos = b2Vec2(j + xOffset, i + yOffset);
					dims = b2Vec2(0.8f, 0.8f);
					SpawnBoxGroup(boxes, iBox, pos, dims);
					break;
			}
		}
	}
	CapsuleDef player(p_world, p_startPlayerPos, b2Vec2(1.0f, 2.0f), 1.0f, 0.1f);
	player.drawDims = glm::vec2(2.0f);
	p_player.Init(&player, this);

}

void Level::SpawnBoxGroup(Box* boxes, int& boxesPos, b2Vec2 & position, b2Vec2 & dims) {
	BoxDef bDef = BoxDef(p_world, b2Vec2(position.x - dims.x, position.y), dims);
	boxes[boxesPos].Init(&bDef, false, false, p_boxSheet.texture, p_boxSheet.getUV(0));
	p_boxes.push_back(&boxes[boxesPos]);
	boxesPos++;

	bDef = BoxDef(p_world, b2Vec2(position.x, position.y), dims);
	boxes[boxesPos].Init(&bDef, false, false, p_boxSheet.texture, p_boxSheet.getUV(0));
	p_boxes.push_back(&boxes[boxesPos]);
	boxesPos++;

	bDef = BoxDef(p_world, b2Vec2(position.x - dims.x /2.0f, position.y + dims.y), dims);
	boxes[boxesPos].Init(&bDef, false, false, p_boxSheet.texture, p_boxSheet.getUV(0));
	p_boxes.push_back(&boxes[boxesPos]);
	boxesPos++;
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
	for (int i = 0; i < p_boxes.size(); ) {
		//Draw boxes only if they are in vision 
		if (p_boxes[i]->GetBody()->GetPosition().x > camera.GetPosition().x + xOffset - 1.5f ||
			p_boxes[i]->GetBody()->GetPosition().x < camera.GetPosition().x - xOffset + 1.5f) {
			if (p_boxes[i]->GetTileID() != 1) {
				p_boxes[i]->Draw(*p_spriteBatch, p_boxSheet);
				if (p_boxes[i]->GetTileID() == -7) {
					p_boxes[i]->Destroy();
					p_boxes[i] = p_boxes.back();
					p_boxes.pop_back();
				}
				else
					i++;
			}
			else {
				p_boxes[i]->Draw(*p_spriteBatch);
				i++;
			}
		}
	}
	//static
	for each(Box* b in p_staticBoxes) {
		//Draw boxes only if they are in vision 
		if (b->GetBody()->GetPosition().x > camera.GetPosition().x + xOffset - 1.5f ||
			b->GetBody()->GetPosition().x < camera.GetPosition().x - xOffset + 1.5f)
			b->Draw(*p_spriteBatch);
	}
	p_spriteBatch->End();
	p_spriteBatch->RenderBatch();
}

void Level::DrawDebug(Angine::DebugRenderer& debugRenderer) {
	glm::vec4 destRect;
	Angine::ColorRGBA8 color(0,255,0,255);
	Angine::ColorRGBA8 SColor(255, 0, 0, 255);
	//Dynamic boxes are green
	for each(Box* b in p_boxes) {
		destRect.x = b->GetBody()->GetPosition().x - b->GetDims().x / 2.0f;
		destRect.y = b->GetBody()->GetPosition().y - b->GetDims().y / 2.0f;
		destRect.w = b->GetDims().x;
		destRect.z = b->GetDims().y;
		debugRenderer.DrawBox(destRect, color, 0);
	}
	//Static boxes are red
	for each(Box* b in p_staticBoxes) {
		destRect.x = b->GetBody()->GetPosition().x - b->GetDims().x / 2.0f;
		destRect.y = b->GetBody()->GetPosition().y - b->GetDims().y / 2.0f;
		destRect.w = b->GetDims().x;
		destRect.z = b->GetDims().y;
		debugRenderer.DrawBox(destRect, SColor, 0);
	}
}

//Returns a vector of boxes near to checkPos
std::vector<Box*> Level::IsNearBox(const b2Vec2& checkPos) {
	std::vector<Box*> nearBoxes;
	b2Vec2 BPos;
	for each(Box* b in p_boxes) {
		BPos = b->GetBody()->GetPosition();
		if (floor(checkPos.y) == floor((float)BPos.y) || ceil(checkPos.y) == ceil((float)BPos.y)) {
			if (floor((float)BPos.x) < floor(checkPos.x) + 1.1f && floor((float)BPos.x) > floor(checkPos.x) - 1.1f)
				nearBoxes.push_back(b);
		}
	}
	return nearBoxes;
}