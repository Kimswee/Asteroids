#include "Game.h"
#include "drawPrimitives.h"
#include <time.h>

/* this is called by std::sort to sort the list based on layerID 
 *  for drawing in the proper order 
 */
bool spriteSortingFunction(Sprite *s1, Sprite *s2)
{
	// return true if s1's layerID is less than s2's layerID
	return (s1->layerID < s2->layerID);
}

/* constructor */
Game::Game(void)
{
	/* green background */
	stateInfo.bgClearColor.red = 1;
	stateInfo.bgClearColor.green = 1;
	stateInfo.bgClearColor.blue = 1;

	/* init state */
	stateInfo.gameState = STATE_GAMEPLAY;
	renderingTimer = new Timer("RENDER");
	updateTimer = new Timer("UPDATE");
}

/* destructor */
Game::~Game(void)
{
	/* deallocate memory and clean up here. if needed */
}

/* 
 * initializeGame()
 * - this function is called in the constructor to initialize everything related
 *   to the game, i..e loading sprites etc.
 * - MUST be called prior to any drawing/updating (you should add in checks to ensure this occurs in the right order)
 */
void Game::initializeGame()
{
	/* this is a sprite without any animations, it is just an image */

	spaceShip = new Sprite("images/spaceship.png");
	spaceShip->setNumberOfAnimations(1);
	spaceShip->setSpriteFrameSize(60, 105);
	spaceShip->addSpriteAnimFrame(0, 0, 0);
	spaceShip->setPosition(450, 200);
	spaceShip->setCenter(60 / 2, 105 / 2); // center of the sprites origin for rotation
	spaceShip->setLayerID(3);
	addSpriteToDrawList(spaceShip);

	thrustMultiplier = 500;
	rotationMultiplier = 100;

	upPressed = false;
	downPressed = false;
	leftPressed = false;
	rightPressed = false;


	//for loop to spawn asteroids, iteration number will be random
	//spawnAsteroid(Vector3(-100, -100, 0), Vector3(20, 20, 0), true);

	srand(time(0));
	//for (int i = 0 ; i < 10 ; i++)
	//	std::cout << rand() << std::endl;

	int subtractNumber = 50;
	int randomNumber = rand() % 10;
	for (int i = 0 ; i < randomNumber + 1 ; i++)
	spawnAsteroid(
		Vector3((rand() % 99) - subtractNumber, (rand() % 99) - subtractNumber, 0), 
		Vector3((rand() % 99) - subtractNumber, (rand() % 99) - subtractNumber, 0)
	,true);

	

	//spawnAsteroid(Vector3(50, 50, 0), Vector3(20, 20, 0));
	//spawnAsteroid(Vector3(80, 20, 0), Vector3(20, 20, 0));

	//testSprite = new Sprite("images/redbird.png");
	//testSprite->setNumberOfAnimations(1);
	//testSprite->setSpriteFrameSize(125,125);
	//testSprite->addSpriteAnimFrame(0,0,0);
	//testSprite->setPosition(100,200);
	//testSprite->setCenter(125/2,125/2); // center of the sprites origin for rotation //where is the center of the sprite?
	//testSprite->setLayerID(3);
	//testSprite->velocity.set(0, 200, 0);
																		   
	/* add it to our list so we can draw it */
	//this->addSpriteToDrawList(testSprite);


	///* load the background */
	bg = new HorizontalScrollingBackground("images/space.jpg",stateInfo.windowWidth,stateInfo.windowHeight);
	this->addSpriteToDrawList(bg);
	bg->setLayerID(0);
}

/* draw()
 * - this gets called automatically about 30 times per second
 * - this function just draws the sprites 
 */
void Game::draw()
{
	/* pre-draw - setup the rendering */
	PreDraw();

	/* draw - actually render to the screen */
	DrawGame();
	
	/* post-draw - after rendering, setup the next frame */
	PostDraw();
}

/*
 * Pre-Draw() is for setting up things that need to happen in order to draw
 *    the game, i.e. sorting, splitting things into appropriate lists etc.
 */
