// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Ai/BTT_ChargeAttack.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include"Animations/BossAnimationInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/EEnemyState.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBTT_ChargeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	bool bIsReadytoCharge =  OwnerComp.GetBlackboardComponent()->GetValueAsBool(TEXT("IsReadyToCharge"));
	

	if (bIsReadytoCharge) {
		OwnerComp.GetBlackboardComponent()
			->SetValueAsBool(IsReadyToCharge, false);

		ChargeAtPlayer();
	}
		if (!bIsFinished) { return; }
		OwnerComp.GetBlackboardComponent()
			->SetValueAsEnum(TEXT("CurrentState"), EEnemyState::Melee);
			
		
	
		ControllerRef->ReceiveMoveCompleted.Remove(MoveCompletedDelagate);

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);	
}

UBTT_ChargeAttack::UBTT_ChargeAttack()
{
	bNotifyTick = true;

	MoveCompletedDelagate.BindUFunction(
		this, "HandleMoveCompleted"
	);
}


EBTNodeResult::Type UBTT_ChargeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	

	ControllerRef = OwnerComp.GetAIOwner();
	CharacterRef = ControllerRef->GetCharacter();
	BossAnim = Cast<UBossAnimationInstance>(CharacterRef->GetMesh()->GetAnimInstance());

	BossAnim->bIsCharging = true;

	OwnerComp.GetBlackboardComponent() ->
		SetValueAsBool(
			TEXT("IsReadyToCharge"),
			false
		);

	bIsFinished = false;
	
	return EBTNodeResult::InProgress;
}

void UBTT_ChargeAttack::ChargeAtPlayer()
{
	APawn* PlayerRef{
		GetWorld()->GetFirstPlayerController()->GetPawn()
	};

	FVector PlayerLocation{ PlayerRef->GetActorLocation() };

	FAIMoveRequest MoveRequest{ PlayerLocation };
	MoveRequest.SetUsePathfinding(true);
	MoveRequest.SetAcceptanceRadius(AcceptableRadius);

	ControllerRef->MoveTo(MoveRequest);
	ControllerRef->SetFocus(PlayerRef);

	ControllerRef->ReceiveMoveCompleted.AddUnique(MoveCompletedDelagate);

	OriginalWalkSpeed = CharacterRef->GetCharacterMovement()->MaxWalkSpeed;

	CharacterRef->GetCharacterMovement()->MaxWalkSpeed = ChargeWalkSpeed;
}

void UBTT_ChargeAttack::HandleMoveCompleted()
{
	BossAnim->bIsCharging = false;
	
	FTimerHandle AttackTimer;
	
	CharacterRef->GetWorldTimerManager().SetTimer(
		AttackTimer,
		this,
		&UBTT_ChargeAttack::FinishAttackTask,
		1.0f,false
	);
	CharacterRef->GetCharacterMovement()->MaxWalkSpeed = OriginalWalkSpeed;
}

void UBTT_ChargeAttack::FinishAttackTask()
{
	
	bIsFinished = true;
	

}
