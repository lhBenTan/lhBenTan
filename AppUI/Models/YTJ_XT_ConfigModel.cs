using DevExpress.Mvvm;
using DevExpress.Mvvm.DataAnnotations;
using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Windows;
using System.Windows.Threading;
using System.Xml.Linq;

namespace AppUI.Models
{
    public class YTJ_XT_ConfigModel: ViewModelBase
    {
        #region field
        //配置文件名称
        public string _Configfile;
     
        // 配置文件读写器    
        public XDocument Config;

        //算法参数
        public string[] JT_Config = new string[30];
        public string[] JP_Config = new string[30];
        public string[] DGP_Config = new string[30];
        public string[] PJ_Config = new string[30];

       
        //吸头算法参数初始化Flag
        public string YTJ_XT_CSH_Flag = "";

        #endregion

        #region BindingItem
        ////编号  
        public int[] YTJ_XT_ID
        {
            get => GetProperty(() => YTJ_XT_ID);
            set => SetProperty(() => YTJ_XT_ID, value);
        }

        //主界面吸头切换标志
        public int YTJ_XTItems
        {
            get => GetProperty(() => YTJ_XTItems);
            set => SetProperty(() => YTJ_XTItems, value);
        }


        /// <summary>
        /// 吸头算法参数
        /// </summary>    
        public class YTJ_XT_Item : DispatcherObject, INotifyPropertyChanged
        {
            public int ID = 0;
            string _newData;
            public string Name { get; set; }
            public XDocument Config { get; set; }
            public string ConfigName { get; set; }
            public string NewData
            {
                get { return _newData; }
                set { _newData = value; OnPropertyChanged("NewData"); SaveConfig(value); SFConfigSet(Name, value); }
            }
            private void SaveConfig(string Value)
            {
                Config.Descendants(Name).ElementAt(0).SetValue(Value);
                Config.Save(ConfigName);
            }
            private void SFConfigSet(string Name, string Value)
            {
                char[] name = Name.ToCharArray();
                string flag = name[3].ToString() + name[4].ToString() + name[5].ToString() + name[6].ToString();
                if (flag == "Para")
                {
                    int ParaNums = int.Parse(name[8].ToString() + name[9].ToString());
                    Dispatcher.BeginInvoke(new MChangeDataShow(ChangeDataShow), new ChangeData
                    {
                        ChangeDataName = Name,
                        ChangeDataValue = Value,
                    });
                }
            }
            protected internal virtual void OnPropertyChanged(string propertyName)
            {
                if (PropertyChanged != null)
                    PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
            }
            public event PropertyChangedEventHandler PropertyChanged;

            //修改数据路由回调
            public struct ChangeData
            {
                public string ChangeDataName;
                public string ChangeDataValue;
            }
            public event EventHandler<ChangeData> ChangeDataInvoked;
            private void ChangeDataShow(ChangeData ChangeData_S)
            {
                ChangeDataInvoked?.Invoke(this, ChangeData_S);
            }
            private delegate void MChangeDataShow(ChangeData ChangeData_S);
        }
        public ObservableCollection<YTJ_XT_Item> YTJ_XT_SF
        {
            get => GetProperty(() => YTJ_XT_SF);
            set => SetProperty(() => YTJ_XT_SF, value);
        }
        public ObservableCollection<YTJ_XT_Item> YTJ_XT_JT
        {
            get => GetProperty(() => YTJ_XT_JT);
            set => SetProperty(() => YTJ_XT_JT, value);
        }
        public ObservableCollection<YTJ_XT_Item> YTJ_XT_JP
        {
            get => GetProperty(() => YTJ_XT_JP);
            set => SetProperty(() => YTJ_XT_JP, value);
        }
        public ObservableCollection<YTJ_XT_Item> YTJ_XT_DGP
        {
            get => GetProperty(() => YTJ_XT_DGP);
            set => SetProperty(() => YTJ_XT_DGP, value);
        }
        public ObservableCollection<YTJ_XT_Item> YTJ_XT_PJ
        {
            get => GetProperty(() => YTJ_XT_PJ);
            set => SetProperty(() => YTJ_XT_PJ, value);
        }

