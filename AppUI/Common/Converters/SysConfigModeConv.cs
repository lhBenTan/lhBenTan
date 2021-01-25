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
    class SysConfigModeConv : IMultiValueConverter
    {
        public object Convert(object[] values, Type targetType, object parameter, CultureInfo culture)
        {
            bool IsDeveloperLogin = (bool)values[0];
            bool SysConfigMode = (bool)values[1];
            if (IsDeveloperLogin == true && (SysConfigMode == false))
            {
                return true;
         
            }
            else
            {
                return false;
            }
        }

        public object[] ConvertBack(object value, Type[] targetTypes, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}
