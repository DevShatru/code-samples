// Copyright (C) Mortal Enemies 2022, All Rights Reserved


#include "Creature/Tasks/BTT_LaunchToNavMesh.h"

#include "AIController.h"
#include "fmod_studio.hpp"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"

// Set task to tick so we can check for completion
UBTT_LaunchToNavMesh::UBTT_LaunchToNavMesh()
{
	NodeName = "LaunchToNavMesh";
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_LaunchToNavMesh::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Early failure returns if we can't find any of: World, Navigation System, own Controller, or own Pawn

	World = GetWorld();
	if(!World) return EBTNodeResult::Failed;
	
	NavSystem = Cast<UNavigationSystemV1>(World->GetNavigationSystem());
	if(!NavSystem) return EBTNodeResult::Failed;

	const AAIController* Controller = Cast<AAIController>(OwnerComp.GetOwner());
	if(!Controller) return EBTNodeResult::Failed;

	Pawn = Controller->GetPawn();
	if(!Pawn) return EBTNodeResult::Failed;

	return EBTNodeResult::InProgress;

}

void UBTT_LaunchToNavMesh::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// If we can project the pawn's current location onto the nav mesh
	FNavLocation OutLocation;
	if(NavSystem->ProjectPointToNavigation(Pawn->GetActorLocation(), OutLocation))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	// If not, check if time since last launch exceeds AirTime
	if(World->GetTimeSeconds() > LaunchTime + AirTime)
	{
		// Find a new target location and launch the pawn
		GetTargetLocation();
		LaunchAtDestination();
	}
}

void UBTT_LaunchToNavMesh::GetTargetLocation()
{
	// Find a point in navigable radius
	FNavLocation OutLocation;
	const bool Success = NavSystem->GetRandomPointInNavigableRadius(Pawn->GetActorLocation(), Bounds, OutLocation);

	// On success, cache the vector location
	if(Success) Destination = OutLocation.Location;
}


void UBTT_LaunchToNavMesh::LaunchAtDestination()
{
	// Early return if our cached variable has the default value
	if (Destination == FVector::ZeroVector) return;

	// Cache launch time
	LaunchTime = World->GetTimeSeconds();

	// Calculation for required velocity to get from current location to destination
	// For horizontal: velocity = horizontalDisplacement / time
	// For vertical: velocity =  ( verticalDisplacement - 0.5 * gravity * time ^ 2 ) / time
	const FVector OwnLocation = Pawn->GetActorLocation();
	FVector LaunchVelocity = FVector{ Destination.X - OwnLocation.X, Destination.Y - OwnLocation.Y, 0.f } / AirTime;
	LaunchVelocity.Z = ( Destination.Z - OwnLocation.Z - 0.5f * Pawn->GetMovementComponent()->GetGravityZ() * powf(AirTime, 2) ) / AirTime;

	// Launch character and override existing velocity
	Cast<ACharacter>(Pawn)->LaunchCharacter(LaunchVelocity, true, true);
}
