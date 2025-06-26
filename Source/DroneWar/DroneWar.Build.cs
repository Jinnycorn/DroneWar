// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DroneWar : ModuleRules
{
	public DroneWar(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
