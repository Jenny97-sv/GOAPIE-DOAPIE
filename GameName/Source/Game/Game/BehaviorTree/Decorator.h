#pragma once

#include "BehaviorTree.h"
#include <functional>

class IEvaluable
{
public:
    virtual ~IEvaluable() = default;
    virtual eNodeStatus Evaluate(const float deltaTime) = 0;
};


class DecoratorNode : public BehaviorTreeNode, public IEvaluable
{
protected:
    BehaviorTreeNodePtr child;
public:
    void SetChild(BehaviorTreeNodePtr c)
    {
        child = std::move(c);
    }
};

// Change ComparisonDecorator to accept std::function<T()> instead of raw T
template<typename T>
class ComparisonDecorator : public DecoratorNode
{
public:
    ComparisonDecorator(std::function<T()> lhsGetter, std::function<T()> rhsGetter, std::function<bool(T, T)> comp)
        : lhsGetter(lhsGetter), rhsGetter(rhsGetter), comp(comp) {}

    eNodeStatus Evaluate(const float deltaTime) override
    {
        if (comp(lhsGetter(), rhsGetter()))
            return eNodeStatus::eSuccess;
        else
            return eNodeStatus::eFailure;
    }

    eNodeStatus TryAndTick(const float deltaTime) override
    {
        if (!child)
            return eNodeStatus::eFailure;

        if (Evaluate(deltaTime) == eNodeStatus::eSuccess)
            return child->TryAndTick(deltaTime);

        return eNodeStatus::eFailure;
    }

private:
    std::function<T()> lhsGetter;
    std::function<T()> rhsGetter;
    std::function<bool(T, T)> comp;
};

//using CompareGreaterEqual = std::greater_equal_to;