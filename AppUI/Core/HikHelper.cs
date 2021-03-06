using AppUI.HikSDK;
using AppUI.Models;
using Hikvision;
using PlayM4;
using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Threading;
using MvCamCtrl.NET;
using System.Windows.Interop;
using System.Collections.Generic;

namespace AppUI.Core
{
    public class HikHelper : DispatcherObject
    {
        #region 安防摄像机

            #region Private field

            /// <summary>
            /// 错误代码
            /// </summary>
            private uint iLastErr = 0;
            /// <summary>
            /// 账号登录状态
            /// </summary>
            private Int32 m_lUserID = -1;
            /// <summary>
            /// 设备信息
            /// </summary>
            private CHCNetSDK.NET_DVR_DEVICEINFO_V30 m_struDeviceInfo;
            /// <summary>
            /// 是否已经初始化SDK
            /// </summary>
            private bool m_bInitSDK = false;
            /// <summary>
            /// 实时画面显示句柄
            /// </summary>
            private Int32 m_lRealHandle = -1;
            /// <summary>
            /// 播放通道号
            /// </summary>
            private Int32 m_lPort = -1;

            /// <summary>
            /// 画面绘图回调函数
            /// </summary>
            //private PlayCtrl.DRAWFUN m_fDrawFun = null;

            /// <summary>
            /// 用于播放实时预览的窗口的指针，即picturebox控件
            /// </summary>
            private readonly IntPtr m_ptrRealHandle = IntPtr.Zero;

            /// <summary>
            /// 实时预览数据回调函数
            /// </summary>
            private CHCNetSDK.REALDATACALLBACK RealData = null;

            /// <summary>
            /// 解码视频回调函数
            /// </summary>
            // private PlayCtrl.DECCBFUN m_fDisplayFun = null;
            private PlayCtrl.DISPLAYCBFUN m_fDisplayFun = null;

            #endregion

            #region Public field

            ///// <summary>
            ///// 路由事件，回调帧给Image控件
            ///// </summary>
            //public event EventHandler<ImageSource> OnFrameInvoked;

            /// <summary>
            /// 图像结构体
            /// </summary>
            public struct IM_INFO
            {
                public IntPtr pData;
                public int width;
                public int height;
                public int size;
                public int FrameRate;
            }

            /// <summary>
            /// 路由事件，回调原始帧
            /// </summary>
            public event EventHandler<IM_INFO> OnOriFrameInvoked;
            
            /// <summary>
            /// 路由事件，回调信息显示
            /// </summary>
            public event EventHandler<string> OnShowLogInvoked;

            /// <summary>
            /// 解码器句柄
            /// </summary>
            private IntPtr m_decoderHandle;

            private byte[] buf;
            private IntPtr pBuf;

            #endregion

            #region Constructor

            public HikHelper()
            {
          
            }

            #endregion

            #region Public method field

            /// <summary>
            /// 初始化预览
            /// </summary>
            public void PreviewInit()
            {
               
            }

            /// <summary>
            /// 登录设备
            /// </summary>
            /// <param name="ip"></param>
            /// <param name="port"></param>
            /// <param name="userName"></param>
            /// <param name="password"></param>
            public bool DeviceLogin(string ip, string port, string userName, string password)
            {
                if (string.IsNullOrWhiteSpace(ip) || string.IsNullOrWhiteSpace(port) ||
                    string.IsNullOrWhiteSpace(userName) || string.IsNullOrWhiteSpace(password))
                {
                    MessageBox.Show("请输入IP，端口，用户名和密码！");
                    return false;
                }
                if (m_lUserID < 0)
                {
                    m_struDeviceInfo = new CHCNetSDK.NET_DVR_DEVICEINFO_V30();
                    // 登录设备
                    m_lUserID = CHCNetSDK.NET_DVR_Login_V30(ip, Int16.Parse(port), userName, password, ref m_struDeviceInfo);
                    if (m_lUserID < 0)
                    {
                        // 登录失败
                        iLastErr = CHCNetSDK.NET_DVR_GetLastError();
                        MessageBox.Show("NET_DVR_Login_V30 failed, error code = " + iLastErr);
                        return false;
                    }
                    else
                    {
                        // 登录成功
                        MessageBox.Show("登陆成功 !", "信息", MessageBoxButton.OKCancel, MessageBoxImage.Information);
                    }
                }
                return true;
            }

            /// <summary>
            /// 注销登录
            /// </summary>
            /// <returns></returns>
            public bool DeviceLogout()
            {
                // 注销登录
                if (m_lRealHandle >= 0)
                {
                    MessageBox.Show("Please stop live view firstly !");
                    return false;
                }

                if (!CHCNetSDK.NET_DVR_Logout(m_lUserID))
                {
                    iLastErr = CHCNetSDK.NET_DVR_GetLastError();
                    MessageBox.Show("NET_DVR_Logout failed, error code = " + iLastErr);
                    return false;
                }
                m_lUserID = -1;
                return true;
            }

            /// <summary>
            /// 启动预览
            /// </summary>
            /// <param name="RealPlayWnd"></param>
            /// <returns></returns>
            public bool StartPreview()
            {
                if (m_lUserID < 0)
                {
                    MessageBox.Show("Please login the device firstly");
                    return false;
                }

                if (m_lRealHandle < 0)
                {
                    CHCNetSDK.NET_DVR_PREVIEWINFO lpPreviewInfo = new CHCNetSDK.NET_DVR_PREVIEWINFO
                    {
                        lChannel = 1,               // 预览的设备通道
                        dwStreamType = 0,           // 码流类型：0-主码流，1-子码流，2-码流3，3-码流4，以此类推
                        dwLinkMode = 0,             // 连接方式：0- TCP方式，1- UDP方式，2- 多播方式，3- RTP方式，4-RTP/RTSP，5-RSTP/HTTP 
                        bBlocked = true,            // 0- 非阻塞取流，1- 阻塞取流
                        dwDisplayBufNum = 1,        // 播放库播放缓冲区最大缓冲帧数
                        byProtoType = 0,            // 应用层取流协议：0- 私有协议，1- RTSP协议
                        byPreviewMode = 0           // 预览模式：0- 正常预览，1- 延迟预览
                    };

                    if (RealData == null)
                    {
                        // 创建预览实时流回调函数
                        RealData = new CHCNetSDK.REALDATACALLBACK(RealDataCallBack);
                    }

                    // 指向用户数据的指针
                    IntPtr pUser = new IntPtr();
                    // 打开预览
                    m_lRealHandle = CHCNetSDK.NET_DVR_RealPlay_V40(m_lUserID, ref lpPreviewInfo, RealData/*这个参数是实时数据回调函数*/, pUser);
                    if (m_lRealHandle < 0)
                    {
                        iLastErr = CHCNetSDK.NET_DVR_GetLastError();
                        MessageBox.Show("NET_DVR_RealPlay_V40 failed, error code = " + iLastErr);
                        OnShowLogInvoked?.Invoke(this, $"[Error] Func: CHCNetSDK.NET_DVR_RealPlay_V40 Code: {iLastErr}");
                        return false;
                    }
                    else
                    {
                        // 预览成功，设置截图模式
                        CHCNetSDK.NET_DVR_SetCapturePictureMode((uint)CHCNetSDK.CAPTURE_MODE.BMP_MODE);
                        return true;
                    }
                }
                return true;
            }

