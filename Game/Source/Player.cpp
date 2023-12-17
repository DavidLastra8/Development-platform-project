#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Animation.h"
#include "Map.h"



Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
	idleAnim.PushBack({ 27, 21, 42, 53 });
	idleAnim.PushBack({ 123, 21, 42, 53 });
	idleAnim.PushBack({ 219, 21, 42, 53 });
	idleAnim.PushBack({ 315, 21, 42, 53 });
	idleAnim.PushBack({ 411, 21, 42, 53 });
	idleAnim.PushBack({ 507, 21, 42, 53 });
	idleAnim.loop = true;
	idleAnim.speed = 0.003f;

	idleLeftAnim.PushBack({ 1081, 19, 42, 53 });
	idleLeftAnim.PushBack({ 987, 19, 42, 53 });
	idleLeftAnim.PushBack({ 888, 19, 42, 53 });
	idleLeftAnim.PushBack({ 791, 19, 42, 53 });
	idleLeftAnim.PushBack({ 699, 19, 42, 53 });
	idleLeftAnim.PushBack({ 604, 19, 42, 53 });
	idleLeftAnim.loop = true;
	idleLeftAnim.speed = 0.003f;

	walkRightAnim.PushBack({ 24, 112, 42, 53 });
	walkRightAnim.PushBack({ 116, 112, 42, 53 });
	walkRightAnim.PushBack({ 208, 112, 42, 53 });
	walkRightAnim.PushBack({ 300, 112, 42, 53 });
	walkRightAnim.PushBack({ 392, 112, 42, 53 });
	walkRightAnim.PushBack({ 484, 112, 42, 53 });
	walkRightAnim.PushBack({ 22, 204, 42, 53 });
	walkRightAnim.PushBack({ 118, 208, 42, 53 });
	walkRightAnim.loop = true;
	walkRightAnim.speed = 0.003f;

	walkLeftAnim.PushBack({ 1083, 117, 42, 53 });
	walkLeftAnim.PushBack({ 988, 117, 42, 53 });
	walkLeftAnim.PushBack({ 888, 117, 42, 53 });
	walkLeftAnim.PushBack({ 796, 117, 42, 53 });
	walkLeftAnim.PushBack({ 697, 117, 42, 53 });
	walkLeftAnim.PushBack({ 603, 117, 42, 53 });
	walkLeftAnim.PushBack({ 1083, 205, 42, 53 });
	walkLeftAnim.PushBack({ 990, 209, 42, 53 });
	walkLeftAnim.loop = true;
	walkLeftAnim.speed = 0.003f;
}

Player::~Player() {

}

bool Player::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load("Assets/Textures/player-Sheet-animations.png");
	

	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 16, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");
	jumpFxId = app->audio->LoadFx("Assets/Audio/Fx/jump2.wav");

	endLevelFxId = app->audio->LoadFx("Assets/Audio/Fx/Retro Success Melody 02 - choir soprano.wav");
	endLevelSoundPlayed = false;
	
	return true;
}

