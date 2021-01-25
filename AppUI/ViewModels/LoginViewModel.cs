using AppUI.Common;
using AppUI.Common.DllImport;
using AppUI.Core;
using AppUI.Models;
using AppUI.Views.SubViews;
using AppUI.Views.UserControls;
using DevExpress.Mvvm;
using DevExpress.Mvvm.DataAnnotations;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Interop;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Threading;
using System.Xml.Linq;

namespace AppUI.ViewModels
{
    class LoginViewModel: ViewModelBase
    {
        //初始化
        public void Init()
        {
            //初始化窗口最大化
            WindowsMin = "Maximized";
            //初始化窗口启用选项
            ViewStart_CSH();
            //软件运行状态
            ViewPlaying = "暂停中";
            // 初始化线程池
            stp = new Amib.Threading.SmartThreadPool
            {
                MaxThreads = 10
            };

            timeRefresh.Tick += new EventHandler(time_tick);
            timeRefresh.Interval = new TimeSpan(0, 0, 1);
            timeRefresh.Start();

            GB_CSH();

            ConsoleText = $"{CmdTag}Sunny vision (v1.0.1).{Environment.NewLine}";
        }
        System.Windows.Threading.DispatcherTimer timeRefresh = new System.Windows.Threading.DispatcherTimer();

        public AsyncTCPServer Light_Server = new AsyncTCPServer();
        #region Field
        /// <summary>
        /// 窗口最小化
        /// </summary>
        public string WindowsMin
        {
            get => GetProperty(() => WindowsMin);
            set => SetProperty(() => WindowsMin, value);
        }

        /// 配置文件读写器
        /// </summary>
        private XDocument config = XDocument.Load("Config.xml");

        /// <summary>
        /// 启用窗口
        /// </summary>
        public int View01
        {
            get => GetProperty(() => View01);
            set => SetProperty(() => View01, value, () =>
            {
                config.Descendants("View01").ElementAt(0).SetValue(value);
                config.Save("Config.xml");
            });
        }
        public int View02
        {
            get => GetProperty(() => View02);
            set => SetProperty(() => View02, value, () =>
            {
                config.Descendants("View02").ElementAt(0).SetValue(value);
                config.Save("Config.xml");
            });
        }
        public int View03
        {
            get => GetProperty(() => View03);
            set => SetProperty(() => View03, value, () =>
            {
                config.Descendants("View03").ElementAt(0).SetValue(value);
                config.Save("Config.xml");
            });
        }
        public int View04
        {
            get => GetProperty(() => View04);
            set => SetProperty(() => View04, value, () =>
            {
                config.Descendants("View04").ElementAt(0).SetValue(value);
                config.Save("Config.xml");
            });
        }
        /// <summary>
        /// 启用工位类型
        /// </summary>
        public int View01Mod
        {
            get => GetProperty(() => View01Mod);
            set => SetProperty(() => View01Mod, value, () =>
            {
                config.Descendants("View01Mod").ElementAt(0).SetValue(value);
                config.Save("Config.xml");
            });
        }
        public int View02Mod
        {
            get => GetProperty(() => View02Mod);
            set => SetProperty(() => View02Mod, value, () =>
            {
                config.Descendants("View02Mod").ElementAt(0).SetValue(value);
                config.Save("Config.xml");
            });
        }
        public int View03Mod
        {
            get => GetProperty(() => View03Mod);
            set => SetProperty(() => View03Mod, value, () =>
            {
                config.Descendants("View03Mod").ElementAt(0).SetValue(value);
                config.Save("Config.xml");
            });
        }
        public int View04Mod
        {
            get => GetProperty(() => View04Mod);
            set => SetProperty(() => View04Mod, value, () =>
            {
                config.Descendants("View04Mod").ElementAt(0).SetValue(value);
                config.Save("Config.xml");
            });
        }

        /// <summary>
        /// 窗口运行状态
        /// </summary>
        public string ViewPlaying
        {
            get => GetProperty(() => ViewPlaying);
            set => SetProperty(() => ViewPlaying, value);
        }

        /// <summary>
        /// 是否有开发者权限
        /// 0：没有开发者权限，1：有开发者权限
        /// </summary>
        public bool IsDeveloperLogin
        {
            get => GetProperty(() => IsDeveloperLogin);
            set => SetProperty(() => IsDeveloperLogin, value);
        }

