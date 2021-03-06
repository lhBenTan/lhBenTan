using System;
using System.Drawing;
using System.Reflection;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Interop;
using System.Windows.Media;
using System.Runtime.InteropServices;
using System.Windows.Media.Imaging;
using System.Windows.Threading;
using Brush = System.Windows.Media.Brush;
using Point = System.Windows.Point;
using Size = System.Windows.Size;
using System.Collections.Generic;
using System.Collections;
using System.Linq;

namespace AdonisUI
{
    /// <summary>
    /// Window with custom chrome supporting theming of non-client areas
    /// </summary>
    [TemplatePart(Name = PART_DragMoveThumb, Type = typeof(FrameworkElement))]
    [TemplatePart(Name = PART_IconPresenter, Type = typeof(FrameworkElement))]
    [TemplatePart(Name = PART_MinimizeButton, Type = typeof(Button))]
    [TemplatePart(Name = PART_MaximizeRestoreButton, Type = typeof(Button))]
    [TemplatePart(Name = PART_CloseButton, Type = typeof(Button))]
    public class AdonisWindow
        : Window
    {
        private const string PART_DragMoveThumb = "PART_DragMoveThumb";
        private const string PART_IconPresenter = "PART_IconPresenter";
        private const string PART_MinimizeButton = "PART_MinimizeButton";
        private const string PART_MaximizeRestoreButton = "PART_MaximizeRestoreButton";
        private const string PART_CloseButton = "PART_CloseButton";

        protected HwndInterop HwndInterop { get; private set; }

        public FrameworkElement DragMoveThumb { get; protected set; }

        public FrameworkElement IconPresenter { get; protected set; }

        public Button MinimizeButton { get; protected set; }

        public Button MaximizeRestoreButton { get; protected set; }

        public Button CloseButton { get; protected set; }

        /// <summary>
        /// Gets or sets the visibility of the icon component of the window.
        /// </summary>
        public Visibility IconVisibility
        {
            get => (Visibility)GetValue(IconVisibilityProperty);
            set => SetValue(IconVisibilityProperty, value);
        }

        /// <summary>
        /// Gets or sets the window's icon as <see cref="ImageSource">ImageSource</see>.
        /// When the <see cref="Window.IconProperty">IconProperty</see> property changes, this is updated accordingly.
        /// </summary>
        protected internal ImageSource IconSource
        {
            get => (ImageSource)GetValue(IconSourceProperty);
            set => SetValue(IconSourceProperty, value);
        }

        /// <summary>
        /// Gets or sets the content of the window's title bar
        /// between the title and the window buttons.
        /// </summary>
        public object TitleBarContent
        {
            get => (object)GetValue(TitleBarContentProperty);
            set => SetValue(TitleBarContentProperty, value);
        }

        /// <summary>
        /// Gets or sets the foreground brush of the window's title bar.
        /// </summary>
        public Brush TitleBarForeground
        {
            get { return (Brush)GetValue(TitleBarForegroundProperty); }
            set { SetValue(TitleBarForegroundProperty, value); }
        }

        /// <summary>
        /// Gets or sets the background brush of the window's title bar.
        /// </summary>
        public Brush TitleBarBackground
        {
            get { return (Brush)GetValue(TitleBarBackgroundProperty); }
            set { SetValue(TitleBarBackgroundProperty, value); }
        }

        /// <summary>
        /// Gets or sets the background brush of the minimize, maximize and restore
        /// buttons when they are hovered.
        /// </summary>
        public Brush WindowButtonHighlightBrush
        {
            get { return (Brush)GetValue(WindowButtonHighlightBrushProperty); }
            set { SetValue(WindowButtonHighlightBrushProperty, value); }
        }

        /// <summary>
        /// Gets the size of the display overlapping area when the window is maximized.
        /// </summary>
        protected internal Thickness MaximizeBorderThickness
        {
            get => (Thickness)GetValue(MaximizeBorderThicknessProperty);
            private set => SetValue(MaximizeBorderThicknessPropertyKey, value);
        }

        public static readonly DependencyProperty IconVisibilityProperty = DependencyProperty.Register("IconVisibility", typeof(Visibility), typeof(AdonisWindow), new PropertyMetadata(Visibility.Visible));

        protected internal static readonly DependencyProperty IconSourceProperty = DependencyProperty.Register("IconSource", typeof(ImageSource), typeof(AdonisWindow), new PropertyMetadata(null));

        public static readonly DependencyProperty TitleBarContentProperty = DependencyProperty.Register("TitleBarContent", typeof(object), typeof(AdonisWindow), new PropertyMetadata(null));

        public static readonly DependencyProperty TitleBarForegroundProperty = DependencyProperty.Register("TitleBarForeground", typeof(Brush), typeof(AdonisWindow), new PropertyMetadata(null));

        public static readonly DependencyProperty TitleBarBackgroundProperty = DependencyProperty.Register("TitleBarBackground", typeof(Brush), typeof(AdonisWindow), new PropertyMetadata(null));

        public static readonly DependencyProperty WindowButtonHighlightBrushProperty = DependencyProperty.Register("WindowButtonHighlightBrush", typeof(Brush), typeof(AdonisWindow), new PropertyMetadata(null));

        protected internal static readonly DependencyPropertyKey MaximizeBorderThicknessPropertyKey = DependencyProperty.RegisterReadOnly("MaximizeBorderThickness", typeof(Thickness), typeof(AdonisWindow), new PropertyMetadata(new Thickness()));

        protected internal static readonly DependencyProperty MaximizeBorderThicknessProperty = MaximizeBorderThicknessPropertyKey.DependencyProperty;

        static AdonisWindow()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(AdonisWindow), new FrameworkPropertyMetadata(typeof(AdonisWindow)));
            IconProperty.OverrideMetadata(typeof(AdonisWindow), new FrameworkPropertyMetadata(OnIconPropertyChanged));
        }

        private static void OnIconPropertyChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            if (!(d is AdonisWindow sourceWindow))
                return;

            string newIcon = e.NewValue.ToString();

            sourceWindow.IconSource = String.IsNullOrEmpty(newIcon) ? null : new BitmapImage(new Uri(newIcon));
        }

        /// <inheritdoc/>
        public AdonisWindow()
        {
            IconSource = GetApplicationIcon();
            MaximizeBorderThickness = GetSystemMaximizeBorderThickness();
        }

        private BitmapSource GetApplicationIcon()
        {
            Icon appIcon = System.Drawing.Icon.ExtractAssociatedIcon(Assembly.GetEntryAssembly()?.ManifestModule.Name);

            if (appIcon == null)
                return null;

            return Imaging.CreateBitmapSourceFromHIcon(appIcon.Handle, Int32Rect.Empty, BitmapSizeOptions.FromEmptyOptions());
        }

        private Thickness GetSystemMaximizeBorderThickness()
        {
            Thickness frameThickness = SystemParameters.WindowNonClientFrameThickness;
            Thickness resizeBorderThickness = SystemParameters.WindowResizeBorderThickness;

            return new Thickness(
                frameThickness.Left + resizeBorderThickness.Left - 1,
                frameThickness.Top + resizeBorderThickness.Top - SystemParameters.CaptionHeight - 1,
                frameThickness.Right + resizeBorderThickness.Right - 1,
                frameThickness.Bottom + resizeBorderThickness.Bottom - 1);
        }

        /// <inheritdoc/>
        public override void OnApplyTemplate()
        {
            base.OnApplyTemplate();

            HwndInterop = new HwndInterop(this);

            DragMoveThumb = GetTemplateChild(PART_DragMoveThumb) as FrameworkElement;
            IconPresenter = GetTemplateChild(PART_IconPresenter) as FrameworkElement;
            MinimizeButton = GetTemplateChild(PART_MinimizeButton) as Button;
            MaximizeRestoreButton = GetTemplateChild(PART_MaximizeRestoreButton) as Button;
            CloseButton = GetTemplateChild(PART_CloseButton) as Button;

            if (DragMoveThumb != null)
                InitDragMoveThumb(DragMoveThumb);
            if (IconPresenter != null)
                InitIconPresenter(IconPresenter);
            if (MinimizeButton != null)
                InitMinimizeButton(MinimizeButton);
            if (MaximizeRestoreButton != null)
                InitMaximizeRestoreButton(MaximizeRestoreButton);
            if (CloseButton != null)
                InitCloseButton(CloseButton);

            UpdateLayoutForSizeToContent();
            HwndInterop.PositionChanging += DisableSizeToContentWhenMaximizing;
        }

        /// <summary>
        /// Initializes functionality of the drag/move thumb component of the window's title bar.
        /// </summary>
        /// <param name="dragMoveThumb">The drag/move thumb component of the window</param>
        protected virtual void InitDragMoveThumb(FrameworkElement dragMoveThumb)
        {
            dragMoveThumb.MouseLeftButtonDown += (s, e) =>
            {
                if (e.ChangedButton == MouseButton.Left)
                {
                    if (WindowState == WindowState.Maximized)
                        dragMoveThumb.MouseMove += RestoreOnMouseMove;

                    if (Mouse.LeftButton == MouseButtonState.Pressed)
                        DragMove();
                }

                if (e.ClickCount == 2 &&
                    (ResizeMode == ResizeMode.CanResize || ResizeMode == ResizeMode.CanResizeWithGrip))
                {
                    ToggleWindowState();
                }
            };

            dragMoveThumb.MouseLeftButtonUp += (s, e) => dragMoveThumb.MouseMove -= RestoreOnMouseMove;

            dragMoveThumb.MouseRightButtonUp += (s, e) => OpenSystemContextMenu(e.GetPosition(this));
        }

        /// <summary>
        /// Initializes functionality of the minimize button of the window's title bar.
        /// </summary>
        /// <param name="minimizeButton">The minimize button of the window</param>
        protected virtual void InitMinimizeButton(Button minimizeButton)
        {
            minimizeButton.Click += MinimizeClick;
        }

        /// <summary>
        /// Initializes functionality of the maximize/restore button of the window's title bar.
        /// </summary>
        /// <param name="maximizeRestoreButton">The maximize/restore button of the window</param>
        protected virtual void InitMaximizeRestoreButton(Button maximizeRestoreButton)
        {
            maximizeRestoreButton.Click += MaximizeRestoreClick;
        }

        /// <summary>
        /// Initializes functionality of the close button of the window's title bar.
        /// </summary>
        /// <param name="closeButton">The close button of the window</param>
        protected virtual void InitCloseButton(Button closeButton)
        {
            closeButton.Click += CloseClick;
        }

        /// <summary>
        /// Initializes functionality of the icon presenter component of the window's title bar.
        /// </summary>
        /// <param name="iconPresenter">The icon presenter component of the window</param>
        protected virtual void InitIconPresenter(FrameworkElement iconPresenter)
        {
            iconPresenter.MouseLeftButtonDown += (s, e) =>
            {
                if (e.ClickCount == 2)
                {
                    Close();
                    return;
                }

                var anchorElement = DragMoveThumb ?? IconPresenter;
                var menuPosition = anchorElement.TranslatePoint(new Point(0, anchorElement.ActualHeight), this);
                OpenSystemContextMenu(menuPosition);
            };
        }

        /// <summary>
        /// Handles the close button's click event.
        /// </summary>
        protected virtual void CloseClick(object sender, RoutedEventArgs e)
        {
            Close();
        }

        /// <summary>
        /// Handles the maximize/restore button's click event.
        /// </summary>
        protected virtual void MaximizeRestoreClick(object sender, RoutedEventArgs e)
        {
            ToggleWindowState();
        }

        /// <summary>
        /// Handles the minimize button's click event.
        /// </summary>
        protected virtual void MinimizeClick(object sender, RoutedEventArgs e)
        {
            WindowState = WindowState.Minimized;
        }

        /// <summary>
        /// Sets the <see cref="Window.WindowState"/> to <see cref="WindowState.Maximized"/>
        /// if it is currently at <see cref="WindowState.Normal"/> or else to <see cref="WindowState.Normal"/>.
        /// </summary>
        protected virtual void ToggleWindowState()
        {
            if (WindowState == WindowState.Normal)
                WindowState = WindowState.Maximized;
            else
                WindowState = WindowState.Normal;
        }

        private void RestoreOnMouseMove(object sender, MouseEventArgs e)
        {
            var dragMoveThumb = sender as FrameworkElement;
            if (dragMoveThumb == null)
                return;

            // detach event handler to ensure it is called only once per mouse down
            dragMoveThumb.MouseMove -= RestoreOnMouseMove;

            // collect given window and screen data
            Point positionInWindow = e.MouseDevice.GetPosition(this);
            Point positionOnScreen = PointToScreen(positionInWindow);
            ScreenInterop currentScreen = ScreenInterop.FromPoint(positionOnScreen);
            Size restoreSizeOnScreen = TransformToScreenCoordinates(new Size(RestoreBounds.Width, RestoreBounds.Height));

            // calculate window's new top left coordinate
            double restoreLeft = positionOnScreen.X - (restoreSizeOnScreen.Width * 0.5);
            double restoreTop = positionOnScreen.Y - MaximizeBorderThickness.Top;

            // make sure the restore bounds are within the current screen bounds
            if (restoreLeft < currentScreen.Bounds.Left)
                restoreLeft = currentScreen.Bounds.Left;
            else if (restoreLeft + restoreSizeOnScreen.Width > currentScreen.Bounds.Right)
                restoreLeft = currentScreen.Bounds.Right - restoreSizeOnScreen.Width;

            // since we calculated with screen values, we need to convert back to window values
            Point restoreTopLeftOnScreen = new Point(restoreLeft, restoreTop);
            Point restoreTopLeft = TransformToWindowCoordinates(restoreTopLeftOnScreen);

            // restore window to calculated position
            Left = restoreTopLeft.X;
            Top = restoreTopLeft.Y;
            WindowState = WindowState.Normal;

            if (Mouse.LeftButton == MouseButtonState.Pressed)
                DragMove();
        }

        /// <summary>
        /// Converts a Size that represents the current coordinate system of the window
        /// into a Size in screen coordinates.
        /// </summary>
        protected Size TransformToScreenCoordinates(Size size)
        {
            PresentationSource presentationSource = PresentationSource.FromVisual(this);

            if (presentationSource?.CompositionTarget == null)
                return size;

            Matrix transformToDevice = presentationSource.CompositionTarget.TransformToDevice;
            return (Size)transformToDevice.Transform(new Vector(size.Width, size.Height));
        }

        /// <summary>
        /// Converts a Point that represents the native coordinate system of the screen
        /// into a Point in device independent coordinates.
        /// </summary>
        protected Point TransformToWindowCoordinates(Point point)
        {
            PresentationSource presentationSource = PresentationSource.FromVisual(this);

            if (presentationSource?.CompositionTarget == null)
                return point;

            Matrix transformFromDevice = presentationSource.CompositionTarget.TransformFromDevice;
            return transformFromDevice.Transform(point);
        }

        /// <summary>
        /// Displays the system's native window context menu at the given position.
        /// </summary>
        /// <param name="positionInWindow">Coordinate of top left corner of the context menu relative to the window</param>
        protected virtual void OpenSystemContextMenu(Point positionInWindow)
        {
            SystemContextMenuInterop.OpenSystemContextMenu(this, positionInWindow);
        }

        /// <summary>
        /// When using <see cref="SizeToContent.WidthAndHeight"/> the layout might not be calculated correctly
        /// which can result in the window being too large and having large black borders filling the remaining space.
        /// This method can be used to force a layout update again to recalculate the window size correctly.
        /// See https://social.msdn.microsoft.com/Forums/vstudio/en-US/89fe6959-ce1a-4064-bdde-94151df7dc01/gradient-style-issue-when-sizetocontentheightandwidth-with-customchrome?forum=wpf
        /// </summary>
        private void UpdateLayoutForSizeToContent()
        {
            if (SizeToContent == SizeToContent.WidthAndHeight)
            {
                var previousSizeToContent = SizeToContent;
                SizeToContent = SizeToContent.Manual;

                Dispatcher?.BeginInvoke(DispatcherPriority.Loaded, (Action)(() =>
                {
                    SizeToContent = previousSizeToContent;
                }));
            }
        }

        /// <summary>
        /// In order to maximize the window correctly, <see cref="SizeToContent.WidthAndHeight"/> must not be set.
        /// This method ensures that <see cref="SizeToContent.Manual"/> is set when the window is about to be maximized.
        /// </summary>
        private void DisableSizeToContentWhenMaximizing(object sender, HwndInteropPositionChangingEventArgs e)
        {
            if (e.Type == HwndInteropPositionChangingEventArgs.PositionChangeType.MAXIMIZERESTORE)
            {
                SizeToContent = SizeToContent.Manual;
            }
        }
    }
}

