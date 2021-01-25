using AppUI.ViewModels;
using System;
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
using System.Windows.Shapes;

namespace AppUI.Views.SubViews
{
    /// <summary>
    /// SysConfig_YTJ.xaml 的交互逻辑
    /// </summary>
    public partial class SysConfig_YTJ : Window
    {
        public SysConfig_YTJ()
        {
            InitializeComponent();
        }

        protected override void OnClosing(System.ComponentModel.CancelEventArgs e)

        {
            var mainVM = DataContext as MainViewModel;
            
            mainVM.SysConfig_Mode = false;

        }
    }
}
