using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Tetris
{
    /// <summary>
    /// 砖块类的配置
    /// 使用了属性和索引器
    /// </summary>
    class ArrInfo
    {
        private ArrayList info = new ArrayList();
        private int _length = 0;
        public int Legth
        {
            //没有set表示只读，不可写
            get
            {
                return _length;
            }
        }
        public BlockInfo this[int index]
        {
            get
            {
                return (BlockInfo)info[index];
            }
        }
        public string this[string id]
        {
            set
            {
                if (value=="")
                {
                    return;
                }
                for (int i = 0; i < info.Count; i++)
                {
                    if (((BlockInfo)info[i]).GetIdStr()==id)
                    {
                        try
                        {
                            ((BlockInfo)info[i]).BColor=Color.FromArgb(Convert.ToInt32(value));
                        }
                        catch(System.FormatException)
                        {
                            MessageBox.Show("颜色信息错误");
                        }
                    }
                }
            }
        }
        public BitArray StrToBit(string id)
        {
            if (id.Length!=25)
            {
                throw new System.FormatException("砖块格式不合法");
            }
            BitArray bArr = new BitArray(25);
            for (int i = 0; i < 25; i++)
            {
                bArr[i]=(id[i]=='1')?true:false;
            }
            return bArr;
        }
        public void Add(BitArray id,Color bColor)
        {
            if (id.Length != 25)
            {
                throw new System.FormatException("砖块格式不合法");
            }
            //这个Add方法是动态数组的Add方法，（）中的是object对象
            info.Add(new BlockInfo(id,bColor));
            _length++;
        }
        public void Add(string id,string bColor)
        {
            Color temp;
            if (bColor != "")
            {
                temp = Color.FromArgb(Convert.ToInt32(bColor));
            }
            else
            {
                temp = Color.Empty;

            }
            info.Add(new BlockInfo(StrToBit(id),temp));
            _length++;
        }
    }
}
