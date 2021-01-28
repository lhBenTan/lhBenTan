using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Threading;

namespace AppUI.Core
{
    public class AsyncTCPClient : DispatcherObject
    {

        private EventWaitHandle allDone = new EventWaitHandle(false, EventResetMode.ManualReset);
        private EventWaitHandle ReallDone = new EventWaitHandle(false, EventResetMode.ManualReset);
        public Socket Client;
        public string data_Output;
        public int SendClient_Flag = 0;
        public int PLCShot_Flag = 0;
        public int WLShot_Flag = 0;
        public int HeartFlag = 0;
        public byte[] data = new byte[32];
        public string IP = "";
        public int Port = 0;
        public void Connect()
        {
            //创建套接字
            //获得本地的IP地址族
            //创建套接字

            Socket client = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            client.SetSocketOption(SocketOptionLevel.Tcp, SocketOptionName.NoDelay, false);
            client.NoDelay = false;
            client.SendBufferSize = 0;
            //连接服务器
            try
            {
                //client.Connect(IP, Port);
                allDone.Reset();
                client.BeginConnect(IP, Port, ConnectCallback, client);
                if (!allDone.WaitOne(200, false))
                {
                    client.Close();
                    throw new SocketException(10060);
                }
                //Clients.Add(client.RemoteEndPoint.ToString(), client);
                //ClientIPList.Add(client.RemoteEndPoint.ToString());
                //Console.WriteLine(string.Format("{0}连接成功", client.RemoteEndPoint.ToString()));
                //data_Output = string.Format("~{0}连接成功", client.RemoteEndPoint.ToString());
                //Dispatcher.BeginInvoke(new MReceiveDataShow(ReceiveDataShow), new ReceiveData
                //{
                //    ReceiveData_s = data_Output,
                //});
                //AsyncReveive(client);
            }
            catch (Exception ex)
            {
                data_Output = "````````````";
                Dispatcher.BeginInvoke(new MReceiveDataShow(ReceiveDataShow), new ReceiveData
                {
                    ReceiveData_s = data_Output,
                });
                Console.WriteLine("服务器连接失败1！");
            }
        }
        public void ConnectCallback(IAsyncResult ar)
        {
            allDone.Set();
            try
            {
                var client = ar.AsyncState as Socket;
                Client = client;
                Console.WriteLine(string.Format("{0}连接成功", client.RemoteEndPoint.ToString()));
                data_Output = string.Format("~{0}连接成功", client.RemoteEndPoint.ToString());
                Dispatcher.BeginInvoke(new MReceiveDataShow(ReceiveDataShow), new ReceiveData
                {
                    ReceiveData_s = data_Output,
                });
                AsyncReveive(client);
            }
            catch (Exception ex)
            {
                data_Output = "```````````````";
                Dispatcher.BeginInvoke(new MReceiveDataShow(ReceiveDataShow), new ReceiveData
                {
                    ReceiveData_s = data_Output,
                });
                Console.WriteLine("服务器连接失败2！");
            }
        }

        public void ReConnect(string ReSendMsg)
        {
            //尝试关闭连接
            try { ClentClose(); }
            catch {  }
            //重新连接
            Thread.Sleep(20);
            Connect();
            Thread.Sleep(20);
            AsyncSend(ReSendMsg);
            //


        }
       
        /// <summary>
        /// 接收消息
        /// </summary>
        /// <param name="client"></param>
        private void AsyncReveive(Socket client)
        {
            client.BeginReceive(data, 0, data.Length, SocketFlags.None, AsyncReveive, client);
        }
        public void AsyncReveive(IAsyncResult result)
        {
            int length = 0;
            var client = result.AsyncState as Socket;
            try
            {
                bool part1 = client.Poll(1000, SelectMode.SelectRead);
                bool part2 = (client.Available == 0);
                if (part1 && part2)
                {
                    client.Close();
                    client.Dispose();
                }
                else
                {
                    length = client.EndReceive(result);
                    //Console.WriteLine(string.Format("客户端" + client.RemoteEndPoint.ToString() + "接收信息：{0}", Encoding.UTF8.GetString(data, 0, length)));
                    data_Output = Encoding.UTF8.GetString(data, 0, length);
                    Dispatcher.BeginInvoke(new MReceiveDataShow(ReceiveDataShow), new ReceiveData
                    {
                        ReceiveData_s = data_Output,
                    });
                    //清空接收数组
                    Array.Clear(data, 0, data.Length);
                    //尾递归实现连续接收
                    client.BeginReceive(data, 0, data.Length, SocketFlags.None, AsyncReveive, client);

                }
            }
            catch (Exception ex)
            {
                data_Output = "`";
                Dispatcher.BeginInvoke(new MReceiveDataShow(ReceiveDataShow), new ReceiveData
                {
                    ReceiveData_s = data_Output,
                });
                //断开连接   
            }

        }

        /// <summary>
        /// 编码发送数据
        /// </summary>
        /// <param name="input"></param>
        /// <returns></returns>
        public string BuildMSG(string input)
        {
            string output = input;

            int BCC = input[0];

            for (int i = 1; i < input.Length; i++)
            {
                BCC ^= input[i];
            }

            output = (input + Convert.ToString(BCC, 16) + "\r").ToUpper();
            return output;

        }

