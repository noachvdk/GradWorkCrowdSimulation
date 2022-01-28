// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CrowdSimulationV4 : ModuleRules
{
	public CrowdSimulationV4(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem" });
	}
}
