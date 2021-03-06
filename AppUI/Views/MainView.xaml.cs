using AppUI.Core;
using AppUI.ViewModels;
using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Interop;

namespace AppUI
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainView : Window
    {

        /// <summary>
        /// 海康帮助类实例
        /// </summary>
        public HikHelper Hik;

        public MainView()
        {
            InitializeComponent();
            Hik = new HikHelper();
            _hik = Hik;
            // 添加关闭窗口委托
            if (MainVM.CloseAction == null)
                MainVM.CloseAction = new Action(this.Close);
       
                MainVM.Hik = _hik;
                MainVM.Init();
            //hik.MV_DeviceListAcq();
            // RenderOptions.SetBitmapScalingMode(Imc_Player, BitmapScalingMode.LowQuality);

            //MainViewModel mainVM01 = (MainViewModel)mainView.DataContext;
        }

        //public MainView(HikHelper hik) : this()
        //{

        //    _hik = hik;
        //    // 添加关闭窗口委托
        //    if (MainVM.CloseAction == null)
        //        MainVM.CloseAction = new Action(this.Close);

        //    Loaded += (s, e) =>
        //    {
        //        MainVM.Hik = _hik;
        //        MainVM.Init();
        //        //hik.MV_DeviceListAcq();
        //        // RenderOptions.SetBitmapScalingMode(Imc_Player, BitmapScalingMode.LowQuality);
        //    };
        //}

        private HikHelper _hik;
        
        private void ConsoleView_TextChanged(object sender, TextChangedEventArgs e)
        {
            ConsoleView.SelectionStart = ConsoleView.Text.Length;
            ConsoleView.ScrollToEnd();
        }

       
    }
}