        //循环发送参数结构体
        private class Client_Send
        {
            public Socket client;
            public string p;
            public int DelayTime;
        }
        /// <summary>
        /// 发送消息
        /// </summary>
        /// <param name="client"></param>
        /// <param name="p"></param>
        public void AsyncSend(string p)
        {

            if (Client == null || p == string.Empty) return;
            //数据转码 
            byte[] data = new byte[64];
            data = Encoding.UTF8.GetBytes(p);
            try
            {
                //开始发送消息
                Client.BeginSend(data, 0, data.Length, SocketFlags.None, asyncResult =>
                {
                    //完成消息发送
                    int length = Client.EndSend(asyncResult);
                    //输出消息
                    Console.WriteLine(string.Format("客户端向" + Client.RemoteEndPoint.ToString() + "发出消息:{0}", p));
                }, null);
            }
            catch (Exception e)
            {
                data_Output = "`";
                Dispatcher.BeginInvoke(new MReceiveDataShow(ReceiveDataShow), new ReceiveData
                {
                    ReceiveData_s = data_Output,
                });
                Console.WriteLine(e.Message);
            }


        }

        public void AsyncSend_AutoStrat(string p, int DelayTime)
        {
            AsyncSend_Auto(Client, p, DelayTime);
        }
        private void AsyncSend_Auto(Socket client, string p, int DelayTime)
        {
            if (client == null || p == string.Empty) return;
            //数据转码
            byte[] data_Send = new byte[32];
            data_Send = Encoding.UTF8.GetBytes(p);
            Client_Send client_Send = new Client_Send();
            client_Send.client = client;
            client_Send.p = p;
            client_Send.DelayTime = DelayTime;
            client.BeginSend(data_Send, 0, data_Send.Length, SocketFlags.None, AsyncSend_Auto, client_Send);
        }

        public void AsyncSend_Auto(IAsyncResult result)
        {

            try
            {
                var client_Send = result.AsyncState as Client_Send;
                //var timer = new System.Timers.Timer();
                Thread.Sleep(client_Send.DelayTime);

                //数据转码
                byte[] data_Send = new byte[32];
                data_Send = Encoding.UTF8.GetBytes(client_Send.p);

                client_Send.client.BeginSend(data_Send, 0, data_Send.Length, SocketFlags.None, AsyncSend_Auto, client_Send);


            }
            catch (Exception ex)
            {

                data_Output = "?";
                Dispatcher.BeginInvoke(new MReceiveDataShow(ReceiveDataShow), new ReceiveData
                {
                    ReceiveData_s = data_Output,
                });
                Console.WriteLine("发送失败");

            }
        }

        public void ResultAsyncSend_AutoStrat(string p, int DelayTime)
        {
            ResultAsyncSend_Auto(Client, p, DelayTime);
        }
        private void ResultAsyncSend_Auto(Socket client, string p, int DelayTime)
        {
            if (client == null || p == string.Empty) return;
            //数据转码
            byte[] data_Send = new byte[32];
            data_Send = Encoding.UTF8.GetBytes(p);
            Client_Send client_Send = new Client_Send();
            client_Send.client = client;
            client_Send.p = p;
            client_Send.DelayTime = DelayTime;
            client.BeginSend(data_Send, 0, data_Send.Length, SocketFlags.None, ResultAsyncSend_Auto, client_Send);
        }
        public void ResultAsyncSend_Auto(IAsyncResult result)
        {

            try
            {
                var client_Send = result.AsyncState as Client_Send;

                //Thread.Sleep(client_Send.DelayTime);
                //数据转码
                byte[] data_Send = new byte[32];
                data_Send = Encoding.UTF8.GetBytes(client_Send.p);
                for (int i = 0; i < 1000; i++)
                {
                    if (SendClient_Flag == 1)
                    {
                        i = 0;
                    }
                    Thread.Sleep(5);
                }
                //while (SendClient_Flag == 1)
                //{
                //    Thread.Sleep(1);
                //}
                //Thread.Sleep(1);
                client_Send.client.BeginSend(data_Send, 0, data_Send.Length, SocketFlags.None, ResultAsyncSend_Auto, client_Send);
            }
            catch (Exception ex)
            {

                data_Output = "`";
                Dispatcher.BeginInvoke(new MReceiveDataShow(ReceiveDataShow), new ReceiveData
                {
                    ReceiveData_s = data_Output,
                });
                Console.WriteLine("发送失败");

            }
        }


        //断开连接
        public void ClentClose()
        {
            try
            {
                Client.Close();
                Client.Dispose();
            }
            catch
            {

            }

        }
        //接收数据路由回调
        public struct ReceiveData
        {
            public string ReceiveData_s;
        }
        public event EventHandler<ReceiveData> ReceiveDataInvoked;
        private void ReceiveDataShow(ReceiveData ReceiveData_S)
        {
            ReceiveDataInvoked?.Invoke(this, ReceiveData_S);
        }
        private delegate void MReceiveDataShow(ReceiveData ReceiveData_S);
    }
}
