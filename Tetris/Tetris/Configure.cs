using System;
using System.Drawing;
using System.Windows.Forms;
using System.Xml;
using System.IO;
using System.Reflection;

namespace Tetris
{
    /// <summary>
    /// 整体的参数配置，包括使用xml文件
    /// </summary>
    class Configure
    {
        private Keys _downKey;
        private Keys _dropKey;
        private Keys _moveLeftKey;
        private Keys _moveRightKey;
        private Keys _deasilRotateKey;
        private Keys _contraRotateKey;
        private int _coorWidth;
        private int _coorHigh;
        private int _rectPix;
        private Color _backColor;
        private ArrInfo info = new ArrInfo();

        #region 私有变量对应的属性
        public Keys DownKey
        {
            get { return _downKey; }
            set { _downKey = value; }
        }
        public Keys DropKey
        {
            get { return _dropKey; }
            set { _dropKey = value; }
        }
        public Keys MoveLeftKey
        {
            get { return _moveLeftKey; }
            set { _moveLeftKey = value; }
        }
        public Keys MoveRightKey
        {
            get { return _moveRightKey; }
            set { _moveRightKey = value; }
        }
        public Keys DeasilRotateKey
        {
            get { return _deasilRotateKey; }
            set { _deasilRotateKey = value; }
        }
        public Keys ContraRotateKey
        {
            get { return _contraRotateKey; }
            set { _contraRotateKey = value; }
        }
        public int CoorWidth
        {
            get { return _coorWidth; }
            set {
                if (value>=10&&value<=30)
                {
                    _coorWidth = value;
                } 
            }
        }
        public int CoorHigh
        {
            get { return _coorHigh; }
            set {
                if (value >= 20 && value <= 50) 
                    _coorHigh = value;
            }
        }
        public int RectPix
        {
            get { return _rectPix; }
            set {
                if (value >= 10 && value <= 30) 
                    _rectPix = value;
            }
        }
        public Color BackColor
        {
            get { return _backColor; }
            set { _backColor = value; }
        }
        public ArrInfo Info
        {
            get { return info; }
            set { info = value; }
        }
        #endregion
        public void LoadFromXmlFile()
        {
            XmlTextReader reader;
            if (File.Exists("BlockSet.xml"))
            {
                reader = new XmlTextReader("BlockSet.xml");
            }
            else
            {
                Assembly asm = Assembly.GetExecutingAssembly();
                Stream sm = asm.GetManifestResourceStream("Tetris.BlockSet.xml");
                reader = new XmlTextReader(sm);
            }
            string key = "";
            try
            {
                while (reader.Read())
                {
                    if (reader.NodeType == XmlNodeType.Element)
                    {
                        if (reader.Name == "ID")
                        {
                            key = reader.ReadElementString().Trim();
                            info.Add(key, "");
                        }
                        else if (reader.Name == "Color")
                        {
                            info[key] = reader.ReadElementString().Trim();
                        }
                        else if (reader.Name == "DownKey")
                        {
                            _downKey = (Keys)Convert.ToInt32(reader.ReadElementString().Trim());
                        }
                        else if (reader.Name == "DropKey")
                        {
                            _dropKey = (Keys)Convert.ToInt32(reader.ReadElementString().Trim());
                        }
                        else if (reader.Name == "MoveLeftKey")
                        {
                            _moveLeftKey = (Keys)Convert.ToInt32(reader.ReadElementString().Trim());
                        }
                        else if (reader.Name == "MoveRightKey")
                        {
                            _moveRightKey = (Keys)Convert.ToInt32(reader.ReadElementString().Trim());
                        }
                        else if (reader.Name == "DeasilRotateKey")
                        {
                            _deasilRotateKey = (Keys)Convert.ToInt32(reader.ReadElementString().Trim());
                        }
                        else if (reader.Name == "ContraRotateKey")
                        {
                            _contraRotateKey = (Keys)Convert.ToInt32(reader.ReadElementString().Trim());
                        }
                        else if (reader.Name == "CoorWidth")
                        {
                            _coorWidth = Convert.ToInt32(reader.ReadElementString().Trim());
                        }
                        else if (reader.Name == "CoorHigh")
                        {
                            _coorHigh = Convert.ToInt32(reader.ReadElementString().Trim());
                        }
                        else if (reader.Name == "RectPix")
                        {
                            _rectPix = Convert.ToInt32(reader.ReadElementString().Trim());
                        }
                        else if (reader.Name == "BackColor")
                        {
                            _backColor = Color.FromArgb(Convert.ToInt32(reader.ReadElementString().Trim()));
                        }
                    }
                }
            }
            catch(Exception ex)
            {
                MessageBox.Show(ex.ToString());
            }
            finally
            {
                if (reader!=null)
                {
                    reader.Close();
                }
            }
        }
        public void SaveToXmlFile()
        {
            XmlDocument doc = new XmlDocument();
            doc.LoadXml("<BlockSet></BlockSet>");
            XmlNode root = doc.SelectSingleNode("BlockSet");
            for (int i = 0; i < info.Legth; i++)
            {
                XmlElement xelType = doc.CreateElement("Type");
                XmlElement xelID = doc.CreateElement("ID");
                xelID.InnerText = ((BlockInfo)info[i]).GetIdStr();
                xelType.AppendChild(xelID);
                XmlElement xelColor= doc.CreateElement("Color");
                xelColor.InnerText = ((BlockInfo)info[i]).GetColorStr();
                xelType.AppendChild(xelColor);
                root.AppendChild(xelType);
            }
            //需要写入其它的信息
            doc.Save("BlockSet.xml");
        }
    }
}
