#include "Coin.h"
#include "Item.h"
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
}

Coin::~Coin() {
    CleanUp();
}

bool Coin::Awake() {
    // Initialization code
    return true;
}

bool Coin::Start() {
    // Code to start coin behavior
    return true;
}

bool Coin::Update(float dt) {
    if (collected) {
        // Update behavior after being collected, if necessary
    }
    // Regular update code
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