            /// <summary>
            /// 停止预览
            /// </summary>
            /// <param name="RealPlayWnd"></param>
            /// <returns></returns>
            public bool StopPreview()
            {
                if (m_lUserID < 0)
                {
                    MessageBox.Show("Please login the device firstly");
                    return false;
                }

                if (m_lRealHandle >= 0)
                {
                    if (!CHCNetSDK.NET_DVR_StopRealPlay(m_lRealHandle))
                    {
                        iLastErr = CHCNetSDK.NET_DVR_GetLastError();
                        MessageBox.Show("NET_DVR_StopRealPlay failed, error code = " + iLastErr);
                        OnShowLogInvoked?.Invoke(this, $"[Error] Func: CHCNetSDK.NET_DVR_StopRealPlay Code: {iLastErr}");
                        return false;
                    }
                    if (m_lPort >= 0)
                    {
                        if (!PlayCtrl.PlayM4_Stop(m_lPort))
                        {
                            iLastErr = PlayCtrl.PlayM4_GetLastError(m_lPort);
                            MessageBox.Show("PlayM4_Stop failed, error code = " + iLastErr);
                            OnShowLogInvoked?.Invoke(this, $"[Error] Func: PlayCtrl.PlayM4_Stop Code: {iLastErr}");
                            return false;
                        }
                        if (!PlayCtrl.PlayM4_CloseStream(m_lPort))
                        {
                            iLastErr = PlayCtrl.PlayM4_GetLastError(m_lPort);
                            MessageBox.Show("PlayM4_CloseStream failed, error code = " + iLastErr);
                            OnShowLogInvoked?.Invoke(this, $"[Error] Func: PlayCtrl.PlayM4_CloseStream Code: {iLastErr}");
                            return false;
                        }
                        if (!PlayCtrl.PlayM4_FreePort(m_lPort))
                        {
                            iLastErr = PlayCtrl.PlayM4_GetLastError(m_lPort);
                            MessageBox.Show("PlayM4_FreePort failed, error code = " + iLastErr);
                            OnShowLogInvoked?.Invoke(this, $"[Error] Func: PlayCtrl.PlayM4_FreePort Code: {iLastErr}");
                            return false;
                        }
                        m_lPort = -1;
                    }

                    m_lRealHandle = -1;
                }
                return true;
            }

            /// <summary>
            /// 清理所有正在使用的资源
            /// </summary>
            /// <param name="disposing"></param>
            public void Dispose(bool disposing)
            {


            if (m_lRealHandle >= 0)
            {
                CHCNetSDK.NET_DVR_StopRealPlay(m_lRealHandle);
            }
            if (m_lUserID >= 0)
            {
                CHCNetSDK.NET_DVR_Logout(m_lUserID);
            }
            if (m_bInitSDK == true)
            {
                CHCNetSDK.NET_DVR_Cleanup();
            }
        }

            /// <summary>
            /// 使用海康SDK进行截图
            /// </summary>
            /// <returns></returns>
            public bool CapturePicture2()
            {
                if (m_lRealHandle < 0)
                {
                    MessageBox.Show("Please start live view firstly after capture.");
                    OnShowLogInvoked?.Invoke(this, $"[Error] Func: CapturePicture2 Msg: Please start live view firstly after capture.");
                    return false;
                }
                // 图片保存路径和文件名
                string sBmpPicFileName = @"./CapturePicArea/" + DateTime.Now.ToString("yyyyMMddHHmmss") + ".bmp";
                // 使用BMP格式保存
                if (!CHCNetSDK.NET_DVR_CapturePicture(m_lRealHandle, sBmpPicFileName))
                {
                    iLastErr = CHCNetSDK.NET_DVR_GetLastError();
                    MessageBox.Show("NET_DVR_CapturePicture failed, error code = " + iLastErr);
                    OnShowLogInvoked?.Invoke(this, $"[Error] Func: CHCNetSDK.NET_DVR_CapturePicture Code: {iLastErr}");
                    return false;
                }
                else
                {
                    OnShowLogInvoked?.Invoke(this, $"[Log] Successful screenshot and the saved file is {sBmpPicFileName}");
                }
                return true;
            }

            /// <summary>
            /// 使用PlayM4_GetBMP函数实现截图功能
            /// </summary>
            /// <returns></returns>
            public bool CapturePicture()
            {
                // 图片保存路径和文件名
                string sBmpPicFileName = @"./CapturePicArea/" + DateTime.Now.ToString("yyyyMMddHHmmss") + ".bmp";

                int iWidth = 0, iHeight = 0;
                uint iActualSize = 0;

                if (!PlayCtrl.PlayM4_GetPictureSize(m_lPort, ref iWidth, ref iHeight))
                {
                    iLastErr = PlayCtrl.PlayM4_GetLastError(m_lPort);
                    MessageBox.Show("PlayM4_GetPictureSize failed, error code = " + iLastErr);
                    OnShowLogInvoked?.Invoke(this, $"[Error] Func: PlayCtrl.PlayM4_GetPictureSize Code: {iLastErr}");
                    return false;
                }

                uint nBufSize = (uint)(iWidth * iHeight) * 5;
                byte[] pBitmap = new byte[nBufSize];
                if (!PlayCtrl.PlayM4_GetBMP(m_lPort, pBitmap, nBufSize, ref iActualSize))
                {
                    iLastErr = PlayCtrl.PlayM4_GetLastError(m_lPort);
                    MessageBox.Show("PlayM4_GetBMP failed, error code = " + iLastErr);
                    OnShowLogInvoked?.Invoke(this, $"[Error] Func: PlayCtrl.PlayM4_GetBMP Code: {iLastErr}");
                    return false;
                }
                else
                {
                    FileStream fs = new FileStream(sBmpPicFileName, FileMode.Create);
                    fs.Write(pBitmap, 0, (int)iActualSize);
                    fs.Close();
                    // Console.WriteLine("PlayM4_GetBMP succ and the saved file is " + sBmpPicFileName);
                    OnShowLogInvoked?.Invoke(this, $"[Log] PlayM4_GetBMP succ and the saved file is {sBmpPicFileName}");
                }

                MessageBox.Show("截图成功，文件保存到：" + sBmpPicFileName + "。");
                return true;
            }

            #endregion

            #region private method field