//make a SetPosition function
void Player::SetPosition(int x, int y) {
	position.x = x;
	position.y = y;
	b2Vec2 newPos(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	pbody->body->SetTransform(newPos, pbody->body->GetAngle());
}

int Player::GetTilex() {
	return (position.x + 20) / 32;
}

int Player::GetTiley() {
	return (position.y + 20) / 32;
}


bool Player::Update(float dt)
{
	if (isAlive)
	{
		

		
		/*vely = -GRAVITY_Y;
		velx = 0;*/
		b2Vec2 currentVel = pbody->body->GetLinearVelocity();
		b2Vec2 jumpImpulse(0.0f, -4.1f); // Upward
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
			//
		}
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
			//
		}

		bool movingLeft = app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT;
		bool movingRight = app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT;

		// Horizontal movement
		if (movingLeft)
		{
			currentVel.x = -8.0f; // Leftward
			currentAnimation = &walkLeftAnim;
		}
		else if (movingRight)
		{
			currentVel.x = 8.0f; // Rightward
			currentAnimation = &walkRightAnim;
		}
		else
		{
			currentVel.x = 0.0f; // Stop horizontal movement when no keys are pressed
			// Determine which idle animation to use based on the last direction
			currentAnimation = (lastDirection == LEFT) ? &idleLeftAnim : &idleAnim;
		}

		// Update lastDirection if moving
		if (movingLeft)
		{
			lastDirection = LEFT;
		}
		else if (movingRight)
		{
			lastDirection = RIGHT;
		}
		
		
		// Apply the updated horizontal velocity
		pbody->body->SetLinearVelocity(b2Vec2(currentVel.x, pbody->body->GetLinearVelocity().y));

		// Advance the current frame of the animation
        // The GetCurrentFrame method should advance the frame based on dt
		currentAnimation->GetCurrentFrame(dt);

		// Jumping
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !IsJumping) {

			pbody->body->ApplyLinearImpulse(jumpImpulse, pbody->body->GetWorldCenter(), true);
			IsJumping = true;

			// Play the jumping sound effect
			app->audio->PlayFx(jumpFxId);
		}
	}
	/*if (isAlive == false) {
		SetPosition(400, 1102);
		isAlive = true;
	}*/



	//we don't want this for now, Instead of directly setting the linear velocity for movement, you can apply forces or impulses in the horizontal direction as well. This will allow both jumping and lateral movement to coexist.

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
		


		if (GodMode == false) {
			GodMode = true;
		}
		else if (GodMode == true) {
			GodMode = false;
		}

		

	}

	//Level complete Condition
	if (position.x >= endLevelX && position.y >= endLevelY && !endLevelSoundPlayed)
	{
		// Play the end-level sound effect
		app->audio->PlayFx(endLevelFxId);
		endLevelSoundPlayed = true;  // Set the flag to true
	}

	//if pressed F6, set the endLevelSoundPlayed to false
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) {
		endLevelSoundPlayed = false;
	}

	if (GodMode == true) {
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
			pbody->body->ApplyForceToCenter(b2Vec2(0.0f, -10.0f), true);
		}
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
			pbody->body->ApplyForceToCenter(b2Vec2(0.0f, 10.0f), true);
		}
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			pbody->body->ApplyForceToCenter(b2Vec2(-30.0f, 0.0f), true);
		}
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			pbody->body->ApplyForceToCenter(b2Vec2(30.0f, 0.0f), true);
		}
		//make gravity stop working
		pbody->body->SetGravityScale(0.0f);
		//don't let the player jump, when press space bar do nothing
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
			//
		}
		//player ignore isAlive variable
		isAlive = true;
		

	}
	else if (GodMode == false) {
		//make gravity work again
		pbody->body->SetGravityScale(1.0f);
	}

	//If pressed F3, Set the player postion back to start
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
		//set position to start
		SetPosition(400, 1102);
		isAlive = true;
	}
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		SetPosition(400, 1102);
		isAlive = true;
	}
	

	

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 23;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 35;

	currentAnimation = &idleAnim;
	SDL_Rect rect = currentAnimation->GetCurrentFrame(dt);
	app->render->DrawTexture(texture, position.x, position.y,&rect);

	
	
	return true;
}

bool Player::CleanUp()
{
	app->tex->UnLoad(texture);


	return true;
}

void Player::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		app->audio->PlayFx(pickCoinFxId);
		break;
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		if (IsJumping) {
			IsJumping = false;
		}
		break;
	case ColliderType:: DEATH:
		LOG("Collision DEATH");
		if (GodMode = true)
		{
			isAlive = true;
		}
		else if (GodMode = false)
		{
			isAlive = false;
		}
		
		pbody->body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
		
		
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		//player not movable
		pbody->body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
		break;
	}
}