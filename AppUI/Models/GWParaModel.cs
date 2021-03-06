using DevExpress.Mvvm;
using DevExpress.Mvvm.DataAnnotations;
using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Windows;
using System.Xml.Linq;

namespace AppUI.Models
{
    public class GWParaModel : ViewModelBase
    {
        //构造函数传入参数
        public GWParaModel(string Configfile, int Mod_LC, int Mod_GW)
        {
            _Configfile = Configfile;
            config = XDocument.Load(_Configfile);
            _Mod_LC = Mod_LC;
            _Mod_GW = Mod_GW;
            if (Mod_LC.ToString() + Mod_GW.ToString() == Mod_Flags[_Configfile])
            {
                _Mod_Flag = true;
            }
            else { _Mod_Flag = false; }
        }
        //工位Flag
        public Dictionary<string, string> Mod_Flags = new Dictionary<string, string>()
            {
                {"Config_GW01_G2.xml","00" },{"Config_GW02_P1.xml","01" },{"Config_GW03_P3.xml","02" },{"Config_GW04_P4.xml","03" },{"Config_DJ01_G2.xml","10" },{"Config_DJ02_P1.xml","11"},{"Config_DJ03_P4.xml","12" },{"Config_PJ01_G2.xml","20" },{"Config_PJ02_P1.xml","21" },{"Config_PJ03_P4.xml","22" },
                {"Config_SL01_JT.xml","30" },
            };

        //配置文件名称
        public string _Configfile;
        //流程
        public int _Mod_LC;
        public int _Mod_GW;
        public bool _Mod_Flag;
        //权限
        public bool IsDeveloperLogin_Dev
        {
            get => GetProperty(() => IsDeveloperLogin_Dev);
            set => SetProperty(() => IsDeveloperLogin_Dev, value);
        }
        /// <summary>
        /// 尾标
        /// </summary>
        public string CmdTag => DateTime.Now.ToString("yyyy-MM-dd-hh-mm-ss-fff") ;
        //配置修改日志
        public FileStream Para_Log;
        //载入镜筒配置文件
        [AsyncCommand]
        public void LoadConfigCommand(object obj)
        {
            Microsoft.Win32.OpenFileDialog open_file = new OpenFileDialog();
            open_file.Filter = "配置文件|*.xml*";   //选择文件类型
            open_file.RestoreDirectory = true;   //设置保存路径
            open_file.Title = "选择配置文件";   //设置对话框的标题    
            bool? result = open_file.ShowDialog();
            string fileName = "";
            fileName = open_file.FileName;
            //MessageBox.Show(fileName);
            if (result == true)
            {
                XDocument configRe = XDocument.Load(fileName);
                JT_Para_01 = float.Parse(configRe.Descendants("JT_Para_01").ElementAt(0).Value);
                JT_Para_02 = float.Parse(configRe.Descendants("JT_Para_02").ElementAt(0).Value);
                JT_Para_03 = float.Parse(configRe.Descendants("JT_Para_03").ElementAt(0).Value);
                JT_Para_04 = float.Parse(configRe.Descendants("JT_Para_04").ElementAt(0).Value);
                JT_Para_05 = float.Parse(configRe.Descendants("JT_Para_05").ElementAt(0).Value);
                JT_Para_06 = float.Parse(configRe.Descendants("JT_Para_06").ElementAt(0).Value);
                JT_Para_07 = float.Parse(configRe.Descendants("JT_Para_07").ElementAt(0).Value);
                JT_Para_08 = float.Parse(configRe.Descendants("JT_Para_08").ElementAt(0).Value);
                JT_Para_09 = float.Parse(configRe.Descendants("JT_Para_09").ElementAt(0).Value);
                JT_Para_10 = float.Parse(configRe.Descendants("JT_Para_10").ElementAt(0).Value);
                JT_Para_11 = float.Parse(configRe.Descendants("JT_Para_11").ElementAt(0).Value);
                JT_Para_12 = float.Parse(configRe.Descendants("JT_Para_12").ElementAt(0).Value);
                JT_Para_13 = float.Parse(configRe.Descendants("JT_Para_13").ElementAt(0).Value);
                JT_Para_14 = float.Parse(configRe.Descendants("JT_Para_14").ElementAt(0).Value);
                JT_Para_15 = float.Parse(configRe.Descendants("JT_Para_15").ElementAt(0).Value);
                JT_Para_16 = float.Parse(configRe.Descendants("JT_Para_16").ElementAt(0).Value);
                JT_Para_17 = float.Parse(configRe.Descendants("JT_Para_17").ElementAt(0).Value);
                JT_Para_18 = float.Parse(configRe.Descendants("JT_Para_18").ElementAt(0).Value);
                JT_Para_19 = float.Parse(configRe.Descendants("JT_Para_19").ElementAt(0).Value);
                JT_Para_20 = float.Parse(configRe.Descendants("JT_Para_20").ElementAt(0).Value);
                JT_Para_21 = 0;
                JT_Config = (configRe.Descendants("JT_Config").ElementAt(0).Value);
            }
        }
        //载入镜片配置文件
        [AsyncCommand]
        public void LoadConfig_JPCommand(object obj)
        {
            Microsoft.Win32.OpenFileDialog open_file = new OpenFileDialog();
            open_file.Filter = "配置文件|*.xml*";   //选择文件类型
            open_file.RestoreDirectory = true;   //设置保存路径
            open_file.Title = "选择配置文件";   //设置对话框的标题    
            bool? result = open_file.ShowDialog();
            string fileName = "";
            fileName = open_file.FileName;
            //MessageBox.Show(fileName);
            if (result == true)
            {
                XDocument configRe = XDocument.Load(fileName);
                Lens_Para_01 = float.Parse(configRe.Descendants("Lens_Para_01").ElementAt(0).Value);
                Lens_Para_02 = float.Parse(configRe.Descendants("Lens_Para_02").ElementAt(0).Value);
                Lens_Para_03 = float.Parse(configRe.Descendants("Lens_Para_03").ElementAt(0).Value);
                Lens_Para_04 = float.Parse(configRe.Descendants("Lens_Para_04").ElementAt(0).Value);
                Lens_Para_05 = float.Parse(configRe.Descendants("Lens_Para_05").ElementAt(0).Value);
                Lens_Para_06 = float.Parse(configRe.Descendants("Lens_Para_06").ElementAt(0).Value);
                Lens_Para_07 = float.Parse(configRe.Descendants("Lens_Para_07").ElementAt(0).Value);
                Lens_Para_08 = float.Parse(configRe.Descendants("Lens_Para_08").ElementAt(0).Value);
                Lens_Para_09 = float.Parse(configRe.Descendants("Lens_Para_09").ElementAt(0).Value);
                Lens_Para_10 = float.Parse(configRe.Descendants("Lens_Para_10").ElementAt(0).Value);
                Lens_Para_11 = float.Parse(configRe.Descendants("Lens_Para_11").ElementAt(0).Value);
                Lens_Para_12 = float.Parse(configRe.Descendants("Lens_Para_12").ElementAt(0).Value);
                Lens_Para_13 = float.Parse(configRe.Descendants("Lens_Para_13").ElementAt(0).Value);
                Lens_Para_14 = float.Parse(configRe.Descendants("Lens_Para_14").ElementAt(0).Value);
                Lens_Para_15 = float.Parse(configRe.Descendants("Lens_Para_15").ElementAt(0).Value);
                Lens_Para_16 = float.Parse(configRe.Descendants("Lens_Para_16").ElementAt(0).Value);
                Lens_Para_17 = float.Parse(configRe.Descendants("Lens_Para_17").ElementAt(0).Value);
                Lens_Para_18 = float.Parse(configRe.Descendants("Lens_Para_18").ElementAt(0).Value);
                Lens_Para_19 = float.Parse(configRe.Descendants("Lens_Para_19").ElementAt(0).Value);
                Lens_Para_20 = float.Parse(configRe.Descendants("Lens_Para_20").ElementAt(0).Value);
                Lens_Para_21 = 0;
                JP_Config = (configRe.Descendants("JP_Config").ElementAt(0).Value);
            }
        }
        //载入挡光片配置文件
        [AsyncCommand]
        public void LoadConfig_DGPCommand(object obj)
        {
            Microsoft.Win32.OpenFileDialog open_file = new OpenFileDialog();
            open_file.Filter = "配置文件|*.xml*";   //选择文件类型
            open_file.RestoreDirectory = true;   //设置保存路径
            open_file.Title = "选择配置文件";   //设置对话框的标题    
            bool? result = open_file.ShowDialog();
            string fileName = "";
            fileName = open_file.FileName;
            //MessageBox.Show(fileName);
            if (result == true)
            {
                XDocument configRe = XDocument.Load(fileName);
                //挡光片参数
                DGP_threshold = int.Parse(configRe.Descendants("DGP_threshold").ElementAt(0).Value);
                DGP_minRadiius = int.Parse(configRe.Descendants("DGP_minRadiius").ElementAt(0).Value);
                DGP_maxRadiius = int.Parse(configRe.Descendants("DGP_maxRadiius").ElementAt(0).Value);
                DGP_Ana_DefectDetection_2_01 = int.Parse(configRe.Descendants("DGP_Ana_DefectDetection_2_01").ElementAt(0).Value);
                DGP_Ana_DefectDetection_2_02 = int.Parse(configRe.Descendants("DGP_Ana_DefectDetection_2_02").ElementAt(0).Value);
                DGP_radiusThr = int.Parse(configRe.Descendants("DGP_radiusThr").ElementAt(0).Value);
                DGP_resultNum = int.Parse(configRe.Descendants("DGP_resultNum").ElementAt(0).Value);
                DGP_QKLength = int.Parse(configRe.Descendants("DGP_QKLength").ElementAt(0).Value);
                DGP_Config = (configRe.Descendants("DGP_Config").ElementAt(0).Value);
            }
        }
        //载入判胶配置文件
        [AsyncCommand]
        public void LoadConfig_PJCommand(object obj)
        {
            Microsoft.Win32.OpenFileDialog open_file = new OpenFileDialog();
            open_file.Filter = "配置文件|*.xml*";   //选择文件类型
            open_file.RestoreDirectory = true;   //设置保存路径
            open_file.Title = "选择配置文件";   //设置对话框的标题    
            bool? result = open_file.ShowDialog();
            string fileName = "";
            fileName = open_file.FileName;
            //MessageBox.Show(fileName);
            if (result == true)
            {
                XDocument configRe = XDocument.Load(fileName);
                PJ_Para_01 = float.Parse(config.Descendants("PJ_Para_01").ElementAt(0).Value);
                PJ_Para_02 = float.Parse(config.Descendants("PJ_Para_02").ElementAt(0).Value);
                PJ_Para_03 = float.Parse(config.Descendants("PJ_Para_03").ElementAt(0).Value);
                PJ_Para_04 = float.Parse(config.Descendants("PJ_Para_04").ElementAt(0).Value);
                PJ_Para_05 = float.Parse(config.Descendants("PJ_Para_05").ElementAt(0).Value);
                PJ_Para_06 = float.Parse(config.Descendants("PJ_Para_06").ElementAt(0).Value);
                PJ_Para_07 = float.Parse(config.Descendants("PJ_Para_07").ElementAt(0).Value);
                PJ_Para_08 = float.Parse(config.Descendants("PJ_Para_08").ElementAt(0).Value);
                PJ_Para_09 = float.Parse(config.Descendants("PJ_Para_09").ElementAt(0).Value);
                PJ_Para_10 = float.Parse(config.Descendants("PJ_Para_10").ElementAt(0).Value);
                PJ_Para_11 = float.Parse(config.Descendants("PJ_Para_11").ElementAt(0).Value);
                PJ_Para_12 = float.Parse(config.Descendants("PJ_Para_12").ElementAt(0).Value);
                PJ_Para_13 = float.Parse(config.Descendants("PJ_Para_13").ElementAt(0).Value);
                PJ_Para_14 = float.Parse(config.Descendants("PJ_Para_14").ElementAt(0).Value);
                PJ_Para_15 = float.Parse(config.Descendants("PJ_Para_15").ElementAt(0).Value);
                PJ_Para_16 = float.Parse(config.Descendants("PJ_Para_16").ElementAt(0).Value);
                PJ_Para_17 = float.Parse(config.Descendants("PJ_Para_17").ElementAt(0).Value);
                PJ_Para_18 = float.Parse(config.Descendants("PJ_Para_18").ElementAt(0).Value);
                PJ_Para_19 = float.Parse(config.Descendants("PJ_Para_19").ElementAt(0).Value);
                PJ_Para_20 = float.Parse(config.Descendants("PJ_Para_20").ElementAt(0).Value);
                PJ_Para_21 = float.Parse(config.Descendants("PJ_Para_21").ElementAt(0).Value);
                PJ_Para_22 = float.Parse(config.Descendants("PJ_Para_22").ElementAt(0).Value);
                PJ_Para_23 = float.Parse(config.Descendants("PJ_Para_23").ElementAt(0).Value);
                PJ_Para_24 = float.Parse(config.Descendants("PJ_Para_24").ElementAt(0).Value);
                PJ_Para_25 = float.Parse(config.Descendants("PJ_Para_25").ElementAt(0).Value);
                PJ_Para_26 = float.Parse(config.Descendants("PJ_Para_26").ElementAt(0).Value);
                PJ_Para_27 = float.Parse(config.Descendants("PJ_Para_27").ElementAt(0).Value);
                PJ_Para_28 = float.Parse(config.Descendants("PJ_Para_28").ElementAt(0).Value);
                PJ_Para_29 = float.Parse(config.Descendants("PJ_Para_29").ElementAt(0).Value);
                PJ_Para_30 = 0;
                PJ_Config = (configRe.Descendants("PJ_Config").ElementAt(0).Value);
            }
        }

