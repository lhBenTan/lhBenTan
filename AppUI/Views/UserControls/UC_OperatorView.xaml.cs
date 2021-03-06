using AppUI.Models;
using System.Collections.ObjectModel;
using System.Windows;
using System.Windows.Controls;

namespace AppUI.Views.UserControls
{
    /// <summary>
    /// UC_OperatorArea.xaml 的交互逻辑
    /// </summary>
    public partial class UC_OperatorView : UserControl
    {
        public UC_OperatorView()
        {
            InitializeComponent();

            Loaded += UC_OperatorView_Loaded;
        }

        /// <summary>
        /// 加载完成事件
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void UC_OperatorView_Loaded(object sender, RoutedEventArgs e)
        {
            //DataContext = UCViewModel;
            
        }

        #region 弃用

        private void ListView_SizeChanged(object sender, System.Windows.SizeChangedEventArgs e)
        {
            ListView listView = sender as ListView;
            GridView gView = listView.View as GridView;

            var workingWidth = listView.ActualWidth - SystemParameters.VerticalScrollBarWidth; // take into account vertical scrollbar
            var col1 = 0.50;
            var col2 = 0.50;

            gView.Columns[0].Width = workingWidth * col1;
            gView.Columns[1].Width = workingWidth * col2;
        }

        #endregion
    }
}