            /// <summary>
            /// 实时数据回调函数
            /// </summary>
            /// <param name="lRealHandle"></param>
            /// <param name="dwDataType"></param>
            /// <param name="pBuffer"></param>
            /// <param name="dwBufSize"></param>
            /// <param name="pUser"></param>
            private void RealDataCallBack(int lRealHandle, uint dwDataType, IntPtr pBuffer, uint dwBufSize, IntPtr pUser)
            {
                #region PlayM4

                //switch (dwDataType)
                //{
                //    case CHCNetSDK.NET_DVR_SYSHEAD:     // sys head
                //        if (dwBufSize > 0)
                //        {
                //            if (m_lPort >= 0)
                //            {
                //                // 同一路码流不需要多次调用开流接口
                //                return;
                //            }
                //            // 获取播放句柄
                //            if (!PlayCtrl.PlayM4_GetPort(ref m_lPort))
                //            {
                //                iLastErr = PlayCtrl.PlayM4_GetLastError(m_lPort);
                //                MessageBox.Show("PlayM4_GetPort failed, error code = " + iLastErr);
                //                break;
                //            }
                //            // 设置流播放模式
                //            if (!PlayCtrl.PlayM4_SetStreamOpenMode(m_lPort, PlayCtrl.STREAME_REALTIME/*流播放模式，0：STREAM_REALTIME*/))
                //            {
                //                iLastErr = PlayCtrl.PlayM4_GetLastError(m_lPort);
                //                MessageBox.Show("Set STREAME_REALTIME mode failed, error code = " + iLastErr);
                //                break;
                //            }
                //            // 打开码流，送入头数据
                //            if (!PlayCtrl.PlayM4_OpenStream(m_lPort, pBuffer, dwBufSize, 10 * 1024 * 1024/*缓冲数据流缓冲 1024*50到1024*100000*/))
                //            {
                //                iLastErr = PlayCtrl.PlayM4_GetLastError(m_lPort);
                //                MessageBox.Show("PlayM4_OpenStream failed, error code = " + iLastErr);
                //                break;
                //            }

                //            // 设置解码回调函数，获取解码后的音视频原始数据，解码视频数据格式为YV12
                //            //m_fDisplayFun = new PlayCtrl.DECCBFUN(DecCallbackFUN);
                //            //if (!PlayCtrl.PlayM4_SetDecCallBackEx(m_lPort, m_fDisplayFun, IntPtr.Zero, 0))
                //            //{
                //            //    iLastErr = PlayCtrl.PlayM4_GetLastError(m_lPort);
                //            //    MessageBox.Show("PlayM4_SetDecCallBackEx failed, error code = " + iLastErr);
                //            //    break;
                //            //}

                //            // 回调
                //            m_fDisplayFun = new PlayCtrl.DISPLAYCBFUN(RemoteDisplayCBFun);
                //            if (!PlayCtrl.PlayM4_SetDisplayCallBack(m_lPort, m_fDisplayFun))
                //            {
                //                iLastErr = PlayCtrl.PlayM4_GetLastError(m_lPort);
                //                MessageBox.Show("PlayM4_SetDisplayCallBack failed, error code = " + iLastErr);
                //                break;
                //            }

                //            // 设置画图回调函数
                //            //m_fDrawFun = new PlayCtrl.DRAWFUN(DrawCallBackFun);
                //            //if (!PlayCtrl.PlayM4_RigisterDrawFun(m_lPort, m_fDrawFun, m_lUserID))
                //            //{
                //            //    iLastErr = PlayCtrl.PlayM4_GetLastError(m_lPort);
                //            //    MessageBox.Show("PlayM4_RigisterDrawFun failed, error code = " + iLastErr);
                //            //    break;
                //            //}

                //            // 进行解码操作
                //            if (!PlayCtrl.PlayM4_Play(m_lPort, m_ptrRealHandle))
                //            {
                //                iLastErr = PlayCtrl.PlayM4_GetLastError(m_lPort);
                //                MessageBox.Show("PlayM4_Play failed, error code = " + iLastErr);
                //                break;
                //            }

                //        }
                //        break;

                //    // 视频流数据
                //    case CHCNetSDK.NET_DVR_STREAMDATA:     // video stream data
                //        if (dwBufSize > 0 && m_lPort != -1)
                //        {
                //            for (int i = 0; i < 999; i++)
                //            {
                //                if (!PlayCtrl.PlayM4_InputData(m_lPort, pBuffer, dwBufSize))
                //                {
                //                    iLastErr = PlayCtrl.PlayM4_GetLastError(m_lPort);
                //                    Console.Error.WriteLine("PlayM4_Play failed, error code = " + iLastErr);
                //                    Thread.Sleep(2);
                //                }
                //                else
                //                {
                //                    break;
                //                }
                //            }
                //        }
                //        break;
                //    default:
                //        if (dwBufSize > 0 && m_lPort != -1)
                //        {
                //            // 送入其他数据
                //            for (int i = 0; i < 999; i++)
                //            {
                //                if (!PlayCtrl.PlayM4_InputData(m_lPort, pBuffer, dwBufSize))
                //                {
                //                    iLastErr = PlayCtrl.PlayM4_GetLastError(m_lPort);
                //                    Console.Error.WriteLine("PlayM4_InputData failed, error code = " + iLastErr);
                //                    Thread.Sleep(2);
                //                }
                //                else
                //                {
                //                    break;
                //                }
                //            }
                //        }
                //        break;
                //}

                #endregion

                #region Hik_DSFilters
                //System.Diagnostics.Stopwatch stopwatch = new System.Diagnostics.Stopwatch();
                switch (dwDataType)
                {
                    case CHCNetSDK.NET_DVR_SYSHEAD:
                        HikDecoderInterface.HK_STD_CreateHandle(pBuffer, dwBufSize, 10 * 1024 * 1024, ref m_decoderHandle);
                        HikDecoderInterface.HK_STD_SetSkipAudio(m_decoderHandle, true);
                        break;
                    case CHCNetSDK.NET_DVR_STREAMDATA:
                        if (dwBufSize > 0)
                        {
                            for (int i = 0; i < 999; i++)
                            {
                                //stopwatch.Start(); //  开始监视代码

                                if (!HikDecoderInterface.HK_STD_InputData(m_decoderHandle, pBuffer, dwBufSize))
                                {
                                    uint errorCode = 0;
                                    HikDecoderInterface.HK_STD_GetLastError(m_decoderHandle, ref errorCode);
                                    OnShowLogInvoked?.Invoke(this, $"[Error] Func: HikDecoderInterface.HK_STD_InputData Code: {errorCode}");
                                    Thread.Sleep(2);
                                }
                                else
                                {
                                    break;
                                }
                            }
                        }
                        break;
                    default:
                        if (dwBufSize > 0)
                        {
                            for (int i = 0; i < 999; i++)
                            {
                                if (!HikDecoderInterface.HK_STD_InputData(m_decoderHandle, pBuffer, dwBufSize))
                                {
                                    uint errorCode = 0;
                                    HikDecoderInterface.HK_STD_GetLastError(m_decoderHandle, ref errorCode);
                                    OnShowLogInvoked?.Invoke(this, $"[Error] Func: HikDecoderInterface.HK_STD_InputData Code: {errorCode}");
                                    Thread.Sleep(2);
                                }
                                else
                                {
                                    break;
                                }
                            }
                        }
                        break;
                }

                HikDecoderInterface.STD_FRAME_INFO stFrameInfo = new HikDecoderInterface.STD_FRAME_INFO();
                while (HikDecoderInterface.HK_STD_GetOneFrame(m_decoderHandle, ref stFrameInfo))
                {
                    if (1 == stFrameInfo.dwFrameType)
                    {
                        //stopwatch.Stop(); //  停止监视
                        //TimeSpan timeSpan = stopwatch.Elapsed; //  获取总时间
                        //Console.WriteLine($"Decode time: {timeSpan.TotalMilliseconds} ms.");

                        lock (this)
                        {
                            // 拷贝中间内存区域
                            if (buf == null)
                            {
                                buf = new byte[stFrameInfo.dwFrameSize];
                                pBuf = Marshal.UnsafeAddrOfPinnedArrayElement(buf, 0);
                            }
                            Marshal.Copy(stFrameInfo.pFrameBuffer, buf, 0, (int)stFrameInfo.dwFrameSize);
                        }

                        Console.WriteLine($"pBuf:0x{pBuf.ToString("X8")} Size:{buf.Length}");
                        Console.WriteLine($"pBuffer:0x{pBuffer.ToString("X8")} Size:{dwBufSize} Type:{stFrameInfo.dwFrameType} Size:{stFrameInfo.dwFrameSize} pData:0x{stFrameInfo.pFrameBuffer.ToString("X8")} width:{stFrameInfo.dwWidthOrChannels} height:{stFrameInfo.dwHeightOrBPS}");

                        Dispatcher.BeginInvoke(new MShow(Show),
                            new IM_INFO
                            {
                                // pData = stFrameInfo.pFrameBuffer,
                                pData = pBuf,
                                width = (int)stFrameInfo.dwWidthOrChannels,
                                height = (int)stFrameInfo.dwHeightOrBPS,
                                size = (int)stFrameInfo.dwFrameSize
                            });

                    }
                }

                #endregion
            }