#region AdonisUI.Helpers

/// <summary>
/// Helper class for interactions with system window events
/// </summary>
public class HwndInterop
{
    [DllImport("user32.dll")]
    public static extern IntPtr SendMessage(IntPtr hwnd, uint Msg, IntPtr wParam, IntPtr lParam);

    [StructLayout(LayoutKind.Sequential)]
    public struct WINDOWPOS
    {
        public IntPtr hwndInsertAfter;
        public IntPtr hwnd;
        public int x;
        public int y;
        public int cx;
        public int cy;
        public uint flags;
    }

    private const Int32 WM_SYSCOMMAND = 0x112;
    private const Int32 WM_SIZE = 0x0005;
    private const Int32 WM_WINDOWPOSCHANGING = 0x0046;

    private const Int32 SC_MAXIMIZE = 0xF030;
    private const Int32 SC_RESTORE = 0xF120;
    private const Int32 SC_MINIMIZE = 0xF020;

    private readonly IntPtr _handle;

    /// <summary>
    /// Is raised when the <see cref="WM_SIZE"/> is occuring.
    /// </summary>
    public event EventHandler<HwndInteropSizeChangedEventArgs> SizeChanged;

    /// <summary>
    /// Is raised when the <see cref="WM_WINDOWPOSCHANGING"/> is occuring.
    /// </summary>
    public event EventHandler<HwndInteropPositionChangingEventArgs> PositionChanging;

