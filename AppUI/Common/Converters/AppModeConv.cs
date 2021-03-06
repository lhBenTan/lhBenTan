using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Data;

namespace AppUI.Common.Converters
{
    class AppModeConv : IMultiValueConverter
    {
        public object Convert(object[] values, Type targetType, object parameter, CultureInfo culture)
        {
            int app_mode = (int)values[0];
            bool app_mode_forConverter = (bool)values[1];
            if (app_mode == 1 && (!app_mode_forConverter))
            {
                return new GridLength(0, GridUnitType.Star);
                //return "1*";
                //return new GridLength(1, GridUnitType.Star);
            }
            else
            {
                return new GridLength(0, GridUnitType.Star);
            }
        }

        public object[] ConvertBack(object value, Type[] targetTypes, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}
