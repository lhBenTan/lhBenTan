using System;
using System.Globalization;
using System.Windows.Data;

namespace AppUI.Common.Converters
{
    public class TotalAreaConv : IMultiValueConverter
    {
        public object Convert(object[] values, Type targetType, object parameter, CultureInfo culture)
        {
            if ((double)values[0] < (double)values[1] && (double)values[0] != 0)
            {
                if (targetType.Name == "String")
                {
                    return "OK";
                }
                return System.Windows.Media.Brushes.Green;
            }
            else
            {
                if (targetType.Name == "String")
                {
                    return "NG";
                }
                return System.Windows.Media.Brushes.Red;
            }
        }

        object[] IMultiValueConverter.ConvertBack(object value, Type[] targetTypes, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}