    /// <summary>
    /// Helper class for interactions with system window events
    /// </summary>
    public HwndInterop(Window window)
    {
        _handle = new WindowInteropHelper(window).Handle;

        HwndSource source = HwndSource.FromHwnd(_handle);
        source?.AddHook(WndProc);
    }

    private IntPtr WndProc(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, ref bool handled)
    {
        switch (msg)
        {
            case WM_SIZE:
                SizeChanged?.Invoke(this, new HwndInteropSizeChangedEventArgs((HwndInteropSizeChangedEventArgs.ResizeRequestType)wParam));
                break;
            case WM_WINDOWPOSCHANGING:
                WINDOWPOS windowPos = (WINDOWPOS)Marshal.PtrToStructure(lParam, typeof(WINDOWPOS));
                PositionChanging?.Invoke(this, new HwndInteropPositionChangingEventArgs((HwndInteropPositionChangingEventArgs.PositionChangeType)windowPos.flags));
                break;
        }

        return IntPtr.Zero;
    }

    /// <summary>
    /// Sends a system event to maximize the window.
    /// </summary>
    public void Maximize()
    {
        SendMessage(_handle, WM_SYSCOMMAND, (IntPtr)SC_MAXIMIZE, IntPtr.Zero);
    }

    /// <summary>
    /// Sends a system event to restore the window.
    /// </summary>
    public void Restore()
    {
        SendMessage(_handle, WM_SYSCOMMAND, (IntPtr)SC_RESTORE, IntPtr.Zero);
    }

