using AppUI.Core;
using AppUI.ViewModels;
using System;
using System.Collections.ObjectModel;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows;
using System.Windows.Media;
using AppUI.Models;
using Walterlv.Demo;
using System.Windows.Interop;
using AppUI.Views.SubViews;
using AppUI.Common.DllImport;
using AppUI.Views.UserControls;

namespace AppUI
{
    /// <summary>
    /// LoginView.xaml 的交互逻辑
    /// </summary>
    public partial class LoginView : Window
    {
        public LoginView()
        {
            InitializeComponent();
            //LoadWindows();
            // 添加关闭窗口委托
            if (LoginVM.CloseAction == null)
                LoginVM.CloseAction = new Action(this.Close);
            //初始化
            MainViewModel mainVM01 = (MainViewModel)_MainV01.DataContext;
            MainViewModel mainVM02 = (MainViewModel)_MainV02.DataContext;
            MainViewModel mainVM03 = (MainViewModel)_MainV03.DataContext;
            MainViewModel mainVM04 = (MainViewModel)_MainV04.DataContext;
            _Guidepanel.GB_DataText01.DataContext = mainVM01;
            _Guidepanel.GB_DataText02.DataContext = mainVM02;
            _Guidepanel.GB_DataText03.DataContext = mainVM03;
            _Guidepanel.GB_DataText04.DataContext = mainVM04;
            LinkLight01.DataContext = mainVM01;
            LinkLight02.DataContext = mainVM02;
            LinkLight03.DataContext = mainVM03;
            LinkLight04.DataContext = mainVM04;

            LoginVM.Init();
     
        }

        //Load窗口
        private async void LoadWindows()
        {
            // 传入的参数是线程的名称，也可以不用传。
            var dispatcher_Load = await UIDispatcher.RunNewAsync("Load_UI0");
            var backgroundWindow = await dispatcher_Load.InvokeAsync(() =>
            {
                LoadWindow Load = new LoadWindow();
                Load.SourceInitialized += OnSourceInitialized;
                Load.Show();
                return Load;
            });

        }
        //句柄
        public IntPtr LoadWindowHwnd;
        const int WM_CLOSE = 0x0010;
        private void OnSourceInitialized(object sender, EventArgs e)
        {
            // 在这里可以获取到窗口句柄。
            var childHazhoingshan1dle = new WindowInteropHelper((Window)sender).Handle;
            LoadWindowHwnd = childHazhoingshan1dle;
            //CVAlgorithms.MoveWindow(childHazhoingshan1dle, 0, 0, 300, 300, true);

        }

        private void FlatButton_Click(object sender, RoutedEventArgs e)
        {
            MainViewModel mainVM01 = (MainViewModel)_MainV01.DataContext;
            mainVM01.Hik.MV_Stop();
        }

        private void _MainV01_Loaded(object sender, RoutedEventArgs e)
        {
            MainViewModel mainVM01 = (MainViewModel)_MainV01.DataContext;
            mainVM01.YTJ_XT_01 = new YTJ_XT_ConfigModel(".\\Para\\Camera01\\YTJ_XT01_Config.xml");
            mainVM01.YTJ_XT_01.CSH();
            mainVM01.YTJ_XT_02 = new YTJ_XT_ConfigModel(".\\Para\\Camera01\\YTJ_XT02_Config.xml");
            mainVM01.YTJ_XT_02.CSH();
            mainVM01.YTJ_XT_03 = new YTJ_XT_ConfigModel(".\\Para\\Camera01\\YTJ_XT03_Config.xml");
            mainVM01.YTJ_XT_03.CSH();
            mainVM01.YTJ_XT_04 = new YTJ_XT_ConfigModel(".\\Para\\Camera01\\YTJ_XT04_Config.xml");
            mainVM01.YTJ_XT_04.CSH();
            mainVM01.YTJ_XT_05 = new YTJ_XT_ConfigModel(".\\Para\\Camera01\\YTJ_XT05_Config.xml");
            mainVM01.YTJ_XT_05.CSH();
            mainVM01.YTJ_XT_06 = new YTJ_XT_ConfigModel(".\\Para\\Camera01\\YTJ_XT06_Config.xml");
            mainVM01.YTJ_XT_06.CSH();
            mainVM01.YTJ_XT_07 = new YTJ_XT_ConfigModel(".\\Para\\Camera01\\YTJ_XT07_Config.xml");
            mainVM01.YTJ_XT_07.CSH();
            mainVM01.YTJ_XT_08 = new YTJ_XT_ConfigModel(".\\Para\\Camera01\\YTJ_XT08_Config.xml");
            mainVM01.YTJ_XT_08.CSH();
            mainVM01.Hik.GW_Num = 1;
            _MainV01.QHSF_Items.DataContext = mainVM01.YTJ_XT_01;

        }

