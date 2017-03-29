#pragma once

class Level {
	const Angine::Window* p_window = nullptr;
	const Angine::Camera2D* p_camera = nullptr;

	//For objects to spawn beginning from (0,0)
	float xOffset; ///= screenWidth / 2 / cameraScale
	float yOffset; ///= screenHeight / 2 / cameraScale

	b2World* p_world = nullptr;
	std::vector<std::string> p_levelData;
	Angine::SpriteBatch* p_spriteBatch = nullptr;
	b2Vec2 p_startPlayerPos;
	std::vector<Box> p_boxes;
	Angine::TileSheet p_tileSheet;
	Angine::TileSheet p_boxSheet;
public:
	Level() : xOffset(0.0f), yOffset(0.0f), p_window(nullptr), p_camera(nullptr) { }
	Level(b2World* world, const Angine::Window* window, Angine::Camera2D camera);
	Level(const Level& obj);
	~Level() {}

	void Load(const std::string& levelPath);
	void Init();
	void Draw(Angine::Camera2D& camera);
	void DrawDebug(Angine::DebugRenderer& debugRenderer);

	void MakeBoxGroup(b2Vec2& position, b2Vec2& dims);

	const b2Vec2& GetStartPlayerPos() const { return p_startPlayerPos; }

	Level operator=(Level&& obj);
	Level &operator=(const Level& obj);
};