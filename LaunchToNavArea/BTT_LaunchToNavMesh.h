// Copyright (C) Mortal Enemies 2022, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_LaunchToNavMesh.generated.h"

class UNavigationSystemV1;
/**
 * A Behaviour Tree task implemented to launch the pawn onto a nearby nav mesh point.
 * Used in case our pawn gets displaced from the nav mesh, either through it's own launch method, actor teleport, or unpredicted physics interations.
 */
UCLASS()
class TENMINUTESTOBREATHE_API UBTT_LaunchToNavMesh : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTT_LaunchToNavMesh();

	// Maximum distance at which to find a point
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Bounds = 1000.f;

	// Time that the pawn should take to launch from current location to generated point
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AirTime = 0.5f;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	// Caches references to own pawn, world, and world's navigation system
	APawn* Pawn;
	UWorld* World;
	UNavigationSystemV1* NavSystem;

	// Cache values for target location and time at which launch started
	FVector Destination;
	float LaunchTime;

	// Return a point on the nav mesh within Bounds distance of the pawn
	void GetTargetLocation();

	// Calculate required velocity and launch the pawn
	void LaunchAtDestination();
};
