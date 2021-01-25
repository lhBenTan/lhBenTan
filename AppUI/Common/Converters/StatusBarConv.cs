using System;
using System.Collections.ObjectModel;
using System.Globalization;
using System.Windows.Data;

namespace AppUI.Common.Converters
{
    public class StatusBarConv : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            bool LinkFlag = (bool)value;
            if (LinkFlag == true)
            { return "#0B6095"; }
            else { return "#CA5100"; }
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return "ERROR";
        }
    }
}