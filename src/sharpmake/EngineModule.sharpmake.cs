using System;
using System.IO;
using Sharpmake;

namespace Ray
{
    class EngineModule : Project
    {
        public EngineModule(string name)
        {
            Name = name;
            
            AddTargets(new Target(
                Platform.win64,
                DevEnv.vs2019,
                Optimization.Debug | Optimization.Development,
                OutputType.Dll));

            AddTargets(new Target(
                Platform.win64,
                DevEnv.vs2019,
                Optimization.Release,
                OutputType.Lib));
        }

        [Configure]
        public virtual void ConfigureAll(Project.Configuration conf, Target target)
        {
            // This is the name of the configuration. By default, it is set to
            // [target.Optimization] (so Debug or Release), but both the debug and
            // release configurations have both a shared and a static version so
            // that would not create unique configuration names.
            conf.Name = @"[target.Optimization] [target.OutputType]";

            // Gives a unique path for the project because Visual Studio does not
            // like shared intermediate directories.
            //conf.ProjectPath = @"[project.SharpmakeCsPath]../build/generated/[project.Name]";
            conf.ProjectPath = @"../build/generated/[project.Name]";

            // conf.IncludePaths.Add(@"[project.SharpmakeCsPath]");

            // TODO: replace with 'thirdparty' folder.
            conf.IncludePaths.Add(@"../../sdk/include");

            // Add the optimization-related defeines.
            if (target.Optimization == Optimization.Debug)
            {
                conf.Defines.Add("RAY_BUILD_DEBUG");
                conf.Defines.Add("RAY_BUILD_DYNAMIC");
            }
            else if(target.Optimization == Optimization.Development)
            {
                conf.Defines.Add("RAY_BUILD_DEVELOPMENT");
                conf.Defines.Add("RAY_BUILD_DYNAMIC");
            }
            else if(target.Optimization == Optimization.Release)
            {
                conf.Defines.Add("RAY_BUILD_RELEASE");
            }
        }
    }
}