        /// <summary>
        /// APP模式
        /// 0：用户模式，1：开发模式， 2：调试模式
        /// </summary>
        public int APP_MODE
        {
            get => GetProperty(() => APP_MODE);
            set => SetProperty(() => APP_MODE, value);
        }

        /// <summary>
        /// APP模式，用于binding，用于算子空间的column的列宽是否可以变为0.
        /// </summary>
        public bool APP_MODE_ForConverter
        {
            get => GetProperty(() => APP_MODE_ForConverter);
            set => SetProperty(() => APP_MODE_ForConverter, value);
        }
        /// <summary>
        /// TCP服务器
        /// </summary>
        public string GB_Server_IP
        {
            get => GetProperty(() => GB_Server_IP);
            set => SetProperty(() => GB_Server_IP, value, () =>
            {
                config.Descendants("GB_Server_IP").ElementAt(0).SetValue(value);
                config.Save("Config.xml");
            });
        }
        public string GB_Server_Port
        {
            get => GetProperty(() => GB_Server_Port);
            set => SetProperty(() => GB_Server_Port, value, () =>
            {
                config.Descendants("GB_Server_Port").ElementAt(0).SetValue(value);
                config.Save("Config.xml");
            });
        }
        public string GB_UDP_IP
        {
            get => GetProperty(() => GB_UDP_IP);
            set => SetProperty(() => GB_UDP_IP, value, () =>
            {
                config.Descendants("GB_UDP_IP").ElementAt(0).SetValue(value);
                config.Save("Config.xml");
            });
        }
        public string GB_UDP_Port
        {
            get => GetProperty(() => GB_UDP_Port);
            set => SetProperty(() => GB_UDP_Port, value, () =>
            {
                config.Descendants("GB_UDP_Port").ElementAt(0).SetValue(value);
                config.Save("Config.xml");
            });
        }
        public string GB_Time
        {
            get => GetProperty(() => GB_Time);
            set => SetProperty(() => GB_Time, value, () =>
            {
                config.Descendants("GB_Time").ElementAt(0).SetValue(value);
                config.Save("Config.xml");
            });
        }
        public int GB_Type
        {
            get => GetProperty(() => GB_Type);
            set => SetProperty(() => GB_Type, value, () =>
            {
                config.Descendants("GB_Type").ElementAt(0).SetValue(value);
                config.Save("Config.xml");
            });
        }
        public int GB_DataNum
        {
            get => GetProperty(() => GB_DataNum);
            set => SetProperty(() => GB_DataNum, value, () =>
            {
                config.Descendants("GB_DataNum").ElementAt(0).SetValue(value);
                config.Save("Config.xml");
            });
        }
        public string GB_Data01
        {
            get => GetProperty(() => GB_Data01);
            set => SetProperty(() => GB_Data01, value);
        }
        public string GB_Data02
        {
            get => GetProperty(() => GB_Data02);
            set => SetProperty(() => GB_Data02, value);
        }
        public string GB_Data03
        {
            get => GetProperty(() => GB_Data03);
            set => SetProperty(() => GB_Data03, value);
        }
        public string GB_Data04
        {
            get => GetProperty(() => GB_Data04);
            set => SetProperty(() => GB_Data04, value);
        }
        /// <summary>
        /// 连接状态
        /// </summary>
        public string Light_LinkFlag
        {
            get => GetProperty(() => Light_LinkFlag);
            set => SetProperty(() => Light_LinkFlag, value);
        }
        public string Server_OpenFlag
        {
            get => GetProperty(() => Server_OpenFlag);
            set => SetProperty(() => Server_OpenFlag, value);
        }
        /// <summary>
        /// 线程编号
        /// </summary>
        private int stpIndex = 0;
        /// <summary>
        /// 智能线程池，用于处理回调图像算法
        /// </summary>
        private Amib.Threading.SmartThreadPool stp;

        /// <summary>
        /// 日期时间
        /// </summary>
        public string DataTime
        {
            get => GetProperty(() => DataTime);
            set => SetProperty(() => DataTime, value);
        }
        public string dataTime => "𒆙  " + DateTime.Now.ToString("yyyy-MM-dd  HH:mm:ss ");

        //计时器回调
        private void time_tick(object sender, EventArgs e)
        {
            DataTime = dataTime;
        }
        #endregion

        //配置名称
        public string ConfigName
        {
            get => GetProperty(() => ConfigName);
            set => SetProperty(() => ConfigName, value, () =>
            {
                config.Descendants("ConfigName").ElementAt(0).SetValue(value);
                config.Save("Config.xml");
            });
        }

