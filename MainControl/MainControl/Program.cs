using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Ports;

namespace MainControl
{
    internal class Program
    {
        static void Main(string[] args)
        {
            SerialPort port;
            // получаем список доступных портов 
            string[] ports = SerialPort.GetPortNames();

            Console.WriteLine("Выберите порт:");

            // выводим список портов
            for (int i = 0; i < ports.Length; i++)
            {
                Console.WriteLine("[" + i.ToString() + "] " + ports[i].ToString());
            }
            port = new SerialPort();
            // настройки порта
            port.PortName = ports[8];
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

