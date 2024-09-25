// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UnrealElectiveV3 : ModuleRules
{
    public UnrealElectiveV3(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "NavigationSystem",
            "AIModule",
            "Niagara",
            "EnhancedInput",
            "ProceduralMeshComponent",
            "Landscape",
            "LandscapeEditor",
            "AIModule",
            "GameplayTasks",
            "NavigationSystem",
        });
    }
}
