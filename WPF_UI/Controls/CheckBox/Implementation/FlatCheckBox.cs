using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace ZdfFlatUI
{
    public enum CheckBoxSkinsEnum
    {
        /// <summary>
        /// 方形的CheckBox
        /// </summary>
        DefaultSquare,
        /// <summary>
        /// 圆形的CheckBox
        /// </summary>
        DefaultEllipse,
        EllipseSkin1,
    }

    public class FlatCheckBox : CheckBox
    {
        static FlatCheckBox()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(FlatCheckBox), new FrameworkPropertyMetadata(typeof(FlatCheckBox)));
        }

        #region 依赖属性
        public static readonly DependencyProperty SkinsProperty = DependencyProperty.Register("Skins"
            , typeof(CheckBoxSkinsEnum), typeof(FlatCheckBox), new PropertyMetadata(CheckBoxSkinsEnum.DefaultSquare));
        /// <summary>
        /// CheckBox皮肤样式
        /// </summary>
        public CheckBoxSkinsEnum Skins
        {
            get { return (CheckBoxSkinsEnum)GetValue(SkinsProperty); }
            set { SetValue(SkinsProperty, value); }
        }

        public static readonly DependencyProperty UnCheckedColorProperty = DependencyProperty.Register("UnCheckedColor"
            , typeof(Brush), typeof(FlatCheckBox));
        /// <summary>
        /// CheckBox未选中时的颜色
        /// </summary>
        public Brush UnCheckedColor
        {
            get { return (Brush)GetValue(UnCheckedColorProperty); }
            set { SetValue(UnCheckedColorProperty, value); }
        }

        public static readonly DependencyProperty CheckedColorProperty = DependencyProperty.Register("CheckedColor"
            , typeof(Brush), typeof(FlatCheckBox));
        /// <summary>
        /// CheckBox选中后的颜色
        /// </summary>
        public Brush CheckedColor
        {
            get { return (Brush)GetValue(CheckedColorProperty); }
            set { SetValue(CheckedColorProperty, value); }
        }
        #endregion


    }
}
