using System;
using System.Globalization;

using System.Windows.Data;

namespace AppUI.Common.Converters
{
    class ViewPlayingClourConv : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {

            if ((string)value == "运行中")
            {
                return "Green";
            }
            else { return "Red"; }

        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}
