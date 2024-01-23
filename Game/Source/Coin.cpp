#include "Coin.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
Coin::Coin() : Entity(EntityType::COIN), collected(false) {
    // Initialize coin properties here
    name.Create("coin");
}

Coin::~Coin() {
    CleanUp();
}

bool Coin::Awake() {
    // Initialization code
    position.x = parameters.attribute("x").as_int();
    position.y = parameters.attribute("y").as_int();
    texturePath = parameters.attribute("texturepath").as_string();
    return true;
}

bool Coin::Start() {
    // Code to start coin behavior
    texture = app->tex->Load(texturePath);
    pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 16, bodyType::DYNAMIC);
    pbody->listener = this;
    pbody->ctype = ColliderType::COIN;
    return true;
}

bool Coin::Update(float dt) {
    if (collected) {
        // Update behavior after being collected, if necessary
    }
    // Regular update code
    app->render->DrawTexture(texture, position.x, position.y);
    return true;
}

bool Coin::CleanUp() {
    // Cleanup code
    return true;
}

void Coin::Collected() {
    collected = true;
    // Additional logic when the coin is collected
}

void Item::SetPosition(int x, int y) {
    position.x = x;
    position.y = y;
    b2Vec2 newPos(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
    pbody->body->SetTransform(newPos, pbody->body->GetAngle());
}
