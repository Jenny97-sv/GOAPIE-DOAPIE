#include "Food.h"
#include "Random.h"
#include "Boidy.h"

#include "HungerComponent.h"
#include "FriendshipComponent.h"

void Food::OnEaten(Boidy* boid)
{
    HungerComponent* hu = boid->GetComponent<HungerComponent>();
    FriendshipComponent* fri = boid->GetComponent<FriendshipComponent>();

    float normTasty = tastiness / maxTasty;
    float normHealthy = healthiness / maxHealthy;
    float normHunger = hu->HungerPercent();

    float imbalance = fabs(normTasty - normHealthy);

    float enjoyment = 0.6f * normTasty + 0.4f * normHealthy;

    float hungerInfluence = 0.5f + 0.5f * normHunger;

    float friendshipDelta = (enjoyment - imbalance * 0.8f) * hungerInfluence;

    friendshipDelta += Random::GetRandomNumber(-0.1f, 0.1f);

    friendshipDelta = std::roundf(friendshipDelta * 4.0f - 2.0f);
    friendshipDelta = std::clamp(friendshipDelta, -3.0f, 3.0f);

    if (friendshipDelta > 0)
        fri->Add(friendshipDelta);
    else
        fri->Subtract(friendshipDelta);

    float newHunger = (normHealthy / normHunger) - (normTasty / normHunger); // if it's tasty, they want more, if it's healthy they want less...
    newHunger *= 10;

    hu->AdjustHunger(newHunger); 

    std::cout << "Friendship = " << friendshipDelta
        << " | New friendship = " << fri->GetCurrent()
        << " | New hunger = " << hu->GetHunger() << std::endl;

    isActive = false;
}