    /// <summary>
    /// Sends a system event to minimize the window.
    /// </summary>
    public void Minimize()
    {
        SendMessage(_handle, WM_SYSCOMMAND, (IntPtr)SC_MINIMIZE, IntPtr.Zero);
    }
}

/// <summary>
/// Represents a display device or multiple display devices on a single system.
/// See https://github.com/micdenny/WpfScreenHelper/
/// </summary>
internal class ScreenInterop
{
    [DllImport("user32.dll", CharSet = CharSet.Auto)]
    public static extern bool GetMonitorInfo(HandleRef hmonitor, [In, Out] MONITORINFOEX info);

    [DllImport("user32.dll", ExactSpelling = true)]
    public static extern bool EnumDisplayMonitors(IntPtr hdc, COMRECT rcClip, MonitorEnumProc lpfnEnum, IntPtr dwData);

    [DllImport("user32.dll", ExactSpelling = true)]
    public static extern IntPtr MonitorFromWindow(HandleRef handle, int flags);

    [DllImport("user32.dll", ExactSpelling = true, CharSet = CharSet.Auto)]
    public static extern int GetSystemMetrics(int nIndex);

    [DllImport("user32.dll", CharSet = CharSet.Auto)]
    public static extern bool SystemParametersInfo(int nAction, int nParam, ref RECT rc, int nUpdate);

