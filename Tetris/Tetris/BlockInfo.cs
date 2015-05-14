using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Tetris
{
    /// <summary>
    /// 建立BlockInfo类，完成单个块样式的各种操作
    /// 私有成员_id、_bColor；对应属性ID、BColor
    /// 方法GetIdStr、GetColorStr
    /// </summary>
    class BlockInfo
    {
        private BitArray _id;
        private Color _bColor;
        public BlockInfo(BitArray id,Color color)
        {
            _id = id;
            _bColor = color;
        }
//#regin shuxing
        public BitArray ID
        {
            set 
            {
                _id = value;
            }
            get
            {
                return _id;
            }
        }
        public Color BColor
        {
            set
            {
                _bColor = value;
            }
            get
            {
                return _bColor;
            }
        }
        public string GetIdStr()
        {
            StringBuilder strBuilder = new StringBuilder(25);
            foreach (bool item in _id)
            {
                strBuilder.Append((item==true)?"1":"0");
            }
            return strBuilder.ToString();
        }
        public string GetColorStr()
        {
            return Convert.ToString(_bColor.ToArgb());
        }
    }
}