            /// <summary>
            /// 画面绘图回调函数
            /// </summary>
            /// <param name="nPort"></param>
            /// <param name="hDc"></param>
            /// <param name="nUser"></param>
            private void DrawCallBackFun(int nPort, IntPtr hDc, int nUser)
            {
                //Graphics gra = Graphics.FromHdc(hDc);
                //DateTime now = DateTime.Now;
                //gra.DrawString(now.ToString(), new Font("Times New Roman", 20, FontStyle.Bold), Brushes.Red, new Point(100, 20));
            }

            /// <summary>
            /// 解码回调函数
            /// 此处调用的代码为自己的算法处理代码，帮助类只提供框架
            /// </summary>
            /// <param name="nPort"></param>
            /// <param name="pBuf"></param>
            /// <param name="nSize"></param>
            /// <param name="pFrameInfo"></param>
            /// <param name="nReserved1"></param>
            /// <param name="nReserved2"></param>
            private void DecCallbackFUN(int nPort, IntPtr pBuf, int nSize, ref PlayCtrl.FRAME_INFO pFrameInfo, int nReserved1, int nReserved2)
            {
                Dispatcher.BeginInvoke(new MShow(Show),
                    new IM_INFO
                    {
                        pData = pBuf,
                        width = pFrameInfo.nWidth,
                        height = pFrameInfo.nHeight,
                        size = nSize
                    });
            }

            /// <summary>
            /// PlayCtrl显示回调函数
            /// </summary>
            /// <param name="nPort"></param>
            /// <param name="pBuf"></param>
            /// <param name="nSize"></param>
            /// <param name="nWidth"></param>
            /// <param name="nHeight"></param>
            /// <param name="nStamp"></param>
            /// <param name="nType"></param>
            /// <param name="nReserved"></param>
            private void RemoteDisplayCBFun(int nPort, IntPtr pBuf, int nSize, int nWidth, int nHeight, int nStamp, int nType, int nReserved)
            {
                //uint frame = PlayCtrl.PlayM4_GetCurrentFrameRate(nPort);
                //Dispatcher.Invoke(new MShow(Show), new IM_INFO { pData = pBuf, width = nWidth, height = nHeight, size = nSize, FrameRate = (int)frame });
                Dispatcher.BeginInvoke(new MShow(Show),
                    new IM_INFO
                    {
                        pData = pBuf,
                        width = nWidth,
                        height = nHeight,
                        size = nSize
                    });
            }

            /// <summary>
            /// 原始
            /// </summary>
            /// <param name="pBuf"></param>
            private void Show(IM_INFO imInfo)
            {
                OnOriFrameInvoked?.Invoke(this, imInfo);
            }
          

            #endregion

            #region D3D render core

            //[DllImport("gdi32.dll")]
            //private static extern int DeleteObject(IntPtr hdc);

            private delegate void MShow(IM_INFO imInfo);
          
            //private delegate void MShow2(IntPtr pBuf);

            ///// <summary>
            ///// D3D
            ///// </summary>
            ///// <param name="pBuf"></param>
            //private void Show2(IntPtr pBuf)
            //{
            //    wbSource.Render(pBuf);
            //    OnFrameInvoked?.Invoke(this, wbSource.ImageSource);

            //    DeleteObject(pBuf);
            //    GC.Collect();

            //}

            #endregion

            #region Test

            public bool GetDvrInfo(DVRModel dvr)
            {
                // 获取曝光信息
                CHCNetSDK.NET_DVR_CAMERAPARAMCFG net_dvr_cameraparamcfg = new CHCNetSDK.NET_DVR_CAMERAPARAMCFG();
                int size = Marshal.SizeOf(net_dvr_cameraparamcfg);
                IntPtr ptrCfg = Marshal.AllocHGlobal(size);
                uint buffSize = 0;
                if (!CHCNetSDK.NET_DVR_GetDVRConfig(m_lUserID, CHCNetSDK.NET_DVR_GET_CCDPARAMCFG, 0, ptrCfg, (uint)size, ref buffSize))
                {
                    uint iLastErr = CHCNetSDK.NET_DVR_GetLastError();
                    MessageBox.Show("NET_DVR_RealPlay_V40 failed, error code = " + iLastErr);
                    return false;
                }
                net_dvr_cameraparamcfg = Marshal.PtrToStructure<CHCNetSDK.NET_DVR_CAMERAPARAMCFG>(ptrCfg);

             

                return true;
            }

        #endregion

        #endregion

        #region 工业相机

