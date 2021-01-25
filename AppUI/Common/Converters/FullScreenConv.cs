using System;
using System.Globalization;
using System.Windows;
using System.Windows.Data;

namespace AppUI.Common.Converters
{
    class FullScreenConv: IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            bool SysConfigMode = (bool)value;
            if (SysConfigMode == true)
            {
                return "Maximized";
            }
            else
            {
                return "Normal";
            }

        }


        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }

    }
}
