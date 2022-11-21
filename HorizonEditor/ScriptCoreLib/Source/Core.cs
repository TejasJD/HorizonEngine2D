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

        public void PrintInts(int number1, int number2)
        {
            Console.WriteLine($"C# number1: {number1}, number2: {number2}");
        }


        public void PrintCustomMessage(string message)
        {
            Console.WriteLine($"C# says : {message}");
        }
    }
}