        /// <summary>
        /// 显示错误信息
        /// </summary>
        /// <returns></returns>
        private void ShowErrorMsg(string csMessage, int nErrorNum)
        {
            string errorMsg;
            if (nErrorNum == 0)
            {
                errorMsg = csMessage;
            }
            else
            {
                errorMsg = csMessage + ": Error =" + String.Format("{0:X}", nErrorNum);
            }

            switch (nErrorNum)
            {
                case MyCamera.MV_E_HANDLE: errorMsg += " Error or invalid handle "; break;
                case MyCamera.MV_E_SUPPORT: errorMsg += " Not supported function "; break;
                case MyCamera.MV_E_BUFOVER: errorMsg += " Cache is full "; break;
                case MyCamera.MV_E_CALLORDER: errorMsg += " Function calling order error "; break;
                case MyCamera.MV_E_PARAMETER: errorMsg += " Incorrect parameter "; break;
                case MyCamera.MV_E_RESOURCE: errorMsg += " Applying resource failed "; break;
                case MyCamera.MV_E_NODATA: errorMsg += " No data "; break;
                case MyCamera.MV_E_PRECONDITION: errorMsg += " Precondition error, or running environment changed "; break;
                case MyCamera.MV_E_VERSION: errorMsg += " Version mismatches "; break;
                case MyCamera.MV_E_NOENOUGH_BUF: errorMsg += " Insufficient memory "; break;
                case MyCamera.MV_E_UNKNOW: errorMsg += " Unknown error "; break;
                case MyCamera.MV_E_GC_GENERIC: errorMsg += " General error "; break;
                case MyCamera.MV_E_GC_ACCESS: errorMsg += " Node accessing condition error "; break;
                case MyCamera.MV_E_ACCESS_DENIED: errorMsg += " No permission "; break;
                case MyCamera.MV_E_BUSY: errorMsg += " Device is busy, or network disconnected "; break;
                case MyCamera.MV_E_NETER: errorMsg += " Network error "; break;
            }

            msgthr.Start(errorMsg +" 工位：" + GW_Num);
        }
        //弹窗线程
        Thread msgthr = new Thread(new ParameterizedThreadStart(msgtxt =>
        {
            MessageBox.Show((string)msgtxt, "异常", MessageBoxButton.OK, MessageBoxImage.Error, MessageBoxResult.OK, MessageBoxOptions.DefaultDesktopOnly);
        }));
        #region 单相机
        /// <summary>
        /// 相机列表
        /// </summary>
        /// <returns></returns>
        MyCamera.MV_CC_DEVICE_INFO_LIST m_pDeviceList;
        /// <summary>
        /// 相机类
        /// </summary>
        /// <returns></returns>
        private MyCamera m_pMyCamera;
        /// <summary>
        /// 相机标志位
        /// </summary>
        /// <returns></returns>
        private bool m_bGrabbing;
        /// <summary>
        /// 创建回调函数
        /// </summary>
        /// <returns></returns>
        private MyCamera.cbOutputExdelegate ImageCallback;

        /// <summary>
        /// 连接相机（单相机）
        /// </summary>
        /// <returns></returns>
        public bool StartPreview_test()
        {

            m_pDeviceList = new MyCamera.MV_CC_DEVICE_INFO_LIST();
            MyCamera.MV_CC_EnumDevices_NET(MyCamera.MV_GIGE_DEVICE | MyCamera.MV_USB_DEVICE, ref m_pDeviceList);
            if ((int)m_pDeviceList.nDeviceNum > 1)
            {
                Console.WriteLine("两个相机");
            }
            MyCamera.MV_CC_DEVICE_INFO device = (MyCamera.MV_CC_DEVICE_INFO)Marshal.PtrToStructure(m_pDeviceList.pDeviceInfo[0], typeof(MyCamera.MV_CC_DEVICE_INFO));
            // 打开相机
            m_pMyCamera = new MyCamera();
            m_pMyCamera.MV_CC_CreateDevice_NET(ref device);
            m_pMyCamera.MV_CC_OpenDevice_NET();
            m_pMyCamera.MV_CC_SetEnumValue_NET("AcquisitionMode", (uint)2);// ch:工作在连续模式 | en:Acquisition On Continuous Mode
            m_pMyCamera.MV_CC_SetEnumValue_NET("TriggerMode", (uint)0);    // ch:连续模式 | en:Continuous                                                                
            m_pMyCamera.MV_CC_SetIntValue_NET("GevHeartbeatTimeout", (uint)500);//心跳时间
            m_pMyCamera.MV_CC_SetEnumValue_NET("PixelFormat", 0x01080001);
            m_pMyCamera.MV_CC_SetFloatValue_NET("AcquisitionFrameRate", (float)30);

            //硬件触发
            //m_pMyCamera.MV_CC_SetEnumValue_NET("TriggerMode", (uint)1);
            //m_pMyCamera.MV_CC_SetEnumValue_NET("TriggerSource", (uint)0);
            //m_pMyCamera.MV_CC_SetFloatValue_NET("TriggerDelay", (float)0);
            //m_pMyCamera.MV_CC_SetBoolValue_NET("TriggerCacheEnable",true);

            //软触发
            //m_pMyCamera.MV_CC_SetEnumValue_NET("TriggerMode", (uint)1);
            //m_pMyCamera.MV_CC_SetEnumValue_NET("TriggerSource", 7);

            int nRet;
            ImageCallback = new MyCamera.cbOutputExdelegate(CallbackRGB);
            nRet = m_pMyCamera.MV_CC_RegisterImageCallBackEx_NET(ImageCallback, IntPtr.Zero);
            nRet = m_pMyCamera.MV_CC_StartGrabbing_NET();
            //MessageBox.Show(nRet.ToString());
            m_bGrabbing = true;

            // m_pMyCamera.MV_CC_Display_NET();
            return true;
        }
        #endregion

        #region 多相机
        //工位编号
        public int GW_Num = 0;

        /// <summary>
        /// 多相机结构体
        /// </summary>
        public struct CAMERA
        {
            public MyCamera Cam_Info;
            //public UInt32 m_nBufSizeForSaveImage;
            //public byte[] m_pBufForSaveImage;         // 用于保存图像的缓存
        }

        /// <summary>
        /// 相机列表
        /// </summary>
        /// <returns></returns>
        MyCamera.MV_CC_DEVICE_INFO_LIST m_pDeviceList_Mulitple;

        /// <summary>
        /// 声明多相机回调调用函数
        /// </summary>
        List<MyCamera.cbOutputExdelegate> ltCbImages = new List<MyCamera.cbOutputExdelegate>();
        /// <summary>
        /// 声明异常回调调用函数
        /// </summary>
        List<MyCamera.cbExceptiondelegate> ltErrors = new List<MyCamera.cbExceptiondelegate>();

        #region 多设备参数
        public int Flag_Switch;   //相机切换标识
        int m_nCanOpenDeviceNum;        // ch:设备使用数量 | en:Used Device Number
        int m_nDevNum;        // ch:在线设备数量 | en:Online Device Number
        int[] m_nFrames;      // ch:帧数 | en:Frame Number
        bool m_bTimerFlag;     // ch:定时器开始计时标志位 | en:Timer Start Timing Flag Bit
        bool[] m_bSaveImg;    // ch:保存图片标志位 | en:Save Image Flag Bit
        public int Flag_Trigger_Chance = 0; //设置触发方式标识符
        int Flag_lianjie;
        #endregion

        /// <summary>
        /// 实例化相机结构体
        /// </summary>
        /// <returns></returns>
        private CAMERA[] m_pMyCamera_Mulitple = new CAMERA[10];


