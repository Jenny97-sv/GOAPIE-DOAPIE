#include "BoidyAIComponent.h"
#include "GameObject.h"
#include "Boidy.h"
#include "Food.h"

#include "TransformComponent.h"
#include "StatsComponent.h"
#include "FriendshipComponent.h"
#include "HungerComponent.h"
#include "SpriteRendererComponent.h"


void BoidyAIComponent::Init()
{
	transformComponent = owner->GetComponent<TransformComponent>();
	statsComponent = owner->GetComponent<StatsComponent>();
	friendshipComponent = owner->GetComponent<FriendshipComponent>();
	hungerComponent = owner->GetComponent<HungerComponent>();
	spriteRendererComponent = owner->GetComponent<SpriteRendererComponent>();
	boidy = dynamic_cast<Boidy*>(owner);
	if (!boidy)
	{
		std::cout << "Couldn't cast BOIDY ;(" << std::endl;
	}

	if (!BuildBehaviorTree())
	{
		std::cout << "No BT!" << std::endl;
	}
	if (!BuildGOAP())
	{
		std::cout << "No GOAP!" << std::endl;
	}

}

void BoidyAIComponent::Update(const float deltaTime)
{
	UpdateWorldState(deltaTime);

	if (currentPlan.empty())
	{
		Goal* bestGoal = GOAPPlanner::ChooseGoal(goals, worldState);
		currentPlan = GOAPPlanner::MakeGoodPlan(worldState, actions, *bestGoal);
		//std::cout << currentPlan.front()->name << std::endl;
	}

	if (!currentPlan.empty())
	{
		GoalAction* action = currentPlan.front();
		if (ExecuteBehavior(action, action->behavior, deltaTime))
			currentPlan.pop_front(); // Behavior finished
	}

	if (shouldEat)
	{
		if (!food || !food->isActive)
		{
			food = nullptr;
			shouldEat = false;
		}
	}
}

void BoidyAIComponent::HandleMessage(const Message msg)
{
	switch (msg.msg)
	{
	case eMessage::ePlayerDashed:
		friendshipComponent->Subtract(1);
		shouldEat = false;
		break;

	case eMessage::ePlayerSmiled:
		friendshipComponent->Add(1);
		shouldEat = false;
		break;

	case eMessage::ePlayerPet:
		if (friendshipComponent->HalfOrLess())
			friendshipComponent->Subtract(1);
		else
			friendshipComponent->Add(1);

		shouldEat = false;
		break;

	case eMessage::ePlayerFeed:
		food = static_cast<Food*>(msg.data);
		if (!food)
			return;
		float normTaste = (food->tastiness / food->maxTasty);
		float desireToEat = 1.0f - hungerComponent->HungerPercent();

		if (normTaste >= 0.5f * desireToEat)
		{
			friendshipComponent->Add(1);
			//std::cout << "TASTY!!!!!!!!!!!" << std::endl;
			shouldEat = true;
		}
		else
		{
			friendshipComponent->Subtract(1);
			shouldEat = false;
			//std::cout << "EEEEEWWWWWWWW" << std::endl;
		}
		//std::cout << "NormTaste = " << normTaste << "\nNormHung = " << desireToEat << std::endl;
		break;
	}
	float curFriend = friendshipComponent->GetCurrent();

	if (curFriend < 4)
	{
		boidy->shouldCircleAroundPlayer = true;
		boidy->hasTakenDamage = true;
		boidy->hasFormationSlot = false;
	}
	else
		boidy->hasTakenDamage = false;

	UpdateFriendColor(curFriend);


	//std::cout << spriteRendererComponent->sprite.GetColor().x << ", " << spriteRendererComponent->sprite.GetColor().y << ", " << spriteRendererComponent->sprite.GetColor().z << std::endl;
	//std::cout << "HasTakenDamage = " << hasTakenDamage << std::endl;
	//std::cout << "IsAlerted = " << isAlerted << std::endl;
	//std::cout << "IsLookingForPlayer = " << isLookingForPlayer << std::endl;
	//std::cout << "ShouldCircle = " << shouldCircleAroundPlayer << std::endl;
	std::cout << curFriend << std::endl;
}

