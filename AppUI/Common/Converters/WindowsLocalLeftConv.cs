using System;
using System.Globalization;
using System.Windows;
using System.Windows.Data;

namespace AppUI.Common.Converters
{
    class WindowsLocalLeftConv: IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            double WindowsWidth = (SystemParameters.FullPrimaryScreenWidth / 2);//得到屏幕工作区域宽度
            double WindowsHeight = (SystemParameters.FullPrimaryScreenHeight / 2);//得到屏幕工作区域高度
            if ((int)value == 0 )
            {
                return 0;
            }
           
            else 
            {
                return WindowsWidth;
            }

        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            double WindowsWidth = (SystemParameters.FullPrimaryScreenWidth / 2);//得到屏幕工作区域宽度
            double WindowsHeight = (SystemParameters.FullPrimaryScreenHeight / 2);//得到屏幕工作区域高度
            if ((double)value == 0)
            {
                return 0;
            }
            
            else
            {
                return 1;
            }
           
        }
    }
}
