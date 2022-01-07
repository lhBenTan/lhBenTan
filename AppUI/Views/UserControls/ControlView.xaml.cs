using System;
using System.Collections.Generic;
using AppUI.Common.DllImport;
using System.Linq;
using AppUI.Core;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using AppUI.ViewModels;
using System.Net;
using System.Net.Sockets;
using System.Windows.Threading;
using Walterlv.Demo;
using AppUI.Views.SubViews;
using System.Windows.Interop;
using System.IO;

namespace AppUI.Views.UserControls
{
    /// <summary>
    /// ControlView.xaml 的交互逻辑
    /// </summary>
    public partial class ControlView : UserControl
    {

        public ControlView()
        {
            InitializeComponent();
        }
        

        //关闭所有相机
        private void CloseCamera_Click(object sender, RoutedEventArgs e)
        {
            DependencyObject source = e.OriginalSource as DependencyObject;
            while (source != null && source.GetType() != typeof(LoginView))
                source = System.Windows.Media.VisualTreeHelper.GetParent(source);
            if (source != null)
            {
                LoginView loginView = source as LoginView;
                try
                {
                    LoginViewModel LoginVM = (LoginViewModel)loginView.DataContext;

                    if (LoginVM.View01 == 0)
                    {
                        MainViewModel mainVM01 = (MainViewModel)loginView._MainV01.DataContext;
                        mainVM01.MV_StopPlay();
                        Thread t1 = new Thread(o => Thread.Sleep(50));
                        t1.Start(this);
                        while (t1.IsAlive)
                        {
                            DoEvents();
                        }
                        //Thread.Sleep(50);
                        mainVM01.YTJ_Stop();
                        //Thread.Sleep(100);
                        Thread t2 = new Thread(o => Thread.Sleep(100));
                        t2.Start(this);
                        while (t2.IsAlive)
                        {
                            DoEvents();
                        }
                    }
                    if (LoginVM.View02 == 0)
                    {
                        MainViewModel mainVM02 = (MainViewModel)loginView._MainV02.DataContext;
                        mainVM02.MV_StopPlay();
                        Thread t1 = new Thread(o => Thread.Sleep(50));
                        t1.Start(this);
                        while (t1.IsAlive)
                        {
                            DoEvents();
                        }
                        //Thread.Sleep(50);
                        mainVM02.YTJ_Stop();
                        //Thread.Sleep(100);
                        Thread t2 = new Thread(o => Thread.Sleep(100));
                        t2.Start(this);
                        while (t2.IsAlive)
                        {
                            DoEvents();
                        }
                    }
                    if (LoginVM.View03 == 0)
                    {
                        MainViewModel mainVM03 = (MainViewModel)loginView._MainV03.DataContext;
                        mainVM03.MV_StopPlay();
                        Thread t1 = new Thread(o => Thread.Sleep(50));
                        t1.Start(this);
                        while (t1.IsAlive)
                        {
                            DoEvents();
                        }
                        //Thread.Sleep(50);
                        mainVM03.YTJ_Stop();
                        //Thread.Sleep(100);
                        Thread t2 = new Thread(o => Thread.Sleep(100));
                        t2.Start(this);
                        while (t2.IsAlive)
                        {
                            DoEvents();
                        }
                    }
                    if (LoginVM.View04 == 0)
                    {

                        MainViewModel mainVM04 = (MainViewModel)loginView._MainV04.DataContext;
                        mainVM04.MV_StopPlay();
                        Thread t1 = new Thread(o => Thread.Sleep(50));
                        t1.Start(this);
                        while (t1.IsAlive)
                        {
                            DoEvents();
                        }
                        //Thread.Sleep(50);
                        mainVM04.YTJ_Stop();

                    }

                }
                catch
                {
                    LoginViewModel LoginVM = (LoginViewModel)loginView.DataContext;
                }

            }
        }

