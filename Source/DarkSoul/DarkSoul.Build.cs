using UnrealBuildTool;

public class DarkSoul : ModuleRules
{
	public DarkSoul(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { 
            "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "AIModule", "GameplayCameras", "Niagara", "UMG", "GameplayTasks",  "AssetRegistry", "LevelSequence", "MovieScene"
        });

        PublicIncludePaths.Add(ModuleDirectory);

        // Uncomment if you are using Slate UI
         PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore"});

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
