using System;
using Sharpmake;

namespace Ray
{
    [Generate]
    class CoreModule : EngineModule
    {
        public CoreModule() : base("core")
        {
            SourceRootPath = @"[project.SharpmakeCsPath]";
        }

        [Configure]
        public override void ConfigureAll(Project.Configuration conf, Target target)
        {
            base.ConfigureAll(conf, target);

            conf.PrecompHeader = "pch.hpp";
            conf.PrecompSource = "pch.cpp";

            conf.Defines.Add("RAY_BUILD_CORE");

            if(target.OutputType == OutputType.Dll)
                conf.Output = Configuration.OutputType.Dll;
            else if(target.OutputType == OutputType.Lib)
                conf.Output = Configuration.OutputType.Lib;
        }
    }
}