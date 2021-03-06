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
using System.ComponentModel;
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
using Walterlv.Demo;

namespace AppUI.ViewModels
{
    public class MainViewModel : ViewModelBase
    {
        #region Public field
        /// <summary>
        /// 海康SDK帮助实例
        /// </summary>
        public HikHelper Hik;
        /// <summary>
        /// 数据库帮助实例
        /// </summary>
        public DatabaseHelper Dtb;

        /// <summary>
        /// Image控件显示的资源
        /// </summary>
        public WriteableBitmap ImSrc_test
        {
            get => GetProperty(() => ImSrc_test);
            set => SetProperty(() => ImSrc_test, value);
        }

        /// <summary>
        /// Image控件显示的资源
        /// </summary>
        public D3DImage DxImSrc
        {
            get => GetProperty(() => DxImSrc);
            set => SetProperty(() => DxImSrc, value);
        }

        /// <summary>
        /// 设备状态
        /// </summary>
        public string DeviceStatus
        {
            get => GetProperty(() => DeviceStatus);
            set => SetProperty(() => DeviceStatus, value);
        }

        /// <summary>
        /// 错误信息
        /// </summary>
        public string ErrorMsg
        {
            get => GetProperty(() => ErrorMsg);
            set => SetProperty(() => ErrorMsg, value);
        }

        /// <summary>
        /// 关闭窗口委托
        /// </summary>
        public System.Action CloseAction { get; set; }

        /// <summary>
        /// 是否正处于播放状态
        /// </summary>
        public bool IsPlaying
        {
            get => GetProperty(() => IsPlaying);
            set => SetProperty(() => IsPlaying, value, () => { IsStopping = !value; DeviceStatus = IsStopping == false ? "运行中..." : "暂停中..."; });
        }

