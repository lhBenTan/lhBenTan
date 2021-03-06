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
    /// DeveloperLoginView.xaml 的交互逻辑
    /// </summary>
    public partial class DeveloperLoginView : Window
    {
        public DeveloperLoginView()
        {
            InitializeComponent();
            //计时器
            System.Timers.Timer AutoLock = new System.Timers.Timer(60000);
            AutoLock.Elapsed += new System.Timers.ElapsedEventHandler(Lock);
            AutoLock.AutoReset = false;
            AutoLock.Enabled = true;
        }
        MainViewModel mainVM_Temp = new MainViewModel();
        public void Lock(object source, System.Timers.ElapsedEventArgs e)
        {
            mainVM_Temp.IsDeveloperLogin = false;
        }
     
        private void Btn_Login_Click(object sender, RoutedEventArgs e)
        {
            var mainVM = DataContext as MainViewModel;
            mainVM_Temp = mainVM;
            if (TB_UserName.Text == "admin" && PB_Password.Password == "admin")
            {
                mainVM.IsDeveloperLogin = true;

                Close();
            }
            else
            {
                if (TB_UserName.Text == "deve" && PB_Password.Password == "0890")
                {
                    mainVM.IsDeveloperLogin = true;
                    mainVM.IsDeveloperLogin_Dev = true;
                  
                    Close();
                }
                else
                {
                    MessageBox.Show("账号或密码错误！");
                }
            }

        }

        private void Btn_Cancel_Click(object sender, RoutedEventArgs e)
        {
            Close();
        }
    }
}
