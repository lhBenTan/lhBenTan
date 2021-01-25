using System;
using AppUI.Core;
using System.Collections.Generic;
using System.Linq;
using System.Text;
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

namespace AppUI.Views.UserControls
{
    /// <summary>
    /// MainViewUserControls.xaml 的交互逻辑
    /// </summary>
    public partial class MainViewUserControls : UserControl
    {
        /// <summary>
        /// 海康帮助类实例
        /// </summary>
        public HikHelper Hik;
        public DatabaseHelper Dtb;
        //图片原始属性
        public TransformGroup Img_Recover ;
        public TransformGroup Img_Recover_tgnew;

        public MainViewUserControls()
        {
            InitializeComponent();
            Hik = new HikHelper();
            Dtb = new DatabaseHelper();
            _hik = Hik;
            _Dtb = Dtb;
            MainVM.Hik = _hik;
            MainVM.Dtb = _Dtb;
            MainVM.Init();
            Img_Recover = Imc_Player.RenderTransform as TransformGroup;
            Img_Recover_tgnew = Img_Recover.CloneCurrentValue();
        }

        private void ConsoleView_TextChanged(object sender, TextChangedEventArgs e)
        {
            ConsoleView.SelectionStart = ConsoleView.Text.Length;
            ConsoleView.ScrollToEnd();
        }
        private HikHelper _hik;
        private DatabaseHelper _Dtb;
        /// <summary>
        /// 图片放大
        /// </summary>
        /// <param name="img">被操作的前台Image控件</param>
        public void ZoomIn(Image img)
        {
            TransformGroup tg = img.RenderTransform as TransformGroup;
            var tgnew = tg.CloneCurrentValue();
            if (tgnew != null)
            {
                ScaleTransform st = tgnew.Children[1] as ScaleTransform;
                img.RenderTransformOrigin = new Point(0.5, 0.5);
                if (st.ScaleX > 0 && st.ScaleX <= 2.0)
                {
                    st.ScaleX += st.ScaleX * 0.05;
                    st.ScaleY += st.ScaleY * 0.05;
                }
                else if (st.ScaleX < 0 && st.ScaleX >= -2.0)
                {
                    st.ScaleX -= st.ScaleX * 0.05;
                    st.ScaleY += st.ScaleY * 0.05;
                }
            }

            // 重新给图像赋值Transform变换属性
            img.RenderTransform = tgnew;
        }
        /// <summary>
        /// 图片缩小
        /// </summary>
        /// <param name="img">被操作的前台Image控件</param>
        public void ZoomOut(Image img)
        {
            TransformGroup tg = img.RenderTransform as TransformGroup;
            var tgnew = tg.CloneCurrentValue();
            if (tgnew != null)
            {
                ScaleTransform st = tgnew.Children[1] as ScaleTransform;
                img.RenderTransformOrigin = new Point(0.5, 0.5);
                if (st.ScaleX >= 0.2)
                {
                    st.ScaleX -= st.ScaleX * 0.05;
                    st.ScaleY -= st.ScaleY * 0.05;
                }
                else if (st.ScaleX <= -0.2)
                {
                    st.ScaleX += st.ScaleX * 0.05;
                    st.ScaleY -= st.ScaleY * 0.05;
                }
            }

            // 重新给图像赋值Transform变换属性
            img.RenderTransform = tgnew;
        }

        /// <summary>
        /// 滚轮缩放
        /// </summary>
        /// <param name="img">被操作的前台Image控件</param>
        public void ZoomWheel(Image img, double Delta)
        {
            TransformGroup tg = img.RenderTransform as TransformGroup;
            var tgnew = tg.CloneCurrentValue();
            if (tgnew != null)
            {
                ScaleTransform st = tgnew.Children[1] as ScaleTransform;
                img.RenderTransformOrigin = new Point(0.5, 0.5);
                if (st.ScaleX < 0.3 && st.ScaleY < 0.3 && Delta < 0)
                {
                    return;
                }
                st.ScaleX += st.ScaleX*0.05*((double)Delta/ System.Math.Abs((double)Delta));
                st.ScaleY += st.ScaleY*0.05*((double)Delta/ System.Math.Abs((double)Delta));
            }

            // 重新给图像赋值Transform变换属性
            img.RenderTransform = tgnew;
        }

