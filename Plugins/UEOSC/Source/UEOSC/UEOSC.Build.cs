// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UEOSC : ModuleRules
{
    public UEOSC (ReadOnlyTargetRules Target) : base (Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange (new string[] { "Core", "CoreUObject", "Engine", /*"UnrealEd",*/ "InputCore", "Networking", "Sockets" });

        PrivateDependencyModuleNames.AddRange(new string[] { });
        if (Target.Type == TargetType.Editor)
            PrivateDependencyModuleNames.Add("UnrealEd");
    }
}