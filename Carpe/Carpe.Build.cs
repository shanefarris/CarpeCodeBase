// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class Carpe : ModuleRules
{
	public Carpe(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PrivateIncludePaths.AddRange(
			new string[] { 
                "/Survival/",
				"/Survival/Player",
                "/Survival/Online",
				"/Survival/UI",
				"/Survival/UI/Menu",
				"/Survival/UI/Style",
				"/Survival/UI/Widgets",
            }
		);

        PublicDependencyModuleNames.AddRange(
			new string[] {
				"Core", 
				"CoreUObject", 
				"Engine",
				"OnlineSubsystem", 
				"OnlineSubsystemUtils",
				"AssetRegistry",
                "AIModule",
                "InputCore", "UMG", "Slate", "SlateCore", "HTTP", "JsonUtilities", "Json",
				"GameplayTasks"
            }
		);

        PrivateDependencyModuleNames.AddRange(
			new string[] {
				"InputCore",
				"Slate",
				"SlateCore"
            }
		);

		DynamicallyLoadedModuleNames.AddRange(
			new string[] {
				"OnlineSubsystemNull",
				"NetworkReplayStreaming",
				"NullNetworkReplayStreaming",
				"HttpNetworkReplayStreaming"
            }
		);

		PrivateIncludePathModuleNames.AddRange(
			new string[] {
				"NetworkReplayStreaming"
            }
		);
	}
}