        //开启所有相机
        private void OpenCamera_Click(object sender, RoutedEventArgs e)
        {
            DependencyObject source = e.OriginalSource as DependencyObject;
            while (source != null && source.GetType() != typeof(LoginView))
                source = System.Windows.Media.VisualTreeHelper.GetParent(source);
            if (source != null)
            {
                LoginView loginView = source as LoginView;
                try
                {
                    LoginViewModel LoginVM = (LoginViewModel)loginView.DataContext;

                    if (LoginVM.View01 == 0)
                    {
                        MainViewModel mainVM01 = (MainViewModel)loginView._MainV01.DataContext;
                       
                        mainVM01.MV_StartPlay();
                    }
                    if (LoginVM.View02 == 0)
                    {
                        MainViewModel mainVM02 = (MainViewModel)loginView._MainV02.DataContext;
                      
                        mainVM02.MV_StartPlay();
                    }
                    if (LoginVM.View03 == 0)
                    {
                        MainViewModel mainVM03 = (MainViewModel)loginView._MainV03.DataContext;
                      
                        mainVM03.MV_StartPlay();
                    }
                    if (LoginVM.View04 == 0)
                    {
                        MainViewModel mainVM04 = (MainViewModel)loginView._MainV04.DataContext;

                        mainVM04.MV_StartPlay();
                    }
                    LoginVM.ViewPlaying = "运行中";
          
                }
                catch
                { }

            }
        }
        public void DoEvents()
        {
            DispatcherFrame frame = new DispatcherFrame();
            Dispatcher.CurrentDispatcher.BeginInvoke(DispatcherPriority.Background,
            new DispatcherOperationCallback(ExitFrame), frame);
            Dispatcher.PushFrame(frame);
        }
        public object ExitFrame(object f)
        {
            ((DispatcherFrame)f).Continue = false;
            return null;
        }

