#pragma once

class Level {
	const Angine::Window* p_window = nullptr;
	const Angine::Camera2D* p_camera = nullptr;

	//For objects to spawn beginning from (0,0)
	float xOffset; ///= screenWidth / 2 / cameraScale
	float yOffset; ///= screenHeight / 2 / cameraScale

	b2World* p_world = nullptr;
	std::vector<std::string> p_levelData;
	b2Vec2 p_startPlayerPos;
	Player *p_player;
	std::vector<Agent*> p_agents;
	std::vector<Box*> p_boxes;
	std::vector<Box*> p_staticBoxes;
	Angine::TileSheet p_tileSheet;
	Angine::TileSheet p_boxSheet;
public:
	Level() : xOffset(0.0f), yOffset(0.0f), p_window(nullptr), p_camera(nullptr) { }
	Level(b2World* world, const Angine::Window* window, Angine::Camera2D camera);
	Level(const Level& obj);
	~Level();

	void Load(const std::string& levelPath);
	void Init();
	void Draw(Angine::Camera2D& camera, Angine::SpriteBatch& spriteBatch);
	void DrawDebug(Angine::DebugRenderer& debugRenderer);
	bool Update(Angine::InputManager& inputManager); ///< Returns true if player is dead
	void Destroy();

	void SpawnBoxGroup(Box* boxes, int& boxesPos, b2Vec2& position, b2Vec2& dims);
	std::vector<Box*> IsNearBox(const b2Vec2& checkPos);
	std::vector<Enemy*> IsNearEnemy(const b2Vec2& checkPos);

	const b2Vec2& GetStartPlayerPos() const { return p_startPlayerPos; }
	const Player* GetPlayerP() const { return p_player; }
	int GetEnemiesLeft() const { return p_agents.size() - 1; }

	Level operator=(Level&& obj);
	Level &operator=(const Level& obj);
};