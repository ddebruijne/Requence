// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System;
using System.IO;
using System.Collections.Generic;

public class RequencePlugin : ModuleRules
{
    public RequencePlugin(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        string sdlDir = Path.GetFullPath(Path.Combine(ModuleDirectory, "../../ThirdParty"));

        //Set library path
        PublicLibraryPaths.AddRange(
            new string[] {
                Path.Combine(ModuleDirectory, "../../Binaries/ThirdParty", Target.Platform.ToString()),
                Path.Combine(ModuleDirectory, "../../ThirdParty", Target.Platform.ToString())
            }
        );

        //Include SDL headers
        PublicIncludePaths.AddRange(
            new string[] {
                "RequencePlugin/Public",
                Path.Combine(ModuleDirectory, "../../ThirdParty/include")
  			}
		);

        PublicAdditionalLibraries.Add("SDL2.lib");
        PublicDelayLoadDLLs.Add("SDL2.dll");


        PrivateIncludePaths.AddRange(
			new string[] {
				"RequencePlugin/Private",
			}
		);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                "Slate",
                "SlateCore",
                "Projects",
                "Json",
                "JsonUtilities"
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateIncludePathModuleNames.AddRange(
			new string[]
			{
				"InputDevice",
			});
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"InputDevice",
                "Json",
                "JsonUtilities"
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

		if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            string _dllPath = Path.GetFullPath(Path.Combine(ModuleDirectory, "../../Binaries/ThirdParty/Win64"));
            RuntimeDependencies.Add(new RuntimeDependency(Path.Combine(_dllPath, "SDL2.dll")));
        }
        else if(Target.Platform == UnrealTargetPlatform.Win32)
        {
            string _dllPath = Path.GetFullPath(Path.Combine(ModuleDirectory, "../../Binaries/ThirdParty/Win32"));
            RuntimeDependencies.Add(new RuntimeDependency(Path.Combine(_dllPath, "SDL2.dll")));
        }
    }
}
//         PublicAdditionalLibraries.Add(Path.GetFullPath(ModuleDirectory + "/../../Binaries/" + Target.Platform.ToString() + "/SDL2.dll"));
