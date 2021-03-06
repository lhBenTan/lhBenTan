using System;
using System.Runtime.InteropServices;
using System.Text;

namespace AppUI.Common.DllImport
{
    public struct ImageInfo
    {
        public IntPtr data;
        public IntPtr pMat;
        public int size;
    }

    public struct SwapData
    {
        public double OutTotalArea;
        public double OutCircularity;
        public double outCircularity2;
        public int InRoiX;
        public int InRoiY;
        public int InRoiW;
        public int InRoiH;
        public int IsEffectDisplay;
    }

    public struct StdData
    {
        public double Threshold;
        public double Area;
        public double Circularit;
        public double HistEvenness;
        public double SFR;
    }

    public class CVAlgorithms
    {
        /// 释放new成员时产生的内存
        // bool _stdcall ReleaseMemUseDelete(double* pArray)
        [DllImport("SunnyVision.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern bool ReleaseMemUseDelete(IntPtr ptr);

        /// 释放内存
        // bool _stdcall ReleaseMemUseFree(unsigned char* buf)
        [DllImport("SunnyVision.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern bool ReleaseMemUseFree(IntPtr ptr);

        /// Opencv wrapper - gray
        // double CvpGray(unsigned char* pBuf, int width, int height, ImageInfo &imgInfo)
        [DllImport("SunnyVision.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern double CvpGray(IntPtr pBuf, int width, int height, ref ImageInfo imgInfo);

        /// 截图
        // void _stdcall CaptureBmp(unsigned char* pBuf, const int width, const int height, ImageInfo& imgInfo)
        [DllImport("SunnyVision.dll", CallingConvention = CallingConvention.StdCall)]
        [return:MarshalAs(UnmanagedType.BStr)]
        public static extern string CaptureBmp(IntPtr pBuf, int width, int height);

        /// 杂光验证算法
        // double _stdcall Algorithm_GetContourArea(unsigned char* pBuf, const int width, const int height, ImageInfo& imgInfo)
        [DllImport("SunnyVision.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern double Algorithm_GetContourArea(IntPtr pBuf, 
            int width, int height, ref ImageInfo imgInfo, ref SwapData swapData, ref StdData stdData);


        [DllImport("kernel32.dll", EntryPoint = "RtlMoveMemory")]
        public static extern void RtlMoveMemory(IntPtr dest, IntPtr src, uint count);

        /// 发出系统提示音
        [DllImport("kernel32.dll")]
        public static extern bool Beep(int frequency, int duration);
        [DllImport("user32.dll")]
        public static extern bool MessageBeep(int beepType);
        [DllImport("user32.dll",EntryPoint ="SendMessage")]
        public static extern int SendMessage(IntPtr hWnd,int Msg, int wParam,int lParam);
        [DllImport("user32.dll", SetLastError = true)]
        public static extern bool MoveWindow(IntPtr hWnd, int X, int Y, int nWidth, int nHeight, bool bRepaint);

        ///工业相机测试

        public struct BmpBuf
        {
            public IntPtr pData;
            public int size;
            public IntPtr pData_IntPtr;
            //public int size_IntPtr;
            public int Height;
            public int Width;
        }

        public struct Pare_Output
        {
            public int output_Parameter_Int_0;
            public int output_Parameter_Int_1;
            public int output_Parameter_Int_2;
            public int output_Parameter_Int_3;

            public float output_Parameter_float_0;
            public float output_Parameter_float_1;
            public float output_Parameter_float_2;
            public float output_Parameter_float_3;
        }

        [DllImport("OpenCVDll_v2017.3406.4601.0001.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static void img_release(ref BmpBuf data);

        [DllImport("OpenCVDll_v2017.3406.4601.0001.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static void img_Defalt(int nHeight, int nWidth, ref BmpBuf data);


        [DllImportAttribute("OpenCVDll_v2017.3406.4601.0001.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static void img_KJT(int nHeight, int nWidth, ref BmpBuf data, ref int input_Parameter_Int, ref int output_Parameter_Int, ref float output_Parameter_Float);

        //[DllImport("Dll1.dll", EntryPoint = "img", ExactSpelling = false, CallingConvention = CallingConvention.Cdecl)]
        //public static extern bool img();
                                                                                            
        [DllImport("MVDll_v2017.3406.4601.0001.dll", CharSet = CharSet.Ansi, EntryPoint = "img", ExactSpelling = false, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool img(int nHeight, int nWidth, ref BmpBuf data , string[] input_Parameter);

        [DllImport("MVDll_v2017.3406.4601.0001.dll", EntryPoint = "MV_Release", ExactSpelling = false, CallingConvention = CallingConvention.Cdecl)]
        public static extern void MV_Release(ref BmpBuf data);

        [DllImport("MVDll_v2017.3406.4601.0001.dll", EntryPoint = "MV_GW01_JT", ExactSpelling = false, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool MV_GW01_JT(int nHeight, int nWidth, ref BmpBuf data, ref float input_Parameter_Int, ref int output_Parameter_Int, ref float output_Parameter_Float);

        [DllImport("MVDll_v2017.3406.4601.0001.dll", EntryPoint = "MV_GW01_Lens", ExactSpelling = false, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool MV_GW01_Lens(int nHeight, int nWidth, ref BmpBuf data, ref float input_Parameter_Int, ref int output_Parameter_Int, ref float output_Parameter_Float);
        [DllImport("MVDll_v2017.3406.4601.0001.dll", EntryPoint = "MV_GW01_DGP", ExactSpelling = false, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool MV_GW01_DGP(int nHeight, int nWidth, ref BmpBuf data, ref float input_Parameter_Int, ref int output_Parameter_Int, ref float output_Parameter_Float);
        [DllImport("MVDll_v2017.3406.4601.0001.dll", EntryPoint = "MV_DJ01", ExactSpelling = false, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool MV_DJ01(int nHeight, int nWidth, ref BmpBuf data, ref float input_Parameter_Int, ref int output_Parameter_Int, ref float output_Parameter_Float);
        [DllImport("MVDll_v2017.3406.4601.0001.dll", EntryPoint = "MV_PJ01", ExactSpelling = false, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool MV_PJ01(int nHeight, int nWidth, ref BmpBuf data, ref float input_Parameter_Int, ref int output_Parameter_Int, ref float output_Parameter_Float);
        [DllImport("MVDll_v2017.3406.4601.0001.dll", EntryPoint = "MV_TP", ExactSpelling = false, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool MV_TP(int nHeight, int nWidth, ref BmpBuf data, ref float input_Parameter_Int, ref int output_Parameter_Int, ref float output_Parameter_Float);

        [DllImport("MVDll_v2017.3406.4601.0001.dll", CharSet = CharSet.Ansi, EntryPoint = "MV_EntryPoint", ExactSpelling = false, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool MV_EntryPoint(int type, int nHeight, int nWidth, ref BmpBuf data, string[] input_Parameter,  ref float output_Parameter_Float);
    }

}
