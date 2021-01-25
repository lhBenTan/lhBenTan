using System;
using System.Globalization;

using System.Windows.Data;

namespace AppUI.Common.Converters
{
    class OKNGColourConv : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
           
            if ((string)value == "OK")
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
