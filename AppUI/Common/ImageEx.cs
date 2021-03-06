using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Windows.Media.Imaging;

namespace AppUI.Common
{
    public struct BitmapInfo
    {
        // 位图数据
        public byte[] data { get; set; }

        // 位图步长，防止偏移
        public int step { get; set; }
    }

    public class ImageEx
    {
        /// <summary>
        /// Byte to bitmapImage.
        /// </summary>
        /// <param name="data"></param>
        /// <returns></returns>
        public static BitmapImage ByteToBitmapImage(byte[] data)
        {
            if (data.Length == 0) { throw new Exception("Falha ao carregar a imagem: Imagem inexistente!"); };

            var bitmap = new BitmapImage();
            using (var stream = new MemoryStream(data))
            {
                bitmap.BeginInit();
                bitmap.StreamSource = stream;
                bitmap.CacheOption = BitmapCacheOption.OnLoad;
                bitmap.EndInit();
                bitmap.Freeze();
            }

            return bitmap;
        }

        /// <summary>
        /// 位数组转位图
        /// </summary>
        /// <param name="byteArray"></param>
        /// <returns></returns>
        public static System.Drawing.Bitmap BytesToBitmap(byte[] byteArray)
        {
            System.Drawing.Bitmap bmp = null;
            unsafe
            {
                fixed (byte* p = byteArray)
                {
                    IntPtr unmanagedPointer = (IntPtr)p;

                    // Deduced from your buffer size
                    int width = 1920;
                    int height = 1080;
                    bmp = new System.Drawing.Bitmap(width, height, width * 3, System.Drawing.Imaging.PixelFormat.Format24bppRgb, unmanagedPointer);
                    return bmp;
                }
            }
        }

        /// <summary>
        /// Byte to WriteableBitmap
        /// </summary>
        /// <param name="data"></param>
        /// <returns></returns>
        public static WriteableBitmap ByteToWriteableBitmap(byte[] data)
        {
            if (data.Length == 0) { throw new Exception("Falha ao carregar a imagem: Imagem inexistente!"); };

            var bitmap = new BitmapImage();
            using (var stream = new MemoryStream(data))
            {
                bitmap.BeginInit();
                bitmap.StreamSource = stream;
                bitmap.CacheOption = BitmapCacheOption.OnLoad;
                bitmap.EndInit();
                bitmap.Freeze();
            }

            return new WriteableBitmap(bitmap);
        }

        /// <summary>
        /// BitmapImage to Bitmap.
        /// </summary>
        /// <param name="bitmapImage"></param>
        /// <returns></returns>
        public static System.Drawing.Bitmap BitmapImage2Bitmap(BitmapSource bitmapImage)
        {
            // BitmapImage bitmapImage = new BitmapImage(new Uri("../Images/test.png", UriKind.Relative));

            using (MemoryStream outStream = new MemoryStream())
            {
                BitmapEncoder enc = new BmpBitmapEncoder();
                enc.Frames.Add(BitmapFrame.Create(bitmapImage));
                enc.Save(outStream);
                System.Drawing.Bitmap bitmap = new System.Drawing.Bitmap(outStream);

                return new System.Drawing.Bitmap(bitmap);
            }
        }

        /// <summary>
        /// 获取位图信息
        /// </summary>
        /// <param name="src"></param>
        /// <returns></returns>
        public static BitmapInfo GetBitmapPixels(System.Drawing.Bitmap src)
        {
            int width = src.Width;
            int height = src.Height;
            System.Drawing.Rectangle rect = new System.Drawing.Rectangle(0, 0, width, height);
            System.Drawing.Imaging.BitmapData bmpData = src.LockBits(rect, System.Drawing.Imaging.ImageLockMode.ReadWrite, src.PixelFormat);
            IntPtr ptr = bmpData.Scan0;
            int bytes = width * height * GetBitmapChannels(src);
            byte[] pixelValues = new byte[bytes];
            Marshal.Copy(ptr, pixelValues, 0, bytes);
            src.UnlockBits(bmpData);
            return new BitmapInfo { data = pixelValues, step = bmpData.Stride };
        }

        /// <summary>
        /// 获取Bitmap图像通道数
        /// </summary>
        /// <param name="src"></param>
        /// <returns></returns>
        public static int GetBitmapChannels(System.Drawing.Bitmap src)
        {
            int channels;
            switch (src.PixelFormat)
            {
                case System.Drawing.Imaging.PixelFormat.Format24bppRgb:
                case System.Drawing.Imaging.PixelFormat.Format32bppRgb:
                    channels = 3; break;
                case System.Drawing.Imaging.PixelFormat.Format32bppArgb:
                case System.Drawing.Imaging.PixelFormat.Format32bppPArgb:
                    channels = 4; break;
                case System.Drawing.Imaging.PixelFormat.Format8bppIndexed:
                case System.Drawing.Imaging.PixelFormat.Format1bppIndexed:
                    channels = 1; break;
                default:
                    throw new NotImplementedException();
            }
            return channels;
        }
    }
}