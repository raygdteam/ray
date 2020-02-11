using Sharpmake;

namespace Ray
{
    class SharkmakeMain
    {
        [Main]
        public static void SharpmakeMain(Arguments sharpmakeArgs)
        {
            sharpmakeArgs.Generate<RaySolution>();
        }
    }
}