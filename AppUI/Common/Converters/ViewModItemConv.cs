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
    class ViewModItemConv : IMultiValueConverter
    {
        public object Convert(object[] values, Type targetType, object parameter, CultureInfo culture)
        {
            int ViewMod_LC = (int)values[0];
            int ViewMod_GW = (int)values[1];

            if (ViewMod_LC == 0)
            {

                switch (ViewMod_GW)
                {
                    case 0: { return "工位名称： G2组装"; };
                    case 1: { return "工位名称： P1组装"; };
                    case 2: { return "工位名称： P3组装"; };
                    case 3: { return "工位名称： P4组装"; };


                    default: return "工位名称： 异常";
                }


            }
            else if (ViewMod_LC == 1)
            {
                switch (ViewMod_GW)
                {
                    case 0: { return "工位名称： G2点胶"; };
                    case 1: { return "工位名称： P1点胶"; };
                    case 2: { return "工位名称： P4点胶"; };


                    default: return "工位名称： 异常";
                }

            }
            else if (ViewMod_LC == 2)
            {
                switch (ViewMod_GW)
                {
                    case 0: { return "工位名称： G2判胶"; };
                    case 1: { return "工位名称： P1判胶"; };
                    case 2: { return "工位名称： P4判胶"; };

                    default: return "工位名称：异常";
                }

            }
            else {
                return "工位名称：上料";
            }
        }

        public object[] ConvertBack(object value, Type[] targetTypes, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}
