// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_ChargeAttack.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONCOMBAT_API UBTT_ChargeAttack : public UBTTaskNode
{
	GENERATED_BODY()

	AAIController* ControllerRef;

	ACharacter* CharacterRef;

	class UBossAnimationInstance* BossAnim;

	UPROPERTY(EditAnywhere)
	float AcceptableRadius{ 100.0f };

	FScriptDelegate MoveCompletedDelagate;

	float OriginalWalkSpeed;

	UPROPERTY(EditAnywhere)
	float ChargeWalkSpeed{ 2000.f };

	UPROPERTY(VisibleAnywhere)
	bool bIsFinished{ false };

	FName IsReadyToCharge{ "IsReadyToCharge" };
	

protected:
	virtual void TickTask
		(
	UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory,
		float DeltaSeconds
		)override ;
	
public:
	UBTT_ChargeAttack();

	void ChargeAtPlayer();

	virtual EBTNodeResult::Type ExecuteTask(
		UBehaviorTreeComponent& OwnerComp, 
		uint8* NodeMemory
	) override;

	UFUNCTION()
	void HandleMoveCompleted();
	
	UFUNCTION()
	void FinishAttackTask();
};