        public int GBStartFLag = 0;
        public UdpClient GB_UDP;
        //开启所有服务器
        private void OpenServer_Click(object sender, RoutedEventArgs e)
        {
            DependencyObject source_Login = e.OriginalSource as DependencyObject;
            while (source_Login != null && source_Login.GetType() != typeof(LoginView))
                source_Login = System.Windows.Media.VisualTreeHelper.GetParent(source_Login);
            if (source_Login != null)
            {
                LoginView loginView = source_Login as LoginView;
                loginView.Menu_button.IsChecked = false;
            }
            DoEvents();
            LoadWindows();
            
            DependencyObject source = e.OriginalSource as DependencyObject;
            while (source != null && source.GetType() != typeof(LoginView))
                source = System.Windows.Media.VisualTreeHelper.GetParent(source);
            if (source != null)
            {
                LoginView loginView = source as LoginView;
                try
                {
                    LoginViewModel LoginVM = (LoginViewModel)loginView.DataContext;

                   
                    LoginVM.Server_OpenFlag = "成功";
                    if (LoginVM.View01 == 0)
                    {
                        MainViewModel mainVM01 = (MainViewModel)loginView._MainV01.DataContext;
                        mainVM01.MV_StartPlay();
                        Thread t1 = new Thread(o => Thread.Sleep(200));
                        t1.Start(this);
                        while (t1.IsAlive)
                        { 
                            DoEvents();
                        }
                        //Thread.Sleep(50);
                        mainVM01.YTJ_Start();
                        //Thread.Sleep(100);
                        Thread t2 = new Thread(o => Thread.Sleep(1000));
                        t2.Start(this);
                        while (t2.IsAlive)
                        {
                            DoEvents();
                        }
                       
                    }
                    if (LoginVM.View02 == 0)
                    {
                        MainViewModel mainVM02 = (MainViewModel)loginView._MainV02.DataContext;
                        mainVM02.MV_StartPlay();
                        Thread t1 = new Thread(o => Thread.Sleep(200));
                        t1.Start(this);
                        while (t1.IsAlive)
                        {
                            DoEvents();
                        }
                        //Thread.Sleep(50);
                        mainVM02.YTJ_Start();
                        //Thread.Sleep(100);
                        Thread t2 = new Thread(o => Thread.Sleep(1000));
                        t2.Start(this);
                        while (t2.IsAlive)
                        {
                            DoEvents();
                        }
                    
                    }
                    if (LoginVM.View03 == 0)
                    {
                        MainViewModel mainVM03 = (MainViewModel)loginView._MainV03.DataContext;
                        mainVM03.MV_StartPlay();
                        Thread t1 = new Thread(o => Thread.Sleep(200));
                        t1.Start(this);
                        while (t1.IsAlive)
                        {
                            DoEvents();
                        }
                        //Thread.Sleep(50);
                        mainVM03.YTJ_Start();
                        //Thread.Sleep(100);
                        Thread t2 = new Thread(o => Thread.Sleep(1000));
                        t2.Start(this);
                        while (t2.IsAlive)
                        {
                            DoEvents();
                        }
                    
                    }
                    if (LoginVM.View04 == 0)
                    {
                        
                        MainViewModel mainVM04 = (MainViewModel)loginView._MainV04.DataContext;
                        mainVM04.MV_StartPlay();
                        Thread t1 = new Thread(o => Thread.Sleep(200));
                        t1.Start(this);
                        while (t1.IsAlive)
                        {
                            DoEvents();
                        }
                        //Thread.Sleep(50);
                        mainVM04.YTJ_Start();
                        Thread t2 = new Thread(o => Thread.Sleep(1000));
                        t2.Start(this);
                        while (t2.IsAlive)
                        {
                            DoEvents();
                        }
                    }
                    LoginVM.ViewPlaying = "运行中";
                    if (LoginVM.GB_AutoStrat == "True" && GBStartFLag == 0)
                    {
                        GBStartFLag = 1;
                        try
                        { GB_UDP = Connect(LoginVM.GB_Server_IP, LoginVM.GB_Server_Port, LoginVM.GB_UDP_IP, LoginVM.GB_UDP_Port);
                          LoginVM.ConsoleText += $"{LoginVM.CmdTag + "广播自动启动成功" + Environment.NewLine}";
                            DispatcherTimer dt = new DispatcherTimer() { Interval = TimeSpan.FromSeconds(120) };
                            dt.Tick += delegate
                            {
                                try
                                {
                                    try
                                    {
                                        GB_UDP.Close();
                                        GB_UDP.Dispose();
                                        Thread.Sleep(33);
                                        LoginVM.ConsoleText += $"{LoginVM.CmdTag + "广播UDP关闭成功" + Environment.NewLine}";
                                    }
                                    catch { LoginVM.ConsoleText += $"{LoginVM.CmdTag + "广播UDP关闭失败" + Environment.NewLine}"; }
                                    try
                                    {
                                        GB_UDP = Connect(LoginVM.GB_Server_IP, LoginVM.GB_Server_Port, LoginVM.GB_UDP_IP, LoginVM.GB_UDP_Port);
                                        LoginVM.ConsoleText = "";
                                        LoginVM.ConsoleText += $"{LoginVM.CmdTag + "广播启动成功" + Environment.NewLine}";
                                    }

                                    catch { LoginVM.ConsoleText += $"{LoginVM.CmdTag + "广播启动异常" + Environment.NewLine}"; }
                                }
                                catch { dt.Stop(); LoginVM.ConsoleText += $"{LoginVM.CmdTag + "广播自动重启异常" + Environment.NewLine}"; }

                            };
                            dt.Start();
                        }
                        catch{
                            LoginVM.ConsoleText += $"{LoginVM.CmdTag + "广播自动启动异常"  + Environment.NewLine}";
                        };
                    }
                }
                catch
                {
                    LoginViewModel LoginVM = (LoginViewModel)loginView.DataContext;
                    LoginVM.Server_OpenFlag = "失败";
                    LoginVM.ViewPlaying = "暂停中";
                    CVAlgorithms.SendMessage(LoadWindowHwnd, WM_CLOSE, 0, 0);
                }

            }

            CVAlgorithms.SendMessage(LoadWindowHwnd,WM_CLOSE,0,0);
        }
        //Load窗口
        private async void LoadWindows()
        {
            // 传入的参数是线程的名称，也可以不用传。
            var dispatcher_Load = await UIDispatcher.RunNewAsync("Load_UI");
            var backgroundWindow = await dispatcher_Load.InvokeAsync(() =>
            {
                LoadWindow Load = new LoadWindow();
                Load.SourceInitialized += OnSourceInitialized;
                Load.Show();
                return Load;
            });
            
        }
        //句柄
        public IntPtr LoadWindowHwnd ;
        const int WM_CLOSE = 0x0010;
        private void OnSourceInitialized(object sender, EventArgs e)
        {
            // 在这里可以获取到窗口句柄。
            var childHazhoingshan1dle = new WindowInteropHelper((Window)sender).Handle;
            LoadWindowHwnd = childHazhoingshan1dle;
            //CVAlgorithms.MoveWindow(childHazhoingshan1dle, 0, 0, 300, 300, true);

        }
        //控制台文本更新
        private void ConsoleView_TextChanged(object sender, TextChangedEventArgs e)
        {
            ConsoleView.SelectionStart = ConsoleView.Text.Length;
            ConsoleView.ScrollToEnd();
        }
    
