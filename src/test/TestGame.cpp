#include "TestGame.h"

TestGame::TestGame() : AbstractGame(), score(0), lives(3), keys(10), gameWon(false), width(510), height(510), tileSize(15) { //npc(1,271,29,29)
	TTF_Font * font = ResourceManager::loadFont("res/fonts/arial.ttf", 72);
	SDL_Texture * entityTexture = ResourceManager::loadTexture("res/texture/test.png", { 0, 0, 0xFF });
	//SDL_Texture * wallTexture = ResourceManager::loadTexture("", { 0, 0, 0xFF });
	gfx->useFont(font);
	gfx->setVerticalSync(true);

	gen = new MazeGenerator(width/tileSize, height/tileSize);
	gen->generateMaze(0, 0);
	for (int i = 0; i < height / tileSize; i++) {
		for (int j = 0; j < width / tileSize; j++) { //set walls as entity type? allow assigning texture.
			if ((getRandom(0, 3) == 1) && (i != 0) && (j != 0)) { //at the very least look into making the aesthetic more dynamic
				wall.push_back(std::make_shared<Rect>(Rect(i*tileSize, j*tileSize, tileSize, tileSize)));
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

			if (keys > 0 && getRandom(0, 200) <= 1) {
				bool check = false;
				for (auto block : wall) {
					if (block->contains(Point2(j*dist + dist / 2, i*dist + dist / 2))) {
						check = true;
						break;
					}
				}
				if (!check) {
					std::shared_ptr<GameKey> k = std::make_shared<GameKey>(); //maybe pathfind to keys, if cannot pathfind, move key? some keys are currently spawning blocked off on all sides
					k->alive = true;											//maybe move demo goal away from 'keys' concept
					k->pos = Point2(j*dist + dist / 2, i*dist + dist / 2);
					points.push_back(k);
					keys--;
				}
			}
		}
	}

	keys = points.size();
	npc = new Entity(271, 1, tileSize-1, tileSize-1, true, entityTexture);
	npc->setSight(10);
	npc->patrol(true); //maybe change to addBehaviour, eg npc->addBehaviour(R_PATROL), npc->addBehaviour(GUARD)
	player = new Entity(1, 1, tileSize-1, tileSize-1, true, entityTexture);
	ai->addMap(tileSize, width, height, wall);
}

TestGame::~TestGame() {
	delete gen;
	delete npc;
	delete player;
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

	if (eventSystem->isPressed(Key::ENTER)) {
		if (!playIntent) { playIntent = true; }
	}
}

void TestGame::update() {
	if (state != PLAY) {
		if (playIntent) { state = PLAY; }
	}
	else {
		player->moveX(velocity.x);

		for (auto block : wall) {
			if (player->collider.intersects(*block) || player->collider.intersects(npc->collider)) {
				player->moveX(-velocity.x);
				break;
			}
		}
		for (auto block : wall) { //currently ai just merges with player
			if (npc->collider.intersects(*block) || npc->collider.intersects(player->collider)) {
				npc->moveX(-npcVel.x);
				break;
			}
		}

		player->moveY(velocity.y);

		for (auto block : wall) {
			if (player->collider.intersects(*block) || player->collider.intersects(npc->collider)) {
				player->moveY(-velocity.y);
				break;
			}
		}
		for (auto block : wall) { //currently ai just merges with player
			if (npc->collider.intersects(*block) || npc->collider.intersects(player->collider)) {
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
		if ((npc->getPathProgress() < 1) && (!npc->collider.intersects(player->collider))) {
			npc->moveAlongPath();
		}
		else {
			ai->givePath(npc, player);
		}
		if (npc->collider.intersects(player->collider)) {
			state = LOSE;
		}

		if (keys == 0) {
			state = WIN;
		}
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
	if (state == PAUSE) {
		gfx->setDrawColor(SDL_COLOR_BLACK);
		gfx->drawRect(0, 0, width, height);
	}
}

void TestGame::renderUI() {
	gfx->setDrawColor(SDL_COLOR_AQUA);
	std::string scoreStr = std::to_string(score);

	switch (state) {
	case WIN :
		gfx->drawText("YOU WON", 250, 500);
		break;
	case LOSE : 
		gfx->drawText("YOU LOSE", 250, 500);
		player->setXY(Point2{ 0, 0 });
		npc->setXY(Point2{ width-tileSize, 0 });
		npc->clearPath();
		score -= 50;
		break;
	case PAUSE:
		gfx->setDrawColor(SDL_COLOR_BLACK);
		gfx->fillRect(0, 0, width + tileSize, height + tileSize);
		gfx->setDrawColor(SDL_COLOR_WHITE);
		gfx->drawText("Are You Ready?", width*0.25, height*0.25);
		gfx->drawText("Press Enter to Play", width*0.25, height*0.5);
		break;
	case PLAY :
		gfx->setDrawColor(SDL_COLOR_AQUA);
		gfx->drawText(scoreStr, 780 - scoreStr.length() * 50, 25);
	default :
		break;
	}
}