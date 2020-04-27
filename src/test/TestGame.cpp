#include "TestGame.h"

TestGame::TestGame() : AbstractGame(), score(0), lives(3), keys(10), gameWon(false), width(510), height(510), tileSize(15) {
	font = ResourceManager::loadFont("res/fonts/arial.ttf", 72);
	fontSmall = ResourceManager::loadFont("res/fonts/arial.ttf", 32);

	entityTexture = ResourceManager::loadTexture("res/texture/test.png", { 0, 0, 0xFF });
	imgWall = ResourceManager::loadTexture("res/texture/imgWall15.png", { 0,0,0xFF }); //self created
	imgBacking = ResourceManager::loadTexture("res/texture/imgBackground510.png", { 0,0,0xFF }); //self created
	imgCoin = ResourceManager::loadTexture("res/texture/imgCoin01.png", { 0,0,0xFF }); //self created

	aiCollide = ResourceManager::loadSound("res/sound/sndFailure.wav");
	coin = ResourceManager::loadSound("res/sound/sndCoin.wav");
	coin->volume = 10;

	btnPlay = Rect(width * 0.25, height * 0.5, 100, 50);
	btnDiff = Rect(width * 0.5, height * 0.5, 150, 50);

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
	ai->addMap(tileSize, width, height, wall);
	//world bounds
	wall.push_back(std::make_shared<Rect>(Rect(-tileSize, -tileSize, width+tileSize, tileSize)));
	wall.push_back(std::make_shared<Rect>(Rect(-tileSize, -tileSize, tileSize, height+tileSize)));
	wall.push_back(std::make_shared<Rect>(Rect(-tileSize, height, width+tileSize, tileSize)));
	wall.push_back(std::make_shared<Rect>(Rect(width, -tileSize, tileSize, height+tileSize)));

	player = new Entity(1, 1, tileSize - 1, tileSize - 1, true, entityTexture);

	for (int i = 0; i < keys; i++) {
		int xCord = getRandom(0, (width / tileSize));
		int yCord = getRandom(0, (height / tileSize));
		if (ai->checkPossible(Point2{ player->getX() / tileSize, player->getY() / tileSize }, Point2{ xCord , yCord })) {
			std::shared_ptr<GameKey> k = std::make_shared<GameKey>(); 
			k->alive = true;
			k->pos = Point2(xCord*tileSize + tileSize / 2, yCord*tileSize + tileSize / 2);
			points.push_back(k);
			keys--;
		} else {
			i--;
		}
	}
	keys = points.size();
}

TestGame::~TestGame() {
	delete gen;
	delete player;

	npcCollection.clear();
}

void TestGame::handleKeyEvents() {
	int speed = 2;

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
			state = PAUSE;
		}
	}

	if (eventSystem->isPressed(Key::ENTER)) {
		if (state == LOSE) {
			state = PLAY;
			score -= 50;
		}
	}
}

