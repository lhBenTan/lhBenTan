using System;
using System.Globalization;
using System.Windows.Data;

namespace AppUI.Common.Converters
{
    public class ResultConv : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if ((string)value == "OK")
            {
                return System.Windows.Media.Brushes.Green;
            }
            else
            {
                return System.Windows.Media.Brushes.Red;
            }
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return "ERROR";
        }
    }
}