    [DllImport("user32.dll", ExactSpelling = true)]
    public static extern IntPtr MonitorFromPoint(POINTSTRUCT pt, int flags);

    [DllImport("user32.dll", ExactSpelling = true, CharSet = CharSet.Auto)]
    public static extern bool GetCursorPos([In, Out] POINT pt);

    // public static HandleRef NullHandleRef;

    public delegate bool MonitorEnumProc(IntPtr monitor, IntPtr hdc, IntPtr lprcMonitor, IntPtr lParam);

    [StructLayout(LayoutKind.Sequential)]
    public struct RECT
    {
        public int left;
        public int top;
        public int right;
        public int bottom;

        public RECT(int left, int top, int right, int bottom)
        {
            this.left = left;
            this.top = top;
            this.right = right;
            this.bottom = bottom;
        }

        public RECT(Rect r)
        {
            this.left = (int)r.Left;
            this.top = (int)r.Top;
            this.right = (int)r.Right;
            this.bottom = (int)r.Bottom;
        }

        public static RECT FromXYWH(int x, int y, int width, int height)
        {
            return new RECT(x, y, x + width, y + height);
        }

        public Size Size
        {
            get { return new Size(this.right - this.left, this.bottom - this.top); }
        }
    }

    // use this in cases where the Native API takes a POINT not a POINT*
    // classes marshal by ref.
    [StructLayout(LayoutKind.Sequential)]
    public struct POINTSTRUCT
    {
        public int x;
        public int y;
        public POINTSTRUCT(int x, int y)
        {
            this.x = x;
            this.y = y;
        }
    }

