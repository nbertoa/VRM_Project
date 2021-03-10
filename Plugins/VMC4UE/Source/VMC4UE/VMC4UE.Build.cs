// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class VMC4UE : ModuleRules
{
    public VMC4UE (ReadOnlyTargetRules Target) : base (Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange (new string[] { "Core", "CoreUObject", "Engine", /*"UnrealEd",*/ "InputCore", "HeadMountedDisplay", "AnimGraphRuntime", "Json", "JsonUtilities", "UEOSC" });

        PrivateDependencyModuleNames.AddRange(new string[] { });
        if (Target.Type == TargetType.Editor)
            PrivateDependencyModuleNames.Add("UnrealEd");
    }
}