bool BoidyAIComponent::BuildGOAP()
{
	if (!goals.empty())
	{
		std::cout << "GOAP already exists!" << std::endl;
		return false;
	}
	if (!actions.empty())
	{
		std::cout << "Actions already exist!" << std::endl;
		return false;
	}

	// Goal: Befriend
	{
		Goal& befriend = goals.emplace_back();
		befriend.AddCondition(eWorldKey::eIsAlerted, 1);
		befriend.name = "Befriend";
	}

	// Goal: Roam
	{
		Goal& roam = goals.emplace_back();
		roam.AddCondition(eWorldKey::eIsAlerted, 0);
		roam.AddCondition(eWorldKey::eHasTakenDamage, 0);
		roam.name = "Roam";
	}

	// Goal: Attack
	{
		Goal& attack = goals.emplace_back();
		attack.AddCondition(eWorldKey::eIsAlerted, 1);
		attack.AddCondition(eWorldKey::eHasTakenDamage, 1);
		attack.name = "Attack";
	}

	// Action: Befriend
	{
		GoalAction& act = actions.emplace_back();
		act.cost = 2;
		act.effectiveness = 1;
		act.behavior = eBoidBehaviorType::eSpiral;
		act.name = "Befriend";

		act.AddEffect(eWorldKey::eIsLookingForPlayer, 1);
		act.AddPrecondition(eWorldKey::eEat, 0);
		act.AddPrecondition(eWorldKey::eHasTakenDamage, 0);
	}

	// Action: Eat
	{
		GoalAction& act = actions.emplace_back();
		act.cost = 1;
		act.effectiveness = 0.5f;
		act.behavior = eBoidBehaviorType::eEat;
		act.name = "Eat";

		act.AddEffect(eWorldKey::eEat, 1);
		act.AddPrecondition(eWorldKey::eEat, 1);
	}

	// Action: Free roam
	{
		GoalAction& act = actions.emplace_back();
		act.cost = 1;
		act.effectiveness = 0.5f;
		act.behavior = eBoidBehaviorType::eFlock;
		act.name = "Flock";

		act.AddEffect(eWorldKey::eIsAlerted, 0);
		act.AddPrecondition(eWorldKey::eHasTakenDamage, 0);
	}

	// Action: Army 
	{
		GoalAction& act = actions.emplace_back();
		act.cost = 4;
		act.effectiveness = 5;
		act.behavior = eBoidBehaviorType::eArmy;
		act.name = "Army";

		act.AddEffect(eWorldKey::eIsAlerted, 1);
		act.AddPrecondition(eWorldKey::eHasTakenDamage, 1);
		act.AddPrecondition(eWorldKey::eCircleAroundPlayer, 0);
		act.AddPrecondition(eWorldKey::eEat, 0);
	}

	// Action: Attack
	{
		GoalAction& act = actions.emplace_back();
		act.cost = 3;
		act.effectiveness = 5;
		act.behavior = eBoidBehaviorType::eAttack;
		act.name = "Attack";

		act.AddEffect(eWorldKey::eIsAlerted, 1);
		act.AddEffect(eWorldKey::eHasFormationSlot, 1);
		act.AddEffect(eWorldKey::eCircleAroundPlayer, 1);

		act.AddPrecondition(eWorldKey::eHasTakenDamage, 1);
		act.AddPrecondition(eWorldKey::eEat, 0);
		act.AddPrecondition(eWorldKey::eHasFormationSlot, 0);
	}

	return true;
}


bool BoidyAIComponent::BuildBehaviorTree()
{
	if (behaviorTree)
	{
		std::cout << "Behavior tree already exists in Boidy" << std::endl;
		return true;
	}

	if (blackboard)
	{
		std::cout << "Blackboard already set" << std::endl;
		return true;
	}

	blackboard = std::make_unique<Blackboard>();
	blackboard->SetValue("Host", boidy);
	//blackboard->SetValue("Position", WorldPosition());
	//blackboard->SetValue("Rotation", WorldRotation());
	//blackboard->SetValue("Speed", speed);

	blackboard->SetValue("Position", transformComponent->position);
	blackboard->SetValue("Rotation", transformComponent->rotation);
	blackboard->SetValue("Speed", statsComponent->stats.Speed);

	behaviorTree = std::make_unique<BehaviorTree>();

	/*
	//std::unique_ptr<SequenceNode> mainSequence = std::make_unique<SequenceNode>();

	//std::unique_ptr<ActionNode> walkAction = std::make_unique<ActionNode>(std::make_unique<BoidyWalk>(this));
	//std::unique_ptr<ComparisonDecorator<bool>> walkCondition =
	//	std::make_unique<ComparisonDecorator<bool>>(
	//		[this]() { return this->isAlerted; },    // LHS getter
	//		[]() { return false; },                  // RHS getter (constant false)
	//		std::equal_to<bool>()                    // Comparator
	//	);

	//std::unique_ptr<ActionNode> idleAction = std::make_unique<ActionNode>(std::make_unique<BoidyIdle>(this));
	//walkCondition->SetChild(std::move(walkAction));
	//mainSequence->AddChild(std::move(walkCondition));
	//mainSequence->AddChild(std::move(idleAction));*/


	//std::unique_ptr<SelectorNode> selector = std::make_unique<SelectorNode>();

	armyAction = (std::make_unique<BoidyArmy>(blackboard.get()));
	//std::unique_ptr<ComparisonDecorator<bool>> armyCondition =
	//	std::make_unique<ComparisonDecorator<bool>>(
	//		[this]() {return this->hasFormationSlot; },
	//		[]() {return true; },
	//		std::equal_to<bool>()
	//	);
	//armyCondition->SetChild(std::move(armyAction));

	//selector->AddChild(std::move(armyCondition));

	flockAction = (std::make_unique<BoidyFlock>(blackboard.get()));
	//std::unique_ptr<ComparisonDecorator<bool>> flockCondition =
	//	std::make_unique<ComparisonDecorator<bool>>(
	//		[this]() {return this->isAlerted; },
	//		[]() {return false; },
	//		std::equal_to<bool>()
	//	);
	//flockCondition->SetChild(std::move(flockAction));
	//selector->AddChild(std::move(flockCondition));


	spiralAction = (std::make_unique<BoidySpiral>(blackboard.get()));

	//selector->AddChild(std::move(spiralAction));

	//behaviorTree->SetRoot(std::move(selector));

	eatAction = std::make_unique<BoidyEat>(blackboard.get());
	attackAction = std::make_unique<BoidyAttack>(blackboard.get());

	return true;
}