    [StructLayout(LayoutKind.Sequential)]
    public class POINT
    {
        public int x;
        public int y;

        public POINT()
        {
        }

        public POINT(int x, int y)
        {
            this.x = x;
            this.y = y;
        }

#if DEBUG
        public override string ToString()
        {
            return "{x=" + x + ", y=" + y + "}";
        }
#endif
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Auto, Pack = 4)]
    public class MONITORINFOEX
    {
        internal int cbSize = Marshal.SizeOf(typeof(MONITORINFOEX));
        internal RECT rcMonitor = new RECT();
        internal RECT rcWork = new RECT();
        internal int dwFlags = 0;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 32)] internal char[] szDevice = new char[32];
    }

    [StructLayout(LayoutKind.Sequential)]
    public class COMRECT
    {
        public int left;
        public int top;
        public int right;
        public int bottom;

        public COMRECT()
        {
        }

        public COMRECT(Rect r)
        {
            this.left = (int)r.X;
            this.top = (int)r.Y;
            this.right = (int)r.Right;
            this.bottom = (int)r.Bottom;
        }

        public COMRECT(int left, int top, int right, int bottom)
        {
            this.left = left;
            this.top = top;
            this.right = right;
            this.bottom = bottom;
        }

        public static COMRECT FromXYWH(int x, int y, int width, int height)
        {
            return new COMRECT(x, y, x + width, y + height);
        }

        public override string ToString()
        {
            return "Left = " + left + " Top " + top + " Right = " + right + " Bottom = " + bottom;
        }
    }

    public const int SM_CMONITORS = 80;

    private readonly IntPtr hmonitor;

    // This identifier is just for us, so that we don't try to call the multimon
    // functions if we just need the primary monitor... this is safer for
    // non-multimon OSes.
    private const int PRIMARY_MONITOR = unchecked((int)0xBAADF00D);

    private const int MONITORINFOF_PRIMARY = 0x00000001;

    private const int MONITOR_DEFAULTTONEAREST = 0x00000002;

    private static readonly bool MultiMonitorSupport;

    static ScreenInterop()
    {
        MultiMonitorSupport = GetSystemMetrics(SM_CMONITORS) != 0;
    }

    private ScreenInterop(IntPtr monitor)
        : this(monitor, IntPtr.Zero)
    {
    }

    private ScreenInterop(IntPtr monitor, IntPtr hdc)
    {
        if (!MultiMonitorSupport || monitor == (IntPtr)PRIMARY_MONITOR)
        {
            this.Bounds = new Rect(SystemParameters.VirtualScreenLeft, SystemParameters.VirtualScreenTop, SystemParameters.VirtualScreenWidth, SystemParameters.VirtualScreenHeight);
            this.Primary = true;
            this.DeviceName = "DISPLAY";
        }
        else
        {
            var info = new MONITORINFOEX();

            GetMonitorInfo(new HandleRef(null, monitor), info);

            this.Bounds = new Rect(
                info.rcMonitor.left, info.rcMonitor.top,
                info.rcMonitor.right - info.rcMonitor.left,
                info.rcMonitor.bottom - info.rcMonitor.top);

            this.Primary = ((info.dwFlags & MONITORINFOF_PRIMARY) != 0);

            this.DeviceName = new string(info.szDevice).TrimEnd((char)0);
        }
        hmonitor = monitor;
    }

    /// <summary>
    /// Gets an array of all displays on the system.
    /// </summary>
    /// <returns>An enumerable of type Screen, containing all displays on the system.</returns>
    public static IEnumerable<ScreenInterop> AllScreens
    {
        get
        {
            if (MultiMonitorSupport)
            {
                var closure = new MonitorEnumCallback();
                var proc = new MonitorEnumProc(closure.Callback);

                EnumDisplayMonitors(IntPtr.Zero, null, proc, IntPtr.Zero);

                if (closure.Screens.Count > 0)
                    return closure.Screens.Cast<ScreenInterop>();
            }

            return new[] { new ScreenInterop((IntPtr)PRIMARY_MONITOR) };
        }
    }

    /// <summary>
    /// Gets the bounds of the display.
    /// </summary>
    /// <returns>A <see cref="T:System.Windows.Rect" />, representing the bounds of the display.</returns>
    public Rect Bounds { get; private set; }

    /// <summary>
    /// Gets the device name associated with a display.
    /// </summary>
    /// <returns>The device name associated with a display.</returns>
    public string DeviceName { get; private set; }

    /// <summary>
    /// Gets a value indicating whether a particular display is the primary device.
    /// </summary>
    /// <returns>true if this display is primary; otherwise, false.</returns>
    public bool Primary { get; private set; }

    /// <summary>
    /// Gets the primary display.
    /// </summary>
    /// <returns>The primary display.</returns>
    public static ScreenInterop PrimaryScreen
    {
        get
        {
            if (MultiMonitorSupport)
            {
                return AllScreens.FirstOrDefault(t => t.Primary);
            }
            return new ScreenInterop((IntPtr)PRIMARY_MONITOR);
        }
    }

    /// <summary>
    /// Gets the working area of the display. The working area is the desktop area of the display, excluding taskbars, docked windows, and docked tool bars.
    /// </summary>
    /// <returns>A <see cref="T:System.Windows.Rect" />, representing the working area of the display.</returns>
    public Rect WorkingArea
    {
        get
        {
            if (!MultiMonitorSupport || hmonitor == (IntPtr)PRIMARY_MONITOR)
            {
                return SystemParameters.WorkArea;
            }
            var info = new MONITORINFOEX();
            GetMonitorInfo(new HandleRef(null, hmonitor), info);
            return new Rect(
                info.rcWork.left, info.rcWork.top,
                info.rcWork.right - info.rcWork.left,
                info.rcWork.bottom - info.rcWork.top);
        }
    }

    /// <summary>
    /// Retrieves a Screen for the display that contains the largest portion of the specified control.
    /// </summary>
    /// <param name="hwnd">The window handle for which to retrieve the Screen.</param>
    /// <returns>A Screen for the display that contains the largest region of the object. In multiple display environments where no display contains any portion of the specified window, the display closest to the object is returned.</returns>
    public static ScreenInterop FromHandle(IntPtr hwnd)
    {
        if (MultiMonitorSupport)
        {
            return new ScreenInterop(MonitorFromWindow(new HandleRef(null, hwnd), 2));
        }
        return new ScreenInterop((IntPtr)PRIMARY_MONITOR);
    }

    /// <summary>
    /// Retrieves a Screen for the display that contains the specified point.
    /// </summary>
    /// <param name="point">A <see cref="T:System.Windows.Point" /> that specifies the location for which to retrieve a Screen.</param>
    /// <returns>A Screen for the display that contains the point. In multiple display environments where no display contains the point, the display closest to the specified point is returned.</returns>
    public static ScreenInterop FromPoint(Point point)
    {
        if (MultiMonitorSupport)
        {
            var pt = new POINTSTRUCT((int)point.X, (int)point.Y);
            return new ScreenInterop(MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST));
        }
        return new ScreenInterop((IntPtr)PRIMARY_MONITOR);
    }

    /// <summary>
    /// Gets or sets a value indicating whether the specified object is equal to this Screen.
    /// </summary>
    /// <param name="obj">The object to compare to this Screen.</param>
    /// <returns>true if the specified object is equal to this Screen; otherwise, false.</returns>
    public override bool Equals(object obj)
    {
        return obj is ScreenInterop monitor && hmonitor == monitor.hmonitor;
    }

    /// <summary>
    /// Computes and retrieves a hash code for an object.
    /// </summary>
    /// <returns>A hash code for an object.</returns>
    public override int GetHashCode()
    {
        return (int)hmonitor;
    }

    private class MonitorEnumCallback
    {
        public ArrayList Screens { get; private set; }

        public MonitorEnumCallback()
        {
            this.Screens = new ArrayList();
        }

        public bool Callback(IntPtr monitor, IntPtr hdc, IntPtr lprcMonitor, IntPtr lparam)
        {
            this.Screens.Add(new ScreenInterop(monitor, hdc));
            return true;
        }
    }
}