        private void _MainV02_Loaded(object sender, RoutedEventArgs e)
        {
            MainViewModel mainVM02 = (MainViewModel)_MainV02.DataContext;
            mainVM02.YTJ_XT_01 = new YTJ_XT_ConfigModel(".\\Para\\Camera02\\YTJ_XT01_Config.xml");
            mainVM02.YTJ_XT_01.CSH();
            mainVM02.YTJ_XT_02 = new YTJ_XT_ConfigModel(".\\Para\\Camera02\\YTJ_XT02_Config.xml");
            mainVM02.YTJ_XT_02.CSH();
            mainVM02.YTJ_XT_03 = new YTJ_XT_ConfigModel(".\\Para\\Camera02\\YTJ_XT03_Config.xml");
            mainVM02.YTJ_XT_03.CSH();
            mainVM02.YTJ_XT_04 = new YTJ_XT_ConfigModel(".\\Para\\Camera02\\YTJ_XT04_Config.xml");
            mainVM02.YTJ_XT_04.CSH();
            mainVM02.YTJ_XT_05 = new YTJ_XT_ConfigModel(".\\Para\\Camera02\\YTJ_XT05_Config.xml");
            mainVM02.YTJ_XT_05.CSH();
            mainVM02.YTJ_XT_06 = new YTJ_XT_ConfigModel(".\\Para\\Camera02\\YTJ_XT06_Config.xml");
            mainVM02.YTJ_XT_06.CSH();
            mainVM02.YTJ_XT_07 = new YTJ_XT_ConfigModel(".\\Para\\Camera02\\YTJ_XT07_Config.xml");
            mainVM02.YTJ_XT_07.CSH();
            mainVM02.YTJ_XT_08 = new YTJ_XT_ConfigModel(".\\Para\\Camera02\\YTJ_XT08_Config.xml");
            mainVM02.YTJ_XT_08.CSH();
            mainVM02.Hik.GW_Num = 2;
            _MainV02.QHSF_Items.DataContext = mainVM02.YTJ_XT_01;
      
        }

