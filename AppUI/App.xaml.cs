using AppUI.ViewModels;
using AppUI.Views;
using System.Windows;
using System.Windows.Media;

namespace AppUI
{
    /// <summary>
    /// App.xaml 的交互逻辑
    /// </summary>
    public partial class App : Application
    {
        private void ApplicationStart(object sender, StartupEventArgs e)
        {
            // 强制使用软件渲染
            RenderOptions.ProcessRenderMode = System.Windows.Interop.RenderMode.SoftwareOnly;
            // Disable shutdown when the dialog closes.
            //Current.ShutdownMode = ShutdownMode.OnExplicitShutdown;
         
            //var mainView = new MainView();
            //初始化窗口高度
            //double WindowsWidth = (SystemParameters.FullPrimaryScreenWidth / 2);//得到屏幕工作区域宽度
            //double WindowsHeight = (SystemParameters.FullPrimaryScreenHeight / 2);//得到屏幕工作区域高度
            //mainView.Width = WindowsWidth;
            //mainView.Height = WindowsHeight;
            //MainViewModel mainVM = (MainViewModel)mainView.DataContext;

            var loginView = new LoginView();
            

           
            loginView.Show();

            //if (loginView.ShowDialog() == true)
            //{
            //    var mainView = new MainView(loginView.Hik);
            //    // Re-enable normal shutdown mode.
            //    Current.ShutdownMode = ShutdownMode.OnMainWindowClose;
            //    Current.MainWindow = mainView;
            //    mainView.Show();
            //}
            //else
            //{
            //    Current.Shutdown(-1);
            //}
        }


    }
}