/// <summary>
/// Helper class for interactions with the system's native context menu
/// </summary>
internal static class SystemContextMenuInterop
{
    public static uint TPM_LEFTALIGN = 0;

    public static uint TPM_RETURNCMD = 256;

    [DllImport("user32.dll", CharSet = CharSet.None, ExactSpelling = false)]
    public static extern IntPtr PostMessage(IntPtr hWnd, uint msg, IntPtr wParam, IntPtr lParam);

    [DllImport("user32.dll", CharSet = CharSet.Auto, ExactSpelling = false, SetLastError = true)]
    public static extern IntPtr GetSystemMenu(IntPtr hWnd, bool bRevert);

    [DllImport("user32.dll", CharSet = CharSet.None, ExactSpelling = false)]
    public static extern bool EnableMenuItem(IntPtr hMenu, uint uIDEnableItem, uint uEnable);

    [DllImport("user32.dll", CharSet = CharSet.None, ExactSpelling = false)]
    public static extern int TrackPopupMenuEx(IntPtr hmenu, uint fuFlags, int x, int y, IntPtr hwnd, IntPtr lptpm);

    private static int c = 0;

    public static void OpenSystemContextMenu(Window window, Point positionInWindow)
    {
        Point screenCoordinate = window.PointToScreen(positionInWindow);
        IntPtr windowHandle = new WindowInteropHelper(window).Handle;
        IntPtr systemMenu = GetSystemMenu(windowHandle, false);

        c++;

        EnableMenuItem(systemMenu, 61488, c % 2 == 0 ? (window.WindowState != WindowState.Maximized ? 0 : (uint)1) : 2);

        int track = TrackPopupMenuEx(
            systemMenu,
            TPM_LEFTALIGN | TPM_RETURNCMD,
            Convert.ToInt32(screenCoordinate.X),
            Convert.ToInt32(screenCoordinate.Y),
            windowHandle,
            IntPtr.Zero);

        if (track == 0)
            return;

        PostMessage(windowHandle, 274, new IntPtr(track), IntPtr.Zero);
    }
}