        /// <summary>
        /// 连接相机（多相机）
        /// </summary>
        /// <returns></returns>
        public void StartPreview_Mulitple(int[] MV_IsplayCamera, int MV_IsplayNum)
        {

            Flag_lianjie = 1;
            Flag_Switch = 0;
            int nRet;
            //相机列表
            m_pDeviceList_Mulitple = new MyCamera.MV_CC_DEVICE_INFO_LIST();
            //枚举设备
            nRet = MyCamera.MV_CC_EnumDevices_NET(MyCamera.MV_GIGE_DEVICE | MyCamera.MV_USB_DEVICE, ref m_pDeviceList_Mulitple);
          
            if (0 != nRet)
            {
                ShowErrorMsg("Enumerate devices fail!" + " 工位：" + GW_Num,  0);
                
                return;
            }

            m_nDevNum = (int)m_pDeviceList_Mulitple.nDeviceNum;

            if (m_nDevNum == 0 || m_nDevNum == -1)
            {
                ShowErrorMsg("No device, please select" + " 工位：" + GW_Num,  0);
                return;
            }

            for (int a = 0, b = 0 ; a < 3; ++a) 
            {
                for (int j = 0; j < m_nDevNum; ++j)
                {
                    if (j == MV_IsplayCamera[a])
                    {
                        m_pMyCamera_Mulitple[b] = new CAMERA();
                        MyCamera.MV_CC_DEVICE_INFO device =
                            (MyCamera.MV_CC_DEVICE_INFO)Marshal.PtrToStructure(m_pDeviceList_Mulitple.pDeviceInfo[j],
                                                                      typeof(MyCamera.MV_CC_DEVICE_INFO));
                        //ch:打开设备 | en:Open Device
                        if (null == m_pMyCamera_Mulitple[b].Cam_Info)
                        {
                            m_pMyCamera_Mulitple[b].Cam_Info = new MyCamera();

                        }
                        nRet = m_pMyCamera_Mulitple[b].Cam_Info.MV_CC_CreateDevice_NET(ref device);
                        nRet = m_pMyCamera_Mulitple[b].Cam_Info.MV_CC_OpenDevice_NET();

                        if (MyCamera.MV_OK != nRet)
                        {
                            ShowErrorMsg("Device open fail!" + " 工位：" + GW_Num,  nRet);
                            m_pMyCamera_Mulitple[b].Cam_Info.MV_CC_DestroyDevice_NET();
                            MV_IsplayNum--;
                         
                        }
                        else
                        {
                            m_nCanOpenDeviceNum++;
                            m_pMyCamera_Mulitple[b].Cam_Info.MV_CC_SetEnumValue_NET("TriggerMode", 0);
                            m_pMyCamera_Mulitple[b].Cam_Info.MV_CC_SetBoolValue_NET("TriggerCacheEnable", false);
                            m_pMyCamera_Mulitple[b].Cam_Info.MV_CC_SetIntValue_NET("GevHeartbeatTimeout", (uint)500);//心跳时间
                            if (MV_IsplayCamera[2] == -2)
                            {
                                m_pMyCamera_Mulitple[b].Cam_Info.MV_CC_SetEnumValue_NET("BalanceWhiteAuto", 0);
                                m_pMyCamera_Mulitple[b].Cam_Info.MV_CC_SetEnumValue_NET("BalanceRatioSelector", 0);
                                //m_pMyCamera_Mulitple[b].Cam_Info.MV_CC_SetIntValue_NET("BalanceRatio", (uint)3000);
                                //m_pMyCamera_Mulitple[b].Cam_Info.MV_CC_SetEnumValue_NET("BalanceRatioSelector", 1);
                                //m_pMyCamera_Mulitple[b].Cam_Info.MV_CC_SetIntValue_NET("BalanceRatio", (uint)3000);
                                //m_pMyCamera_Mulitple[b].Cam_Info.MV_CC_SetEnumValue_NET("BalanceRatioSelector", 2);
                                //m_pMyCamera_Mulitple[b].Cam_Info.MV_CC_SetIntValue_NET("BalanceRatio", (uint)3000);
                                m_pMyCamera_Mulitple[b].Cam_Info.MV_CC_SetEnumValue_NET("PixelFormat", 0x02180014);
                            }
                            else
                            {

                                m_pMyCamera_Mulitple[b].Cam_Info.MV_CC_SetEnumValue_NET("PixelFormat", 0x01080001);
                            }

                            m_pMyCamera_Mulitple[b].Cam_Info.MV_CC_SetFloatValue_NET("AcquisitionFrameRate", (float)5);

                            //注册回调
                            ltCbImages.Add(new MyCamera.cbOutputExdelegate(Callback_M));
                            m_pMyCamera_Mulitple[b].Cam_Info.MV_CC_RegisterImageCallBackEx_NET(ltCbImages[b], (IntPtr)b);
                            //开始采集
                            nRet = m_pMyCamera_Mulitple[b].Cam_Info.MV_CC_StartGrabbing_NET();
                            //注册异常回调函数
                            ltErrors.Add(new MyCamera.cbExceptiondelegate(Callback_Error));
                            m_pMyCamera_Mulitple[b].Cam_Info.MV_CC_RegisterExceptionCallBack_NET(ltErrors[b], (IntPtr)b);
                        }

                        b++;
                    }

                }  
            }
            m_nDevNum = MV_IsplayNum;
            Flag_Switch = 0;
        }

        #endregion

        #region 回调路由
        /// <summary>
        /// 图像结构体
        /// </summary>
        public struct MV_IM_INFO
        {
            public IntPtr pData;
            public int width;
            public int height;
            public int pUser;
            public int CameraNum;

        }
        /// <summary>
        /// 路由事件，回调原始帧
        /// </summary>
        public event EventHandler<MV_IM_INFO> MV_OnOriFrameInvoked;
        /// <summary>
        /// 单相机回调函数
        /// </summary>
        /// <param name="pData"></param>
        /// <param name="pFrameInfo"></param>
        /// <param name="pUser"></param>
        private void CallbackRGB(IntPtr pData, ref MyCamera.MV_FRAME_OUT_INFO_EX pFrameInfo, IntPtr pUser)
        {
            if ((int)pUser == 1)
            {
                Console.WriteLine("01");
                //lock (this)
                //{
                //    // 拷贝中间内存区域
                //    if (buf == null)
                //    {
                //        buf = new byte[pFrameInfo.nFrameLen];
                //        pBuf = Marshal.UnsafeAddrOfPinnedArrayElement(buf, 0);
                //    }
                //    Marshal.Copy(pData, buf, 0, (int)pFrameInfo.nFrameLen);
                //}
                //Dispatcher.BeginInvoke(new MShow_Test(Show_Test),
                //              new IM_INFO_Test
                //              {
                //                  pData_Test = pBuf,
                //                  width = (int)pFrameInfo.nWidth,
                //                  height = (int)pFrameInfo.nHeight,
                //                  pUser_test = (int)pUser,
                //              });

            }
            if ((int)pUser == 0)
            {
                Console.WriteLine("00");
                lock (this)
                {
                    // 拷贝中间内存区域
                    if (buf == null)
                    {
                        buf = new byte[pFrameInfo.nFrameLen];
                        pBuf = Marshal.UnsafeAddrOfPinnedArrayElement(buf, 0);
                    }
                    Marshal.Copy(pData, buf, 0, (int)pFrameInfo.nFrameLen);
                }
                Dispatcher.BeginInvoke(new MV_MShow(MV_Show),
                              new MV_IM_INFO
                              {
                                  pData = pBuf,
                                  width = (int)pFrameInfo.nWidth,
                                  height = (int)pFrameInfo.nHeight,
                                  pUser = (int)pUser,
                              });

            }
            // Console.WriteLine(pData.ToString());
            //Console.WriteLine($"帧：0x{pData.ToString("X8")} 缓冲：0x{pBuf.ToString("X8")}");
        }