        //导出配置
        [AsyncCommand]
        public void UploadConfigCommand(object obj)
        {
            SaveFileDialog saveFileDialog = new SaveFileDialog();
            saveFileDialog.Filter = "配置文件|*.xml*";
            saveFileDialog.RestoreDirectory = true;
            saveFileDialog.Title = "输出配置文件";   //设置对话框的标题    
            saveFileDialog.DefaultExt = "xml";
            bool? result = saveFileDialog.ShowDialog();
            if (result == true)
            {
                //获得文件路径
                string localFilePath = "";
                localFilePath = saveFileDialog.FileName.ToString();
                System.Xml.Linq.XDocument xdoc = new XDocument();
                XDeclaration xdec = new XDeclaration("1.0", "utf-8", "yes");
                xdoc.Declaration = xdec;
                ObservableCollection<YTJ_XT_Item> YTJ_XT_Temp;
                //添加根节点
                XElement rootEle ;
                switch (int.Parse(YTJ_XT_SF[0].NewData))
                {
                    case 0: { YTJ_XT_Temp = YTJ_XT_JT; rootEle = new XElement("JT"); };break;
                    case 1: { YTJ_XT_Temp = YTJ_XT_JP; rootEle = new XElement("JP"); }; break;
                    case 2: { YTJ_XT_Temp = YTJ_XT_DGP; rootEle = new XElement("DGP"); }; break;
                    case 3: { YTJ_XT_Temp = YTJ_XT_PJ; rootEle = new XElement("PJ"); }; break;
                    default: { YTJ_XT_Temp = YTJ_XT_JT; rootEle = new XElement(""); };break;
                }
                xdoc.Add(rootEle);
                //给根节点添加子节点
                XElement classEle;
                for (int i = 0;i< YTJ_XT_Temp.Count;i++)
                {
                    classEle = new XElement("Para_" + i.ToString(), YTJ_XT_Temp[i].NewData);
                    rootEle.Add(classEle);
                }
                //保存文件
                xdoc.Save(localFilePath);
            }
        }

        //载入配置
        [AsyncCommand]
        public void DownloadConfigCommand(object obj)
        {
            Microsoft.Win32.OpenFileDialog open_file = new Microsoft.Win32.OpenFileDialog();
            open_file.Filter = "配置文件|*.xml*";   //选择文件类型
            open_file.RestoreDirectory = true;   //设置保存路径
            open_file.Title = "选择配置文件";   //设置对话框的标题    
            bool? result = open_file.ShowDialog();
            string fileName = "";
            fileName = open_file.FileName;
            //MessageBox.Show(fileName);
            if (result == true)
            {
                ObservableCollection<YTJ_XT_Item> YTJ_XT_Temp;
                switch (int.Parse(YTJ_XT_SF[0].NewData))
                {
                    case 0: { YTJ_XT_Temp = YTJ_XT_JT;}; break;
                    case 1: { YTJ_XT_Temp = YTJ_XT_JP;}; break;
                    case 2: { YTJ_XT_Temp = YTJ_XT_DGP;}; break;
                    case 3: { YTJ_XT_Temp = YTJ_XT_PJ;}; break;
                    default: { YTJ_XT_Temp = YTJ_XT_JT;}; break;
                }
                XDocument configRe = XDocument.Load(fileName);
                for (int i = 0; i < YTJ_XT_Temp.Count; i++)
                {
                    YTJ_XT_Temp[i].NewData = configRe.Descendants("Para_"+i.ToString()).ElementAt(0).Value;
                }
            }
        }

        //打开截图文件夹
        [AsyncCommand]
        public void OpenPictureFileCommand(object obj)
        {
            System.Diagnostics.Process.Start("explorer.exe", ".\\picture\\" +"Camera0"+(int.Parse(YTJ_XT_SF[10].NewData)+1)+"\\");
        }
        #endregion

        #region Method

        //构造函数
        public YTJ_XT_ConfigModel(string Configfile)
        {
            _Configfile = Configfile;
            Config = XDocument.Load(_Configfile);
        }

