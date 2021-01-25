using System;
using System.Collections.ObjectModel;
using System.Globalization;
using System.Windows;
using System.Windows.Data;

namespace AppUI.Common.Converters
{
    class ViewStartItemConv : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            ObservableCollection<String> GWItems = new ObservableCollection<String>();
            ObservableCollection<String> DJItems = new ObservableCollection<String>();
            ObservableCollection<String> PJItems = new ObservableCollection<String>();
            ObservableCollection<String> SLItems = new ObservableCollection<String>();

            GWItems.Add("G2组装");
            GWItems.Add("P1组装");
            GWItems.Add("P3组装");
            GWItems.Add("P4组装");

            DJItems.Add("G2点胶");
            DJItems.Add("P1点胶");
            DJItems.Add("P4点胶");

            PJItems.Add("G2判胶");
            PJItems.Add("P1判胶");
            PJItems.Add("P4判胶");

            SLItems.Add("镜筒上料");

            if ((int)value == 0)
            {
                return GWItems;
            }
            else if ((int)value == 1)
            {
                return DJItems;
            }
            else if ((int)value == 2)
            { return PJItems; }
            else {
                return SLItems;
            }
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}
