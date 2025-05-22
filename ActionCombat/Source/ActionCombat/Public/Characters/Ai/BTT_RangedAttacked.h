// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_RangedAttacked.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONCOMBAT_API UBTT_RangedAttacked : public UBTTaskNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UAnimMontage* AnimMontage;

	double Threshhold{ 0.9 };


public:
	virtual EBTNodeResult::Type ExecuteTask(
		UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory
	) override;
	
};
