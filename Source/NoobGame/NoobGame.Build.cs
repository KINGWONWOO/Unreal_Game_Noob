// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class NoobGame : ModuleRules
{
	public NoobGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
            "MoviePlayer",
            "AudioMixer",
			"AudioCapture",
			"AudioCaptureCore",
            "Voice",
			"Niagara"
        });

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"NoobGame",
			"NoobGame/Variant_Platforming",
			"NoobGame/Variant_Platforming/Animation",
			"NoobGame/Variant_Combat",
			"NoobGame/Variant_Combat/AI",
			"NoobGame/Variant_Combat/Animation",
			"NoobGame/Variant_Combat/Gameplay",
			"NoobGame/Variant_Combat/Interfaces",
			"NoobGame/Variant_Combat/UI",
			"NoobGame/Variant_SideScrolling",
			"NoobGame/Variant_SideScrolling/AI",
			"NoobGame/Variant_SideScrolling/Gameplay",
			"NoobGame/Variant_SideScrolling/Interfaces",
			"NoobGame/Variant_SideScrolling/UI"
		});

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