        //初始化函数
        public void CSH()
        {
           
            //初始化参集合
            YTJ_XT_SF = new ObservableCollection<YTJ_XT_Item>();
            YTJ_XT_JT = new ObservableCollection<YTJ_XT_Item>();
            YTJ_XT_JP = new ObservableCollection<YTJ_XT_Item>();
            YTJ_XT_DGP = new ObservableCollection<YTJ_XT_Item>();
            YTJ_XT_PJ = new ObservableCollection<YTJ_XT_Item>();

            //吸头编号
            YTJ_XT_ID = new int[8];
            for (int i = 0; i < 8; i++)
            {
                YTJ_XT_ID[i] = i;
            }

            //获取配置项目和初始值
            XElement xroot = Config.Root;
            IEnumerable<XElement> elements = xroot.Elements();
            foreach (XElement item in elements)
            {
                YTJ_XT_CSH_Flag = item.Name.ToString();
                DiGuiNode(item);
                //YTJ_XT_ConfigNames.Add(item.Name.ToString());
                //YTJ_XT_ConfigValue.Add(item.Value.ToString());
            }

            //主界面吸头选项初始化
            YTJ_XTItems = 0;
            Console.WriteLine("参数初始化完成");


        }
        private void DiGuiNode(XElement xroot)
        {
            if (xroot != null)
            {
                foreach (var item in xroot.Elements())
                {
                    switch (YTJ_XT_CSH_Flag)
                    {
                        case "Item_SF": { YTJ_XT_SF.Add(new YTJ_XT_Item() { ID = YTJ_XT_SF.Count, ConfigName = _Configfile, Config = Config, Name = item.Name.ToString(), NewData =item.Value.ToString() }); YTJ_XT_SF[YTJ_XT_SF.Count-1].ChangeDataInvoked += ChangeDataInvoked; };break;
                        case "Item_JT": { YTJ_XT_JT.Add(new YTJ_XT_Item() { ID = YTJ_XT_JT.Count, ConfigName = _Configfile, Config = Config, Name = item.Name.ToString(), NewData = item.Value.ToString() });YTJ_XT_JT[YTJ_XT_JT.Count - 1].ChangeDataInvoked += ChangeDataInvoked; }; break;
                        case "Item_JP": { YTJ_XT_JP.Add(new YTJ_XT_Item() { ID = YTJ_XT_JP.Count, ConfigName = _Configfile, Config = Config, Name = item.Name.ToString(), NewData = item.Value.ToString() }); YTJ_XT_JP[YTJ_XT_JP.Count - 1].ChangeDataInvoked += ChangeDataInvoked; }; break;
                        case "Item_DGP": { YTJ_XT_DGP.Add(new YTJ_XT_Item() { ID = YTJ_XT_DGP.Count, ConfigName = _Configfile, Config = Config, Name = item.Name.ToString(), NewData = item.Value.ToString() }); YTJ_XT_DGP[YTJ_XT_DGP.Count - 1].ChangeDataInvoked += ChangeDataInvoked; }; break;
                        case "Item_PJ": { YTJ_XT_PJ.Add(new YTJ_XT_Item() { ID = YTJ_XT_PJ.Count, ConfigName = _Configfile, Config = Config, Name = item.Name.ToString(), NewData = item.Value.ToString() }); YTJ_XT_PJ[YTJ_XT_PJ.Count - 1].ChangeDataInvoked += ChangeDataInvoked; }; break;
                        default:;break;
                    }
                    DiGuiNode(item);
                }
            }
        }

        //更新数据
        private void ChangeDataInvoked(object sender, YTJ_XT_Item.ChangeData e)
        {
            char[] name = e.ChangeDataName.ToCharArray();
            string Paratype = name[0].ToString() + name[1].ToString();
            int ParaNum = int.Parse(name[8].ToString() + name[9].ToString());
            switch (Paratype)
            {
                case "JT": { JT_Config[ParaNum-1] = e.ChangeDataValue; };break;
                case "JP": { JP_Config[ParaNum-1] = e.ChangeDataValue; }; break;
                case "DP": { DGP_Config[ParaNum-1] = e.ChangeDataValue; }; break;
                case "PJ": { PJ_Config[ParaNum-1] = e.ChangeDataValue; }; break;
                default:;break;
            }          
        }
        #endregion

      
    }
}