        private void FlatButton_Click(object sender, RoutedEventArgs e)
        {
            ZoomIn(Imc_Player);
        }

        private void FlatButton_Click_1(object sender, RoutedEventArgs e)
        {
            ZoomOut(Imc_Player);
        }

        private Image movingObject;  // 记录当前被拖拽移动的图片
        private Point StartPosition; // 本次移动开始时的坐标点位置
        private Point EndPosition;   // 本次移动结束时的坐标点位置

        private void Imc_Player_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            Image img = sender as Image;
            movingObject = img;
            StartPosition = e.GetPosition(img);
        }

        private void Imc_Player_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            Image img = sender as Image;
            movingObject = null;
        }

        private void Imc_Player_MouseMove(object sender, MouseEventArgs e)
        {
            Image img = sender as Image;
            if (e.LeftButton == MouseButtonState.Pressed && sender == movingObject)
            {
                EndPosition = e.GetPosition(img);

                TransformGroup tg = img.RenderTransform as TransformGroup;
                var tgnew = tg.CloneCurrentValue();
                if (tgnew != null)
                {
                    TranslateTransform tt = tgnew.Children[0] as TranslateTransform;

                    var X = EndPosition.X - StartPosition.X;
                    var Y = EndPosition.Y - StartPosition.Y;
                    tt.X += X;
                    tt.Y += Y;
                }

                // 重新给图像赋值Transform变换属性
                img.RenderTransform = tgnew;
            }
        }

        private void Imc_Player_MouseWheel(object sender, MouseWheelEventArgs e)
        {
            ZoomWheel(Imc_Player, e.Delta);
        }

        /// <summary>
        /// 图片上移
        /// </summary>
        /// <param name="img">被操作的前台Image控件</param>
        public void MoveUp(Image img)
        {
            TransformGroup tg = img.RenderTransform as TransformGroup;
            
            var tgnew = tg.CloneCurrentValue();
            if (tgnew != null)
            {
                TranslateTransform tt = tgnew.Children[0] as TranslateTransform;
                ScaleTransform st = tgnew.Children[1] as ScaleTransform;
                Console.WriteLine(st.ScaleY);
                tt.Y = tt.Y+50;
            }

            // 重新给图像赋值Transform变换属性
            img.RenderTransform = tgnew;
        }
        /// <summary>
        /// 图片下移
        /// </summary>
        /// <param name="img">被操作的前台Image控件</param>
        public void MoveDown(Image img)
        {
            TransformGroup tg = img.RenderTransform as TransformGroup;

            var tgnew = tg.CloneCurrentValue();
            if (tgnew != null)
            {
                TranslateTransform tt = tgnew.Children[0] as TranslateTransform;
                ScaleTransform st = tgnew.Children[1] as ScaleTransform;
       
                tt.Y = tt.Y - 50;
            }

            // 重新给图像赋值Transform变换属性
            img.RenderTransform = tgnew;
        }
        /// <summary>
        /// 图片左移
        /// </summary>
        /// <param name="img">被操作的前台Image控件</param>
        public void MoveLeft(Image img)
        {
            TransformGroup tg = img.RenderTransform as TransformGroup;

            var tgnew = tg.CloneCurrentValue();
            if (tgnew != null)
            {
                TranslateTransform tt = tgnew.Children[0] as TranslateTransform;
                ScaleTransform st = tgnew.Children[1] as ScaleTransform;
            
                tt.X = tt.X - 50;
            }

            // 重新给图像赋值Transform变换属性
            img.RenderTransform = tgnew;
        }
        /// <summary>
        /// 图片右移
        /// </summary>
        /// <param name="img">被操作的前台Image控件</param>
        public void MoveRight(Image img)
        {
            TransformGroup tg = img.RenderTransform as TransformGroup;

            var tgnew = tg.CloneCurrentValue();
            if (tgnew != null)
            {
                TranslateTransform tt = tgnew.Children[0] as TranslateTransform;
                ScaleTransform st = tgnew.Children[1] as ScaleTransform;
                Console.WriteLine(tt.X);
                tt.X = tt.X + 50;
            }

            // 重新给图像赋值Transform变换属性
            img.RenderTransform = tgnew;
        }
        /// <summary>
        /// 图片左转
        /// </summary>
        /// <param name="img">被操作的前台Image控件</param>
        public void RotateLeft(Image img)
        {
            TransformGroup tg = img.RenderTransform as TransformGroup;
            var tgnew = tg.CloneCurrentValue();
            if (tgnew != null)
            {
                RotateTransform rt = tgnew.Children[2] as RotateTransform;
                img.RenderTransformOrigin = new Point(0.5, 0.5);
                rt.Angle += 90;
            }

            // 重新给图像赋值Transform变换属性
            img.RenderTransform = tgnew;
        }

        /// <summary>
        /// 图片右转
        /// </summary>
        /// <param name="img">被操作的前台Image控件</param>
        public void RotateRight(Image img)
        {
            TransformGroup tg = img.RenderTransform as TransformGroup;
            var tgnew = tg.CloneCurrentValue();
            if (tgnew != null)
            {
                RotateTransform rt = tgnew.Children[2] as RotateTransform;
                img.RenderTransformOrigin = new Point(0.5, 0.5);
                rt.Angle -= 90;
            }

            // 重新给图像赋值Transform变换属性
            img.RenderTransform = tgnew;
        }

        private void FlatButton_Click_2(object sender, RoutedEventArgs e)
        {
            RotateRight(Imc_Player);
        }

        private void FlatButton_Click_3(object sender, RoutedEventArgs e)
        {
            RotateLeft(Imc_Player);
        }
        /// <summary>
        /// 图片复原
        /// </summary>
        /// <param name="img">被操作的前台Image控件</param>
        public void Img_recover(Image img)
        {
         
            //TransformGroup tg = img.RenderTransform as TransformGroup;
            //var tgnew = tg.CloneCurrentValue();
            ////旋转方向复原
            //if (tgnew != null)
            //{
            //    RotateTransform rt = tgnew.Children[2] as RotateTransform;
            //    img.RenderTransformOrigin = new Point(0.5, 0.5);
            //    rt.Angle -= 90;
            //}
            ////位置复原
            //if (tgnew != null)
            //{
            //    TranslateTransform tt = tgnew.Children[0] as TranslateTransform;

            //    var X = EndPosition.X - StartPosition.X;
            //    var Y = EndPosition.Y - StartPosition.Y;
            //    tt.X += X;
            //    tt.Y += Y;
            //}
            ////大小复原
            //if (tgnew != null)
            //{
            //    ScaleTransform st = tgnew.Children[1] as ScaleTransform;
            //    img.RenderTransformOrigin = new Point(0.5, 0.5);
               
            //    st.ScaleX += st.ScaleX * 0.05 * ((double)Delta / System.Math.Abs((double)Delta));
            //    st.ScaleY += st.ScaleY * 0.05 * ((double)Delta / System.Math.Abs((double)Delta));
            //}
            //// 重新给图像赋值Transform变换属性
            //img.RenderTransform = tgnew;
        }

        private void FlatButton_Click_4(object sender, RoutedEventArgs e)
        {
            Imc_Player.RenderTransform = Img_Recover_tgnew;
        }

        private void FlatButton_Click_5(object sender, RoutedEventArgs e)
        {
            MoveUp(Imc_Player);
        }

        private void FlatButton_Click_6(object sender, RoutedEventArgs e)
        {
            MoveRight(Imc_Player);
        }

        private void FlatButton_Click_7(object sender, RoutedEventArgs e)
        {
            MoveDown(Imc_Player);
        }

        private void FlatButton_Click_8(object sender, RoutedEventArgs e)
        {
            MoveLeft(Imc_Player);
        }



    }
}