        /// <summary>
        /// 多相机回调函数
        /// </summary>
        /// <param name="pData"></param>
        /// <param name="pFrameInfo"></param>
        /// <param name="pUser"></param>
        private void Callback_M(IntPtr pData, ref MyCamera.MV_FRAME_OUT_INFO_EX pFrameInfo, IntPtr pUser)
        {
            if (((int)pUser == 1) && (Flag_Switch == 1))
            {

                lock (this)
                {
                    // 拷贝中间内存区域
                    if (buf == null || buf.Length != pFrameInfo.nFrameLen)
                    {
                        buf = new byte[pFrameInfo.nFrameLen];
                        pBuf = Marshal.UnsafeAddrOfPinnedArrayElement(buf, 0);
                    }
                    Marshal.Copy(pData, buf, 0, (int)pFrameInfo.nFrameLen);
                }
                Dispatcher.BeginInvoke(new MV_MShow(MV_Show),
                              new MV_IM_INFO
                              {
                                  pData = pBuf,
                                  width = (int)pFrameInfo.nWidth,
                                  height = (int)pFrameInfo.nHeight,
                                  pUser = (int)pUser,
                                  CameraNum = (int)m_nDevNum,
                              });
            }
            if (((int)pUser == 0) && (Flag_Switch == 0))
            {

                lock (this)
                {
                    // 拷贝中间内存区域
                    if (buf == null || buf.Length != pFrameInfo.nFrameLen)
                    {
                        buf = new byte[pFrameInfo.nFrameLen];
                        pBuf = Marshal.UnsafeAddrOfPinnedArrayElement(buf, 0);
                    }
                    Marshal.Copy(pData, buf, 0, (int)pFrameInfo.nFrameLen);
                }
                Dispatcher.BeginInvoke(new MV_MShow(MV_Show),
                              new MV_IM_INFO
                              {
                                  pData = pBuf,
                                  width = (int)pFrameInfo.nWidth,
                                  height = (int)pFrameInfo.nHeight,
                                  pUser = (int)pUser,
                                  CameraNum = (int)m_nDevNum,
                              });

            }

            if (((int)pUser == 2) && (Flag_Switch == 2))
            {

                lock (this)
                {
                    // 拷贝中间内存区域
                    if (buf == null || buf.Length != pFrameInfo.nFrameLen)
                    {
                        buf = new byte[pFrameInfo.nFrameLen];
                        pBuf = Marshal.UnsafeAddrOfPinnedArrayElement(buf, 0);
                    }
                    Marshal.Copy(pData, buf, 0, (int)pFrameInfo.nFrameLen);
                }
                Dispatcher.BeginInvoke(new MV_MShow(MV_Show),
                              new MV_IM_INFO
                              {
                                  pData = pBuf,
                                  width = (int)pFrameInfo.nWidth,
                                  height = (int)pFrameInfo.nHeight,
                                  pUser = (int)pUser,
                                  CameraNum = (int)m_nDevNum,
                              });

            }
            // Console.WriteLine(pData.ToString());
            //    //Console.WriteLine($"帧：0x{pData.ToString("X8")} 缓冲：0x{pBuf.ToString("X8")}");
            //}
        }

        //异常回调函数
        private void Callback_Error(UInt32 nMsgType, IntPtr pUser)
        {
            int nRet;

            try
            {
                ShowErrorMsg("相机进入异常回调重连!" + " 工位：" + GW_Num, 0);
                nRet = m_pMyCamera_Mulitple[(int)pUser].Cam_Info.MV_CC_CloseDevice_NET();
                Thread.Sleep(10);
                nRet = m_pMyCamera_Mulitple[(int)pUser].Cam_Info.MV_CC_OpenDevice_NET();
                nRet = m_pMyCamera_Mulitple[(int)pUser].Cam_Info.MV_CC_SetIntValue_NET("GevHeartbeatTimeout", (uint)500);//心跳时间
                nRet = m_pMyCamera_Mulitple[(int)pUser].Cam_Info.MV_CC_StartGrabbing_NET();
                if (0 != nRet)
                {
                    ShowErrorMsg("Device open fail!" + " 工位：" + GW_Num  , nRet); 
                }
              
            }
            catch { ShowErrorMsg("重连失败!" + " 工位：" + GW_Num , 0); }


        }

        private void MV_Show(MV_IM_INFO imInfo)
        {
            MV_OnOriFrameInvoked?.Invoke(this, imInfo);
        }

        private delegate void MV_MShow(MV_IM_INFO imInfo);

        //相机列表回调
        public struct MV_DEVICE_INFO_LIST
        {
            public string[] MV_CameraList;
        }
        public event EventHandler<MV_DEVICE_INFO_LIST> MV_CameraListInvoked;
        private void MV_CameraListShow(MV_DEVICE_INFO_LIST MV_CameraLists)
        {
            MV_CameraListInvoked?.Invoke(this, MV_CameraLists);
        }
        private delegate void MV_MCameraListShow(MV_DEVICE_INFO_LIST MV_CameraLists);
        #endregion

        #region 按钮操作

        /// <summary>
        /// 枚举设备
        /// </summary>
        public void MV_DeviceListAcq()
        {
            //枚举设备
            int nRet;
            nRet = MyCamera.MV_CC_EnumDevices_NET(MyCamera.MV_GIGE_DEVICE | MyCamera.MV_USB_DEVICE, ref m_pDeviceList_Mulitple);

            if (0 != nRet)
            {
                ShowErrorMsg("Enumerate devices fail!", 0);

                return;
            }

            string[] Cameralist = new string[m_pDeviceList_Mulitple.nDeviceNum];
            for (int i = 0; i < m_pDeviceList_Mulitple.nDeviceNum; i++)
            {
                MyCamera.MV_CC_DEVICE_INFO device = (MyCamera.MV_CC_DEVICE_INFO)Marshal.PtrToStructure(m_pDeviceList_Mulitple.pDeviceInfo[i], typeof(MyCamera.MV_CC_DEVICE_INFO));
                if (device.nTLayerType == MyCamera.MV_GIGE_DEVICE)
                {
                    IntPtr buffer = Marshal.UnsafeAddrOfPinnedArrayElement(device.SpecialInfo.stGigEInfo, 0);
                    MyCamera.MV_GIGE_DEVICE_INFO gigeInfo = (MyCamera.MV_GIGE_DEVICE_INFO)Marshal.PtrToStructure(buffer, typeof(MyCamera.MV_GIGE_DEVICE_INFO));
                    if (gigeInfo.chUserDefinedName != "")
                    {
                        Cameralist[i] = ("Camera" + i.ToString() + " :GigE: " + gigeInfo.chUserDefinedName + " (" + gigeInfo.chSerialNumber + ")");
                    }
                    else
                    {
                        Cameralist[i] = ("Camera" + i.ToString() + " :GigE: " + gigeInfo.chManufacturerName + " " + gigeInfo.chModelName + " (" + gigeInfo.chSerialNumber + ")");
                    }
                }
                else if (device.nTLayerType == MyCamera.MV_USB_DEVICE)
                {
                    IntPtr buffer = Marshal.UnsafeAddrOfPinnedArrayElement(device.SpecialInfo.stUsb3VInfo, 0);
                    MyCamera.MV_USB3_DEVICE_INFO usbInfo = (MyCamera.MV_USB3_DEVICE_INFO)Marshal.PtrToStructure(buffer, typeof(MyCamera.MV_USB3_DEVICE_INFO));
                    if (usbInfo.chUserDefinedName != "")
                    {
                        Cameralist[i] = ("Camera" + i.ToString() + " :USB: " + usbInfo.chUserDefinedName + " (" + usbInfo.chSerialNumber + ")");
                    }
                    else
                    {
                        Cameralist[i] = ("Camera" + i.ToString() + " :USB: " + usbInfo.chManufacturerName + " " + usbInfo.chModelName + " (" + usbInfo.chSerialNumber + ")");
                    }
                }
            }

            Dispatcher.BeginInvoke(new MV_MCameraListShow(MV_CameraListShow), new MV_DEVICE_INFO_LIST
            {
                MV_CameraList = Cameralist,
            });
        }

     
        /// <summary>
        /// 清理所有正在使用的资源
        /// </summary>
        public void MV_Dispose()
        {
            try
            {
                for (int i = 0, j = 0; j < m_nDevNum; ++i, ++j)
                {
                    m_pMyCamera_Mulitple[i].Cam_Info.MV_CC_StopGrabbing_NET();
                    m_pMyCamera_Mulitple[i].Cam_Info.MV_CC_CloseDevice_NET();
                    m_pMyCamera_Mulitple[i].Cam_Info.MV_CC_DestroyDevice_NET();
                }
                //m_pMyCamera.MV_CC_StopGrabbing_NET();
                m_bGrabbing = false;
            }
            catch { ShowErrorMsg("相机关闭失败!" + " 工位：" + GW_Num, 0); };
        }

