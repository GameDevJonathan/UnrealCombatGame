// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Ai/BTT_RangedAttacked.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/EEnemyState.h"
#include "Interfaces/Fighter.h"




EBTNodeResult::Type UBTT_RangedAttacked::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACharacter* CharacterRef{ OwnerComp.GetAIOwner()->GetPawn<ACharacter>() };
	if (!IsValid(CharacterRef)) {
		return EBTNodeResult::Failed;
	}

	float Distance{ OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("Distance")) };
	
	IFighter* FighterRef{
		Cast<IFighter>(CharacterRef)
	};
	
	if (Distance < FighterRef->GetAnimDuration()) {
		OwnerComp.GetBlackboardComponent()
			->SetValueAsEnum(TEXT("CurrentState"), EEnemyState::Melee);
		AbortTask(OwnerComp,NodeMemory);
		return EBTNodeResult::Aborted;
	}

	CharacterRef->PlayAnimMontage(AnimMontage);
	double RandomValue{ UKismetMathLibrary::RandomFloat() };

	if (RandomValue > Threshhold) {
		Threshhold = 0.9f;
		
		OwnerComp.GetBlackboardComponent()
			->SetValueAsEnum(
				TEXT("CurrentState"),
				EEnemyState::Charge
			);
	}
	else {
		Threshhold -= 0.1f;
	}

	return EBTNodeResult::Succeeded;
}