        /// <summary>
        /// 是否正处于停止播放
        /// </summary>
        public bool IsStopping
        {
            get => GetProperty(() => IsStopping);
            set => SetProperty(() => IsStopping, value);
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
        public string CmdTag => DateTime.Now.ToString("O") + ": ";

        /// <summary>
        /// 算法耗时
        /// </summary>
        public double ElapsedTime
        {
            get => GetProperty(() => ElapsedTime);
            set => SetProperty(() => ElapsedTime, value);
        }

        /// <summary>
        /// 实时帧数
        /// </summary>
        public string FPS
        {
            get => GetProperty(() => FPS);
            set => SetProperty(() => FPS, value);
        }

        /// <summary>
        /// 监控设备信息
        /// </summary>
        public DVRModel CurrDvrInfo
        {
            get => GetProperty(() => CurrDvrInfo);
            set => SetProperty(() => CurrDvrInfo, value);
        }

       
        /// <summary>
        /// 实时活动线程数
        /// </summary>
        public int ActiveThreadNum
        {
            get => GetProperty(() => ActiveThreadNum);
            set => SetProperty(() => ActiveThreadNum, value);
        }


        #endregion Public field

        #region Private field

        /// <summary>
        /// 智能线程池，用于处理回调图像算法
        /// </summary>
        private Amib.Threading.SmartThreadPool stp;
        
        /// <summary>
        /// 线程编号
        /// </summary>
        private int stpIndex = 0;
        
        private double time;

        /// <summary>
        /// 线程锁对象
        /// </summary>
        private static readonly object objlock = new object();

        /// <summary>
        /// 延时计算器
        /// </summary>
        private int OldLag = DateTime.Now.Millisecond;

       
        #endregion Private field

        #region Public method

        /// <summary>
        /// 初始化
        /// </summary>
        public void Init()
        {
            //SysConfigView view = new SysConfigView() { DataContext = this };
            //Dtb.test();
            Hik.MV_DeviceListAcq();
            // 初始化池
            stp = new Amib.Threading.SmartThreadPool
            {
                MaxThreads = 1
            };

            // 原始帧回调
            Hik.MV_OnOriFrameInvoked += Hik_MV_OnOriFrameInvoked;
            Hik.MV_CameraListInvoked += MV_RefreshCameraListInvoked;

            // 参数设置
            IsPlaying = false;
            IsStopping = true;
            ConsoleText = $"{CmdTag}Sunny vision (v1.0.1).{Environment.NewLine}";     
            // 结果参数
            CurrDvrInfo = new DVRModel();
            //初始化参数设置
        
            //初始化结果参数
            CurrDvrInfo.OK_OR_NG = "NG";
            CurrDvrInfo.Angle = "0";
            CurrDvrInfo.ComparaResult = "NG";
     
            //初始化算子控件
            APP_MODE_ForConverter = false;

            //初始化系统配置按钮
            SysConfig_Mode = false;
           
            //配置初始化
            YTJ_CSH();
        }

        #endregion Public method
         
        #region Command

        /// <summary>
        ///     播放器双击事件
        /// </summary>
        /// <param name="obj"></param>
        [AsyncCommand]
        public void ImageDoubleClickCommand(object obj)
        {
            Grid mainArea = obj as Grid;
            Image image = VisualHelper.FindVisualChildren<Image>(mainArea)[0];
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
        /// <summary>
        ///     系统设置菜单命令
        /// </summary>
        /// <param name="obj"></param>
        [AsyncCommand]
        public void SysConfigCommand(object obj)
        {
            SysConfig_Mode = true;

            Thread thread = new Thread(() =>
            {
                Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                {
                    SysConfig_YTJ view2 = new SysConfig_YTJ() { DataContext = this };
                    view2.Config.DataContext = YTJ_XT_01;
                    view2.CameraList.ItemsSource = MV_Cameralist2;
                    view2.CameraListRefresh.DataContext = this;
                    view2.XT_01.DataContext = YTJ_XT_01;
                    view2.XT_02.DataContext = YTJ_XT_02;
                    view2.XT_03.DataContext = YTJ_XT_03;
                    view2.XT_04.DataContext = YTJ_XT_04;
                    view2.XT_05.DataContext = YTJ_XT_05;
                    view2.XT_06.DataContext = YTJ_XT_06;
                    view2.XT_07.DataContext = YTJ_XT_07;
                    view2.XT_08.DataContext = YTJ_XT_08;
                    view2.Show();
                }));
            });
            thread.IsBackground = true;
            thread.Start();
            // 传入的参数是线程的名称，也可以不用传。
            //SysConfigStart();
        }
        private async void SysConfigStart()
        {
            // 传入的参数是线程的名称，也可以不用传。
            var dispatcher = await UIDispatcher.RunNewAsync("Background UI");
            var backgroundWindow = await dispatcher.InvokeAsync(() =>
            {
                SysConfig_YTJ view2 = new SysConfig_YTJ() { DataContext = this };
                view2.Config.DataContext = YTJ_XT_01;
                view2.CameraList.ItemsSource = MV_Cameralist2;
                view2.CameraListRefresh.DataContext = this;
                view2.XT_01.DataContext = YTJ_XT_01;
                view2.XT_02.DataContext = YTJ_XT_02;
                view2.XT_03.DataContext = YTJ_XT_03;
                view2.XT_04.DataContext = YTJ_XT_04;
                view2.XT_05.DataContext = YTJ_XT_05;
                view2.XT_06.DataContext = YTJ_XT_06;
                view2.XT_07.DataContext = YTJ_XT_07;
                view2.XT_08.DataContext = YTJ_XT_08;
                view2.SourceInitialized += OnSourceInitialized;
                view2.Show();
                return view2;
            });
        }
        private void OnSourceInitialized(object sender, EventArgs e)
        {
            // 在这里可以获取到窗口句柄。
        }
        //系统设置按钮状态
        public bool SysConfig_Mode
        {
            get => GetProperty(() => SysConfig_Mode);
            set => SetProperty(() => SysConfig_Mode, value);
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
        //权限
        public bool IsDeveloperLogin_Dev
        {
            get => GetProperty(() => IsDeveloperLogin_Dev);
            set => SetProperty(() => IsDeveloperLogin_Dev, value);
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
        /// 开发者权限登陆命令
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        [AsyncCommand]
        public void DeveloperLoginCommand(object obj)
        {
            if (!IsDeveloperLogin)
            {
                DeveloperLoginView view = new DeveloperLoginView { DataContext = this };
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
            IsDeveloperLogin_Dev = false;
           
            APP_MODE = 0;

        }
      
        #endregion Command

        #region 工业相机

        /// <summary>
        /// 回调函数，回调帧为YUV格式
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Hik_MV_OnOriFrameInvoked(object sender, HikHelper.MV_IM_INFO e)
        {
            stp.QueueWorkItem(new Amib.Threading.Action<int, HikHelper.MV_IM_INFO>(MV_STPAction), stpIndex, e, Amib.Threading.WorkItemPriority.Normal);
        }

        /// <summary>
        /// 实例化bmpBuf
        /// </summary>
        public CVAlgorithms.BmpBuf currBmpBuf = new CVAlgorithms.BmpBuf();

        /// <summary>
        /// 实例化输出参数结构体
        /// </summary>
        public CVAlgorithms.Pare_Output pare_Output = new CVAlgorithms.Pare_Output();

        /// <summary>
        /// 设备在线数量
        /// </summary>
        public int MV_CameraNum
        {
            get => GetProperty(() => MV_CameraNum);
            set => SetProperty(() => MV_CameraNum, value);
        }
            
        /// <summary>
        /// 线程执行函数
        /// </summary>
        /// <param name="i"></param>
        /// <param name="e"></param>
        private void MV_STPAction(int i, HikHelper.MV_IM_INFO e)
        {
            // 传入数据指针头
            lock (objlock)
            { currBmpBuf.pData_IntPtr = e.pData; }

            lock (objlock)
            {
                //非PLC触发拍照
                if (YTJ_PLC_ShotFlag == 0)
                {
                    //吸头选择
                    YTJ_XT_ConfigModel YTJ_XT;
                    switch (YTJ_XT_01.YTJ_XTItems)
                    {
                        case 0: { YTJ_XT = YTJ_XT_01; }; break;
                        case 1: { YTJ_XT = YTJ_XT_02; }; break;
                        case 2: { YTJ_XT = YTJ_XT_03; }; break;
                        case 3: { YTJ_XT = YTJ_XT_04; }; break;
                        case 4: { YTJ_XT = YTJ_XT_05; }; break;
                        case 5: { YTJ_XT = YTJ_XT_06; }; break;
                        case 6: { YTJ_XT = YTJ_XT_07; }; break;
                        case 7: { YTJ_XT = YTJ_XT_08; }; break;
                        default: { YTJ_XT = YTJ_XT_01; }; break;
                    }
                  
                    int XT_Num = YTJ_XT.YTJ_XTItems;
                    int YTJ_SF_Flag = int.Parse(YTJ_XT.YTJ_XT_SF[0].NewData);
                    int BG = 0; int ZY = 0; int ZL = 0;
                    switch (YTJ_SF_Flag)
                    {
                        case 0: {YTJ_SFPara = YTJ_XT.JT_Config; CurrDvrInfo.YTJ_SFType = "镜筒识别"; CurrDvrInfo.ModNum_01 = YTJ_XT.YTJ_XT_JT[0].NewData; BG = int.Parse(YTJ_XT.YTJ_XT_JT[1].NewData); ZY = int.Parse(YTJ_XT.YTJ_XT_JT[2].NewData); ZL = int.Parse(YTJ_XT.YTJ_XT_JT[3].NewData); }; break;
                        case 1: { YTJ_SFPara = YTJ_XT.JP_Config; CurrDvrInfo.YTJ_SFType = "镜片识别"; CurrDvrInfo.ModNum_01 = YTJ_XT.YTJ_XT_JP[0].NewData; BG = int.Parse(YTJ_XT.YTJ_XT_JP[1].NewData); ZY = int.Parse(YTJ_XT.YTJ_XT_JP[2].NewData); ZL = int.Parse(YTJ_XT.YTJ_XT_JP[3].NewData); }; break;
                        case 2: { YTJ_SFPara = YTJ_XT.DGP_Config; CurrDvrInfo.YTJ_SFType = "挡光片识别"; CurrDvrInfo.ModNum_01 = YTJ_XT.YTJ_XT_DGP[0].NewData; BG = int.Parse(YTJ_XT.YTJ_XT_DGP[1].NewData); ZY = int.Parse(YTJ_XT.YTJ_XT_DGP[2].NewData); ZL = int.Parse(YTJ_XT.YTJ_XT_DGP[3].NewData); }; break;
                        case 3: { YTJ_SFPara = YTJ_XT.PJ_Config; CurrDvrInfo.YTJ_SFType = "判胶识别"; CurrDvrInfo.ModNum_01 = YTJ_XT.YTJ_XT_PJ[0].NewData; BG = int.Parse(YTJ_XT.YTJ_XT_PJ[1].NewData); ZY = int.Parse(YTJ_XT.YTJ_XT_PJ[2].NewData); ZL = int.Parse(YTJ_XT.YTJ_XT_PJ[3].NewData); }; break;
                        default:; break;
                    }
                    //设置相机参数
                    Hik.MV_CameraParaSet(0, BG, ZY, 5);
                    CVAlgorithms.MV_EntryPoint(YTJ_SF_Flag, e.height, e.width, ref currBmpBuf, YTJ_SFPara, ref YTJ_output_Parameter_Float[0]);

                    //界面更新
                    //拍照结果
                    if (YTJ_output_Parameter_Float[5] == 0) { CurrDvrInfo.OK_OR_NG = "OK"; }
                    else { CurrDvrInfo.OK_OR_NG = "NG"; }
                    //角度
                    YTJ_output_Parameter_Float[1] = YTJ_output_Parameter_Float[1] % 360;
                    CurrDvrInfo.Angle = String.Format("{0:F2}", YTJ_output_Parameter_Float[1]);
                    //偏差
                    CurrDvrInfo.Deviation_X = YTJ_output_Parameter_Float[3];
                    CurrDvrInfo.Deviation_Y = YTJ_output_Parameter_Float[4];
                    //吸头数量
                    CurrDvrInfo.YTJ_XTNums = int.Parse(YTJ_XT_01.YTJ_XT_SF[2].NewData) +1;
                    //当前吸头
                    CurrDvrInfo.YTJ_NowXT = "吸头" + (YTJ_XT_01.YTJ_XTItems+1).ToString();
                   

                }
                //PLC触发拍照
                else if(YTJ_PLC_ShotFlag > 0)
                {
                    
                     //吸头选择
                     YTJ_XT_ConfigModel YTJ_XT;
                    switch (YTJ_PLC_ShotFlag)
                    {
                        case 1: { YTJ_XT = YTJ_XT_01; }; break;
                        case 2: { YTJ_XT = YTJ_XT_02; }; break;
                        case 3: { YTJ_XT = YTJ_XT_03; }; break;
                        case 4: { YTJ_XT = YTJ_XT_04; }; break;
                        case 5: { YTJ_XT = YTJ_XT_05; }; break;
                        case 6: { YTJ_XT = YTJ_XT_06; }; break;
                        case 7: { YTJ_XT = YTJ_XT_07; }; break;
                        case 8: { YTJ_XT = YTJ_XT_08; }; break;
                        default: { YTJ_XT = YTJ_XT_01; }; break;
                    }
                    int YTJ_SF_Flag = int.Parse(YTJ_XT.YTJ_XT_SF[0].NewData);
                    ConsoleText += $"{CmdTag + "算法类型:" + YTJ_SF_Flag + Environment.NewLine}";

                    switch (YTJ_SF_Flag)
                    {
                        case 0: { YTJ_SFPara = YTJ_XT.JT_Config; CurrDvrInfo.YTJ_SFType = "镜筒识别"; CurrDvrInfo.ModNum_01 = YTJ_XT.YTJ_XT_JT[0].NewData;}; break;
                        case 1: { YTJ_SFPara = YTJ_XT.JP_Config; CurrDvrInfo.YTJ_SFType = "镜片识别"; CurrDvrInfo.ModNum_01 = YTJ_XT.YTJ_XT_JP[0].NewData; }; break;
                        case 2: { YTJ_SFPara = YTJ_XT.DGP_Config; CurrDvrInfo.YTJ_SFType = "挡光片识别"; CurrDvrInfo.ModNum_01 = YTJ_XT.YTJ_XT_DGP[0].NewData;  }; break;
                        case 3: { YTJ_SFPara = YTJ_XT.PJ_Config; CurrDvrInfo.YTJ_SFType = "判胶识别"; CurrDvrInfo.ModNum_01 = YTJ_XT.YTJ_XT_PJ[0].NewData; }; break;
                        default:; break;
                    }
                    CVAlgorithms.MV_EntryPoint(YTJ_SF_Flag, e.height, e.width, ref currBmpBuf, YTJ_SFPara, ref YTJ_output_Parameter_Float[0]);

                    //界面更新
                    //拍照结果      
                    if (YTJ_output_Parameter_Float[5] == 0) { CurrDvrInfo.OK_OR_NG = "OK"; }
                    else { CurrDvrInfo.OK_OR_NG = "NG"; }
                    //角度
                    YTJ_output_Parameter_Float[1] = YTJ_output_Parameter_Float[1] % 360;
                    CurrDvrInfo.Angle = String.Format("{0:F2}", YTJ_output_Parameter_Float[1]);
                    //偏差
                    
                    CurrDvrInfo.Deviation_X = YTJ_output_Parameter_Float[3];
                    CurrDvrInfo.Deviation_Y = YTJ_output_Parameter_Float[4];
                    //吸头数量
                    CurrDvrInfo.YTJ_XTNums = int.Parse(YTJ_XT_01.YTJ_XT_SF[2].NewData)+1;
                    //当前吸头
                    CurrDvrInfo.YTJ_NowXT = "吸头" + (YTJ_PLC_ShotFlag).ToString();
                    

                    //回复数据编码
                    //OK/NG信息
                    int OKNGMsg = 0;
                    if (YTJ_output_Parameter_Float[5] == 0) { OKNGMsg = 66;  }
                    else { OKNGMsg = 44; }
                    //角度信息
                    float Angle_Temp = YTJ_output_Parameter_Float[1] * 100;
                    int Angle = (int)(Angle_Temp);
                    //偏差
                    string Result_X_H = "", Result_Y_H = "";
                    double X_Temp = (YTJ_output_Parameter_Float[3] * 1000);
                    double Y_Temp = (YTJ_output_Parameter_Float[4] * 1000);
                    int Result_X = (int)X_Temp;
                    int Result_Y = (int)Y_Temp;
                    if (Result_X >= 0) { Result_X_H = "0000"; }
                    else { Result_X_H = "FFFF"; }
                    if (Result_Y >= 0) { Result_Y_H = "0000"; }
                    else { Result_Y_H = "FFFF"; }
                    //寄存器地址
                    int SendLocal01 = 30000;
                    int SendLocal02 = 30007;
                    //相机编号
                    int YTJ_GW = int.Parse(YTJ_XT_01.YTJ_XT_SF[10].NewData);
                    switch (YTJ_GW)
                    {
                        case 0: { SendLocal01 = 29100; SendLocal02 = 29107; }; break;
                        case 1: { SendLocal01 = 29200; SendLocal02 = 29207; }; break;
                        case 2: { SendLocal01 = 29400; SendLocal02 = 29407; }; break;
                        case 3: { SendLocal01 = 29500; SendLocal02 = 29507; }; break;
                        default:; break;
                    }
                    //编码
                    string Result_SendMsg = "";
                    Result_SendMsg = "%01#WDD" + SendLocal01.ToString() + SendLocal02.ToString() + SendSJEncoding(OKNGMsg) + SendSJEncoding(0) + SendSJEncoding(Angle) + SendSJEncoding(0) + SendSJEncoding(Result_X) + Result_X_H + SendSJEncoding(Result_Y) + Result_Y_H;
                    Result_SendMsg = PCSend_Client.BuildMSG(Result_SendMsg);

                    //发送
                    try
                    {
                        PCSend_Client.AsyncSend(Result_SendMsg);
                        Thread.Sleep(5);
                        PCSend_Client.AsyncSend(Result_SendMsg);
                        ConsoleText += $"{CmdTag + "回复信息:" + Result_SendMsg + Environment.NewLine}";
                        Thread.Sleep(10);
                    }
                    catch { ConsoleText += $"{CmdTag + "视觉结果发送失败" + Environment.NewLine}"; }

                    YTJ_PLC_ShotFlag = 0;
                }
            }

           
            //// 显示活动线程数
            //ActiveThreadNum = stp_JP.ActiveThreads + stp_JT.ActiveThreads + stp_DGP.ActiveThreads + stp_PJ.ActiveThreads + stp_SL.ActiveThreads;
            // 显示算法耗时
            time = YTJ_output_Parameter_Float[0];
            ElapsedTime = time;
            FPS = $"{Math.Round(1000.0f / time, 2) * ActiveThreadNum} FPS";
            // 显示在线设备数
            MV_CameraNum = e.CameraNum;

            //显示
            Application.Current.Dispatcher.Invoke(() =>
            {
                if (ImSrc_test == null || ImSrc_test.Width != currBmpBuf.Width || ImSrc_test.Height != currBmpBuf.Height)
                {
                    ImSrc_test = new WriteableBitmap(currBmpBuf.Width, currBmpBuf.Height, 96.0, 96.0, PixelFormats.Bgr24, null);
                }
                lock (objlock)
                {
                    //if ((currBmpBuf.size >= (e.width * e.height * 3)) && (currBmpBuf.pData != (IntPtr)0x00000000))
                    if ((currBmpBuf.pData != (IntPtr)0x00000000))
                    {
                        ImSrc_test.Lock();
                        ImSrc_test.WritePixels(new Int32Rect(0, 0, currBmpBuf.Width, currBmpBuf.Height), currBmpBuf.pData, currBmpBuf.size, ImSrc_test.BackBufferStride);
                        // Buffer.MemoryCopy(info.data.ToPointer(), ImSrc.BackBuffer.ToPointer(), info.size, info.size);
                        // CVAlgorithms.RtlMoveMemory(ImSrc.BackBuffer, info.data, (uint)info.size);
                        ImSrc_test.AddDirtyRect(new Int32Rect(0, 0, currBmpBuf.Width, currBmpBuf.Height));
                        ImSrc_test.Unlock();
                    }
                }
            });
            //释放内存
            lock (objlock)
            {
                CVAlgorithms.MV_Release(ref currBmpBuf);
            }
           
        }

        #region Command

        /// <summary>
        /// 开始采集        
        /// </summary>
        /// <param name="obj"></param>
        [AsyncCommand]
        public void MV_StartPlayCommand(object obj)
        {
           MV_StartPlay();
        }
        //控件调用
        public void MV_StartPlay()
        {
            try
            {
                //传入启动的相机编号
                int[] MV_IsplayCamera = new int[5];
                int MV_IsplayNum = 0;
                if (YTJ_XT_01.YTJ_XT_SF[0].NewData == "3")
                { MV_IsplayNum = 1; MV_IsplayCamera[0] = int.Parse(YTJ_XT_01.YTJ_XT_SF[1].NewData); MV_IsplayCamera[1] = -2; MV_IsplayCamera[2] = -2; MV_IsplayCamera[3] = -2; }
                else
                { MV_IsplayNum = 1; MV_IsplayCamera[0] = int.Parse(YTJ_XT_01.YTJ_XT_SF[1].NewData); MV_IsplayCamera[1] = -1; MV_IsplayCamera[2] = -1; MV_IsplayCamera[3] = -1; }
                Hik.StartPreview_Mulitple(MV_IsplayCamera, MV_IsplayNum);
                IsPlaying = true;
            }
            catch {
                ConsoleText += $"{CmdTag + "相机启动失败:" + Environment.NewLine}";
            }
        }

        /// <summary>
        ///     命令：停止播放
        /// </summary>
        /// <param name="obj"></param>
        [AsyncCommand]
        public void MV_StopPlayCommand(object obj)
        {
            MV_StopPlay();
        }
        //控件调用
        public void MV_StopPlay()
        {
            try
            {
                stp.Cancel();

                Thread.Sleep(100);

                Hik.MV_Stop();
                IsPlaying = false;
            }
            catch { ConsoleText += $"{CmdTag + "相机关闭失败:"  + Environment.NewLine}"; };
        }

        /// <summary>
        /// 退出程序
        /// </summary>
        /// <param name="obj"></param>
        [AsyncCommand]
        public void MV_ExitCommand(object obj)
        {
            Hik.MV_Dispose();
            IsPlaying = false;
            CloseAction();  // Calls Close() method of the View.
        }

        /// <summary>
        ///     命令：软触发
        /// </summary>
        /// <param name="obj"></param>
        [AsyncCommand]
        public void MV_SoftTrigger01Command(object obj)
        {
            //publicValue.Light_Client.AsyncSend(publicValue.Light_Client.Clients[SL01_JT.Light_TCP_IP + ":" + SL01_JT.Light_TCP_Port], "C3000SA0225#");
            
            Hik.MV_Camera_Switch(0);
            Hik.MV_Trigger_Soft(0, MV_CameraNum);       
        }
        //控件调用
        public void MV_SoftTrigger01Command()
        {
            Hik.MV_Camera_Switch(0);
            Hik.MV_Trigger_Soft(0, MV_CameraNum);   
        }
        /// <summary>
        /// 刷新相机
        /// </summary>
        /// <param name="obj"></param>
        [AsyncCommand]
        public void MV_RefreshCameraListCommand(object obj)
        {
            Hik.MV_DeviceListAcq();
        }
        private void MV_RefreshCameraListInvoked(object sender, HikHelper.MV_DEVICE_INFO_LIST e)
        {
            stp.QueueWorkItem(new Amib.Threading.Action<int, HikHelper.MV_DEVICE_INFO_LIST>(MV_RefreshCameraList), stpIndex, e, Amib.Threading.WorkItemPriority.Normal);
        }
        public class CameraList
        {
            public int ID { get; set; }
            public string CameraName { get; set; }
        }
        public ObservableCollection<CameraList> MV_Cameralist2
        {
            get => GetProperty(() => MV_Cameralist2);
            set => SetProperty(() => MV_Cameralist2, value);
        }
        private void MV_RefreshCameraList(int i, HikHelper.MV_DEVICE_INFO_LIST e)
        {
            ObservableCollection<CameraList> MV_Cameralist = new ObservableCollection<CameraList>();
            for (int a = 0; a < e.MV_CameraList.Length; ++a)
            {
                MV_Cameralist.Add(new CameraList { ID = a, CameraName = e.MV_CameraList[a] });
            }

            //刷新数据
            MV_Cameralist2 = MV_Cameralist;
            Thread.Sleep(50);
            //恢复数据
           
        }

        #endregion Command

        #endregion 工业相机

        #region 一体机

        #region Value

        //配置文件名称
        string YTJ_Config_Name = ".\\Para\\Camera01\\YTJ_Config.xml";

        // 配置文件读写器
        private XDocument YTJ_Config = XDocument.Load(".\\Para\\Camera01\\YTJ_Config.xml");
        // 配置文件读写器
        private XDocument GBJZ_Config = XDocument.Load("Config.xml");

        //吸头算法配置参数
        public YTJ_XT_ConfigModel YTJ_XT_01
        {
            get => GetProperty(() => YTJ_XT_01);
            set => SetProperty(() => YTJ_XT_01, value);
        }
        public YTJ_XT_ConfigModel YTJ_XT_02
        {
            get => GetProperty(() => YTJ_XT_02);
            set => SetProperty(() => YTJ_XT_02, value);
        }
        public YTJ_XT_ConfigModel YTJ_XT_03
        {
            get => GetProperty(() => YTJ_XT_03);
            set => SetProperty(() => YTJ_XT_03, value);
        }
        public YTJ_XT_ConfigModel YTJ_XT_04
        {
            get => GetProperty(() => YTJ_XT_04);
            set => SetProperty(() => YTJ_XT_04, value);
        }
        public YTJ_XT_ConfigModel YTJ_XT_05
        {
            get => GetProperty(() => YTJ_XT_05);
            set => SetProperty(() => YTJ_XT_05, value);
        }
        public YTJ_XT_ConfigModel YTJ_XT_06
        {
            get => GetProperty(() => YTJ_XT_06);
            set => SetProperty(() => YTJ_XT_06, value);
        }
        public YTJ_XT_ConfigModel YTJ_XT_07
        {
            get => GetProperty(() => YTJ_XT_07);
            set => SetProperty(() => YTJ_XT_07, value);
        }
        public YTJ_XT_ConfigModel YTJ_XT_08
        {
            get => GetProperty(() => YTJ_XT_08);
            set => SetProperty(() => YTJ_XT_08, value);
        }

        //算法输出参数
        public float[] YTJ_output_Parameter_Float = new float[30];

        //PLC触发拍照标志
        public int YTJ_PLC_ShotFlag = 0;

        //算法参数传递
        public string[] YTJ_SFPara = new string[30];

        //Socekt
        public AsyncTCPClient PLCShot_Client = new AsyncTCPClient();
        public AsyncTCPClient PCSend_Client = new AsyncTCPClient();
        public AsyncTCPClient PCHeart_Client = new AsyncTCPClient();

        //Socekt线程池
        private Amib.Threading.SmartThreadPool YTJ_stp_PLCShot;
        private Amib.Threading.SmartThreadPool YTJ_stp_PCSend;
        private Amib.Threading.SmartThreadPool YTJ_stp_PCHeart;

        #endregion

        #region BindingItems

        /// <summary>
        /// TCP连接情况
        /// </summary>
        public string TCP_Sta
        {
            get => GetProperty(() => TCP_Sta);
            set => SetProperty(() => TCP_Sta, value);
        }

        //输出信息等级
        public int YTJ_ColoseLevel
         {
            get => GetProperty(() => YTJ_ColoseLevel);
            set => SetProperty(() => YTJ_ColoseLevel, value, () =>
            {
                YTJ_Config.Descendants("YTJ_ColoseLevel").ElementAt(0).SetValue(value);
                YTJ_Config.Save(".\\Para\\Camera01\\YTJ_Config.xml");
            });
        }

        //广播信息
        public string GB_Msg
        {
            get => GetProperty(() => GB_Msg);
            set => SetProperty(() => GB_Msg, value);
        }
        public string GBJZ_Msg
        {
            get => GetProperty(() => GBJZ_Msg);
            set => SetProperty(() => GBJZ_Msg, value, () =>
            {
                GBJZ_Config.Descendants("GBJZ_Msg").ElementAt(0).SetValue(value);
                GBJZ_Config.Save("Config.xml");
            });
        }
        /// <summary>
        /// 连接指示灯
        /// </summary>
        public bool LinkLight
        {
            get => GetProperty(() => LinkLight);
            set => SetProperty(() => LinkLight, value);
        }

        /// <summary>
        /// 连接进度条
        /// </summary>
        public int LinkLoadFlag
        {
            get => GetProperty(() => LinkLoadFlag);
            set => SetProperty(() => LinkLoadFlag, value);
        }

        ////编号  
        public int[] YTJ_XT_ID
        {
            get => GetProperty(() => YTJ_XT_ID);
            set => SetProperty(() => YTJ_XT_ID, value);
        }
        /// <summary>
        /// 连接Command
        /// </summary>
        /// <param name="obj"></param>
        [AsyncCommand]
        public void YTJ_StartCommand(object obj)
        {

            YTJ_Start();
        }
        /// <summary>
        /// 断开Command
        /// </summary>
        /// <param name="obj"></param>
        [AsyncCommand]
        public void YTJ_StopCommand(object obj)
        {
            YTJ_Stop();
        }
        #endregion

        #region Method

        //初始化
        private void YTJ_CSH()
        {
            // 初始化池
            YTJ_stp_PLCShot = new Amib.Threading.SmartThreadPool
            {
                MaxThreads = 1
            };
            YTJ_stp_PCSend = new Amib.Threading.SmartThreadPool
            {
                MaxThreads = 1
            };
            YTJ_stp_PCHeart = new Amib.Threading.SmartThreadPool
            {
                MaxThreads = 1
            };

            //注册委托
            PLCShot_Client.ReceiveDataInvoked += PLCShot_ClientReceiveDataInvoked;
            PCSend_Client.ReceiveDataInvoked += PCSend_ClientReceiveDataInvoked;
            PCHeart_Client.ReceiveDataInvoked += PCHeart_ClientReceiveDataInvoked;

            YTJ_ColoseLevel = int.Parse(YTJ_Config.Descendants("YTJ_ColoseLevel").ElementAt(0).Value);
       
            GBJZ_Msg = GBJZ_Config.Descendants("GBJZ_Msg").ElementAt(0).Value;
            //吸头编号
            YTJ_XT_ID = new int[8];
            for (int i = 0; i < 8; i++)
            {
                YTJ_XT_ID[i] = i;
            }
            LinkLight = false;

            //广播信息
            GB_Msg = "0";

            //连接进度条
            LinkLoadFlag = 0;
        }
        //启动检测
        public void YTJ_Start()
        {
            //连接进度条
            LinkLoadFlag = 1;
            //心跳包编码
            string HeartMsg = "";
            string CLSendMsg = "";
            //相机编号
            int YTJ_GW = int.Parse(YTJ_XT_01.YTJ_XT_SF[10].NewData);
            switch (YTJ_GW)
            {
                case 0: { HeartMsg = PCSend_Client.BuildMSG("%01#WDD29600296030800080008000800"); CLSendMsg = PCSend_Client.BuildMSG("%01#RDD6160461604"); }; break;
                case 1: { HeartMsg = PCSend_Client.BuildMSG("%01#WDD29600296030800080008000800"); CLSendMsg = PCSend_Client.BuildMSG("%01#RDD6160261602"); }; break;
                case 2: { HeartMsg = PCSend_Client.BuildMSG("%01#WDD29600296030800080008000800"); CLSendMsg = PCSend_Client.BuildMSG("%01#RDD6160061600"); }; break;
                case 3: { HeartMsg = PCSend_Client.BuildMSG("%01#WDD29600296030800080008000800"); CLSendMsg = "test"; }; break;
                default:; break;
            }
            string HeartMsg2 = PCSend_Client.BuildMSG("Heart");
            string HeartMsg3 = ("Heart");

            //连接PLC
            try
            {
                if (TCP_Sta != "连接")
                {
                    //设置为触发模式
                    Hik.MV_SoftTrigger_Chance();
                   
                    //界面显示
                    TCP_Sta = "连接";
                    LinkLight = true;
                    //清空PLC拍照标志
                    YTJ_PLC_ShotFlag = 0;
                    //设置IP和Port
                    PLCShot_Client.IP = YTJ_XT_01.YTJ_XT_SF[3].NewData;
                    PLCShot_Client.Port = int.Parse(YTJ_XT_01.YTJ_XT_SF[4].NewData);
                    PCSend_Client.IP = YTJ_XT_01.YTJ_XT_SF[5].NewData;
                    PCSend_Client.Port = int.Parse(YTJ_XT_01.YTJ_XT_SF[6].NewData);
                    PCHeart_Client.IP = YTJ_XT_01.YTJ_XT_SF[7].NewData;
                    PCHeart_Client.Port = int.Parse(YTJ_XT_01.YTJ_XT_SF[8].NewData);
                    //连接
                    Thread.Sleep(10);
                    PLCShot_Client.Connect();
                    Thread.Sleep(10);
                    PCSend_Client.Connect();
                    Thread.Sleep(10);
                    PCHeart_Client.Connect();

                    Thread.Sleep(100);
                    //维持连接
                    PLCShot_Client.ResultAsyncSend_AutoStrat(HeartMsg3, 2000);
                    Thread.Sleep(10);
                    PCSend_Client.AsyncSend_AutoStrat(HeartMsg2, 2000);
                    Thread.Sleep(10);
                    PCHeart_Client.AsyncSend_AutoStrat(HeartMsg, 2000);
                    Thread.Sleep(10);
                    PCHeart_Client.AsyncSend_AutoStrat(CLSendMsg, 1333);

                    Thread.Sleep(50);
                }
            }
            catch
            {
                TCP_Sta = "断开";
                LinkLight = false;
                PLCShot_Client.ClentClose();
                PCSend_Client.ClentClose();
                PCHeart_Client.ClentClose();
            }
            //连接进度条
            LinkLoadFlag =0;
        }
        //断开检测
        public void YTJ_Stop()
        {
            //界面显示
            TCP_Sta = "断开";
            LinkLight = false;
            //清空PLC拍照标志
            YTJ_PLC_ShotFlag = 0;
            //设置IP和Port
            PLCShot_Client.ClentClose();
            PCSend_Client.ClentClose();
            PCHeart_Client.ClentClose();
            //设置为连续模式
            Hik.MV_NotTrigger_Chance();
        }
        //Socekt处理事件
        //PLC触发拍照
        private void PLCShot_ClientReceiveDataInvoked(object sender, AsyncTCPClient.ReceiveData e)
        {
            YTJ_stp_PLCShot.QueueWorkItem(new Amib.Threading.Action<int, AsyncTCPClient.ReceiveData>(PLCShot_ClientRefreshData), 0, e, Amib.Threading.WorkItemPriority.Normal);
        }
        private void PLCShot_ClientRefreshData(int i, AsyncTCPClient.ReceiveData e)
        {

            lock (objlock)
            {
                char[] ReceiveData = e.ReceiveData_s.ToCharArray();

                if (ReceiveData[0] == '~')
                {
                    ConsoleText += $"{CmdTag + "PLC触发Socekt连接成功" + e.ReceiveData_s + Environment.NewLine}";
                }
                else if (ReceiveData[0] == '`')
                {
                    ConsoleText += $"{CmdTag + "PLC触发Socekt连接失败" + Environment.NewLine}";
                }
                else if (ReceiveData[0] != '%')
                {
                    ConsoleText = "";
                    ConsoleText += $"{CmdTag + "接收到触发信号:" + e.ReceiveData_s + Environment.NewLine}";
                    if (ReceiveData[4].ToString() == "0")
                    {
                        //PLC触发拍照
                        YTJ_PLC_ShotFlag = int.Parse(ReceiveData[5].ToString());
                        //设置相机参数
                       
                        //吸头选择
                        YTJ_XT_ConfigModel YTJ_XT;
                        switch (YTJ_PLC_ShotFlag)
                        {
                            case 1: { YTJ_XT = YTJ_XT_01; }; break;
                            case 2: { YTJ_XT = YTJ_XT_02; }; break;
                            case 3: { YTJ_XT = YTJ_XT_03; }; break;
                            case 4: { YTJ_XT = YTJ_XT_04; }; break;
                            case 5: { YTJ_XT = YTJ_XT_05; }; break;
                            case 6: { YTJ_XT = YTJ_XT_06; }; break;
                            case 7: { YTJ_XT = YTJ_XT_07; }; break;
                            case 8: { YTJ_XT = YTJ_XT_08; }; break;
                            default: { YTJ_XT = YTJ_XT_01; }; break;
                        }
                        int BG = 0; int ZY = 0; int ZL = 0;
                        int YTJ_SF_Flag = int.Parse(YTJ_XT.YTJ_XT_SF[0].NewData);
                        switch (YTJ_SF_Flag)
                        {
                            case 0: { BG = int.Parse(YTJ_XT.YTJ_XT_JT[1].NewData); ZY = int.Parse(YTJ_XT.YTJ_XT_JT[2].NewData); ZL = int.Parse(YTJ_XT.YTJ_XT_JT[3].NewData); }; break;
                            case 1: { BG = int.Parse(YTJ_XT.YTJ_XT_JP[1].NewData); ZY = int.Parse(YTJ_XT.YTJ_XT_JP[2].NewData); ZL = int.Parse(YTJ_XT.YTJ_XT_JP[3].NewData); }; break;
                            case 2: { BG = int.Parse(YTJ_XT.YTJ_XT_DGP[1].NewData); ZY = int.Parse(YTJ_XT.YTJ_XT_DGP[2].NewData); ZL = int.Parse(YTJ_XT.YTJ_XT_DGP[3].NewData); }; break;
                            case 3: { BG = int.Parse(YTJ_XT.YTJ_XT_PJ[1].NewData); ZY = int.Parse(YTJ_XT.YTJ_XT_PJ[2].NewData); ZL = int.Parse(YTJ_XT.YTJ_XT_PJ[3].NewData); }; break;
                            default:; break;
                        }
                        Hik.MV_CameraParaSet(0, BG, ZY, ZL);
                        //触发拍照
                        ConsoleText += $"{CmdTag + "触发吸头拍照:" + YTJ_PLC_ShotFlag + Environment.NewLine}";
                        MV_SoftTrigger01Command();

                    }
                }

            }
        }
        //PC回复PLC
        private void PCSend_ClientReceiveDataInvoked(object sender, AsyncTCPClient.ReceiveData e)
        {
            YTJ_stp_PCSend.QueueWorkItem(new Amib.Threading.Action<int, AsyncTCPClient.ReceiveData>(PCSend_ClientRefreshData), 0, e, Amib.Threading.WorkItemPriority.Normal);
        }
        private void PCSend_ClientRefreshData(int i, AsyncTCPClient.ReceiveData e)
        {

            lock (objlock)
            {
                char[] ReceiveData = e.ReceiveData_s.ToCharArray();

                if (ReceiveData[0] == '~')
                {
                    ConsoleText += $"{CmdTag + "PC回复Socekt连接成功" + e.ReceiveData_s + Environment.NewLine}";
                }
                else if (ReceiveData[0] == '`')
                {
                    ConsoleText += $"{CmdTag + "PC回复Socekt连接失败" + Environment.NewLine}";
                }
                if (ReceiveData[3] == '$' && ReceiveData[4] == 'W')
                {
                    Console.WriteLine("PC回复数据结果发送成功" + e.ReceiveData_s);
                    //ConsoleText += $"{CmdTag + "数据结果发送成功" + Environment.NewLine}";
                }
                else if (ReceiveData[3] == '!')
                {
                    Console.WriteLine("PC回复数据结果发送失败" + e.ReceiveData_s);
                    ConsoleText += $"{CmdTag + "PC回复数据结果发送失败" + e.ReceiveData_s + Environment.NewLine}";
                }

            }
        }
        //心跳
        private void PCHeart_ClientReceiveDataInvoked(object sender, AsyncTCPClient.ReceiveData e)
        {
            YTJ_stp_PCHeart.QueueWorkItem(new Amib.Threading.Action<int, AsyncTCPClient.ReceiveData>(PCHeart_ClientRefreshData), 0, e, Amib.Threading.WorkItemPriority.Normal);
        }
        private void PCHeart_ClientRefreshData(int i, AsyncTCPClient.ReceiveData e)
        {

            lock (objlock)
            {
                char[] ReceiveData = e.ReceiveData_s.ToCharArray();

                if (ReceiveData[0] == '~')
                {
                    ConsoleText += $"{CmdTag + "心跳Socekt连接成功" + e.ReceiveData_s + Environment.NewLine}";
                }
                else if (ReceiveData[0] == '`')
                {
                    ConsoleText += $"{CmdTag + "心跳Socekt连接失败" + Environment.NewLine}";
                }
                if (ReceiveData[3] == '$' && ReceiveData[4] == 'R' )
                {
                    string OKNum_16 = "0x" + ReceiveData[8].ToString() + ReceiveData[9].ToString() + ReceiveData[6].ToString() + ReceiveData[7].ToString();
                    int OKNum_10 = System.Convert.ToInt32(OKNum_16, 16);
                    GB_Msg = OKNum_10.ToString();
   
                }

            }
        }
        //写指令编码
        public string SendMessageEncoding(string JCQ, int SJ)
        {
            string Message = "";
            if (SJ >= 0)
            {
                Message = Convert.ToString(SJ, 16).ToUpper();
                if (Message.Length < 4)
                {
                    for (int i = 0; Message.Length < 4; i++)
                    {
                        { Message = "0" + Message; }
                    }
                }
            }
            else
            {

                Message = Convert.ToString(SJ, 16).ToUpper();
                Message = Message.Substring(4);
                if (Message.Length < 4)
                {
                    for (int i = 0; Message.Length < 4; i++)
                    {
                        { Message = "0" + Message; }
                    }
                }
            }
            string Message_H, Message_L, MessageSend;
            Message_L = Message.Substring(2);
            Message_H = Message.Substring(0, 2);
            MessageSend = "%01#WDD" + JCQ + JCQ + Message_L + Message_H;
            return MessageSend;
        }
        public string SendSJEncoding(int SJ)
        {
            string Message = "";
            if (SJ >= 0)
            {
                Message = Convert.ToString(SJ, 16).ToUpper();
                if (Message.Length < 4)
                {
                    for (int i = 0; Message.Length < 4; i++)
                    {
                        { Message = "0" + Message; }
                    }
                }
            }
            else
            {
                Message = Convert.ToString(SJ, 16).ToUpper();
                Message = Message.Substring(4);
                if (Message.Length < 4)
                {
                    for (int i = 0; Message.Length < 4; i++)
                    {
                        { Message = "0" + Message; }
                    }
                }
            }
            string Message_H, Message_L, MessageSend;
            Message_L = Message.Substring(2);
            Message_H = Message.Substring(0, 2);
            MessageSend = Message_L + Message_H;
            return MessageSend;
        }
        //手动刷新界面
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

        #endregion

        #endregion


    }


}