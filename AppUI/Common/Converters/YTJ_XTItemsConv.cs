using System;
using System.Collections.ObjectModel;
using System.Globalization;
using System.Windows.Data;

namespace AppUI.Common.Converters
{
    public class YTJ_XTItemsConv : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            int XTNums = int.Parse(value.ToString())+1;
            ObservableCollection<String> Items = new ObservableCollection<String>();
            for (int i = 0; i < XTNums; i++)
            {
                Items.Add("吸头" + (i+1).ToString());
            }
            return Items;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return "ERROR";
        }
    }
}