        public UdpClient Connect(string LocalIP, string LocalPort, string SendIP, string SendPort)
        {
          
            
            try
            {
                IPEndPoint ipe = new IPEndPoint(IPAddress.Parse(LocalIP), int.Parse(LocalPort));
                IPEndPoint endpoint = new IPEndPoint(IPAddress.Parse(SendIP), int.Parse(SendPort));
                UdpClient UDPclient = new UdpClient(ipe);
                DispatcherTimer dt = new DispatcherTimer() { Interval = TimeSpan.FromSeconds(10) };
                string OKNums = "Null";
                dt.Tick += delegate
                {
                    switch (GB_DataNumCom.SelectedIndex)
                    {
                        case 0: { OKNums = GB_DataText01.Text; }; break;
                        case 1: { OKNums = GB_DataText02.Text; }; break;
                        case 2: { OKNums = GB_DataText03.Text; }; break;
                        case 3: { }; break;
                        default:; break;
                    }
                    try
                    {
                        byte[] data_Send = new byte[128];
                        string OKNumMsg = GB_DataText04.Text + "：产出良品：" + OKNums;
                        data_Send = Encoding.Default.GetBytes(OKNumMsg);
                        UDPclient.Send(data_Send, data_Send.Length, endpoint);
                    }
                    catch { dt.Stop(); }
                };

                dt.Start();
                return UDPclient;
                //msgthr.Start("启动广播成功");
            }
            catch {
                return null;//msgthr.Start("启动广播失败"); 
            };
        }

        //弹窗线程
        Thread msgthr = new Thread(new ParameterizedThreadStart(msgtxt =>
        {
            MessageBox.Show((string)msgtxt, "通知", MessageBoxButton.OK, MessageBoxImage.Information, MessageBoxResult.OK, MessageBoxOptions.DefaultDesktopOnly);
        }));
        private void GB_Start_Click(object sender, RoutedEventArgs e)
        {           
                DependencyObject source = e.OriginalSource as DependencyObject;
                while (source != null && source.GetType() != typeof(LoginView))
                    source = System.Windows.Media.VisualTreeHelper.GetParent(source);
                if (source != null)
                {
                    LoginView loginView = source as LoginView;
                    LoginViewModel LoginVM = (LoginViewModel)loginView.DataContext;
                DispatcherTimer dt = new DispatcherTimer() { Interval = TimeSpan.FromSeconds(120) };
                dt.Tick += delegate
                {
                    try
                    {
                        try
                        {
                            GB_UDP.Close();
                            GB_UDP.Dispose();
                            Thread.Sleep(33);
                            LoginVM.ConsoleText += $"{LoginVM.CmdTag + "广播UDP关闭成功" + Environment.NewLine}";
                        }
                        catch { LoginVM.ConsoleText += $"{LoginVM.CmdTag + "广播UDP关闭失败" + Environment.NewLine}"; }
                        try
                        {
                            GB_UDP = Connect(LoginVM.GB_Server_IP, LoginVM.GB_Server_Port, LoginVM.GB_UDP_IP, LoginVM.GB_UDP_Port);
                            LoginVM.ConsoleText = "";
                            LoginVM.ConsoleText += $"{LoginVM.CmdTag + "广播启动成功" + Environment.NewLine}";
                        }

                        catch { LoginVM.ConsoleText += $"{LoginVM.CmdTag + "广播启动异常" + Environment.NewLine}"; }
                    }
                    catch { dt.Stop(); LoginVM.ConsoleText += $"{LoginVM.CmdTag + "广播自动重启异常" + Environment.NewLine}"; }
                  
                };
                dt.Start();
                }
            
        }

         
        //导出配置文件夹
        public void UploadConfigFile()
        {
            System.Windows.Forms.FolderBrowserDialog open_file = new System.Windows.Forms.FolderBrowserDialog();
            open_file.Description = "请选择导出文件保存路径";
            if (open_file.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                string savePath = open_file.SelectedPath +"\\" + ConfigName.Text;
                if (false == System.IO.Directory.Exists(savePath))
                {
                    //创建文件夹
                    Directory.CreateDirectory(savePath);
                }
                //CopyFolder(".\\Para", savePath);
                bool copy = CopyDirectory(".\\Para\\", savePath, true);
                File.Copy(".\\Config.xml", savePath+"\\Config.xml", true);
     
            }

        }