public class HwndInteropPositionChangingEventArgs
        : EventArgs
{
    public enum PositionChangeType
    {
        /// <summary>
        /// Draws a frame (defined in the window's class description) around the window. Same as the <see cref="FRAMECHANGED"/> flag.
        /// </summary>
        DRAWFRAME = 0x0020,

        /// <summary>
        /// Sends a WM_NCCALCSIZE message to the window, even if the window's size is not being changed.
        /// </summary>
        FRAMECHANGED = DRAWFRAME,

        /// <summary>
        /// Hides the window.
        /// </summary>
        HIDEWINDOW = 0x0080,

        /// <summary>
        /// Does not activate the window.
        /// </summary>
        NOACTIVATE = 0x0010,

        /// <summary>
        /// Discards the entire contents of the client area.
        /// </summary>
        NOCOPYBITS = 0x0100,

        /// <summary>
        /// Retains the current position (ignores the x and y members).
        /// </summary>
        NOMOVE = 0x0002,

        /// <summary>
        /// Does not change the owner window's position in the Z order.
        /// </summary>
        NOOWNERZORDER = 0x0200,

        /// <summary>
        /// Does not redraw changes.
        /// </summary>
        SWP_NOREDRAW = 0x0008,

        /// <summary>
        /// Does not change the owner window's position in the Z order. Same as the <see cref="NOOWNERZORDER"/> flag.
        /// </summary>
        NOREPOSITION = NOOWNERZORDER,

        /// <summary>
        /// Prevents the window from receiving the WM_WINDOWPOSCHANGING message.
        /// </summary>
        NOSENDCHANGING = 0x0400,

        /// <summary>
        /// Retains the current size (ignores the cx and cy members).
        /// </summary>
        NOSIZE = 0x0001,

        /// <summary>
        /// Retains the current Z order (ignores the hwndInsertAfter member).
        /// </summary>
        NOZORDER = 0x0004,

        /// <summary>
        /// Displays the window.
        /// </summary>
        SHOWWINDOW = 0x0040,

        /// <summary>
        /// No official documentation found. Seems to occur whe maximizing or restoring a window.
        /// </summary>
        MAXIMIZERESTORE = 0x8020,
    }

    public PositionChangeType Type { get; private set; }

    public HwndInteropPositionChangingEventArgs(PositionChangeType positionChangeType)
    {
        Type = positionChangeType;
    }
}

public class HwndInteropSizeChangedEventArgs
        : EventArgs
{
    /// <summary>
    /// The type of resizing requested.
    /// </summary>
    public enum ResizeRequestType
    {
        /// <summary>
        /// The window has been resized, but neither the <see cref="Minimized"/> nor <see cref="Maximized"/> value applies.
        /// </summary>
        Restored = 0,

        /// <summary>
        /// The window has been minimized.

        /// </summary>
        Minimized = 1,

        /// <summary>
        /// The window has been maximized.
        /// </summary>
        Maximized = 2,

        /// <summary>
        /// Message is sent to all pop-up windows when some other window has been restored to its former size.
        /// </summary>
        MaxShow = 3,

        /// <summary>
        /// Message is sent to all pop-up windows when some other window is maximized.
        /// </summary>
        MaxHide = 4,
    }

    /// <summary>
    /// The type of resizing requested.
    /// </summary>
    public ResizeRequestType Type { get; private set; }

    public HwndInteropSizeChangedEventArgs(ResizeRequestType resizeRequestType)
    {
        Type = resizeRequestType;
    }
}

#endregion