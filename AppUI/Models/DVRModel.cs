using DevExpress.Mvvm;

namespace AppUI.Models
{
    public class  DVRModel : ViewModelBase
    {
        //当前吸头
        public string YTJ_NowXT
        {
            get => GetProperty(() => YTJ_NowXT);
            set => SetProperty(() => YTJ_NowXT, value);
        }

        //吸头数量
        public int YTJ_XTNums
        {
            get => GetProperty(() => YTJ_XTNums);
            set => SetProperty(() => YTJ_XTNums, value);
        }
        //算法类型
        public string YTJ_SFType
        {
            get => GetProperty(() => YTJ_SFType);
            set => SetProperty(() => YTJ_SFType, value);
        }
        //旋转角度
        public string Angle
        {
            get => GetProperty(() => Angle);
            set => SetProperty(() => Angle, value);
        }

        //整体结果
        public string OK_OR_NG
        {
            get => GetProperty(() => OK_OR_NG);
            set => SetProperty(() => OK_OR_NG, value);
        }

        //匹配结果
        public string ComparaResult
        {
            get => GetProperty(() => ComparaResult);
            set => SetProperty(() => ComparaResult, value);
        }

        //镜筒良率
        public double JT_Sum
        {
            get => GetProperty(() => JT_Sum);
            set => SetProperty(() => JT_Sum, value);
        }
        public double JT_OKSum
        {
            get => GetProperty(() => JT_OKSum);
            set => SetProperty(() => JT_OKSum, value);
        }

        //镜片良率
        public double Lens_Sum
        {
            get => GetProperty(() => Lens_Sum);
            set => SetProperty(() => Lens_Sum, value);
        }
        public double Lens_OKSum
        {
            get => GetProperty(() => Lens_OKSum);
            set => SetProperty(() => Lens_OKSum, value);
        }

        //模号
        public string ModNum_01
        {
            get => GetProperty(() => ModNum_01);
            set => SetProperty(() => ModNum_01, value);
        }
        public string ModNum_02
        {
            get => GetProperty(() => ModNum_02);
            set => SetProperty(() => ModNum_02, value);
        }
        public string ModNum_03
        {
            get => GetProperty(() => ModNum_03);
            set => SetProperty(() => ModNum_03, value);
        }

        //偏差值
        ///偏差值
        public double Deviation_X
        {
            get => GetProperty(() => Deviation_X);
            set => SetProperty(() => Deviation_X, double.Parse(string.Format("{0:F3}", value)));
        }
        public double Deviation_Y
        {
            get => GetProperty(() => Deviation_Y);
            set => SetProperty(() => Deviation_Y, double.Parse(string.Format("{0:F3}", value)));
        }
    }
}