        /// <summary>
        /// 软触发
        /// </summary>
        /// <returns></returns>
        public void MV_Trigger_Soft(int Cam_Num, int MV_CameraNum)
        {
            try
            {
                if (Cam_Num < MV_CameraNum)
                {
                    Console.WriteLine("触发");
                    int nRet = m_pMyCamera_Mulitple[Cam_Num].Cam_Info.MV_CC_SetCommandValue_NET("TriggerSoftware");
                    if (0 != nRet)
                    {
                        ShowErrorMsg("相机触发拍照失败!" + " 工位：" + GW_Num, nRet);
                    }

                }
            }
            catch { };
        }

        /// <summary>
        /// 切换相机
        /// </summary>
        /// <returns></returns>
        /// <param name="Flag_Switch">切换相机标识符</param>
        public void MV_Camera_Switch(int Switch_Num)
        {
  
            //if (Flag_Trigger_Chance == 0)
            //{
            //    if (Flag_Switch == 1)
            //    {
            //        Flag_Switch = 0;
            //    }
            //    else if (Flag_Switch == 0)
            //    {
            //        Flag_Switch = 1;
            //    }
            //}
            //else if (Flag_Trigger_Chance == 1)
            //{
            //    Flag_Switch = Switch_Num;
            //}
            Flag_Switch = Switch_Num;
        }

        /// <summary>
        /// 设置为触发方式
        /// </summary>
        public void MV_SoftTrigger_Chance()
        {
            try
            {
                for (int i = 0, j = 0; j < m_nDevNum; ++i, ++j)
                {
                    //硬件触发
                    //m_pMyCamera.MV_CC_SetEnumValue_NET("TriggerMode", (uint)1);
                    //m_pMyCamera.MV_CC_SetEnumValue_NET("TriggerSource", (uint)0);
                    //m_pMyCamera.MV_CC_SetFloatValue_NET("TriggerDelay", (float)0);
                    //m_pMyCamera.MV_CC_SetBoolValue_NET("TriggerCacheEnable",true);

                    //软触发

                    m_pMyCamera_Mulitple[i].Cam_Info.MV_CC_SetEnumValue_NET("TriggerMode", (uint)1);
                    m_pMyCamera_Mulitple[i].Cam_Info.MV_CC_SetEnumValue_NET("TriggerSource", 7);
                    m_pMyCamera_Mulitple[i].Cam_Info.MV_CC_SetBoolValue_NET("TriggerCacheEnable", true);
                    m_pMyCamera_Mulitple[i].Cam_Info.MV_CC_SetFloatValue_NET("AcquisitionFrameRate", (float)30);
                }
            }
            catch {
                ShowErrorMsg("设置触发失败" + " 工位：" + GW_Num, 0);
            }
        }
        /// <summary>
        /// 设置为连续方式
        /// </summary>
        public void MV_NotTrigger_Chance()
        {
            for (int i = 0, j = 0; j < m_nDevNum; ++i, ++j)
            {
                m_pMyCamera_Mulitple[i].Cam_Info.MV_CC_SetEnumValue_NET("TriggerMode", (uint)0);
                m_pMyCamera_Mulitple[i].Cam_Info.MV_CC_SetBoolValue_NET("TriggerCacheEnable", false);
                m_pMyCamera_Mulitple[i].Cam_Info.MV_CC_SetFloatValue_NET("AcquisitionFrameRate", (float)5);
            }
        }

        /// <summary>
        /// 断开相机
        /// </summary>
        public void MV_Stop()
        {
            try
            {
                if (m_nDevNum != 0)
                {
                    for (int i = 0, j = 0; j < m_nDevNum; ++i, ++j)
                    {
                        //MessageBox.Show(Convert.ToString(i)); 
                        int nRet;
                        nRet = m_pMyCamera_Mulitple[i].Cam_Info.MV_CC_StopGrabbing_NET();
                        nRet = m_pMyCamera_Mulitple[i].Cam_Info.MV_CC_CloseDevice_NET();
                        nRet = m_pMyCamera_Mulitple[i].Cam_Info.MV_CC_DestroyDevice_NET();
                        //if (0 != nRet)
                        //{
                        //    ShowErrorMsg("close devices fail!", 0);

                        //    return;
                        //}
                    }
                    //m_pMyCamera.MV_CC_StopGrabbing_NET();
                    m_bGrabbing = false;
                }
            }
            catch { };
        }

        public void MV_5FPS()
        {
            for (int i = 0, j = 0; j < m_nDevNum; ++i, ++j)
            {
                m_pMyCamera_Mulitple[i].Cam_Info.MV_CC_SetFloatValue_NET("AcquisitionFrameRate", (float)5);
            }
        }
        public void MV_30FPS()
        {
            for (int i = 0, j = 0; j < m_nDevNum; ++i, ++j)
            {
                m_pMyCamera_Mulitple[i].Cam_Info.MV_CC_SetFloatValue_NET("AcquisitionFrameRate", (float)30);
            }
        }
        /// <summary>
        /// 修改相机参数
        /// </summary>
        public void MV_CameraParaSet(int CameraParaNum, float ExposureTime, float Gain, float AcquisitionFrameRate)
        {
            try
            {
                m_pMyCamera_Mulitple[CameraParaNum].Cam_Info.MV_CC_SetFloatValue_NET("ExposureTime", (float)ExposureTime);
                m_pMyCamera_Mulitple[CameraParaNum].Cam_Info.MV_CC_SetFloatValue_NET("AcquisitionFrameRate", (float)AcquisitionFrameRate);
                m_pMyCamera_Mulitple[CameraParaNum].Cam_Info.MV_CC_SetFloatValue_NET("Gain", (float)Gain);
            }
            catch { ShowErrorMsg("修改参数失败" + " 工位：" + GW_Num, 0); };
           
        }

        #endregion

        #endregion
    }
}
                                                                                                                                                    