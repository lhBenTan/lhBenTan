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
    class ViewStartNumConv : IMultiValueConverter
    {
        public object Convert(object[] values, Type targetType, object parameter, CultureInfo culture)
        {
            int View01 = (int)values[0];
            int View02 = (int)values[1];
            int View03 = (int)values[2];

            int Num = View01 + View02 + View03;
            if (Num == 0)
            {
                return "工位数量: 3";
            }
            else if (Num == 1)
            {
                return "工位数量: 2";
            }
            else if (Num == 2)
            {
                return "工位数量: 1";
            }
            else
            {
                return "工位数量: 0";
            }
        }


        public object[] ConvertBack(object value, Type[] targetTypes, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}
