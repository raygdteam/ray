using Sharpmake; 

[module: Sharpmake.Import("sharpmake/**.sharpmake.cs")]
[module: Sharpmake.Import("engine/**.sharpmake.cs")]

namespace Ray
{
    [Generate]
    class BasicsSolution : Solution
    {
        public BasicsSolution()
        {
            // The name of the solution.
            Name = "Ray";

            // As with the project, define which target this solution builds for.
            // It's usually the same thing.
            AddTargets(new Target(
                Platform.win64,
                DevEnv.vs2019,
                Optimization.Debug | Optimization.Development | Optimization.Release));
        }

        // Configure for all 4 generated targets. Note that the type of the
        // configuration object is of type Solution.Configuration this time.
        // (Instead of Project.Configuration.)
        [Configure]
        public void ConfigureAll(Solution.Configuration conf, Target target)
        {
            // Puts the generated solution in the /generated folder too.
            conf.SolutionPath = @"[solution.SharpmakeCsPath]";

            // Adds the project described by BasicsProject into the solution.
            // Note that this is done in the configuration, so you can generate
            // solutions that contain different projects based on their target.
            //
            // You could, for example, exclude a project that only supports 64-bit
            // from the 32-bit targets.
            conf.AddProject<BasicsProject>(target);
        }
    }
}