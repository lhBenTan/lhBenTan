using AppUI.Models;
using DevExpress.Mvvm;
using OxyPlot;
using OxyPlot.Axes;
using OxyPlot.Series;
using System;
using System.Collections.ObjectModel;

namespace AppUI.ViewModels
{
    public class UC_OperatorViewModel : ViewModelBase
    {
        #region Constructor

        public UC_OperatorViewModel()
        {
            // 初始化图像信息集合
            FAImageInfos = new ObservableCollection<PropertyModel>
            {
                new PropertyModel { PropertyName = "长度", PropertyValue = "null" },
                new PropertyModel { PropertyName = "宽度", PropertyValue = "null" },
                new PropertyModel { PropertyName = "位深度", PropertyValue = "null" },
                new PropertyModel { PropertyName = "类型", PropertyValue = "null" },
                new PropertyModel { PropertyName = "大小", PropertyValue = "null" }
            };
            // 初始化圆信息集合
            CircleInfos = new ObservableCollection<PropertyModel>
            {
                new PropertyModel { PropertyName = "圆心", PropertyValue = "null" },
                new PropertyModel { PropertyName = "半径", PropertyValue = "null" }
            };
            // 初始化矩形信息集合
            RectInfos = new ObservableCollection<PropertyModel>
            {
                new PropertyModel { PropertyName = "长度", PropertyValue = "null" },
                new PropertyModel { PropertyName = "宽度", PropertyValue = "null" }
            };
            // 初始化椭圆信息集合
            EllipseInfos = new ObservableCollection<PropertyModel>
            {
                new PropertyModel { PropertyName = "圆心", PropertyValue = "null" },
                new PropertyModel { PropertyName = "长轴", PropertyValue = "null" },
                new PropertyModel { PropertyName = "短轴", PropertyValue = "null" }
            };

            T1Model = new PlotModel { Title = "Gray histogram" };
            T1Model.Series.Add(new FunctionSeries(Math.Cos, 0, 10, 0.1));

            //var blueLine = new LineSeries { Color = OxyColors.Blue, InterpolationAlgorithm = InterpolationAlgorithms.CanonicalSpline };
            //T1Model.Series.Add(blueLine);
            T1Model.Axes.Add(new LinearAxis { Position = AxisPosition.Left, Minimum = -1, Maximum = 1, MajorStep = 0.2, MinorStep = 0.05 });
            T1Model.Axes.Add(new LinearAxis { Position = AxisPosition.Bottom, Minimum = 0, Maximum = 10 });

            T2Model = new PlotModel { Title = "FFT" };
            T2Model.Series.Add(new FunctionSeries(Math.Sin, 0, 10, 0.1));
            T2Model.Axes.Add(new LinearAxis { Position = AxisPosition.Left, Minimum = -1, Maximum = 1, MajorStep = 0.2, MinorStep = 0.05 });
            T2Model.Axes.Add(new LinearAxis { Position = AxisPosition.Bottom, Minimum = 0, Maximum = 10 });
        }

        #endregion

        #region Pubilc field

        /// <summary>
        /// 图像属性集合
        /// </summary>
        public ObservableCollection<PropertyModel> FAImageInfos { set; get; }

        /// <summary>
        /// 图像属性集合-圆形
        /// </summary>
        public ObservableCollection<PropertyModel> CircleInfos { set; get; }

        /// <summary>
        /// 图像属性集合-矩形
        /// </summary>
        public ObservableCollection<PropertyModel> RectInfos { set; get; }

        /// <summary>
        /// 图像属性集合-椭圆
        /// </summary>
        public ObservableCollection<PropertyModel> EllipseInfos { set; get; }

        public string Test { set; get; }

        /// <summary>
        /// 测试用Plot模型1
        /// </summary>
        public PlotModel T1Model { get; private set; }

        /// <summary>
        /// 测试用Plot模型2
        /// </summary>
        public PlotModel T2Model { get; private set; }

        #endregion
    }
}
