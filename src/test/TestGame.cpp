#include "TestGame.h"

TestGame::TestGame() : AbstractGame(), score(0), lives(3), keys(5), gameWon(false), width(510), height(510), tileSize(15) { //npc(1,271,29,29)
	TTF_Font * font = ResourceManager::loadFont("res/fonts/arial.ttf", 72);
	SDL_Texture * texture = ResourceManager::loadTexture("res/texture/test.png", { 0, 0, 0xFF });
	gfx->useFont(font);
	gfx->setVerticalSync(true);

	gen = new MazeGenerator(width/tileSize, height/tileSize);
	gen->generateMaze(0, 0);
	for (int i = 0; i < height / tileSize; i++) {
		for (int j = 0; j < width / tileSize; j++) {
			if ((getRandom(0, 3) == 1) && (i != 0) && (j != 0)) {
				wall.push_back(std::make_shared<Rect>(Rect(i*tileSize,j*tileSize,tileSize,tileSize)));
			}
		}
	}
	//world bounds
	wall.push_back(std::make_shared<Rect>(Rect(-tileSize, -tileSize, width+tileSize, tileSize)));
	wall.push_back(std::make_shared<Rect>(Rect(-tileSize, -tileSize, tileSize, height+tileSize)));
	wall.push_back(std::make_shared<Rect>(Rect(-tileSize, height, width+tileSize, tileSize)));
	wall.push_back(std::make_shared<Rect>(Rect(width, -tileSize, tileSize, height+tileSize)));

	int dist = tileSize;

	for (int i = 0; i < gen->y; ++i) {

		int y = i * dist;

		for (int j = 0; j < gen->x; ++j) {

			int x = j * dist;

			if ((gen->maze[j][i] & 1) == 0) {
				//lines.push_back(std::make_shared<Line2i>(Point2(x, y), Point2(x+dist, y)));
			}

			if ((gen->maze[j][i] & 8) == 0) {
				//lines.push_back(std::make_shared<Line2i>(Point2(x, y), Point2(x, y+dist)));
			}

			if (keys > 0 && getRandom(0, 100) <= 5) {
				std::shared_ptr<GameKey> k = std::make_shared<GameKey>();
				k->alive = true;
				k->pos = Point2(j*dist + dist/2, i*dist + dist/2);
				points.push_back(k);
				keys--;
			}
		}

		//lines.push_back(std::make_shared<Line2i>(Point2(gen->x*dist, y), Point2(gen->x*dist, y + dist)));
	}

	for (int j = 0; j < gen->x; j++) {
		int x = j * dist;
		//lines.push_back(std::make_shared<Line2i>(Point2(x, gen->y * dist), Point2(x + dist, gen->y * dist)));
	}

	keys = 5;
	npc = new Entity(1, 271, tileSize-1, tileSize-1, true, texture);
	player = new Entity(1, 1, tileSize-1, tileSize-1, true, texture);
	theAI = new AIController(1,300,300,lines);
}

TestGame::~TestGame() {
	delete gen;
}

void TestGame::handleKeyEvents() {
	int speed = 3;

	if (eventSystem->isPressed(Key::W)) {
		velocity.y = -speed;
	}

	if (eventSystem->isPressed(Key::S)) {
		velocity.y = speed;
	}

	if (eventSystem->isPressed(Key::A)) {
		velocity.x = -speed;
	}

	if (eventSystem->isPressed(Key::D)) {
		velocity.x = speed;
	}
}

void TestGame::update() {
	player->moveX(velocity.x);
	if (player->collider.x > npc->collider.x) { //would need to go in ai sub?
		npcVel.x = 1;
	}
	else if (player->collider.x < npc->collider.x) {
		npcVel.x = -1;
	}
	npc->moveX(npcVel.x); //
	
	for (auto block : wall) {
		if (player->collider.intersects(*block)||player->collider.intersects(npc->collider)) {
			player->moveX(-velocity.x);
			break;
		}
	}
	for (auto block : wall) {
		if (npc->collider.intersects(*block)||npc->collider.intersects(player->collider)) {
			npc->moveX(-npcVel.x);
			break;
		}
	}

	player->moveY(velocity.y);
	if (player->collider.y > npc->collider.y) { //would need to go in ai sub?
		npcVel.y = 1;
	}
	else if (player->collider.y < npc->collider.y){
		npcVel.y = -1;
	}
	npc->moveY(npcVel.y); //

	for (auto block : wall) {
		if (player->collider.intersects(*block)||player->collider.intersects(npc->collider)) {
			player->moveY(-velocity.y);
			break;
		}
	}
	for (auto block : wall) {
		if (npc->collider.intersects(*block)||npc->collider.intersects(player->collider)) {
			npc->moveY(-npcVel.y);
			break;
		}
	}

	for (auto key : points) {
		if (key->alive && player->collider.contains(key->pos)) {
			score += 200;
			key->alive = false;
			keys--;
		}
	}

	velocity = Vector2i(0, 0);

	if (keys == 0) {
		gameWon = true;
	}
}

void TestGame::render() {
	gfx->drawTexture(npc->texture, NULL, npc->display);
	gfx->drawTexture(npc->texture, NULL, player->display);
	gfx->setDrawColor(SDL_COLOR_WHITE);
	for (auto block : wall)
		gfx->drawRect(block->x, block->y, block->w, block->h);

	gfx->setDrawColor(SDL_COLOR_RED); //temp to show boundingBox
	gfx->drawRect(npc->collider); //temp to show boundingBox
	gfx->drawRect(player->collider); //temp to show boundingBox

	gfx->setDrawColor(SDL_COLOR_YELLOW);
	for (auto key : points)
		if (key->alive)
			gfx->drawPoint(key->pos);
}

void TestGame::renderUI() {
	gfx->setDrawColor(SDL_COLOR_AQUA);
	std::string scoreStr = std::to_string(score);
	gfx->drawText(scoreStr, 780 - scoreStr.length() * 50, 25);

	if (gameWon)
		gfx->drawText("YOU WON", 250, 500);
}