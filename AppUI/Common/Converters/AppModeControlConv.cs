using System;
using System.Globalization;

using System.Windows.Data;

namespace AppUI.Common.Converters
{
    class AppModeControlConv : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if ((int)value == 1)
            {
                return 110;
            }
            else { return 0; }

        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}
