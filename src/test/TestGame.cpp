#include "TestGame.h"

TestGame::TestGame() : AbstractGame(), score(0), lives(3), keys(10), gameWon(false), width(510), height(510), tileSize(15) {
	TTF_Font * font = ResourceManager::loadFont("res/fonts/arial.ttf", 72);

	SDL_Texture * entityTexture = ResourceManager::loadTexture("res/texture/test.png", { 0, 0, 0xFF });
	imgWall = ResourceManager::loadTexture("res/texture/imgWall15.png", { 0,0,0xFF }); //self created
	imgBacking = ResourceManager::loadTexture("res/texture/imgBackground510.png", { 0,0,0xFF }); //self created
	imgCoin = ResourceManager::loadTexture("res/texture/imgCoin01.png", { 0,0,0xFF }); //self created

	aiCollide = ResourceManager::loadSound("res/sound/sndFailure.wav");
	coin = ResourceManager::loadSound("res/sound/sndCoin.wav");
	coin->volume = 10;

	gfx->useFont(font);
	gfx->setVerticalSync(true);

	gen = new MazeGenerator(width/tileSize, height/tileSize);
	gen->generateMaze(0, 0);
	for (int i = 0; i < height / tileSize; i++) {
		for (int j = 0; j < width / tileSize; j++) { //set walls as entity type? allow assigning different texture.
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

	ai->addMap(tileSize, width, height, wall);

	int dist = tileSize;

	for (int i = 0; i < gen->y; ++i) {

		int y = i * dist;

		for (int j = 0; j < gen->x; ++j) {

			int x = j * dist;

			if (keys > 0 && getRandom(0, 200) <= 1) {
				bool check = false;
				if (ai->checkPossible(Point2{ (width/2)/dist, (height/2)/dist }, Point2{ j , i })) { //checks for valid path to key --make faster, cut corners
					check = true;
				}
				/*if (block->contains(Point2(j*dist + dist / 2, i*dist + dist / 2))) {
					check = true;
					break;
				}*/
				if (check == true) {
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
	npc->patrol(true); //maybe change to addBehaviour, eg npc->addBehaviour(R_PATROL), npc->addBehaviour(GUARD) etc.
	player = new Entity(1, 1, tileSize-1, tileSize-1, true, entityTexture);
	npc2 = new Entity(1, 271, tileSize - 1, tileSize - 1, true, entityTexture);
	npc2->setSight(20);

	/*npc2 = new Entity(1, 271, tileSize - 1, tileSize - 1, true, entityTexture);
	npc2->setSight(10);
	npc2->patrol(true); 
	npc3 = new Entity(271, 271, tileSize - 1, tileSize - 1, true, entityTexture);
	npc3->setSight(10);
	npc3->patrol(true); */
}

TestGame::~TestGame() {
	delete gen;
	delete npc;
	delete npc2;
	//delete npc3;
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

	if (eventSystem->isPressed(Key::P)) {
		if (state == PLAY) {
			playIntent = false;
			state = PAUSE;
		}
	}

	if (eventSystem->isPressed(Key::ENTER)) {
		if (!playIntent) { 
			playIntent = true; 
		}
	}
}

void TestGame::update() {
	if (state != PLAY) {
		if (playIntent) { state = PLAY; }
	}
	else {
		player->moveX(velocity.x);

		for (auto block : wall) {
			if (player->getCollider().intersects(*block) || player->getCollider().intersects(npc->getCollider())) {
				player->moveX(-velocity.x);
				break;
			}
		}
		for (auto block : wall) { //currently ai just merges with player
			if (npc->getCollider().intersects(*block) || npc->getCollider().intersects(player->getCollider())) {
				npc->moveX(-npcVel.x);
				break;
			}
		}

		player->moveY(velocity.y);

		for (auto block : wall) {
			if (player->getCollider().intersects(*block) || player->getCollider().intersects(npc->getCollider())) {
				player->moveY(-velocity.y);
				break;
			}
		}
		for (auto block : wall) { //currently ai just merges with player
			if (npc->getCollider().intersects(*block) || npc->getCollider().intersects(player->getCollider())) {
				npc->moveY(-npcVel.y);
				break;
			}
		}

		for (auto key : points) {
			if (key->alive && player->getCollider().contains(key->pos)) {
				Mix_PlayChannel(-1, coin, 0);
				score += 200;
				key->alive = false;
				keys--;
			}
		}

		velocity = Vector2i(0, 0);
		if ((npc->getPathProgress() < 1) && (!npc->getCollider().intersects(player->getCollider()))) {
			npc->moveAlongPath();
		}
		else {
			ai->givePath(npc, player);
		}

		if ((npc2->getPathProgress() < 1) && (!npc2->getCollider().intersects(player->getCollider()))) {
			npc2->moveAlongPath();
		}
		else {
			ai->givePath(npc2, player);
		}
		/*if ((npc3->getPathProgress() < 1) && (!npc3->collider.intersects(player->collider))) {
			npc3->moveAlongPath();
		}
		else {
			ai->givePath(npc3, player);
		}*/

		if (npc->getCollider().intersects(player->getCollider())) {
			state = LOSE;
			Mix_PlayChannel(-1, aiCollide, 0);
		}
		if (npc2->getCollider().intersects(player->getCollider())) {
			state = LOSE;
			Mix_PlayChannel(-1, aiCollide, 0);
		}

		if (keys == 0) {
			state = WIN;
		}
	}
}

void TestGame::render() {
	SDL_Rect * bg = new SDL_Rect{ 0,0,width,height };
	gfx->drawTexture(imgBacking, NULL, bg);
	delete bg;

	gfx->drawTexture(npc->getTexture(), NULL, npc->getDisplay());
	gfx->drawTexture(npc2->getTexture(), NULL, npc2->getDisplay());
	//gfx->drawTexture(npc3->texture, NULL, npc3->display);

	gfx->drawTexture(player->getTexture(), NULL, player->getDisplay());

	gfx->setDrawColor(SDL_COLOR_WHITE);


	for (auto block : wall) {
		gfx->drawRect(block->x, block->y, block->w, block->h);
		SDL_Rect * draw = new SDL_Rect{ block->x, block->y, block->w, block->h };
		gfx->drawTexture(imgWall, NULL, draw);
		delete draw;
	}

	for (auto key : points)
		if (key->alive) {
			SDL_Rect * coinRect = new SDL_Rect{ key->pos.x - 4, key->pos.y - 4, 9, 9 };
			gfx->drawTexture(imgCoin, NULL, coinRect);
			gfx->drawPoint(key->pos);
			delete coinRect;
		}
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
		gfx->drawText(scoreStr, 780 - scoreStr.length() * 50, 25);
		playIntent = false;
		break;
	case LOSE : 
		gfx->drawText("YOU LOSE", 250, 500);
		player->setXY(Point2{ 0, 0 });
		npc->setXY(Point2{ width-tileSize, 0 });
		npc->clearPath();
		score -= 50;
		playIntent = false;
		break;
	case PAUSE :
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