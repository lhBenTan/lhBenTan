using DevExpress.Mvvm;

namespace AppUI.Models
{
    /// <summary>
    /// 图像信息类，用于左侧算子功能区图像信息
    /// </summary>
    public class PropertyModel : ViewModelBase
    {
        /// <summary>
        /// 属性名
        /// </summary>
        public string PropertyName
        {
            get => GetProperty(() => PropertyName);
            set => SetProperty(() => PropertyName, value);
        }

        /// <summary>
        /// 属性值
        /// </summary>
        public string PropertyValue
        {
            get => GetProperty(() => PropertyValue);
            set => SetProperty(() => PropertyValue, value);
        }
    }
}
