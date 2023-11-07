#include "PatrolPath.h"
#include "Components/SplineComponent.h"

APatrolPath::APatrolPath()
{

	PrimaryActorTick.bCanEverTick = false;

	Path = CreateDefaultSubobject<USplineComponent>(TEXT("Patrol Path"));
	Path->SetupAttachment(RootComponent);
	Path->bDrawDebug = true;

}

void APatrolPath::BeginPlay()
{

	Super::BeginPlay();

	GetSplinePoints();

}

void APatrolPath::GetSplinePoints()
{

	for (int32 i = 0; i <= Path->GetNumberOfSplinePoints(); i++)
	{
		Locations.Add(Path->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World));
	}

}