        //导入配置参数
        public void DownloadConfigFile()
        {
            System.Windows.Forms.FolderBrowserDialog open_file = new System.Windows.Forms.FolderBrowserDialog();
            open_file.Description = "请选择载入配置";
            if (open_file.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                string savePath = open_file.SelectedPath;
               
                //CopyFolder(".\\Para", savePath);
                bool copy = CopyDirectory(savePath, ".\\Para\\", true);
                File.Copy(savePath + "\\Config.xml", ".\\Config.xml", true);
                System.Windows.Forms.Application.Restart();
                Application.Current.Shutdown();
            }

        }

        public void CopyFolder(string sourceFolder, string destFolder)
        {
            string folderName = System.IO.Path.GetFileName(sourceFolder);
            string destfolderdir = System.IO.Path.Combine(destFolder, folderName);
            string[] filenames = System.IO.Directory.GetFileSystemEntries(sourceFolder);
            foreach (string file in filenames)// 遍历所有的文件和目录
            {
                if (System.IO.Directory.Exists(file))
                {
                    string currentdir = System.IO.Path.Combine(destfolderdir, System.IO.Path.GetFileName(file));
                    if (!System.IO.Directory.Exists(currentdir))
                    {
                        System.IO.Directory.CreateDirectory(currentdir);
                    }
                    CopyFolder(file, destfolderdir);
                }
                else
                {
                    string srcfileName = System.IO.Path.Combine(destfolderdir, System.IO.Path.GetFileName(file));
                    if (!System.IO.Directory.Exists(destfolderdir))
                    {
                        System.IO.Directory.CreateDirectory(destfolderdir);
                    }
                    System.IO.File.Copy(file, srcfileName);
                }
            }
            }

        private static bool CopyDirectory(string SourcePath, string DestinationPath, bool overwriteexisting)
        {
            bool ret = false;
            try
            {
                SourcePath = SourcePath.EndsWith(@"\") ? SourcePath : SourcePath + @"\";
                DestinationPath = DestinationPath.EndsWith(@"\") ? DestinationPath : DestinationPath + @"\";

                if (Directory.Exists(SourcePath))
                {
                    if (Directory.Exists(DestinationPath) == false)
                        Directory.CreateDirectory(DestinationPath);

                    foreach (string fls in Directory.GetFiles(SourcePath))
                    {
                        FileInfo flinfo = new FileInfo(fls);
                        flinfo.CopyTo(DestinationPath + flinfo.Name, overwriteexisting);
                    }
                    foreach (string drs in Directory.GetDirectories(SourcePath))
                    {
                        DirectoryInfo drinfo = new DirectoryInfo(drs);
                        if (CopyDirectory(drs, DestinationPath + drinfo.Name, overwriteexisting) == false)
                            ret = false;
                    }
                }
                ret = true;
            }
            catch (Exception ex)
            {
                ret = false;
            }
            return ret;
        }

        private void UploadConfigFile_Click(object sender, RoutedEventArgs e)
        {
            UploadConfigFile();
        }

        private void DownloadConfigFile_Click(object sender, RoutedEventArgs e)
        {
            DownloadConfigFile();
        }
    }
}