void TestGame::update() {
	if (state != PLAY) {
		if (state == MENU) {
			handleMenu();
		}
	} else {
		player->moveX(velocity.x);

		for (auto block : wall) {
			if (player->getCollider().intersects(*block) || player->getCollider().intersects(npc->getCollider())) {
				player->moveX(-velocity.x);
				break;
			}
		}
		for (auto block : wall) {
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
		for (auto block : wall) {
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
		for (Entity* x : npcCollection) {
			if ((x->getPathProgress() < 1) && (!x->getCollider().intersects(player->getCollider()))) {
				x->moveAlongPath();
			}
			else {
				ai->givePath(x, player);
			}
			if (x->getCollider().intersects(player->getCollider())) {
				state = LOSE;
				Mix_PlayChannel(-1, aiCollide, 0);
			}
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
	
	for (Entity * x : npcCollection) {
		gfx->drawTexture(x->getTexture(), NULL, x->getDisplay());
	}

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
		gfx->drawText("YOU FINISHED", 250, 500);
		break;
	case LOSE : 
		gfx->drawText("YOU LOSE", 250, 500);
		player->setXY(Point2{ 0, 0 });
		npc->setXY(Point2{ width-tileSize, 0 });
		npc->clearPath();
		break;
	case PAUSE :
		gfx->setDrawColor(SDL_COLOR_BLACK);
		gfx->fillRect(0, 0, width + tileSize, height + tileSize);
		gfx->setDrawColor(SDL_COLOR_WHITE);
		gfx->drawText("Are You Ready?", width*0.25, height*0.25);
		gfx->useFont(fontSmall);
		gfx->drawText("Press Enter to RESUME", width*0.25, height*0.5);
		break;
	case PLAY :
		gfx->setDrawColor(SDL_COLOR_AQUA);
		gfx->drawText(scoreStr, 780 - scoreStr.length() * 50, 25);
		break;
	case MENU :
		gfx->setDrawColor(SDL_COLOR_BLACK);
		gfx->fillRect(0, 0, width + tileSize, height + tileSize);
		if (playHover) {
			gfx->setDrawColor(SDL_COLOR_GRAY);
			gfx->fillRect(btnPlay.x, btnPlay.y, btnPlay.w, btnPlay.h);
		}
		if (diffHover) {
			gfx->setDrawColor(SDL_COLOR_GRAY);
			gfx->fillRect(btnDiff.x, btnDiff.y, btnDiff.w, btnDiff.h);
		}
		gfx->setDrawColor(SDL_COLOR_WHITE);
		gfx->drawText("Are You Ready?", width * 0.25, height * 0.25);
		gfx->drawRect(btnPlay);
		gfx->drawRect(btnDiff);
		gfx->useFont(fontSmall);
		gfx->drawText("Play", width * 0.25, height * 0.5);
		switch (npcCount) {
		case 3 :
			gfx->drawText("EASY", width * 0.5, height * 0.5);
			break;
		case 5 :
			gfx->drawText("MEDIUM", width * 0.5, height * 0.5);
			break;
		case 7 :
			gfx->drawText("HARD", width * 0.5, height * 0.5);
			break;
		default:
			break;
		}
		break;
	default :
		break;
	}
	gfx->useFont(font);
}

void TestGame::handleMenu() {
	if ((eventSystem->getMousePos().x > btnPlay.x) && (eventSystem->getMousePos().x < btnPlay.x + btnPlay.w) && (eventSystem->getMousePos().y > btnPlay.y) && (eventSystem->getMousePos().y < btnPlay.y + btnPlay.h)) {
		if (eventSystem->isPressed(BTN_LEFT) && btnBreak) {
			state = PLAY;
			btnBreak = false;
			//NPC generation
			for (int i = 0; i < npcCount; i++) {
				int xCord = getRandom(0, width / tileSize);
				int yCord = getRandom(0, height / tileSize);
				if (ai->checkPossible(Point2{ xCord, yCord }, Point2{ xCord, yCord })) {
					npc = new Entity(xCord * tileSize, yCord * tileSize, tileSize - 1, tileSize - 1, true, entityTexture);
					npc->setSight(10);
					npc->patrol(getRandom(0, 2));
					npcCollection.push_back(npc);
					std::cout << "NPC " << i + 1 << " Spawned at " << xCord << " " << yCord << std::endl;
				}
				else {
					i--;
				}
			}
		}
		else {
			if (!eventSystem->isPressed(BTN_LEFT)) {
				btnBreak = true;
			}
			playHover = true;
		}
	}
	else {
		playHover = false;
	}
	if ((eventSystem->getMousePos().x > btnDiff.x) && (eventSystem->getMousePos().x < btnDiff.x + btnDiff.w) && (eventSystem->getMousePos().y > btnDiff.y) && (eventSystem->getMousePos().y < btnDiff.y + btnDiff.h)) {
		if (eventSystem->isPressed(BTN_LEFT) && btnBreak) {
			btnBreak = false;
			if (npcCount != 7) {
				npcCount += 2;
			}
			else {
				npcCount = 3;
			}
		}
		else {
			if (!eventSystem->isPressed(BTN_LEFT)) {
				btnBreak = true;
			}
			diffHover = true;
		}
	}
	else {
		diffHover = false;
	}
}