        /// <summary>
        /// 控制台文本
        /// </summary>
        public string ConsoleText
        {
            get => GetProperty(() => ConsoleText);
            set => SetProperty(() => ConsoleText, value);
        }

        /// <summary>
        /// 尾标
        /// </summary>
        public string CmdTag => DateTime.Now.ToString("T") + ": ";

       
        #region Command
        /// <summary>
        /// 关闭窗口委托
        /// </summary>
        public System.Action CloseAction { get; set; }

        /// <summary>
        /// 退出程序
        /// </summary>
        /// <param name="obj"></param>
        [AsyncCommand]
        public void ExitCommand(object obj)
        {
       
            //Hik.MV_Dispose();
            //IsPlaying = false;
            CloseAction();  // Calls Close() method of the View.
        }

        /// <summary>
        /// 窗口最小化
        /// </summary>
        /// <param name="obj"></param>
        [AsyncCommand]
        public void MinCommand(object obj)
        {
            WindowsMin = "Minimized";
        }

        /// <summary>
        ///  导航栏
        /// </summary>
        /// <param name="obj"></param>
        [AsyncCommand]
        public void CtrlClickCommand(object obj)
        {
            Grid mainArea = obj as Grid;
            Grid mainGrid = VisualHelper.FindVisualAncestorOfType<Grid>(mainArea);

            if (mainGrid.RowDefinitions[2].Height != new GridLength(0, GridUnitType.Pixel))
            {
                // 隐藏控制台
                mainGrid.RowDefinitions[2].Height = new GridLength(0, GridUnitType.Pixel);    // 控制台
                //mainArea.ColumnDefinitions[0].Width = new GridLength(1, GridUnitType.Star);     // 左侧
                mainArea.ColumnDefinitions[2].Width = new GridLength(1.5, GridUnitType.Star);     // 右侧
            }
            else
            {
                // 显示控制台
                mainGrid.RowDefinitions[2].Height = new GridLength(100, GridUnitType.Pixel);      // 控制台
                //mainArea.ColumnDefinitions[0].Width = new GridLength(0, GridUnitType.Star);     // 左侧
                mainArea.ColumnDefinitions[2].Width = new GridLength(1.5, GridUnitType.Star);     // 右侧
            }
        }


        #endregion

        #region 初始化
        private void ViewStart_CSH()
        {
            View01 = int.Parse(config.Descendants("View01").ElementAt(0).Value);
            View02 = int.Parse(config.Descendants("View02").ElementAt(0).Value);
            View03 = int.Parse(config.Descendants("View03").ElementAt(0).Value);
            View04 = int.Parse(config.Descendants("View04").ElementAt(0).Value);
        }
        private void GB_CSH()
        {
            GB_Data01 = "0";
            GB_Data02 = "0";
            GB_Data03 = "0";
            GB_Data04 = "0";

            GB_Server_IP = config.Descendants("GB_Server_IP").ElementAt(0).Value;
            GB_Server_Port = config.Descendants("GB_Server_Port").ElementAt(0).Value;
            GB_UDP_IP = config.Descendants("GB_UDP_IP").ElementAt(0).Value;
            GB_UDP_Port = config.Descendants("GB_UDP_Port").ElementAt(0).Value;
            GB_Time = config.Descendants("GB_Time").ElementAt(0).Value;
            GB_Type = int.Parse(config.Descendants("GB_Type").ElementAt(0).Value);
            GB_DataNum = int.Parse(config.Descendants("GB_DataNum").ElementAt(0).Value);
            ConfigName = config.Descendants("ConfigName").ElementAt(0).Value;

        }
        /// <summary>
        /// 开发者权限登陆命令
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        [AsyncCommand]
        public void DeveloperLoginCommand(object obj)
        {
            if (!IsDeveloperLogin)
            {
                DeveloperLoginViewLogin view = new DeveloperLoginViewLogin { DataContext = this };
                //view.Owner = Window.GetWindow(this);
                view.Show();
            }

        }

        /// <summary>
        ///     开发者权限退出
        /// </summary>
        /// <param name="obj"></param>
        [AsyncCommand]
        public void DeveloperLogoutCommand(object obj)
        {
            IsDeveloperLogin = false;
            APP_MODE = 0;

        }


        #endregion
    }
}
