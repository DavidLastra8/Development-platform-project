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
	idleAnim.speed = 0.1f;
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
	texture = app->tex->Load("Assets/Textures/player.png");
	

	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 16, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");

	return true;
}

//make a SetPosition function
void Player::SetPosition(int x, int y) {
	position.x = x;
	position.y = y;
	b2Vec2 newPos(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	pbody->body->SetTransform(newPos, pbody->body->GetAngle());
}

bool Player::LoadState(pugi::xml_node& node)
{
	pugi::xml_node playerNode = node.child("player");
	if (playerNode)
	{
		position.x = playerNode.attribute("x").as_int();
		position.y = playerNode.attribute("y").as_int();
	}

	return true;
}

bool Player::SaveState(pugi::xml_node& node) 
{
	pugi::xml_node playerNode = node.append_child("player");
	playerNode.append_attribute("x").set_value(position.x);
	playerNode.append_attribute("y").set_value(position.y);

	return true;
}


bool Player::Update(float dt)
{
	if (isAlive)
	{
		currentAnimation = &idleAnim;
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

		// Horizontal movement
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			currentVel.x = -8.0f; // Leftward
		}
		else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			currentVel.x = 8.0f; // Rightward
		}
		else {
			currentVel.x = 0.0f; // Stop horizontal movement when no keys are pressed
		}
		
		// Apply the updated horizontal velocity
		pbody->body->SetLinearVelocity(b2Vec2(currentVel.x, pbody->body->GetLinearVelocity().y));

		// Jumping
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !IsJumping) {

			pbody->body->ApplyLinearImpulse(jumpImpulse, pbody->body->GetWorldCenter(), true);
			IsJumping = true;
		}
	}
	if (isAlive == false) {
		SetPosition(400, 1102);
		isAlive = true;
	 }



	//we don't want this for now, Instead of directly setting the linear velocity for movement, you can apply forces or impulses in the horizontal direction as well. This will allow both jumping and lateral movement to coexist.

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
		


		if (GodMode == false) {
			GodMode = true;
		}
		else if (GodMode == true) {
			GodMode = false;
		}

		

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
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 50;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 58;

	app->render->DrawTexture(texture, position.x, position.y);

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
		isAlive = false;
		pbody->body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
		
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		//player not movable
		pbody->body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
		break;
	}
}