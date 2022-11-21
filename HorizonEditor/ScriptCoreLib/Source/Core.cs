using System;

namespace Hzn
{
    public class Main
    {
        public float FloatVar { get; set; }

        public Main()
        {
            Console.WriteLine("Main Constructor!");
        }

        public void PrintMessage()
        {
            Console.WriteLine("Hello from C# world!");
        }

        public void PrintInt(int number)
        {
            Console.WriteLine($"C# number: {number}");
        }

        public void PrintCustomMessage(string message)
        {
            Console.WriteLine($"C# says : {message}");
        }
    }
}