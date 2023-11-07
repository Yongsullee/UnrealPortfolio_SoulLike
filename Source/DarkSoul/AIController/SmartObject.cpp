#include "SmartObject.h"
#include "Components/ArrowComponent.h"
#include "Components/BillboardComponent.h"

ASmartObject::ASmartObject()
{

	PrimaryActorTick.bCanEverTick = false;

	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	RootComponent = Billboard;

	FacingDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("Facing Direction"));
	FacingDirection->SetupAttachment(Billboard);

	SetActorHiddenInGame(true);

}

void ASmartObject::BeginPlay()
{

	Super::BeginPlay();
	
}