void Game::PreDraw()
{
	/* clear the screen */
	glViewport(0,0,stateInfo.windowWidth,stateInfo.windowHeight);
	glClearColor(stateInfo.bgClearColor.red, 
				 stateInfo.bgClearColor.green, 
				 stateInfo.bgClearColor.blue, 0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity(); // clear out the transformation matrix
	glEnable(GL_TEXTURE_2D); // turn on texturing

	// if we update our timer it will tell us the elapsed time since the previous 
	// frame that we rendered
	renderingTimer->tick();


	/* sort the sprites by layerID so we draw them in the right order */
	std::sort(spriteListToDraw.begin(), spriteListToDraw.end(), spriteSortingFunction);

}
/* 
 * DrawGame()
 *  - this is the actual drawing of the current frame of the game.
 */
void Game::DrawGame()
{
	/* here is where your drawing stuff goes */
	drawSprites();

	glDisable(GL_TEXTURE_2D);
	drawTestPrimitives();

	//setColor(0.2, 0.2, 0);
	//drawRectangle(true, 0, 0, 300, 400, 0);
	//setColor(0, 1, 0);
	//drawText("TEST", 100, 100);
	/* this makes it actually show up on the screen */
	glutSwapBuffers();
}

/*
 * PostDraw()
 *  - in here you should clean up and set up things for the next frame
 *  - i.e. once I've used my assets, I can change them to set up things for
 *    the next frame, usually just memory management or setting up game state 
 *    boolean values etc.  
 */
void Game::PostDraw()
{
	// nothing here at the moment
}

/* drawSprites()
 * - this function is what actually draws the sprites
 *   onto the screen at their appropriate location
 * - it actually loops through a list of active sprites
 *   and then sorts them by their layerID and then draws them
 * - the sorting has to happen so that you draw from back to front
 *   just like a painter and a canvas.
 */
void Game::drawSprites()
{
	// this just goes through the list of sprites and tells them to draw
	std::vector<Sprite*>::iterator it; 
	for(it=spriteListToDraw.begin(); it != spriteListToDraw.end();it++)
	{
		Sprite *s = (*it);
		s->draw();
	}

}

/* for testing purposes to show you how to use
   the drawing primitives capabilities */
void Game::drawTestPrimitives()
{
	/* draw line */
	if (input.mouseDown)
	{
		setLineWidth(5.f);
		setColor(1, 0, 0);
		drawLine(input.clickX, input.clickY, input.currentX, input.currentY);
		setLineWidth(1.f);
	}
}

/* update()
  - this function is essentially the game loop
    it gets called often and as such you
	don't actually need a "loop" to define the game
	it happens behind the scenes
  - think of this function as one iteration of the game loop
  - if you need to update physics calculations, sprite animation info,
    or sound etc, it goes in here
*/
void Game::update()
{
	// update our clock so we have the delta time since the last update
	updateTimer->tick();
	float theta = spaceShip->theta;
	float rTheta = theta * M_PI / 180; // converting from degrees to radians
	Vector3 force = Vector3(sinf(rTheta), cosf(rTheta), 0) * thrustMultiplier; // sinf and cosf uses radian
	if (!upPressed && !downPressed) {
		force.set(0, 0, 0);
	}
	else if (downPressed) {
		force = force * -1;
	}
	spaceShip->force = force;
	spaceShip->update(updateTimer->getElapsedTimeSeconds());

	if (leftPressed) {
		theta -= rotationMultiplier * updateTimer->getElapsedTimeSeconds();
		//theta--; //same as above but not as accurate
	}
	else if (rightPressed) {
		//theta++; //same as below but not as accurate
		theta += rotationMultiplier * updateTimer->getElapsedTimeSeconds();
	}
	(*spaceShip).theta = theta;
	
	for (Sprite* asteroid : asteroids) {
		asteroid->update(updateTimer->getElapsedTimeSeconds());
	}

	for (Sprite* projectile : projectiles) {
		projectile->update(updateTimer->getElapsedTimeSeconds());
	}

	/* you should probably update all of the sprites in a list just like the drawing */
	/* maybe two lists, one for physics updates and another for sprite animation frame update */
	//for (Sprite * asteroid : asteroids) {
	//	for (Sprite * projectile : projectiles) {
	//		// Check if the asteroid collides with the projectile
	//		// If it does, split the asteroid in half
	//	}
	//}

	// CHECK COLLISIONS AND RESPOND TO THEM IF THEY OCCUR

	for each (Sprite* projectile in projectiles) {
		for each (Sprite* asteroid in asteroids) {
			if (checkCollision(projectile->getCenter() + projectile->position, projectile->sz.width / 2, asteroid->getCenter() + asteroid->position, asteroid->sz.width / 2)) {
				// if small asteroid: delete
				// increase score
				asteroid->setPosition(-999.0f, -999.0f);
				projectile->setPosition(-999.0f, -999.0f);
				// if large asteroid: split
			}
		}
	}
	{

	}

}

//check collision function
bool Game::checkCollision(Vector3 p1, float r1, Vector3 p2, float r2)
{
	float D = sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
	if (D < (r1 + r2))
	{
		return true;
	}
	else {
		return false;
	}
}

/* 
 * addSpriteToDrawList()
 * - this function simply pushes the sprite to the end of the list
 */
void Game::addSpriteToDrawList(Sprite *s)
{
	if(s)
	{
		/* push the sprite to the back of the list */
		this->spriteListToDraw.push_back(s);
	}
}


/*************************************************/
/* INPUT - keyboard/mouse functions below        */
/*************************************************/
/* keyboardDown()
   - this gets called when you press a key down
   - you are given the key that was pressed
     and where the (x,y) location of the mouse is when pressed
*/
void Game::keyboardDown(unsigned char key, int mouseX, int mouseY)
{
	std::cout << key << std::endl;
	switch(key)
	{
	case 'a': //left arrow
		leftPressed = true;
		break;
	case 'w': //up arrow
		upPressed = true;
		break;
	case 'd': //right arrow
		rightPressed = true;
		break;
	case 's': //down arrow
		downPressed = true;
		break;
	case 'r':  // reset position, velocity, and force
	/*	testSprite->position.set(100, 100, 0);
		testSprite->velocity.set(0, 0, 0);
		testSprite->acceleration.set(0, 0, 0);
		testSprite->force.set(0, 0, 0);*/
		break;
	case 32: // the space bar
		{
			Vector3 position = spaceShip->position;
			float rTheta = spaceShip->theta * M_PI / 180;
			spawnProjectile(position, Vector3(sinf(rTheta), cosf(rTheta), 0) * 500);
		}
		break;
	case 27: // the escape key
	case 'q': // the 'q' key
		exit(1);
		break;
	}
}
/* keyboardUp()
   - this gets called when you lift a key up
   - you are given the key that was pressed
     and where the (x,y) location of the mouse is when pressed
*/
void Game::keyboardUp(unsigned char key, int mouseX, int mouseY)
{
	switch(key)
	{
	case 'a': //left arrow
		leftPressed = false;
		break;
	case 'w': //up arrow
		upPressed = false;
		break;
	case 'd': //right arrow
		rightPressed = false;
		break;
	case 's': //down arrow
		downPressed = false;
		break;
	case 32: // the space bar
		break;
	case 27: // the escape key
	case 'q': // the 'q' key
		exit(1);
		break;
	}
}

/*
 * mouseClicked
 * - this function is called when the mouse is clicked and it handles the 
 *   input state managment
 */
void Game::mouseClicked(int button, int state, int x, int y)
{
	if(state == GLUT_DOWN) 
	{
		input.mouseDown = true;
		input.clickX = x*stateInfo.ratioWidth;
		input.clickY = (stateInfo.windowHeight-y)*stateInfo.ratioHeight;
		input.button = button;

		switch(button)
		{
		case GLUT_LEFT_BUTTON:
			
			break;
		case GLUT_RIGHT_BUTTON:
		
			break;
		case GLUT_MIDDLE_BUTTON:
			break;
		}
	}
	else
	{
	
		input.mouseDown = false;
	}

}

/*
 * mouseMoved(x,y)
 * - this occurs only when the mouse is pressed down
 *   and the mouse has moved.  you are given the x,y locations
 *   in window coordinates (from the top left corner) and thus 
 *   must be converted to screen coordinates using the screen to window pixels ratio
 *   and the y must be flipped to make the bottom left corner the origin.
 */
void Game::mouseMoved(int x, int y)
{
	/* convert from window to screen pixel coordinates */
	input.currentX = x*stateInfo.ratioWidth;
	input.currentY = (stateInfo.windowHeight-y)*stateInfo.ratioHeight;
	
	if(input.mouseDown)
	{
		if(input.button == GLUT_LEFT_BUTTON)
		{
			// nothing yet
		}
	}
}

void Game::spawnAsteroid(Vector3 position, Vector3 velocity, bool isBig)
{
	//this function is to spawn an asteroid
	Sprite* asteroid = new Sprite("images/asteroid2.png"); //need to delete later on = new Sprite("images/asteroid2.png");

	asteroid->setNumberOfAnimations(1);
	if (isBig)
	{
		//asteroid = new Sprite("images/asteroid2.png");

		asteroid->setSpriteFrameSize(150, 150);
		asteroid->setCenter(75, 75); // center of the sprites origin for rotation

	}
	else {
		//asteroid = new Sprite("images/asteroid.png");

		asteroid->setSpriteFrameSize(75, 75);
		asteroid->setCenter(75 / 2, 75 / 2); // center of the sprites origin for rotation

	}
	asteroid->addSpriteAnimFrame(0, 0, 0);
	asteroid->setPosition(position.x, position.y);
	asteroid->setLayerID(3);
	asteroid->velocity = velocity;
	asteroids.push_back(asteroid);
	addSpriteToDrawList(asteroid);
}

void Game::spawnProjectile(Vector3 position, Vector3 velocity)
{
	Sprite* projectile = new Sprite("images/projectile.png");
	projectile->setNumberOfAnimations(1);
	projectile->setSpriteFrameSize(50, 50);
	projectile->addSpriteAnimFrame(0, 0, 0);
	projectile->setPosition(position.x, position.y);
	projectile->setCenter(50 / 2, 50 / 2); // center of the sprites origin for rotation
	projectile->setLayerID(2);
	projectile->velocity = velocity;
	projectiles.push_back(projectile);
	addSpriteToDrawList(projectile);

}
