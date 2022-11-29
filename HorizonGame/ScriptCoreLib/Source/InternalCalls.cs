using System.Runtime.CompilerServices;

namespace Hzn
{
    public class InternalCalls
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void Greet();
    }
}