        private void _MainV03_Loaded(object sender, RoutedEventArgs e)
        {
            MainViewModel mainVM03 = (MainViewModel)_MainV03.DataContext;
            mainVM03.YTJ_XT_01 = new YTJ_XT_ConfigModel(".\\Para\\Camera03\\YTJ_XT01_Config.xml");
            mainVM03.YTJ_XT_01.CSH();
            mainVM03.YTJ_XT_02 = new YTJ_XT_ConfigModel(".\\Para\\Camera03\\YTJ_XT02_Config.xml");
            mainVM03.YTJ_XT_02.CSH();
            mainVM03.YTJ_XT_03 = new YTJ_XT_ConfigModel(".\\Para\\Camera03\\YTJ_XT03_Config.xml");
            mainVM03.YTJ_XT_03.CSH();
            mainVM03.YTJ_XT_04 = new YTJ_XT_ConfigModel(".\\Para\\Camera03\\YTJ_XT04_Config.xml");
            mainVM03.YTJ_XT_04.CSH();
            mainVM03.YTJ_XT_05 = new YTJ_XT_ConfigModel(".\\Para\\Camera03\\YTJ_XT05_Config.xml");
            mainVM03.YTJ_XT_05.CSH();
            mainVM03.YTJ_XT_06 = new YTJ_XT_ConfigModel(".\\Para\\Camera03\\YTJ_XT06_Config.xml");
            mainVM03.YTJ_XT_06.CSH();
            mainVM03.YTJ_XT_07 = new YTJ_XT_ConfigModel(".\\Para\\Camera03\\YTJ_XT07_Config.xml");
            mainVM03.YTJ_XT_07.CSH();
            mainVM03.YTJ_XT_08 = new YTJ_XT_ConfigModel(".\\Para\\Camera03\\YTJ_XT08_Config.xml");
            mainVM03.YTJ_XT_08.CSH();
            mainVM03.Hik.GW_Num = 3;
            _MainV03.QHSF_Items.DataContext = mainVM03.YTJ_XT_01;
         
        }

        private void _MainV04_Loaded(object sender, RoutedEventArgs e)
        {
             MainViewModel mainVM04 = (MainViewModel)_MainV04.DataContext;
            mainVM04.YTJ_XT_01 = new YTJ_XT_ConfigModel(".\\Para\\Camera04\\YTJ_XT01_Config.xml");
            mainVM04.YTJ_XT_01.CSH();
            mainVM04.YTJ_XT_02 = new YTJ_XT_ConfigModel(".\\Para\\Camera04\\YTJ_XT02_Config.xml");
            mainVM04.YTJ_XT_02.CSH();
            mainVM04.YTJ_XT_03 = new YTJ_XT_ConfigModel(".\\Para\\Camera04\\YTJ_XT03_Config.xml");
            mainVM04.YTJ_XT_03.CSH();
            mainVM04.YTJ_XT_04 = new YTJ_XT_ConfigModel(".\\Para\\Camera04\\YTJ_XT04_Config.xml");
            mainVM04.YTJ_XT_04.CSH();
            mainVM04.YTJ_XT_05 = new YTJ_XT_ConfigModel(".\\Para\\Camera04\\YTJ_XT05_Config.xml");
            mainVM04.YTJ_XT_05.CSH();
            mainVM04.YTJ_XT_06 = new YTJ_XT_ConfigModel(".\\Para\\Camera04\\YTJ_XT06_Config.xml");
            mainVM04.YTJ_XT_06.CSH();
            mainVM04.YTJ_XT_07 = new YTJ_XT_ConfigModel(".\\Para\\Camera04\\YTJ_XT07_Config.xml");
            mainVM04.YTJ_XT_07.CSH();
            mainVM04.YTJ_XT_08 = new YTJ_XT_ConfigModel(".\\Para\\Camera04\\YTJ_XT08_Config.xml");
            mainVM04.YTJ_XT_08.CSH();
            mainVM04.Hik.GW_Num = 4;
            _MainV04.QHSF_Items.DataContext = mainVM04.YTJ_XT_01;
            //CVAlgorithms.SendMessage(LoadWindowHwnd, WM_CLOSE, 0, 0);
        }


        private void ToggleButton_Checked(object sender, RoutedEventArgs e)
        {
            mainGrid.ColumnDefinitions[0].Width =  new GridLength(1, GridUnitType.Star);
            
        }

        private void ToggleButton_Unchecked(object sender, RoutedEventArgs e)
        {
            mainGrid.ColumnDefinitions[0].Width = new GridLength(0, GridUnitType.Star);
        }

        private void _Guidepanel_Loaded(object sender, RoutedEventArgs e)
        {

        }

        private void _Login_Loaded(object sender, RoutedEventArgs e)
        {
          


        }

       
       
    }
}
