// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SpmG5 : ModuleRules
{
	public SpmG5(ReadOnlyTargetRules Target) : base(Target)
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
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"SpmG5",
			"SpmG5/Variant_Platforming",
			"SpmG5/Variant_Platforming/Animation",
			"SpmG5/Variant_Combat",
			"SpmG5/Variant_Combat/AI",
			"SpmG5/Variant_Combat/Animation",
			"SpmG5/Variant_Combat/Gameplay",
			"SpmG5/Variant_Combat/Interfaces",
			"SpmG5/Variant_Combat/UI",
			"SpmG5/Variant_SideScrolling",
			"SpmG5/Variant_SideScrolling/AI",
			"SpmG5/Variant_SideScrolling/Gameplay",
			"SpmG5/Variant_SideScrolling/Interfaces",
			"SpmG5/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