        //输出镜筒配置文件
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

                //添加根节点
                XElement rootEle = new XElement("StdValue");
                xdoc.Add(rootEle);

                //给根节点添加子节点
                XElement classEle = new XElement("JT_Para_01", JT_Para_01.ToString());
                //XAttribute attrClass = new XAttribute("No", 1);
                //classEle.Add(attrClass);
                rootEle.Add(classEle);

                //添加子节点下的元素
                //XElement stuEle = new XElement("student");
                //XAttribute atrStu = new XAttribute("JT_Para_01", JT_Para_01.ToString());
                //rootEle.Add(atrStu);

                //classEle.Add(stuEle);
                classEle = new XElement("JT_Para_02", JT_Para_02.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("JT_Para_03", JT_Para_03.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("JT_Para_04", JT_Para_04.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("JT_Para_05", JT_Para_05.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("JT_Para_06", JT_Para_06.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("JT_Para_07", JT_Para_07.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("JT_Para_08", JT_Para_08.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("JT_Para_09", JT_Para_09.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("JT_Para_10", JT_Para_10.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("JT_Para_11", JT_Para_11.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("JT_Para_12", JT_Para_12.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("JT_Para_13", JT_Para_13.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("JT_Para_14", JT_Para_14.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("JT_Para_15", JT_Para_15.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("JT_Para_16", JT_Para_16.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("JT_Para_17", JT_Para_17.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("JT_Para_18", JT_Para_18.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("JT_Para_19", JT_Para_19.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("JT_Para_20", JT_Para_20.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("JT_Para_21", JT_Para_21.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("JT_Config", JT_Config.ToString());
                rootEle.Add(classEle);
                //保存文件
                xdoc.Save(localFilePath);
          
            }

        }
        [AsyncCommand]
        public void UploadConfig_JPCommand(object obj)
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

                //添加根节点
                XElement rootEle = new XElement("StdValue");
                xdoc.Add(rootEle);

                //给根节点添加子节点
                XElement classEle = new XElement("Lens_Para_01", Lens_Para_01.ToString());
                //XAttribute attrClass = new XAttribute("No", 1);
                //classEle.Add(attrClass);
                rootEle.Add(classEle);

                //添加子节点下的元素
                //XElement stuEle = new XElement("student");
                //XAttribute atrStu = new XAttribute("Lens_Para_01", Lens_Para_01.ToString());
                //rootEle.Add(atrStu);

                //classEle.Add(stuEle);
                classEle = new XElement("Lens_Para_02", Lens_Para_02.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("Lens_Para_03", Lens_Para_03.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("Lens_Para_04", Lens_Para_04.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("Lens_Para_05", Lens_Para_05.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("Lens_Para_06", Lens_Para_06.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("Lens_Para_07", Lens_Para_07.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("Lens_Para_08", Lens_Para_08.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("Lens_Para_09", Lens_Para_09.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("Lens_Para_10", Lens_Para_10.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("Lens_Para_11", Lens_Para_11.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("Lens_Para_12", Lens_Para_12.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("Lens_Para_13", Lens_Para_13.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("Lens_Para_14", Lens_Para_14.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("Lens_Para_15", Lens_Para_15.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("Lens_Para_16", Lens_Para_16.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("Lens_Para_17", Lens_Para_17.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("Lens_Para_18", Lens_Para_18.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("Lens_Para_19", Lens_Para_19.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("Lens_Para_20", Lens_Para_20.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("Lens_Para_21", Lens_Para_21.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("JP_Config", JP_Config.ToString());
                rootEle.Add(classEle);
                //保存文件
                xdoc.Save(localFilePath);

            }

        }
        [AsyncCommand]
        public void UploadConfig_DGPCommand(object obj)
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

                //添加根节点
                XElement rootEle = new XElement("StdValue");
                xdoc.Add(rootEle);

                //给根节点添加子节点
                XElement classEle = new XElement("DGP_threshold", DGP_threshold.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("DGP_minRadiius", DGP_minRadiius.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("DGP_maxRadiius", DGP_maxRadiius.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("DGP_Ana_DefectDetection_2_01", DGP_Ana_DefectDetection_2_01.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("DGP_Ana_DefectDetection_2_02", DGP_Ana_DefectDetection_2_02.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("DGP_radiusThr", DGP_radiusThr.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("DGP_resultNum", DGP_resultNum.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("DGP_QKLength", DGP_QKLength.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("DGP_Config", DGP_Config.ToString());
                rootEle.Add(classEle);
                //保存文件
                xdoc.Save(localFilePath);

            }

        }
        [AsyncCommand]
        public void UploadConfig_PJCommand(object obj)
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

                //添加根节点
                XElement rootEle = new XElement("StdValue");
                xdoc.Add(rootEle);

                //给根节点添加子节点
                XElement classEle = new XElement("PJ_Para_01", PJ_Para_01.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("PJ_Para_02", PJ_Para_02.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("PJ_Para_03", PJ_Para_03.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("PJ_Para_04", PJ_Para_04.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("PJ_Para_05", PJ_Para_05.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("PJ_Para_06", PJ_Para_06.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("PJ_Para_07", PJ_Para_07.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("PJ_Para_08", PJ_Para_08.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("PJ_Para_09", PJ_Para_09.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("PJ_Para_10", PJ_Para_10.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("PJ_Para_11", PJ_Para_11.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("PJ_Para_12", PJ_Para_12.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("PJ_Para_13", PJ_Para_13.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("PJ_Para_14", PJ_Para_14.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("PJ_Para_15", PJ_Para_15.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("PJ_Para_16", PJ_Para_16.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("PJ_Para_17", PJ_Para_17.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("PJ_Para_18", PJ_Para_18.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("PJ_Para_19", PJ_Para_19.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("PJ_Para_20", PJ_Para_20.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("PJ_Para_21", PJ_Para_21.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("PJ_Para_22", PJ_Para_22.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("PJ_Para_23", PJ_Para_23.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("PJ_Para_24", PJ_Para_24.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("PJ_Para_25", PJ_Para_25.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("PJ_Para_26", PJ_Para_26.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("PJ_Para_27", PJ_Para_27.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("PJ_Para_28", PJ_Para_28.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("PJ_Para_29", PJ_Para_29.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("PJ_Para_30", PJ_Para_30.ToString());
                rootEle.Add(classEle);
                classEle = new XElement("PJ_Config", PJ_Config.ToString());
                rootEle.Add(classEle);
                //保存文件
                xdoc.Save(localFilePath);

            }

        }

        //算法输入参数数组
        public int[] _input_Parameter_JT = new int[20];
        public int[] _input_Parameter_Lens = new int[20];
        public float[] _input_Parameter_DGP = new float[20];

        public float[] _input_Parameter_JT_New = new float[30];
        public float[] _input_Parameter_Lens_New = new float[30];
        public float[] _input_Parameter_DJ_New = new float[30];
        public float[] _input_Parameter_PJ_New = new float[40];
        public float[] _input_Parameter_TP_New = new float[30];
        /// <summary>
        /// 配置文件读写器
        /// </summary>
        public XDocument config;

        //初始化
        public void CSH()
        {
            //配置文件
            if (_Mod_Flag == true)
            { Para_Log = new FileStream(".\\Log\\" + _Configfile + CmdTag.ToString() + ".txt", FileMode.Create); }

            //镜筒类型
            config = XDocument.Load(_Configfile);
            _input_Parameter_JT[17] = int.Parse(config.Descendants("JT_Mod").ElementAt(0).Value);

            //镜筒相机参数
            JT_CameraPara_ExposureTime = float.Parse(config.Descendants("JT_CameraPara_ExposureTime").ElementAt(0).Value);
            JT_CameraPara_AcquisitionFrameRate = float.Parse(config.Descendants("JT_CameraPara_AcquisitionFrameRate").ElementAt(0).Value);
            JT_CameraPara_Gain = float.Parse(config.Descendants("JT_CameraPara_Gain").ElementAt(0).Value);

            //镜筒相机参数
            Lens_CameraPara_ExposureTime = float.Parse(config.Descendants("Lens_CameraPara_ExposureTime").ElementAt(0).Value);
            Lens_CameraPara_AcquisitionFrameRate = float.Parse(config.Descendants("Lens_CameraPara_AcquisitionFrameRate").ElementAt(0).Value);
            Lens_CameraPara_Gain = float.Parse(config.Descendants("Lens_CameraPara_Gain").ElementAt(0).Value);

            //挡光片相机参数
            DGP_CameraPara_ExposureTime = float.Parse(config.Descendants("DGP_CameraPara_ExposureTime").ElementAt(0).Value);
            DGP_CameraPara_AcquisitionFrameRate = float.Parse(config.Descendants("DGP_CameraPara_AcquisitionFrameRate").ElementAt(0).Value);
            DGP_CameraPara_Gain = float.Parse(config.Descendants("DGP_CameraPara_Gain").ElementAt(0).Value);

            //托盘相机参数
            TP_CameraPara_ExposureTime = float.Parse(config.Descendants("TP_CameraPara_ExposureTime").ElementAt(0).Value);
            TP_CameraPara_AcquisitionFrameRate = float.Parse(config.Descendants("TP_CameraPara_AcquisitionFrameRate").ElementAt(0).Value);
            TP_CameraPara_Gain = float.Parse(config.Descendants("TP_CameraPara_Gain").ElementAt(0).Value);

            //TCP参数设置
            JT_TCP_Port = int.Parse(config.Descendants("JT_TCP_Port").ElementAt(0).Value);
            JT_TCP_IP = (config.Descendants("JT_TCP_IP").ElementAt(0).Value).ToString();
            Lens_TCP_Port = int.Parse(config.Descendants("Lens_TCP_Port").ElementAt(0).Value);
            Lens_TCP_IP = (config.Descendants("Lens_TCP_IP").ElementAt(0).Value).ToString();
            DGP_TCP_Port = int.Parse(config.Descendants("DGP_TCP_Port").ElementAt(0).Value);
            DGP_TCP_IP = (config.Descendants("DGP_TCP_IP").ElementAt(0).Value).ToString();
            Clent_TCP_Port = int.Parse(config.Descendants("Clent_TCP_Port").ElementAt(0).Value);
            Clent_TCP_IP = (config.Descendants("Clent_TCP_IP").ElementAt(0).Value).ToString();
            Server_TCP_Port = int.Parse(config.Descendants("Server_TCP_Port").ElementAt(0).Value);
            Server_TCP_IP = (config.Descendants("Server_TCP_IP").ElementAt(0).Value).ToString();
            Heart_TCP_Port = int.Parse(config.Descendants("Heart_TCP_Port").ElementAt(0).Value);
            Heart_TCP_IP = (config.Descendants("Heart_TCP_IP").ElementAt(0).Value).ToString();
            SendClient_TCP_Port = int.Parse(config.Descendants("SendClient_TCP_Port").ElementAt(0).Value);
            SendClient_TCP_IP = (config.Descendants("SendClient_TCP_IP").ElementAt(0).Value).ToString();
            EndClient_TCP_Port = int.Parse(config.Descendants("EndClient_TCP_Port").ElementAt(0).Value);
            EndClient_TCP_IP = (config.Descendants("EndClient_TCP_IP").ElementAt(0).Value).ToString();

            //相机选择Flag
            JT_Camera = int.Parse(config.Descendants("JT_Camera").ElementAt(0).Value);
            Lens_Camera = int.Parse(config.Descendants("Lens_Camera").ElementAt(0).Value);
            DGP_Camera = int.Parse(config.Descendants("DGP_Camera").ElementAt(0).Value);

            //镜筒参数_New
            JT_Para_01 = float.Parse(config.Descendants("JT_Para_01").ElementAt(0).Value);
            JT_Para_02 = float.Parse(config.Descendants("JT_Para_02").ElementAt(0).Value);
            JT_Para_03 = float.Parse(config.Descendants("JT_Para_03").ElementAt(0).Value);
            JT_Para_04 = float.Parse(config.Descendants("JT_Para_04").ElementAt(0).Value);
            JT_Para_05 = float.Parse(config.Descendants("JT_Para_05").ElementAt(0).Value);
            JT_Para_06 = float.Parse(config.Descendants("JT_Para_06").ElementAt(0).Value);
            JT_Para_07 = float.Parse(config.Descendants("JT_Para_07").ElementAt(0).Value);
            JT_Para_08 = float.Parse(config.Descendants("JT_Para_08").ElementAt(0).Value);
            JT_Para_09 = float.Parse(config.Descendants("JT_Para_09").ElementAt(0).Value);
            JT_Para_10 = float.Parse(config.Descendants("JT_Para_10").ElementAt(0).Value);
            JT_Para_11 = float.Parse(config.Descendants("JT_Para_11").ElementAt(0).Value);
            JT_Para_12 = float.Parse(config.Descendants("JT_Para_12").ElementAt(0).Value);
            JT_Para_13 = float.Parse(config.Descendants("JT_Para_13").ElementAt(0).Value);
            JT_Para_14 = float.Parse(config.Descendants("JT_Para_14").ElementAt(0).Value);
            JT_Para_15 = float.Parse(config.Descendants("JT_Para_15").ElementAt(0).Value);
            JT_Para_16 = float.Parse(config.Descendants("JT_Para_16").ElementAt(0).Value);
            JT_Para_17 = float.Parse(config.Descendants("JT_Para_17").ElementAt(0).Value);
            JT_Para_18 = float.Parse(config.Descendants("JT_Para_18").ElementAt(0).Value);
            JT_Para_19 = float.Parse(config.Descendants("JT_Para_19").ElementAt(0).Value);
            JT_Para_20 = float.Parse(config.Descendants("JT_Para_20").ElementAt(0).Value);
            JT_Para_21 = 0;

            //镜片参数_New
            Lens_Para_01 = float.Parse(config.Descendants("Lens_Para_01").ElementAt(0).Value);
            Lens_Para_02 = float.Parse(config.Descendants("Lens_Para_02").ElementAt(0).Value);
            Lens_Para_03 = float.Parse(config.Descendants("Lens_Para_03").ElementAt(0).Value);
            Lens_Para_04 = float.Parse(config.Descendants("Lens_Para_04").ElementAt(0).Value);
            Lens_Para_05 = float.Parse(config.Descendants("Lens_Para_05").ElementAt(0).Value);
            Lens_Para_06 = float.Parse(config.Descendants("Lens_Para_06").ElementAt(0).Value);
            Lens_Para_07 = float.Parse(config.Descendants("Lens_Para_07").ElementAt(0).Value);
            Lens_Para_08 = float.Parse(config.Descendants("Lens_Para_08").ElementAt(0).Value);
            Lens_Para_09 = float.Parse(config.Descendants("Lens_Para_09").ElementAt(0).Value);
            Lens_Para_10 = float.Parse(config.Descendants("Lens_Para_10").ElementAt(0).Value);
            Lens_Para_11 = float.Parse(config.Descendants("Lens_Para_11").ElementAt(0).Value);
            Lens_Para_12 = float.Parse(config.Descendants("Lens_Para_12").ElementAt(0).Value);
            Lens_Para_13 = float.Parse(config.Descendants("Lens_Para_13").ElementAt(0).Value);
            Lens_Para_14 = float.Parse(config.Descendants("Lens_Para_14").ElementAt(0).Value);
            Lens_Para_15 = float.Parse(config.Descendants("Lens_Para_15").ElementAt(0).Value);
            Lens_Para_16 = float.Parse(config.Descendants("Lens_Para_16").ElementAt(0).Value);
            Lens_Para_17 = float.Parse(config.Descendants("Lens_Para_17").ElementAt(0).Value);
            Lens_Para_18 = float.Parse(config.Descendants("Lens_Para_18").ElementAt(0).Value);
            Lens_Para_19 = float.Parse(config.Descendants("Lens_Para_19").ElementAt(0).Value);
            Lens_Para_20 = float.Parse(config.Descendants("Lens_Para_20").ElementAt(0).Value);
            Lens_Para_21 = 0;

            //挡光片参数
            DGP_threshold = int.Parse(config.Descendants("DGP_threshold").ElementAt(0).Value);
            DGP_minRadiius = int.Parse(config.Descendants("DGP_minRadiius").ElementAt(0).Value);
            DGP_maxRadiius = int.Parse(config.Descendants("DGP_maxRadiius").ElementAt(0).Value);
            DGP_Ana_DefectDetection_2_01 = int.Parse(config.Descendants("DGP_Ana_DefectDetection_2_01").ElementAt(0).Value);
            DGP_Ana_DefectDetection_2_02 = int.Parse(config.Descendants("DGP_Ana_DefectDetection_2_02").ElementAt(0).Value);
            DGP_radiusThr = int.Parse(config.Descendants("DGP_radiusThr").ElementAt(0).Value);
            DGP_resultNum = int.Parse(config.Descendants("DGP_resultNum").ElementAt(0).Value);
            DGP_QKLength = int.Parse(config.Descendants("DGP_QKLength").ElementAt(0).Value);

            //判胶参数_New
            PJ_Para_01 = float.Parse(config.Descendants("PJ_Para_01").ElementAt(0).Value);
            PJ_Para_02 = float.Parse(config.Descendants("PJ_Para_02").ElementAt(0).Value);
            PJ_Para_03 = float.Parse(config.Descendants("PJ_Para_03").ElementAt(0).Value);
            PJ_Para_04 = float.Parse(config.Descendants("PJ_Para_04").ElementAt(0).Value);
            PJ_Para_05 = float.Parse(config.Descendants("PJ_Para_05").ElementAt(0).Value);
            PJ_Para_06 = float.Parse(config.Descendants("PJ_Para_06").ElementAt(0).Value);
            PJ_Para_07 = float.Parse(config.Descendants("PJ_Para_07").ElementAt(0).Value);
            PJ_Para_08 = float.Parse(config.Descendants("PJ_Para_08").ElementAt(0).Value);
            PJ_Para_09 = float.Parse(config.Descendants("PJ_Para_09").ElementAt(0).Value);
            PJ_Para_10 = float.Parse(config.Descendants("PJ_Para_10").ElementAt(0).Value);
            PJ_Para_11 = float.Parse(config.Descendants("PJ_Para_11").ElementAt(0).Value);
            PJ_Para_12 = float.Parse(config.Descendants("PJ_Para_12").ElementAt(0).Value);
            PJ_Para_13 = float.Parse(config.Descendants("PJ_Para_13").ElementAt(0).Value);
            PJ_Para_14 = float.Parse(config.Descendants("PJ_Para_14").ElementAt(0).Value);
            PJ_Para_15 = float.Parse(config.Descendants("PJ_Para_15").ElementAt(0).Value);
            PJ_Para_16 = float.Parse(config.Descendants("PJ_Para_16").ElementAt(0).Value);
            PJ_Para_17 = float.Parse(config.Descendants("PJ_Para_17").ElementAt(0).Value);
            PJ_Para_18 = float.Parse(config.Descendants("PJ_Para_18").ElementAt(0).Value);
            PJ_Para_19 = float.Parse(config.Descendants("PJ_Para_19").ElementAt(0).Value);
            PJ_Para_20 = float.Parse(config.Descendants("PJ_Para_20").ElementAt(0).Value);
            PJ_Para_21 = float.Parse(config.Descendants("PJ_Para_21").ElementAt(0).Value);
            PJ_Para_22 = float.Parse(config.Descendants("PJ_Para_22").ElementAt(0).Value);
            PJ_Para_23 = float.Parse(config.Descendants("PJ_Para_23").ElementAt(0).Value);
            PJ_Para_24 = float.Parse(config.Descendants("PJ_Para_24").ElementAt(0).Value);
            PJ_Para_25 = float.Parse(config.Descendants("PJ_Para_25").ElementAt(0).Value);
            PJ_Para_26 = float.Parse(config.Descendants("PJ_Para_26").ElementAt(0).Value);
            PJ_Para_27 = float.Parse(config.Descendants("PJ_Para_27").ElementAt(0).Value);
            PJ_Para_28 = float.Parse(config.Descendants("PJ_Para_28").ElementAt(0).Value);
            PJ_Para_29 = float.Parse(config.Descendants("PJ_Para_29").ElementAt(0).Value);
            PJ_Para_30 = 0;

            //比例尺
            Rule_DJ_X = float.Parse(config.Descendants("Rule_DJ_x").ElementAt(0).Value);
            Rule_DJ_Y = float.Parse(config.Descendants("Rule_DJ_y").ElementAt(0).Value);
            Rule_JT_X = float.Parse(config.Descendants("Rule_JT_x").ElementAt(0).Value);
            Rule_JT_Y = float.Parse(config.Descendants("Rule_JT_y").ElementAt(0).Value);
            Rule_Lens_X = float.Parse(config.Descendants("Rule_Lens_x").ElementAt(0).Value); 
            Rule_Lens_Y = float.Parse(config.Descendants("Rule_Lens_y").ElementAt(0).Value);

            //光源触发
            JT_Light_Flag = int.Parse(config.Descendants("JT_Light_Flag").ElementAt(0).Value);
            Lens_Light_Flag = int.Parse(config.Descendants("Lens_Light_Flag").ElementAt(0).Value);
            DGP_Light_Flag = int.Parse(config.Descendants("DGP_Light_Flag").ElementAt(0).Value);

            JT_Light_Brightness = float.Parse(config.Descendants("JT_Light_Brightness").ElementAt(0).Value);
            Lens_Light_Brightness = float.Parse(config.Descendants("Lens_Light_Brightness").ElementAt(0).Value);
            DGP_Light_Brightness = float.Parse(config.Descendants("DGP_Light_Brightness").ElementAt(0).Value);

            JT_Light_Time = float.Parse(config.Descendants("JT_Light_Time").ElementAt(0).Value);
            Lens_Light_Time = float.Parse(config.Descendants("Lens_Light_Time").ElementAt(0).Value);
            DGP_Light_Time = float.Parse(config.Descendants("DGP_Light_Time").ElementAt(0).Value);

            Light_TCP_IP = config.Descendants("Light_TCP_IP").ElementAt(0).Value;
            Light_TCP_Port = int.Parse(config.Descendants("Light_TCP_Port").ElementAt(0).Value);

            //配置文件编号
            JT_Config = config.Descendants("JT_Config").ElementAt(0).Value;
            JP_Config = config.Descendants("JP_Config").ElementAt(0).Value;
            DGP_Config = config.Descendants("DGP_Config").ElementAt(0).Value;
            PJ_Config = config.Descendants("PJ_Config").ElementAt(0).Value;

            //托盘
            TP_Para_01 = float.Parse(config.Descendants("TP_Para_01").ElementAt(0).Value);
            TP_Para_02 = float.Parse(config.Descendants("TP_Para_02").ElementAt(0).Value);
            TP_Para_03 = float.Parse(config.Descendants("TP_Para_03").ElementAt(0).Value);
            TP_Para_04 = float.Parse(config.Descendants("TP_Para_04").ElementAt(0).Value);
            TP_Para_05 = float.Parse(config.Descendants("TP_Para_05").ElementAt(0).Value);
            TP_Para_06 = float.Parse(config.Descendants("TP_Para_06").ElementAt(0).Value);
            TP_Para_07 = float.Parse(config.Descendants("TP_Para_07").ElementAt(0).Value);
            TP_Para_08 = float.Parse(config.Descendants("TP_Para_08").ElementAt(0).Value);
            TP_Para_09 = float.Parse(config.Descendants("TP_Para_09").ElementAt(0).Value);
            TP_Para_10 = 0;

           
        }
        /// <summary>
        /// 测试
        /// </summary>
        public int test
        {
            get => GetProperty(() => test);
            set => SetProperty(() => test, value, () =>
            {
                config.Descendants("test").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_JT[0] = value;
            });
        }

      

        /// <summary>
        /// 镜筒相机参数
        /// </summary>
        public float JT_CameraPara_ExposureTime
        {
            get => GetProperty(() => JT_CameraPara_ExposureTime);
            set => SetProperty(() => JT_CameraPara_ExposureTime, value, () =>
            {
                config.Descendants("JT_CameraPara_ExposureTime").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜筒相机曝光时间：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }

        public float JT_CameraPara_AcquisitionFrameRate
        {
            get => GetProperty(() => JT_CameraPara_AcquisitionFrameRate);
            set => SetProperty(() => JT_CameraPara_AcquisitionFrameRate, value, () =>
            {
                config.Descendants("JT_CameraPara_AcquisitionFrameRate").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜筒相机帧数：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }

        public float JT_CameraPara_Gain
        {
            get => GetProperty(() => JT_CameraPara_Gain);
            set => SetProperty(() => JT_CameraPara_Gain, value, () =>
            {
                config.Descendants("JT_CameraPara_Gain").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜筒相机增益：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }

      

        /// <summary>
        /// 挡光片检测参数
        /// </summary>
        public int DGP_threshold
        {
            get => GetProperty(() => DGP_threshold);
            set => SetProperty(() => DGP_threshold, value, () =>
            {
                config.Descendants("DGP_threshold").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_DGP[0] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改挡光片配置-最大半径：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public int DGP_minRadiius
        {
            get => GetProperty(() => DGP_minRadiius);
            set => SetProperty(() => DGP_minRadiius, value, () =>
            {
                config.Descendants("DGP_minRadiius").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_DGP[1] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改挡光片配置-最小半径：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public int DGP_maxRadiius
        {
            get => GetProperty(() => DGP_maxRadiius);
            set => SetProperty(() => DGP_maxRadiius, value, () =>
            {
                config.Descendants("DGP_maxRadiius").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_DGP[2] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改挡光片配置-灰度阈值：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public int DGP_Ana_DefectDetection_2_01
        {
            get => GetProperty(() => DGP_Ana_DefectDetection_2_01);
            set => SetProperty(() => DGP_Ana_DefectDetection_2_01, value, () =>
            {
                config.Descendants("DGP_Ana_DefectDetection_2_01").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_DGP[3] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改挡光片配置-缺口-外圈半径：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public int DGP_Ana_DefectDetection_2_02
        {
            get => GetProperty(() => DGP_Ana_DefectDetection_2_02);
            set => SetProperty(() => DGP_Ana_DefectDetection_2_02, value, () =>
            {
                config.Descendants("DGP_Ana_DefectDetection_2_02").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_DGP[4] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改挡光片配置-缺口-内圈半径：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }

        public int DGP_radiusThr
        {
            get => GetProperty(() => DGP_radiusThr);
            set => SetProperty(() => DGP_radiusThr, value, () =>
            {
                config.Descendants("DGP_radiusThr").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_DGP[5] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改挡光片配置-缺口-灰度阈值：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }

        public int DGP_resultNum
        {
            get => GetProperty(() => DGP_resultNum);
            set => SetProperty(() => DGP_resultNum, value, () =>
            {
                config.Descendants("DGP_resultNum").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_DGP[6] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改挡光片配置-缺口数量：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }

        public int DGP_QKLength
        {
            get => GetProperty(() => DGP_QKLength);
            set => SetProperty(() => DGP_QKLength, value, () =>
            {
                config.Descendants("DGP_QKLength").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_DGP[7] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改挡光片配置-缺口长度：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        /// <summary>
        /// 镜片相机参数
        /// </summary>
        public float Lens_CameraPara_ExposureTime
        {
            get => GetProperty(() => Lens_CameraPara_ExposureTime);
            set => SetProperty(() => Lens_CameraPara_ExposureTime, value, () =>
            {
                config.Descendants("Lens_CameraPara_ExposureTime").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜片相机曝光时间：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float Lens_CameraPara_AcquisitionFrameRate
        {
            get => GetProperty(() => Lens_CameraPara_AcquisitionFrameRate);
            set => SetProperty(() => Lens_CameraPara_AcquisitionFrameRate, value, () =>
            {
                config.Descendants("Lens_CameraPara_AcquisitionFrameRate").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜片相机帧数：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float Lens_CameraPara_Gain
        {
            get => GetProperty(() => Lens_CameraPara_Gain);
            set => SetProperty(() => Lens_CameraPara_Gain, value, () =>
            {
                config.Descendants("Lens_CameraPara_Gain").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜片相机增益：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }

        /// <summary>
        /// 挡光片相机参数
        /// </summary>
        public float DGP_CameraPara_ExposureTime
        {
            get => GetProperty(() => DGP_CameraPara_ExposureTime);
            set => SetProperty(() => DGP_CameraPara_ExposureTime, value, () =>
            {
                config.Descendants("DGP_CameraPara_ExposureTime").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改挡光片相机曝光时间：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }

        public float DGP_CameraPara_AcquisitionFrameRate
        {
            get => GetProperty(() => DGP_CameraPara_AcquisitionFrameRate);
            set => SetProperty(() => DGP_CameraPara_AcquisitionFrameRate, value, () =>
            {
                config.Descendants("DGP_CameraPara_AcquisitionFrameRate").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改挡光片相机帧数：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }

        public float DGP_CameraPara_Gain
        {
            get => GetProperty(() => DGP_CameraPara_Gain);
            set => SetProperty(() => DGP_CameraPara_Gain, value, () =>
            {
                config.Descendants("DGP_CameraPara_Gain").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改挡光片相机增益：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }

        /// <summary>
        /// TCP连接参数
        /// </summary>
        public int JT_TCP_Port
        {
            get => GetProperty(() => JT_TCP_Port);
            set => SetProperty(() => JT_TCP_Port, value, () =>
            {
                config.Descendants("JT_TCP_Port").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
            });
        }
        public string JT_TCP_IP
        {
            get => GetProperty(() => JT_TCP_IP);
            set => SetProperty(() => JT_TCP_IP, value, () =>
            {
                config.Descendants("JT_TCP_IP").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
            });
        }
        public int Lens_TCP_Port
        {
            get => GetProperty(() => Lens_TCP_Port);
            set => SetProperty(() => Lens_TCP_Port, value, () =>
            {
                config.Descendants("Lens_TCP_Port").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
            });
        }
        public string Lens_TCP_IP
        {
            get => GetProperty(() => Lens_TCP_IP);
            set => SetProperty(() => Lens_TCP_IP, value, () =>
            {
                config.Descendants("Lens_TCP_IP").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
            });
        }
        public int DGP_TCP_Port
        {
            get => GetProperty(() => DGP_TCP_Port);
            set => SetProperty(() => DGP_TCP_Port, value, () =>
            {
                config.Descendants("DGP_TCP_Port").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
            });
        }
        public string DGP_TCP_IP
        {
            get => GetProperty(() => DGP_TCP_IP);
            set => SetProperty(() => DGP_TCP_IP, value, () =>
            {
                config.Descendants("DGP_TCP_IP").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
            });
        }
        public int Clent_TCP_Port
        {
            get => GetProperty(() => Clent_TCP_Port);
            set => SetProperty(() => Clent_TCP_Port, value, () =>
            {
                config.Descendants("Clent_TCP_Port").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
            });
        }
        public string Clent_TCP_IP
        {
            get => GetProperty(() => Clent_TCP_IP);
            set => SetProperty(() => Clent_TCP_IP, value, () =>
            {
                config.Descendants("Clent_TCP_IP").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
            });
        }
        public int Server_TCP_Port
        {
            get => GetProperty(() => Server_TCP_Port);
            set => SetProperty(() => Server_TCP_Port, value, () =>
            {
                config.Descendants("Server_TCP_Port").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
            });
        }
        public string Server_TCP_IP
        {
            get => GetProperty(() => Server_TCP_IP);
            set => SetProperty(() => Server_TCP_IP, value, () =>
            {
                config.Descendants("Server_TCP_IP").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
            });
        }
        public int Heart_TCP_Port
        {
            get => GetProperty(() => Heart_TCP_Port);
            set => SetProperty(() => Heart_TCP_Port, value, () =>
            {
                config.Descendants("Heart_TCP_Port").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
            });
        }
        public string Heart_TCP_IP
        {
            get => GetProperty(() => Heart_TCP_IP);
            set => SetProperty(() => Heart_TCP_IP, value, () =>
            {
                config.Descendants("Heart_TCP_IP").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
            });
        }
        public int SendClient_TCP_Port
        {
            get => GetProperty(() => SendClient_TCP_Port);
            set => SetProperty(() => SendClient_TCP_Port, value, () =>
            {
                config.Descendants("SendClient_TCP_Port").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
            });
        }
        public string SendClient_TCP_IP
        {
            get => GetProperty(() => SendClient_TCP_IP);
            set => SetProperty(() => SendClient_TCP_IP, value, () =>
            {
                config.Descendants("SendClient_TCP_IP").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
            });
        }
        public int EndClient_TCP_Port
        {
            get => GetProperty(() => EndClient_TCP_Port);
            set => SetProperty(() => EndClient_TCP_Port, value, () =>
            {
                config.Descendants("EndClient_TCP_Port").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
            });
        }
        public string EndClient_TCP_IP
        {
            get => GetProperty(() => EndClient_TCP_IP);
            set => SetProperty(() => EndClient_TCP_IP, value, () =>
            {
                config.Descendants("EndClient_TCP_IP").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
            });
        }
        /// <summary>
        /// 相机连接flag
        /// </summary>
        public int JT_Camera
        {
            get => GetProperty(() => JT_Camera);
            set => SetProperty(() => JT_Camera, value, () =>
            {
                config.Descendants("JT_Camera").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜筒相机连接：" + value.ToString() + "\n");
                    R.Flush();
                }

            });
        }
        public int Lens_Camera
        {
            get => GetProperty(() => Lens_Camera);
            set => SetProperty(() => Lens_Camera, value, () =>
            {
                config.Descendants("Lens_Camera").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜片相机连接：" + value.ToString() + "\n");
                    R.Flush();
                }

            });
        }
        public int DGP_Camera
        {
            get => GetProperty(() => DGP_Camera);
            set => SetProperty(() => DGP_Camera, value, () =>
            {
                config.Descendants("DGP_Camera").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改挡光片相机连接：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }


        ///镜筒参数_New
        public float JT_Para_01
        {
            get => GetProperty(() => JT_Para_01);
            set => SetProperty(() => JT_Para_01, value, () =>
            {
                config.Descendants("JT_Para_01").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_JT_New[0] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜筒配置-扫描点数：" +value.ToString()+"\n");
                    R.Flush();
                }
            });
        }
        public float JT_Para_02
        {
            get => GetProperty(() => JT_Para_02);
            set => SetProperty(() => JT_Para_02, value, () =>
            {
                config.Descendants("JT_Para_02").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_JT_New[1] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜筒配置-灰度显示：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float JT_Para_03
        {
            get => GetProperty(() => JT_Para_03);
            set => SetProperty(() => JT_Para_03, value, () =>
            {
                config.Descendants("JT_Para_03").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_JT_New[2] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜筒配置-灰度阈值：" + value.ToString() + "\n");
                    R.Flush();
                }

            });
        }
        public float JT_Para_04
        {
            get => GetProperty(() => JT_Para_04);
            set => SetProperty(() => JT_Para_04, value, () =>
            {
                config.Descendants("JT_Para_04").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_JT_New[3] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜筒配置-半径上限：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float JT_Para_05
        {
            get => GetProperty(() => JT_Para_05);
            set => SetProperty(() => JT_Para_05, value, () =>
            {
                config.Descendants("JT_Para_05").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_JT_New[4] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜筒配置-半径下限：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float JT_Para_06
        {
            get => GetProperty(() => JT_Para_06);
            set => SetProperty(() => JT_Para_06, value, () =>
            {
                config.Descendants("JT_Para_06").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_JT_New[5] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜筒配置-角度-外圆半径：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float JT_Para_07
        {
            get => GetProperty(() => JT_Para_07);
            set => SetProperty(() => JT_Para_07, value, () =>
            {
                config.Descendants("JT_Para_07").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_JT_New[6] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜筒配置-角度-内圆半径：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float JT_Para_08
        {
            get => GetProperty(() => JT_Para_08);
            set => SetProperty(() => JT_Para_08, value, () =>
            {
                config.Descendants("JT_Para_08").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_JT_New[7] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜筒配置-角度-灰度阈值：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float JT_Para_09
        {
            get => GetProperty(() => JT_Para_09);
            set => SetProperty(() => JT_Para_09, value, () =>
            {
                config.Descendants("JT_Para_09").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_JT_New[8] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜筒配置-角度-有效占比：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float JT_Para_10
        {
            get => GetProperty(() => JT_Para_10);
            set => SetProperty(() => JT_Para_10, value, () =>
            {
                config.Descendants("JT_Para_10").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_JT_New[9] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜筒配置-边缘检测上限：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float JT_Para_11
        {
            get => GetProperty(() => JT_Para_11);
            set => SetProperty(() => JT_Para_11, value, () =>
            {
                config.Descendants("JT_Para_11").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_JT_New[10] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜筒配置-边缘检测下限：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float JT_Para_12
        {
            get => GetProperty(() => JT_Para_12);
            set => SetProperty(() => JT_Para_12, value, () =>
            {
                config.Descendants("JT_Para_12").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_JT_New[11] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜筒配置-边缘检测：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float JT_Para_13
        {
            get => GetProperty(() => JT_Para_13);
            set => SetProperty(() => JT_Para_13, value, () =>
            {
                config.Descendants("JT_Para_13").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_JT_New[12] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜筒配置-匹配-外圆半径：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float JT_Para_14
        {
            get => GetProperty(() => JT_Para_14);
            set => SetProperty(() => JT_Para_14, value, () =>
            {
                config.Descendants("JT_Para_14").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_JT_New[13] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜筒配置-匹配-内圆半径：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float JT_Para_15
        {
            get => GetProperty(() => JT_Para_15);
            set => SetProperty(() => JT_Para_15, value, () =>
            {
                config.Descendants("JT_Para_15").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_JT_New[14] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜筒配置-匹配-灰度阈值：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float JT_Para_16
        {
            get => GetProperty(() => JT_Para_16);
            set => SetProperty(() => JT_Para_16, value, () =>
            {
                config.Descendants("JT_Para_16").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_JT_New[15] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜筒配置-匹配-有效占比：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float JT_Para_17
        {
            get => GetProperty(() => JT_Para_17);
            set => SetProperty(() => JT_Para_17, value, () =>
            {
                config.Descendants("JT_Para_17").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_JT_New[16] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜筒配置-匹配-角度占比：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float JT_Para_18
        {
            get => GetProperty(() => JT_Para_18);
            set => SetProperty(() => JT_Para_18, value, () =>
            {
                config.Descendants("JT_Para_18").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_JT_New[17] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜筒配置-预设X坐标：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float JT_Para_19
        {
            get => GetProperty(() => JT_Para_19);
            set => SetProperty(() => JT_Para_19, value, () =>
            {
                config.Descendants("JT_Para_19").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_JT_New[18] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜筒配置-预设Y坐标：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float JT_Para_20
        {
            get => GetProperty(() => JT_Para_20);
            set => SetProperty(() => JT_Para_20, value, () =>
            {
                config.Descendants("JT_Para_20").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_JT_New[19] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜筒配置-回正角度：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float JT_Para_21
        {
            get => GetProperty(() => JT_Para_21);
            set => SetProperty(() => JT_Para_21, value, () =>
            {
                config.Descendants("JT_Para_21").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_JT_New[20] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜筒配置-保存截图：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        //镜片参数_New
        public float Lens_Para_01
        {
            get => GetProperty(() => Lens_Para_01);
            set => SetProperty(() => Lens_Para_01, value, () =>
            {
                config.Descendants("Lens_Para_01").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_Lens_New[0] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜片配置-扫描点数：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float Lens_Para_02
        {
            get => GetProperty(() => Lens_Para_02);
            set => SetProperty(() => Lens_Para_02, value, () =>
            {
                config.Descendants("Lens_Para_02").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_Lens_New[1] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜片配置-灰度显示：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float Lens_Para_03
        {
            get => GetProperty(() => Lens_Para_03);
            set => SetProperty(() => Lens_Para_03, value, () =>
            {
                config.Descendants("Lens_Para_03").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_Lens_New[2] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜片配置-灰度阈值：" + value.ToString() + "\n");
                    R.Flush();
                }

            });
        }
        public float Lens_Para_04
        {
            get => GetProperty(() => Lens_Para_04);
            set => SetProperty(() => Lens_Para_04, value, () =>
            {
                config.Descendants("Lens_Para_04").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_Lens_New[3] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜片配置-半径上限：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float Lens_Para_05
        {
            get => GetProperty(() => Lens_Para_05);
            set => SetProperty(() => Lens_Para_05, value, () =>
            {
                config.Descendants("Lens_Para_05").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_Lens_New[4] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜片配置-半径下限：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float Lens_Para_06
        {
            get => GetProperty(() => Lens_Para_06);
            set => SetProperty(() => Lens_Para_06, value, () =>
            {
                config.Descendants("Lens_Para_06").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_Lens_New[5] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜片配置-角度-外圆半径：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float Lens_Para_07
        {
            get => GetProperty(() => Lens_Para_07);
            set => SetProperty(() => Lens_Para_07, value, () =>
            {
                config.Descendants("Lens_Para_07").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_Lens_New[6] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜片配置-角度-内圆半径：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float Lens_Para_08
        {
            get => GetProperty(() => Lens_Para_08);
            set => SetProperty(() => Lens_Para_08, value, () =>
            {
                config.Descendants("Lens_Para_08").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_Lens_New[7] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜片配置-角度-灰度阈值：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float Lens_Para_09
        {
            get => GetProperty(() => Lens_Para_09);
            set => SetProperty(() => Lens_Para_09, value, () =>
            {
                config.Descendants("Lens_Para_09").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_Lens_New[8] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜片配置-角度-有效占比：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float Lens_Para_10
        {
            get => GetProperty(() => Lens_Para_10);
            set => SetProperty(() => Lens_Para_10, value, () =>
            {
                config.Descendants("Lens_Para_10").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_Lens_New[9] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜片配置-边缘检测上限：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float Lens_Para_11
        {
            get => GetProperty(() => Lens_Para_11);
            set => SetProperty(() => Lens_Para_11, value, () =>
            {
                config.Descendants("Lens_Para_11").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_Lens_New[10] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜片配置-边缘检测下限：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float Lens_Para_12
        {
            get => GetProperty(() => Lens_Para_12);
            set => SetProperty(() => Lens_Para_12, value, () =>
            {
                config.Descendants("Lens_Para_12").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_Lens_New[11] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜片配置-边缘检测：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float Lens_Para_13
        {
            get => GetProperty(() => Lens_Para_13);
            set => SetProperty(() => Lens_Para_13, value, () =>
            {
                config.Descendants("Lens_Para_13").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_Lens_New[12] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜片配置-匹配-外援半径：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float Lens_Para_14
        {
            get => GetProperty(() => Lens_Para_14);
            set => SetProperty(() => Lens_Para_14, value, () =>
            {
                config.Descendants("Lens_Para_14").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_Lens_New[13] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜片配置-匹配-内圆半径：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float Lens_Para_15
        {
            get => GetProperty(() => Lens_Para_15);
            set => SetProperty(() => Lens_Para_15, value, () =>
            {
                config.Descendants("Lens_Para_15").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_Lens_New[14] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜片配置-匹配-灰度阈值：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float Lens_Para_16
        {
            get => GetProperty(() => Lens_Para_16);
            set => SetProperty(() => Lens_Para_16, value, () =>
            {
                config.Descendants("Lens_Para_16").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_Lens_New[15] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜片配置-匹配-有效占比：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float Lens_Para_17
        {
            get => GetProperty(() => Lens_Para_17);
            set => SetProperty(() => Lens_Para_17, value, () =>
            {
                config.Descendants("Lens_Para_17").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_Lens_New[16] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜片配置-匹配-角度占比：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float Lens_Para_18
        {
            get => GetProperty(() => Lens_Para_18);
            set => SetProperty(() => Lens_Para_18, value, () =>
            {
                config.Descendants("Lens_Para_18").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_Lens_New[17] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜片配置-预设X坐标：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float Lens_Para_19
        {
            get => GetProperty(() => Lens_Para_19);
            set => SetProperty(() => Lens_Para_19, value, () =>
            {
                config.Descendants("Lens_Para_19").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_Lens_New[18] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜片配置-预设Y坐标：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float Lens_Para_20
        {
            get => GetProperty(() => Lens_Para_20);
            set => SetProperty(() => Lens_Para_20, value, () =>
            {
                config.Descendants("Lens_Para_20").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_Lens_New[19] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜片配置-回正角度：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float Lens_Para_21
        {
            get => GetProperty(() => Lens_Para_21);
            set => SetProperty(() => Lens_Para_21, value, () =>
            {
                config.Descendants("Lens_Para_21").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_Lens_New[20] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜片配置-保存截图：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }

        //判胶参数
        public float PJ_Para_01
        {
            get => GetProperty(() => PJ_Para_01);
            set => SetProperty(() => PJ_Para_01, value, () =>
            {
                config.Descendants("PJ_Para_01").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_PJ_New[0] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改判胶配置-灰度显示：" + value.ToString() + "\n");
                    R.Flush();
                }

            });
        }
        public float PJ_Para_02
        {
            get => GetProperty(() => PJ_Para_02);
            set => SetProperty(() => PJ_Para_02, value, () =>
            {
                config.Descendants("PJ_Para_02").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_PJ_New[1] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改判胶配置-定位-B：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float PJ_Para_03
        {
            get => GetProperty(() => PJ_Para_03);
            set => SetProperty(() => PJ_Para_03, value, () =>
            {
                config.Descendants("PJ_Para_03").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_PJ_New[2] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改判胶配置-定位-G：" + value.ToString() + "\n");
                    R.Flush();
                }

            });
        }
        public float PJ_Para_04
        {
            get => GetProperty(() => PJ_Para_04);
            set => SetProperty(() => PJ_Para_04, value, () =>
            {
                config.Descendants("PJ_Para_04").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_PJ_New[3] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改判胶配置-定位-R：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float PJ_Para_05
        {
            get => GetProperty(() => PJ_Para_05);
            set => SetProperty(() => PJ_Para_05, value, () =>
            {
                config.Descendants("PJ_Para_05").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_PJ_New[4] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改判胶配置-定位-Gain：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float PJ_Para_06
        {
            get => GetProperty(() => PJ_Para_06);
            set => SetProperty(() => PJ_Para_06, value, () =>
            {
                config.Descendants("PJ_Para_06").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_PJ_New[5] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改判胶配置-定位-灰度阈值：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float PJ_Para_07
        {
            get => GetProperty(() => PJ_Para_07);
            set => SetProperty(() => PJ_Para_07, value, () =>
            {
                config.Descendants("PJ_Para_07").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_PJ_New[6] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改判胶配置-定位-外径：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float PJ_Para_08
        {
            get => GetProperty(() => PJ_Para_08);
            set => SetProperty(() => PJ_Para_08, value, () =>
            {
                config.Descendants("PJ_Para_08").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_PJ_New[7] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改判胶配置-定位-内径：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float PJ_Para_09
        {
            get => GetProperty(() => PJ_Para_09);
            set => SetProperty(() => PJ_Para_09, value, () =>
            {
                config.Descendants("PJ_Para_09").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_PJ_New[8] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改判胶配置-判别-B：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float PJ_Para_10
        {
            get => GetProperty(() => PJ_Para_10);
            set => SetProperty(() => PJ_Para_10, value, () =>
            {
                config.Descendants("PJ_Para_10").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_PJ_New[9] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改判胶配置-判别G：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float PJ_Para_11
        {
            get => GetProperty(() => PJ_Para_11);
            set => SetProperty(() => PJ_Para_11, value, () =>
            {
                config.Descendants("PJ_Para_11").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_PJ_New[10] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改判胶配置-判别-R：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float PJ_Para_12
        {
            get => GetProperty(() => PJ_Para_12);
            set => SetProperty(() => PJ_Para_12, value, () =>
            {
                config.Descendants("PJ_Para_12").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_PJ_New[11] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改判胶配置-判别-Gain：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float PJ_Para_13
        {
            get => GetProperty(() => PJ_Para_13);
            set => SetProperty(() => PJ_Para_13, value, () =>
            {
                config.Descendants("PJ_Para_13").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_PJ_New[12] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改判胶配置-判别-灰度阈值：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float PJ_Para_14
        {
            get => GetProperty(() => PJ_Para_14);
            set => SetProperty(() => PJ_Para_14, value, () =>
            {
                config.Descendants("PJ_Para_14").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_PJ_New[13] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改判胶配置-欠胶阈值：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float PJ_Para_15
        {
            get => GetProperty(() => PJ_Para_15);
            set => SetProperty(() => PJ_Para_15, value, () =>
            {
                config.Descendants("PJ_Para_15").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_PJ_New[14] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改判胶配置-胶环宽度下限：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float PJ_Para_16
        {
            get => GetProperty(() => PJ_Para_16);
            set => SetProperty(() => PJ_Para_16, value, () =>
            {
                config.Descendants("PJ_Para_16").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_PJ_New[15] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改判胶配置-外溢胶检测-外径：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float PJ_Para_17
        {
            get => GetProperty(() => PJ_Para_17);
            set => SetProperty(() => PJ_Para_17, value, () =>
            {
                config.Descendants("PJ_Para_17").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_PJ_New[16] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改判胶配置-外溢胶检测-内径：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float PJ_Para_18
        {
            get => GetProperty(() => PJ_Para_18);
            set => SetProperty(() => PJ_Para_18, value, () =>
            {
                config.Descendants("PJ_Para_18").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_PJ_New[17] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改判胶配置-欠胶检测-外径：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float PJ_Para_19
        {
            get => GetProperty(() => PJ_Para_19);
            set => SetProperty(() => PJ_Para_19, value, () =>
            {
                config.Descendants("PJ_Para_19").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_PJ_New[18] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改判胶配置-胶环检测-内径：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float PJ_Para_20
        {
            get => GetProperty(() => PJ_Para_20);
            set => SetProperty(() => PJ_Para_20, value, () =>
            {
                config.Descendants("PJ_Para_20").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_PJ_New[19] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改判胶配置-胶环检测-外径：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
    
        public float PJ_Para_21
        {
            get => GetProperty(() => PJ_Para_21);
            set => SetProperty(() => PJ_Para_21, value, () =>
            {
                config.Descendants("PJ_Para_21").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_PJ_New[20] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改判胶配置-内溢胶检测-内径：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float PJ_Para_22
        {
            get => GetProperty(() => PJ_Para_22);
            set => SetProperty(() => PJ_Para_22, value, () =>
            {
                config.Descendants("PJ_Para_22").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_PJ_New[21] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改判胶配置-内溢胶检测-外径：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float PJ_Para_23
        {
            get => GetProperty(() => PJ_Para_23);
            set => SetProperty(() => PJ_Para_23, value, () =>
            {
                config.Descendants("PJ_Para_23").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_PJ_New[22] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改判胶配置-胶水段数：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float PJ_Para_24
        {
            get => GetProperty(() => PJ_Para_24);
            set => SetProperty(() => PJ_Para_24, value, () =>
            {
                config.Descendants("PJ_Para_24").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_PJ_New[23] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改判胶配置-胶水长度：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float PJ_Para_25
        {
            get => GetProperty(() => PJ_Para_25);
            set => SetProperty(() => PJ_Para_25, value, () =>
            {
                config.Descendants("PJ_Para_25").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_PJ_New[24] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改判胶配置-显示编号：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float PJ_Para_26
        {
            get => GetProperty(() => PJ_Para_26);
            set => SetProperty(() => PJ_Para_26, value, () =>
            {
                config.Descendants("PJ_Para_26").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_PJ_New[25] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改判胶配置-左上角X：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float PJ_Para_27
        {
            get => GetProperty(() => PJ_Para_27);
            set => SetProperty(() => PJ_Para_27, value, () =>
            {
                config.Descendants("PJ_Para_27").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_PJ_New[26] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改判胶配置-左上角Y：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float PJ_Para_28
        {
            get => GetProperty(() => PJ_Para_28);
            set => SetProperty(() => PJ_Para_28, value, () =>
            {
                config.Descendants("PJ_Para_28").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_PJ_New[27] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改判胶配置-右下角X：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float PJ_Para_29
        {
            get => GetProperty(() => PJ_Para_29);
            set => SetProperty(() => PJ_Para_29, value, () =>
            {
                config.Descendants("PJ_Para_29").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_PJ_New[28] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改判胶配置-右下角Y：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float PJ_Para_30
        {
            get => GetProperty(() => PJ_Para_30);
            set => SetProperty(() => PJ_Para_30, value, () =>
            {
                config.Descendants("PJ_Para_30").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_PJ_New[29] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改判胶配置-是否截图：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
     
        //比例尺
        public float Rule_DJ_X
        {
            get => GetProperty(() => Rule_DJ_X);
            set => SetProperty(() => Rule_DJ_X, value, () =>
            {
                config.Descendants("Rule_DJ_x").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_DJ_New[21] = value;
            });
        }
        public float Rule_DJ_Y
        {
            get => GetProperty(() => Rule_DJ_Y);
            set => SetProperty(() => Rule_DJ_Y, value, () =>
            {
                config.Descendants("Rule_DJ_y").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_DJ_New[22] = value;
            });
        }
        public float Rule_JT_X
        {
            get => GetProperty(() => Rule_JT_X);
            set => SetProperty(() => Rule_JT_X, value, () =>
            {
                config.Descendants("Rule_JT_x").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_JT_New[21] = value;
            });
        }
        public float Rule_JT_Y
        {
            get => GetProperty(() => Rule_JT_Y);
            set => SetProperty(() => Rule_JT_Y, value, () =>
            {
                config.Descendants("Rule_JT_y").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_JT_New[22] = value;
            });
        }
        public float Rule_Lens_X
        {
            get => GetProperty(() => Rule_Lens_X);
            set => SetProperty(() => Rule_Lens_X, value, () =>
            {
                config.Descendants("Rule_Lens_x").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_Lens_New[21] = value;
            });
        }
        public float Rule_Lens_Y
        {
            get => GetProperty(() => Rule_Lens_Y);
            set => SetProperty(() => Rule_Lens_Y, value, () =>
            {
                config.Descendants("Rule_Lens_y").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_Lens_New[22] = value;
            });
        }

        //光源触发
        public int JT_Light_Flag
        {
            get => GetProperty(() => JT_Light_Flag);
            set => SetProperty(() => JT_Light_Flag, value, () =>
            {
                config.Descendants("JT_Light_Flag").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
            });
        }
        public int Lens_Light_Flag
        {
            get => GetProperty(() => Lens_Light_Flag);
            set => SetProperty(() => Lens_Light_Flag, value, () =>
            {
                config.Descendants("Lens_Light_Flag").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
            });
        }
        public int DGP_Light_Flag
        {
            get => GetProperty(() => DGP_Light_Flag);
            set => SetProperty(() => DGP_Light_Flag, value, () =>
            {
                config.Descendants("DGP_Light_Flag").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
            });
        }
        public float JT_Light_Brightness
        {
            get => GetProperty(() => JT_Light_Brightness);
            set => SetProperty(() => JT_Light_Brightness, value, () =>
            {
                config.Descendants("JT_Light_Brightness").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
            });
        }
        public float Lens_Light_Brightness
        {
            get => GetProperty(() => Lens_Light_Brightness);
            set => SetProperty(() => Lens_Light_Brightness, value, () =>
            {
                config.Descendants("Lens_Light_Brightness").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
            });
        }
        public float DGP_Light_Brightness
        {
            get => GetProperty(() => DGP_Light_Brightness);
            set => SetProperty(() => DGP_Light_Brightness, value, () =>
            {
                config.Descendants("DGP_Light_Brightness").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
            });
        }
        public float JT_Light_Time
        {
            get => GetProperty(() => JT_Light_Time);
            set => SetProperty(() => JT_Light_Time, value, () =>
            {
                config.Descendants("JT_Light_Time").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
            });
        }
        public float Lens_Light_Time
        {
            get => GetProperty(() => Lens_Light_Time);
            set => SetProperty(() => Lens_Light_Time, value, () =>
            {
                config.Descendants("Lens_Light_Time").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
            });
        }
        public float DGP_Light_Time
        {
            get => GetProperty(() => DGP_Light_Time);
            set => SetProperty(() => DGP_Light_Time, value, () =>
            {
                config.Descendants("DGP_Light_Time").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
            });
        }
        public string Light_TCP_IP
        {
            get => GetProperty(() => Light_TCP_IP);
            set => SetProperty(() => Light_TCP_IP, value, () =>
            {
                config.Descendants("Light_TCP_IP").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
            });
        }
        public int Light_TCP_Port
        {
            get => GetProperty(() => Light_TCP_Port);
            set => SetProperty(() => Light_TCP_Port, value, () =>
            {
                config.Descendants("Light_TCP_Port").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
            });
        }
        //配置编号
        public string JT_Config
        {
            get => GetProperty(() => JT_Config);
            set => SetProperty(() => JT_Config, value, () =>
            {
                config.Descendants("JT_Config").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜筒配置-模号名称：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public string JP_Config
        {
            get => GetProperty(() => JP_Config);
            set => SetProperty(() => JP_Config, value, () =>
            {
                config.Descendants("JP_Config").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改镜片配置-模号名称：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public string DGP_Config
        {
            get => GetProperty(() => DGP_Config);
            set => SetProperty(() => DGP_Config, value, () =>
            {
                config.Descendants("DGP_Config").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改挡光片配置-模号名称：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public string PJ_Config
        {
            get => GetProperty(() => PJ_Config);
            set => SetProperty(() => PJ_Config, value, () =>
            {
                config.Descendants("PJ_Config").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改判胶配置-模号名称：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        //托盘拍照参数
        public float TP_Para_01
        {
            get => GetProperty(() => TP_Para_01);
            set => SetProperty(() => TP_Para_01, value, () =>
            {
                config.Descendants("TP_Para_01").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_TP_New[0] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改托盘参数-左上角x：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float TP_Para_02
        {
            get => GetProperty(() => TP_Para_02);
            set => SetProperty(() => TP_Para_02, value, () =>
            {
                config.Descendants("TP_Para_02").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_TP_New[1] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改托盘参数-左上角Y：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float TP_Para_03
        {
            get => GetProperty(() => TP_Para_03);
            set => SetProperty(() => TP_Para_03, value, () =>
            {
                config.Descendants("TP_Para_03").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_TP_New[2] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改托盘参数-右下角x：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float TP_Para_04
        {
            get => GetProperty(() => TP_Para_04);
            set => SetProperty(() => TP_Para_04, value, () =>
            {
                config.Descendants("TP_Para_04").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_TP_New[3] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改托盘参数-右下角Y：" + value.ToString() + "\n");
                    R.Flush();
                }
            });

        }
        public float TP_Para_05
        {
            get => GetProperty(() => TP_Para_05);
            set => SetProperty(() => TP_Para_05, value, () =>
            {
                config.Descendants("TP_Para_05").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_TP_New[4] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改托盘参数-画面旋转：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float TP_Para_06
        {
            get => GetProperty(() => TP_Para_06);
            set => SetProperty(() => TP_Para_06, value, () =>
            {
                config.Descendants("TP_Para_06").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_TP_New[5] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改托盘参数-灰度阈值：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float TP_Para_07
        {
            get => GetProperty(() => TP_Para_07);
            set => SetProperty(() => TP_Para_07, value, () =>
            {
                config.Descendants("TP_Para_07").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_TP_New[6] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改托盘参数-半径上限：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float TP_Para_08
        {
            get => GetProperty(() => TP_Para_08);
            set => SetProperty(() => TP_Para_08, value, () =>
            {
                config.Descendants("TP_Para_08").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_TP_New[7] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改托盘参数-半径下限：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float TP_Para_09
        {
            get => GetProperty(() => TP_Para_09);
            set => SetProperty(() => TP_Para_09, value, () =>
            {
                config.Descendants("TP_Para_09").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_TP_New[8] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改托盘参数-灰度显示：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
        public float TP_Para_10
        {
            get => GetProperty(() => TP_Para_10);
            set => SetProperty(() => TP_Para_10, value, () =>
            {
                config.Descendants("TP_Para_10").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                _input_Parameter_TP_New[9] = value;
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改托盘参数-是否截图：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }

        /// <summary>
        /// 托盘相机参数
        /// </summary>
        public float TP_CameraPara_ExposureTime
        {
            get => GetProperty(() => TP_CameraPara_ExposureTime);
            set => SetProperty(() => TP_CameraPara_ExposureTime, value, () =>
            {
                config.Descendants("TP_CameraPara_ExposureTime").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改托盘相机参数-曝光：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }

        public float TP_CameraPara_AcquisitionFrameRate
        {
            get => GetProperty(() => TP_CameraPara_AcquisitionFrameRate);
            set => SetProperty(() => TP_CameraPara_AcquisitionFrameRate, value, () =>
            {
                config.Descendants("TP_CameraPara_AcquisitionFrameRate").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改托盘相机参数-帧数：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }

        public float TP_CameraPara_Gain
        {
            get => GetProperty(() => TP_CameraPara_Gain);
            set => SetProperty(() => TP_CameraPara_Gain, value, () =>
            {
                config.Descendants("TP_CameraPara_Gain").ElementAt(0).SetValue(value);
                config.Save(_Configfile);
                if (_Mod_Flag == true)
                {
                    StreamWriter R = new StreamWriter(Para_Log);
                    R.Write(CmdTag + " 修改托盘相机参数-增益：" + value.ToString() + "\n");
                    R.Flush();
                }
            });
        }
    }
}
