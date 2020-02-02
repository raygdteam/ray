using Sharpmake; 

[module: Sharpmake.Include("sharpmake/*.sharpmake.cs")]
[module: Sharpmake.Include("/engine/**/*.sharpmake.cs")]
[module: Sharpmake.Include("engine/core/core.sharpmake.cs")]

namespace Ray
{
    [Generate]
    class RaySolution : Solution
    {
        public RaySolution()
        {
            // The name of the solution.
            Name = "Ray";

            // As with the project, define which target this solution builds for.
            // It's usually the same thing.
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

        // Configure for all 4 generated targets. Note that the type of the
        // configuration object is of type Solution.Configuration this time.
        // (Instead of Project.Configuration.)
        [Configure]
        public void ConfigureAll(Solution.Configuration conf, Target target)
        {
            // Puts the generated solution in the /generated folder too.
            conf.SolutionPath = @"[solution.SharpmakeCsPath]";

            conf.AddProject<CoreModule>(target);
        }
    }
}