using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MediaHub
{
    internal class Program
    {
        static void Main(string[] args)
        {
            SerialPort port;
            
            port = new SerialPort(System.Environment.GetEnvironmentVariable("ARDUINO_COM_LOAD"));
            // настройки порта
            port.BaudRate = 115200;
            port.DataBits = 8;
            port.Parity = System.IO.Ports.Parity.None;
            port.StopBits = System.IO.Ports.StopBits.One;
            port.ReadTimeout = 1000;
            port.WriteTimeout = 1000;

            port.DataReceived += new SerialDataReceivedEventHandler(DataReceivedHandler);
            port.Open();

            string line;

            while ((line = Console.ReadLine()) != null)
            {

                port.WriteLine(line);
            }

            port.Close();
        }
        private static void DataReceivedHandler(
                        object sender,
                        SerialDataReceivedEventArgs e)
        {
            SerialPort sp = (SerialPort)sender;
            string indata = sp.ReadExisting();
            Console.WriteLine(indata);
            Console.WriteLine("==============");
        }

    }
}
