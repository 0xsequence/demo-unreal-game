// Some copyright should be here...
using UnrealBuildTool;

public class UnrealSequence : ModuleRules
{
    public UnrealSequence(ReadOnlyTargetRules Target) :
        base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core" });

        PrivateDependencyModuleNames
            .AddRange(new string[] {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "UMG",
                "WebBrowserWidget",
                "WebBrowser"
            });

        RuntimeDependencies
            .Add("$(ProjectDir)/Plugins/UnrealSequence/Data/...",
            StagedFileType.UFS);
    }
}