//void BoidyAIComponent::UpdateWorldState(float dt)
//{
//	//worldState["HP"] = stats.HP;
//	worldState["HP"] = statsComponent->stats.HP;
//	worldState["HasTakenDamage"] = boidy->hasTakenDamage;
//	worldState["IsAlerted"] = boidy->isAlerted;
//	worldState["IsLookingForPlayer"] = boidy->isLookingForPlayer;
//	//worldState["FriendValue"] = friendshipMeter;
//	worldState["FriendValue"] = friendshipComponent->GetCurrent();
//	worldState["Eat"] = shouldEat;
//	worldState["CircleAroundPlayer"] = boidy->shouldCircleAroundPlayer;
//	worldState["HasFormationSlot"] = boidy->hasFormationSlot;
//}

void BoidyAIComponent::UpdateWorldState(float dt)
{
	SetWorldValue(worldState, eWorldKey::eHP, statsComponent->stats.HP);
	SetWorldValue(worldState, eWorldKey::eHasTakenDamage, boidy->hasTakenDamage);
	SetWorldValue(worldState, eWorldKey::eIsAlerted, boidy->isAlerted);
	SetWorldValue(worldState, eWorldKey::eIsLookingForPlayer, boidy->isLookingForPlayer);
	SetWorldValue(worldState, eWorldKey::eFriendValue, friendshipComponent->GetCurrent());
	SetWorldValue(worldState, eWorldKey::eEat, shouldEat);
	SetWorldValue(worldState, eWorldKey::eCircleAroundPlayer, boidy->shouldCircleAroundPlayer);
	SetWorldValue(worldState, eWorldKey::eHasFormationSlot, boidy->hasFormationSlot);
}



bool BoidyAIComponent::ExecuteBehavior(GoalAction* action, eBoidBehaviorType type, float dt)
{
	eNodeStatus status = eNodeStatus::eInvalid;
	bool interrupt = !(action->ArePreconditionsMet(worldState));

	//status = behaviorTree->TickAction(action, dt, interrupt);
	switch (type)
	{
	case eBoidBehaviorType::eArmy:
		status = behaviorTree->TickAction(armyAction.get(), dt, interrupt);
		break;

	case eBoidBehaviorType::eFlock:
		status = behaviorTree->TickAction(flockAction.get(), dt, interrupt);
		break;

	case eBoidBehaviorType::eSpiral:
		status = behaviorTree->TickAction(spiralAction.get(), dt, interrupt);
		break;

	case eBoidBehaviorType::eEat:
		status = behaviorTree->TickAction(eatAction.get(), dt, interrupt);
		break;

	case eBoidBehaviorType::eAttack:
		status = behaviorTree->TickAction(attackAction.get(), dt, interrupt);
		break;

	case eBoidBehaviorType::eNone:
		return true;

	default:
		return true;
	}

	if (interrupt)
		return true;

	switch (status)
	{
	case eNodeStatus::eRunning:	return false;
	default: return true; // Success, failure or invalid
	}
}

void BoidyAIComponent::UpdateFriendColor(const float friendshipMeter)
{

	//Vector4f hostile = { 1, 0, 0, 1 };
	//Vector4f neutral = { 1, 1, 1, 1 };
	//Vector4f friendly = { 0, 1, 0, 1 };

	Vector4f finalColor;

	if (friendshipMeter <= 5)
	{
		float t = friendshipMeter / 5.0f;  // 0–5 → 0–1
		finalColor = Vector4f::Lerp(hostileColor, normalColor, t);
	}
	else
	{
		float t = (friendshipMeter - 5) / 5.0f;  // 5–10 → 0–1
		finalColor = Vector4f::Lerp(normalColor, friendColor, t);
	}

	spriteRendererComponent->SwitchColor(finalColor);
	//switch (static_cast<int>(friendshipMeter))
	//{
	//case 0:
	//case 1:
	//case 2:
	//case 3:
	//	spriteRendererComponent->SwitchColor({ 1,0,0,1 });
	//	break;
	//case 4:
	//case 5:
	//case 6:
	//case 7:
	//	spriteRendererComponent->SwitchColor({ 1,1,1,1 });
	//	break;

	//case 8:
	//case 9:
	//case 10:
	//	spriteRendererComponent->SwitchColor({ 0,1,1,1 });
	//}
}
