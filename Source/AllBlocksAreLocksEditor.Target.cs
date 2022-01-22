// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class AllBlocksAreLocksEditorTarget : TargetRules
{
	public AllBlocksAreLocksEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("AllBlocksAreLocks");
		
		// Allocate more memory to prevent build error:
		// Failed to create virtual memory for PCH
		WindowsPlatform.PCHMemoryAllocationFactor = 2000;
	}
}
