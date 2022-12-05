using System;

namespace Hzn
{
    public class Main
    {
        public Main()
        {
            Console.WriteLine("From Main Constructor");
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

        public void PrintKeyCode(KeyCode code)
        {
            InternalCalls.Input_IsKeyDown(code);

        }
    }
}