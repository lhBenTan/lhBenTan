using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Threading;

namespace AppUI.Core
{
    public class AsyncUDPServer: DispatcherObject
    {
        public string OKNums = "0";
        public void Connect(string LocalIP, string LocalPort, string SendIP, string SendPort)
        {
            IPEndPoint ipe = new IPEndPoint(IPAddress.Parse(LocalIP), int.Parse(LocalPort));

            IPEndPoint endpoint = new IPEndPoint(IPAddress.Parse(SendIP), int.Parse(SendPort));
            UdpClient UDPclient = new UdpClient(ipe);

            DispatcherTimer dt = new DispatcherTimer() { Interval = TimeSpan.FromSeconds(10) };

            dt.Tick += delegate
            {
                byte[] data_Send = new byte[128];
                string OKNumMsg = "产出良品：" + OKNums;
                data_Send = Encoding.Default.GetBytes(OKNumMsg);
                UDPclient.Send(data_Send, data_Send.Length, endpoint);
            };

            dt.Start();
        }
    }
}
