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
    class XT_LightConv : IMultiValueConverter
    {
        public object Convert(object[] values, Type targetType, object parameter, CultureInfo culture)
        {
            if (values[0] !=null )
                {
                string YTJ_XT_S = values[0].ToString();
                char[] ReceiveData = YTJ_XT_S.ToCharArray();
                if (ReceiveData[0] == '吸')
                {
                    string OKNG = values[2].ToString();
                    int YTJ_XT_Nums = int.Parse(ReceiveData[2].ToString());
                    int YTJ_XT_ID = (int)values[1] + 1;

                    if (YTJ_XT_ID <= YTJ_XT_Nums)
                    { if (OKNG == "OK")
                        { return true; }
                        else if (YTJ_XT_ID == YTJ_XT_Nums) { return false; }
                        else { return true; }
                    }
                    else
                    { return null; }
                }
                else { return null; }
            }
            else { return null; }

        }

        public object[] ConvertBack(object value, Type[] targetTypes, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
            
        }
    }
}
