#include "EffectSound.h"
#include "Global/Global.h"

#include "GameObjects/GameObject.h"
#include "ActorComponents/C_AudioComponent.h"

AEffectSound::AEffectSound()
{
}

void AEffectSound::BeginPlay()
{

	Super::BeginPlay();

	// [23-06-20] Event Queue
	TArray<AActor*> Objects;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGameObject::StaticClass(), Objects);
	for (const auto& Object : Objects)
	{
		if (Object)
		{
			UC_AudioComponent* AudioComponent = Object->FindComponentByClass<UC_AudioComponent>();
			if (AudioComponent)
			{
				AudioComponent->OnSendEffectSound.BindUFunction(this, "AddNewSoundCueToEventQueue");
			}
		}
	}

}

void AEffectSound::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

	// [23-06-30] Sound Event Queue
	if (!QSoundEvents.IsEmpty())
	{
		PlaySoundAtLocation(QSoundEvents.Peek()->Get<0>(), QSoundEvents.Peek()->Get<1>());
		QSoundEvents.Pop();
	}

}
