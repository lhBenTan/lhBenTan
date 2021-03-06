using AppUI.ViewModels;
using System.Linq;
using System.Windows;
using System.Xml.Linq;
using AppUI.Core;

namespace AppUI.Views.SubViews
{
    /// <summary>
    /// SysConfigView.xaml 的交互逻辑
    /// </summary>
    public partial class SysConfigView : Window
    {
        /// <summary>
        /// 海康SDK帮助实例
        /// </summary>
        //public HikHelper Hik = new HikHelper();
        public SysConfigView()
        {
          
            InitializeComponent();
            //var mainVM = DataContext as MainViewModel;
            //mainVM.Hik.MV_DeviceListAcq();

        }

        #region Private field

        /// <summary>
        /// 配置文件读写器
        /// </summary>
        private XDocument config;


        private int oldRoiX;
        private int oldRoiY;
        private int oldRoiW;
        private int oldRoiH;

        #endregion

        /// <summary>
        ///     确定按钮单击事件
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Btn_OK_Click(object sender, RoutedEventArgs e)
        {
            Close();
            var mainVM = DataContext as MainViewModel; 
            mainVM.SysConfig_Mode = false;
        }

        /// <summary>
        ///     取消按钮单击事件
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Btn_Cancel_Click(object sender, RoutedEventArgs e)
        {
            var mainVM = DataContext as MainViewModel;
            mainVM.SysConfig_Mode = false;
         

            Close();
        }

        private void FlatButton_Click(object sender, RoutedEventArgs e)
        {
            var mainVM = DataContext as MainViewModel;
   
            Close();
        }

 

        protected override void OnClosing(System.ComponentModel.CancelEventArgs e)

        {
            var mainVM = DataContext as MainViewModel;
            
            mainVM.SysConfig_Mode = false;

        }
    }
}
