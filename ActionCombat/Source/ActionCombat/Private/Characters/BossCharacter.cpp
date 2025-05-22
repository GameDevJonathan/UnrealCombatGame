// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BossCharacter.h"
#include "Characters/EStats.h"
#include "Characters/StatsComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Combat/CombatComponent.h"
#include "Characters/MainCharacter.h"
#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "Interfaces/MainPlayer.h"

// Sets default values
ABossCharacter::ABossCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StatsComp = CreateDefaultSubobject<UStatsComponent>(TEXT("Stats Component"));

	CombatComp = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat Component"));

}

// Called when the game starts or when spawned
void ABossCharacter::BeginPlay()
{
	Super::BeginPlay();

	ControllerRef = GetController<AAIController>();

	BlackboardComp = ControllerRef 
		-> GetBlackboardComponent();

	BlackboardComp->SetValueAsEnum(
		TEXT("CurrentState"),
		IntialState
		);

	GetWorld()->GetFirstPlayerController()->GetPawn<AMainCharacter>()->StatsComp
		->OnDeathDelegate.AddDynamic(this, &ABossCharacter::HandlePlayerDeath);
	
}

// Called every frame
void ABossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABossCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABossCharacter::DetectPawn(APawn* PawnDecteted, APawn* TargetPawn)
{
	EEnemyState CurrentState{
	static_cast<EEnemyState>(BlackboardComp->GetValueAsEnum(
			TEXT("CurrentState")))
	};
		
	
	if (PawnDecteted != TargetPawn || CurrentState != EEnemyState:: Idle) { return; }
	BlackboardComp->SetValueAsEnum(
		TEXT("CurrentState"),
		EEnemyState::Range
	);	
}

float ABossCharacter::GetDamage()
{
	return StatsComp->Stats[EStat::Strength];
}

void ABossCharacter::Attack()
{
	CombatComp->RandomAttack();
	
}

float ABossCharacter::GetAnimDuration()
{
	return CombatComp->AnimationDuration;
}

float ABossCharacter::GetMeleeRange()
{
	return StatsComp->Stats[EStat::MeleeRange];
}

void ABossCharacter::HandlePlayerDeath()
{
	ControllerRef->GetBlackboardComponent()
		->SetValueAsEnum(
			TEXT("CurrentState"), EEnemyState::GameOver);
}

void ABossCharacter::HandleDeath()
{
	float Duration{ PlayAnimMontage(DeathAnimation) };
	ControllerRef->GetBrainComponent()->StopLogic("Defeated");

	FindComponentByClass<UCapsuleComponent>()
		->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FTimerHandle DestroyTimerHandle;

	GetWorld()->GetTimerManager().SetTimer(
		DestroyTimerHandle,
		this,
		&ABossCharacter::FinishDeathAnimation,
		Duration,
		false
	);

	IMainPlayer* PlayerRef{
		GetWorld()->GetFirstPlayerController()->GetPawn<IMainPlayer>()	
	};

	if (!PlayerRef) { return; }
	PlayerRef->EndLockOnWithActor(this);


}

void ABossCharacter::FinishDeathAnimation()
{
	Destroy();
}
